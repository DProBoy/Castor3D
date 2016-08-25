#include "ShaderCache.hpp"

#include "Engine.hpp"
#include "Event/Frame/InitialiseEvent.hpp"
#include "Event/Frame/CleanupEvent.hpp"
#include "Render/Pipeline.hpp"
#include "Render/RenderSystem.hpp"
#include "Technique/RenderTechnique.hpp"
#include "Shader/FrameVariableBuffer.hpp"
#include "Shader/OneFrameVariable.hpp"
#include "Shader/ShaderProgram.hpp"

using namespace Castor;

namespace Castor3D
{
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

	ShaderProgramSPtr ShaderProgramCache::GetNewProgram()
	{
		ShaderProgramSPtr l_return = GetEngine()->GetRenderSystem()->CreateShaderProgram();

		if ( l_return )
		{
			m_arrayPrograms.push_back( l_return );
			GetEngine()->PostEvent( MakeInitialiseEvent( *l_return ) );
		}

		return l_return;
	}
	
	ShaderProgramSPtr ShaderProgramCache::GetAutomaticProgram( RenderTechnique const & p_technique, uint32_t p_textureFlags, uint32_t p_programFlags )
	{
		ShaderProgramSPtr l_return;
		uint64_t l_key = p_textureFlags | ( uint64_t( p_programFlags ) << 32 );
		ShaderProgramWPtrUInt64MapConstIt l_it = m_mapAutogenerated.find( l_key );

		if ( l_it != m_mapAutogenerated.end() )
		{
			l_return = l_it->second.lock();
		}
		else
		{
			l_return = GetNewProgram();

			if ( l_return )
			{
				eSHADER_MODEL l_model = GetEngine()->GetRenderSystem()->GetGpuInformations().GetMaxShaderModel();
				l_return->SetSource( ShaderType::Vertex, l_model, GetEngine()->GetRenderSystem()->GetVertexShaderSource( p_textureFlags, p_programFlags ) );
				l_return->SetSource( ShaderType::Pixel, l_model, p_technique.GetPixelShaderSource( p_textureFlags, p_programFlags ) );

				CreateTextureVariables( *l_return, p_textureFlags );
				CreateMatrixBuffer( *l_return, MASK_SHADER_TYPE_VERTEX | MASK_SHADER_TYPE_PIXEL );
				CreateSceneBuffer( *l_return, MASK_SHADER_TYPE_PIXEL );
				CreatePassBuffer( *l_return, MASK_SHADER_TYPE_PIXEL );
				CreateAnimationBuffer( *l_return, p_programFlags, MASK_SHADER_TYPE_VERTEX );

				m_mapAutogenerated.insert( { l_key, l_return } );
			}
		}

		return l_return;
	}

	ShaderProgramSPtr ShaderProgramCache::GetBillboardProgram( uint32_t p_textureFlags, uint32_t p_programFlags )const
	{
		ShaderProgramSPtr l_return;
		uint64_t l_key = p_textureFlags | ( uint64_t( p_programFlags ) << 32 );
		auto const & l_it = m_mapBillboards.find( l_key );

		if ( l_it != m_mapBillboards.end() )
		{
			l_return = l_it->second.lock();
		}

		return l_return;
	}

	void ShaderProgramCache::AddBillboardProgram( ShaderProgramSPtr p_program, uint32_t p_textureFlags, uint32_t p_programFlags )
	{
		uint64_t l_key = p_textureFlags | ( uint64_t( p_programFlags ) << 32 );
		auto const & l_it = m_mapBillboards.find( l_key );

		if ( l_it == m_mapBillboards.end() )
		{
			m_mapBillboards.insert( { l_key, p_program } );
			p_program->GetRenderSystem()->GetEngine()->PostEvent( MakeInitialiseEvent( *p_program ) );
			m_arrayPrograms.push_back( p_program );
		}
	}

	FrameVariableBufferSPtr ShaderProgramCache::CreateMatrixBuffer( ShaderProgram & p_shader, uint32_t p_shaderMask )
	{
		auto l_buffer = GetEngine()->GetRenderSystem()->CreateFrameVariableBuffer( ShaderProgram::BufferMatrix );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, Pipeline::MtxProjection, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, Pipeline::MtxModel, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, Pipeline::MtxView, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, Pipeline::MtxNormal, 1 );

		for ( uint32_t i = 0; i < C3D_MAX_TEXTURE_MATRICES; ++i )
		{
			l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, Pipeline::MtxTexture[i], 1 );
		}

		p_shader.AddFrameVariableBuffer( l_buffer, p_shaderMask );
		return l_buffer;
	}

	FrameVariableBufferSPtr ShaderProgramCache::CreateSceneBuffer( ShaderProgram & p_shader, uint32_t p_shaderMask )
	{
		auto l_buffer = GetEngine()->GetRenderSystem()->CreateFrameVariableBuffer( ShaderProgram::BufferScene );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::AmbientLight, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::BackgroundColour, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4i, ShaderProgram::LightsCount, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec3r, ShaderProgram::CameraPos, 1 );
		p_shader.AddFrameVariableBuffer( l_buffer, p_shaderMask );
		return l_buffer;
	}

	FrameVariableBufferSPtr ShaderProgramCache::CreatePassBuffer( ShaderProgram & p_shader, uint32_t p_shaderMask )
	{
		auto l_buffer = GetEngine()->GetRenderSystem()->CreateFrameVariableBuffer( ShaderProgram::BufferPass );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::MatAmbient, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::MatDiffuse, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::MatEmissive, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Vec4f, ShaderProgram::MatSpecular, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Float, ShaderProgram::MatShininess, 1 );
		l_buffer->CreateVariable( p_shader, FrameVariableType::Float, ShaderProgram::MatOpacity, 1 );
		p_shader.AddFrameVariableBuffer( l_buffer, p_shaderMask );
		return l_buffer;
	}

	FrameVariableBufferSPtr ShaderProgramCache::CreateAnimationBuffer( ShaderProgram & p_shader, uint32_t p_programFlags, uint32_t p_shaderMask )
	{
		FrameVariableBufferSPtr l_buffer;

		if ( CheckFlag( p_programFlags, ProgramFlag::Skinning )
			|| CheckFlag( p_programFlags, ProgramFlag::Morphing ) )
		{
			l_buffer = GetEngine()->GetRenderSystem()->CreateFrameVariableBuffer( ShaderProgram::BufferAnimation );

			if ( CheckFlag( p_programFlags, ProgramFlag::Skinning ) )
			{
				l_buffer->CreateVariable( p_shader, FrameVariableType::Mat4x4r, ShaderProgram::Bones, 400 );
			}

			if ( CheckFlag( p_programFlags, ProgramFlag::Morphing ) )
			{
				l_buffer->CreateVariable( p_shader, FrameVariableType::Float, ShaderProgram::Time );
			}

			p_shader.AddFrameVariableBuffer( l_buffer, p_shaderMask );
		}

		return l_buffer;
	}

	void ShaderProgramCache::CreateTextureVariables( ShaderProgram & p_shader, uint32_t p_uiTextureFlags )
	{
		p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::Lights, ShaderType::Pixel );

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Ambient ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapAmbient, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Colour ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapColour, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Diffuse ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapDiffuse, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Normal ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapNormal, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Specular ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapSpecular, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Emissive ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapEmissive, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Opacity ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapOpacity, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Gloss ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapGloss, ShaderType::Pixel );
		}

		if ( CheckFlag( p_uiTextureFlags, TextureChannel::Height ) )
		{
			p_shader.CreateFrameVariable< OneIntFrameVariable >( ShaderProgram::MapHeight, ShaderType::Pixel );
		}
	}
}
