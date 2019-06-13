
#ifndef _OOCACTORCALLBACK_H_
#define _OOCACTORCALLBACK_H_

#include "config.h"
#include <actor.h>
#include "oocprimitive.h"


namespace ysl
{
	namespace vm
	{

		class ArrayFloat3;
		class OutOfCorePrimitive;

		class VMAPPS_EXPORT_IMPORT OOCActorEvent :public IActorEvent
		{
		public:
			OOCActorEvent();
			void OnActorRenderStartedEvent(Actor* actor, const Camera* camera, Renderable* renderable, const Shading* shading, int pass) override;
			void BindToActor(Ref<Actor> actor);
			Ref<OutOfCorePrimitive> ProxyGeometry() { return proxyGeometry; }
		private:
			Ref<OutOfCorePrimitive> proxyGeometry;
			Ref<ArrayFloat3> vertexArray;
			Ref<ArrayFloat3> texCoordArray;

		};

	}
}
#endif
