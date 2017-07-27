﻿#include "SpecularGlossinessPassBuffer.hpp"

#include "Material/LegacyPass.hpp"
#include "Material/SpecularGlossinessPbrPass.hpp"

using namespace Castor;

namespace Castor3D
{
	namespace
	{
#if GLSL_MATERIALS_STRUCT_OF_ARRAY

		SpecularGlossinessPassBuffer::PassesData DoBindData( CpuBuffer< uint8_t > & buffer
			, uint32_t count )
		{
			auto data = buffer.GetData();
			auto diffDiv = make_array_view( reinterpret_cast< PassBuffer::RgbaColour * >( data )
				, reinterpret_cast< PassBuffer::RgbaColour * >( data ) + count );
			data += sizeof( PassBuffer::RgbaColour ) * count;
			auto specGloss = make_array_view( reinterpret_cast< PassBuffer::RgbaColour * >( data )
				, reinterpret_cast< PassBuffer::RgbaColour * >( data ) + count );
			data += sizeof( PassBuffer::RgbaColour ) * count;
			auto common = make_array_view( reinterpret_cast< PassBuffer::RgbaColour * >( data )
				, reinterpret_cast< PassBuffer::RgbaColour * >( data ) + count );
			data += sizeof( PassBuffer::RgbaColour ) * count;
			auto reflRefr = make_array_view( reinterpret_cast< PassBuffer::RgbaColour * >( data )
				, reinterpret_cast< PassBuffer::RgbaColour * >( data ) + count );
			data += sizeof( PassBuffer::RgbaColour ) * count;
			return
			{
				diffDiv,
				specGloss,
				common,
				reflRefr,
			};
		}

#else

		SpecularGlossinessPassBuffer::PassesData DoBindData( CpuBuffer< uint8_t > & buffer
			, uint32_t count )
		{
			auto data = buffer.GetData();
			return make_array_view( reinterpret_cast< SpecularGlossinessPassBuffer::PassData * >( data )
				, reinterpret_cast< SpecularGlossinessPassBuffer::PassData * >( data ) + count );
		}

#endif
	}

	//*********************************************************************************************

	SpecularGlossinessPassBuffer::SpecularGlossinessPassBuffer( Engine & engine
		, uint32_t count )
		: PassBuffer{ engine, count, DataSize }
		, m_data{ DoBindData( m_buffer, count ) }
	{
	}

	SpecularGlossinessPassBuffer::~SpecularGlossinessPassBuffer()
	{
	}

	void SpecularGlossinessPassBuffer::Visit( LegacyPass const & pass )
	{
		REQUIRE( pass.GetId() > 0 );
		auto index = pass.GetId() - 1;

#if GLSL_MATERIALS_STRUCT_OF_ARRAY

		m_data.diffDiv[index].r = pass.GetDiffuse().red();
		m_data.diffDiv[index].g = pass.GetDiffuse().green();
		m_data.diffDiv[index].b = pass.GetDiffuse().blue();
		m_data.specGloss[index].r = pass.GetSpecular().red();
		m_data.specGloss[index].g = pass.GetSpecular().green();
		m_data.specGloss[index].b = pass.GetSpecular().blue();
		m_data.specGloss[index].a = pass.GetShininess();
		m_data.common[index].r = pass.GetOpacity();
		m_data.common[index].g = pass.GetEmissive();
		m_data.common[index].b = pass.GetAlphaValue();
		m_data.common[index].a = pass.NeedsGammaCorrection() ? 2.2f : 1.0f;
		m_data.reflRefr[index].r = pass.GetRefractionRatio();
		m_data.reflRefr[index].g = CheckFlag( pass.GetTextureFlags(), TextureChannel::eRefraction ) ? 1.0f : 0.0f;
		m_data.reflRefr[index].b = CheckFlag( pass.GetTextureFlags(), TextureChannel::eReflection ) ? 1.0f : 0.0f;
		m_data.reflRefr[index].a = 1.0f;

#else

		m_data[index].diffDiv.r = pass.GetDiffuse().red();
		m_data[index].diffDiv.g = pass.GetDiffuse().green();
		m_data[index].diffDiv.b = pass.GetDiffuse().blue();
		m_data[index].specGloss.r = pass.GetSpecular().red();
		m_data[index].specGloss.g = pass.GetSpecular().green();
		m_data[index].specGloss.b = pass.GetSpecular().blue();
		m_data[index].specGloss.a = pass.GetShininess();
		m_data[index].common.r = pass.GetOpacity();
		m_data[index].common.g = pass.GetEmissive();
		m_data[index].common.b = pass.GetAlphaValue();
		m_data[index].common.a = pass.NeedsGammaCorrection() ? 2.2f : 1.0f;
		m_data[index].reflRefr.r = pass.GetRefractionRatio();
		m_data[index].reflRefr.g = CheckFlag( pass.GetTextureFlags(), TextureChannel::eRefraction ) ? 1.0f : 0.0f;
		m_data[index].reflRefr.b = CheckFlag( pass.GetTextureFlags(), TextureChannel::eReflection ) ? 1.0f : 0.0f;
		m_data[index].reflRefr.a = 1.0f;

#endif
	}

	void SpecularGlossinessPassBuffer::Visit( SpecularGlossinessPbrPass const & pass )
	{
		REQUIRE( pass.GetId() > 0 );
		auto index = pass.GetId() - 1;

#if GLSL_MATERIALS_STRUCT_OF_ARRAY

		m_data.diffDiv[index].r = pass.GetDiffuse().red();
		m_data.diffDiv[index].g = pass.GetDiffuse().green();
		m_data.diffDiv[index].b = pass.GetDiffuse().blue();
		m_data.specGloss[index].r = pass.GetSpecular().red();
		m_data.specGloss[index].g = pass.GetSpecular().green();
		m_data.specGloss[index].b = pass.GetSpecular().blue();
		m_data.specGloss[index].a = pass.GetGlossiness();
		m_data.common[index].r = pass.GetOpacity();
		m_data.common[index].g = pass.GetEmissive();
		m_data.common[index].b = pass.GetAlphaValue();
		m_data.common[index].a = pass.NeedsGammaCorrection() ? 2.2f : 1.0f;
		m_data.reflRefr[index].r = pass.GetRefractionRatio();
		m_data.reflRefr[index].g = CheckFlag( pass.GetTextureFlags(), TextureChannel::eRefraction ) ? 1.0f : 0.0f;
		m_data.reflRefr[index].b = CheckFlag( pass.GetTextureFlags(), TextureChannel::eReflection ) ? 1.0f : 0.0f;
		m_data.reflRefr[index].a = 1.0f;

#else

		m_data[index].diffDiv.r = pass.GetDiffuse().red();
		m_data[index].diffDiv.g = pass.GetDiffuse().green();
		m_data[index].diffDiv.b = pass.GetDiffuse().blue();
		m_data[index].specGloss.r = pass.GetSpecular().red();
		m_data[index].specGloss.g = pass.GetSpecular().green();
		m_data[index].specGloss.b = pass.GetSpecular().blue();
		m_data[index].specGloss.a = pass.GetGlossiness();
		m_data[index].common.r = pass.GetOpacity();
		m_data[index].common.g = pass.GetEmissive();
		m_data[index].common.b = pass.GetAlphaValue();
		m_data[index].common.a = pass.NeedsGammaCorrection() ? 2.2f : 1.0f;
		m_data[index].reflRefr.r = pass.GetRefractionRatio();
		m_data[index].reflRefr.g = CheckFlag( pass.GetTextureFlags(), TextureChannel::eRefraction ) ? 1.0f : 0.0f;
		m_data[index].reflRefr.b = CheckFlag( pass.GetTextureFlags(), TextureChannel::eReflection ) ? 1.0f : 0.0f;
		m_data[index].reflRefr.a = 1.0f;

#endif
	}
}