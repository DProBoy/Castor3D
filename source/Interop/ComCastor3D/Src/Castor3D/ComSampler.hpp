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
#ifndef __COMC3D_COM_SAMPLER_H__
#define __COMC3D_COM_SAMPLER_H__

#include "ComColour.hpp"

#include <Texture/Sampler.hpp>

namespace CastorCom
{
	/*!
	\author 	Sylvain DOREMUS
	\version	0.7.0
	\date		10/09/2014
	\~english
	\brief		This class defines a CSampler object accessible from COM.
	\~french
	\brief		Cette classe définit un CSampler accessible depuis COM.
	*/
	class ATL_NO_VTABLE CSampler
		:	COM_ATL_OBJECT( Sampler )
	{
	public:
		/**
		 *\~english
		 *\brief		Default constructor.
		 *\~french
		 *\brief		Constructeur par défaut.
		 */
		CSampler();
		/**
		 *\~english
		 *\brief		Destructor.
		 *\~french
		 *\brief		Destructeur.
		 */
		virtual ~CSampler();

		inline castor3d::SamplerSPtr getInternal()const
		{
			return m_sampler;
		}

		inline void setInternal( castor3d::SamplerSPtr state )
		{
			m_sampler = state;
		}

		COM_PROPERTY( MinFilter, eINTERPOLATION_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getInterpolationMode, castor3d::InterpolationFilter::eMin ), make_putter( m_sampler.get(), &castor3d::Sampler::setInterpolationMode, castor3d::InterpolationFilter::eMin ) );
		COM_PROPERTY( MagFilter, eINTERPOLATION_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getInterpolationMode, castor3d::InterpolationFilter::eMag ), make_putter( m_sampler.get(), &castor3d::Sampler::setInterpolationMode, castor3d::InterpolationFilter::eMag ) );
		COM_PROPERTY( MipFilter, eINTERPOLATION_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getInterpolationMode, castor3d::InterpolationFilter::eMip ), make_putter( m_sampler.get(), &castor3d::Sampler::setInterpolationMode, castor3d::InterpolationFilter::eMip ) );
		COM_PROPERTY( UWrappingMode, eWRAP_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getWrappingMode, castor3d::TextureUVW::eU ), make_putter( m_sampler.get(), &castor3d::Sampler::setWrappingMode, castor3d::TextureUVW::eU ) );
		COM_PROPERTY( VWrappingMode, eWRAP_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getWrappingMode, castor3d::TextureUVW::eV ), make_putter( m_sampler.get(), &castor3d::Sampler::setWrappingMode, castor3d::TextureUVW::eV ) );
		COM_PROPERTY( WWrappingMode, eWRAP_MODE, make_getter( m_sampler.get(), &castor3d::Sampler::getWrappingMode, castor3d::TextureUVW::eW ), make_putter( m_sampler.get(), &castor3d::Sampler::setWrappingMode, castor3d::TextureUVW::eW ) );
		COM_PROPERTY( MaxAnisotropy, float, make_getter( m_sampler.get(), &castor3d::Sampler::getMaxAnisotropy ), make_putter( m_sampler.get(), &castor3d::Sampler::setMaxAnisotropy ) );
		COM_PROPERTY( MinLod, float, make_getter( m_sampler.get(), &castor3d::Sampler::getMinLod ), make_putter( m_sampler.get(), &castor3d::Sampler::setMinLod ) );
		COM_PROPERTY( MaxLod, float, make_getter( m_sampler.get(), &castor3d::Sampler::getMaxLod ), make_putter( m_sampler.get(), &castor3d::Sampler::setMaxLod ) );
		COM_PROPERTY( LodBias, float, make_getter( m_sampler.get(), &castor3d::Sampler::getLodBias ), make_putter( m_sampler.get(), &castor3d::Sampler::setLodBias ) );
		COM_PROPERTY( BorderColour, IColour *, make_getter( m_sampler.get(), &castor3d::Sampler::getBorderColour ), make_putter( m_sampler.get(), &castor3d::Sampler::setBorderColour ) );

		STDMETHOD( Initialise )();
		STDMETHOD( Cleanup )();

	private:
		castor3d::SamplerSPtr m_sampler;
	};
	//!\~english Enters the ATL object into the object map, updates the registry and creates an instance of the object	\~french Ecrit l'objet ATL dans la table d'objets, met à jour le registre et crée une instance de l'objet
	OBJECT_ENTRY_AUTO( __uuidof( Sampler ), CSampler );

	DECLARE_VARIABLE_PTR_GETTER( Sampler, castor3d, Sampler );
	DECLARE_VARIABLE_PTR_PUTTER( Sampler, castor3d, Sampler );
}

#endif