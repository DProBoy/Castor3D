/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.html)
Copyright (c) 2016 dragonjoker59@hotmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef ___RenderPanel___
#define ___RenderPanel___

#include <NodeState.hpp>

#include <Engine.hpp>

#include <Math/Point.hpp>
#include <Math/Quaternion.hpp>

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/timer.h>

namespace CastorViewer
{
	typedef enum eTIMER_ID
	{
		eTIMER_ID_FORWARD = 1,
		eTIMER_ID_BACK,
		eTIMER_ID_LEFT,
		eTIMER_ID_RIGHT,
		eTIMER_ID_UP,
		eTIMER_ID_DOWN,
		eTIMER_ID_MOUSE,
		eTIMER_ID_MOVEMENT,
		eTIMER_ID_COUNT,
	}	eTIMER_ID;

	class MouseNodeEvent;
	class TranslateNodeEvent;
	class KeyboardEvent;

	DECLARE_SMART_PTR( MouseNodeEvent );
	DECLARE_SMART_PTR( TranslateNodeEvent );
	DECLARE_SMART_PTR( KeyboardEvent );

	class RenderPanel
		: public wxPanel
	{
	public:
		struct SelectedSubmesh
		{
			SelectedSubmesh( castor3d::SubmeshSPtr p_submesh
				, castor3d::MaterialSPtr p_originalMaterial )
				: m_submesh{ p_submesh }
				, m_originalMaterial{ p_originalMaterial }
			{
			}
			castor3d::SubmeshSPtr m_submesh;
			castor3d::MaterialSPtr m_originalMaterial;
			castor3d::MaterialSPtr m_selectedMaterial;
		};

		struct SelectedGeometry
		{
			std::vector< SelectedSubmesh > m_submeshes;
			castor3d::GeometrySPtr m_geometry;
		};


	public:
		RenderPanel( wxWindow * parent, wxWindowID p_id, wxPoint const & pos = wxDefaultPosition, wxSize const & size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE );
		virtual ~RenderPanel();

		virtual void Focus();
		virtual void UnFocus();

		virtual void DrawOneFrame();
		void setRenderWindow( castor3d::RenderWindowSPtr p_window );

		inline castor3d::RenderWindowSPtr getRenderWindow()const
		{
			return m_renderWindow.lock();
		}

	private:
		void doResetTimers();
		void doStartMovement();
		void doStartTimer( int p_iId );
		void doStopMovement();
		void doStopTimer( int p_iId );
		void doResetNode();
		void doTurnCameraHoriz();
		void doTurnCameraVertic();
		void doChangeCamera();
		void doReloadScene();
		castor::real doTransformX( int x );
		castor::real doTransformY( int y );
		int doTransformX( castor::real x );
		int doTransformY( castor::real y );
		void doUpdateSelectedGeometry( castor3d::GeometrySPtr p_geometry, castor3d::SubmeshSPtr p_submesh );
		GuiCommon::NodeState & doAddNodeState( castor3d::SceneNodeSPtr p_node );

		DECLARE_EVENT_TABLE()
		void OnTimerFwd( wxTimerEvent & p_event );
		void OnTimerBck( wxTimerEvent & p_event );
		void OnTimerLft( wxTimerEvent & p_event );
		void OnTimerRgt( wxTimerEvent & p_event );
		void OnTimerUp( wxTimerEvent & p_event );
		void OnTimerDwn( wxTimerEvent & p_event );
		void OnTimerMouse( wxTimerEvent & p_event );
		void OnTimerMovement( wxTimerEvent & p_event );
		void OnSize( wxSizeEvent & p_event );
		void OnMove( wxMoveEvent & p_event );
		void OnPaint( wxPaintEvent & p_event );
		void OnEnterWindow( wxMouseEvent & p_event );
		void OnLeaveWindow( wxMouseEvent & p_event );
		void OnEraseBackground( wxEraseEvent & p_event );
		void OnsetFocus( wxFocusEvent & p_event );
		void OnKillFocus( wxFocusEvent & p_event );
		void onKeydown( wxKeyEvent & p_event );
		void OnKeyUp( wxKeyEvent & p_event );
		void OnChar( wxKeyEvent & p_event );
		void OnMouseLDClick( wxMouseEvent & p_event );
		void OnMouseLdown( wxMouseEvent & p_event );
		void OnMouseLUp( wxMouseEvent & p_event );
		void OnMouseMdown( wxMouseEvent & p_event );
		void OnMouseMUp( wxMouseEvent & p_event );
		void OnMouseRdown( wxMouseEvent & p_event );
		void OnMouseRUp( wxMouseEvent & p_event );
		void OnMouseMove( wxMouseEvent & p_event );
		void OnMouseWheel( wxMouseEvent & p_event );
		void OnMenuClose( wxCommandEvent & p_event );

	public:
		castor::real m_x{ 0.0_r };
		castor::real m_y{ 0.0_r };
		castor::real m_oldX{ 0.0_r };
		castor::real m_oldY{ 0.0_r };
		bool m_altdown{ false };
		bool m_mouseLeftDown{ false };
		bool m_mouseRightDown{ false };
		bool m_mouseMiddleDown{ false };
		std::atomic_bool m_movementStarted{ false };
		wxTimer * m_pTimer[eTIMER_ID_COUNT];
		castor3d::RenderWindowWPtr m_renderWindow;
		castor3d::CameraWPtr m_camera;
		castor3d::SceneWPtr m_scene;
		castor3d::FrameListenerSPtr m_listener;
		wxCursor * m_pCursorArrow;
		wxCursor * m_pCursorHand;
		wxCursor * m_pCursorNone;

		castor3d::SceneNodeSPtr m_lightsNode;
		castor3d::SceneNodeSPtr m_currentNode;
		KeyboardEventUPtr m_keyboardEvent;
		castor::RangedValue< castor::real > m_camSpeed;

		std::map< castor::String, GuiCommon::NodeStatePtr > m_nodesStates;
		GuiCommon::NodeState * m_currentState{ nullptr };
		SelectedGeometry m_selectedGeometry;
		SelectedSubmesh * m_selectedSubmesh{ nullptr };
	};
}

#endif