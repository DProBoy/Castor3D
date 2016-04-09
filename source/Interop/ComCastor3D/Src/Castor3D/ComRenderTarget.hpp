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
#ifndef __COMC3D_COM_RENDER_TARGET_H__
#define __COMC3D_COM_RENDER_TARGET_H__

#include "ComSize.hpp"
#include "ComCamera.hpp"
#include "ComScene.hpp"

#include <RenderTarget.hpp>

namespace CastorCom
{
	/*!
	\author 	Sylvain DOREMUS
	\version	0.7.0
	\date		10/09/2014
	\~english
	\brief		This class defines a CRenderWindow object accessible from COM.
	\~french
	\brief		Cette classe définit un CRenderWindow accessible depuis COM.
	*/
	class ATL_NO_VTABLE CRenderTarget
		:	COM_ATL_OBJECT( RenderTarget )
	{
	public:
		/**
		 *\~english
		 *\brief		Default constructor.
		 *\~french
		 *\brief		Constructeur par défaut.
		 */
		CRenderTarget();
		/**
		 *\~english
		 *\brief		Destructor.
		 *\~french
		 *\brief		Destructeur.
		 */
		virtual ~CRenderTarget();

		inline Castor3D::RenderTargetSPtr GetInternal()const
		{
			return m_internal;
		}

		inline void SetInternal( Castor3D::RenderTargetSPtr value )
		{
			m_internal = value;
		}

		COM_PROPERTY( SamplesCount, unsigned int, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetSamplesCount ), make_putter( m_internal.get(), &Castor3D::RenderTarget::SetSamplesCount ) );
		COM_PROPERTY( Camera, ICamera *, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetCamera ), make_putter( m_internal.get(), &Castor3D::RenderTarget::SetCamera ) );
		COM_PROPERTY( ViewportType, eVIEWPORT_TYPE, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetViewportType ), make_putter( m_internal.get(), &Castor3D::RenderTarget::SetViewportType ) );
		COM_PROPERTY( Scene, IScene *, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetScene ), make_putter( m_internal.get(), &Castor3D::RenderTarget::SetScene ) );
		COM_PROPERTY( PixelFormat, ePIXEL_FORMAT, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetPixelFormat ), make_putter( m_internal.get(), &Castor3D::RenderTarget::SetPixelFormat ) );

		COM_PROPERTY_GET( Size, ISize *, make_getter( m_internal.get(), &Castor3D::RenderTarget::GetSize ) );

		STDMETHOD( Initialise )( /* [in] */ unsigned int index );
		STDMETHOD( Cleanup )();

	private:
		Castor3D::RenderTargetSPtr m_internal;
	};
	//!\~english Enters the ATL object into the object map, updates the registry and creates an instance of the object	\~french Ecrit l'objet ATL dans la table d'objets, met à jour le registre et crée une instance de l'objet
	OBJECT_ENTRY_AUTO( __uuidof( RenderTarget ), CRenderTarget );

	DECLARE_VARIABLE_PTR_GETTER( RenderTarget, Castor3D, RenderTarget );
	DECLARE_VARIABLE_PTR_PUTTER( RenderTarget, Castor3D, RenderTarget );
}

#endif