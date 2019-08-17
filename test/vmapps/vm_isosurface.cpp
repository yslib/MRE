
#include "vm_isosurface.h"

#include <rendercontext.h>
#include <abstraarray.h>
#include <drawelements.h>
#include <trivialscenemanager.h>
#include <rawreader.h>

namespace ysl
{
	namespace vm
	{
		void VM_ISOSurface::InitEvent()
		{
			SetupPrimitive();
			auto artist = MakeRef<Artist>();
			artist->GetLOD(0)->push_back(MakePhongShading());
			auto scale = MakeRef<Transform>();
			scale->SetScale(Vec3f{ 0.1,0.1,0.1 });
			auto actor = MakeRef<Actor>(surface, artist, scale);

			auto sceneManager = MakeRef<TrivialSceneManager>();
			sceneManager->AddActor(actor);

			SetAggregation(MakeRef<Aggregate>());
			std::static_pointer_cast<Aggregate>(GetAggregate())->SceneManager().push_back(sceneManager);
			std::static_pointer_cast<Aggregate>(GetAggregate())->Renderers()[0]->SetFramebuffer(Context()->GetFramebuffer());
			std::static_pointer_cast<Aggregate>(GetAggregate())->CreateGetCamera()->GetViewport()->SetClearFlag(CF_CLEAR_COLOR_DEPTH);
			std::static_pointer_cast<Aggregate>(GetAggregate())->CreateGetCamera()->GetViewport()->SetClearColor(Vec4f{ 0,0,0,1 });

			manipulator->SetCamera(std::static_pointer_cast<Aggregate>(GetAggregate())->CreateGetCamera());

			// Open a Test File 

			RawReader reader(R"(D:\scidata\head.raw)", Size3{ 256,256,225 }, 1);
			buffer.resize(256 * 256 * 225ULL);
			reader.readRegion(Size3{ 0,0,0 }, Size3{ 256,256,225 }, buffer.data());
			meshGen = MakeRef<MeshGenerator>(buffer.data(), Size3{ 256,256,225 });
		}
		void VM_ISOSurface::UpdateScene()
		{
			UpdatePrimitive();
		}
		void VM_ISOSurface::SetNewData(const unsigned char* data, const Size3& size)
		{
			meshGen = MakeRef<MeshGenerator>(data, size);
			isoValue = 0;
			Context()->Update();
		}

		void VM_ISOSurface::KeyPressEvent(KeyButton key)
		{
			if (key == Key_Up)
			{
				isoValue = Clamp(++isoValue, 0, 255);
				changed = true;
			}

			if (key == Key_Down)
			{
				isoValue = Clamp(--isoValue, 0, 255);
				changed = true;
			}
		}

		void VM_ISOSurface::SetupPrimitive()
		{
			surface = MakeRef<Primitive>();
			const auto vertexPos = MakeRef<ArrayFloat3>();
			const auto vertexNormal = MakeRef<ArrayFloat3>();
			const auto vertexIndex = MakeRef<ArrayUInt>();
			surface->SetVertexPositionArray(vertexPos);
			surface->SetVertexNormalArray(vertexNormal);

			auto drawElemCall = MakeRef<DrawElementsUInt>();
			drawElemCall->SetIndexBuffer(vertexIndex);
			surface->DrawCalls().push_back(drawElemCall);

		}

		void VM_ISOSurface::UpdatePrimitive()
		{
			if (!changed)return;
			if (!meshGen)
				return;

			assert(isoValue >= 0 && isoValue < 256);

			// time-consuming operation
			const auto mesh = meshGen->GenerateMesh(isoValue);
			const auto nVertex = mesh->VertexCount();

			surface->GetVertexArray()->GetBufferObject()->ReallocBufferData(sizeof(Point3f) * nVertex, mesh->Vertices(), BU_STATIC_DRAW);
			surface->GetVertexArray()->SetbufferObjectDataDirty(false);
			surface->GetNormalArray()->GetBufferObject()->ReallocBufferData(sizeof(Point3f) * nVertex, mesh->Normals(), BU_STATIC_DRAW);
			surface->GetNormalArray()->SetbufferObjectDataDirty(false);
			assert(surface->DrawCalls().size());

			auto de = std::static_pointer_cast<DrawElementsUInt>(surface->DrawCalls()[0]);
			de->GetIndexArray()->GetBufferObject()->ReallocBufferData(sizeof(unsigned int) * nVertex, mesh->Indices(), BU_STATIC_DRAW);
			de->GetIndexArray()->SetbufferObjectDataDirty(false);
			changed = false;
		}
	}
}
