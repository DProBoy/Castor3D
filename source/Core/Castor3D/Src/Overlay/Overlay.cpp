#include "Overlay.hpp"

#include "BorderPanelOverlay.hpp"
#include "Engine.hpp"
#include "OverlayFactory.hpp"
#include "OverlayManager.hpp"
#include "PanelOverlay.hpp"
#include "TextOverlay.hpp"

#include <TransformationMatrix.hpp>

using namespace Castor;

namespace Castor3D
{
	//*************************************************************************************************

	Overlay::TextLoader::TextLoader( File::eENCODING_MODE p_encodingMode )
		:	Loader< Overlay, eFILE_TYPE_TEXT, TextFile >( File::eOPEN_MODE_DUMMY, p_encodingMode )
	{
	}

	bool Overlay::TextLoader::operator()( Overlay const & p_overlay, TextFile & p_file )
	{
		return OverlayCategory::TextLoader()( *p_overlay.m_category, p_file );
	}

	//*************************************************************************************************

	Overlay::Overlay( Engine & p_engine, eOVERLAY_TYPE p_type )
		: OwnedBy< Engine >( p_engine )
		, m_manager( p_engine.GetOverlayManager() )
		, m_parent()
		, m_pScene()
		, m_renderSystem( p_engine.GetRenderSystem() )
		, m_category( p_engine.GetOverlayManager().GetFactory().Create( p_type ) )
	{
		m_category->SetOverlay( this );
	}

	Overlay::Overlay( Engine & p_engine, eOVERLAY_TYPE p_type, SceneSPtr p_scene, OverlaySPtr p_parent )
		: OwnedBy< Engine >( p_engine )
		, m_manager( p_engine.GetOverlayManager() )
		, m_parent( p_parent )
		, m_pScene( p_scene )
		, m_renderSystem( p_engine.GetRenderSystem() )
		, m_category( p_engine.GetOverlayManager().GetFactory().Create( p_type ) )
	{
		m_category->SetOverlay( this );
	}

	Overlay::~Overlay()
	{
	}

	void Overlay::Render( Castor::Size const & p_size )
	{
		if ( IsVisible() )
		{
			m_category->Render();

			for ( auto && l_overlay : m_overlays )
			{
				l_overlay->Render( p_size );
			}
		}
	}

	bool Overlay::AddChild( OverlaySPtr p_overlay )
	{
		bool l_return = false;
		int l_index = 1;

		if ( !m_overlays.empty() )
		{
			l_index = ( *( m_overlays.end() - 1 ) )->GetIndex() + 1;
		}

		p_overlay->SetOrder( l_index, GetLevel() + 1 );
		m_overlays.push_back( p_overlay );
		return true;
	}

	int Overlay::GetChildsCount( int p_level )const
	{
		int l_return = 0;

		if ( p_level == GetLevel() + 1 )
		{
			l_return = int( m_overlays.size() );
		}
		else if ( p_level > GetLevel() )
		{
			for ( auto && l_overlay : m_overlays )
			{
				l_return += l_overlay->GetChildsCount( p_level );
			}
		}

		return l_return;
	}

	PanelOverlaySPtr Overlay::GetPanelOverlay()const
	{
		if ( m_category->GetType() != eOVERLAY_TYPE_PANEL )
		{
			CASTOR_EXCEPTION( "This overlay is not a panel." );
		}

		return std::static_pointer_cast< PanelOverlay >( m_category );
	}

	BorderPanelOverlaySPtr Overlay::GetBorderPanelOverlay()const
	{
		if ( m_category->GetType() != eOVERLAY_TYPE_BORDER_PANEL )
		{
			CASTOR_EXCEPTION( "This overlay is not a border panel." );
		}

		return std::static_pointer_cast< BorderPanelOverlay >( m_category );
	}

	TextOverlaySPtr Overlay::GetTextOverlay()const
	{
		if ( m_category->GetType() != eOVERLAY_TYPE_TEXT )
		{
			CASTOR_EXCEPTION( "This overlay is not a text." );
		}

		return std::static_pointer_cast< TextOverlay >( m_category );
	}

	bool Overlay::IsVisible()const
	{
		bool l_return = m_category->IsVisible();

		if ( l_return && GetParent() )
		{
			l_return = GetParent()->IsVisible();
		}

		return l_return;
	}
}
