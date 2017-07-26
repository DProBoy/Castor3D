#include "ShaderCache.hpp"

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
		uint64_t MakeKey( TextureChannels const & textureFlags
			, ProgramFlags const & programFlags
			, SceneFlags const & sceneFlags
			, ComparisonFunc alphaFunc
			, bool invertNormals )
		{
			return ( uint64_t( textureFlags ) << 52 ) // Texture flags on 12 bits
				   | ( uint64_t( programFlags ) << 36 ) // Program flags on 16 bits
				   | ( uint64_t( sceneFlags ) << 28 ) // SceneFlags on 8 bits
				   | ( uint64_t( alphaFunc ) << 20 ) // Alpha func on 8 bits
				   | ( uint64_t( invertNormals ? 0x01 : 0x00 ) );
		}
	}

	ShaderProgramCache::ShaderProgramCache( Engine & engine )
		: OwnedBy< Engine >( engine )
	{
	}

	ShaderProgramCache::~ShaderProgramCache()
	{
	}

	void ShaderProgramCache::Cleanup()
	{
		for ( auto program : m_arrayPrograms )
		{
			GetEngine()->PostEvent( MakeCleanupEvent( *program ) );
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
		ShaderProgramSPtr result = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( result )
		{
			DoAddProgram( result, p_initialise );
		}

		return result;
	}

	ShaderProgramSPtr ShaderProgramCache::GetAutomaticProgram( RenderPass const & p_renderPass
		, TextureChannels const & textureFlags
		, ProgramFlags const & programFlags
		, SceneFlags const & sceneFlags
		, ComparisonFunc alphaFunc
		, bool invertNormals )
	{
		ShaderProgramSPtr result;

		if ( CheckFlag( programFlags, ProgramFlag::eBillboards ) )
		{
			uint64_t key = MakeKey( textureFlags
				, programFlags
				, sceneFlags
				, alphaFunc
				, false );
			auto const & it = m_mapBillboards.find( key );

			if ( it != m_mapBillboards.end() )
			{
				result = it->second.lock();
			}
			else
			{
				result = DoCreateBillboardProgram( p_renderPass
					, textureFlags
					, programFlags
					, sceneFlags
					, alphaFunc );

				if ( result )
				{
					DoAddBillboardProgram( result
						, textureFlags
						, programFlags
						, sceneFlags
						, alphaFunc );
				}
			}
		}
		else
		{
			uint64_t key = MakeKey( textureFlags
				, programFlags
				, sceneFlags
				, alphaFunc
				, invertNormals );
			ShaderProgramWPtrUInt64MapConstIt it = m_mapAutogenerated.find( key );

			if ( it != m_mapAutogenerated.end() )
			{
				result = it->second.lock();
			}
			else
			{
				result = DoCreateAutomaticProgram( p_renderPass
					, textureFlags
					, programFlags
					, sceneFlags
					, alphaFunc
					, invertNormals );

				if ( result )
				{
					DoAddAutomaticProgram( result
						, textureFlags
						, programFlags
						, sceneFlags
						, alphaFunc
						, invertNormals );
				}
			}
		}

		return result;
	}

	void ShaderProgramCache::CreateTextureVariables(
		ShaderProgram & p_shader,
		TextureChannels const & textureFlags,
		ProgramFlags const & programFlags )const
	{
		if ( CheckFlag( programFlags, ProgramFlag::eLighting ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::Lights, ShaderType::ePixel )->SetValue( Pass::LightBufferIndex );
		}

		if ( CheckFlag( textureFlags, TextureChannel::eNormal ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapNormal, ShaderType::ePixel );
		}

		if ( CheckFlag( textureFlags, TextureChannel::eEmissive ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEmissive, ShaderType::ePixel );
		}

		if ( CheckFlag( textureFlags, TextureChannel::eOpacity ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapOpacity, ShaderType::ePixel );
		}

		if ( CheckFlag( textureFlags, TextureChannel::eHeight ) )
		{
			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapHeight, ShaderType::ePixel );
		}

		if ( CheckFlag( programFlags, ProgramFlag::ePbrMetallicRoughness ) )
		{
			if ( CheckFlag( textureFlags, TextureChannel::eAlbedo ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapAlbedo, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eMetallic ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapMetallic, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eRoughness ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapRoughness, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eAmbientOcclusion ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapAmbientOcclusion, ShaderType::ePixel );
			}

			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEnvironment, ShaderType::ePixel );
		}
		else if ( CheckFlag( programFlags, ProgramFlag::ePbrSpecularGlossiness ) )
		{
			if ( CheckFlag( textureFlags, TextureChannel::eDiffuse ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapDiffuse, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eSpecular ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapSpecular, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eGloss ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapGloss, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eAmbientOcclusion ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapAmbientOcclusion, ShaderType::ePixel );
			}

			p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEnvironment, ShaderType::ePixel );
		}
		else
		{
			if ( CheckFlag( textureFlags, TextureChannel::eReflection )
				|| CheckFlag( textureFlags, TextureChannel::eRefraction ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapEnvironment, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eDiffuse ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapDiffuse, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eSpecular ) )
			{
				p_shader.CreateUniform< UniformType::eSampler >( ShaderProgram::MapSpecular, ShaderType::ePixel );
			}

			if ( CheckFlag( textureFlags, TextureChannel::eGloss ) )
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
		, TextureChannels const & textureFlags
		, ProgramFlags const & programFlags
		, SceneFlags const & sceneFlags
		, ComparisonFunc alphaFunc
		, bool invertNormals )const
	{
		ShaderProgramSPtr result = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( result )
		{
			ShaderTypeFlags matrixUboShaderMask = ShaderTypeFlag::eVertex | ShaderTypeFlag::ePixel;
			result->CreateObject( ShaderType::eVertex );
			result->CreateObject( ShaderType::ePixel );
			result->SetSource( ShaderType::eVertex, p_renderPass.GetVertexShaderSource( textureFlags, programFlags, sceneFlags, invertNormals ) );
			result->SetSource( ShaderType::ePixel, p_renderPass.GetPixelShaderSource( textureFlags, programFlags, sceneFlags, alphaFunc ) );
			auto geometry = p_renderPass.GetGeometryShaderSource( textureFlags, programFlags, sceneFlags );

			if ( !geometry.GetSource().empty() )
			{
				AddFlag( matrixUboShaderMask, ShaderTypeFlag::eGeometry );
				result->CreateObject( ShaderType::eGeometry );
				result->SetSource( ShaderType::eGeometry, geometry );
			}

			CreateTextureVariables( *result, textureFlags, programFlags );
		}

		return result;
	}

	void ShaderProgramCache::DoAddAutomaticProgram( ShaderProgramSPtr p_program
		, TextureChannels const & textureFlags
		, ProgramFlags const & programFlags
		, SceneFlags const & sceneFlags
		, ComparisonFunc alphaFunc
		, bool invertNormals )
	{
		uint64_t key = MakeKey( textureFlags
			, programFlags
			, sceneFlags
			, alphaFunc
			, invertNormals );
		auto const & it = m_mapAutogenerated.find( key );

		if ( it == m_mapAutogenerated.end() )
		{
			m_mapAutogenerated.insert( { key, p_program } );
			DoAddProgram( p_program, true );
		}
	}

	ShaderProgramSPtr ShaderProgramCache::DoCreateBillboardProgram( RenderPass const & p_renderPass
		, TextureChannels const & textureFlags
		, ProgramFlags const & programFlags
		, SceneFlags const & sceneFlags
		, ComparisonFunc alphaFunc )const
	{
		auto & engine = *GetEngine();
		auto & renderSystem = *engine.GetRenderSystem();
		ShaderProgramSPtr result = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( result )
		{
			GLSL::Shader strVtxShader;
			{
				using namespace GLSL;
				auto writer = renderSystem.CreateGlslWriter();

				// Shader inputs
				auto position = writer.DeclAttribute< Vec4 >( ShaderProgram::Position );
				auto texture = writer.DeclAttribute< Vec2 >( ShaderProgram::Texture );
				auto center = writer.DeclAttribute< Vec3 >( cuT( "center" ) );
				auto gl_InstanceID( writer.DeclBuiltin< Int >( cuT( "gl_InstanceID" ) ) );
				auto gl_VertexID( writer.DeclBuiltin< Int >( cuT( "gl_VertexID" ) ) );
				UBO_MATRIX( writer );
				UBO_MODEL_MATRIX( writer );
				UBO_SCENE( writer );
				UBO_MODEL( writer );
				UBO_BILLBOARD( writer );

				// Shader outputs
				auto vtx_position = writer.DeclOutput< Vec3 >( cuT( "vtx_position" ) );
				auto vtx_normal = writer.DeclOutput< Vec3 >( cuT( "vtx_normal" ) );
				auto vtx_tangent = writer.DeclOutput< Vec3 >( cuT( "vtx_tangent" ) );
				auto vtx_bitangent = writer.DeclOutput< Vec3 >( cuT( "vtx_bitangent" ) );
				auto vtx_texture = writer.DeclOutput< Vec3 >( cuT( "vtx_texture" ) );
				auto vtx_instance = writer.DeclOutput< Int >( cuT( "vtx_instance" ) );
				auto vtx_material = writer.DeclOutput< Int >( cuT( "vtx_material" ) );
				auto gl_Position = writer.DeclBuiltin< Vec4 >( cuT( "gl_Position" ) );

				writer.ImplementFunction< void >( cuT( "main" ), [&]()
				{
					auto bbcenter = writer.DeclLocale( cuT( "bbcenter" ), writer.Paren( c3d_mtxModel * vec4( center, 1.0 ) ).xyz() );
					auto toCamera = writer.DeclLocale( cuT( "toCamera" ), c3d_v3CameraPosition - bbcenter );
					toCamera.y() = 0.0_f;
					toCamera = normalize( toCamera );
					auto right = writer.DeclLocale( cuT( "right" ), vec3( c3d_mtxView[0][0], c3d_mtxView[1][0], c3d_mtxView[2][0] ) );
					auto up = writer.DeclLocale( cuT( "up" ), vec3( c3d_mtxView[0][1], c3d_mtxView[1][1], c3d_mtxView[2][1] ) );

					if ( !CheckFlag( programFlags, ProgramFlag::eSpherical ) )
					{
						right = normalize( vec3( right.x(), 0.0, right.z() ) );
						up = vec3( 0.0_f, 1.0f, 0.0f );
					}

					vtx_material = c3d_materialIndex;
					vtx_normal = toCamera;
					vtx_tangent = up;
					vtx_bitangent = right;

					auto width = writer.DeclLocale( cuT( "width" ), c3d_v2iDimensions.x() );
					auto height = writer.DeclLocale( cuT( "height" ), c3d_v2iDimensions.y() );

					if ( CheckFlag( programFlags, ProgramFlag::eFixedSize ) )
					{
						width = c3d_v2iDimensions.x() / c3d_v2iWindowSize.x();
						height = c3d_v2iDimensions.y() / c3d_v2iWindowSize.y();
					}

					vtx_position = bbcenter
						+ right * position.x() * width
						+ up * position.y() * height;

					vtx_texture = vec3( texture, 0.0 );
					vtx_instance = gl_InstanceID;
					auto wvPosition = writer.DeclLocale( cuT( "wvPosition" ), writer.Paren( c3d_mtxView * vec4( vtx_position, 1.0 ) ).xyz() );
					gl_Position = c3d_mtxProjection * vec4( wvPosition, 1.0 );
				} );

				strVtxShader = writer.Finalise();
			}

			GLSL::Shader strPxlShader = p_renderPass.GetPixelShaderSource( textureFlags
				, programFlags
				, sceneFlags
				, alphaFunc );

			result->CreateObject( ShaderType::eVertex );
			result->CreateObject( ShaderType::ePixel );
			result->SetSource( ShaderType::eVertex, strVtxShader );
			result->SetSource( ShaderType::ePixel, strPxlShader );

			CreateTextureVariables( *result, textureFlags, programFlags );
		}

		return result;
	}

	void ShaderProgramCache::DoAddBillboardProgram( ShaderProgramSPtr p_program
		, TextureChannels const & textureFlags
		, ProgramFlags const & programFlags
		, SceneFlags const & sceneFlags
		, ComparisonFunc alphaFunc )
	{
		uint64_t key = MakeKey( textureFlags
			, programFlags
			, sceneFlags
			, alphaFunc
			, false );
		auto const & it = m_mapBillboards.find( key );

		if ( it == m_mapBillboards.end() )
		{
			m_mapBillboards.insert( { key, p_program } );
			DoAddProgram( p_program, true );
		}
	}
}
