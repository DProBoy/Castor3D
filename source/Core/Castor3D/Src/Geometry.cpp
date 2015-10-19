﻿#include "Geometry.hpp"

#include "AnimatedObject.hpp"
#include "Engine.hpp"
#include "Material.hpp"
#include "MaterialManager.hpp"
#include "Mesh.hpp"
#include "MeshManager.hpp"
#include "Pass.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "SceneNode.hpp"
#include "ShaderManager.hpp"
#include "ShaderProgram.hpp"
#include "Submesh.hpp"

#include <Logger.hpp>

using namespace Castor;

namespace Castor3D
{
	bool Geometry::TextLoader::operator()( Geometry const & p_geometry, TextFile & p_file )
	{
		Logger::LogInfo( cuT( "Writing Geometry " ) + p_geometry.GetName() );
		bool l_return = p_file.WriteText( cuT( "\tobject \"" ) + p_geometry.GetName() + cuT( "\"\n\t{\n" ) ) > 0;

		if ( l_return )
		{
			l_return = MovableObject::TextLoader()( p_geometry, p_file );
		}

		if ( l_return )
		{
			l_return = Mesh::TextLoader()( *p_geometry.GetMesh(), p_file );
		}

		if ( l_return )
		{
			l_return = p_file.WriteText( cuT( "\t\tmaterials\n\t\t{\n" ) ) > 0;
		}

		if ( l_return )
		{
			for ( auto && l_submesh : *p_geometry.GetMesh() )
			{
				l_return = p_file.WriteText( cuT( "\t\t\t\tmaterial \"" ) + p_geometry.GetMaterial( l_submesh )->GetName() + cuT( "\"\n" ) ) > 0;
			}
		}

		if ( l_return )
		{
			l_return = p_file.WriteText( cuT( "\t\t}\n" ) ) > 0;
		}

		if ( l_return )
		{
			l_return = p_file.WriteText( cuT( "\t}\n" ) ) > 0;
		}

		return l_return;
	}

	//*************************************************************************************************

	Geometry::BinaryParser::BinaryParser( Path const & p_path )
		:	MovableObject::BinaryParser( p_path )
	{
	}

	bool Geometry::BinaryParser::Fill( Geometry const & p_obj, BinaryChunk & p_chunk )const
	{
		bool l_return = true;
		BinaryChunk l_chunk( eCHUNK_TYPE_GEOMETRY );

		if ( l_return )
		{
			l_return = MovableObject::BinaryParser( m_path ).Fill( p_obj, l_chunk );
		}

		if ( l_return && p_obj.GetMesh() )
		{
			l_return = DoFillChunk( p_obj.GetMesh()->GetName(), eCHUNK_TYPE_GEOMETRY_MESH, l_chunk );

			if ( l_return )
			{
				uint32_t l_id = 0;

				for ( auto && l_submesh : *p_obj.GetMesh() )
				{
					l_return = DoFillChunk( l_id, eCHUNK_TYPE_GEOMETRY_MATERIAL_ID, l_chunk );
					l_return = DoFillChunk( p_obj.GetMaterial( l_submesh )->GetName(), eCHUNK_TYPE_GEOMETRY_MATERIAL_NAME, l_chunk );
					l_id++;
				}
			}
		}

		if ( l_return )
		{
			l_chunk.Finalise();
			p_chunk.AddSubChunk( l_chunk );
		}

		return l_return;
	}

	bool Geometry::BinaryParser::Parse( Geometry & p_obj, BinaryChunk & p_chunk )const
	{
		bool l_return = true;
		String l_name;
		uint32_t l_id = 0;
		MaterialManager & l_mtlManager = p_obj.GetScene()->GetOwner()->GetMaterialManager();

		while ( p_chunk.CheckAvailable( 1 ) )
		{
			BinaryChunk l_chunk;
			l_return = p_chunk.GetSubChunk( l_chunk );

			if ( l_return )
			{
				switch ( l_chunk.GetChunkType() )
				{
				case eCHUNK_TYPE_MOVABLE_NODE:
					l_return = MovableObject::BinaryParser( m_path ).Parse( p_obj, l_chunk );
					break;

				case eCHUNK_TYPE_GEOMETRY_MESH:
					l_return = DoParseChunk( l_name, l_chunk );

					if ( l_return )
					{
						p_obj.SetMesh( p_obj.GetScene()->GetOwner()->GetMeshManager().Find( l_name ) );
					}

					break;

				case eCHUNK_TYPE_GEOMETRY_MATERIAL_ID:
					l_return = DoParseChunk( l_id, l_chunk );
					break;

				case eCHUNK_TYPE_GEOMETRY_MATERIAL_NAME:
					l_return = DoParseChunk( l_name, l_chunk );

					if ( l_return )
					{
						p_obj.SetMaterial( p_obj.GetMesh()->GetSubmesh( l_id ), l_mtlManager.Find( l_name ) );
					}

					break;
				}
			}

			if ( !l_return )
			{
				p_chunk.EndParse();
			}
		}

		return l_return;
	}

	//*************************************************************************************************

	Geometry::Geometry( SceneSPtr p_scene, MeshSPtr p_mesh, SceneNodeSPtr p_sn, String const & p_name )
		:	MovableObject( p_scene, p_sn, p_name, eMOVABLE_TYPE_GEOMETRY )
		,	m_mesh( p_mesh )
		,	m_changed( true )
		,	m_listCreated( false )
		,	m_visible( true )
	{
	}

	Geometry::~Geometry()
	{
		Cleanup();
	}

	void Geometry::Cleanup()
	{
	}

	void Geometry::CreateBuffers( uint32_t & p_nbFaces, uint32_t & p_nbVertex )
	{
		if ( !m_listCreated )
		{
			Cleanup();
			MeshSPtr l_pMesh = GetMesh();

			if ( l_pMesh )
			{
				uint32_t l_nbFaces = l_pMesh->GetFaceCount();
				uint32_t l_nbVertex = l_pMesh->GetVertexCount();
				p_nbFaces += l_nbFaces;
				p_nbVertex += l_nbVertex;
				l_pMesh->ComputeContainers();
				Logger::LogInfo( StringStream() << cuT( "Geometry::CreateBuffers - NbVertex: " ) << l_nbVertex << cuT( ", NbFaces: " ) << l_nbFaces );
				m_listCreated = l_pMesh->GetSubmeshCount() > 0;
			}
		}
	}

	void Geometry::Render()
	{
		if ( !m_listCreated )
		{
			uint32_t l_nbFaces = 0, l_nbVertex = 0;
			CreateBuffers( l_nbFaces, l_nbVertex );
		}
	}

	void Geometry::SetMesh( MeshSPtr p_pMesh )
	{
		m_submeshesMaterials.clear();
		m_mesh = p_pMesh;

		if ( p_pMesh )
		{
			m_strMeshName = p_pMesh->GetName();

			for ( auto && l_submesh : *p_pMesh )
			{
				SetMaterial( l_submesh, l_submesh->GetDefaultMaterial() );
			}
		}
		else
		{
			m_strMeshName = cuEmptyString;
		}
	}

	void Geometry::SetMaterial( SubmeshSPtr p_submesh, MaterialSPtr p_material )
	{
		MeshSPtr l_mesh = GetMesh();

		if ( l_mesh )
		{
			auto && l_it = std::find( l_mesh->begin(), l_mesh->end(), p_submesh );

			if ( l_it != l_mesh->end() )
			{
				m_submeshesMaterials[p_submesh] = p_material;
			}
			else
			{
				CASTOR_EXCEPTION( "Couldn't retrive the submesh in mesh's submeshes" );
			}
		}
		else
		{
			CASTOR_EXCEPTION( "No mesh" );
		}
	}

	MaterialSPtr Geometry::GetMaterial( SubmeshSPtr p_submesh )const
	{
		MaterialSPtr l_return;
		std::map< SubmeshSPtr, MaterialSPtr >::const_iterator l_it = m_submeshesMaterials.find( p_submesh );

		if ( l_it != m_submeshesMaterials.end() )
		{
			l_return = l_it->second;
		}
		else
		{
			Logger::LogError( cuT( "Geometry::GetMaterial - Wrong submesh" ) );
		}

		return l_return;
	}
	
	uint32_t Geometry::GetProgramFlags( Submesh & p_submesh )
	{
		uint32_t l_return = p_submesh.GetProgramFlags();

		if ( ( l_return & ePROGRAM_FLAG_SKINNING ) == ePROGRAM_FLAG_SKINNING )
		{
			auto l_animated = GetAnimatedObject();

			if ( !l_animated || !l_animated->IsPlayingAnimation() )
			{
				l_return &= ~ePROGRAM_FLAG_SKINNING;
			}
		}

		return l_return;
	}
}
