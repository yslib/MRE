#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "graphictype.h"
#include "texture.h"
#include <VMGraphics/interpulator.h>
#include <cassert>
#include <VMFoundation/rawreader.h>
#include <stb_image_write.h>
#include <VMUtils/log.hpp>
#include <VMUtils/timer.hpp>

namespace vm
{
int GetBaseFormatBySizedFormat( TextureFormat sizedFormat )
{
	switch ( sizedFormat ) {
	case TF_RED:
	case TF_R8:
	case TF_R8_SNORM:
	case TF_R16:
	case TF_R16_SNORM:
	case TF_R16F:
	case TF_R32F:
		return GL_RED;

	case TF_RG:
	case TF_RG8:
	case TF_RG8_SNORM:
	case TF_RG16:
	case TF_RG16_SNORM:
	case TF_RG16F:
	case TF_RG32F:
		return GL_RG;

	case TF_RGB:
	case TF_RGB4:
	case TF_RGB5:
	case TF_RGB8:
	case TF_RGB8_SNORM:
	case TF_RGB10:
	case TF_RGB12:
	case TF_RGB16:
		// case TF_RGB16_SNORM:
	case TF_RGB16F:
	case TF_RGB32F:
	case TF_R3_G3_B2:
	case TF_R11F_G11F_B10F:
		// case TF_RGB9_A5:
	case TF_SRGB8:
		return GL_RGB;

	case TF_RGBA:
	case TF_RGBA2:
	case TF_RGBA4:
	case TF_RGBA8:
	case TF_RGBA8_SNORM:
	case TF_RGBA12:
	case TF_RGBA16:
	case TF_RGBA16_SNORM:
	case TF_RGBA16F:
	case TF_RGBA32F:
	case TF_RGB5_A1:
	case TF_RGB10_A2:
	case TF_SRGB8_ALPHA8:
		return GL_RGBA;

	case TF_R8I:
	case TF_R8UI:
	case TF_R16I:
	case TF_R16UI:
	case TF_R32I:
	case TF_R32UI:
		return GL_RED_INTEGER;

	case TF_RG8I:
	case TF_RG8UI:
	case TF_RG16I:
	case TF_RG16UI:
	case TF_RG32I:
	case TF_RG32UI:
		return GL_RG_INTEGER;

	case TF_RGB8I:
		// case TF_RGB8I_EXT: // Has the same value has above
	case TF_RGB8UI:
		// case TF_RGB8UI_EXT: // Has the same value has above
	case TF_RGB16I:
		// case TF_RGB16I_EXT: // Has the same value has above
	case TF_RGB16UI:
		// case TF_RGB16UI_EXT: // Has the same value has above
	case TF_RGB32I:
		// case TF_RGB32I_EXT: // Has the same value has above
	case TF_RGB32UI:
		// case TF_RGB32UI_EXT: // Has the same value has above
		return GL_RGB_INTEGER;

	case TF_RGBA8I:
		// case TF_RGBAI_EXT: // Has the same value has above
	case TF_RGBA8UI:
		// case TF_RGBA8UI_EXT: // Has the same value has above
	case TF_RGBA16I:
		// case TF_RGBA16I_EXT: // Has the same value has above
	case TF_RGBA16UI:
		// case TF_RGBA16UI_EXT: // Has the same value has above
	case TF_RGBA32I:
		// case TF_RGBA32I_EXT: // Has the same value has above
	case TF_RGBA32UI:
		// case TF_RGBA32UI_EXT: // Has the same value has above
	case TF_RGB10_A2UI:
		return GL_RGBA_INTEGER;

	case TF_DEPTH_STENCIL:
	case TF_DEPTH24_STENCIL8:
	case TF_DEPTH32F_STENCIL8:
		return GL_DEPTH_STENCIL;

	case TF_DEPTH_COMPONENT:
	case TF_DEPTH_COMPONENT16:
	case TF_DEPTH_COMPONENT24:
	case TF_DEPTH_COMPONENT32:
	case TF_DEPTH_COMPONENT32F:
		return GL_DEPTH_COMPONENT;

	default:
		return GL_RED;
	}
}

int GetTypeBySizedFormat( TextureFormat sizedFormat )
{
	switch ( sizedFormat ) {
	case TF_R8:
	case TF_R8UI:
	case TF_RG8:
	case TF_RG8UI:
	case TF_RGB8:
	case TF_RGB8UI:
	case TF_RGBA8:
	case TF_RGBA8UI:
		return GL_UNSIGNED_BYTE;
	case TF_R8I:
	case TF_RG8I:
	case TF_RGB8I:
	case TF_RGBA8I:
		return GL_BYTE;
	case TF_R16:
	case TF_R16UI:
	case TF_RG16:
	case TF_RG16UI:
	case TF_RGB10:
	case TF_RGB12:
	case TF_RGB16:
	case TF_RGB16UI:
	case TF_RGBA12:
	case TF_RGBA16:
	case TF_RGBA16UI:
	case TF_DEPTH_COMPONENT16:
		return GL_UNSIGNED_SHORT;

	case TF_R16I:
	case TF_RG16I:
	case TF_RGB16I:
	case TF_RGBA16I:
		return GL_SHORT;

	case TF_R32UI:
	case TF_RG32UI:
	case TF_RGB32UI:
	case TF_RGBA32UI:
	case TF_DEPTH_COMPONENT24:
	case TF_DEPTH_COMPONENT32:
		return GL_UNSIGNED_INT;

	case TF_R32I:
	case TF_RG32I:
	case TF_RGB32I:
	case TF_RGBA32I:
		return GL_INT;

	case TF_DEPTH24_STENCIL8:
		return GL_UNSIGNED_INT_24_8;

	case TF_R16F:
	case TF_R32F:
	case TF_RG16F:
	case TF_RG32F:
	case TF_RGB16F:
	case TF_RGB32F:
	case TF_RGBA16F:
	case TF_RGBA32F:
	case TF_R11F_G11F_B10F:
	case TF_DEPTH_COMPONENT32F:
		return GL_FLOAT;

	case TF_DEPTH32F_STENCIL8:
		return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

	default:
		return GL_UNSIGNED_BYTE;
	}
}

Texture::~Texture()
{
	DestroyTexture();
}

/**
		 * \brief  Create a texture by given \a TexCreateParams
		 */
bool Texture::CreateTexture()
{
	if ( !createParams ) {
		::vm::Debug( "Setup Parameters have not been set.\n" );
		return false;
	}

	return CreateTexture( createParams->GetTextureTarget(),
						  createParams->GetTextureFormat(),
						  createParams->Width(),
						  createParams->Height(),
						  createParams->Depth(),
						  createParams->IsBorder(),
						  createParams->GetBufferObject(),
						  0 );
}

/**
		 * \brief Create a texture by the given parameters
		 *
		 *  A buffer object is need only when the target is a buffer texture.
		 */
bool Texture::CreateTexture( TextureTarget target,
							 TextureFormat texFormat,
							 int w,
							 int h,
							 int d,
							 bool border,
							 VMRef<BufferObject> bufferObject,
							 int samples )
{
	DestroyTexture();

	GL( glCreateTextures( target, 1, &handle ) );
	if ( handle == 0 ) {
		::vm::Warning( "Texture can not be created. {} {}", __FILE__, __LINE__ );
		return false;
	}

	if ( target == TD_TEXTURE_1D ) {
		GL( glTextureStorage1D( handle, 1, texFormat, w ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_R, texParams->WrapR() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MAG_FILTER, texParams->MaxFilter() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MIN_FILTER, texParams->MinFilter() ) );

		//createParams.reset();
	} else if ( target == TD_TEXTURE_2D || target == TD_TEXTURE_RECTANGLE ) {
		GL( glTextureStorage2D( handle, 1, texFormat, w, h ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_R, texParams->WrapR() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_S, texParams->WrapS() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MAG_FILTER, texParams->MaxFilter() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MIN_FILTER, texParams->MinFilter() ) );

		//createParams.reset();
	} else if ( target == TD_TEXTURE_3D ) {
		GL( glTextureStorage3D( handle, 1, texFormat, w, h, d ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_R, texParams->WrapR() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_S, texParams->WrapS() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_WRAP_T, texParams->WrapT() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MAG_FILTER, texParams->MaxFilter() ) );
		GL( glTextureParameteri( handle, GL_TEXTURE_MIN_FILTER, texParams->MinFilter() ) );

		//createParams.reset();
	} else {
		assert( false );
		::vm::Debug( "Unsupported format" );
	}
	// Set up the texture properties and delete the setup params

	this->target = target;
	this->format = texFormat;
	this->d = d;
	this->w = w;
	this->h = h;
	this->samples = samples;
	this->border = border;
	this->bufferObject = std::move( bufferObject );

	return true;
}


void Texture::SaveTextureAs( const std::string &fileName )
{
	int texDim[ 2 ];
	glGetTextureLevelParameteriv( handle, 0, GL_TEXTURE_WIDTH, texDim );
	glGetTextureLevelParameteriv( handle, 0, GL_TEXTURE_HEIGHT, texDim + 1 );
	//GL_ERROR_REPORT;
	const auto bufSize = std::size_t( texDim[ 0 ] ) * texDim[ 1 ] * 4 * sizeof( char );
	std::unique_ptr<char[]> imageBuf( new char[ bufSize ] );
	if ( !imageBuf ) {
		throw std::runtime_error( "Create buffer failed\n" );
	}
	glGetTextureImage( handle, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufSize, imageBuf.get() );
	//GL_ERROR_REPORT;
	stbi_write_jpg( fileName.c_str(), texDim[ 0 ], texDim[ 1 ], 4, imageBuf.get(), 100 );
}

void Texture::DestroyTexture()
{
	if ( handle ) {
		GL( glDeleteTextures( 1, &handle ) );
		handle = 0;
	}
}

void Texture::SetSetupParams( VMRef<TexCreateParams> params )
{
	createParams = std::move( params );
}

void Texture::SetSubTextureData( const void *data, ImageFormat imageFormat, ImageType imageType, int xOffset, int yOffset, int zOffset, int w, int h, int d )
{
	if ( handle == 0 ) {
		::vm::Warning( "No Texture has been created. {}: {}", __FILE__, __LINE__ );
		if ( !CreateTexture() ) {
			::vm::Debug( "Texture has not beed created. But it created failed\n. {}: {}", __FILE__, __LINE__ );
			return;
		}
	}

	if ( target == GL_TEXTURE_1D ) {
		GL( glTextureSubImage1D( handle, 0, xOffset, w, imageFormat, imageType, data ) );
	} else if ( target == GL_TEXTURE_2D || target == GL_TEXTURE_RECTANGLE ) {
		GL( glTextureSubImage2D( handle, 0, xOffset, yOffset, w, h, imageFormat, imageType, data ) );
	} else if ( target == GL_TEXTURE_3D ) {
		GL( glTextureSubImage3D( handle, 0, xOffset, yOffset, zOffset, w, h, d, imageFormat, imageType, data ) );
	} else {
		::vm::Debug( "Unsuported format. {} {}", __FILE__, __LINE__ );
	}
}

void Texture::SetSubTextureData( const void *data, int xOffset, int yOffset, int zOffset, int w, int h, int d )
{
	assert( createParams );
	const auto sizedFormat = createParams->GetTextureFormat();
	const auto imageFormat = ImageFormat( GetBaseFormatBySizedFormat( sizedFormat ) );
	const auto imageType = ImageType( GetTypeBySizedFormat( sizedFormat ) );
	SetSubTextureData( data, imageFormat, imageType, xOffset, yOffset, zOffset, w, h, d );
}

void Texture::SetSubTextureData( ImageFormat imageFormat, ImageType imageType, int xOffset, int yOffset,
								 int zOffset, int w, int h, int d )
{
	if ( bufferObject ) {
		SetSubTextureData( bufferObject->LocalData(), imageFormat, imageType, xOffset, yOffset, zOffset, w, h, d );
	} else {
		::vm::Debug( "No Buffer Object. {}:{}", __FILE__, __LINE__ );
	}
}

void Texture::SetSubTextureData( int xOffset, int yOffset, int zOffset, int w, int h, int d )
{
	assert( createParams );
	const auto sizedFormat = createParams->GetTextureFormat();
	const auto imageFormat = ImageFormat( GetBaseFormatBySizedFormat( sizedFormat ) );
	const auto imageType = ImageType( GetTypeBySizedFormat( sizedFormat ) );
	SetSubTextureData( imageFormat, imageType, xOffset, yOffset, zOffset, w, h, d );
}

void Texture::SetSubTextureDataUsePBO( ImageFormat imageFormat, ImageType imageType, int xOffset, int yOffset,
									   int zOffset, int w, int h, int d )
{
	assert( bufferObject );
	assert( bufferObject->Handle() );

	GL( glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
	GL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, bufferObject->Handle() ) );
	SetSubTextureData( nullptr, imageFormat, imageType, xOffset, yOffset, zOffset, w, h, d );
	GL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 ) );
	GL( glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
}

void Texture::SetSubTextureDataUsePBO( int xOffset, int yOffset, int zOffset, int w, int h, int d )
{
	assert( bufferObject );
	const auto sizedFormat = createParams->GetTextureFormat();
	const auto imageFormat = ImageFormat( GetBaseFormatBySizedFormat( sizedFormat ) );
	const auto imageType = ImageType( GetTypeBySizedFormat( sizedFormat ) );
	SetSubTextureDataUsePBO( imageFormat, imageType, xOffset, yOffset, zOffset, w, h, d );
}

VMRef<Texture> MakeVolumeTexture( const std::string &fileName, size_t x, size_t y, size_t z )
{
	VMRef<TexCreateParams> params = MakeVMRef<TexCreateParams>();

	RawReader rawReader( fileName, { x, y, z }, sizeof( char ) );
	std::unique_ptr<char[]> buffer( new char[ x * y * z * sizeof( char ) ] );
	if ( x * y * z != rawReader.readRegion( { 0, 0, 0 }, { x, y, z }, (unsigned char *)buffer.get() ) ) {
		throw std::runtime_error( "Size read error" );
	}

	auto texBuffer = MakeVMRef<BufferObject>();

	texBuffer->SetLocalData( buffer.get(), size_t( x ) * y * z );

	params->EnableMipMap( false );
	params->EnableBorder( false );
	params->SetSize( x, y, z );
	params->SetTextureFormat( TF_R8 );
	params->SetTextureTarget( TD_TEXTURE_3D );
	params->SetBufferObject( texBuffer );
	texBuffer->CreateBufferObject();

	texBuffer->ReallocBufferData( BU_STREAM_DRAW, true );

	auto volumeTex = MakeVMRef<Texture>();
	volumeTex->SetSetupParams( params );
	if ( !volumeTex->CreateTexture() ) {
		return nullptr;
	}
	//volumeTex->SetSubTextureData(buffer.get(), IF_RED, IT_UNSIGNED_BYTE, 0, 0, 0, x, y, z);
	//volumeTex->SetSubTextureData(buffer.get(), 0, 0, 0, x, y, z);
	//volumeTex->SetSubTextureData(0, 0, 0, x, y, z);
	volumeTex->SetSubTextureDataUsePBO( 0, 0, 0, x, y, z );
	return volumeTex;
}

VMRef<Texture> MakeTransferFunction1DTexture( const std::string &fileName )
{
	VMRef<TexCreateParams> params = MakeVMRef<TexCreateParams>();
	params->EnableMipMap( false );
	params->EnableBorder( false );
	params->SetSize( 256, 0, 0 );
	params->SetTextureFormat( TF_RGBA32F );
	params->SetTextureTarget( TD_TEXTURE_1D );
	auto tfTex = MakeVMRef<Texture>();
	tfTex->SetSetupParams( params );
	if ( !tfTex->CreateTexture() ) {
		return nullptr;
	}
	ColorInterpulator interpulator( fileName );
	if ( !interpulator.valid() ) {
		throw std::runtime_error( "Return Color Table error" );
	}
	std::unique_ptr<float[]> buffer( new float[ 256 * 4 ] );
	interpulator.FetchData( (float *)( buffer.get() ), 256 );
	tfTex->SetSubTextureData( buffer.get(), IF_RGBA, IT_FLOAT, 0, 0, 0, 256, 0, 0 );
	return tfTex;
}

VMRef<Texture> MakePreIntegratedTransferFunction2DTexture( const std::string &fileName )
{
	VMRef<TexCreateParams> params = MakeVMRef<TexCreateParams>();
	params->EnableMipMap( false );
	params->EnableBorder( false );
	params->SetSize( 256, 256, 0 );
	params->SetTextureFormat( TF_RGBA32F );
	params->SetTextureTarget( TD_TEXTURE_2D );
	auto tfTex = MakeVMRef<Texture>();
	tfTex->SetSetupParams( params );
	if ( !tfTex->CreateTexture() ) {
		return nullptr;
	}

	const ColorInterpulator interpulator( fileName );
	if ( !interpulator.valid() ) {
		throw std::runtime_error( "Return Color Table error" );
	}
	const std::unique_ptr<float[]> buffer( new float[ 256 * 4 ] );
	interpulator.FetchData( reinterpret_cast<float *>( buffer.get() ), 256 );

	const int dimension = 256;
	const std::unique_ptr<float[]> preIntTransferFunction( new float[ dimension * dimension * 4 ] );


	{

		::vm::println( "Calculating pre-integrated transfer function..." );
		::vm::Timer::Scoped _( [&]( auto dt ) {
			::vm::println( "Time Consuming: {}", dt.s() );
		} );
		/*
	 * Taking alpha in transfer function as t * deltaL, based on code by Klaus Engel
	 */
		float rayStep = 1.0;
		for ( int sb = 0; sb < 256; ++sb ) {
			for ( int sf = 0; sf <= sb; ++sf ) {
				const int offset = sf != sb;			// 0 for the same point
				const int n = 20 + 1 * abs( sb - sf );  // sampling number
				const float stepWidth = rayStep / n;	// sampling step
				float rgba[ 4 ] = { 0, 0, 0, 0 };		// accumulated color

				for ( int i = 0; i < n; ++i ) {
					const float s = sf + ( sb - sf ) * (float)i / n;																					   // assume each subinterval with equal values
					const float sFrac = s - floor( s );																									   // the fraction part of s in [floor(s), floor(s) + 1]
					const float opacity = ( buffer[ int( s ) * 4 + 3 ] * ( 1.0 - sFrac ) + buffer[ ( int( s ) + offset ) * 4 + 3 ] * sFrac ) * stepWidth;  // linear interpolation of absorption coefficient in the exponent (ray segment)

					const float temp = std::exp( -rgba[ 3 ] ) * opacity;																	 // visibility mulitiplied alpha
					rgba[ 0 ] += ( buffer[ int( s ) * 4 + 0 ] * ( 1.0 - sFrac ) + buffer[ ( int( s ) + offset ) * 4 + 0 ] * sFrac ) * temp;  // associated color for the i-th ray segment
					rgba[ 1 ] += ( buffer[ int( s ) * 4 + 1 ] * ( 1.0 - sFrac ) + buffer[ ( int( s ) + offset ) * 4 + 1 ] * sFrac ) * temp;
					rgba[ 2 ] += ( buffer[ int( s ) * 4 + 2 ] * ( 1.0 - sFrac ) + buffer[ ( int( s ) + offset ) * 4 + 2 ] * sFrac ) * temp;
					rgba[ 3 ] += opacity;  // the exponent part of this ray segment
				}

				preIntTransferFunction[ ( sf * dimension + sb ) * 4 + 0 ] = preIntTransferFunction[ ( sb * dimension + sf ) * 4 + 0 ] = rgba[ 0 ];
				preIntTransferFunction[ ( sf * dimension + sb ) * 4 + 1 ] = preIntTransferFunction[ ( sb * dimension + sf ) * 4 + 1 ] = rgba[ 1 ];
				preIntTransferFunction[ ( sf * dimension + sb ) * 4 + 2 ] = preIntTransferFunction[ ( sb * dimension + sf ) * 4 + 2 ] = rgba[ 2 ];
				preIntTransferFunction[ ( sf * dimension + sb ) * 4 + 3 ] = preIntTransferFunction[ ( sb * dimension + sf ) * 4 + 3 ] = 1.0 - exp( -rgba[ 3 ] );
			}
		}
		tfTex->SetSubTextureData( preIntTransferFunction.get(), IF_RGBA, IT_FLOAT, 0, 0, 0, 256, 256, 0 );
	}

	return tfTex;
}

VMRef<Texture> MakeTransferFunction1DTexture( const std::vector<Color> &colors )
{
	VMRef<TexCreateParams> params = MakeVMRef<TexCreateParams>();
	params->EnableMipMap( false );
	params->EnableBorder( false );
	params->SetSize( 256, 0, 0 );
	params->SetTextureFormat( TF_RGBA32F );
	params->SetTextureTarget( TD_TEXTURE_1D );
	auto tfTex = MakeVMRef<Texture>();
	tfTex->SetSetupParams( params );

	if ( !tfTex->CreateTexture() ) {
		return nullptr;
	}
	ColorInterpulator interpulator;

	for ( int i = 0; i < colors.size(); i++ )
		interpulator.AddColorKey( i * 1.0 / colors.size(), colors[ i ] );
	interpulator.Sort();
	std::unique_ptr<float[]> buffer( new float[ 256 * 4 ] );
	interpulator.FetchData( ( buffer.get() ), 256 );
	tfTex->SetSubTextureData( buffer.get(), IF_RGBA, IT_FLOAT, 0, 0, 0, 256, 0, 0 );
	return tfTex;
}
}  // namespace vm
