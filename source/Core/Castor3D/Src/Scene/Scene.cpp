#include "SceneManager.hpp"

#include "AnimatedObjectGroupManager.hpp"
#include "BlendStateManager.hpp"
#include "CameraManager.hpp"
#include "BillboardManager.hpp"
#include "DepthStencilStateManager.hpp"
#include "Engine.hpp"
#include "GeometryManager.hpp"
#include "LightManager.hpp"
#include "MaterialManager.hpp"
#include "MeshManager.hpp"
#include "OverlayManager.hpp"
#include "SamplerManager.hpp"
#include "SceneNodeManager.hpp"
#include "ShaderManager.hpp"
#include "WindowManager.hpp"

#include "Skybox.hpp"

#include "Animation/AnimatedObject.hpp"
#include "Animation/Animation.hpp"
#include "Animation/Bone.hpp"
#include "Animation/Skeleton.hpp"
#include "Event/Frame/CleanupEvent.hpp"
#include "Event/Frame/FunctorEvent.hpp"
#include "Event/Frame/InitialiseEvent.hpp"
#include "Material/Pass.hpp"
#include "Mesh/Face.hpp"
#include "Mesh/Importer.hpp"
#include "Mesh/Submesh.hpp"
#include "Mesh/Vertex.hpp"
#include "Mesh/Buffer/Buffer.hpp"
#include "Miscellaneous/Ray.hpp"
#include "Render/Pipeline.hpp"
#include "Render/RenderSystem.hpp"
#include "Scene/Light/DirectionalLight.hpp"
#include "Scene/Light/PointLight.hpp"
#include "Scene/Light/SpotLight.hpp"
#include "Shader/FrameVariable.hpp"
#include "Shader/FrameVariableBuffer.hpp"
#include "Shader/MatrixFrameVariable.hpp"
#include "Shader/OneFrameVariable.hpp"
#include "Shader/PointFrameVariable.hpp"
#include "Texture/TextureLayout.hpp"
#include "Texture/TextureLayout.hpp"
#include "Texture/TextureUnit.hpp"
#include "Manager/ManagerView.hpp"

#include <Image.hpp>
#include <Logger.hpp>

using namespace Castor;

namespace Castor3D
{
	Scene::TextLoader::TextLoader( String const & p_tabs, File::eENCODING_MODE p_encodingMode )
		: Castor::TextLoader< Scene >( p_tabs, p_encodingMode )
	{
	}

	bool Scene::TextLoader::operator()( Scene const & p_scene, TextFile & p_file )
	{
		Logger::LogInfo( cuT( "Scene::Write - Scene Name" ) );
		bool l_return = p_file.WriteText( m_tabs + cuT( "scene \"" ) + p_scene.GetName() + cuT( "\"\n{\n" ) ) > 0;

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Samplers" ) );
			for ( auto const & l_name : p_scene.GetSamplerView() )
			{
				auto l_sampler = p_scene.GetSamplerView().Find( l_name );
				l_return &= Sampler::TextLoader( m_tabs + cuT( "\t" ) )( *l_sampler, p_file );
				p_file.WriteText( cuT( "\n" ) );
			}
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Materials" ) );
			for ( auto const & l_name : p_scene.GetMaterialView() )
			{
				auto l_material = p_scene.GetMaterialView().Find( l_name );
				l_return &= Material::TextLoader( m_tabs + cuT( "\t" ) )( *l_material, p_file );
				p_file.WriteText( cuT( "\n" ) );
			}
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Background colour" ) );
			l_return = p_file.Print( 256, cuT( "%s\tbackground_colour " ), m_tabs.c_str() ) > 0
				&& Colour::TextLoader( String() )( p_scene.GetBackgroundColour(), p_file )
				&& p_file.WriteText( cuT( "\n" ) ) > 0;
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Ambient light" ) );
			l_return = p_file.Print( 256, cuT( "%s\tambient_light " ), m_tabs.c_str() ) > 0
				&& Colour::TextLoader( String() )( p_scene.GetAmbientLight(), p_file )
				&& p_file.WriteText( cuT( "\n" ) ) > 0;
			p_file.WriteText( cuT( "\n" ) );
		}

		Logger::LogInfo( cuT( "Scene::Write - Cameras nodes" ) );
		for ( auto const & l_it : p_scene.GetCameraRootNode()->GetChilds() )
		{
			l_return &= SceneNode::TextLoader( m_tabs + cuT( "\t" ) )( *l_it.second.lock(), p_file );
			p_file.WriteText( cuT( "\n" ) );
		}

		Logger::LogInfo( cuT( "Scene::Write - Objects nodes" ) );
		for ( auto const & l_it : p_scene.GetObjectRootNode()->GetChilds() )
		{
			l_return &= SceneNode::TextLoader( m_tabs + cuT( "\t" ) )( *l_it.second.lock(), p_file );
			p_file.WriteText( cuT( "\n" ) );
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Cameras" ) );
			auto l_lock = make_unique_lock( p_scene.GetCameraManager() );

			for ( auto const & l_it : p_scene.GetCameraManager() )
			{
				l_return &= Camera::TextLoader( m_tabs + cuT( "\t" ) )( *l_it.second, p_file );
				p_file.WriteText( cuT( "\n" ) );
			}
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Lights" ) );
			auto l_lock = make_unique_lock( p_scene.GetLightManager() );

			for ( auto const & l_it : p_scene.GetLightManager() )
			{
				auto l_light = l_it.second;

				switch ( l_light->GetLightType() )
				{
				case eLIGHT_TYPE_DIRECTIONAL:
					l_return &= DirectionalLight::TextLoader( m_tabs + cuT( "\t" ) )( *l_light->GetDirectionalLight(), p_file );
					break;

				case eLIGHT_TYPE_POINT:
					l_return &= PointLight::TextLoader( m_tabs + cuT( "\t" ) )( *l_light->GetPointLight(), p_file );
					break;

				case eLIGHT_TYPE_SPOT:
					l_return &= SpotLight::TextLoader( m_tabs + cuT( "\t" ) )( *l_light->GetSpotLight(), p_file );
					break;

				default:
					l_return = false;
					break;
				}

				p_file.WriteText( cuT( "\n" ) );
			}
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Geometries" ) );
			auto l_lock = make_unique_lock( p_scene.GetGeometryManager() );

			for ( auto const & l_it : p_scene.GetGeometryManager() )
			{
				l_return &= Geometry::TextLoader( m_tabs + cuT( "\t" ) )( *l_it.second, p_file );
				p_file.WriteText( cuT( "\n" ) );
			}
		}

		if ( l_return )
		{
			Logger::LogInfo( cuT( "Scene::Write - Windows" ) );
			for ( auto const & l_name : p_scene.GetRenderWindowView() )
			{
				auto l_window = p_scene.GetRenderWindowView().Find( l_name );
				l_return &= RenderWindow::TextLoader( m_tabs + cuT( "\t" ) )( *l_window, p_file );
				p_file.WriteText( cuT( "\n" ) );
			}
		}

		p_file.WriteText( cuT( "}\n\n" ) );
		return l_return;
	}

	//*************************************************************************************************

	String Scene::RootNode = cuT( "RootNode" );
	String Scene::CameraRootNode = cuT( "CameraRootNode" );
	String Scene::ObjectRootNode = cuT( "ObjectRootNode" );

	Scene::Scene( String const & p_name, Engine & p_engine )
		: OwnedBy< Engine >{ p_engine }
		, Named( p_name )
		, m_rootCameraNode()
		, m_rootObjectNode()
		, m_changed( false )
	{
		m_rootNode = std::make_shared< SceneNode >( RootNode, *this );
		m_rootCameraNode = std::make_shared< SceneNode >( CameraRootNode, *this );
		m_rootObjectNode = std::make_shared< SceneNode >( ObjectRootNode, *this );
		m_rootCameraNode->AttachTo( m_rootNode );
		m_rootObjectNode->AttachTo( m_rootNode );

		m_animatedObjectGroupManager = std::make_unique< AnimatedObjectGroupManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );
		m_billboardManager = std::make_unique< BillboardManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );
		m_cameraManager = std::make_unique< CameraManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );
		m_geometryManager = std::make_unique< GeometryManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );
		m_lightManager = std::make_unique< LightManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );
		m_sceneNodeManager = std::make_unique< SceneNodeManager >( *this, m_rootNode, m_rootCameraNode, m_rootObjectNode );

		m_meshManagerView = std::make_unique< ManagerView< Mesh, MeshManager, eEVENT_TYPE_PRE_RENDER > >( GetName(), GetEngine()->GetMeshManager() );
		m_materialManagerView = std::make_unique< ManagerView< Material, MaterialManager, eEVENT_TYPE_PRE_RENDER > >( GetName(), GetEngine()->GetMaterialManager() );
		m_samplerManagerView = std::make_unique< ManagerView< Sampler, SamplerManager, eEVENT_TYPE_PRE_RENDER > >( GetName(), GetEngine()->GetSamplerManager() );
		m_windowManagerView = std::make_unique< ManagerView< RenderWindow, WindowManager, eEVENT_TYPE_POST_RENDER > >( GetName(), GetEngine()->GetWindowManager() );

		auto l_notify = [this]()
		{
			m_changed = true;
		};

		m_sceneNodeManager->Insert( cuT( "ObjectRootNode" ), m_rootObjectNode );
	}

	Scene::~Scene()
	{
		m_skybox.reset();
		m_animatedObjectGroupManager.reset();
		m_billboardManager.reset();
		m_cameraManager.reset();
		m_geometryManager.reset();
		m_lightManager.reset();
		m_sceneNodeManager.reset();

		m_meshManagerView.reset();
		m_materialManagerView.reset();
		m_samplerManagerView.reset();
		m_windowManagerView.reset();

		if ( m_rootCameraNode )
		{
			m_rootCameraNode->Detach();
			m_rootCameraNode.reset();
		}

		if ( m_rootObjectNode )
		{
			m_rootObjectNode->Detach();
			m_rootObjectNode.reset();
		}

		if ( m_rootNode )
		{
			m_rootNode->Detach();
		}

		m_rootNode.reset();
	}

	void Scene::Initialise()
	{
	}

	void Scene::Cleanup()
	{
		auto l_lock = Castor::make_unique_lock( m_mutex );
		m_overlays.clear();
		m_animatedObjectGroupManager->Cleanup();
		m_cameraManager->Cleanup();
		m_billboardManager->Cleanup();
		m_geometryManager->Cleanup();
		m_lightManager->Cleanup();
		m_sceneNodeManager->Cleanup();
		m_meshManagerView->Clear();
		m_materialManagerView->Clear();
		m_samplerManagerView->Clear();
		m_windowManagerView->Clear();

		if ( m_backgroundImage )
		{
			GetEngine()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [this]()
			{
				m_backgroundImage->Cleanup();
				m_backgroundImage->Destroy();
			} ) );
		}

		if ( m_skybox )
		{
			GetEngine()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [this]()
			{
				m_skybox->Cleanup();
			} ) );
		}
	}

	void Scene::RenderBackground( Size const & p_size, Pipeline & p_pipeline )
	{
		if ( m_backgroundImage )
		{
			if ( m_backgroundImage->IsInitialised() )
			{
				ContextRPtr l_context = GetEngine()->GetRenderSystem()->GetCurrentContext();
				l_context->GetNoDepthState()->Apply();
				l_context->RenderTexture( p_size, *m_backgroundImage );
			}
		}
	}

	void Scene::RenderForeground( Size const & p_size, Camera const & p_camera, Pipeline & p_pipeline )
	{
		if ( m_skybox )
		{
			m_skybox->Render( p_camera, p_pipeline );
		}
	}

	void Scene::Update()
	{
		m_animatedObjectGroupManager->Update();
		m_changed = false;
	}

	bool Scene::SetBackground( Path const & p_pathFile )
	{
		bool l_return = false;

		try
		{
			auto l_texture = GetEngine()->GetRenderSystem()->CreateTexture( eTEXTURE_TYPE_2D, 0, eACCESS_TYPE_READ );
			l_texture->GetImage().SetSource( p_pathFile );
			m_backgroundImage = l_texture;
			GetEngine()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [this]()
			{
				m_backgroundImage->Create();
				m_backgroundImage->Initialise();
				m_backgroundImage->Bind( 0 );
				m_backgroundImage->GenerateMipmaps();
				m_backgroundImage->Unbind( 0 );
			} ) );
			l_return = true;
		}
		catch ( Castor::Exception & p_exc )
		{
			Logger::LogError( p_exc.what() );
		}

		return l_return;
	}

	bool Scene::SetForeground( SkyboxSPtr p_skybox )
	{
		m_skybox = p_skybox;
		GetEngine()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [p_skybox]()
		{
			p_skybox->Initialise();
		} ) );
		return true;
	}

	void Scene::Merge( SceneSPtr p_scene )
	{
		{
			auto l_lock = Castor::make_unique_lock( m_mutex );
			auto l_lockOther = Castor::make_unique_lock( p_scene->m_mutex );
			p_scene->GetAnimatedObjectGroupManager().MergeInto( *m_animatedObjectGroupManager );
			p_scene->GetCameraManager().MergeInto( *m_cameraManager );
			p_scene->GetBillboardManager().MergeInto( *m_billboardManager );
			p_scene->GetGeometryManager().MergeInto( *m_geometryManager );
			p_scene->GetLightManager().MergeInto( *m_lightManager );
			p_scene->GetSceneNodeManager().MergeInto( *m_sceneNodeManager );
			m_ambientLight = p_scene->GetAmbientLight();
			m_changed = true;
		}

		p_scene->Cleanup();
	}

	bool Scene::ImportExternal( String const & p_fileName, Importer & p_importer )
	{
		auto l_lock = Castor::make_unique_lock( m_mutex );
		bool l_return = true;
		SceneSPtr l_scene = p_importer.ImportScene( p_fileName, Parameters() );

		if ( l_scene )
		{
			Merge( l_scene );
			m_changed = true;
			l_return = true;
		}

		return l_return;
	}

	MeshSPtr Scene::ImportMesh( Castor::Path const & p_fileName, Importer & p_importer, Parameters const & p_parameters )
	{
		auto l_return = p_importer.ImportMesh( *this, p_fileName, p_parameters );

		if ( l_return )
		{
			m_meshManagerView->Insert( l_return->GetName(), l_return );
		}

		return l_return;
	}

	void Scene::AddOverlay( OverlaySPtr p_overlay )
	{
		auto l_lock = Castor::make_unique_lock( m_mutex );
		m_overlays.push_back( p_overlay );
	}

	uint32_t Scene::GetVertexCount()const
	{
		uint32_t l_return = 0;
		auto l_lock = make_unique_lock( *m_geometryManager );

		for ( auto && l_pair : *m_geometryManager )
		{
			auto l_mesh = l_pair.second->GetMesh();

			if ( l_mesh )
			{
				l_return += l_pair.second->GetMesh()->GetVertexCount();
			}
		}

		return l_return;
	}

	uint32_t Scene::GetFaceCount()const
	{
		uint32_t l_return = 0;
		auto l_lock = make_unique_lock( *m_geometryManager );

		for ( auto && l_pair : *m_geometryManager )
		{
			auto l_mesh = l_pair.second->GetMesh();

			if ( l_mesh )
			{
				l_return += l_mesh->GetFaceCount();
			}
		}

		return l_return;
	}
}
