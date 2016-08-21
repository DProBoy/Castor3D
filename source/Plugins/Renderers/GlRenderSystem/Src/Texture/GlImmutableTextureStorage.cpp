#include "GlImmutableTextureStorage.hpp"

#include "Common/OpenGl.hpp"
#include "Texture/GlTexture.hpp"
#include "Texture/GlTextureStorage.hpp"

#include <Graphics/PixelBufferBase.hpp>

#include <Texture/TextureImage.hpp>

using namespace Castor3D;
using namespace Castor;

namespace GlRender
{
	GlImmutableTextureStorageTraits::GlImmutableTextureStorageTraits( TextureStorage & p_storage )
	{
		auto l_buffer = p_storage.GetOwner()->GetBuffer();
		Fill( p_storage, l_buffer->const_ptr(), l_buffer->dimensions(), l_buffer->format() );
	}

	GlImmutableTextureStorageTraits::~GlImmutableTextureStorageTraits()
	{
	}

	bool GlImmutableTextureStorageTraits::Bind( TextureStorage const & p_storage, uint32_t p_index )const
	{
		return true;
	}

	void GlImmutableTextureStorageTraits::Unbind( TextureStorage const & p_storage, uint32_t p_index )const
	{
	}

	uint8_t * GlImmutableTextureStorageTraits::Lock( TextureStorage & p_storage, uint32_t p_lock )
	{
		uint8_t * l_return = nullptr;

		if ( ( p_storage.GetCPUAccess() && p_lock & eACCESS_TYPE_READ ) == eACCESS_TYPE_READ
				|| ( p_storage.GetCPUAccess() & p_lock & eACCESS_TYPE_WRITE ) == eACCESS_TYPE_WRITE )
		{
			l_return = p_storage.GetOwner()->GetBuffer()->ptr();
		}

		return l_return;
	}

	void GlImmutableTextureStorageTraits::Unlock( TextureStorage & p_storage, bool p_modified )
	{
		if ( p_modified && ( p_storage.GetCPUAccess() & eACCESS_TYPE_WRITE ) == eACCESS_TYPE_WRITE )
		{
			auto l_buffer = p_storage.GetOwner()->GetBuffer();
			DoUploadImage( p_storage, l_buffer->dimensions().width(), l_buffer->dimensions().height(), l_buffer->format(), l_buffer->const_ptr() );
		}
	}

	void GlImmutableTextureStorageTraits::Fill( TextureStorage & p_storage, uint8_t const * p_buffer, Castor::Size const & p_size, Castor::PixelFormat p_format )
	{
		auto & l_storage = static_cast< GlTextureStorage< GlImmutableTextureStorageTraits > & >( p_storage );
		OpenGl::PixelFmt l_glPixelFmt = l_storage.GetOpenGl().Get( p_format );

		switch ( l_storage.GetGlType() )
		{
		case eGL_TEXTURE_STORAGE_1D:
			l_storage.GetOpenGl().TexStorage1D( l_storage.GetGlType(), 4, l_glPixelFmt.Internal, p_size.width() );
			break;

		case eGL_TEXTURE_STORAGE_2D:
			l_storage.GetOpenGl().TexStorage2D( l_storage.GetGlType(), 4, l_glPixelFmt.Internal, p_size.width(), p_size.height() );
			break;

		case eGL_TEXTURE_STORAGE_2DMS:
			l_storage.GetOpenGl().TexStorage2DMultisample( l_storage.GetGlType(), 8, l_glPixelFmt.Internal, p_size.width(), p_size.height(), true );
			break;

		case eGL_TEXTURE_STORAGE_3D:
		case eGL_TEXTURE_STORAGE_2DARRAY:
			l_storage.GetOpenGl().TexStorage3D( l_storage.GetGlType(), 4, l_glPixelFmt.Internal, p_size.width(), p_size.height() / p_storage.GetOwner()->GetDepth(), p_storage.GetOwner()->GetDepth() );
			break;

		default:
			FAILURE( "Storage type unsupported for immutable storages" );
			CASTOR_EXCEPTION( cuT( "Storage type unsupported for immutable storages" ) );
			break;
		}

		DoUploadImage( p_storage, p_size.width(), p_size.height(), p_format, p_buffer );
	}

	void GlImmutableTextureStorageTraits::DoUploadImage( TextureStorage & p_storage, uint32_t p_width, uint32_t p_height, Castor::PixelFormat p_format, uint8_t const * p_buffer )
	{
		auto & l_storage = static_cast< GlTextureStorage< GlImmutableTextureStorageTraits > & >( p_storage );
		OpenGl::PixelFmt l_format = l_storage.GetOpenGl().Get( p_format );

		switch ( l_storage.GetGlType() )
		{
		case eGL_TEXTURE_STORAGE_1D:
			l_storage.GetOpenGl().TexSubImage1D( l_storage.GetGlType(), 0, 0, p_width, l_format.Format, l_format.Type, p_buffer );
			break;

		case eGL_TEXTURE_STORAGE_2D:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_POSX:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_NEGX:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_POSY:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_NEGY:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_POSZ:
		case eGL_TEXTURE_STORAGE_CUBE_MAP_FACE_NEGZ:
			l_storage.GetOpenGl().TexSubImage2D( l_storage.GetGlType(), 0, 0, 0, p_width, p_height, l_format.Format, l_format.Type, p_buffer );
			break;

		case eGL_TEXTURE_STORAGE_2DMS:
			l_storage.GetOpenGl().TexSubImage2D( l_storage.GetGlType(), 0, 0, 0, p_width, p_height, l_format.Format, l_format.Type, p_buffer );
			break;

		case eGL_TEXTURE_STORAGE_3D:
		case eGL_TEXTURE_STORAGE_2DARRAY:
			l_storage.GetOpenGl().TexSubImage3D( l_storage.GetGlType(), 0, 0, 0, 0, p_width, p_height / p_storage.GetOwner()->GetDepth(), p_storage.GetOwner()->GetDepth(), l_format.Format, l_format.Type, p_buffer );
			break;
		}
	}
}
