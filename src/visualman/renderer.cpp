
#include "renderer.h"
#include <iostream>
#include "graphictype.h"
#include "rendercontext.h"
#include "renderable.h"
#include "transformchangecallback.h"

namespace vm
{
Renderer::Renderer()
{
	transformCallback = MakeVMRef<TransformChangeCallback>();
}

void Renderer::Render( const RenderQueue &renderQueue,
					   const VMRef<Camera> &camera )
{
	// active framebuffer and get context from fb

	// set render state for the fb

	// dispatch before-render events

	// render the queue

	/// for every token and for each lod
	/// set shader state and enable for the token
	/// dispatch actor before-render event
	/// set glsl , cameras, uniforms dependent on current state
	/// if the projection matrix has been changed, dispatch corresponding events
	/// finally render the renderable object

	// clear enables
	// clear render states

	// dispatch after-render events;

	class Raii
	{
		Renderer *renderer;
		std::vector<RenderState> oldRenderStates;

	public:
		Raii( Renderer *renderer, Camera *camera ) :
		  renderer( renderer )
		{
			assert( renderer->GetFramebuffer() );
			renderer->GetFramebuffer()->Activate( FBB_FRAMEBUFFER );
			assert( camera );
			camera->GetViewport()->Activate();	// Clear
			renderer->DispatchOnRenderStartedEvent();
			GL_CHECK
		}
		~Raii()
		{
			renderer->DispatchOnRenderFinishedEvent();
			GL_CHECK
		}
	} raii( this, camera.get() );

	// Apply Render state
	assert( framebuffer );

	RenderContext *context = framebuffer->Context();

	for ( auto i = 0; i < renderQueue.Size(); i++ ) {
		auto node = renderQueue[ i ];

		for ( auto pass = 0; node != nullptr; node = node->nextPass, pass++ ) {
			assert( node->shading );

			// Set Render State
			// program also be used here.

			//auto program = node->shading->GetProgram();
			auto renderStateSet = node->shading->GetRenderStateSet();
			context->ApplyRenderState( renderStateSet.get() );
			GL_CHECK;

			// Set Enable State
			auto enableStateSet = node->shading->GetEnableStateSet();
			context->ApplyRenderEnable( enableStateSet.get() );
			GL_CHECK;

			// Set Program and Uniforms

			// Actor's Uniforms can also be set by user before rendering
			node->actor->DispatchOnActorRenderStartedEvent( camera.get(), node->renderable, node->shading, pass );

			auto program = node->shading->GetProgram();

			assert( node->actor );

			if ( program ) {
				//context->UseProgram(program.get());
				auto actorUniformSet = node->actor->GetUniformSet();
				auto programUniformSet = program->GetUniformSet();
				auto shadingUniformSet = node->shading->GetUniformSet();
				///TODO:: the uniforms of the three sets must not be overlapped. A check should be applied here
				// Apply transform uniforms
				transformCallback->UpdateTransform( program.get(), camera.get(), node->actor->GetTransform().get() );
				// Apply Uniforms
				program->ApplyUniformSet( actorUniformSet );
				program->ApplyUniformSet( programUniformSet );
				program->ApplyUniformSet( shadingUniformSet );
			}
			assert( node->renderable );
			node->renderable->Render( node->actor, node->shading, camera.get(), context );
		}
	}
}
}  // namespace vm
