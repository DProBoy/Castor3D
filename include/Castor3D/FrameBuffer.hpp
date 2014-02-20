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
#ifndef ___C3D_FrameBuffer___
#define ___C3D_FrameBuffer___

#include "Prerequisites.hpp"
#include <CastorUtils/Rectangle.hpp>

#pragma warning( push )
#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

namespace Castor3D
{
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Render buffer base class
	\remark		A render buffer is a buffer that receives specific component from a frame buffer
	\~french
	\brief		Classe de base d'un tampon de rendu
	\remark		Un tampon de rendu est un tampon qui re�oit une composante sp�cifique d'un tampon d'image
	*/
	class C3D_API RenderBuffer
	{
	private:
		eBUFFER_COMPONENT m_eComponent;
		Castor::Size m_size;
		Castor::ePIXEL_FORMAT m_ePixelFormat;
		int m_iSamplesCount;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_eType			The buffer component
		 *\param[in]	p_ePixelFormat	The buffer pixel format
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_eType			La composante du tampon
		 *\param[in]	p_ePixelFormat	Le format des pixels du tampon
		 */
		RenderBuffer( eBUFFER_COMPONENT p_eType, Castor::ePIXEL_FORMAT p_ePixelFormat );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~RenderBuffer();
		/**
		 *\~english
		 *\brief		Creation function
		 *\return		\p true if OK
		 *\~french
		 *\brief		Fonction de cr�ation
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Create() = 0;
		/**
		 *\~english
		 *\brief		Destruction function
		 *\~french
		 *\brief		Fonction de destruction
		 */
		virtual void Destroy() = 0;
		/**
		 *\~english
		 *\brief		Initialisation function
		 *\param[in]	p_size	The buffer dimensions
		 *\return		\p true if OK
		 *\~french
		 *\brief		Fonction d'initialisation
		 *\param[in]	p_size	Les dimensions du tampon
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Initialise( Castor::Size const & p_size ) = 0;
		/**
		 *\~english
		 *\brief		Cleanup function
		 *\~french
		 *\brief		Fonction de nettoyage
		 */
		virtual void Cleanup() = 0;
		/**
		 *\~english
		 *\brief		Activation function, to tell the GPU it is active
		 *\return		\p true if successful
		 *\~french
		 *\brief		Fonction d'activation, pour dire au GPU qu'il est activ�
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Bind() = 0;
		/**
		 *\~english
		 *\brief		Deactivation function, to tell the GPU it is inactive
		 *\~french
		 *\brief		Fonction de d�sactivation, pour dire au GPU qu'il est d�sactiv�
		 */
		virtual void Unbind() = 0;
		/**
		 *\~english
		 *\brief		Resizing function
		 *\param[in]	p_size	The buffer dimensions
		 *\return		\p true if OK
		 *\~french
		 *\brief		Fonction de redimensionnement
		 *\param[in]	p_size	Les dimensions du tampon
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Resize( Castor::Size const & p_size ) = 0;
		/**
		 *\~english
		 *\brief		Retrieves the buffer pixel format
		 *\return		The pixel format
		 *\~french
		 *\brief		R�cup�re le format des pixels du tampon
		 *\return		Le format des pixels
		 */
		inline Castor::ePIXEL_FORMAT GetPixelFormat()const { return m_ePixelFormat; }
		/**
		 *\~english
		 *\brief		Retrieves the buffer dimensions
		 *\return		The dimensions
		 *\~french
		 *\brief		R�cup�re les dimensions du tampon
		 *\return		Les dimensions
		 */
		inline Castor::Size const & GetDimensions()const { return m_size; }
		/**
		 *\~english
		 *\brief		Retrieves the buffer width
		 *\return		The width
		 *\~french
		 *\brief		R�cup�re la largeur du tampon
		 *\return		La largeur
		 */
		inline uint32_t GetWidth()const { return m_size.width(); }
		/**
		 *\~english
		 *\brief		Retrieves the buffer height
		 *\return		The height
		 *\~french
		 *\brief		R�cup�re la hauteur du tampon
		 *\return		La hauteur
		 */
		inline uint32_t GetHeight()const { return m_size.height(); }
		/**
		 *\~english
		 *\brief		Retrieves the buffer component
		 *\return		The component
		 *\~french
		 *\brief		R�cup�re la composante du tampon
		 *\return		La composante
		 */
		inline eBUFFER_COMPONENT GetComponent()const { return m_eComponent; }
		/**
		 *\~english
		 *\brief		Defines the buffer samples count
		 *\param[in]	p_iSamples	The samples count
		 *\~french
		 *\brief		D�finit le nombre de samples du tampon
		 *\param[in]	p_iSamples	Le nombre de samples
		 */
		inline void SetSamplesCount( int p_iSamples ) { m_iSamplesCount = p_iSamples; }
		/**
		 *\~english
		 *\brief		Retrieves the buffer samples count
		 *\return		The samples count
		 *\~french
		 *\brief		R�cup�re le nombre de samples du tampon
		 *\return		Le nombre de samples
		 */
		inline int GetSamplesCount() { return m_iSamplesCount; }
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Render buffer receiving colour component
	\~french
	\brief		Tampon de rendu recevant une composante de couleur
	*/
	class C3D_API ColourRenderBuffer : public RenderBuffer
	{
	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_ePixelFormat	The buffer pixel format
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_ePixelFormat	Le format des pixels du tampon
		 */
		ColourRenderBuffer( Castor::ePIXEL_FORMAT p_eFormat );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~ColourRenderBuffer();
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Render buffer receiving depth component
	\~french
	\brief		Render tampon recevant une composante de profondeur
	*/
	class C3D_API DepthRenderBuffer : public RenderBuffer
	{
	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_ePixelFormat	The buffer pixel format
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_ePixelFormat	Le format des pixels du tampon
		 */
		DepthRenderBuffer( Castor::ePIXEL_FORMAT p_eFormat );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~DepthRenderBuffer();
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Render buffer receiving stencil component
	\~french
	\brief		Render tampon recevant une composante de stencil
	*/
	class C3D_API StencilRenderBuffer : public RenderBuffer
	{
	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_ePixelFormat	The buffer pixel format
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_ePixelFormat	Le format des pixels du tampon
		 */
		StencilRenderBuffer( Castor::ePIXEL_FORMAT p_eFormat );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~StencilRenderBuffer();
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Description of an attachment between a frame buffer and a render buffer
	\~french
	\brief		Description d'une liaison entre un tampon d'image et un tamon de rendu
	*/
	class C3D_API FrameBufferAttachment
	{
	private:
		eATTACHMENT_POINT m_eAttachmentPoint;
		FrameBufferWPtr m_pFrameBuffer;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\~french
		 *\brief		Constructeur
		 */
		FrameBufferAttachment();
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~FrameBufferAttachment();
		/**
		 *\~english
		 *\brief		Attaches the render buffer to the given frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pFrameBuffer	The frame buffer
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache le tampon de rendu au tampon d'image donn�, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pFrameBuffer	Le tampon d'image
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Attach( eATTACHMENT_POINT p_eAttachment, FrameBufferSPtr p_pFrameBuffer );
		/**
		 *\~english
		 *\brief		Detaches the render buffer from the frame buffer
		 *\~french
		 *\brief		D�tache le tampon de rendu du tampon d'image
		 */
		virtual void Detach();
		/**
		 *\~english
		 *\brief		Downloads the render buffer data
		 *\param[out]	p_pBuffer	Receives the buffer
		 *\return		\p true if OK
		 *\~french
		 *\brief		R�cup�re les donn�es du tampon
		 *\param[out]	p_pBuffer	Re�oit les donn�es
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DownloadBuffer( Castor::PxBufferBaseSPtr p_pBuffer ) = 0;
		/**
		 *\~english
		 *\return		\p true if the attachment has been made
		 *\~french
		 *\return		\p true si la liaison a �t� faite
		 */
		inline bool HasFrameBuffer()const { return !m_pFrameBuffer.expired(); }
		/**
		 *\~english
		 *\brief		Retrieves the frame buffer
		 *\return		The frame buffer
		 *\~french
		 *\brief		R�cup�re le tampon d'image
		 *\return		Le tampon d'image
		 */
		inline FrameBufferSPtr GetFrameBuffer()const { return m_pFrameBuffer.lock(); }
		/**
		 *\~english
		 *\return		The attachment point
		 *\~french
		 *\return		Le point d'attache
		 */
		inline eATTACHMENT_POINT GetAttachmentPoint()const { return m_eAttachmentPoint; }
		/**
		 *\~english
		 *\brief		Blit this attachment's buffer into the given one
		 *\remark		Interpolation for depth or stencil buffer must be eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			The destination buffer
		 *\param[in]	p_rectSrc			The source rectangle
		 *\param[in]	p_rectDst			The destination rectangle
		 *\param[in]	p_eInterpolation	The interpolation to apply if the image is stretched
		 *\return		\p true if successful
		 *\~french
		 *\brief		Blitte le tampon de cette attache dans celui donn�
		 *\remark		L'interpolation pour un tampon stencil ou profondeur doit �tre eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			Le tampon destination
		 *\param[in]	p_rectSrc			Le rectangle source
		 *\param[in]	p_rectDst			Le rectangle destination
		 *\param[in]	p_eInterpolation	L'interpolation � appliquer si l'image est redimensionn�e
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Blit( FrameBufferSPtr p_pBuffer, Castor::Rect const & p_rectSrc, Castor::Rect const & p_rectDst, eINTERPOLATION_MODE p_eInterpolation ) = 0;

	protected:
		/**
		 *\~english
		 *\brief		Attaches the render buffer to the given frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pFrameBuffer	The frame buffer
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache le tampon de rendu au tampon d'image donn�, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pFrameBuffer	Le tampon d'image
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DoAttach( eATTACHMENT_POINT p_eAttachment, FrameBufferSPtr p_pFrameBuffer ) = 0;
		/**
		 *\~english
		 *\brief		Detaches the render buffer from the frame buffer
		 *\~french
		 *\brief		D�tache le tampon de rendu du tampon d'image
		 */
		virtual void DoDetach() = 0;
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Description of an attachment between a frame buffer and a render buffer
	\~french
	\brief		Description d'une liaison entre un tampon d'image et un tampon de rendu
	*/
	class C3D_API RenderBufferAttachment : public FrameBufferAttachment
	{
	private:
		RenderBufferWPtr m_pRenderBuffer;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_pRenderBuffer	The render buffer
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_pRenderBuffer	Le tampon de rendu
		 */
		RenderBufferAttachment( RenderBufferSPtr p_pRenderBuffer );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~RenderBufferAttachment();
		/**
		 *\~english
		 *\brief		Attaches the render buffer to the given frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pFrameBuffer	The frame buffer
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache le tampon de rendu au tampon d'image donn�, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pFrameBuffer	Le tampon d'image
		 *\return		\p true si tout s'est bien pass�
		 */
		bool Attach( eATTACHMENT_POINT p_eAttachment, FrameBufferSPtr p_pFrameBuffer );
		/**
		 *\~english
		 *\brief		Detaches the render buffer from the frame buffer
		 *\~french
		 *\brief		D�tache le tampon de rendu du tampon d'image
		 */
		void Detach();
		/**
		 *\~english
		 *\brief		Retrieves the render buffer
		 *\return		The render buffer
		 *\~french
		 *\brief		R�cup�re le tampon de rendu
		 *\return		Le tampon de rendu
		 */
		inline RenderBufferSPtr GetRenderBuffer()const { return m_pRenderBuffer.lock(); }
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Description of an attachment between a frame buffer and a texture
	\~french
	\brief		Description d'une liaison entre un tampon d'image et une texture
	*/
	class C3D_API TextureAttachment : public FrameBufferAttachment
	{
	private:
		DynamicTextureWPtr m_pTexture;
		eTEXTURE_TARGET m_eAttachedTarget;
		int m_iAttachedLayer;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_pTexture	The texture
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_pTexture	La texture
		 */
		TextureAttachment( DynamicTextureSPtr p_pTexture );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~TextureAttachment();
		/**
		 *\~english
		 *\brief		Attaches the render buffer to the given frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pFrameBuffer	The frame buffer
		 *\param[in]	p_eTarget		The dimension to which the texture must be attached
		 *\param[in]	p_iLayer		The associated layer, if p_eDimension equal eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache la texture au tampon d'image donn�, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pFrameBuffer	Le tampon d'image
		 *\param[in]	p_eTarget		La dimension � laquelle la texture doit �tre attach�e
		 *\param[in]	p_iLayer		La couche associ�e, si p_eDimension vaut eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true si tout s'est bien pass�
		 */
		bool Attach( eATTACHMENT_POINT p_eAttachment, FrameBufferSPtr p_pFrameBuffer, eTEXTURE_TARGET p_eTarget, int p_iLayer=0 );
		/**
		 *\~english
		 *\brief		Detaches the render buffer from the frame buffer
		 *\~french
		 *\brief		D�tache la texture du tampon d'image
		 */
		void Detach();
		/**
		 *\~english
		 *\brief		Retrieves the texture
		 *\return		The texture
		 *\~french
		 *\brief		R�cup�re la texture
		 *\return		La texture
		 */
		inline DynamicTextureSPtr GetTexture()const { return m_pTexture.lock(); }
		/**
		 *\~english
		 *\brief		Retrieves the attached dimension
		 *\return		The attached dimension
		 *\~french
		 *\brief		R�cup�re la dimension attach�e
		 *\return		La dimension attach�e
		 */
		inline eTEXTURE_TARGET GetAttachedTarget()const { return m_eAttachedTarget; }
		/**
		 *\~english
		 *\brief		Retrieves the attached layer
		 *\return		The attached layer
		 *\~french
		 *\brief		R�cup�re la couche associ�e
		 *\return		La couche associ�e
		 */
		inline int GetAttachedLayer()const { return m_iAttachedLayer; }
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Frame buffer class
	\remark		A frame buffer is where a render target makes its render
	\~french
	\brief		Classe de tampon d'image
	\remark		Un tampon d'image est l� o� une cible de rendu fait son rendu
	*/
	class C3D_API FrameBuffer
	{
	protected:
		DECLARE_MAP( eATTACHMENT_POINT,	RenderBufferSPtr,	RboAttach	);
		DECLARE_MAP( eATTACHMENT_POINT,	DynamicTextureSPtr,	TexAttach	);
		DECLARE_VECTOR( eATTACHMENT_POINT,		Attach	);
		//!\~english	Attached render buffers map	\~french	Map des tampons de rendu attach�s
		RboAttachMap m_mapRbo;
		TexAttachMap m_mapTex;
		Engine * m_pEngine;
		AttachArray m_arrayAttaches;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\para[in]		p_pEngine	The core engine
		 *\~french
		 *\brief		Constructeur
		 *\para[in]		p_pEngine	Le moteur
		 */
		FrameBuffer( Engine * p_pEngine );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~FrameBuffer();
		/**
		 *\~english
		 *\brief		Creation function
		 *\param[in]	p_iSamplesCount	Samples count, if the frame buffer must support multisampling
		 *\return		\p true if OK
		 *\~french
		 *\brief		Fonction de cr�ation
		 *\param[in]	p_iSamplesCount	Nombre de samples, si le tampon d'image doit supporter le multisampling
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool Create( int p_iSamplesCount ) = 0;
		/**
		 *\~english
		 *\brief		Destruction function
		 *\~french
		 *\brief		Fonction de destruction
		 */
		virtual void Destroy() = 0;
		/**
		 *\~english
		 *\brief		Activation function, to tell the GPU it is active
		 *\param[in]	p_eTarget	The frame buffer binding target
		 *\param[in]	p_eMode		The frame buffer binding mode
		 *\return		\p true if successful
		 *\~french
		 *\brief		Fonction d'activation, pour dire au GPU qu'il est activ�
		 *\param[in]	p_eTarget	La cible d'activation du tampon d'image
		 *\param[in]	p_eMode		Le mode d'activation du tampon d'image
		 *\return		\p true si tout s'est bien pass�
		 */
		bool Bind( eFRAMEBUFFER_MODE p_eMode=eFRAMEBUFFER_MODE_AUTOMATIC, eFRAMEBUFFER_TARGET p_eTarget=eFRAMEBUFFER_TARGET_BOTH );
		/**
		 *\~english
		 *\brief		Deactivation function, to tell the GPU it is inactive
		 *\~french
		 *\brief		Fonction de d�sactivation, pour dire au GPU qu'il est d�sactiv�
		 */
		void Unbind();
		/**
		 *\~english
		 *\brief		Blit this frame buffer into the given one
		 *\param[in]	p_pBuffer			The buffer receiving this one
		 *\param[in]	p_rectSrcDst		The rectangle
		 *\param[in]	p_uiComponents		Bitwise OR of eBUFFER_COMPONENT indicating which buffers are to be copied
		 *\return		\p true if successful
		 *\~french
		 *\brief		Blitte ce tampon dans celui donn�
		 *\param[in]	p_pBuffer			Le tampon recevant celui-ci
		 *\param[in]	p_rectSrcDst		Le rectangle
		 *\param[in]	p_uiComponents		OU logique de eBUFFER_COMPONENT indiquant les buffers � copier
		 *\return		\p true si tout s'est bien pass�
		 */
		bool BlitInto( FrameBufferSPtr p_pBuffer, Castor::Rect const & p_rectSrcDst, uint32_t p_uiComponents );
		/**
		 *\~english
		 *\brief		Blit this frame buffer into the given one
		 *\remark		Interpolation for depth or stencil buffer must be eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			The buffer receiving this one
		 *\param[in]	p_rectSrc			The source rectangle
		 *\param[in]	p_rectDst			The destination rectangle
		 *\param[in]	p_uiComponents		Bitwise OR of eBUFFER_COMPONENT indicating which buffers are to be copied
		 *\param[in]	p_eInterpolation	The interpolation to apply if the image is stretched
		 *\return		\p true if successful
		 *\~french
		 *\brief		Blitte ce tampon dans celui donn�
		 *\remark		L'interpolation pour un tampon stencil ou profondeur doit �tre eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			Le tampon recevant celui-ci
		 *\param[in]	p_rectSrc			Le rectangle source
		 *\param[in]	p_rectDst			Le rectangle destination
		 *\param[in]	p_uiComponents		OU logique de eBUFFER_COMPONENT indiquant les buffers � copier
		 *\param[in]	p_eInterpolation	L'interpolation � appliquer si l'image est redimensionn�e
		 *\return		\p true si tout s'est bien pass�
		 */
		bool StretchInto( FrameBufferSPtr p_pBuffer, Castor::Rect const & p_rectSrc, Castor::Rect const & p_rectDst, uint32_t p_uiComponents, eINTERPOLATION_MODE p_eInterpolation );
		/**
		 *\~english
		 *\brief		Renders this buffer into another buffer's selected components
		 *\param[in]	p_pBuffer		The buffer receiving the render
		 *\param[in]	p_sizeDst		The destination dimensions
		 *\param[in]	p_uiComponents	Bitwise OR of eBUFFER_COMPONENT indicating the selected destination buffer's components
		 *\~french
		 *\brief		Rend ce tampon dans les composantes choisies d'un autre tampon
		 *\param[in]	p_pBuffer		Le tampon recevant le rendu
		 *\param[in]	p_sizeDst		Les dimensions de la destination
		 *\param[in]	p_uiComponents	OU logique de eBUFFER_COMPONENT indiquant les composantes du tampon destination
		 */
		virtual void RenderToBuffer( FrameBufferSPtr p_pBuffer, Castor::Size const & p_sizeDst, uint32_t p_uiComponents, DepthStencilStateSPtr p_pDepthStencilState, RasteriserStateSPtr p_pRasteriserState );
		/**
		 *\~english
		 *\brief		Specifies the color buffer to be drawn into
		 *\param[in]	p_eAttach	The color buffer
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit le tampon de couleur dans lequel le dessin doit �tre effectu�
		 *\param[in]	p_eAttach	Le tampon de couleur
		 *\return		\p true si tout s'est bien pass�
		 */
		bool SetDrawBuffer( eATTACHMENT_POINT p_eAttach )
		{
			return SetDrawBuffers( 1, &p_eAttach );
		}
		/**
		 *\~english
		 *\brief		Specifies the color buffers to be drawn into
		 *\param[in]	p_uiSize	The buffers count
		 *\param[in]	p_eAttach	The color buffers
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit les buffers de couleur dans lesquel le dessin doit �tre effectu�
		 *\param[in]	p_uiSize	Le nombre de buffers
		 *\param[in]	p_eAttach	Les buffers de couleur
		 *\return		\p true si tout s'est bien pass�
		 */
		template< uint32_t N > bool SetDrawBuffers( eATTACHMENT_POINT p_eAttach[N] ) { return SetDrawBuffers( N, p_eAttach ); }
		/**
		 *\~english
		 *\brief		Specifies the color buffers to be drawn into
		 *\param[in]	p_arrayAttaches	The color buffers
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit les buffers de couleur dans lesquel le dessin doit �tre effectu�
		 *\param[in]	p_arrayAttaches	Les buffers de couleur
		 *\return		\p true si tout s'est bien pass�
		 */
		inline bool SetDrawBuffers( std::vector< eATTACHMENT_POINT > const & p_arrayAttaches ) { return SetDrawBuffers( uint32_t( p_arrayAttaches.size() ), &p_arrayAttaches[0] ); }
		/**
		 *\~english
		 *\brief		Specifies the color buffers to be drawn into
		 *\param[in]	p_uiSize	The buffers count
		 *\param[in]	p_eAttach	The color buffers
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit les buffers de couleur dans lesquel le dessin doit �tre effectu�
		 *\param[in]	p_uiSize	Le nombre de buffers
		 *\param[in]	p_eAttach	Les buffers de couleur
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool SetDrawBuffers( uint32_t p_uiSize, eATTACHMENT_POINT const * p_eAttach ) = 0;
		/**
		 *\~english
		 *\brief		Specifies the buffers to be drawn into
		 *\remark		All buffers attached are selected
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit les buffers dans lesquels le dessin doit �tre effectu�
		 *\remark		Tous les buffers attach�s sont s�lectionn�s
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool SetDrawBuffers() = 0;
		/**
		 *\~english
		 *\brief		Specifies the color buffer source for pixels
		 *\param[in]	p_eAttach	The color buffer
		 *\return		\p true if successful
		 *\~french
		 *\brief		D�finit le tampon de couleur source pour la lecture de pixels
		 *\param[in]	p_eAttach	Le tampon de couleur
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool SetReadBuffer( eATTACHMENT_POINT p_eAttach ) = 0;
		/**
		 *\~english
		 *\brief		Creates a colour render buffer
		 *\param[in]	p_ePixelFormat	The buffer's pixel format
		 *\~french
		 *\brief		Cr�e un tampon de rendu couleur
		 *\param[in]	p_ePixelFormat	Le fromat de pixels du tampon
		 */
		virtual ColourRenderBufferSPtr CreateColourRenderBuffer( Castor::ePIXEL_FORMAT p_ePixelFormat ) = 0;
		/**
		 *\~english
		 *\brief		Creates a depth render buffer
		 *\param[in]	p_ePixelFormat	The buffer's pixel format
		 *\~french
		 *\brief		Cr�e un tampon de rendu profondeur
		 *\param[in]	p_ePixelFormat	Le fromat de pixels du tampon
		 */
		virtual DepthRenderBufferSPtr CreateDepthRenderBuffer( Castor::ePIXEL_FORMAT p_ePixelFormat ) = 0;
		/**
		 *\~english
		 *\brief		Creates a stencil render buffer
		 *\param[in]	p_ePixelFormat	The buffer's pixel format
		 *\~french
		 *\brief		Cr�e un tampon de rendu stencil
		 *\param[in]	p_ePixelFormat	Le fromat de pixels du tampon
		 */
		virtual StencilRenderBufferSPtr CreateStencilRenderBuffer( Castor::ePIXEL_FORMAT p_ePixelFormat ) = 0;
		/**
		 *\~english
		 *\brief		Attaches a render buffer to this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		Attache un tampon de rendu � ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		void Attach( RenderBufferAttachmentRPtr p_pAttach );
		/**
		 *\~english
		 *\brief		Detaches a render buffer from this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		D�tache un tampon de rendu de ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		void Detach( RenderBufferAttachmentRPtr p_pAttach );
		/**
		 *\~english
		 *\brief		Attaches a texture to this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		Attache une texture � ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		void Attach( TextureAttachmentRPtr p_pAttach );
		/**
		 *\~english
		 *\brief		Detaches a texture from this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		D�tache une texture de ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		void Detach( TextureAttachmentRPtr p_pAttach );
		/**
		 *\~english
		 *\brief		Attaches a texture to this frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pTexture		The texture
		 *\param[in]	p_eTarget		The dimension to which the texture must be attached
		 *\param[in]	p_iLayer		The associated layer, if p_eDimension equal eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache une texture � ce tampon d'image, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pTexture		La texture
		 *\param[in]	p_eTarget		La dimension � laquelle la texture doit �tre attach�e
		 *\param[in]	p_iLayer		La couche associ�e, si p_eDimension vaut eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true si tout s'est bien pass�
		 */
		bool Attach( eATTACHMENT_POINT p_eAttachment, DynamicTextureSPtr p_pTexture, eTEXTURE_TARGET p_eTarget, int p_iLayer=0 );
		/**
		 *\~english
		 *\brief		Attaches a render buffer to this frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pRenderBuffer	The render buffer
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache un tampon de rendu � ce tampon d'image, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pRenderBuffer	Le tampon de rendu
		 *\return		\p true si tout s'est bien pass�
		 */
		bool Attach( eATTACHMENT_POINT p_eAttachment, RenderBufferSPtr p_pRenderBuffer );
		/**
		 *\~english
		 *\brief		Detaches all attached objects
		 *\~french
		 *\brief		D�tache tous les objets attach�s
		 */
		void DetachAll();
		/**
		 *\~english
		 *\brief		Resizes each attached buffer
		 *\param[in]	p_size	The new dimensions
		 *\~french
		 *\brief		Redimensionne tous les tampons attach�s
		 *\param[in]	p_size	Les nouvelles dimensions
		 */
		virtual void Resize( Castor::Size const & p_size );
		/**
		 *\~english
		 *\brief		Checks if the FBO is complete
		 *\return		\p false if the buffer is in error if there is an attachment missing
		 *\~french
		 *\brief		V�rifies i le FBO est complet
		 *\return		\p false si le tampon est en erreur ou s'il manque une attache.
		 */
		virtual bool IsComplete()=0;

	protected:
		/**
		 *\~english
		 *\brief		Activation function, to tell the GPU it is active
		 *\param[in]	p_eTarget	The frame buffer binding target
		 *\return		\p true if successful
		 *\~french
		 *\brief		Fonction d'activation, pour dire au GPU qu'il est activ�
		 *\param[in]	p_eTarget	La cible d'activation du tampon d'image
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DoBind( eFRAMEBUFFER_TARGET p_eTarget ) = 0;
		/**
		 *\~english
		 *\brief		Deactivation function, to tell the GPU it is inactive
		 *\~french
		 *\brief		Fonction de d�sactivation, pour dire au GPU qu'il est d�sactiv�
		 */
		virtual void DoUnbind() = 0;
		/**
		 *\~english
		 *\brief		Attaches a render buffer to this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		Attache un tampon de rendu � ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		virtual void DoAttach( RenderBufferAttachmentRPtr p_pAttach )=0;
		/**
		 *\~english
		 *\brief		Detaches a render buffer from this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		D�tache un tampon de rendu de ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		virtual void DoDetach( RenderBufferAttachmentRPtr p_pAttach )=0;
		/**
		 *\~english
		 *\brief		Attaches a texture to this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		Attache un texture � ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		virtual void DoAttach( TextureAttachmentRPtr p_pAttach )=0;
		/**
		 *\~english
		 *\brief		Detaches a texture from this frame buffer
		 *\param[in]	p_pAttach	The attachment
		 *\~french
		 *\brief		D�tache une texture de ce tampon d'image
		 *\param[in]	p_pAttach	L'attache
		 */
		virtual void DoDetach( TextureAttachmentRPtr p_pAttach )=0;
		/**
		 *\~english
		 *\brief		Attaches a texture to this frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pTexture		The texture
		 *\param[in]	p_eTarget		The dimension to which the texture must be attached
		 *\param[in]	p_iLayer		The associated layer, if p_eDimension equal eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache une texture � ce tampon d'image, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pTexture		La texture
		 *\param[in]	p_eTarget		La dimension � laquelle la texture doit �tre attach�e
		 *\param[in]	p_iLayer		La couche associ�e, si p_eDimension vaut eTEXTURE_TARGET_3D or eTEXTURE_TARGET_LAYER
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DoAttach( eATTACHMENT_POINT p_eAttachment, DynamicTextureSPtr p_pTexture, eTEXTURE_TARGET p_eTarget, int p_iLayer=0 )=0;
		/**
		 *\~english
		 *\brief		Attaches a render buffer to this frame buffer, at given attachment point
		 *\param[in]	p_eAttachment	The attachment point
		 *\param[in]	p_pRenderBuffer	The tampon de rendu
		 *\return		\p true if OK
		 *\~french
		 *\brief		Attache un tampon de rendu � ce tampon d'image, au point d'attache voulu
		 *\param[in]	p_eAttachment	Le point d'attache
		 *\param[in]	p_pRenderBuffer	Le tampon de rendu
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DoAttach( eATTACHMENT_POINT p_eAttachment, RenderBufferSPtr p_pRenderBuffer )=0;
		/**
		 *\~english
		 *\brief		Detaches all attached objects
		 *\~french
		 *\brief		D�tache tous les objets attach�s
		 */
		virtual void DoDetachAll()=0;
		/**
		 *\~english
		 *\brief		Blit this frame buffer into the given one
		 *\remark		Interpolation for depth or stencil buffer must be eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			The buffer receiving this one
		 *\param[in]	p_rectSrc			The source rectangle
		 *\param[in]	p_rectDst			The destination rectangle
		 *\param[in]	p_uiComponents		Bitwise OR of eBUFFER_COMPONENT indicating which buffers are to be copied
		 *\param[in]	p_eInterpolation	The interpolation to apply if the image is stretched
		 *\return		\p true if successful
		 *\~french
		 *\brief		Blitte ce tampon dans celui donn�
		 *\remark		L'interpolation pour un tampon stencil ou profondeur doit �tre eINTERPOLATION_MODE_NEAREST
		 *\param[in]	p_pBuffer			Le tampon recevant celui-ci
		 *\param[in]	p_rectSrc			Le rectangle source
		 *\param[in]	p_rectDst			Le rectangle destination
		 *\param[in]	p_uiComponents		OU logique de eBUFFER_COMPONENT indiquant les buffers � copier
		 *\param[in]	p_eInterpolation	L'interpolation � appliquer si l'image est redimensionn�e
		 *\return		\p true si tout s'est bien pass�
		 */
		virtual bool DoStretchInto( FrameBufferSPtr p_pBuffer, Castor::Rect const & p_rectSrc, Castor::Rect const & p_rectDst, uint32_t p_uiComponents, eINTERPOLATION_MODE p_eInterpolation ) = 0;

	private:
		void DoTexAttach( eATTACHMENT_POINT p_eAttach, DynamicTextureSPtr p_pTexture );
		void DoRboAttach( eATTACHMENT_POINT p_eAttach, RenderBufferSPtr p_pRenderBuffer );
		void DoDetach( eATTACHMENT_POINT p_eAttach );
	};
}

#pragma warning( pop )

#endif
