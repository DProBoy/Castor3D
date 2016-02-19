#include "SceneExporter.hpp"

#include <GeometryBuffers.hpp>
#include <IndexBuffer.hpp>
#include <MaterialManager.hpp>
#include <MeshManager.hpp>
#include <Pass.hpp>
#include <Submesh.hpp>
#include <SamplerManager.hpp>
#include <Scene.hpp>
#include <TextureUnit.hpp>
#include <Vertex.hpp>
#include <VertexBuffer.hpp>
#include <WindowManager.hpp>

using namespace Castor3D;
using namespace Castor;
using namespace GuiCommon;

namespace CastorViewer
{
	namespace
	{
		template< typename TObj, typename TKey >
		bool ParseCollection( Engine * p_engine, Collection< TObj, TKey > & p_collection, BinaryChunk & p_chunk, typename TObj::BinaryParser p_parser )
		{
			bool l_result = true;
			p_collection.lock();
			auto l_it = p_collection.begin();

			while ( l_result && l_it != p_collection.end() )
			{
				l_result = p_parser.Fill( *l_it->second, p_chunk );
				++l_it;
			}

			p_collection.unlock();
			return l_result;
		}

		template< typename TObj, typename TKey >
		bool ParseManager( Engine * p_engine, ResourceManager< TKey, TObj > & p_manager, BinaryChunk & p_chunk, typename TObj::BinaryParser p_parser )
		{
			bool l_result = true;
			auto l_lock = make_unique_lock( p_manager );
			auto l_it = p_manager.begin();

			while ( l_result && l_it != p_manager.end() )
			{
				l_result = p_parser.Fill( *l_it->second, p_chunk );
				++l_it;
			}

			return l_result;
		}

		Path GetTextureNewPath( Path const & p_pathSrcFile, Path const & p_pathFolder )
		{
			Path l_pathReturn( cuT( "Texture" ) );
			l_pathReturn /= p_pathSrcFile.GetFullFileName();

			if ( !wxDirExists( ( p_pathFolder / cuT( "Texture" ) ).c_str() ) )
			{
				wxMkDir( string::string_cast< char >( p_pathFolder / cuT( "Texture" ) ).c_str(), 0777 );
			}

			if ( wxFileExists( p_pathSrcFile ) )
			{
				Logger::LogDebug( cuT( "Copying [" ) + p_pathSrcFile + cuT( "] to [" ) + p_pathFolder / l_pathReturn + cuT( "]" ) );
				wxCopyFile( p_pathSrcFile, p_pathFolder / l_pathReturn );
			}

			return l_pathReturn;
		}
	}

	//************************************************************************************************

	SceneExporter::SceneExporter()
	{
	}

	SceneExporter::~SceneExporter()
	{
	}

	//************************************************************************************************

	ObjSceneExporter::ObjSceneExporter()
	{
	}

	ObjSceneExporter::~ObjSceneExporter()
	{
	}

	void ObjSceneExporter::ExportScene( Engine * p_castor3d, Scene const & p_scene, Path const & p_fileName )
	{
		String l_fileName = make_String( p_fileName );
		StringStream l_mtl, l_obj;
		Version l_version;
		StringStream l_streamVersion;
		l_streamVersion << l_version.m_iMajor << cuT( "." ) << l_version.m_iMinor << cuT( "." ) << l_version.m_iBuild;
		// First we export the materials
		Path l_pathFileMtl( l_fileName );
		string::replace( l_pathFileMtl, l_pathFileMtl.GetExtension(), cuT( "mtl" ) );
		MaterialManager const & l_mtlManager = p_castor3d->GetMaterialManager();
		l_mtl << cuT( "#######################################################\n" );
		l_mtl << cuT( "#      MTL File generated by CastorViewer v" ) + l_streamVersion.str() + cuT( "      #\n" );
		l_mtl << cuT( "#######################################################\n\n" );
		l_mtlManager.lock();

		for ( auto && l_pair : l_mtlManager )
		{
			l_mtl << DoExportMaterial( l_pathFileMtl.GetPath(), *l_pair.second ) + cuT( "\n" );
		}

		l_mtlManager.unlock();
		TextFile l_fileMtl( l_pathFileMtl, File::eOPEN_MODE_WRITE, File::eENCODING_MODE_ASCII );
		l_fileMtl.WriteText( l_mtl.str() );
		// Then we export meshes
		Path l_pathFileObj( l_fileName );
		string::replace( l_pathFileObj, l_pathFileObj.GetExtension(), cuT( "obj" ) );
		MeshManager const & l_mshManager = p_castor3d->GetMeshManager();
		uint32_t l_uiOffset = 1;
		uint32_t l_count = 0;
		l_obj << cuT( "#######################################################\n" );
		l_obj << cuT( "#      OBJ File generated by CastorViewer v" ) + l_streamVersion.str() + cuT( "      #\n" );
		l_obj << cuT( "#######################################################\n\n" );
		l_obj << cuT( "mtllib " ) + l_pathFileMtl.GetFullFileName() + cuT( "\n\n" );
		l_mshManager.lock();

		for ( auto && l_pair : l_mshManager )
		{
			l_obj << DoExportMesh( *l_pair.second, l_uiOffset, l_count ) << cuT( "\n" );
		}

		l_mshManager.unlock();
		TextFile l_fileObj( l_pathFileObj, File::eOPEN_MODE_WRITE, File::eENCODING_MODE_ASCII );
		l_fileObj.WriteText( l_obj.str() );
	}

	String ObjSceneExporter::DoExportMaterial( Path const & p_pathMtlFolder, Material const & p_material )const
	{
		StringStream l_strReturn;
		PassSPtr l_pPass = p_material.GetPass( 0 );

		if ( l_pPass )
		{
			Colour l_clAmbient = l_pPass->GetAmbient();
			Colour l_clDiffuse = l_pPass->GetDiffuse();
			Colour l_clSpecular = l_pPass->GetSpecular();
			TextureUnitSPtr l_pAmbient = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_AMBIENT );
			TextureUnitSPtr l_pDiffuse = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_DIFFUSE );
			TextureUnitSPtr l_pNormal = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_NORMAL );
			TextureUnitSPtr l_pOpacity = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_OPACITY );
			TextureUnitSPtr l_pSpecular = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_SPECULAR );
			TextureUnitSPtr l_pEmissive = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_EMISSIVE );
			TextureUnitSPtr l_pGloss = l_pPass->GetTextureUnit( eTEXTURE_CHANNEL_GLOSS );
			l_strReturn << cuT( "newmtl " ) << p_material.GetName() << cuT( "\n" );
			l_strReturn << cuT( "	Ka " ) << l_clAmbient.red().value() << cuT( " " ) << l_clAmbient.green().value() << cuT( " " ) << l_clAmbient.blue().value() << cuT( "\n" );
			l_strReturn << cuT( "	Kd " ) << l_clDiffuse.red().value() << cuT( " " ) << l_clDiffuse.green().value() << cuT( " " ) << l_clDiffuse.blue().value() << cuT( "\n" );
			l_strReturn << cuT( "	Ks " ) << l_clSpecular.red().value() << cuT( " " ) << l_clSpecular.green().value() << cuT( " " ) << l_clSpecular.blue().value() << cuT( "\n" );
			l_strReturn << cuT( "	Ns " ) << l_pPass->GetShininess() << cuT( "\n" );
			l_strReturn << cuT( "	d " ) << l_pPass->GetAlpha() << cuT( "\n" );

			if ( l_pAmbient && !l_pAmbient->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_Ka " ) + GetTextureNewPath( l_pAmbient->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}

			if ( l_pDiffuse && !l_pDiffuse->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_Kd " ) + GetTextureNewPath( l_pDiffuse->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}

			if ( l_pNormal && !l_pNormal->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_Bump " ) + GetTextureNewPath( l_pNormal->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}

			if ( l_pOpacity && !l_pOpacity->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_d " ) + GetTextureNewPath( l_pOpacity->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}

			if ( l_pSpecular && !l_pSpecular->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_Ks " ) + GetTextureNewPath( l_pSpecular->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}

			if ( l_pGloss && !l_pGloss->GetTexturePath().empty() )
			{
				l_strReturn << cuT( "	map_Ns " ) + GetTextureNewPath( l_pGloss->GetTexturePath(), p_pathMtlFolder ) << cuT( "\n" );
			}
		}

		return l_strReturn.str();
	}

	String ObjSceneExporter::DoExportMesh( Mesh const & p_mesh, uint32_t & p_offset, uint32_t & p_count )const
	{
		StringStream l_strReturn;

		for ( auto && l_submesh : p_mesh )
		{
			StringStream l_strV;
			StringStream l_strVT;
			StringStream l_strVN;
			StringStream l_strF;
			VertexBuffer & l_vtxBuffer = l_submesh->GetVertexBuffer();
			IndexBuffer & l_idxBuffer = l_submesh->GetIndexBuffer();
			uint32_t l_stride = l_vtxBuffer.GetDeclaration().GetStride();
			uint32_t l_uiNbPoints = l_vtxBuffer.GetSize() / l_stride;
			uint32_t l_uiNbFaces = l_idxBuffer.GetSize() / 3;
			uint8_t * l_pVtx = l_vtxBuffer.data();
			uint32_t * l_pIdx = l_idxBuffer.data();
			Point3r l_ptPos;
			Point3r l_ptNml;
			Point3r l_ptTex;

			for ( uint32_t j = 0; j < l_uiNbPoints; j++ )
			{
				real * l_vertex = reinterpret_cast< real * >( &l_pVtx[j * l_stride] );
				Vertex::GetPosition( l_vertex, l_ptPos );
				Vertex::GetNormal( l_vertex, l_ptNml );
				Vertex::GetTexCoord( l_vertex, l_ptTex );
				l_strV  << cuT( "v " ) << l_ptPos[0] << " " << l_ptPos[1] << " " << l_ptPos[2] << cuT( "\n" );
				l_strVN << cuT( "vn " ) << l_ptNml[0] << " " << l_ptNml[1] << " " << l_ptNml[2] << cuT( "\n" );
				l_strVT << cuT( "vt " ) << l_ptTex[0] << " " << l_ptTex[1] << cuT( "\n" );
			}

			l_strF << cuT( "usemtl " ) << l_submesh->GetDefaultMaterial()->GetName() << cuT( "\ns off\n" );

			for ( uint32_t j = 0; j < l_uiNbFaces; j++ )
			{
				uint32_t * l_pFace = &l_pIdx[j * 3];
				uint32_t l_v0 = p_offset + l_pFace[0];
				uint32_t l_v1 = p_offset + l_pFace[1];
				uint32_t l_v2 = p_offset + l_pFace[2];
				l_strF << cuT( "f " ) << l_v0 << cuT( "/" ) << l_v0 << cuT( "/" ) << l_v0 << cuT( " " ) << l_v1 << cuT( "/" ) << l_v1 << cuT( "/" ) << l_v1 << cuT( " " ) << l_v2 << cuT( "/" ) << l_v2 << cuT( "/" ) << l_v2 << cuT( "\n" );
			}

			l_strReturn << cuT( "g mesh" ) << p_count << cuT( "\n" ) << l_strV.str() << cuT( "\n" ) << l_strVN.str() << cuT( "\n" ) << l_strVT.str() << cuT( "\n" ) << l_strF.str() << cuT( "\n" );
			p_offset += l_uiNbPoints;
			p_count++;
		}

		return l_strReturn.str();
	}

	//************************************************************************************************

	CscnSceneExporter::CscnSceneExporter()
	{
	}

	CscnSceneExporter::~CscnSceneExporter()
	{
	}

	void CscnSceneExporter::ExportScene( Engine * p_castor3d, Scene const & p_scene, Path const & p_fileName )
	{
		bool l_result = true;
		Path l_folder = p_fileName.GetPath() / p_fileName.GetFileName();

		if ( !File::DirectoryExists( l_folder ) )
		{
			File::DirectoryCreate( l_folder );
		}

		Path l_filePath = l_folder / p_fileName.GetFileName();
		TextFile l_mtlFile( l_filePath + cuT( ".cmtl" ), File::eOPEN_MODE_WRITE, File::eENCODING_MODE_ASCII );
		TextFile l_scnFile( l_filePath + cuT( ".cscn" ), File::eOPEN_MODE_WRITE, File::eENCODING_MODE_ASCII );
		l_result = p_castor3d->GetMaterialManager().Write( l_mtlFile );

		if ( l_result )
		{
			if ( l_scnFile.WriteText( cuT( "mtl_file \"" ) + p_fileName.GetFileName() + cuT( ".cmtl\"\n\n" ) ) > 0 )
			{
				l_result = Scene::TextLoader()( p_scene, l_scnFile );
			}
		}

		p_castor3d->GetWindowManager().lock();

		for ( auto l_it : p_castor3d->GetWindowManager() )
		{
			l_result &= RenderWindow::TextLoader()( *l_it.second, l_scnFile );
		}

		p_castor3d->GetWindowManager().unlock();

		if ( l_result )
		{
			wxMessageBox( _( "Export successful" ) );
		}
		else
		{
			wxMessageBox( _( "Export failed" ) );
		}
	}

	//************************************************************************************************

	CbsnSceneExporter::CbsnSceneExporter()
	{
	}

	CbsnSceneExporter::~CbsnSceneExporter()
	{
	}

	void CbsnSceneExporter::ExportScene( Engine * p_castor3d, Scene const & p_scene, Path const & p_fileName )
	{
		Path l_folder = p_fileName.GetPath() / p_fileName.GetFileName();

		if ( !File::DirectoryExists( l_folder ) )
		{
			File::DirectoryCreate( l_folder );
		}

		Path l_filePath = l_folder / p_fileName.GetFileName();
		bool l_result = true;
		BinaryChunk l_chunk( eCHUNK_TYPE_CBSN_FILE );
		ParseManager( p_castor3d, p_castor3d->GetSamplerManager(), l_chunk, Sampler::BinaryParser( l_folder ) );
		ParseManager( p_castor3d, p_castor3d->GetMaterialManager(), l_chunk, Material::BinaryParser( l_folder, p_castor3d ) );
		ParseManager( p_castor3d, p_castor3d->GetMeshManager(), l_chunk, Mesh::BinaryParser( l_folder ) );

		if ( l_result )
		{
			l_result = Scene::BinaryParser( l_folder ).Fill( const_cast< Scene const & >( p_scene ), l_chunk );
		}

		p_castor3d->GetWindowManager().lock();

		for ( auto l_it : p_castor3d->GetWindowManager() )
		{
			l_result &= RenderWindow::BinaryParser( l_folder ).Fill( *l_it.second, l_chunk );
		}

		p_castor3d->GetWindowManager().unlock();

		if ( l_result )
		{
			BinaryFile l_file( l_filePath + cuT( ".cbsn" ), File::eOPEN_MODE_WRITE );
			l_chunk.Write( l_file );
		}

		if ( l_result )
		{
			wxMessageBox( _( "Export successful" ) );
		}
		else
		{
			wxMessageBox( _( "Export failed" ) );
		}
	}
}
