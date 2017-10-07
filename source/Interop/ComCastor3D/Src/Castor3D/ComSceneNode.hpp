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
#ifndef __COMC3D_COM_SCENE_NODE_H__
#define __COMC3D_COM_SCENE_NODE_H__

#include "ComTextureLayout.hpp"
#include "ComColour.hpp"
#include "ComVector3D.hpp"
#include "ComQuaternion.hpp"

#include <Scene/Scene.hpp>
#include <Scene/SceneNode.hpp>

namespace CastorCom
{
	template< typename Value >
	struct VariablePutterEvt< castor3d::SceneNode, Value >
	{
		typedef void ( castor3d::SceneNode::*Function )( Value );
		VariablePutterEvt( castor3d::SceneNode * instance, Function function )
			: m_instance( instance )
			, m_function( function )
		{
		}
		template< typename _Value >
		HRESULT operator()( _Value value )
		{
			HRESULT hr = E_POINTER;

			if ( m_instance )
			{
				if ( value )
				{
					m_instance->getScene()->getEngine()->postEvent( castor3d::MakeFunctorEvent( castor3d::EventType::ePreRender
						, [this, value]
						{
							( m_instance->*m_function )( parameter_cast< Value >( value ) );
						} ) );
					hr = S_OK;
				}
			}
			else
			{
				hr = CComError::DispatchError( E_FAIL, IID_ISceneNode, cuT( "NULL instance" ), ERROR_UNINITIALISED_INSTANCE.c_str(), 0, NULL );
			}

			return hr;
		}

	private:
		castor3d::SceneNode * m_instance;
		Function m_function;
	};

	/*!
	\author 	Sylvain DOREMUS
	\version	0.7.0
	\date		10/09/2014
	\~english
	\brief		This class defines a CSceneNode object accessible from COM.
	\~french
	\brief		Cette classe définit un CSceneNode accessible depuis COM.
	*/
	class ATL_NO_VTABLE CSceneNode
		:	COM_ATL_OBJECT( SceneNode )
	{
	public:
		/**
		 *\~english
		 *\brief		Default constructor.
		 *\~french
		 *\brief		Constructeur par défaut.
		 */
		CSceneNode();
		/**
		 *\~english
		 *\brief		Destructor.
		 *\~french
		 *\brief		Destructeur.
		 */
		virtual ~CSceneNode();

		inline castor3d::SceneNodeSPtr getInternal()const
		{
			return m_internal;
		}

		inline void setInternal( castor3d::SceneNodeSPtr internal )
		{
			m_internal = internal;
		}

		COM_EVT_PROPERTY( Position, IVector3D *, make_getter( m_internal.get(), &castor3d::SceneNode::getPosition ), make_putter_evt( m_internal.get(), &castor3d::SceneNode::setPosition ) );
		COM_EVT_PROPERTY( Orientation, IQuaternion *, make_getter( m_internal.get(), &castor3d::SceneNode::getOrientation ), make_putter_evt( m_internal.get(), &castor3d::SceneNode::setOrientation ) );
		COM_EVT_PROPERTY( Scaling, IVector3D *, make_getter( m_internal.get(), &castor3d::SceneNode::getScale ), make_putter_evt( m_internal.get(), &castor3d::SceneNode::setScale ) );

		STDMETHOD( attachObject )( /* [in] */ IMovableObject * val );
		STDMETHOD( detachObject )( /* [in] */ IMovableObject * val );
		STDMETHOD( attachTo )( /* [in] */ ISceneNode * val );
		STDMETHOD( Detach )();
		STDMETHOD( Yaw )( /* [in] */ IAngle * val );
		STDMETHOD( Pitch )( /* [in] */ IAngle * val );
		STDMETHOD( Roll )( /* [in] */ IAngle * val );
		STDMETHOD( Rotate )( /* [in] */ IQuaternion * val );
		STDMETHOD( Translate )( /* [in] */ IVector3D * val );
		STDMETHOD( Scale )( /* [in] */ IVector3D * val );
	private:
		castor3d::SceneNodeSPtr m_internal;
	};
	//!\~english Enters the ATL object into the object map, updates the registry and creates an instance of the object	\~french Ecrit l'objet ATL dans la table d'objets, met à jour le registre et crée une instance de l'objet
	OBJECT_ENTRY_AUTO( __uuidof( SceneNode ), CSceneNode );

	DECLARE_VARIABLE_PTR_GETTER( SceneNode, castor3d, SceneNode );
	DECLARE_VARIABLE_PTR_PUTTER( SceneNode, castor3d, SceneNode );
}

#endif