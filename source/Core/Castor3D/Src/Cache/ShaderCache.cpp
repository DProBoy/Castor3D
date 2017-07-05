﻿#include "ShaderCache.hpp"

#include "Engine.hpp"

#include "Event/Frame/CleanupEvent.hpp"
#include "Event/Frame/InitialiseEvent.hpp"
#include "Material/Pass.hpp"
#include "Render/RenderPipeline.hpp"
#include "Render/RenderPass.hpp"
#include "Shader/ShaderProgram.hpp"

#include <GlslSource.hpp>

using namespace Castor;

namespace Castor3D
{
	namespace
	{
		uint64_t MakeKey( TextureChannels const & p_textureFlags
			, ProgramFlags const & p_programFlags
			, SceneFlags const & p_sceneFlags
			, ComparisonFunc p_alphaFunc
			, bool p_invertNormals )
		{
			return ( uint64_t( p_textureFlags ) << 52 ) // Texture flags on 12 bits
				   | ( uint64_t( p_programFlags ) << 36 ) // Program flags on 16 bits
				   | ( uint64_t( p_sceneFlags ) << 28 ) // SceneFlags on 8 bits
				   | ( uint64_t( p_alphaFunc ) << 20 ) // Alpha func on 8 bits
				   | ( uint64_t( p_invertNormals ? 0x01 : 0x00 ) );
		}
	}

	ShaderProgramCache::ShaderProgramCache( Engine & p_engine )
		: OwnedBy< Engine >( p_engine )
	{
	}

	ShaderProgramCache::~ShaderProgramCache()
	{
	}

	void ShaderProgramCache::Cleanup()
	{
		for ( auto l_program : m_arrayPrograms )
		{
			GetEngine()->PostEvent( MakeCleanupEvent( *l_program ) );
		}
	}

	void ShaderProgramCache::Clear()
	{
		m_mapBillboards.clear();
		m_mapAutogenerated.clear();
		m_arrayPrograms.clear();
	}

	ShaderProgramSPtr ShaderProgramCache::GetNewProgram( bool p_initialise )
	{
		ShaderProgramSPtr l_return = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( l_return )
		{
			DoAddProgram( l_return, p_initialise );
		}

		return l_return;
	}

	ShaderProgramSPtr ShaderProgramCache::GetAutomaticProgram( RenderPass const & p_renderPass
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, ComparisonFunc p_alphaFunc
		, bool p_invertNormals )
	{
		ShaderProgramSPtr l_return;

		if ( CheckFlag( p_programFlags, ProgramFlag::eBillboards ) )
		{
			uint64_t l_key = MakeKey( p_textureFlags
				, p_programFlags
				, p_sceneFlags
				, p_alphaFunc
				, false );
			auto const & l_it = m_mapBillboards.find( l_key );

			if ( l_it != m_mapBillboards.end() )
			{
				l_return = l_it->second.lock();
			}
			else
			{
				l_return = DoCreateBillboardProgram( p_renderPass
					, p_textureFlags
					, p_programFlags
					, p_sceneFlags
					, p_alphaFunc );

				if ( l_return )
				{
					DoAddBillboardProgram( l_return
						, p_textureFlags
						, p_programFlags
						, p_sceneFlags
						, p_alphaFunc );
				}
			}
		}
		else
		{
			uint64_t l_key = MakeKey( p_textureFlags
				, p_programFlags
				, p_sceneFlags
				, p_alphaFunc
				, p_invertNormals );
			ShaderProgramWPtrUInt64MapConstIt l_it = m_mapAutogenerated.find( l_key );

			if ( l_it != m_mapAutogenerated.end() )
			{
				l_return = l_it->second.lock();
			}
			else
			{
				l_return = DoCreateAutomaticProgram( p_renderPass
					, p_textureFlags
					, p_programFlags
					, p_sceneFlags
					, p_alphaFunc
					, p_invertNormals );

				if ( l_return )
				{
					DoAddAutomaticProgram( l_return
						, p_textureFlags
						, p_programFlags
						, p_sceneFlags
						, p_alphaFunc
						, p_invertNormals );
				}
			}
		}

		return l_return;
	}

	void ShaderProgramCache::CreateTextureVariables(
		ShaderProgram & p_shader,
		TextureChannels const & p_textureFlags,
		ProgramFlags const & p_programFlags )const
	{
		if ( CheckFlag( p_programFlags, ProgramFlag::eLighting ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::Lights, ShaderType::ePixel )->SetValue( Pass::LightBufferIndex );
		}

		if ( CheckFlag( p_textureFlags, TextureChannel::eNormal ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapNormal, ShaderType::ePixel );
		}

		if ( CheckFlag( p_textureFlags, TextureChannel::eEmissive ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEmissive, ShaderType::ePixel );
		}

		if ( CheckFlag( p_textureFlags, TextureChannel::eOpacity ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapOpacity, ShaderType::ePixel );
		}

		if ( CheckFlag( p_textureFlags, TextureChannel::eHeight ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapHeight, ShaderType::ePixel );
		}

		if ( CheckFlag( p_programFlags, ProgramFlag::ePbr ) )
		{
			if ( CheckFlag( p_textureFlags, TextureChannel::eAlbedo ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapAlbedo, ShaderType::ePixel );
			}

			if ( CheckFlag( p_textureFlags, TextureChannel::eMetallic ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapMetallic, ShaderType::ePixel );
			}

			if ( CheckFlag( p_textureFlags, TextureChannel::eRoughness ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapRoughness, ShaderType::ePixel );
			}

			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEnvironment, ShaderType::ePixel );
		}
		else
		{
			if ( CheckFlag( p_textureFlags, TextureChannel::eReflection )
				|| CheckFlag( p_textureFlags, TextureChannel::eRefraction ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEnvironment, ShaderType::ePixel );
			}

			if ( CheckFlag( p_textureFlags, TextureChannel::eDiffuse ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapDiffuse, ShaderType::ePixel );
			}

			if ( CheckFlag( p_textureFlags, TextureChannel::eSpecular ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapSpecular, ShaderType::ePixel );
			}

			if ( CheckFlag( p_textureFlags, TextureChannel::eGloss ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapGloss, ShaderType::ePixel );
			}
		}
	}

	void ShaderProgramCache::DoAddProgram( ShaderProgramSPtr p_program, bool p_initialise )
	{
		m_arrayPrograms.push_back( p_program );

		if ( p_initialise )
		{
			if ( GetEngine()->GetRenderSystem()->GetCurrentContext() )
			{
				p_program->Initialise();
			}
			else
			{
				GetEngine()->PostEvent( MakeInitialiseEvent( *p_program ) );
			}
		}
	}

	ShaderProgramSPtr ShaderProgramCache::DoCreateAutomaticProgram( RenderPass const & p_renderPass
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, ComparisonFunc p_alphaFunc
		, bool p_invertNormals )const
	{
		ShaderProgramSPtr l_return = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( l_return )
		{
			ShaderTypeFlags l_matrixUboShaderMask = ShaderTypeFlag::eVertex | ShaderTypeFlag::ePixel;
			l_return->CreateObject( ShaderType::eVertex );
			l_return->CreateObject( ShaderType::ePixel );
			l_return->SetSource( ShaderType::eVertex, p_renderPass.GetVertexShaderSource( p_textureFlags, p_programFlags, p_sceneFlags, p_invertNormals ) );
			l_return->SetSource( ShaderType::ePixel, p_renderPass.GetPixelShaderSource( p_textureFlags, p_programFlags, p_sceneFlags, p_alphaFunc ) );
			auto l_geometry = p_renderPass.GetGeometryShaderSource( p_textureFlags, p_programFlags, p_sceneFlags );

			if ( !l_geometry.GetSource().empty() )
			{
				AddFlag( l_matrixUboShaderMask, ShaderTypeFlag::eGeometry );
				l_return->CreateObject( ShaderType::eGeometry );
				l_return->SetSource( ShaderType::eGeometry, l_geometry );
			}

			CreateTextureVariables( *l_return, p_textureFlags, p_programFlags );
		}

		return l_return;
	}

	void ShaderProgramCache::DoAddAutomaticProgram( ShaderProgramSPtr p_program
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, ComparisonFunc p_alphaFunc
		, bool p_invertNormals )
	{
		uint64_t l_key = MakeKey( p_textureFlags
			, p_programFlags
			, p_sceneFlags
			, p_alphaFunc
			, p_invertNormals );
		auto const & l_it = m_mapAutogenerated.find( l_key );

		if ( l_it == m_mapAutogenerated.end() )
		{
			m_mapAutogenerated.insert( { l_key, p_program } );
			DoAddProgram( p_program, true );
		}
	}

	ShaderProgramSPtr ShaderProgramCache::DoCreateBillboardProgram( RenderPass const & p_renderPass
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, ComparisonFunc p_alphaFunc )const
	{
		auto & l_engine = *GetEngine();
		auto & l_renderSystem = *l_engine.GetRenderSystem();
		ShaderProgramSPtr l_return = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( l_return )
		{
			GLSL::Shader l_strVtxShader;
			{
				using namespace GLSL;
				auto l_writer = l_renderSystem.CreateGlslWriter();

				// Shader inputs
				auto position = l_writer.DeclAttribute< Vec4 >( ShaderProgram::Position );
				auto texture = l_writer.DeclAttribute< Vec2 >( ShaderProgram::Texture );
				auto center = l_writer.DeclAttribute< Vec3 >( cuT( "center" ) );
				auto gl_InstanceID( l_writer.DeclBuiltin< Int >( cuT( "gl_InstanceID" ) ) );
				auto gl_VertexID( l_writer.DeclBuiltin< Int >( cuT( "gl_VertexID" ) ) );
				UBO_MATRIX( l_writer );
				UBO_MODEL_MATRIX( l_writer );
				UBO_SCENE( l_writer );
				UBO_MODEL( l_writer );
				UBO_BILLBOARD( l_writer );

				// Shader outputs
				auto vtx_position = l_writer.DeclOutput< Vec3 >( cuT( "vtx_position" ) );
				auto vtx_normal = l_writer.DeclOutput< Vec3 >( cuT( "vtx_normal" ) );
				auto vtx_tangent = l_writer.DeclOutput< Vec3 >( cuT( "vtx_tangent" ) );
				auto vtx_bitangent = l_writer.DeclOutput< Vec3 >( cuT( "vtx_bitangent" ) );
				auto vtx_texture = l_writer.DeclOutput< Vec3 >( cuT( "vtx_texture" ) );
				auto vtx_instance = l_writer.DeclOutput< Int >( cuT( "vtx_instance" ) );
				auto vtx_material = l_writer.DeclOutput< Int >( cuT( "vtx_material" ) );
				auto gl_Position = l_writer.DeclBuiltin< Vec4 >( cuT( "gl_Position" ) );

				l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
				{
					auto l_center = l_writer.DeclLocale( cuT( "l_center" ), l_writer.Paren( c3d_mtxModel * vec4( center, 1.0 ) ).xyz() );
					auto l_toCamera = l_writer.DeclLocale( cuT( "l_toCamera" ), c3d_v3CameraPosition - l_center );
					l_toCamera.y() = 0.0_f;
					l_toCamera = normalize( l_toCamera );
					auto l_right = l_writer.DeclLocale( cuT( "l_right" ), vec3( c3d_mtxView[0][0], c3d_mtxView[1][0], c3d_mtxView[2][0] ) );
					auto l_up = l_writer.DeclLocale( cuT( "l_up" ), vec3( c3d_mtxView[0][1], c3d_mtxView[1][1], c3d_mtxView[2][1] ) );

					if ( !CheckFlag( p_programFlags, ProgramFlag::eSpherical ) )
					{
						l_right = normalize( vec3( l_right.x(), 0.0, l_right.z() ) );
						l_up = vec3( 0.0_f, 1.0f, 0.0f );
					}

					vtx_material = c3d_materialIndex;
					vtx_normal = l_toCamera;
					vtx_tangent = l_up;
					vtx_bitangent = l_right;

					auto l_width = l_writer.DeclLocale( cuT( "l_width" ), c3d_v2iDimensions.x() );
					auto l_height = l_writer.DeclLocale( cuT( "l_height" ), c3d_v2iDimensions.y() );

					if ( CheckFlag( p_programFlags, ProgramFlag::eFixedSize ) )
					{
						l_width = c3d_v2iDimensions.x() / c3d_v2iWindowSize.x();
						l_height = c3d_v2iDimensions.y() / c3d_v2iWindowSize.y();
					}

					vtx_position = l_center
						+ l_right * position.x() * l_width
						+ l_up * position.y() * l_height;

					vtx_texture = vec3( texture, 0.0 );
					vtx_instance = gl_InstanceID;
					auto l_wvPosition = l_writer.DeclLocale( cuT( "l_wvPosition" ), l_writer.Paren( c3d_mtxView * vec4( vtx_position, 1.0 ) ).xyz() );
					gl_Position = c3d_mtxProjection * vec4( l_wvPosition, 1.0 );
				} );

				l_strVtxShader = l_writer.Finalise();
			}

			GLSL::Shader l_strPxlShader = p_renderPass.GetPixelShaderSource( p_textureFlags
				, p_programFlags
				, p_sceneFlags
				, p_alphaFunc );

			l_return->CreateObject( ShaderType::eVertex );
			l_return->CreateObject( ShaderType::ePixel );
			l_return->SetSource( ShaderType::eVertex, l_strVtxShader );
			l_return->SetSource( ShaderType::ePixel, l_strPxlShader );

			CreateTextureVariables( *l_return, p_textureFlags, p_programFlags );
		}

		return l_return;
	}

	void ShaderProgramCache::DoAddBillboardProgram( ShaderProgramSPtr p_program
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, ComparisonFunc p_alphaFunc )
	{
		uint64_t l_key = MakeKey( p_textureFlags
			, p_programFlags
			, p_sceneFlags
			, p_alphaFunc
			, false );
		auto const & l_it = m_mapBillboards.find( l_key );

		if ( l_it == m_mapBillboards.end() )
		{
			m_mapBillboards.insert( { l_key, p_program } );
			DoAddProgram( p_program, true );
		}
	}
}
