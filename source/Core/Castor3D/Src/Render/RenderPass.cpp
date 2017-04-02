﻿#include "RenderPass.hpp"

#include "Engine.hpp"

#include "Material/Pass.hpp"
#include "Mesh/Submesh.hpp"
#include "Mesh/Buffer/GeometryBuffers.hpp"
#include "Mesh/Buffer/VertexBuffer.hpp"
#include "Render/RenderPipeline.hpp"
#include "Render/RenderNode/RenderNode_Render.hpp"
#include "Scene/BillboardList.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Geometry.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneNode.hpp"
#include "Shader/UniformBuffer.hpp"
#include "Shader/ShaderProgram.hpp"

#include <GlslSource.hpp>

using namespace Castor;

namespace Castor3D
{
	namespace
	{
		template< typename MapType, typename FuncType >
		inline void DoTraverseNodes( RenderPass const & p_pass
			, MapType & p_nodes
			, FuncType p_function )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				l_itPipelines.first->Apply();

				for ( auto l_itPass : l_itPipelines.second )
				{
					for ( auto l_itSubmeshes : l_itPass.second )
					{
						DoBindPassOpacityMap( l_itSubmeshes.second[0].m_passNode
							, l_itSubmeshes.second[0].m_passNode.m_pass );

						p_function( *l_itPipelines.first
							, *l_itPass.first
							, *l_itSubmeshes.first
							, l_itSubmeshes.second );

						DoUnbindPassOpacityMap( l_itSubmeshes.second[0].m_passNode
							, l_itSubmeshes.second[0].m_passNode.m_pass );
					}
				}
			}
		}

		template< typename MapType, typename FuncType >
		inline void DoTraverseNodes( RenderPass const & p_pass
			, MapType & p_nodes
			, PassRenderNodeUniforms & p_passNode
			, Scene & p_scene
			, DepthMapArray & p_depthMaps
			, FuncType p_function )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				l_itPipelines.first->Apply();

				for ( auto l_itPass : l_itPipelines.second )
				{
					for ( auto l_itSubmeshes : l_itPass.second )
					{
						DoBindPass( l_itSubmeshes.second[0].m_passNode
							, *l_itPass.first
							, p_scene
							, *l_itPipelines.first
							, p_depthMaps );

						p_function( *l_itPipelines.first
							, *l_itPass.first
							, *l_itSubmeshes.first
							, l_itSubmeshes.second );

						DoUnbindPass( l_itSubmeshes.second[0].m_passNode
							, *l_itPass.first
							, p_scene
							, *l_itPipelines.first
							, p_depthMaps );
					}
				}
			}
		}

		template< typename MapType, typename FuncType >
		inline void DoTraverseNodes( RenderPass const & p_pass
			, Camera const & p_camera
			, MapType & p_nodes
			, FuncType p_function )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				p_pass.UpdatePipeline( *l_itPipelines.first );
				l_itPipelines.first->Apply();

				for ( auto l_itPass : l_itPipelines.second )
				{
					for ( auto l_itSubmeshes : l_itPass.second )
					{
						DoBindPassOpacityMap( l_itSubmeshes.second[0].m_passNode
							, l_itSubmeshes.second[0].m_passNode.m_pass );

						p_function( *l_itPipelines.first
							, *l_itPass.first
							, *l_itSubmeshes.first
							, l_itSubmeshes.second );

						DoUnbindPassOpacityMap( l_itSubmeshes.second[0].m_passNode
							, l_itSubmeshes.second[0].m_passNode.m_pass );
					}
				}
			}
		}

		template< typename MapType, typename FuncType >
		inline void DoTraverseNodes( RenderPass const & p_pass
			, Camera const & p_camera
			, MapType & p_nodes
			, PassRenderNodeUniforms & p_passNode
			, Scene & p_scene
			, DepthMapArray & p_depthMaps
			, FuncType p_function )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				p_pass.UpdatePipeline( *l_itPipelines.first );
				l_itPipelines.first->Apply();

				for ( auto l_itPass : l_itPipelines.second )
				{
					for ( auto l_itSubmeshes : l_itPass.second )
					{
						DoBindPass( l_itSubmeshes.second[0].m_passNode
							, *l_itPass.first
							, p_scene
							, *l_itPipelines.first
							, p_depthMaps );

						p_function( *l_itPipelines.first
							, *l_itPass.first
							, *l_itSubmeshes.first
							, l_itSubmeshes.second );

						DoUnbindPass( l_itSubmeshes.second[0].m_passNode
							, *l_itPass.first
							, p_scene
							, *l_itPipelines.first
							, p_depthMaps );
					}
				}
			}
		}

		template< typename MapType >
		inline void DoRenderNonInstanced( RenderPass const & p_pass
			, MapType & p_nodes )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				l_itPipelines.first->Apply();

				for ( auto & l_renderNode : l_itPipelines.second )
				{
					DoBindPassOpacityMap( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass );

					DoRenderNodeNoPass( l_renderNode );

					DoUnbindPassOpacityMap( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass );
				}
			}
		}

		template< typename MapType >
		inline void DoRenderNonInstanced( RenderPass const & p_pass
			, MapType & p_nodes
			, Scene & p_scene
			, PassRenderNodeUniforms & p_passNode
			, DepthMapArray & p_depthMaps )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				l_itPipelines.first->Apply();

				for ( auto & l_renderNode : l_itPipelines.second )
				{
					DoBindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );

					DoRenderNode( l_renderNode );

					DoUnbindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );
				}
			}
		}

		template< typename MapType >
		inline void DoRenderNonInstanced( RenderPass const & p_pass
			, Camera const & p_camera
			, MapType & p_nodes )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				p_pass.UpdatePipeline( *l_itPipelines.first );
				l_itPipelines.first->Apply();

				for ( auto & l_renderNode : l_itPipelines.second )
				{
					DoBindPassOpacityMap( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass );

					DoRenderNodeNoPass( l_renderNode );

					DoUnbindPassOpacityMap( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass );
				}
			}
		}

		template< typename MapType >
		inline void DoRenderNonInstanced( RenderPass const & p_pass
			, Camera const & p_camera
			, MapType & p_nodes
			, Scene & p_scene
			, PassRenderNodeUniforms & p_passNode
			, DepthMapArray & p_depthMaps )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				p_pass.UpdatePipeline( *l_itPipelines.first );
				l_itPipelines.first->Apply();

				for ( auto & l_renderNode : l_itPipelines.second )
				{
					DoBindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );

					DoRenderNode( l_renderNode );

					DoUnbindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );
				}
			}
		}

		template< typename MapType >
		inline void DoRenderNonInstanced( RenderPass const & p_pass
			, Camera const & p_camera
			, MapType & p_nodes
			, Scene & p_scene
			, PassRenderNodeUniforms & p_passNode
			, DepthMapArray & p_depthMaps
			, RenderInfo & p_info )
		{
			for ( auto l_itPipelines : p_nodes )
			{
				p_pass.UpdatePipeline( *l_itPipelines.first );
				l_itPipelines.first->Apply();

				for ( auto & l_renderNode : l_itPipelines.second )
				{
					DoBindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );

					DoRenderNode( l_renderNode );
					++p_info.m_drawCalls;

					DoUnbindPass( l_renderNode.m_passNode
						, l_renderNode.m_passNode.m_pass
						, p_scene
						, *l_itPipelines.first
						, p_depthMaps );

					++p_info.m_visibleObjectsCount;
				}
			}
		}
	}

	RenderPass::RenderPass( String const & p_name, Engine & p_engine, bool p_opaque, bool p_multisampling )
		: OwnedBy< Engine >{ p_engine }
		, Named{ p_name }
		, m_renderSystem{ *p_engine.GetRenderSystem() }
		, m_multisampling{ p_multisampling }
		, m_renderQueue{ *this, p_opaque }
		, m_opaque{ p_opaque }
		, m_matrixUbo{ ShaderProgram::BufferMatrix, *p_engine.GetRenderSystem() }
		, m_modelMatrixUbo{ ShaderProgram::BufferModelMatrix, *p_engine.GetRenderSystem() }
		, m_sceneUbo{ ShaderProgram::BufferScene, *p_engine.GetRenderSystem() }
		, m_passUbo{ ShaderProgram::BufferPass, *p_engine.GetRenderSystem() }
		, m_modelUbo{ ShaderProgram::BufferModel, *p_engine.GetRenderSystem() }
		, m_billboardUbo{ ShaderProgram::BufferBillboards, *p_engine.GetRenderSystem() }
		, m_skinningUbo{ ShaderProgram::BufferSkinning, *p_engine.GetRenderSystem() }
		, m_morphingUbo{ ShaderProgram::BufferMorphing, *p_engine.GetRenderSystem() }
	{
		UniformBuffer::FillMatrixBuffer( m_matrixUbo );
		UniformBuffer::FillModelMatrixBuffer( m_modelMatrixUbo );
		UniformBuffer::FillSceneBuffer( m_sceneUbo );
		UniformBuffer::FillPassBuffer( m_passUbo );
		UniformBuffer::FillModelBuffer( m_modelUbo );
		UniformBuffer::FillBillboardBuffer( m_billboardUbo );
		UniformBuffer::FillSkinningBuffer( m_skinningUbo );
		UniformBuffer::FillMorphingBuffer( m_morphingUbo );

		m_projectionUniform = m_matrixUbo.GetUniform< UniformType::eMat4x4f >( RenderPipeline::MtxProjection );
		m_viewUniform = m_matrixUbo.GetUniform< UniformType::eMat4x4f >( RenderPipeline::MtxView );

		m_passNode = std::make_unique< PassRenderNodeUniforms >( m_passUbo );
	}

	RenderPass::~RenderPass()
	{
	}

	bool RenderPass::Initialise( Size const & p_size )
	{
		return DoInitialise( p_size );
	}

	void RenderPass::Cleanup()
	{
		m_skinningUbo.Cleanup();
		m_morphingUbo.Cleanup();
		m_billboardUbo.Cleanup();
		m_modelUbo.Cleanup();
		m_passUbo.Cleanup();
		m_modelMatrixUbo.Cleanup();
		m_matrixUbo.Cleanup();
		m_sceneUbo.Cleanup();
		DoCleanup();

		for ( auto & l_buffers : m_geometryBuffers )
		{
			l_buffers->Cleanup();
		}

		m_geometryBuffers.clear();
	}

	void RenderPass::Update( RenderQueueArray & p_queues )
	{
		DoUpdate( p_queues );
	}

	String RenderPass::GetVertexShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, bool p_invertNormals )const
	{
		return DoGetVertexShaderSource( p_textureFlags, p_programFlags, p_sceneFlags, p_invertNormals );
	}

	String RenderPass::GetPixelShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		return DoGetPixelShaderSource( p_textureFlags, p_programFlags, p_sceneFlags );
	}

	String RenderPass::GetTessellationControlShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		return DoGetTessellationControlShaderSource( p_textureFlags, p_programFlags, p_sceneFlags );
	}

	String RenderPass::GetTessellationEvaluationShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		return DoGetTessellationEvaluationShaderSource( p_textureFlags, p_programFlags, p_sceneFlags );
	}
	String RenderPass::GetGeometryShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		return DoGetGeometryShaderSource( p_textureFlags, p_programFlags, p_sceneFlags );
	}

	void RenderPass::PreparePipeline( BlendMode p_colourBlendMode
		, BlendMode p_alphaBlendMode
		, TextureChannels & p_textureFlags
		, ProgramFlags & p_programFlags
		, SceneFlags & p_sceneFlags
		, bool p_twoSided )
	{
		DoUpdateFlags( p_textureFlags
			, p_programFlags
			, p_sceneFlags );

		if ( CheckFlag( p_programFlags, ProgramFlag::eAlphaBlending ) != m_opaque
			&& ( !CheckFlag( p_programFlags, ProgramFlag::eBillboards )
				|| !IsShadowMapProgram( p_programFlags ) ) )
		{
			if ( m_opaque )
			{
				p_alphaBlendMode = BlendMode::eNoBlend;
			}

			auto l_backProgram = DoGetProgram( p_textureFlags, p_programFlags, p_sceneFlags, false );

			if ( !m_opaque )
			{
				auto l_frontProgram = DoGetProgram( p_textureFlags, p_programFlags, p_sceneFlags, true );
				auto l_flags = PipelineFlags{ p_colourBlendMode, p_alphaBlendMode, p_textureFlags, p_programFlags, p_sceneFlags };
				DoPrepareFrontPipeline( *l_frontProgram, l_flags );
				DoPrepareBackPipeline( *l_backProgram, l_flags );
			}
			else
			{
				auto l_flags = PipelineFlags{ p_colourBlendMode, p_alphaBlendMode, p_textureFlags, p_programFlags, p_sceneFlags };

				if ( p_twoSided || CheckFlag( p_textureFlags, TextureChannel::eOpacity ) )
				{
					auto l_frontProgram = DoGetProgram( p_textureFlags, p_programFlags, p_sceneFlags, true );
					DoPrepareFrontPipeline( *l_frontProgram, l_flags );
				}

				DoPrepareBackPipeline( *l_backProgram, l_flags );
			}
		}
	}

	RenderPipeline * RenderPass::GetPipelineFront( BlendMode p_colourBlendMode
		, BlendMode p_alphaBlendMode
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )
	{
		if ( m_opaque )
		{
			p_alphaBlendMode = BlendMode::eNoBlend;
		}

		auto l_it = m_frontPipelines.find( { p_colourBlendMode, p_alphaBlendMode, p_textureFlags, p_programFlags, p_sceneFlags } );
		RenderPipeline * l_return{ nullptr };

		if ( l_it != m_frontPipelines.end() )
		{
			l_return = l_it->second.get();
		}

		return l_return;
	}

	RenderPipeline * RenderPass::GetPipelineBack( BlendMode p_colourBlendMode
		, BlendMode p_alphaBlendMode
		, TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )
	{
		if ( m_opaque )
		{
			p_alphaBlendMode = BlendMode::eNoBlend;
		}

		auto l_it = m_backPipelines.find( { p_colourBlendMode, p_alphaBlendMode, p_textureFlags, p_programFlags, p_sceneFlags } );
		RenderPipeline * l_return{ nullptr };

		if ( l_it != m_backPipelines.end() )
		{
			l_return = l_it->second.get();
		}

		return l_return;
	}

	SkinningRenderNode RenderPass::CreateSkinningNode( Pass & p_pass
		, RenderPipeline & p_pipeline
		, Submesh & p_submesh
		, Geometry & p_primitive
		, AnimatedSkeleton & p_skeleton )
	{
		auto l_buffers = p_submesh.GetGeometryBuffers( p_pipeline.GetProgram() );
		m_geometryBuffers.insert( l_buffers );

		return SkinningRenderNode
		{
			p_pipeline,
			DoCreatePassRenderNode( p_pass, p_pipeline ),
			m_modelMatrixUbo,
			m_modelUbo,
			*l_buffers,
			*p_primitive.GetParent(),
			p_submesh,
			p_primitive,
			p_skeleton,
			m_skinningUbo
		};
	}

	MorphingRenderNode RenderPass::CreateMorphingNode( Pass & p_pass
		, RenderPipeline & p_pipeline
		, Submesh & p_submesh
		, Geometry & p_primitive
		, AnimatedMesh & p_mesh )
	{
		auto l_buffers = p_submesh.GetGeometryBuffers( p_pipeline.GetProgram() );
		m_geometryBuffers.insert( l_buffers );

		return MorphingRenderNode
		{
			p_pipeline,
			DoCreatePassRenderNode( p_pass, p_pipeline ),
			m_modelMatrixUbo,
			m_modelUbo,
			*l_buffers,
			*p_primitive.GetParent(),
			p_submesh,
			p_primitive,
			p_mesh,
			m_morphingUbo
		};
	}

	StaticRenderNode RenderPass::CreateStaticNode( Pass & p_pass
		, RenderPipeline & p_pipeline
		, Submesh & p_submesh
		, Geometry & p_primitive )
	{
		auto l_buffers = p_submesh.GetGeometryBuffers( p_pipeline.GetProgram() );
		m_geometryBuffers.insert( l_buffers );

		return StaticRenderNode
		{
			p_pipeline,
			DoCreatePassRenderNode( p_pass, p_pipeline ),
			m_modelMatrixUbo,
			m_modelUbo,
			*l_buffers,
			*p_primitive.GetParent(),
			p_submesh,
			p_primitive,
		};
	}

	BillboardRenderNode RenderPass::CreateBillboardNode( Pass & p_pass
		, RenderPipeline & p_pipeline
		, BillboardBase & p_billboard )
	{
		auto l_buffers = p_billboard.GetGeometryBuffers( p_pipeline.GetProgram() );
		m_geometryBuffers.insert( l_buffers );

		return BillboardRenderNode
		{
			p_pipeline,
			DoCreatePassRenderNode( p_pass, p_pipeline ),
			m_modelMatrixUbo,
			m_modelUbo,
			*l_buffers,
			*p_billboard.GetNode(),
			p_billboard,
			m_billboardUbo,
		};
	}

	void RenderPass::UpdatePipeline( RenderPipeline & p_pipeline )const
	{
		DoUpdatePipeline( p_pipeline );
	}

	PassRenderNode RenderPass::DoCreatePassRenderNode( Pass & p_pass
		, RenderPipeline & p_pipeline )
	{
		return PassRenderNode
		{
			p_pass,
			p_pipeline.GetProgram(),
			m_passUbo,
		};
	}

	SceneRenderNode RenderPass::DoCreateSceneRenderNode( Scene & p_scene
		, RenderPipeline & p_pipeline )
	{
		return SceneRenderNode
		{
			m_sceneUbo
		};
	}

	ShaderProgramSPtr RenderPass::DoGetProgram( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, bool p_invertNormals )const
	{
		return GetEngine()->GetShaderProgramCache().GetAutomaticProgram( *this, p_textureFlags, p_programFlags, p_sceneFlags, p_invertNormals );
	}

	uint32_t RenderPass::DoCopyNodesMatrices( StaticRenderNodeArray const & p_renderNodes
		, VertexBuffer & p_matrixBuffer )
	{
		constexpr uint32_t l_stride = 16 * sizeof( real );
		auto const l_count = std::min( p_matrixBuffer.GetSize() / l_stride, uint32_t( p_renderNodes.size() ) );
		auto l_buffer = p_matrixBuffer.data();
		auto l_it = p_renderNodes.begin();
		auto i = 0u;

		while ( i < l_count )
		{
			std::memcpy( l_buffer, l_it->m_sceneNode.GetDerivedTransformationMatrix().const_ptr(), l_stride );
			l_buffer += l_stride;
			++i;
			++l_it;
		}

		p_matrixBuffer.Upload( 0u, l_stride * l_count, p_matrixBuffer.data() );
		return l_count;
	}

	uint32_t RenderPass::DoCopyNodesMatrices( StaticRenderNodeArray const & p_renderNodes
		, VertexBuffer & p_matrixBuffer
		, RenderInfo & p_info )
	{
		constexpr uint32_t l_stride = 16 * sizeof( real );
		auto const l_count = std::min( p_matrixBuffer.GetSize() / l_stride, uint32_t( p_renderNodes.size() ) );
		auto l_buffer = p_matrixBuffer.data();
		auto l_it = p_renderNodes.begin();
		auto i = 0u;

		while ( i < l_count )
		{
			std::memcpy( l_buffer, l_it->m_sceneNode.GetDerivedTransformationMatrix().const_ptr(), l_stride );
			++p_info.m_visibleObjectsCount;
			l_buffer += l_stride;
			++i;
			++l_it;
		}

		p_matrixBuffer.Upload( 0u, l_stride * l_count, p_matrixBuffer.data() );
		return l_count;
	}

	void RenderPass::DoRenderInstancedSubmeshes( SubmeshStaticRenderNodesByPipelineMap & p_nodes )
	{
		DoTraverseNodes( *this
			, p_nodes
			, [this]( RenderPipeline & p_pipeline
				, Pass & p_pass
				, Submesh & p_submesh
				, StaticRenderNodeArray & p_renderNodes )
			{
				if ( !p_renderNodes.empty() && p_submesh.HasMatrixBuffer() )
				{
					uint32_t l_count = DoCopyNodesMatrices( p_renderNodes, p_submesh.GetMatrixBuffer() );
					p_submesh.DrawInstanced( p_renderNodes[0].m_buffers, l_count );
				}
			} );
	}

	void RenderPass::DoRenderInstancedSubmeshes( SubmeshStaticRenderNodesByPipelineMap & p_nodes
		, DepthMapArray & p_depthMaps )
	{
		DoTraverseNodes( *this
			, p_nodes
			, *m_passNode
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, p_depthMaps
			, [this, &p_depthMaps]( RenderPipeline & p_pipeline
				, Pass & p_pass
				, Submesh & p_submesh
				, StaticRenderNodeArray & p_renderNodes )
			{
				if ( !p_renderNodes.empty() && p_submesh.HasMatrixBuffer() )
				{
					uint32_t l_count = DoCopyNodesMatrices( p_renderNodes, p_submesh.GetMatrixBuffer() );
					p_submesh.DrawInstanced( p_renderNodes[0].m_buffers, l_count );
				}
			} );
	}

	void RenderPass::DoRenderInstancedSubmeshes( SubmeshStaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera )
	{
		DoTraverseNodes( *this
			, p_camera
			, p_nodes
			, [this]( RenderPipeline & p_pipeline
				, Pass & p_pass
				, Submesh & p_submesh
				, StaticRenderNodeArray & p_renderNodes )
			{
				if ( !p_renderNodes.empty() && p_submesh.HasMatrixBuffer() )
				{
					uint32_t l_count = DoCopyNodesMatrices( p_renderNodes, p_submesh.GetMatrixBuffer() );
					p_submesh.DrawInstanced( p_renderNodes[0].m_buffers, l_count );
				}
			} );
	}

	void RenderPass::DoRenderInstancedSubmeshes( SubmeshStaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps )
	{
		DoTraverseNodes( *this
			, p_camera
			, p_nodes
			, *m_passNode
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, p_depthMaps
			, [this, &p_depthMaps]( RenderPipeline & p_pipeline
				, Pass & p_pass
				, Submesh & p_submesh
				, StaticRenderNodeArray & p_renderNodes )
			{
				if ( !p_renderNodes.empty() && p_submesh.HasMatrixBuffer() )
				{
					uint32_t l_count = DoCopyNodesMatrices( p_renderNodes, p_submesh.GetMatrixBuffer() );
					p_submesh.DrawInstanced( p_renderNodes[0].m_buffers, l_count );
				}
			} );
	}

	void RenderPass::DoRenderInstancedSubmeshes( SubmeshStaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps
		, RenderInfo & p_info )
	{
		DoTraverseNodes( *this
			, p_camera
			, p_nodes
			, *m_passNode
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, p_depthMaps
			, [this, &p_info, &p_depthMaps]( RenderPipeline & p_pipeline
				, Pass & p_pass
				, Submesh & p_submesh
				, StaticRenderNodeArray & p_renderNodes )
			{
				if ( !p_renderNodes.empty() && p_submesh.HasMatrixBuffer() )
				{
					uint32_t l_count = DoCopyNodesMatrices( p_renderNodes, p_submesh.GetMatrixBuffer(), p_info );
					p_submesh.DrawInstanced( p_renderNodes[0].m_buffers, l_count );
					++p_info.m_drawCalls;
				}
			} );
	}

	void RenderPass::DoRenderStaticSubmeshes( StaticRenderNodesByPipelineMap & p_nodes )
	{
		DoRenderNonInstanced( *this
			, p_nodes );
	}

	void RenderPass::DoRenderStaticSubmeshes( StaticRenderNodesByPipelineMap & p_nodes
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderStaticSubmeshes( StaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes );
	}

	void RenderPass::DoRenderStaticSubmeshes( StaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderStaticSubmeshes( StaticRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps
		, RenderInfo & p_info )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps
			, p_info );
	}

	void RenderPass::DoRenderSkinningSubmeshes( SkinningRenderNodesByPipelineMap & p_nodes )
	{
		DoRenderNonInstanced( *this
			, p_nodes );
	}

	void RenderPass::DoRenderSkinningSubmeshes( SkinningRenderNodesByPipelineMap & p_nodes
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderSkinningSubmeshes( SkinningRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes );
	}

	void RenderPass::DoRenderSkinningSubmeshes( SkinningRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderSkinningSubmeshes( SkinningRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps
		, RenderInfo & p_info )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps
			, p_info );
	}

	void RenderPass::DoRenderMorphingSubmeshes( MorphingRenderNodesByPipelineMap & p_nodes )
	{
		DoRenderNonInstanced( *this
			, p_nodes );
	}

	void RenderPass::DoRenderMorphingSubmeshes( MorphingRenderNodesByPipelineMap & p_nodes
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderMorphingSubmeshes( MorphingRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes );
	}

	void RenderPass::DoRenderMorphingSubmeshes( MorphingRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderMorphingSubmeshes( MorphingRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps
		, RenderInfo & p_info )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps
			, p_info );
	}

	void RenderPass::DoRenderBillboards( BillboardRenderNodesByPipelineMap & p_nodes )
	{
		DoRenderNonInstanced( *this
			, p_nodes );
	}

	void RenderPass::DoRenderBillboards( BillboardRenderNodesByPipelineMap & p_nodes
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderBillboards( BillboardRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes );
	}

	void RenderPass::DoRenderBillboards( BillboardRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps );
	}

	void RenderPass::DoRenderBillboards( BillboardRenderNodesByPipelineMap & p_nodes
		, Camera const & p_camera
		, DepthMapArray & p_depthMaps
		, RenderInfo & p_info )
	{
		DoRenderNonInstanced( *this
			, p_camera
			, p_nodes
			, *GetEngine()->GetRenderSystem()->GetTopScene()
			, *m_passNode
			, p_depthMaps
			, p_info );
	}

	String RenderPass::DoGetVertexShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags
		, bool p_invertNormals )const
	{
		using namespace GLSL;
		auto l_writer = GetEngine()->GetRenderSystem()->CreateGlslWriter();
		// Vertex inputs
		auto position = l_writer.GetAttribute< Vec4 >( ShaderProgram::Position );
		auto normal = l_writer.GetAttribute< Vec3 >( ShaderProgram::Normal );
		auto tangent = l_writer.GetAttribute< Vec3 >( ShaderProgram::Tangent );
		auto bitangent = l_writer.GetAttribute< Vec3 >( ShaderProgram::Bitangent );
		auto texture = l_writer.GetAttribute< Vec3 >( ShaderProgram::Texture );
		auto bone_ids0 = l_writer.GetAttribute< IVec4 >( ShaderProgram::BoneIds0, CheckFlag( p_programFlags, ProgramFlag::eSkinning ) );
		auto bone_ids1 = l_writer.GetAttribute< IVec4 >( ShaderProgram::BoneIds1, CheckFlag( p_programFlags, ProgramFlag::eSkinning ) );
		auto weights0 = l_writer.GetAttribute< Vec4 >( ShaderProgram::Weights0, CheckFlag( p_programFlags, ProgramFlag::eSkinning ) );
		auto weights1 = l_writer.GetAttribute< Vec4 >( ShaderProgram::Weights1, CheckFlag( p_programFlags, ProgramFlag::eSkinning ) );
		auto transform = l_writer.GetAttribute< Mat4 >( ShaderProgram::Transform, CheckFlag( p_programFlags, ProgramFlag::eInstantiation ) );
		auto position2 = l_writer.GetAttribute< Vec4 >( ShaderProgram::Position2, CheckFlag( p_programFlags, ProgramFlag::eMorphing ) );
		auto normal2 = l_writer.GetAttribute< Vec3 >( ShaderProgram::Normal2, CheckFlag( p_programFlags, ProgramFlag::eMorphing ) );
		auto tangent2 = l_writer.GetAttribute< Vec3 >( ShaderProgram::Tangent2, CheckFlag( p_programFlags, ProgramFlag::eMorphing ) );
		auto bitangent2 = l_writer.GetAttribute< Vec3 >( ShaderProgram::Bitangent2, CheckFlag( p_programFlags, ProgramFlag::eMorphing ) );
		auto texture2 = l_writer.GetAttribute< Vec3 >( ShaderProgram::Texture2, CheckFlag( p_programFlags, ProgramFlag::eMorphing ) );
		auto gl_InstanceID( l_writer.GetBuiltin< Int >( cuT( "gl_InstanceID" ) ) );

		UBO_MATRIX( l_writer );
		UBO_MODEL_MATRIX( l_writer );
		UBO_SKINNING( l_writer, p_programFlags );
		UBO_MORPHING( l_writer, p_programFlags );
		UBO_SCENE( l_writer );

		// Outputs
		Vec3 vtx_position;
		Vec3 vtx_tangentSpaceFragPosition;
		Vec3 vtx_tangentSpaceViewPosition;
		Vec3 vtx_normal;
		Vec3 vtx_tangent;
		Vec3 vtx_bitangent;
		Vec3 vtx_texture;
		Int vtx_instance;

		if ( !CheckFlag( p_textureFlags, TextureChannel::eNormal )
			|| !CheckFlag( p_textureFlags, TextureChannel::eHeight ) )
		{
			vtx_position = l_writer.GetOutput< Vec3 >( cuT( "vtx_position" ) );
			vtx_tangentSpaceFragPosition = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangentSpaceFragPosition" ) );
			vtx_tangentSpaceViewPosition = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangentSpaceViewPosition" ) );
			vtx_normal = l_writer.GetOutput< Vec3 >( cuT( "vtx_normal" ) );
			vtx_tangent = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangent" ) );
			vtx_bitangent = l_writer.GetOutput< Vec3 >( cuT( "vtx_bitangent" ) );
			vtx_texture = l_writer.GetOutput< Vec3 >( cuT( "vtx_texture" ) );
			vtx_instance = l_writer.GetOutput< Int >( cuT( "vtx_instance" ) );
		}
		else
		{
			vtx_position = l_writer.GetOutput< Vec3 >( cuT( "out_position" ) );
			vtx_tangentSpaceFragPosition = l_writer.GetOutput< Vec3 >( cuT( "out_tangentSpaceFragPosition" ) );
			vtx_tangentSpaceViewPosition = l_writer.GetOutput< Vec3 >( cuT( "out_tangentSpaceViewPosition" ) );
			vtx_normal = l_writer.GetOutput< Vec3 >( cuT( "out_normal" ) );
			vtx_tangent = l_writer.GetOutput< Vec3 >( cuT( "out_tangent" ) );
			vtx_bitangent = l_writer.GetOutput< Vec3 >( cuT( "out_bitangent" ) );
			vtx_texture = l_writer.GetOutput< Vec3 >( cuT( "out_texture" ) );
			vtx_instance = l_writer.GetOutput< Int >( cuT( "out_instance" ) );
		}

		auto gl_Position = l_writer.GetBuiltin< Vec4 >( cuT( "gl_Position" ) );

		std::function< void() > l_main = [&]()
		{
			auto l_v4Vertex = l_writer.GetLocale( cuT( "l_v4Vertex" ), vec4( position.xyz(), 1.0 ) );
			auto l_v4Normal = l_writer.GetLocale( cuT( "l_v4Normal" ), vec4( normal, 0.0 ) );
			auto l_v4Tangent = l_writer.GetLocale( cuT( "l_v4Tangent" ), vec4( tangent, 0.0 ) );
			auto l_v3Texture = l_writer.GetLocale( cuT( "l_v3Texture" ), texture );
			auto l_mtxModel = l_writer.GetLocale< Mat4 >( cuT( "l_mtxModel" ) );

			if ( CheckFlag( p_programFlags, ProgramFlag::eSkinning ) )
			{
				auto l_mtxBoneTransform = l_writer.GetLocale< Mat4 >( cuT( "l_mtxBoneTransform" ) );
				l_mtxBoneTransform = c3d_mtxBones[bone_ids0[0_i]] * weights0[0_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids0[1_i]] * weights0[1_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids0[2_i]] * weights0[2_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids0[3_i]] * weights0[3_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids1[0_i]] * weights1[0_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids1[1_i]] * weights1[1_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids1[2_i]] * weights1[2_i];
				l_mtxBoneTransform += c3d_mtxBones[bone_ids1[3_i]] * weights1[3_i];
				l_mtxModel = c3d_mtxModel * l_mtxBoneTransform;
			}
			else if ( CheckFlag( p_programFlags, ProgramFlag::eInstantiation ) )
			{
				l_mtxModel = transform;
			}
			else
			{
				l_mtxModel = c3d_mtxModel;
			}

			if ( CheckFlag( p_programFlags, ProgramFlag::eMorphing ) )
			{
				auto l_time = l_writer.GetLocale( cuT( "l_time" ), 1.0_f - c3d_fTime );
				l_v4Vertex = vec4( l_v4Vertex.xyz() * l_time + position2.xyz() * c3d_fTime, 1.0 );
				l_v4Normal = vec4( l_v4Normal.xyz() * l_time + normal2.xyz() * c3d_fTime, 1.0 );
				l_v4Tangent = vec4( l_v4Tangent.xyz() * l_time + tangent2.xyz() * c3d_fTime, 1.0 );
				l_v3Texture = l_v3Texture * l_writer.Paren( 1.0_f - c3d_fTime ) + texture2 * c3d_fTime;
			}

			vtx_texture = l_v3Texture;
			l_v4Vertex = l_mtxModel * l_v4Vertex;
			vtx_position = l_v4Vertex.xyz();

			if ( !CheckFlag( p_textureFlags, TextureChannel::eNormal )
				|| !CheckFlag( p_textureFlags, TextureChannel::eHeight ) )
			{
				l_v4Vertex = c3d_mtxView * l_v4Vertex;
				gl_Position = c3d_mtxProjection * l_v4Vertex;
			}

			vtx_instance = gl_InstanceID;
			l_mtxModel = transpose( inverse( l_mtxModel ) );

			if ( p_invertNormals )
			{
				vtx_normal = normalize( l_writer.Paren( l_mtxModel * -l_v4Normal ).xyz() );
			}
			else
			{
				vtx_normal = normalize( l_writer.Paren( l_mtxModel * l_v4Normal ).xyz() );
			}

			vtx_tangent = normalize( l_writer.Paren( l_mtxModel * l_v4Tangent ).xyz() );
			vtx_tangent = normalize( vtx_tangent - vtx_normal * dot( vtx_tangent, vtx_normal ) );
			vtx_bitangent = cross( vtx_normal, vtx_tangent );

			auto l_tbn = l_writer.GetLocale( cuT( "l_tbn" ), transpose( mat3( vtx_tangent, vtx_bitangent, vtx_normal ) ) );
			vtx_tangentSpaceFragPosition = l_tbn * vtx_position;
			vtx_tangentSpaceViewPosition = l_tbn * c3d_v3CameraPosition;
		};

		l_writer.ImplementFunction< void >( cuT( "main" ), l_main );
		return l_writer.Finalise();
	}

	struct BezierPatch
	{
		BezierPatch( GLSL::GlslWriter & p_writer, String const & p_name )
			: m_struct{ p_writer, p_name }
			, m_wpB030{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB030" ) ) }
			, m_wpB021{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB021" ) ) }
			, m_wpB012{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB012" ) ) }
			, m_wpB003{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB003" ) ) }
			, m_wpB102{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB102" ) ) }
			, m_wpB201{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB201" ) ) }
			, m_wpB300{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB300" ) ) }
			, m_wpB210{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB210" ) ) }
			, m_wpB120{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB120" ) ) }
			, m_wpB111{ m_struct.GetMember< GLSL::Vec3 >( cuT( "m_wpB111" ) ) }
		{
			m_struct.End();
		}

		GLSL::Struct m_struct;
		GLSL::Vec3 m_wpB030;
		GLSL::Vec3 m_wpB021;
		GLSL::Vec3 m_wpB012;
		GLSL::Vec3 m_wpB003;
		GLSL::Vec3 m_wpB102;
		GLSL::Vec3 m_wpB201;
		GLSL::Vec3 m_wpB300;
		GLSL::Vec3 m_wpB210;
		GLSL::Vec3 m_wpB120;
		GLSL::Vec3 m_wpB111;
	};

	String RenderPass::DoGetTessellationControlShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		String l_result;

		if ( CheckFlag( p_textureFlags, TextureChannel::eNormal )
			&& CheckFlag( p_textureFlags, TextureChannel::eHeight ) )
		{
			using namespace GLSL;
			auto l_writer = GetEngine()->GetRenderSystem()->CreateGlslWriter();

			l_writer.OutputGeometryLayout( cuT( "vertices = 3" ) );

			// TCS inputs
			UBO_MATRIX( l_writer );
			UBO_SCENE( l_writer );
			auto out_position = l_writer.GetInputArray< Vec3 >( cuT( "out_position" ) );
			auto out_tangentSpaceFragPosition = l_writer.GetInputArray< Vec3 >( cuT( "out_tangentSpaceFragPosition" ) );
			auto out_tangentSpaceViewPosition = l_writer.GetInputArray< Vec3 >( cuT( "out_tangentSpaceViewPosition" ) );
			auto out_normal = l_writer.GetInputArray< Vec3 >( cuT( "out_normal" ) );
			auto out_tangent = l_writer.GetInputArray< Vec3 >( cuT( "out_tangent" ) );
			auto out_bitangent = l_writer.GetInputArray< Vec3 >( cuT( "out_bitangent" ) );
			auto out_texture = l_writer.GetInputArray< Vec3 >( cuT( "out_texture" ) );
			auto out_instance = l_writer.GetInputArray< Int >( cuT( "out_instance" ) );
			auto gl_InvocationID = l_writer.GetBuiltin< Int >( cuT( "gl_InvocationID" ) );

			// TCS outputs
			//auto l_outputPatch = l_writer.GetOutput< BezierPatch >( cuT( "tcs_controlPoints" ) );
			auto tcs_position = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_position" ) );
			auto tcs_tangentSpaceFragPosition = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_tangentSpaceFragPosition" ) );
			auto tcs_tangentSpaceViewPosition = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_tangentSpaceViewPosition" ) );
			auto tcs_normal = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_normal" ) );
			auto tcs_tangent = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_tangent" ) );
			auto tcs_bitangent = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_bitangent" ) );
			auto tcs_texture = l_writer.GetOutputArray< Vec3 >( cuT( "tcs_texture" ) );
			auto tcs_instance = l_writer.GetOutputArray< Int >( cuT( "tcs_instance" ) );
			auto gl_TessLevelOuter = l_writer.GetBuiltin< Float >( cuT( "gl_TessLevelOuter" ), 4u );
			auto gl_TessLevelInner = l_writer.GetBuiltin< Float >( cuT( "gl_TessLevelInner" ), 2u );
			
			auto l_getTessLevel = l_writer.ImplementFunction< Float >( cuT( "GetTessLevel" )
				, [&]( Float const & p_a, Float const & p_b )
				{
					auto l_avgDistance = l_writer.GetLocale( cuT( "l_avgDistance" )
						, l_writer.Paren( p_a + p_b ) / 2.0 );

					IF( l_writer, l_avgDistance <= 20.0 )
					{
						l_writer.Return( 256.0_f );
					}
					ELSEIF( l_writer, l_avgDistance <= 50.0 )
					{
						l_writer.Return( 128.0_f );
					}
					ELSEIF( l_writer, l_avgDistance <= 100.0_f )
					{
						l_writer.Return( 64.0_f );
					}
					FI;

					l_writer.Return( 16.0_f );
				}, InFloat{ &l_writer, cuT( "p_a" ) }
				, InFloat{ &l_writer, cuT( "p_b" ) } );

			l_writer.ImplementFunction< Void >( cuT( "main" )
				, [&]()
				{
					tcs_position[gl_InvocationID] = out_position[gl_InvocationID];
					tcs_tangentSpaceFragPosition[gl_InvocationID] = out_tangentSpaceFragPosition[gl_InvocationID];
					tcs_tangentSpaceViewPosition[gl_InvocationID] = out_tangentSpaceViewPosition[gl_InvocationID];
					tcs_normal[gl_InvocationID] = out_normal[gl_InvocationID];
					tcs_tangent[gl_InvocationID] = out_tangent[gl_InvocationID];
					tcs_bitangent[gl_InvocationID] = out_bitangent[gl_InvocationID];
					tcs_texture[gl_InvocationID] = out_texture[gl_InvocationID];
					tcs_instance[gl_InvocationID] = out_instance[gl_InvocationID];

					// Calculate the distance from the camera to the three control points
					auto l_eyeToVertexDistance0 = l_writer.GetLocale( cuT( "l_eyeToVertexDistance0" )
						, distance( c3d_v3CameraPosition, tcs_position[0] ) );
					auto l_eyeToVertexDistance1 = l_writer.GetLocale( cuT( "l_eyeToVertexDistance1" )
						, distance( c3d_v3CameraPosition, tcs_position[1] ) );
					auto l_eyeToVertexDistance2 = l_writer.GetLocale( cuT( "l_eyeToVertexDistance2" )
						, distance( c3d_v3CameraPosition, tcs_position[2] ) );

					// Calculate the tessellation levels
					gl_TessLevelOuter[0] = l_getTessLevel( l_eyeToVertexDistance1, l_eyeToVertexDistance2 );
					gl_TessLevelOuter[1] = l_getTessLevel( l_eyeToVertexDistance2, l_eyeToVertexDistance0 );
					gl_TessLevelOuter[2] = l_getTessLevel( l_eyeToVertexDistance0, l_eyeToVertexDistance1 );
					gl_TessLevelInner[0] = gl_TessLevelOuter[2];
				} );
			l_result = l_writer.Finalise();
		}

		return l_result;
	}

	String RenderPass::DoGetTessellationEvaluationShaderSource( TextureChannels const & p_textureFlags
		, ProgramFlags const & p_programFlags
		, SceneFlags const & p_sceneFlags )const
	{
		String l_result;

		if ( CheckFlag( p_textureFlags, TextureChannel::eNormal )
			&& CheckFlag( p_textureFlags, TextureChannel::eHeight ) )
		{
			using namespace GLSL;
			auto l_writer = GetEngine()->GetRenderSystem()->CreateGlslWriter();

			l_writer.InputGeometryLayout( cuT( "triangles, equal_spacing, ccw" ) );

			// ECS inputs
			UBO_MATRIX( l_writer );
			auto c3d_mapHeight( l_writer.GetUniform< Sampler2D >( ShaderProgram::MapHeight, CheckFlag( p_textureFlags, TextureChannel::eHeight ) ) );
			auto tcs_position = l_writer.GetInputArray< Vec3 >( cuT( "tcs_position" ) );
			auto tcs_tangentSpaceFragPosition = l_writer.GetInputArray< Vec3 >( cuT( "tcs_tangentSpaceFragPosition" ) );
			auto tcs_tangentSpaceViewPosition = l_writer.GetInputArray< Vec3 >( cuT( "tcs_tangentSpaceViewPosition" ) );
			auto tcs_normal = l_writer.GetInputArray< Vec3 >( cuT( "tcs_normal" ) );
			auto tcs_tangent = l_writer.GetInputArray< Vec3 >( cuT( "tcs_tangent" ) );
			auto tcs_bitangent = l_writer.GetInputArray< Vec3 >( cuT( "tcs_bitangent" ) );
			auto tcs_texture = l_writer.GetInputArray< Vec3 >( cuT( "tcs_texture" ) );
			auto tcs_instance = l_writer.GetInputArray< Int >( cuT( "tcs_instance" ) );
			auto gl_TessCoord = l_writer.GetBuiltin< Vec3 >( cuT( "gl_TessCoord" ) );

			// ECS outputs
			auto vtx_position = l_writer.GetOutput< Vec3 >( cuT( "vtx_position" ) );
			auto vtx_tangentSpaceFragPosition = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangentSpaceFragPosition" ) );
			auto vtx_tangentSpaceViewPosition = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangentSpaceViewPosition" ) );
			auto vtx_normal = l_writer.GetOutput< Vec3 >( cuT( "vtx_normal" ) );
			auto vtx_tangent = l_writer.GetOutput< Vec3 >( cuT( "vtx_tangent" ) );
			auto vtx_bitangent = l_writer.GetOutput< Vec3 >( cuT( "vtx_bitangent" ) );
			auto vtx_texture = l_writer.GetOutput< Vec3 >( cuT( "vtx_texture" ) );
			auto vtx_instance = l_writer.GetOutput< Int >( cuT( "vtx_instance" ) );
			auto gl_Position = l_writer.GetBuiltin< Vec4 >( cuT( "gl_Position" ) );

			auto l_interpolate1D = l_writer.ImplementFunction< Int >( cuT( "Interpolate1D" )
				, [&]( Int const & p_v0, Int const & p_v1, Int const & p_v2 )
				{
					l_writer.Return( l_writer.Cast< Int >( gl_TessCoord.x() * p_v0
						+ gl_TessCoord.y() * p_v1
						+ gl_TessCoord.z() * p_v2 ) );
				}, InInt{ &l_writer, cuT( "p_v0" ) }
				, InInt{ &l_writer, cuT( "p_v1" ) }
				, InInt{ &l_writer, cuT( "p_v2" ) } );

			auto l_interpolate2D = l_writer.ImplementFunction< Vec2 >( cuT( "Interpolate2D" )
				, [&]( Vec2 const & p_v0, Vec2 const & p_v1, Vec2 const & p_v2 )
				{
					l_writer.Return( vec2( gl_TessCoord.x() ) * p_v0
						+ vec2( gl_TessCoord.y() ) * p_v1
						+ vec2( gl_TessCoord.z() ) * p_v2 );
				}, InVec2{ &l_writer, cuT( "p_v0" ) }
				, InVec2{ &l_writer, cuT( "p_v1" ) } 
				, InVec2{ &l_writer, cuT( "p_v2" ) } );

			auto l_interpolate3D = l_writer.ImplementFunction< Vec3 >( cuT( "Interpolate3D" )
				, [&]( Vec3 const & p_v0, Vec3 const & p_v1, Vec3 const & p_v2 )
				{
					l_writer.Return( vec3( gl_TessCoord.x() ) * p_v0
						+ vec3( gl_TessCoord.y() ) * p_v1
						+ vec3( gl_TessCoord.z() ) * p_v2 );
				}, InVec3{ &l_writer, cuT( "p_v0" ) }
				, InVec3{ &l_writer, cuT( "p_v1" ) } 
				, InVec3{ &l_writer, cuT( "p_v2" ) } );

			l_writer.ImplementFunction< Void >( cuT( "main" )
				, [&]()
				{
					// Interpolate the attributes of the output vertex using the barycentric coordinates
					vtx_position = l_interpolate3D( tcs_position[0], tcs_position[1], tcs_position[2] );
					vtx_tangentSpaceFragPosition = l_interpolate3D( tcs_tangentSpaceFragPosition[0], tcs_tangentSpaceFragPosition[1], tcs_tangentSpaceFragPosition[2] );
					vtx_tangentSpaceViewPosition = l_interpolate3D( tcs_tangentSpaceViewPosition[0], tcs_tangentSpaceViewPosition[1], tcs_tangentSpaceViewPosition[2] );
					vtx_normal = l_interpolate3D( tcs_normal[0], tcs_normal[1], tcs_normal[2] );
					vtx_tangent = l_interpolate3D( tcs_tangent[0], tcs_tangent[1], tcs_tangent[2] );
					vtx_bitangent = l_interpolate3D( tcs_bitangent[0], tcs_bitangent[1], tcs_bitangent[2] );
					vtx_texture = l_interpolate3D( tcs_texture[0], tcs_texture[1], tcs_texture[2] );
					vtx_instance = l_interpolate1D( tcs_instance[0], tcs_instance[1], tcs_instance[2] );
					vtx_normal = normalize( vtx_normal );
					vtx_tangent = normalize( vtx_tangent );
					vtx_bitangent = normalize( vtx_bitangent );

					// Displace the vertex along the normal
					auto l_displacement = l_writer.GetLocale( cuT( "l_displacement" )
						, texture( c3d_mapHeight, vtx_texture.xy() ).x() );
					vtx_position += vtx_normal * l_writer.Paren( 1.0 - l_displacement );
					gl_Position = c3d_mtxProjection * c3d_mtxView * vec4( vtx_position, 1.0 );
				} );
			l_result = l_writer.Finalise();
		}

		return l_result;
	}
}
