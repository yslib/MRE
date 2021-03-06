
#ifndef _LARGEVOLUMERAYCAST_H_
#define _LARGEVOLUMERAYCAST_H_

#include <visualman.h>
#include <VMUtils/json_binding.hpp>
#include <vector>
#include <string>
#include "config.h"
#include "oocprimitive.h"
#include "ooctexture.h"

namespace vm
{
struct LVDJSONStruct : ::vm::json::Serializable<LVDJSONStruct>
{
	VM_JSON_FIELD( std::vector<std::string>, fileNames );
	VM_JSON_FIELD( float, samplingRate );
	VM_JSON_FIELD( std::vector<float>, spacing );
};

class Primitive;
class ArrayFloat3;
class FrustumEventCallback : public IActorEvent
{
public:
	FrustumEventCallback( VMRef<Camera> camera );
	void BindFrustumActor( VMRef<Actor> actor );
	void OnActorRenderStartedEvent( Actor *actor, const Camera *camera, Renderable *renderable, const Shading *shading, int pass ) override;
	VMRef<Primitive> GetFrustumPrimitive();

private:
	void UpdateFrustum();
	VMRef<Primitive> frustum;
	VMRef<Camera> camera;
	VMRef<ArrayFloat3> vertexArray;
};

class BoundingBoxEventCallback : public IActorEvent
{
public:
	BoundingBoxEventCallback();
	void BindBoundingBoxActor( VMRef<Actor> actor );
	void OnActorRenderStartedEvent( Actor *actor, const Camera *camera, Renderable *renderable, const Shading *shading, int pass ) override;

private:
	VMRef<Primitive> boundingBox;
};






class VMAPPS_EXPORT_IMPORT VM_LargeVolumeRayCast : public VisualMan
{
public:
	VM_LargeVolumeRayCast() = default;
	VM_LargeVolumeRayCast( bool offline, const std::string &outFileName, const std::string &jsonFileName, const std::string &tfFilename );
	void InitEvent() override;
	void UpdateScene() override;
	void DestroyEvent() override;
	void MouseWheelEvent( int ydegree, int xdegree ) override;
	void FileDropEvent( const std::vector<std::string> &fileNames ) override;
	void ResizeEvent( int w, int h ) override;
	void KeyPressEvent( KeyButton key ) override;
	void MouseMoveEvent( MouseButton button, int xpos, int ypos ) override;
	void UpdateEvent() override;


private:
	void PrintInfo();
	void SetupShading();
	void SetupResources( const std::string &fileName );
	void SetupTF( const std::string &fileName );
	void SetupCamera( const std::string &fileName );
	void SetupConfigurationFiles( const std::vector<std::string> &fileNames );

	//bool offlineRendering = false;

	std::string outFileName;
	std::string jsonFile;
	std::string tfFunctionFile;
	std::string lodsFileName;
	Vec3i dataResolution;


	Timer timer;
	LVDJSONStruct lvdJSON;
	VMRef<Shading> rayCastShading;
	VMRef<Aggregate> mrtAgt, raycastAgt, navigationAgt;
	VMRef<OutOfCorePrimitive> oocPrimitive;
	VMRef<Texture> intermediateResult;
	VMRef<Texture> randomOffsets;
	Vec2i vSize;
	VMRef<Transform> proxyGemoryScaleTrans;
	VMRef<Transform> navigationScale;
	VMRef<ViewMatrixWrapper> navigationCameraViewMatrix;
	VMRef<OutOfCoreVolumeTexture> oocTexture;
	VMRef<Actor> boundingboxActor;
};
}  // namespace vm

#endif
