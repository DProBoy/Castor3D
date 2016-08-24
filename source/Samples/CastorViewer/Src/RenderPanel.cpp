#include "RenderPanel.hpp"
#include "CastorViewer.hpp"
#include "MainFrame.hpp"
#include "RotateNodeEvent.hpp"
#include "TranslateNodeEvent.hpp"
#include "KeyboardEvent.hpp"

#include <algorithm>

#include <wx/display.h>

#include <Engine.hpp>
#include <Event/Frame/FrameListener.hpp>
#include <Event/Frame/FunctorEvent.hpp>
#include <Event/UserInput/UserInputListener.hpp>
#include <Mesh/Submesh.hpp>
#include <Miscellaneous/WindowHandle.hpp>
#include <Render/RenderWindow.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Scene.hpp>
#include <Scene/SceneNode.hpp>

#include <Math/Math.hpp>
#include <Miscellaneous/Utils.hpp>

#define ID_NEW_WINDOW 10000

using namespace Castor3D;
using namespace Castor;

namespace CastorViewer
{
	namespace
	{
		static const real MAX_CAM_SPEED = 10.0_r;
		static const real DEF_CAM_SPEED = 2.0_r;
		static const real MIN_CAM_SPEED = 0.1_r;
		static const real CAM_SPEED_INC = 0.9_r;
	}

	namespace
	{
		eKEYBOARD_KEY ConvertKeyCode( int p_code )
		{
			eKEYBOARD_KEY l_return = eKEYBOARD_KEY_NONE;

			if ( p_code < 0x20 )
			{
				switch ( p_code )
				{
				case WXK_BACK:
				case WXK_TAB:
				case WXK_RETURN:
				case WXK_ESCAPE:
					l_return = eKEYBOARD_KEY( p_code );
					break;
				}
			}
			else if ( p_code == 0x7F )
			{
				l_return = eKEY_DELETE;
			}
			else if ( p_code > 0xFF )
			{
				l_return = eKEYBOARD_KEY( p_code + eKEY_START - WXK_START );
			}
			else
			{
				// ASCII or extended ASCII character
				l_return = eKEYBOARD_KEY( p_code );
			}

			return l_return;
		}
	}

	RenderPanel::RenderPanel( wxWindow * parent, wxWindowID p_id, wxPoint const & pos, wxSize const & size, long style )
		: wxPanel( parent, p_id, pos, size, style )
		, m_mouseLeftDown( false )
		, m_mouseRightDown( false )
		, m_mouseMiddleDown( false )
		, m_x( 0.0_r )
		, m_y( 0.0_r )
		, m_oldX( 0.0_r )
		, m_oldY( 0.0_r )
		, m_eCameraMode( eCAMERA_MODE_FIXED )
		, m_camSpeed( DEF_CAM_SPEED )
	{
		for ( int i = 0; i < eTIMER_ID_COUNT; i++ )
		{
			m_pTimer[i] = new wxTimer( this, i );
		}

		m_pCursorArrow = new wxCursor( wxCURSOR_ARROW );
		m_pCursorHand = new wxCursor( wxCURSOR_HAND );
		m_pCursorNone = new wxCursor( wxCURSOR_BLANK );
	}

	RenderPanel::~RenderPanel()
	{
		delete m_pCursorArrow;
		delete m_pCursorHand;
		delete m_pCursorNone;

		for ( int i = 0; i < eTIMER_ID_COUNT; i++ )
		{
			delete m_pTimer[i];
			m_pTimer[i] = nullptr;
		}
	}

	void RenderPanel::Focus()
	{
	}

	void RenderPanel::UnFocus()
	{
	}

	void RenderPanel::DrawOneFrame()
	{
		wxClientDC l_dc( this );
	}

	void RenderPanel::SetRenderWindow( Castor3D::RenderWindowSPtr p_window )
	{
		m_renderWindow.reset();

		if ( p_window )
		{
			Castor::Size l_sizeWnd = GuiCommon::make_Size( GetClientSize() );

			if ( p_window->Initialise( l_sizeWnd, GuiCommon::make_WindowHandle( this ) ) )
			{
				Castor::Size l_sizeScreen;
				Castor::System::GetScreenSize( 0, l_sizeScreen );
				GetParent()->SetClientSize( l_sizeWnd.width(), l_sizeWnd.height() );
				l_sizeWnd = GuiCommon::make_Size( GetParent()->GetClientSize() );
				GetParent()->SetPosition( wxPoint( std::abs( int( l_sizeScreen.width() ) - int( l_sizeWnd.width() ) ) / 2, std::abs( int( l_sizeScreen.height() ) - int( l_sizeWnd.height() ) ) / 2 ) );
				m_pListener = p_window->GetListener();
				SceneSPtr l_scene = p_window->GetScene();

				if ( l_scene )
				{
					m_cameraNode = p_window->GetCamera()->GetParent();

					if ( l_scene->GetSceneNodeCache().Has( cuT( "PointLightsNode" ) ) )
					{
						m_lightsNode = l_scene->GetSceneNodeCache().Find( cuT( "PointLightsNode" ) );
					}

					if ( m_cameraNode )
					{
						m_ptOriginalPosition = m_cameraNode->GetPosition();
						m_qOriginalOrientation = m_cameraNode->GetOrientation();
						m_currentNode = m_cameraNode;
					}

					m_renderWindow = p_window;
					m_keyboardEvent = std::make_unique< KeyboardEvent >( p_window );
				}
			}
		}

		if ( !m_selectedPass )
		{
			m_selectedPass = std::make_shared< Pass >( *wxGetApp().GetCastor() );
			m_selectedPass->SetColourBlendMode( BlendMode::Interpolative );
			m_selectedPass->SetAmbient( Colour::from_predef( Colour::ePREDEFINED_MEDALPHA_RED ) );
			m_selectedPass->SetDiffuse( Colour::from_predef( Colour::ePREDEFINED_MEDALPHA_RED ) );
			m_selectedPass->SetSpecular( Colour::from_predef( Colour::ePREDEFINED_MEDALPHA_RED ) );
		}
	}

	void RenderPanel::DoResetTimers()
	{
		DoStopTimer( eTIMER_ID_COUNT );
		m_camSpeed = DEF_CAM_SPEED;
		m_x = 0.0_r;
		m_y = 0.0_r;
		m_oldX = 0.0_r;
		m_oldY = 0.0_r;
	}

	void RenderPanel::DoStartTimer( int p_iId )
	{
		m_pTimer[p_iId]->Start( 10 );
	}

	void RenderPanel::DoStopTimer( int p_iId )
	{
		if ( p_iId != eTIMER_ID_COUNT )
		{
			m_pTimer[p_iId]->Stop();
		}
		else
		{
			for ( int i = 0; i < eTIMER_ID_COUNT; i++ )
			{
				m_pTimer[i]->Stop();
			}
		}
	}

	void RenderPanel::DoResetCamera()
	{
		DoResetTimers();

		if ( m_cameraNode )
		{
			wxGetApp().GetCastor()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [this]()
			{
				m_cameraNode->SetOrientation( m_qOriginalOrientation );
				m_cameraNode->SetPosition( m_ptOriginalPosition );
			} ) );
			m_camSpeed = DEF_CAM_SPEED;
		}
	}
	void RenderPanel::DoTurnCamera()
	{
		DoResetTimers();

		if ( m_cameraNode )
		{
			wxGetApp().GetCastor()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_PRE_RENDER, [this]()
			{
				Quaternion l_orientation{ m_cameraNode->GetOrientation() };
				l_orientation *= Quaternion{ Point3r{ 0.0_r, 1.0_r, 0.0_r }, Angle::from_degrees( 90.0_r ) };
				m_cameraNode->SetOrientation( l_orientation );
			} ) );
		}
	}

	void RenderPanel::DoReloadScene()
	{
		DoResetTimers();
		wxGetApp().GetMainFrame()->LoadScene();
	}

	real RenderPanel::DoTransformX( int x )
	{
		real l_result = real( x );
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( l_window )
		{
			l_result *= real( l_window->GetCamera()->GetWidth() ) / GetClientSize().x;
		}

		return l_result;
	}

	real RenderPanel::DoTransformY( int y )
	{
		real l_result = real( y );
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( l_window )
		{
			l_result *= real( l_window->GetCamera()->GetHeight() ) / GetClientSize().y;
		}

		return l_result;
	}

	int RenderPanel::DoTransformX( real x )
	{
		int l_result = int( x );
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( l_window )
		{
			l_result = int( x * GetClientSize().x / real( l_window->GetCamera()->GetWidth() ) );
		}

		return l_result;
	}

	int RenderPanel::DoTransformY( real y )
	{
		int l_result = int( y );
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( l_window )
		{
			l_result = int( y * GetClientSize().y / real( l_window->GetCamera()->GetHeight() ) );
		}

		return l_result;
	}

	void RenderPanel::DoUpdateSelectedGeometry( Castor3D::GeometrySPtr p_geometry, Castor3D::SubmeshSPtr p_submesh )
	{
		auto l_submesh = m_selectedSubmesh.lock();
		auto l_geometry = m_selectedGeometry.lock();

		if ( p_submesh != l_submesh && p_geometry != l_geometry )
		{
			if ( l_submesh && l_geometry )
			{
				wxGetApp().GetCastor()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_POST_RENDER, [this, l_geometry, l_submesh]()
				{
					l_geometry->GetMaterial( l_submesh )->RemovePass( m_selectedPass );
					l_geometry->GetScene()->SetChanged();
				} ) );
			}

			if ( p_submesh && p_geometry )
			{
				m_selectedSubmeshMaterial = p_geometry->GetMaterial( p_submesh );

				wxGetApp().GetCastor()->PostEvent( MakeFunctorEvent( eEVENT_TYPE_POST_RENDER, [this, p_geometry, p_submesh]()
				{
					p_geometry->GetMaterial( p_submesh )->AddPass( m_selectedPass );
					p_geometry->GetScene()->SetChanged();
				} ) );
			}

			m_selectedSubmesh = p_submesh;
			m_selectedGeometry = p_geometry;
		}
	}

	BEGIN_EVENT_TABLE( RenderPanel, wxPanel )
		EVT_TIMER( eTIMER_ID_FORWARD, RenderPanel::OnTimerFwd )
		EVT_TIMER( eTIMER_ID_BACK, RenderPanel::OnTimerBck )
		EVT_TIMER( eTIMER_ID_LEFT, RenderPanel::OnTimerLft )
		EVT_TIMER( eTIMER_ID_RIGHT, RenderPanel::OnTimerRgt )
		EVT_TIMER( eTIMER_ID_UP, RenderPanel::OnTimerUp )
		EVT_TIMER( eTIMER_ID_DOWN, RenderPanel::OnTimerDwn )
		EVT_SIZE( RenderPanel::OnSize )
		EVT_MOVE( RenderPanel::OnMove )
		EVT_PAINT( RenderPanel::OnPaint )
		EVT_ENTER_WINDOW( RenderPanel::OnEnterWindow )
		EVT_LEAVE_WINDOW( RenderPanel::OnLeaveWindow )
		EVT_ERASE_BACKGROUND( RenderPanel::OnEraseBackground )
		EVT_SET_FOCUS( RenderPanel::OnSetFocus )
		EVT_KILL_FOCUS( RenderPanel::OnKillFocus )
		EVT_KEY_DOWN( RenderPanel::OnKeyDown )
		EVT_KEY_UP( RenderPanel::OnKeyUp )
		EVT_CHAR( RenderPanel::OnChar )
		EVT_LEFT_DCLICK( RenderPanel::OnMouseLDClick )
		EVT_LEFT_DOWN( RenderPanel::OnMouseLDown )
		EVT_LEFT_UP( RenderPanel::OnMouseLUp )
		EVT_MIDDLE_DOWN( RenderPanel::OnMouseMDown )
		EVT_MIDDLE_UP( RenderPanel::OnMouseMUp )
		EVT_RIGHT_DOWN( RenderPanel::OnMouseRDown )
		EVT_RIGHT_UP( RenderPanel::OnMouseRUp )
		EVT_MOTION( RenderPanel::OnMouseMove )
		EVT_MOUSEWHEEL( RenderPanel::OnMouseWheel )
		EVT_MENU( wxID_EXIT, RenderPanel::OnMenuClose )
	END_EVENT_TABLE()

	void RenderPanel::OnTimerFwd( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, 0.0_r, 0.0_r, m_camSpeed ) );
		p_event.Skip();
	}

	void RenderPanel::OnTimerBck( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, 0.0_r, 0.0_r, -m_camSpeed ) );
		p_event.Skip();
	}

	void RenderPanel::OnTimerLft( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, m_camSpeed, 0.0_r, 0.0_r ) );
		p_event.Skip();
	}

	void RenderPanel::OnTimerRgt( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, -m_camSpeed, 0.0_r, 0.0_r ) );
		p_event.Skip();
	}

	void RenderPanel::OnTimerUp( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, 0.0_r, m_camSpeed, 0.0_r ) );
		p_event.Skip();
	}

	void RenderPanel::OnTimerDwn( wxTimerEvent & p_event )
	{
		m_pListener->PostEvent( std::make_unique< TranslateNodeEvent >( m_currentNode, 0.0_r, -m_camSpeed, 0.0_r ) );
		p_event.Skip();
	}

	void RenderPanel::OnSize( wxSizeEvent & p_event )
	{
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( l_window )
		{
			l_window->Resize( p_event.GetSize().x, p_event.GetSize().y );
		}
		else
		{
			wxClientDC l_dc( this );
			l_dc.SetBrush( wxBrush( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.SetPen( wxPen( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.DrawRectangle( 0, 0, p_event.GetSize().x, p_event.GetSize().y );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMove( wxMoveEvent & p_event )
	{
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( !l_window )
		{
			wxClientDC l_dc( this );
			l_dc.SetBrush( wxBrush( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.SetPen( wxPen( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
		}

		p_event.Skip();
	}

	void RenderPanel::OnPaint( wxPaintEvent & p_event )
	{
		RenderWindowSPtr l_window = m_renderWindow.lock();

		if ( !l_window )
		{
			wxPaintDC l_dc( this );
			l_dc.SetBrush( wxBrush( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.SetPen( wxPen( GuiCommon::INACTIVE_TAB_COLOUR ) );
			l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
		}

		p_event.Skip();
	}

	void RenderPanel::OnEnterWindow( wxMouseEvent & p_event )
	{
		p_event.Skip();
	}

	void RenderPanel::OnLeaveWindow( wxMouseEvent & p_event )
	{
		p_event.Skip();
	}

	void RenderPanel::OnEraseBackground( wxEraseEvent & p_event )
	{
		p_event.Skip();
	}

	void RenderPanel::OnSetFocus( wxFocusEvent & p_event )
	{
		p_event.Skip();
	}

	void RenderPanel::OnKillFocus( wxFocusEvent & p_event )
	{
		DoStopTimer( eTIMER_ID_COUNT );
		p_event.Skip();
	}

	void RenderPanel::OnKeyDown( wxKeyEvent & p_event )
	{
		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireKeyDown( ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() ) )
		{
			switch ( p_event.GetKeyCode() )
			{
			case WXK_LEFT:
			case 'Q':
				DoStartTimer( eTIMER_ID_LEFT );
				break;

			case WXK_RIGHT:
			case 'D':
				DoStartTimer( eTIMER_ID_RIGHT );
				break;

			case WXK_UP:
			case 'Z':
				DoStartTimer( eTIMER_ID_FORWARD );
				break;

			case WXK_DOWN:
			case 'S':
				DoStartTimer( eTIMER_ID_BACK );
				break;

			case WXK_PAGEUP:
				DoStartTimer( eTIMER_ID_UP );
				break;

			case WXK_PAGEDOWN:
				DoStartTimer( eTIMER_ID_DOWN );
				break;

			case 'L':
				m_currentNode = m_lightsNode;
				break;
			}
		}

		p_event.Skip();
	}

	void RenderPanel::OnKeyUp( wxKeyEvent & p_event )
	{
		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireKeyUp( ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() ) )
		{
			switch ( p_event.GetKeyCode() )
			{
			case 'R':
				DoResetCamera();
				break;

			case 'T':
				DoTurnCamera();
				break;

			case WXK_F5:
				DoReloadScene();
				break;

			case WXK_LEFT:
			case 'Q':
				DoStopTimer( eTIMER_ID_LEFT );
				break;

			case WXK_RIGHT:
			case 'D':
				DoStopTimer( eTIMER_ID_RIGHT );
				break;

			case WXK_UP:
			case 'Z':
				DoStopTimer( eTIMER_ID_FORWARD );
				break;

			case WXK_DOWN:
			case 'S':
				DoStopTimer( eTIMER_ID_BACK );
				break;

			case WXK_PAGEUP:
				DoStopTimer( eTIMER_ID_UP );
				break;

			case WXK_PAGEDOWN:
				DoStopTimer( eTIMER_ID_DOWN );
				break;

			case 'L':
				m_currentNode = m_cameraNode;
				break;
			}
		}

		p_event.Skip();
	}

	void RenderPanel::OnChar( wxKeyEvent & p_event )
	{
		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( l_inputListener )
		{
			wxChar l_key = p_event.GetUnicodeKey();
			wxString l_tmp;
			l_tmp << l_key;
			l_inputListener->FireChar( ConvertKeyCode( p_event.GetKeyCode() ), String( l_tmp.mb_str( wxConvUTF8 ) ) );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseLDClick( wxMouseEvent & p_event )
	{
		if ( m_pListener )
		{
			m_pListener->PostEvent( std::make_unique< KeyboardEvent >( *m_keyboardEvent ) );
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				wxGetApp().GetMainFrame()->ToggleFullScreen( !l_window->IsFullscreen() );
			}
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseLDown( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = true;
		m_oldX = DoTransformX( p_event.GetX() );
		m_oldY = DoTransformY( p_event.GetY() );

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonPushed( eMOUSE_BUTTON_LEFT ) )
		{
			SetCursor( *m_pCursorNone );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseLUp( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = false;

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonReleased( eMOUSE_BUTTON_LEFT ) )
		{
			SetCursor( *m_pCursorArrow );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseMDown( wxMouseEvent & p_event )
	{
		m_mouseMiddleDown = true;
		m_oldX = DoTransformX( p_event.GetX() );
		m_oldY = DoTransformY( p_event.GetY() );

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonPushed( eMOUSE_BUTTON_MIDDLE ) )
		{
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				Camera const & l_camera = *l_window->GetCamera();
				SubmeshSPtr l_submesh;
				Face l_face{ 0, 0, 0 };
				real l_distance{ std::numeric_limits< real >::max() };
				auto l_geometry = l_window->GetScene()->GetObjectRootNode()->GetNearestGeometry( Ray{ int( m_oldX ), int ( m_oldY ), l_camera }
																								 , l_camera
																								 , l_distance
																								 , l_face
																								 , l_submesh );
				DoUpdateSelectedGeometry( l_geometry, l_submesh );
			}
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseMUp( wxMouseEvent & p_event )
	{
		m_mouseMiddleDown = false;

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonReleased( eMOUSE_BUTTON_MIDDLE ) )
		{
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseRDown( wxMouseEvent & p_event )
	{
		m_mouseRightDown = true;
		m_oldX = DoTransformX( p_event.GetX() );
		m_oldY = DoTransformY( p_event.GetY() );

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonPushed( eMOUSE_BUTTON_RIGHT ) )
		{
			SetCursor( *m_pCursorNone );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseRUp( wxMouseEvent & p_event )
	{
		m_mouseRightDown = false;

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseButtonReleased( eMOUSE_BUTTON_RIGHT ) )
		{
			SetCursor( *m_pCursorArrow );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseMove( wxMouseEvent & p_event )
	{
		m_x = DoTransformX( p_event.GetX() );
		m_y = DoTransformY( p_event.GetY() );

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( l_inputListener && l_inputListener->FireMouseMove( Position( int32_t( m_x ), int32_t( m_y ) ) ) )
		{
			p_event.Skip();
		}
		else
		{
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				real l_deltaX = ( std::min( m_camSpeed, 2.0_r ) / 2.0_r ) * ( m_oldX - m_x ) / 2.0_r;
				real l_deltaY = ( std::min( m_camSpeed, 2.0_r ) / 2.0_r ) * ( m_oldY - m_y ) / 2.0_r;

				if ( p_event.ControlDown() )
				{
					l_deltaX = 0;
				}
				else if ( p_event.ShiftDown() )
				{
					l_deltaY = 0;
				}

				if ( m_mouseLeftDown )
				{
					m_pListener->PostEvent( std::make_unique< RotateNodeEvent >( m_currentNode, -l_deltaY, l_deltaX, 0.0_r ) );
				}
				else if ( m_mouseRightDown )
				{
					m_pListener->PostEvent( std::make_unique< RotateNodeEvent >( m_currentNode, l_deltaX, l_deltaY, 0.0_r ) );
				}

				if ( m_mouseLeftDown || m_mouseRightDown )
				{
					WarpPointer( DoTransformX( m_oldX ), DoTransformY( m_oldY ) );
					m_x = m_oldX;
					m_y = m_oldY;
				}

				p_event.Skip( false );
			}
			else
			{
				p_event.Skip();
			}
		}

		m_oldX = m_x;
		m_oldY = m_y;
	}

	void RenderPanel::OnMouseWheel( wxMouseEvent & p_event )
	{
		int l_wheelRotation = p_event.GetWheelRotation();

		auto l_inputListener = wxGetApp().GetCastor()->GetUserInputListener();

		if ( !l_inputListener || !l_inputListener->FireMouseWheel( Position( 0, l_wheelRotation ) ) )
		{
			if ( l_wheelRotation < 0 )
			{
				m_camSpeed *= CAM_SPEED_INC;
			}
			else
			{
				m_camSpeed /= CAM_SPEED_INC;
			}

			clamp( m_camSpeed, MIN_CAM_SPEED, MAX_CAM_SPEED );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMenuClose( wxCommandEvent & p_event )
	{
		Close( true );
		p_event.Skip();
	}
}
