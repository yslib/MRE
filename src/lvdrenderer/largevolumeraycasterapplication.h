#ifndef _LARGEVOLUMERAYCASTERAPPLICATION_H_
#define _LARGEVOLUMERAYCASTERAPPLICATION_H_

#include "guiapplication.h"
#include "openglbuffer.h"
#include "openglvertexarrayobject.h"
#include "shader.h"
#include "transferfunctionwidget.h"
#include "lodaggregate.h"
#include "framebuffer.h"

#include <camera.h>
#include <tfgenerator.h>
#include <memory>

//#define COUNT_VALID_BLOCK


namespace ysl
{
	namespace app
	{

		constexpr int pageTableBlockEntry = 16;
		class LargeVolumeRayCaster:public ImGuiApplication

		{
		public:
			LargeVolumeRayCaster(int argc, char ** argv, int w, int h,const std::string & fileName);
		protected:
			void MousePressEvent(MouseEvent* event) override;
			void MouseMoveEvent(MouseEvent* event) override;
			void MouseReleaseEvent(MouseEvent* e) override;
			void MouseWheelEvent(WheelEvent* e) override;
			void WindowResizeEvent(ResizeEvent* event) override;
			void InitializeOpenGLResources() override;
			void DrawImGui() override;
			void RenderLoop() override;
		private:
			void OpenGLConfiguration();
			//void InitGPUPageTableBuffer();
			//void InitGPUBlockCacheTexture();		// texCache
			void InitializeProxyGeometry();
			void InitializeResource();
			void InitTransferFunctionTexture();
			void InitRayCastingTexture();
			void InitializeShaders();
			void SetShaderUniforms();
			bool CaptureAndHandleCacheFault();
			int CalcLOD()const;

			FocusCamera camera;
			Transform projMatrix;
			Transform orthoMatrix;
			Transform modelMatrix;
			ShaderProgram rayCastingShaderProgram;
			ShaderProgram positionShaderProgram;
			ShaderProgram quadsShaderProgram;
			ShaderProgram clearIntermediateProgram;
			//ShaderProgram testShader;
			Point2i lastMousePos;
			TransferFunction tfObject;
			std::vector<ysl::RGBASpectrum> tfData;
			Vector3f lightDirection;

			float step;
			float ka;
			float ks;
			float kd;
			float shininess;


			int totalTracedBlocks;

			//static constexpr std::size_t missedBlockCapacity = 5000 * sizeof(unsigned int);

			std::shared_ptr<OpenGLTexture> texTransferFunction;
			std::shared_ptr<OpenGLFramebufferObject> framebuffer;
			std::shared_ptr<OpenGLTexture> texEntryPos;
			std::shared_ptr<OpenGLTexture> texExitPos;
			std::shared_ptr<OpenGLTexture> texDepth;
			std::shared_ptr<OpenGLTexture> texIntermediateResult;

			std::shared_ptr<OpenGLBuffer> proxyEBO;
			std::shared_ptr<OpenGLBuffer> proxyVBO;
			OpenGLVertexArrayObject proxyVAO;
			std::shared_ptr<OpenGLBuffer> rayCastingVBO;
			OpenGLVertexArrayObject rayCastingVAO;


			SHADERBINDINGPOINT sbp;

			// One LOD Data

			//ysl::Size3 gpuCacheBlockSize;				
			//std::shared_ptr<HashBasedGPUCacheFaultHandler> cacheFaultHandler; // Belong to Client-end memory
			//std::shared_ptr<PingPongTransferManager> pingpongTransferManager;
			//std::shared_ptr<GPUVolumeDataCache> texCache;			 // Client-end memory
			//std::shared_ptr<CPUVolumeDataCache> largeVolumeCache;
			//std::shared_ptr<PageTableManager> pageTableManager;		// Server-end memory

			std::vector<std::shared_ptr<LODAggregate>> aggregates;
			int currentLod;
			int lodCount;
			void InitializeLODs(const std::vector<std::string> fileNames);

#ifdef COUNT_VALID_BLOCK
			std::shared_ptr<OpenGLBuffer> bufMissedHash;
			std::shared_ptr<OpenGLBuffer> atomicCounter;
			void InitCounter(int capacity);
			int ResetCounter();
#endif

			//int windowWidth;
			//int windowHeight;
			Vec2i windowSize;
			std::shared_ptr<imgui::TransferFunctionWidget> TFWidget;
		};
	}
}

#endif