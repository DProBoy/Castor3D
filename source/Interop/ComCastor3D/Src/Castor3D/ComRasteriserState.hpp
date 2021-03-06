/* See LICENSE file in root folder */
#ifndef __COMC3D_COM_RASTERISER_STATE_H__
#define __COMC3D_COM_RASTERISER_STATE_H__

#include "ComAtlObject.hpp"

#include <State/RasteriserState.hpp>

namespace CastorCom
{
	/*!
	\author 	Sylvain DOREMUS
	\version	0.7.0
	\date		10/09/2014
	\~english
	\brief		This class defines a CRasteriserState object accessible from COM.
	\~french
	\brief		Cette classe définit un CRasteriserState accessible depuis COM.
	*/
	class ATL_NO_VTABLE CRasteriserState
		:	COM_ATL_OBJECT( RasteriserState )
	{
	public:
		/**
		 *\~english
		 *\brief		Default constructor.
		 *\~french
		 *\brief		Constructeur par défaut.
		 */
		CRasteriserState();
		/**
		 *\~english
		 *\brief		Destructor.
		 *\~french
		 *\brief		Destructeur.
		 */
		virtual ~CRasteriserState();

		inline castor3d::RasteriserStateSPtr getInternal()const
		{
			return m_state;
		}

		inline void setInternal( castor3d::RasteriserStateSPtr state )
		{
			m_state = state;
		}

		COM_PROPERTY( FillMode, eFILL_MODE, makeGetter( m_state.get(), &castor3d::RasteriserState::getFillMode ), makePutter( m_state.get(), &castor3d::RasteriserState::setFillMode ) );
		COM_PROPERTY( CulledFaces, eFACE, makeGetter( m_state.get(), &castor3d::RasteriserState::getCulledFaces ), makePutter( m_state.get(), &castor3d::RasteriserState::setCulledFaces ) );
		COM_PROPERTY( FrontCCW, boolean, makeGetter( m_state.get(), &castor3d::RasteriserState::getFrontCCW ), makePutter( m_state.get(), &castor3d::RasteriserState::setFrontCCW ) );
		COM_PROPERTY( AntialiasedLines, boolean, makeGetter( m_state.get(), &castor3d::RasteriserState::getAntialiasedLines ), makePutter( m_state.get(), &castor3d::RasteriserState::setAntialiasedLines ) );
		COM_PROPERTY( DepthBiasUnits, float, makeGetter( m_state.get(), &castor3d::RasteriserState::getDepthBiasUnits ), makePutter( m_state.get(), &castor3d::RasteriserState::setDepthBiasUnits ) );
		COM_PROPERTY( DepthBiasFactor, float, makeGetter( m_state.get(), &castor3d::RasteriserState::getDepthBiasFactor ), makePutter( m_state.get(), &castor3d::RasteriserState::setDepthBiasFactor ) );
		COM_PROPERTY( DepthClipping, boolean, makeGetter( m_state.get(), &castor3d::RasteriserState::getDepthClipping ), makePutter( m_state.get(), &castor3d::RasteriserState::setDepthClipping ) );
		COM_PROPERTY( Scissor, boolean, makeGetter( m_state.get(), &castor3d::RasteriserState::getScissor ), makePutter( m_state.get(), &castor3d::RasteriserState::setScissor ) );

	private:
		castor3d::RasteriserStateSPtr m_state;
	};
	//!\~english Enters the ATL object into the object map, updates the registry and creates an instance of the object	\~french Ecrit l'objet ATL dans la table d'objets, met à jour le registre et crée une instance de l'objet
	OBJECT_ENTRY_AUTO( __uuidof( RasteriserState ), CRasteriserState );

	DECLARE_VARIABLE_PTR_GETTER( RasteriserState, castor3d, RasteriserState );
	DECLARE_VARIABLE_PTR_PUTTER( RasteriserState, castor3d, RasteriserState );
}

#endif
