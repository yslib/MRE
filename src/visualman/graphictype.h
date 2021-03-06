
#ifndef _GRAPHICTYPE_H_
#define _GRAPHICTYPE_H_

#include <VMUtils/log.hpp>

#include <GL/gl3w.h>
//#include "../../lib/gl3w/GL/glcorearb.h"

inline void PrintGLErrorType( GLenum glerr )
{
	std::string error;
	switch ( glerr ) {
	case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
	case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
	case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
	case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
	case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
	case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
	default: error = "UNKNOWN_ERROR"; break;
	}
	vm::Warning( "{}", error.c_str() );
}

inline GLenum PrintGLErrorMsg( const char *file, int line )
{
	GLenum errorCode;
	while ( ( errorCode = glGetError() ) != GL_NO_ERROR ) {
		std::string error;
		switch ( errorCode ) {
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		vm::Warning( "{} | {} ({})", error.c_str(), file, line );
	}
	return errorCode;
}

#ifdef NDEBUG
#define GL_ERROR_REPORT void( 0 );
#define GL_ERROR_ASSERT void( 0 );
#define GL( stmt ) stmt;
#define GL_CHECK void( 0 );
#else
#define GL_ERROR_REPORT PrintGLErrorMsg( __FILE__, __LINE__ );
//{												\
	//	GLenum err;									\
	//	while((err = glGetError()) != GL_NO_ERROR)	\
	//	{											\
	//		ysl::Warning("OpenGL Error Code:{}. File:{}, Line:{}. \n",err,__FILE__,__LINE__);\
	//	}											\
	//}

#define GL_ERROR_ASSERT \
	assert( glGetError() == GL_NO_ERROR );

#define GL( stmt )                                                              \
	do {                                                                        \
		GLenum glerr;                                                           \
		unsigned int iCounter = 0;                                              \
		while ( ( glerr = glGetError() ) != GL_NO_ERROR ) {                     \
			::vm::Warning( "GL error calling {} before line %u ({}): (%x)",     \
						   #stmt, __LINE__, __FILE__,                           \
						   static_cast<unsigned>( glerr ) );                    \
			PrintGLErrorType( glerr );                                          \
			iCounter++;                                                         \
			if ( iCounter > 100 ) break;                                        \
		}                                                                       \
		stmt;                                                                   \
		iCounter = 0;                                                           \
		while ( ( glerr = glGetError() ) != GL_NO_ERROR ) {                     \
			::vm::Warning( "'{}' on line %u ({}) caused GL error: ({})", #stmt, \
						   __LINE__, __FILE__,                                  \
						   static_cast<unsigned>( glerr ) );                    \
			PrintGLErrorType( glerr );                                          \
			iCounter++;                                                         \
			if ( iCounter > 100 ) break;                                        \
		}                                                                       \
	} while ( 0 );

#define GL_CHECK                                                     \
	do {                                                             \
		GLenum glerr;                                                \
		unsigned int iCounter = 0;                                   \
		while ( ( glerr = glGetError() ) != GL_NO_ERROR ) {          \
			::vm::Warning( "before line %u ({}):(%#x)",              \
						   __LINE__, __FILE__,                       \
						   static_cast<unsigned>( glerr ) );         \
			PrintGLErrorType( glerr );                               \
			iCounter++;                                              \
			if ( iCounter > 100 ) break;                             \
		}                                                            \
		iCounter = 0;                                                \
		while ( ( glerr = glGetError() ) != GL_NO_ERROR ) {          \
			::vm::Warning( " on line %u ({}) caused GL error:(%#x)", \
						   __LINE__, __FILE__,                       \
						   static_cast<unsigned>( glerr ) );         \
			PrintGLErrorType( glerr );                               \
			iCounter++;                                              \
			if ( iCounter > 100 ) break;                             \
		}                                                            \
	} while ( 0 );

#endif /*NDBUG*/

//
//#define USE_SMART_POINTER
//
//
//#define Ref(typeName) std::shared_ptr<typeName>
//
//#ifdef USE_SMART_POINTER
//
//
//#define SharedPtr(typeName) std::shared_ptr<typeName>
//#define RSharedPtr(typeName) std::shared_ptr<typeName> &
//#define CSharedPtr(typeName) std::shared_ptr<const typeName>
//#define CRSharedPtr(typeName) const std::shared_ptr<typeName> &
//
//#define UniquePtr(typeName) std::unique_ptr<typeName>
//#define RUniquePtr(typeName) std::unique_ptr<typeName> &
//#define CUniquePtr(typeName) std::unique_ptr<const typeName>
//#define CRUniquePtr(typeName) const std::unique_ptr<typeName> &
//
//#define WeakPtr(typeName) std::weak_ptr<typeName>
//#define RWeakPtr(typeName) std::weak_ptr<typeName> &
//#define CWeakPtr(typeName) std::weak_ptr<const typeName>
//#define CRWeakPtr(typeName) const std::weak<typeName> &
//
//#define GetPtr(ptr) (ptr.get())
//
//#elif
//
//#define SharedPtr(typeName) typeName*
//#define RSharedPtr(typeName) typeName*
//#define CSharedPtr(typeName) const typeName*
//#define CRSharedPtr(typeName) const typeName*
//
//#define UniquePtr(typeName) typeName*
//#define RUniquePtr(typeName) typeName*
//#define CUniquePtr(typeName) const typeName*
//#define CRUniquePtr(typeName) const typeName*
//
//#define WeakPtr(typeName) typeName*
//#define RWeakPtr(typeName) typeName*
//#define CWeakPtr(typeName) const typeName*
//#define CRWeakPtr(typeName) const typeName*
//
//#define GetPtr(ptr) (ptr)
//
//#endif

#include <memory>
#include <GL/glcorearb.h>
#include <map>
#include <set>

#if defined( _WIN32 ) && defined( VM_SHARED_LIBRARY )
#ifdef vm_EXPORTS
#define VISUALMAN_EXPORT_IMPORT __declspec( dllexport )
#else
#define VISUALMAN_EXPORT_IMPORT __declspec( dllimport )
#endif
#else
#define VISUALMAN_EXPORT_IMPORT
#endif

//#define GRAPHICS_EXPORT_IMPORT



template <typename Ty>
using VMRef = std::shared_ptr<Ty>;

template <typename Ty>
using WeakRef = std::weak_ptr<Ty>;

template <typename Ty>
using ExclRef = std::unique_ptr<Ty>;

template <typename Ty1, typename Ty2>
using Map = std::map<Ty1, Ty2>;

template <typename Ty>
using Set = std::set<Ty>;

template <typename Ty, typename... Args>
inline VMRef<Ty> MakeVMRef( Args &&... args )
{
	return std::make_shared<Ty>( std::forward<Args>( args )... );
}

namespace vm
{
struct Descriptor3D
{
	int w = 0, h = 0, d = 0;
	int xOffset = 0, yOffset = 0, zOffset = 0;
};

struct Descriptor2D
{
	int w = 0, h = 0;
	int xOffset = 0, yOffset = 0;
};

struct Descriptor1D
{
	size_t offset = 0;
	size_t bytes = 0;
};
}  // namespace vm


// Enum for OpenGL

enum Function
{
	FU_NEVER = GL_NEVER,
	FU_LESS = GL_LESS,
	FU_EQUAL = GL_EQUAL,
	FU_LEQUAL = GL_LEQUAL,
	FU_GREATER = GL_GREATER,
	FU_NOTEQUAL = GL_NOTEQUAL,
	FU_GEQUAL = GL_GEQUAL,
	FU_ALWAYS = GL_ALWAYS
};

enum BufferObjectUsage
{
	BU_STREAM_DRAW = GL_STREAM_DRAW,
	BU_STREAM_READ = GL_STREAM_READ,
	BU_STREAM_COPY = GL_STREAM_COPY,
	BU_STATIC_DRAW = GL_STATIC_DRAW,
	BU_STATIC_READ = GL_STATIC_READ,
	BU_STATIC_COPY = GL_STATIC_COPY,
	BU_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
	BU_DYNAMIC_READ = GL_DYNAMIC_READ,
	BU_DYNAMIC_COPY = GL_DYNAMIC_COPY
};

enum BufferStorageBit
{
	VM_BST_DYNAMIC_STORAGE_BIT = GL_DYNAMIC_STORAGE_BIT,
	VM_BST_MAP_READ_BIT = GL_MAP_READ_BIT,
	VM_BST_MAP_WRITE_BIT = GL_MAP_WRITE_BIT,
	VM_BST_MAP_PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
	VM_BST_MAP_COHERENT_BIT = GL_MAP_COHERENT_BIT,
	VM_BST_CLIENT_STORAGE_BIT = GL_CLIENT_STORAGE_BIT
};

enum BufferRangeMapBit
{
	VM_BMB_MAP_READ_BIT = GL_MAP_READ_BIT,
	VM_BMB_MAP_WRITE_BIT = GL_MAP_WRITE_BIT,
	VM_BMB_MAP_PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
	VM_BMB_COHERENT_BIT = GL_MAP_COHERENT_BIT
};

enum BufferMapAccess
{
	BA_READ_ONLY = GL_READ_ONLY,
	BA_WRITE_ONLY = GL_WRITE_ONLY,
	BA_READ_WRITE = GL_READ_WRITE
};

enum BufferObjectTarget
{
	VM_BT_ARRAY_BUFFER = GL_ARRAY_BUFFER,
	VM_BT_ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
	VM_BT_PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
	VM_BT_PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
	VM_BT_SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER
};

enum ImageUnitAccess
{
	VM_IA_READ_ONLY = GL_READ_ONLY,
	VM_IA_WRITE_ONLY = GL_WRITE_ONLY,
	VM_IA_READ_WRITE = GL_READ_WRITE
};

enum FramebufferBind
{
	FBB_FRAMEBUFFER = GL_FRAMEBUFFER,
	FBB_DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
	FBB_READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER,
};

enum ReadDrawBuffer
{
	RDB_NONE = GL_NONE,
	RDB_FRONT_LEFT = GL_FRONT_LEFT,
	RDB_FRONT_RIGHT = GL_FRONT_RIGHT,
	RDB_BACK_LEFT = GL_BACK_LEFT,
	RDB_BACK_RIGHT = GL_BACK_RIGHT,
	RDB_COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
	RDB_COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
	RDB_COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
	RDB_COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
	RDB_COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
	RDB_COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
	RDB_COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
	RDB_COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
	RDB_COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
	RDB_COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
	RDB_COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
	RDB_COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
	RDB_COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
	RDB_COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
	RDB_COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
	RDB_COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15
};

enum AttachmentBindPoint
{
	AP_NO_ATTACHMENT = 0,

	AP_COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
	AP_COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
	AP_COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
	AP_COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
	AP_COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
	AP_COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
	AP_COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
	AP_COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
	AP_COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
	AP_COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
	AP_COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
	AP_COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
	AP_COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
	AP_COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
	AP_COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
	AP_COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
	AP_DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
	AP_STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
	AP_DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT,
};

enum ColorBufferFormat
{
	CBF_RED = GL_RED,
	CBF_RG = GL_RG,
	CBF_RGB = GL_RGB,
	CBF_RGBA = GL_RGBA,
	CBF_R8 = GL_R8,
	CBF_R16 = GL_R16,
	CBF_RG8 = GL_RG8,
	CBF_RG16 = GL_RG16,
	CBF_RGBA8 = GL_RGBA8,
	CBF_RGBA16 = GL_RGBA16,
	CBF_R16F = GL_R16F,
	CBF_RG16F = GL_RG16F,
	CBF_RGBA16F = GL_RGBA16F,
	CBF_R32F = GL_R32F,
	CBF_RG32F = GL_RG32F,
	CBF_RGBA32F = GL_RGBA32F,
	CBF_R8I = GL_R8I,
	CBF_R8UI = GL_R8UI,
	CBF_R16I = GL_R16I,
	CBF_R16UI = GL_R16UI,
	CBF_R32I = GL_R32I,
	CBF_R32UI = GL_R32UI,
	CBF_RG8I = GL_RG8I,
	CBF_RG8UI = GL_RG8UI,
	CBF_RG16I = GL_RG16I,
	CBF_RG16UI = GL_RG16UI,
	CBF_RG32I = GL_RG32I,
	CBF_RG32UI = GL_RG32UI,
	CBF_RGB8I = GL_RGB8I,
	CBF_RGB8UI = GL_RGB8UI,
	CBF_RGB16I = GL_RGB16I,
	CBF_RGB16UI = GL_RGB16UI,
	CBF_RGB32I = GL_RGB32I,
	CBF_RGB32UI = GL_RGB32UI,
	CBF_RGBA8I = GL_RGBA8I,
	CBF_RGBA8UI = GL_RGBA8UI,
	CBF_RGBA16I = GL_RGBA16I,
	CBF_RGBA16UI = GL_RGBA16UI,
	CBF_RGBA32I = GL_RGBA32I,
	CBF_RGBA32UI = GL_RGBA32UI
};

enum DepthBufferFormat
{
	DBF_DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	DBF_DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
	DBF_DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
	DBF_DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
	DBF_DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
};

enum StencilBufferFormat
{
	SBF_STENCIL_INDEX1 = GL_STENCIL_INDEX1,
	SBF_STENCIL_INDEX4 = GL_STENCIL_INDEX4,
	SBF_STENCIL_INDEX8 = GL_STENCIL_INDEX8,
	SBF_STENCIL_INDEX16 = GL_STENCIL_INDEX16
};

enum DepthStencilBufferFormat
{
	DSBT_DEPTH_STENCIL = GL_DEPTH_STENCIL,
	DSBT_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
	DSBT_DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8
};

enum TextureTarget
{
	TD_TEXTURE_NONE = 0,
	TD_TEXTURE_1D = GL_TEXTURE_1D,
	TD_TEXTURE_2D = GL_TEXTURE_2D,
	TD_TEXTURE_3D = GL_TEXTURE_3D,
	TD_TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
	TD_TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
	TD_TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
	TD_TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
	TD_TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
	TD_TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
	TD_TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

enum Texture2DTarget
{
	T2DT_TEXTURE_2D = GL_TEXTURE_2D,
	T2DT_TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	T2DT_TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	T2DT_TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	T2DT_TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	T2DT_TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	T2DT_TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	T2DT_TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
	T2DT_TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
	T2DT_TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
};

enum TextureFormat
{

	TF_UNKNOWN = 0,

	TF_R3_G3_B2 = GL_R3_G3_B2,
	TF_RGB = GL_RGB,
	TF_RGB4 = GL_RGB4,
	TF_RGB5 = GL_RGB5,
	TF_RGB8 = GL_RGB8,
	TF_RGB10 = GL_RGB10,
	TF_RGB12 = GL_RGB12,
	TF_RGB16 = GL_RGB16,
	TF_RGBA = GL_RGBA,
	TF_RGBA2 = GL_RGBA2,
	TF_RGBA4 = GL_RGBA4,
	TF_RGB5_A1 = GL_RGB5_A1,
	TF_RGBA8 = GL_RGBA8,
	TF_RGB10_A2 = GL_RGB10_A2,
	TF_RGBA12 = GL_RGBA12,
	TF_RGBA16 = GL_RGBA16,

	// ARB_texture_float / OpenGL 3
	TF_RGBA32F = GL_RGBA32F,
	TF_RGB32F = GL_RGB32F,
	TF_RGBA16F = GL_RGBA16F,
	TF_RGB16F = GL_RGB16F,

	// EXT_packed_depth_stencil / GL_ARB_framebuffer_object
	TF_DEPTH_STENCIL = GL_DEPTH_STENCIL,
	TF_DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,

	// ARB_depth_buffer_float
	TF_DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
	TF_DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,

	// ARB_depth_texture
	TF_DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	TF_DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
	TF_DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
	TF_DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,

	// GL_ARB_texture_rg
	TF_RED = GL_RED,
	TF_COMPRESSED_RED = GL_COMPRESSED_RED,
	TF_COMPRESSED_RG = GL_COMPRESSED_RG,
	TF_RG = GL_RG,
	TF_R8 = GL_R8,
	TF_R16 = GL_R16,
	TF_RG8 = GL_RG8,
	TF_RG16 = GL_RG16,
	TF_R16F = GL_R16F,
	TF_R32F = GL_R32F,
	TF_RG16F = GL_RG16F,
	TF_RG32F = GL_RG32F,
	TF_R8I = GL_R8I,
	TF_R8UI = GL_R8UI,
	TF_R16I = GL_R16I,
	TF_R16UI = GL_R16UI,
	TF_R32I = GL_R32I,
	TF_R32UI = GL_R32UI,
	TF_RG8I = GL_RG8I,
	TF_RG8UI = GL_RG8UI,
	TF_RG16I = GL_RG16I,
	TF_RG16UI = GL_RG16UI,
	TF_RG32I = GL_RG32I,
	TF_RG32UI = GL_RG32UI,

	// sRGB OpenGL 2.1
	//TF_SLUMINANCE_ALPHA = GL_SLUMINANCE_ALPHA,
	//TF_SLUMINANCE8_ALPHA8 = GL_SLUMINANCE8_ALPHA8,
	//TF_SLUMINANCE = GL_SLUMINANCE,
	//TF_SLUMINANCE8 = GL_SLUMINANCE8,
	//TF_COMPRESSED_SLUMINANCE = GL_COMPRESSED_SLUMINANCE,
	//TF_COMPRESSED_SLUMINANCE_ALPHA = GL_COMPRESSED_SLUMINANCE_ALPHA,

	// sRGB OpenGL 2.1 / 3.x
	TF_SRGB = GL_SRGB,
	TF_SRGB8 = GL_SRGB8,
	TF_SRGB_ALPHA = GL_SRGB_ALPHA,
	TF_SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
	TF_COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
	TF_COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA,

	// from table 3.12 opengl api specs 4.1
	TF_R8_SNORM = GL_R8_SNORM,
	TF_R16_SNORM = GL_R16_SNORM,
	TF_RG8_SNORM = GL_RG8_SNORM,
	TF_RG16_SNORM = GL_RG16_SNORM,
	TF_RGB8_SNORM = GL_RGB8_SNORM,
	TF_RGBA8_SNORM = GL_RGBA8_SNORM,
	TF_RGB10_A2UI = GL_RGB10_A2UI,
	TF_RGBA16_SNORM = GL_RGBA16_SNORM,
	TF_R11F_G11F_B10F = GL_R11F_G11F_B10F,
	TF_RGB9_E5 = GL_RGB9_E5,
	TF_RGB8I = GL_RGB8I,
	TF_RGB8UI = GL_RGB8UI,
	TF_RGB16I = GL_RGB16I,
	TF_RGB16UI = GL_RGB16UI,
	TF_RGB32I = GL_RGB32I,
	TF_RGB32UI = GL_RGB32UI,
	TF_RGBA8I = GL_RGBA8I,
	TF_RGBA8UI = GL_RGBA8UI,
	TF_RGBA16I = GL_RGBA16I,
	TF_RGBA16UI = GL_RGBA16UI,
	TF_RGBA32I = GL_RGBA32I,
	TF_RGBA32UI = GL_RGBA32UI
};

enum TexFilter
{
	TPF_NEAREST = GL_NEAREST,
	TPF_LINEAR = GL_LINEAR,
	TPF_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	TPF_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	TPF_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	TPF_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum TexWrap
{
	//TPW_CLAMP = GL_CLAMP,
	TPW_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	TPW_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	TPW_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	TPW_REPEAT = GL_REPEAT
};

enum ImageFormat
{
	IF_RGB = GL_RGB,
	IF_RGBA = GL_RGBA,
	IF_BGR = GL_BGR,
	IF_BGRA = GL_BGRA,
	IF_RG = GL_RG,
	IF_RG_INTEGER = GL_RG_INTEGER,
	IF_RED = GL_RED,
	IF_GREEN = GL_GREEN,
	IF_BLUE = GL_BLUE,
	IF_ALPHA = GL_ALPHA,
	IF_DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	IF_STENCIL_INDEX = GL_STENCIL_INDEX,

	// EXT_packed_depth_stencil / GL_ARB_framebuffer_object
	IF_DEPTH_STENCIL = GL_DEPTH_STENCIL,

	// compressed formats
	// note: with these format the type must be IT_IMPLICIT_TYPE

	IF_COMPRESSED_RGB_S3TC_DXT1 = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
	IF_COMPRESSED_RGBA_S3TC_DXT1 = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
	IF_COMPRESSED_RGBA_S3TC_DXT3 = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	IF_COMPRESSED_RGBA_S3TC_DXT5 = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,

	// GL 3.0 (EXT_texture_integer)
	IF_RED_INTEGER = GL_RED_INTEGER,
	IF_GREEN_INTEGER = GL_GREEN_INTEGER,
	IF_BLUE_INTEGER = GL_BLUE_INTEGER,
	IF_RGB_INTEGER = GL_RGB_INTEGER,
	IF_RGBA_INTEGER = GL_RGBA_INTEGER,
	IF_BGR_INTEGER = GL_BGR_INTEGER,
	IF_BGRA_INTEGER = GL_BGRA_INTEGER,
};

enum ImageType
{
	IT_IMPLICIT_TYPE = 0,  //!< The type is implicitly defined by the EImageFormat value, for ex. IF_COMPRESSED_RGB_S3TC_DXT1.

	IT_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	IT_BYTE = GL_BYTE,
	IT_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
	IT_SHORT = GL_SHORT,
	IT_UNSIGNED_INT = GL_UNSIGNED_INT,
	IT_INT = GL_INT,
	IT_FLOAT = GL_FLOAT,
	IT_UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
	IT_UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
	IT_UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
	IT_UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
	IT_UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
	IT_UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	IT_UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
	IT_UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	IT_UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
	IT_UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
	IT_UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
	IT_UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,

	IT_UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8,						  /* EXT_packed_depth_stencil/GL_ARB_framebuffer_object */
	IT_FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV /* ARB_depth_buffer_float */

};

enum ClearFlag
{
	CF_DO_NOT_CLEAR = 0,
	CF_CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
	CF_CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
	CF_CLEAR_STENCIL = GL_STENCIL_BUFFER_BIT,
	CF_CLEAR_COLOR_DEPTH = CF_CLEAR_COLOR + CF_CLEAR_DEPTH,
	CF_CLEAR_COLOR_STENCIL = CF_CLEAR_COLOR + CF_CLEAR_STENCIL,
	CF_CLEAR_DEPTH_STENCIL = CF_CLEAR_DEPTH + CF_CLEAR_STENCIL,
	CF_CLEAR_COLOR_DEPTH_STENCIL = CF_CLEAR_COLOR + CF_CLEAR_DEPTH + CF_CLEAR_STENCIL,
};

enum PrimitiveType
{
	PT_POINTS = GL_POINTS,
	PT_LINES = GL_LINES,
	PT_LINE_LOOP = GL_LINE_LOOP,

	PT_LINE_STRIP = GL_LINE_STRIP,
	PT_TRIANGLES = GL_TRIANGLES,
	PT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	PT_TRIANGLE_FAN = GL_TRIANGLE_FAN,
};

enum PolygonMode
{
	PM_FILL = GL_FILL,
	PM_LINE = GL_LINE,
	PM_POINT = GL_POINT
};

enum PolygonFace
{
	PF_FRONT = GL_FRONT,
	PF_BACK = GL_BACK,
	PF_FONRT_AND_BACK = GL_FRONT_AND_BACK
};

enum FrontFace
{
	FF_CW = GL_CW,
	FF_CCW = GL_CCW
};

enum BlendEquation
{
	BE_FUNC_ADD = GL_FUNC_ADD,
	BE_FUNC_SUBTRACT = GL_FUNC_SUBTRACT,
	BE_FUNC_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
	BE_MIN = GL_MIN,
	BE_MAX = GL_MAX
};

enum BufferBits
{
	VM_BB_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
	VM_BB_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
	VM_BB_STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT
};

enum BlendFactor
{
	BF_ZERO = GL_ZERO,
	BF_ONE = GL_ONE,
	BF_SRC_COLOR = GL_SRC_COLOR,
	BF_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	BF_DST_COLOR = GL_DST_COLOR,
	BF_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	BF_SRC_ALPHA = GL_SRC_ALPHA,
	BF_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	BF_DST_ALPHA = GL_DST_ALPHA,
	BF_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	BF_CONSTANT_COLOR = GL_CONSTANT_COLOR,
	BF_ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
	BF_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
	BF_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
	BF_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
};

enum StencilOperator
{
	SO_KEEP = GL_KEEP,
	SO_ZERO = GL_ZERO,
	SO_REPLACE = GL_REPLACE,
	SO_INCR = GL_INCR,
	SO_INCR_WRAP = GL_INCR_WRAP,
	SO_DECR = GL_DECR,
	SO_DECR_WRAP = GL_DECR_WRAP,
	SO_INVERT = GL_INVERT
};

enum EnableState
{
	EN_BLEND,
	EN_CULL_FACE,
	EN_DEPTH_TEST,
	EN_STENCIL_TEST,

	EN_EnableState_Count
};

enum RenderStateType
{
	RS_VertexAttrib,
	RS_VertexAttrib0 = RS_VertexAttrib,
	RS_VertexAttrib1,
	RS_VertexAttrib2,
	RS_VertexAttrib3,
	RS_VertexAttrib4,
	RS_VertexAttrib5,
	RS_VertexAttrib6,
	RS_VertexAttrib7,

	// Non-indexed state
	RS_AlphaFunc,
	RS_BlendEquation,
	RS_BlendFunc,
	RS_CullFace,
	RS_DepthFunc,
	RS_FrontFace,
	RS_PolygonMode,
	RS_LineWidth,
	RS_GLSLProgram,

	//RS_StencilFunc,
	//RS_StencilMask,
	//RS_StencilOp,

	// Indexed state
	RS_TextureSampler,
	RS_TextureSampler0 = RS_TextureSampler + 0,
	RS_TextureSampler1 = RS_TextureSampler + 1,
	RS_TextureSampler2 = RS_TextureSampler + 2,
	RS_TextureSampler3 = RS_TextureSampler + 3,
	RS_TextureSampler4 = RS_TextureSampler + 4,
	RS_TextureSampler5 = RS_TextureSampler + 5,
	RS_TextureSampler6 = RS_TextureSampler + 6,
	RS_TextureSampler7 = RS_TextureSampler + 7,
	RS_TextureSampler8 = RS_TextureSampler + 8,
	RS_TextureSampler9 = RS_TextureSampler + 9,
	RS_TextureSampler10 = RS_TextureSampler + 10,
	RS_TextureSampler11 = RS_TextureSampler + 11,
	RS_TextureSampler12 = RS_TextureSampler + 12,
	RS_TextureSampler13 = RS_TextureSampler + 13,
	RS_TextureSampler14 = RS_TextureSampler + 14,
	RS_TextureSampler15 = RS_TextureSampler + 15,

	RS_TextureImageUnit,
	RS_TextureImageUnit0 = RS_TextureImageUnit + 0,
	RS_TextureImageUnit1 = RS_TextureImageUnit + 1,
	RS_TextureImageUnit2 = RS_TextureImageUnit + 2,
	RS_TextureImageUnit3 = RS_TextureImageUnit + 3,
	RS_TextureImageUnit4 = RS_TextureImageUnit + 4,
	RS_TextureImageUnit5 = RS_TextureImageUnit + 5,
	RS_TextureImageUnit6 = RS_TextureImageUnit + 6,
	RS_TextureImageUnit7 = RS_TextureImageUnit + 7,
	RS_TextureImageUnit8 = RS_TextureImageUnit + 8,
	RS_TextureImageUnit9 = RS_TextureImageUnit + 9,
	RS_TextureImageUnit10 = RS_TextureImageUnit + 10,
	RS_TextureImageUnit11 = RS_TextureImageUnit + 11,
	RS_TextureImageUnit12 = RS_TextureImageUnit + 12,
	RS_TextureImageUnit13 = RS_TextureImageUnit + 13,
	RS_TextureImageUnit14 = RS_TextureImageUnit + 14,
	RS_TextureImageUnit15 = RS_TextureImageUnit + 15,

	RS_AtomicCounterBuffer,
	RS_AtomicCounterBuffer0 = RS_AtomicCounterBuffer + 0,
	RS_AtomicCounterBuffer1 = RS_AtomicCounterBuffer + 1,
	RS_AtomicCounterBuffer2 = RS_AtomicCounterBuffer + 2,
	RS_AtomicCounterBuffer3 = RS_AtomicCounterBuffer + 3,
	RS_AtomicCounterBuffer4 = RS_AtomicCounterBuffer + 4,
	RS_AtomicCounterBuffer5 = RS_AtomicCounterBuffer + 5,
	RS_AtomicCounterBuffer6 = RS_AtomicCounterBuffer + 6,
	RS_AtomicCounterBuffer7 = RS_AtomicCounterBuffer + 7,

	RS_ShaderStorageBuffer,
	RS_ShaderStorageBuffer0 = RS_ShaderStorageBuffer + 0,
	RS_ShaderStorageBuffer1 = RS_ShaderStorageBuffer + 1,
	RS_ShaderStorageBuffer2 = RS_ShaderStorageBuffer + 2,
	RS_ShaderStorageBuffer3 = RS_ShaderStorageBuffer + 3,
	RS_ShaderStorageBuffer4 = RS_ShaderStorageBuffer + 4,
	RS_ShaderStorageBuffer5 = RS_ShaderStorageBuffer + 5,
	RS_ShaderStorageBuffer6 = RS_ShaderStorageBuffer + 6,
	RS_ShaderStorageBuffer7 = RS_ShaderStorageBuffer + 7,

	RS_UniformBuffer,
	RS_UniformBuffer0 = RS_UniformBuffer + 0,
	RS_UniformBuffer1 = RS_UniformBuffer + 1,
	RS_UniformBuffer2 = RS_UniformBuffer + 2,
	RS_UniformBuffer3 = RS_UniformBuffer + 3,
	RS_UniformBuffer4 = RS_UniformBuffer + 4,
	RS_UniformBuffer5 = RS_UniformBuffer + 5,
	RS_UniformBuffer6 = RS_UniformBuffer + 6,
	RS_UniformBuffer7 = RS_UniformBuffer + 7,

	RS_RenderState_Count,
	RS_NONE,
};

enum GLSLShaderType
{
	ST_VERTEX_SHADER = GL_VERTEX_SHADER,
	ST_TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
	ST_TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
	ST_GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
	ST_FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

enum VertexAttribPosition
{
	VA_VertexPositionAttribLocation = 0,
	VA_VertexColorAttribLocation = 1,
	VA_VertexNormalAttribLocation = 2,
	VA_VertexTexCoord0AttribLocation = 3,
	VA_VertexTexCoord1AttribLocation = 4,
	VA_VertexTexCoord2AttribLocation = 5,
	VA_VertexTexCoord3AttribLocation = 6,
	VA_VertexTexCoord4AttribLocation = 7,

	VA_VertexAttribPosition_Count
};

enum VertexAttribArrayIndexType
{
	VA_VertexPositionAttrib,
	VA_VertexColorAttrib,
	VA_VertexNormalAttrib,
	VA_VertexTexCoordAttrib,
	VA_VertexAttribArray_Count
};

enum BufferObjectUpdateFlag
{
	VM_BUF_Force = 1,
	VM_BUF_DiscardRAM = 2,
};

enum BufferObjectUpdateMode
{
	VM_UM_KeepRAM,
	VM_UM_ForceKeepRAM = VM_BUF_Force,
	VM_UM_DiscardRAM = VM_BUF_DiscardRAM,
	VM_UM_ForceDiscardRAM = VM_BUF_Force | VM_BUF_DiscardRAM,
};

#endif