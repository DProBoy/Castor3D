#include "ShadowMapDirectional.hpp"

#include "Engine.hpp"

#include "FrameBuffer/FrameBuffer.hpp"
#include "FrameBuffer/TextureAttachment.hpp"
#include "Render/RenderPipeline.hpp"
#include "Render/RenderSystem.hpp"
#include "Scene/Light/Light.hpp"
#include "Scene/Light/DirectionalLight.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Shader/UniformBuffer.hpp"
#include "ShadowMap/ShadowMapPassDirectional.hpp"
#include "Texture/Sampler.hpp"
#include "Texture/TextureImage.hpp"
#include "Texture/TextureLayout.hpp"

#include <GlslSource.hpp>

#include <Graphics/Image.hpp>

using namespace Castor;

namespace Castor3D
{
	namespace
	{
		void DoInitialiseShadowMap( Engine & p_engine, Size const & p_size, TextureUnit & p_unit )
		{
			auto l_sampler = p_engine.GetSamplerCache().Add( cuT( "ShadowMap_Directional" ) );
			l_sampler->SetInterpolationMode( InterpolationFilter::eMin, InterpolationMode::eLinear );
			l_sampler->SetInterpolationMode( InterpolationFilter::eMag, InterpolationMode::eLinear );
			l_sampler->SetWrappingMode( TextureUVW::eU, WrapMode::eClampToBorder );
			l_sampler->SetWrappingMode( TextureUVW::eV, WrapMode::eClampToBorder );
			l_sampler->SetWrappingMode( TextureUVW::eW, WrapMode::eClampToBorder );
			l_sampler->SetComparisonMode( ComparisonMode::eRefToTexture );
			l_sampler->SetComparisonFunc( ComparisonFunc::eLEqual );

			auto l_texture = p_engine.GetRenderSystem()->CreateTexture(
				TextureType::eTwoDimensions,
				AccessType::eNone,
				AccessType::eRead | AccessType::eWrite,
				PixelFormat::eD32F, p_size );
			p_unit.SetTexture( l_texture );
			p_unit.SetSampler( l_sampler );

			for ( auto & l_image : *l_texture )
			{
				l_image->InitialiseSource();
			}

			p_unit.Initialise();
		}
	}

	ShadowMapDirectional::ShadowMapDirectional( Engine & p_engine )
		: ShadowMap{ p_engine }
	{
	}

	ShadowMapDirectional::~ShadowMapDirectional()
	{
	}

	bool ShadowMapDirectional::DoInitialise( Size const & p_size )
	{
		DoInitialiseShadowMap( *GetEngine(), p_size, m_shadowMap );

		auto l_texture = m_shadowMap.GetTexture();
		m_depthAttach = m_frameBuffer->CreateAttachment( l_texture );
		bool l_return{ false };

		m_frameBuffer->Bind( FrameBufferMode::eConfig );
		m_frameBuffer->Attach( AttachmentPoint::eDepth, 0, m_depthAttach, l_texture->GetType() );
		l_return = m_frameBuffer->IsComplete();
		m_frameBuffer->Unbind();

		m_frameBuffer->SetClearColour( Colour::from_predef( PredefinedColour::eOpaqueBlack ) );
		return l_return;
	}

	void ShadowMapDirectional::DoCleanup()
	{
		m_frameBuffer->Bind( FrameBufferMode::eConfig );
		m_frameBuffer->DetachAll();
		m_frameBuffer->Unbind();
		m_depthAttach.reset();
	}

	ShadowMapPassSPtr ShadowMapDirectional::DoCreatePass( Light & p_light )const
	{
		return std::make_shared< ShadowMapPassDirectional >( *GetEngine(), p_light, *this );
	}

	void ShadowMapDirectional::DoUpdateFlags( TextureChannels & p_textureFlags
		, ProgramFlags & p_programFlags )const
	{
		AddFlag( p_programFlags, ProgramFlag::eShadowMapDirectional );
	}

	String ShadowMapDirectional::DoGetPixelShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, uint8_t p_sceneFlags )const
	{
		using namespace GLSL;
		GlslWriter l_writer = GetEngine()->GetRenderSystem()->CreateGlslWriter();

		// Fragment Intputs
		auto vtx_texture = l_writer.GetInput< Vec3 >( cuT( "vtx_texture" ) );
		auto c3d_mapOpacity( l_writer.GetUniform< Sampler2D >( ShaderProgram::MapOpacity, CheckFlag( p_textureFlags, TextureChannel::eOpacity ) ) );
		auto gl_FragCoord( l_writer.GetBuiltin< Vec4 >( cuT( "gl_FragCoord" ) ) );

		// Fragment Outputs
		auto pxl_fFragDepth( l_writer.GetFragData< Float >( cuT( "pxl_fFragDepth" ), 0 ) );

		l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
		{
			Float l_fAlpha;

			if ( CheckFlag( p_textureFlags, TextureChannel::eOpacity ) )
			{
				l_fAlpha = l_writer.GetLocale( cuT( "l_fAlpha" ), texture( c3d_mapOpacity, vtx_texture.xy() ).r() );

				IF( l_writer, l_fAlpha < 1.0_f )
				{
					l_writer.Discard();
				}
				FI;
			}
			else
			{
				pxl_fFragDepth = gl_FragCoord.z();
			}
		} );

		return l_writer.Finalise();
	}
}
