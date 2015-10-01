#include "CtrlComboBox.hpp"

#include "ControlsManager.hpp"
#include "CtrlButton.hpp"
#include "CtrlListBox.hpp"

#include <BorderPanelOverlay.hpp>
#include <TextOverlay.hpp>

using namespace Castor;
using namespace Castor3D;

namespace CastorGui
{
	ComboBoxCtrl::ComboBoxCtrl( ControlSPtr p_parent, uint32_t p_id )
		: Control( eCONTROL_TYPE_COMBO, p_parent, p_id )
	{
	}

	ComboBoxCtrl::ComboBoxCtrl( ControlSPtr p_parent, uint32_t p_id, StringArray const & p_values, int p_selected, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_COMBO, p_parent, p_id, p_position, p_size, p_style, p_visible )
		, m_values( p_values )
		, m_selected( p_selected )
	{
	}

	ComboBoxCtrl::~ComboBoxCtrl()
	{
	}

	void ComboBoxCtrl::AppendItem( String  const & p_value )
	{
		m_choices->AppendItem( p_value );
	}

	void ComboBoxCtrl::RemoveItem( int p_value )
	{
		m_choices->RemoveItem( p_value );
	}

	void ComboBoxCtrl::SetItemText( int p_index, String const & p_text )
	{
		return m_choices->SetItemText( p_index, p_text );
	}

	void ComboBoxCtrl::Clear()
	{
		return m_choices->Clear();
	}

	void ComboBoxCtrl::SetSelected( int p_value )
	{
		return m_choices->SetSelected( p_value );
	}

	StringArray const & ComboBoxCtrl::GetItems()const
	{
		return m_choices->GetItems();
	}

	uint32_t ComboBoxCtrl::GetItemCount()const
	{
		return m_choices->GetItemCount();
	}

	int ComboBoxCtrl::GetSelected()const
	{
		return m_choices->GetSelected();
	}

	void ComboBoxCtrl::DoCreate()
	{
		SetBackgroundBorders( Rectangle( 1, 1, 1, 1 ) );

		m_expand = std::make_shared< ButtonCtrl >( shared_from_this(), GetId() << 12, cuT( "+" ), Position( GetSize().width() - GetSize().height(), 0 ), Size( GetSize().height(), GetSize().height() ) );
		m_expand->SetForegroundMaterial( GetForegroundMaterial() );
		m_expand->SetVisible( DoIsVisible() );
		m_expand->Connect( eBUTTON_EVENT_CLICKED, std::bind( &ComboBoxCtrl::DoSwitchExpand, this ) );

		m_choices = std::make_shared< ListBoxCtrl >( shared_from_this(), ( GetId() << 12 ) + 1, m_values, m_selected, Position( 0, GetSize().height() ), Size( GetSize().width() - GetSize().height(), -1 ), 0, false );
		m_choices->SetBackgroundMaterial( GetBackgroundMaterial() );
		m_choices->SetForegroundMaterial( GetForegroundMaterial() );
		m_choices->Connect( eLISTBOX_EVENT_SELECTED, std::bind( &ComboBoxCtrl::OnSelected, this, std::placeholders::_1 ) );

		EventHandler::Connect( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ComboBoxCtrl::OnKeyDown, this, std::placeholders::_1 ) );
		EventHandler::ConnectNC( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ComboBoxCtrl::OnNcKeyDown, this, std::placeholders::_1, std::placeholders::_2 ) );

		TextOverlaySPtr l_text = GetEngine()->GetOverlayManager().CreateText( cuT( "T_CtrlCombo_" ) + string::to_string( GetId() ), Position(), Size( GetSize().width() - GetSize().height(), GetSize().height() ), GetForegroundMaterial(), GetControlsManager()->GetDefaultFont(), GetBackground()->GetOverlay().shared_from_this() );
		l_text->SetVAlign( eVALIGN_CENTER );
		int l_sel = GetSelected();

		if ( l_sel >= 0 && uint32_t( l_sel ) < GetItemCount() )
		{
			l_text->SetCaption( GetItems()[l_sel] );
		}

		m_text = l_text;
		ControlsManagerSPtr l_manager = GetControlsManager();

		if ( l_manager )
		{
			l_manager->Create( m_expand );
			l_manager->Create( m_choices );
		}
	}

	void ComboBoxCtrl::DoDestroy()
	{
		if ( m_expand )
		{
			m_expand->Destroy();
		}

		if ( m_choices )
		{
			m_choices->Destroy();
		}
	}

	void ComboBoxCtrl::DoSetPosition( Position const & p_value )
	{
		TextOverlaySPtr l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelPosition( p_value );
			l_text.reset();
		}

		m_expand->SetPosition( Position( GetSize().width() - GetSize().height(), 0 ) );
		m_choices->SetPosition( Position( 0, GetSize().height() ) );
	}

	void ComboBoxCtrl::DoSetSize( Size const & p_value )
	{
		TextOverlaySPtr l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelSize( p_value );
			l_text.reset();
		}

		m_expand->SetSize( Size( p_value.height(), p_value.height() ) );
		m_choices->SetSize( Size( p_value.width() - p_value.height(), -1 ) );
		m_expand->SetPosition( Position( p_value.width() - p_value.height(), 0 ) );
		m_choices->SetPosition( Position( 0, p_value.height() ) );
	}

	void ComboBoxCtrl::DoSetBackgroundMaterial( MaterialSPtr p_material )
	{
		m_choices->SetBackgroundMaterial( GetBackgroundMaterial() );
	}

	void ComboBoxCtrl::DoSetForegroundMaterial( MaterialSPtr p_material )
	{
		m_expand->SetForegroundMaterial( GetForegroundMaterial() );
		m_choices->SetForegroundMaterial( GetForegroundMaterial() );
		TextOverlaySPtr l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetMaterial( p_material );
			l_text.reset();
		}
	}

	bool ComboBoxCtrl::DoCatchesMouseEvents()const
	{
		return false;
	}

	void ComboBoxCtrl::OnKeyDown( KeyboardEvent const & p_event )
	{
		if ( GetSelected() != -1 )
		{
			bool l_changed = false;
			int l_index = GetSelected();

			if ( p_event.GetKey() == eKEY_UP )
			{
				l_index--;
				l_changed = true;
			}
			else if ( p_event.GetKey() == eKEY_DOWN )
			{
				l_index++;
				l_changed = true;
			}

			if ( l_changed )
			{
				l_index = std::max( 0, std::min( l_index, int( GetItemCount() - 1 ) ) );
				SetSelected( l_index );
				OnSelected( l_index );
			}
		}
	}

	void ComboBoxCtrl::OnNcKeyDown( ControlSPtr p_control, KeyboardEvent const & p_event )
	{
		OnKeyDown( p_event );
	}

	void ComboBoxCtrl::DoSetVisible( bool p_visible )
	{
		m_expand->SetVisible( p_visible );
		m_choices->Hide();
	}

	void ComboBoxCtrl::DoSwitchExpand()
	{
		m_choices->SetVisible( !m_choices->IsVisible() );
	}

	void ComboBoxCtrl::OnSelected( int p_selected )
	{
		if ( p_selected >= 0 )
		{
			DoSwitchExpand();
			TextOverlaySPtr l_text = m_text.lock();

			if ( l_text )
			{
				l_text->SetCaption( m_choices->GetItemText( p_selected ) );
			}
		}

		m_signals[eCOMBOBOX_EVENT_SELECTED]( p_selected );
	}
}
