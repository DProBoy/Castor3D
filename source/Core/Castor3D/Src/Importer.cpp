#include "Importer.hpp"

#include "Engine.hpp"
#include "InitialiseEvent.hpp"
#include "Mesh.hpp"
#include "Submesh.hpp"

using namespace Castor3D;
using namespace Castor;

Importer::Importer( Engine & p_engine )
	: OwnedBy< Engine >( p_engine )
	, m_fileName()
{
}

SceneSPtr Importer::ImportScene( Path const & p_fileName, Parameters const & p_parameters )
{
	m_fileName = p_fileName;
	m_filePath = m_fileName.GetPath();
	m_parameters = p_parameters;
	m_nodes.clear();
	m_geometries.clear();
	return DoImportScene();
}

MeshSPtr Importer::ImportMesh( Path const & p_fileName, Parameters const & p_parameters )
{
	m_fileName = p_fileName;
	m_filePath = m_fileName.GetPath();
	m_parameters = p_parameters;
	m_nodes.clear();
	m_geometries.clear();
	MeshSPtr l_mesh = DoImportMesh();

	for ( auto && l_submesh : *l_mesh )
	{
		GetEngine()->PostEvent( MakeInitialiseEvent( *l_submesh ) );
	}

	return l_mesh;
}
