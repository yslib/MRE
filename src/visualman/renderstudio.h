
#ifndef _AGGREGATE_H_
#define _AGGREGATE_H_
#include <vector>
#include <memory>
#include "abstrascenemanager.h"
#include "camera.h"
#include "graphictype.h"
#include "abstrarenderstudio.h"
#include "renderer.h"

namespace ysl
{
namespace vm
{
class VISUALMAN_EXPORT_IMPORT Aggregate : public AbstraAggregate
{
public:
	Aggregate()
	{
		//camera = MakeRef<Camera>();  // A default camera
		renderers.push_back( MakeRef<Renderer>() );
	}
	void SetCamera( const Ref<Camera> &camera ) { this->camera = camera; }
	Ref<Camera> CreateGetCamera() { return camera ? camera : ( camera = MakeRef<Camera>() ); }

	void Render() override;

	std::vector<Ref<AbstraSceneManager>> &SceneManager() { return sceneManagers; }
	const std::vector<Ref<AbstraSceneManager>> &SceneManager() const { return sceneManagers; }

	std::vector<Ref<Renderer>> &Renderers() { return renderers; }
	const std::vector<Ref<Renderer>> &Renderers() const { return renderers; }

protected:
	std::vector<Ref<AbstraSceneManager>> sceneManagers;
	std::vector<Ref<Renderer>> renderers;
	std::vector<Ref<Actor>> actorQueue;	 // extract from scene manager
	Ref<Camera> camera;

private:
	RenderQueue MakeRenderQueue( const std::vector<Ref<Actor>> &queue );
};

/**
		 * \brief This class is used to run aggregates in a serialized order
		 */
class VISUALMAN_EXPORT_IMPORT SerializedAggregates : public AbstraAggregate	 // Decorator Pattern
{
public:
	void Render() override;
	std::vector<Ref<AbstraAggregate>> &GetAggregates() { return aggregates; }
	const std::vector<Ref<AbstraAggregate>> &GetAggregates() const { return aggregates; }

private:
	std::vector<Ref<AbstraAggregate>> aggregates;
};
}  // namespace vm
}  // namespace ysl

#endif