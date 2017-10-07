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
#ifndef ___GL_TEXTURE_H___
#define ___GL_TEXTURE_H___

#include "Common/GlObject.hpp"

#include <Texture/TextureLayout.hpp>

namespace GlRender
{
	/*!
	\author		Sylvain doremus.
	\version	0.8.0
	\date		12/10/2015
	\brief		Class used to handle texture, and texture storage.
	*/
	class GlTexture
		: private Object< std::function< void( int, uint32_t * ) >, std::function< void( int, uint32_t const * ) > >
		, public castor3d::TextureLayout
	{
		using ObjectType = Object< std::function< void( int, uint32_t * ) >, std::function< void( int, uint32_t const * ) > >;

	public:
		/**
		 *\brief		Constructor.
		 *\param[in]	p_gl			The OpenGL APIs.
		 *\param[in]	p_type			The texture type.
		 *\param[in]	renderSystem	The RenderSystem.
		 *\param[in]	p_cpuAccess		The required CPU access (combination of AccessType).
		 *\param[in]	p_gpuAccess		The required GPU access (combination of AccessType).
		 */
		GlTexture(
			OpenGl & p_gl,
			GlRenderSystem & renderSystem,
			castor3d::TextureType p_type,
			castor3d::AccessTypes const & p_cpuAccess,
			castor3d::AccessTypes const & p_gpuAccess );
		/**
		 *\brief		Constructor.
		 *\param[in]	p_gl			The OpenGL APIs.
		 *\param[in]	p_type			The texture type.
		 *\param[in]	renderSystem	The RenderSystem.
		 *\param[in]	p_cpuAccess		The required CPU access (combination of AccessType).
		 *\param[in]	p_gpuAccess		The required GPU access (combination of AccessType).
		 *\param[in]	p_format		The texture format.
		 *\param[in]	p_size			The texture dimensions.
		 */
		GlTexture(
			OpenGl & p_gl,
			GlRenderSystem & renderSystem,
			castor3d::TextureType p_type,
			castor3d::AccessTypes const & p_cpuAccess,
			castor3d::AccessTypes const & p_gpuAccess,
			castor::PixelFormat p_format,
			castor::Size const & p_size );
		/**
		 *\brief		Constructor.
		 *\param[in]	p_gl			The OpenGL APIs.
		 *\param[in]	p_type			The texture type.
		 *\param[in]	renderSystem	The RenderSystem.
		 *\param[in]	p_cpuAccess		The required CPU access (combination of AccessType).
		 *\param[in]	p_gpuAccess		The required GPU access (combination of AccessType).
		 *\param[in]	p_format		The texture format.
		 *\param[in]	p_size			The texture dimensions.
		 */
		GlTexture(
			OpenGl & p_gl,
			GlRenderSystem & renderSystem,
			castor3d::TextureType p_type,
			castor3d::AccessTypes const & p_cpuAccess,
			castor3d::AccessTypes const & p_gpuAccess,
			castor::PixelFormat p_format,
			castor::Point3ui const & p_size );
		/**
		 *\brief		Destructor.
		 */
		~GlTexture();
		/**
		*\brief		Forces mipmaps generation.
		*/
		void generateMipmaps()const;
		/**
		*\return	The OpenGL texture dimension.
		*/
		GlTexDim getGlDimension()const
		{
			return m_glDimension;
		}

	private:
		/**
		 *\copydoc		castor3d::TextureLayout::doInitialise
		 */
		bool doInitialise()override;
		/**
		 *\copydoc		castor3d::TextureLayout::doCleanup
		 */
		void doCleanup()override;
		/**
		 *\copydoc		castor3d::TextureLayout::doBind
		 */
		void doBind( uint32_t p_index )const override;
		/**
		 *\copydoc		castor3d::TextureLayout::doUnbind
		 */
		void doUnbind( uint32_t p_index )const override;

	public:
		using ObjectType::isValid;
		using ObjectType::getGlName;
		using ObjectType::getOpenGl;

	private:
		//! The RenderSystem.
		GlRenderSystem * m_glRenderSystem;
		//! The OpenGL texture dimension.
		GlTexDim m_glDimension;
	};
}

#endif