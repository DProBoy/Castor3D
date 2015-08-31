#include "PropertiesHolder.hpp"

#include "CameraTreeItemProperty.hpp"
#include "GeometryTreeItemProperty.hpp"
#include "LightTreeItemProperty.hpp"
#include "NodeTreeItemProperty.hpp"
#include "OverlayTreeItemProperty.hpp"
#include "SubmeshTreeItemProperty.hpp"
#include "MaterialTreeItemProperty.hpp"
#include "PassTreeItemProperty.hpp"
#include "TextureTreeItemProperty.hpp"

#include "AdditionalProperties.hpp"

#include <Geometry.hpp>
#include <Submesh.hpp>
#include <Light.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <SpotLight.hpp>
#include <SceneNode.hpp>
#include <Overlay.hpp>
#include <PanelOverlay.hpp>
#include <BorderPanelOverlay.hpp>
#include <TextOverlay.hpp>
#include <Material.hpp>
#include <Pass.hpp>
#include <TextureUnit.hpp>

#include <MaterialManager.hpp>
#include <Engine.hpp>
#include <Font.hpp>

#include <wx/propgrid/advprops.h>

using namespace Castor3D;
using namespace Castor;

namespace GuiCommon
{
	wxPropertiesHolder::wxPropertiesHolder( bool p_bCanEdit, wxWindow * p_pParent, wxPoint const & p_ptPos, wxSize const & p_size )
		: wxPropertyGrid( p_pParent, wxID_ANY, p_ptPos, p_size, wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE )
		, m_bCanEdit( p_bCanEdit )
		, m_data( NULL )
	{
		SetBackgroundColour( *wxWHITE );
		Connect( wxEVT_PG_CHANGED, wxEVENT_HANDLER_CAST( wxPropertyGridEventFunction, wxPropertiesHolder::OnPropertyChange ) );
	}

	wxPropertiesHolder::~wxPropertiesHolder()
	{
	}

	void wxPropertiesHolder::SetPropertyData( wxTreeItemProperty * p_data )
	{
		if ( m_data )
		{
			wxPropertyGrid::Clear();
		}

		m_data = p_data;

		if ( m_data )
		{
			m_data->CreateProperties( this );
		}
	}

	void wxPropertiesHolder::OnPropertyChange( wxPropertyGridEvent & p_event )
	{
		if ( m_data )
		{
			m_data->OnPropertyChange( p_event );
		}
	}
}
