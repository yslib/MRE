
#include "renderable.h"
#include <iostream>

namespace ysl
{
	namespace vpl
	{
		void Renderable::Render(const Actor* actor, const Shading * shading,  const Camera * camera, RenderContext * context)
		{
			Render_Implement(actor,shading,camera,context);
		}
	}
}