#include "TextureStorage.hpp"

#include "Engine.hpp"

#include "Render/RenderSystem.hpp"

using namespace Castor;

namespace Castor3D
{
	TextureStorage::TextureStorage( eTEXTURE_BASE_TYPE p_baseType, RenderSystem & p_renderSystem, uint8_t p_cpuAccess, uint8_t p_gpuAccess )
		: OwnedBy< RenderSystem >( p_renderSystem )
		, m_initialised( false )
		, m_baseType( p_baseType )
		, m_type( eTEXTURE_TYPE_2D )
		, m_mapMode( eTEXTURE_MAP_MODE_NONE )
		, m_cpuAccess( p_cpuAccess )
		, m_gpuAccess( p_gpuAccess )
	{
	}

	TextureStorage::~TextureStorage()
	{
	}

	void TextureStorage::SetImage( Castor::PxBufferBaseSPtr p_buffer )
	{
		m_depth = 1;
		m_pixelBuffer = p_buffer;
	}

	bool TextureStorage::Bind( uint32_t p_index )const
	{
		bool l_return = false;

		if ( m_initialised )
		{
			l_return = DoBind( p_index );
		}

		return l_return;
	}

	void TextureStorage::Unbind( uint32_t p_index )const
	{
		DoUnbind( p_index );
	}
}
