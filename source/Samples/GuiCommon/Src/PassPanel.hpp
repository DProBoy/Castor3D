/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GUICOMMON_PASS_PANEL_H___
#define ___GUICOMMON_PASS_PANEL_H___

#include "GuiCommonPrerequisites.hpp"

#include <Colour.hpp>

#include <wx/scrolwin.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace GuiCommon
{
	class wxPassPanel
		:	public wxPanel
	{
	public:
		wxPassPanel( bool p_small, bool p_bCanEdit, wxWindow * p_pParent, wxPoint const & p_ptPos = wxDefaultPosition, wxSize const & p_size = wxSize( 460, 380 ) );
		~wxPassPanel();

		void SetPass( Castor3D::Engine * p_pEngine, Castor3D::PassWPtr p_wpPass );
		void GetDiffuse( Castor::Colour & p_clrColour )const;
		void GetAmbient( Castor::Colour & p_clrColour )const;
		void GetEmissive( Castor::Colour & p_clrColour )const;
		void GetSpecular( Castor::Colour & p_clrColour )const;
		Castor::real GetShininess()const;
		int GetTextureUnitIndex()const;
		void SetMaterialImage( uint32_t p_uiIndex, uint32_t p_uiWidth, uint32_t p_uiHeight );
		void SetMaterialImage( Castor3D::TextureUnitSPtr p_pTexture, uint32_t p_uiWidth, uint32_t p_uiHeight );

	private:
		void DoSelectTextureUnit( int p_iIndex );
		void DoShowPassFields( bool p_bShow );
		void DoShowTextureFields( bool p_bShow );
		void DoFillTexturesProperty( uint32_t p_count );

		DECLARE_EVENT_TABLE()
		void OnDeleteTextureUnit( wxCommandEvent & p_event );
		void OnAmbientColour( wxCommandEvent & p_event );
		void OnDiffuseColour( wxCommandEvent & p_event );
		void OnSpecularColour( wxCommandEvent & p_event );
		void OnEmissiveColour( wxCommandEvent & p_event );
		void OnTextureImage( wxCommandEvent & p_event );
		void OnShininess( wxCommandEvent & p_event );
		void OnTexturePath( wxCommandEvent & p_event );
		void OnTextureSelect( wxCommandEvent & p_event );
		void OnDoubleFace( wxCommandEvent & p_event );
		void OnTextureChannel( wxCommandEvent & p_event );
		void OnAlpha( wxCommandEvent & p_event );
		void OnHasShader( wxCommandEvent & p_event );

	private:
		// Main property grid
		wxPropertyGrid * m_propertyGridPass;

		// Pass properties
		wxColourProperty * m_propertyAmbient;
		wxColourProperty * m_propertyDiffuse;
		wxColourProperty * m_propertySpecular;
		wxColourProperty * m_propertyEmissive;
		wxFloatProperty * m_propertyExponent;
		wxBoolProperty * m_propertyTwoSided;
		wxFloatProperty * m_propertyOpacity;
		wxEnumProperty * m_propertyTextures;

		// Texture properties
		wxImageFileProperty * m_propertyImage;
		wxEnumProperty * m_propertyChannel;

		// Other controls
		wxButton * m_pButtonShader;

		// Other members
		wxImage m_imgSelected;
		Castor3D::PassWPtr m_wpPass;
		uint32_t m_uiSelectedUnitIndex;
		Castor3D::TextureUnitWPtr m_pTextureUnit;
		bool m_bCanEdit;
		wxSize m_sizeImage;
		Castor3D::Engine * m_pEngine;
	};
}

#endif
