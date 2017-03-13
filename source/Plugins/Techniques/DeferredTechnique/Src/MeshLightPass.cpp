#include "MeshLightPass.hpp"

#include <Engine.hpp>
#include <Render/RenderPipeline.hpp>
#include <Render/RenderSystem.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Light/PointLight.hpp>
#include <Shader/ShaderProgram.hpp>

#include <GlslSource.hpp>
#include <GlslLight.hpp>
#include <GlslShadow.hpp>

using namespace Castor;
using namespace Castor3D;

namespace deferred
{
	//*********************************************************************************************

	MeshLightPass::Program::Program( Scene const & p_scene
		, String const & p_vtx
		, String const & p_pxl )
		: LightPass::Program{ p_scene, p_vtx, p_pxl }
	{
		RasteriserState l_rsstate1;
		l_rsstate1.SetCulledFaces( Culling::eFront );
		DepthStencilState l_dsstate1;
		l_dsstate1.SetDepthTest( false );
		l_dsstate1.SetDepthMask( WritingMask::eZero );
		l_dsstate1.SetStencilTest( true );
		l_dsstate1.SetStencilWriteMask( 0xFFFFFFFFu );
		l_dsstate1.SetStencilBackFunc( StencilFunc::eNEqual );
		l_dsstate1.SetStencilBackRef( 0u );
		l_dsstate1.SetStencilFrontFunc( StencilFunc::eNEqual );
		l_dsstate1.SetStencilFrontRef( 0u );
		m_firstPipeline = m_program->GetRenderSystem()->CreateRenderPipeline( std::move( l_dsstate1 )
			, std::move( l_rsstate1 )
			, BlendState{}
			, MultisampleState{}
			, *m_program
			, PipelineFlags{} );

		RasteriserState l_rsstate2;
		l_rsstate2.SetCulledFaces( Culling::eFront );
		DepthStencilState l_dsstate2;
		l_dsstate2.SetDepthTest( false );
		l_dsstate2.SetDepthMask( WritingMask::eZero );
		l_dsstate2.SetStencilTest( true );
		l_dsstate2.SetStencilWriteMask( 0xFFFFFFFFu );
		l_dsstate2.SetStencilBackFunc( StencilFunc::eNEqual );
		l_dsstate2.SetStencilBackRef( 0u );
		l_dsstate2.SetStencilFrontFunc( StencilFunc::eNEqual );
		l_dsstate2.SetStencilFrontRef( 0u );
		BlendState l_blstate;
		l_blstate.EnableBlend( true );
		l_blstate.SetRgbBlendOp( BlendOperation::eAdd );
		l_blstate.SetRgbSrcBlend( BlendOperand::eOne );
		l_blstate.SetRgbDstBlend( BlendOperand::eOne );
		m_blendPipeline = m_program->GetRenderSystem()->CreateRenderPipeline( std::move( l_dsstate2 )
			, std::move( l_rsstate2 )
			, std::move( l_blstate )
			, MultisampleState{}
			, *m_program
			, PipelineFlags{} );
	}

	MeshLightPass::Program::~Program()
	{
	}

	//*********************************************************************************************

	MeshLightPass::MeshLightPass( Engine & p_engine
		, FrameBuffer & p_frameBuffer
		, RenderBufferAttachment & p_depthAttach
		, LightType p_type
		, bool p_shadows )
		: LightPass{ p_engine, p_frameBuffer, p_depthAttach, p_shadows }
		, m_stencilPass{ p_frameBuffer, p_depthAttach }
		, m_modelMatrixUbo{ ShaderProgram::BufferModelMatrix, *p_engine.GetRenderSystem() }
		, m_type{ p_type }
	{
		UniformBuffer::FillModelMatrixBuffer( m_modelMatrixUbo );
		m_projectionUniform = m_matrixUbo.GetUniform< UniformType::eMat4x4f >( RenderPipeline::MtxProjection );
		m_viewUniform = m_matrixUbo.GetUniform< UniformType::eMat4x4f >( RenderPipeline::MtxView );
		m_modelUniform = m_modelMatrixUbo.GetUniform< UniformType::eMat4x4f >( RenderPipeline::MtxModel );
	}

	MeshLightPass::~MeshLightPass()
	{
		m_modelUniform = nullptr;
		m_viewUniform = nullptr;
		m_projectionUniform = nullptr;
		m_stencilPass.Cleanup();
		m_indexBuffer->Cleanup();
		m_vertexBuffer->Cleanup();
		m_modelMatrixUbo.Cleanup();
		m_matrixUbo.Cleanup();
		m_indexBuffer.reset();
		m_vertexBuffer.reset();
	}

	void MeshLightPass::Initialise( Scene const & p_scene
		, UniformBuffer & p_sceneUbo )
	{
		auto l_declaration = BufferDeclaration(
		{
			BufferElementDeclaration( ShaderProgram::Position, uint32_t( ElementUsage::ePosition ), ElementType::eVec3 ),
		} );

		auto l_data = DoGenerateVertices();
		m_vertexBuffer = std::make_shared< VertexBuffer >( m_engine, l_declaration );
		auto l_size = l_data.size() * sizeof( *l_data.data() );
		m_vertexBuffer->Resize( uint32_t( l_size ) );
		std::memcpy( m_vertexBuffer->data()
			, l_data.data()->const_ptr()
			, l_size );
		m_vertexBuffer->Initialise( BufferAccessType::eStatic, BufferAccessNature::eDraw );

		auto l_faces = DoGenerateFaces();
		m_indexBuffer = std::make_shared< IndexBuffer >( m_engine );
		m_indexBuffer->Resize( uint32_t( l_faces.size() ) );
		std::memcpy( m_indexBuffer->data()
			, l_faces.data()
			, l_faces.size() * sizeof( *l_faces.data() ) );
		m_indexBuffer->Initialise( BufferAccessType::eStatic, BufferAccessNature::eDraw );

		m_stencilPass.Initialise( *m_vertexBuffer
			, m_indexBuffer );

		DoInitialise( p_scene
			, m_type
			, *m_vertexBuffer
			, m_indexBuffer
			, p_sceneUbo
			, &m_modelMatrixUbo );
	}

	void MeshLightPass::Cleanup()
	{
		DoCleanup();
	}

	uint32_t MeshLightPass::GetCount()const
	{
		return m_indexBuffer->GetSize();
	}

	void MeshLightPass::DoUpdate( Size const & p_size
		, Light const & p_light
		, Camera const & p_camera )
	{
		auto l_model = DoComputeModelMatrix( p_light, p_camera );
		m_projectionUniform->SetValue( p_camera.GetViewport().GetProjection() );
		m_viewUniform->SetValue( p_camera.GetView() );
		m_modelUniform->SetValue( l_model );
		m_matrixUbo.Update();
		m_modelMatrixUbo.Update();
		m_stencilPass.Render( m_indexBuffer->GetSize() );
		p_camera.Apply();
	}
	
	String MeshLightPass::DoGetVertexShaderSource( SceneFlags const & p_sceneFlags )const
	{
		using namespace GLSL;
		GlslWriter l_writer = m_engine.GetRenderSystem()->CreateGlslWriter();

		// Shader inputs
		UBO_MATRIX( l_writer );
		UBO_MODEL_MATRIX( l_writer );
		auto vertex = l_writer.GetAttribute< Vec3 >( ShaderProgram::Position );

		// Shader outputs
		auto gl_Position = l_writer.GetBuiltin< Vec4 >( cuT( "gl_Position" ) );

		l_writer.ImplementFunction< void >( cuT( "main" ), [&]()
		{
			gl_Position = c3d_mtxProjection * c3d_mtxView * c3d_mtxModel * vec4( vertex, 1.0 );
		} );

		return l_writer.Finalise();
	}

	//*********************************************************************************************
}
