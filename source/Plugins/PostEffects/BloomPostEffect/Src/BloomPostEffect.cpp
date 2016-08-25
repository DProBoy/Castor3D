#include "BloomPostEffect.hpp"

#include <Engine.hpp>
#include <SamplerCache.hpp>
#include <ShaderCache.hpp>

#include <FrameBuffer/BackBuffers.hpp>
#include <FrameBuffer/FrameBufferAttachment.hpp>
#include <FrameBuffer/TextureAttachment.hpp>
#include <Mesh/Vertex.hpp>
#include <Mesh/Buffer/BufferDeclaration.hpp>
#include <Mesh/Buffer/BufferElementDeclaration.hpp>
#include <Mesh/Buffer/GeometryBuffers.hpp>
#include <Mesh/Buffer/VertexBuffer.hpp>
#include <Miscellaneous/Parameter.hpp>
#include <Render/Context.hpp>
#include <Render/Pipeline.hpp>
#include <Render/RenderSystem.hpp>
#include <Render/RenderTarget.hpp>
#include <Render/RenderWindow.hpp>
#include <Render/Viewport.hpp>
#include <Shader/FrameVariableBuffer.hpp>
#include <Shader/OneFrameVariable.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Texture/Sampler.hpp>
#include <Texture/TextureLayout.hpp>
#include <Texture/TextureUnit.hpp>

#include <numeric>

#include <GlslSource.hpp>

using namespace Castor;
using namespace Castor3D;

namespace Bloom
{
	namespace
	{
		static const String FilterConfig = cuT( "FilterConfig" );
		static const String FilterConfigCoefficients = cuT( "c3d_fCoefficients" );
		static const String FilterConfigOffsetX = cuT( "c3d_fOffsetX" );
		static const String FilterConfigOffsetY = cuT( "c3d_fOffsetY" );
		static const String CombineMapPass0 = cuT( "c3d_mapPass0" );
		static const String CombineMapPass1 = cuT( "c3d_mapPass1" );
		static const String CombineMapPass2 = cuT( "c3d_mapPass2" );
		static const String CombineMapPass3 = cuT( "c3d_mapPass3" );
		static const String CombineMapScene = cuT( "c3d_mapScene" );

		Castor::String GetVertexProgram( RenderSystem * p_renderSystem )
		{
			using namespace GLSL;
			GlslWriter l_writer = p_renderSystem->CreateGlslWriter();

			UBO_MATRIX( l_writer );

			// Shader inputs
			Vec2 position = l_writer.GetAttribute< Vec2 >( ShaderProgram::Position );

			// Shader outputs
			auto vtx_texture = l_writer.GetOutput< Vec2 >( cuT( "vtx_texture" ) );
			auto gl_Position = l_writer.GetBuiltin< Vec4 >( cuT( "gl_Position" ) );

			l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
			{
				vtx_texture = position;
				gl_Position = c3d_mtxProjection * vec4( position.SWIZZLE_XY, 0.0, 1.0 );
			} );
			return l_writer.Finalise();
		}

		Castor::String GetHiPassProgram( RenderSystem * p_renderSystem )
		{
			using namespace GLSL;
			GlslWriter l_writer = p_renderSystem->CreateGlslWriter();

			// Shader inputs
			auto c3d_mapDiffuse = l_writer.GetUniform< Sampler2D >( ShaderProgram::MapDiffuse );
			auto vtx_texture = l_writer.GetInput< Vec2 >( cuT( "vtx_texture" ) );

			// Shader outputs
			auto plx_v4FragColor = l_writer.GetFragData< Vec4 >( cuT( "plx_v4FragColor" ), 0 );

			l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
			{
				plx_v4FragColor = vec4( texture2D( c3d_mapDiffuse, vec2( vtx_texture.SWIZZLE_X, vtx_texture.SWIZZLE_Y ) ).SWIZZLE_XYZ, 1.0 );

				plx_v4FragColor.SWIZZLE_X = TERNARY( l_writer, Float, plx_v4FragColor.SWIZZLE_X > 1.0, Float( 1 ), Float( 0 ) );
				plx_v4FragColor.SWIZZLE_Y = TERNARY( l_writer, Float, plx_v4FragColor.SWIZZLE_Y > 1.0, Float( 1 ), Float( 0 ) );
				plx_v4FragColor.SWIZZLE_Z = TERNARY( l_writer, Float, plx_v4FragColor.SWIZZLE_Z > 1.0, Float( 1 ), Float( 0 ) );
			} );
			return l_writer.Finalise();
		}

		Castor::String GetBlurProgram( RenderSystem * p_renderSystem )
		{
			using namespace GLSL;
			GlslWriter l_writer = p_renderSystem->CreateGlslWriter();

			// Shader inputs
			auto c3d_mapDiffuse = l_writer.GetUniform< Sampler2D >( ShaderProgram::MapDiffuse );
			auto vtx_texture = l_writer.GetInput< Vec2 >( cuT( "vtx_texture" ) );

			Ubo l_config{ l_writer, FilterConfig };
			auto c3d_fCoefficients = l_config.GetUniform< Float >( FilterConfigCoefficients, 3u );
			auto c3d_fOffsetX = l_config.GetUniform< Float >( FilterConfigOffsetX );
			auto c3d_fOffsetY = l_config.GetUniform< Float >( FilterConfigOffsetY );
			l_config.End();

			// Shader outputs
			auto plx_v4FragColor = l_writer.GetFragData< Vec4 >( cuT( "plx_v4FragColor" ), 0 );

			l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
			{
				LOCALE_ASSIGN( l_writer, Vec2, l_offset, vec2( c3d_fOffsetX, c3d_fOffsetY ) );
				plx_v4FragColor = c3d_fCoefficients[0] * texture2D( c3d_mapDiffuse, vtx_texture );

				FOR( l_writer, Int, i, 0, cuT( "i < 3" ), cuT( "++i" ) )
				{
					plx_v4FragColor += c3d_fCoefficients[i] * texture2D( c3d_mapDiffuse, vtx_texture - l_offset );
					plx_v4FragColor += c3d_fCoefficients[i] * texture2D( c3d_mapDiffuse, vtx_texture + l_offset );
				}
				ROF;
			} );
			return l_writer.Finalise();
		}

		Castor::String GetCombineProgram( RenderSystem * p_renderSystem )
		{
			using namespace GLSL;
			GlslWriter l_writer = p_renderSystem->CreateGlslWriter();

			// Shader inputs
			auto c3d_mapPass0 = l_writer.GetUniform< Sampler2D >( CombineMapPass0 );
			auto c3d_mapPass1 = l_writer.GetUniform< Sampler2D >( CombineMapPass1 );
			auto c3d_mapPass2 = l_writer.GetUniform< Sampler2D >( CombineMapPass2 );
			auto c3d_mapPass3 = l_writer.GetUniform< Sampler2D >( CombineMapPass3 );
			auto c3d_mapScene = l_writer.GetUniform< Sampler2D >( CombineMapScene );
			auto vtx_texture = l_writer.GetInput< Vec2 >( cuT( "vtx_texture" ) );

			// Shader outputs
			auto plx_v4FragColor = l_writer.GetFragData< Vec4 >( cuT( "plx_v4FragColor" ), 0 );

			l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
			{
				plx_v4FragColor  = texture2D( c3d_mapPass0, vtx_texture );
				plx_v4FragColor += texture2D( c3d_mapPass1, vtx_texture );
				plx_v4FragColor += texture2D( c3d_mapPass2, vtx_texture );
				plx_v4FragColor += texture2D( c3d_mapPass3, vtx_texture );
				plx_v4FragColor += texture2D( c3d_mapScene, vtx_texture );
			} );
			return l_writer.Finalise();
		}
	}
	//*********************************************************************************************

	String BloomPostEffect::Type = cuT( "bloom" );
	String BloomPostEffect::Name = cuT( "Bloom PostEffect" );

	BloomPostEffect::BloomPostEffect( RenderTarget & p_renderTarget, RenderSystem & p_renderSystem, Parameters const & p_param )
		: PostEffect( BloomPostEffect::Type, p_renderTarget, p_renderSystem, p_param )
		, m_viewport{ *p_renderSystem.GetEngine() }
		, m_offsetX( 1.2f )
		, m_offsetY( 1.2f )
		, m_kernel( { 5, 6, 5 } )
		, m_declaration(
		{
			BufferElementDeclaration( ShaderProgram::Position, uint32_t( ElementUsage::Position ), ElementType::Vec2 ),
		} )
		, m_hiPassSurfaces(
		{
			{
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() }
			}
		} )
		, m_blurSurfaces(
		{
			{
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() },
				PostEffectSurface{ *p_renderSystem.GetEngine() }
			}
		} )
	{
		String l_count;

		if ( p_param.Get( cuT( "OffsetX" ), l_count ) )
		{
			m_offsetX = string::to_float( l_count );
		}

		if ( p_param.Get( cuT( "OffsetX" ), l_count ) )
		{
			m_offsetY = string::to_float( l_count );
		}

		// Normalize kernel coefficients
		float l_sum = std::accumulate( std::begin( m_kernel ), std::end( m_kernel ), 0.0f );
		std::transform( std::begin( m_kernel ), std::end( m_kernel ), std::begin( m_kernel ), [&l_sum]( float & p_value )
		{
			return p_value /= l_sum;
		} );

		real l_pBuffer[] =
		{
			0, 0,
			1, 1,
			0, 1,
			0, 0,
			1, 0,
			1, 1,
		};

		std::memcpy( m_buffer, l_pBuffer, sizeof( l_pBuffer ) );
		uint32_t i = 0;

		for ( auto & l_vertex : m_vertices )
		{
			l_vertex = std::make_shared< BufferElementGroup >( &reinterpret_cast< uint8_t * >( m_buffer )[i++ * m_declaration.GetStride()] );
		}

		m_linearSampler = DoCreateSampler( true );
		m_nearestSampler = DoCreateSampler( false );

		DepthStencilState l_dsstate;
		l_dsstate.SetDepthTest( false );
		l_dsstate.SetDepthMask( WritingMask::Zero );
		m_pipeline = p_renderSystem.CreatePipeline( std::move( l_dsstate ), RasteriserState{}, BlendState{}, MultisampleState{} );
	}

	BloomPostEffect::~BloomPostEffect()
	{
	}

	PostEffectSPtr BloomPostEffect::Create( RenderTarget & p_renderTarget, RenderSystem & p_renderSystem, Parameters const & p_param )
	{
		return std::make_shared< BloomPostEffect >( p_renderTarget, p_renderSystem, p_param );
	}

	bool BloomPostEffect::Initialise()
	{
		bool l_return = false;
		m_viewport.Initialise();
		auto & l_cache = GetRenderSystem()->GetEngine()->GetShaderProgramCache();
		eSHADER_MODEL l_model = GetRenderSystem()->GetGpuInformations().GetMaxShaderModel();
		Size l_size = m_renderTarget.GetSize();
		String l_vertex;
		String l_hipass;
		String l_blur;
		String l_combine;

		l_vertex = GetVertexProgram( GetRenderSystem() );
		l_hipass = GetHiPassProgram( GetRenderSystem() );
		l_blur = GetBlurProgram( GetRenderSystem() );
		l_combine = GetCombineProgram( GetRenderSystem() );

		if ( !l_vertex.empty() && !l_hipass.empty() )
		{
			ShaderProgramSPtr l_program = l_cache.GetNewProgram();
			m_hiPassMapDiffuse = l_program->CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapDiffuse, ShaderType::Pixel );
			l_cache.CreateMatrixBuffer( *l_program, MASK_SHADER_TYPE_VERTEX );
			l_program->SetSource( ShaderType::Vertex, l_model, l_vertex );
			l_program->SetSource( ShaderType::Pixel, l_model, l_hipass );
			l_program->Initialise();
			m_hiPassProgram = l_program;
		}

		if ( !l_vertex.empty() && !l_blur.empty() )
		{
			ShaderProgramSPtr l_program = l_cache.GetNewProgram();
			m_filterMapDiffuse = l_program->CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapDiffuse, ShaderType::Pixel );
			auto l_filterConfig = GetRenderSystem()->CreateFrameVariableBuffer( FilterConfig );
			m_filterCoefficients = std::static_pointer_cast< OneFloatFrameVariable >( l_filterConfig->CreateVariable( *l_program, FrameVariableType::Float, FilterConfigCoefficients, KERNEL_SIZE ) );
			m_filterOffsetX = std::static_pointer_cast< OneFloatFrameVariable >( l_filterConfig->CreateVariable( *l_program, FrameVariableType::Float, FilterConfigOffsetX ) );
			m_filterOffsetY = std::static_pointer_cast< OneFloatFrameVariable >( l_filterConfig->CreateVariable( *l_program, FrameVariableType::Float, FilterConfigOffsetY ) );
			l_program->AddFrameVariableBuffer( l_filterConfig, MASK_SHADER_TYPE_PIXEL );
			l_cache.CreateMatrixBuffer( *l_program, MASK_SHADER_TYPE_VERTEX );

			l_program->SetSource( ShaderType::Vertex, l_model, l_vertex );
			l_program->SetSource( ShaderType::Pixel, l_model, l_blur );
			l_program->Initialise();
			m_filterProgram = l_program;
		}

		if ( !l_vertex.empty() && !l_combine.empty() )
		{
			ShaderProgramSPtr l_program = l_cache.GetNewProgram();
			l_program->CreateFrameVariable< OneIntFrameVariable >( CombineMapPass0, ShaderType::Pixel )->SetValue( 0 );
			l_program->CreateFrameVariable< OneIntFrameVariable >( CombineMapPass1, ShaderType::Pixel )->SetValue( 1 );
			l_program->CreateFrameVariable< OneIntFrameVariable >( CombineMapPass2, ShaderType::Pixel )->SetValue( 2 );
			l_program->CreateFrameVariable< OneIntFrameVariable >( CombineMapPass3, ShaderType::Pixel )->SetValue( 3 );
			l_program->CreateFrameVariable< OneIntFrameVariable >( CombineMapScene, ShaderType::Pixel )->SetValue( 4 );

			l_cache.CreateMatrixBuffer( *l_program, MASK_SHADER_TYPE_VERTEX );

			l_program->SetSource( ShaderType::Vertex, l_model, l_vertex );
			l_program->SetSource( ShaderType::Pixel, l_model, l_combine );
			l_program->Initialise();
			m_combineProgram = l_program;

			m_vertexBuffer = std::make_shared< VertexBuffer >( *GetRenderSystem()->GetEngine(), m_declaration );
			m_vertexBuffer->Resize( uint32_t( m_vertices.size() * m_declaration.GetStride() ) );
			m_vertexBuffer->LinkCoords( m_vertices.begin(), m_vertices.end() );
			m_vertexBuffer->Create();
			m_vertexBuffer->Initialise( BufferAccessType::Static, BufferAccessNature::Draw );
			m_geometryBuffers = GetRenderSystem()->CreateGeometryBuffers( Topology::Triangles, *l_program );
			m_geometryBuffers->Initialise( m_vertexBuffer, nullptr, nullptr, nullptr, nullptr );
		}

		uint32_t l_index = 0;

		for ( auto & l_surface : m_hiPassSurfaces )
		{
			l_surface.Initialise( m_renderTarget, l_size, l_index++, m_linearSampler );
			l_size.width() >>= 1;
			l_size.height() >>= 1;
		}

		l_size = m_renderTarget.GetSize();
		l_index = 0;

		for ( auto & l_surface : m_blurSurfaces )
		{
			l_surface.Initialise( m_renderTarget, l_size, l_index++, m_nearestSampler );
			l_size.width() >>= 1;
			l_size.height() >>= 1;
		}

		return l_return;
	}

	void BloomPostEffect::Cleanup()
	{
		m_hiPassMapDiffuse.reset();
		m_filterMapDiffuse.reset();
		m_filterCoefficients.reset();
		m_filterOffsetX.reset();
		m_filterOffsetY.reset();

		m_vertexBuffer->Cleanup();
		m_vertexBuffer->Destroy();
		m_vertexBuffer.reset();
		m_geometryBuffers->Cleanup();
		m_geometryBuffers.reset();

		for ( auto & l_surface : m_blurSurfaces )
		{
			l_surface.Cleanup();
		}

		for ( auto & l_surface : m_hiPassSurfaces )
		{
			l_surface.Cleanup();
		}

		m_viewport.Cleanup();
	}

	bool BloomPostEffect::Apply( FrameBuffer & p_framebuffer )
	{
		auto l_attach = p_framebuffer.GetAttachment( AttachmentPoint::Colour, 0 );

		if ( l_attach && l_attach->GetAttachmentType() == eATTACHMENT_TYPE_TEXTURE )
		{
			auto const & l_texture = *std::static_pointer_cast< TextureAttachment >( l_attach )->GetTexture();

			if ( DoHiPassFilter( l_texture ) )
			{
				DoDownSample( l_texture );
				DoBlur( l_texture, m_hiPassSurfaces, m_blurSurfaces, m_filterOffsetX, m_offsetX );
				DoBlur( l_texture, m_blurSurfaces, m_hiPassSurfaces, m_filterOffsetY, m_offsetY );
				DoCombine( l_texture );
			}

			if ( p_framebuffer.Bind( FrameBufferMode::Automatic, FrameBufferTarget::Draw ) )
			{
				GetRenderSystem()->GetCurrentContext()->RenderTexture( l_texture.GetImage().GetDimensions(), *m_blurSurfaces[0].m_colourTexture.GetTexture() );
				p_framebuffer.Unbind();
			}
		}

		return true;
	}

	bool BloomPostEffect::DoWriteInto( TextFile & p_file )
	{
		return p_file.WriteText( cuT( " -OffsetX " ) + string::to_string( m_offsetX ) + cuT( " -OffsetY " ) + string::to_string( m_offsetY ) ) > 0;
	}

	bool BloomPostEffect::DoHiPassFilter( TextureLayout const & p_origin )
	{
		auto l_source = &m_hiPassSurfaces[0];
		bool l_return = l_source->m_fbo->Bind( FrameBufferMode::Automatic, FrameBufferTarget::Draw );

		if ( l_return )
		{
			l_source->m_fbo->Clear();
			m_hiPassMapDiffuse->SetValue( 0 );
			GetRenderSystem()->GetCurrentContext()->RenderTexture( l_source->m_size, p_origin, m_hiPassProgram.lock() );
			l_source->m_fbo->Unbind();
		}

		return l_return;
	}

	void BloomPostEffect::DoDownSample( TextureLayout const & p_origin )
	{
		auto l_context = GetRenderSystem()->GetCurrentContext();
		auto l_source = &m_hiPassSurfaces[0];

		for ( uint32_t i = 1; i < m_hiPassSurfaces.size(); ++i )
		{
			auto l_destination = &m_hiPassSurfaces[i];

			if ( l_destination->m_fbo->Bind( FrameBufferMode::Automatic, FrameBufferTarget::Draw ) )
			{
				l_destination->m_fbo->Clear();
				l_context->RenderTexture( l_destination->m_size, *l_source->m_colourTexture.GetTexture() );
				l_destination->m_fbo->Unbind();
			}

			l_source = l_destination;
		}
	}

	void BloomPostEffect::DoBlur( TextureLayout const & p_origin, SurfaceArray & p_sources, SurfaceArray & p_destinations, Castor3D::OneFloatFrameVariableSPtr p_offset, float p_offsetValue )
	{
		auto l_context = GetRenderSystem()->GetCurrentContext();
		m_filterCoefficients->SetValues( m_kernel );
		m_filterOffsetX->SetValue( 0 );
		m_filterOffsetY->SetValue( 0 );

		for ( uint32_t i = 0; i < FILTER_COUNT; ++i )
		{
			auto l_source = &p_sources[i];
			auto l_destination = &p_destinations[i];

			if ( l_destination->m_fbo->Bind( FrameBufferMode::Automatic, FrameBufferTarget::Draw ) )
			{
				l_destination->m_fbo->Clear();
				p_offset->SetValue( p_offsetValue / l_source->m_size.width() );
				l_context->RenderTexture( l_source->m_size, *l_source->m_colourTexture.GetTexture(), m_filterProgram.lock() );
				l_destination->m_fbo->Unbind();
			}
		}
	}

	void BloomPostEffect::DoCombine( TextureLayout const & p_origin )
	{
		if ( m_blurSurfaces[0].m_fbo->Bind( FrameBufferMode::Automatic, FrameBufferTarget::Draw ) )
		{
			m_blurSurfaces[0].m_fbo->Clear();
			ShaderProgramSPtr l_program = m_combineProgram.lock();

			if ( l_program && l_program->GetStatus() == ePROGRAM_STATUS_LINKED )
			{
				m_viewport.Resize( p_origin.GetImage().GetDimensions() );
				m_viewport.Update();
				m_pipeline->SetProjectionMatrix( m_viewport.GetProjection() );
				m_pipeline->Apply();

				auto const & l_texture0 = m_hiPassSurfaces[0].m_colourTexture;
				auto const & l_texture1 = m_hiPassSurfaces[1].m_colourTexture;
				auto const & l_texture2 = m_hiPassSurfaces[2].m_colourTexture;
				auto const & l_texture3 = m_hiPassSurfaces[3].m_colourTexture;
				FrameVariableBufferSPtr l_matrices = l_program->FindFrameVariableBuffer( ShaderProgram::BufferMatrix );

				if ( l_matrices )
				{
					m_pipeline->ApplyProjection( *l_matrices );
				}

				l_program->Bind();

				l_texture0.Bind();
				l_texture1.Bind();
				l_texture2.Bind();
				l_texture3.Bind();
				p_origin.Bind( 4 );
				m_linearSampler->Bind( 4 );

				m_geometryBuffers->Draw( uint32_t( m_vertices.size() ), 0 );

				l_texture0.Unbind();
				l_texture1.Unbind();
				l_texture2.Unbind();
				l_texture3.Unbind();
				p_origin.Unbind( 4 );
				m_linearSampler->Unbind( 4 );

				l_program->Unbind();
			}

			m_blurSurfaces[0].m_fbo->Unbind();

			m_blurSurfaces[0].m_colourTexture.Bind();
			m_blurSurfaces[0].m_colourTexture.GetTexture()->GenerateMipmaps();
			m_blurSurfaces[0].m_colourTexture.Unbind();
		}
	}

	SamplerSPtr BloomPostEffect::DoCreateSampler( bool p_linear )
	{
		String l_name = cuT( "BloomSampler_" );
		InterpolationMode l_mode;

		if ( p_linear )
		{
			l_name += cuT( "Linear" );
			l_mode = InterpolationMode::Linear;
		}
		else
		{
			l_name += cuT( "Nearest" );
			l_mode = InterpolationMode::Nearest;
		}

		SamplerSPtr l_sampler;

		if ( !m_renderTarget.GetEngine()->GetSamplerCache().Has( l_name ) )
		{
			l_sampler = m_renderTarget.GetEngine()->GetSamplerCache().Add( l_name );
			l_sampler->SetInterpolationMode( InterpolationFilter::Min, l_mode );
			l_sampler->SetInterpolationMode( InterpolationFilter::Mag, l_mode );
			l_sampler->SetWrappingMode( TextureUVW::U, WrapMode::ClampToBorder );
			l_sampler->SetWrappingMode( TextureUVW::V, WrapMode::ClampToBorder );
			l_sampler->SetWrappingMode( TextureUVW::W, WrapMode::ClampToBorder );
		}
		else
		{
			l_sampler = m_renderTarget.GetEngine()->GetSamplerCache().Find( l_name );
		}

		return l_sampler;
	}
}
