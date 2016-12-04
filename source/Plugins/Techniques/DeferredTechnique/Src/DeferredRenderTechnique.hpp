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
#ifndef ___C3D_DEFERRED_SHADING_RENDER_TECHNIQUE_H___
#define ___C3D_DEFERRED_SHADING_RENDER_TECHNIQUE_H___

#include <Mesh/Buffer/BufferDeclaration.hpp>
#include <Technique/RenderTechnique.hpp>
#include <Render/Viewport.hpp>

namespace Deferred
{
	using Castor3D::Point3rFrameVariable;

	enum class DsTexture
		: uint8_t
	{
		ePosition,
		eDiffuse,
		eNormals,
		eTangent,
		eSpecular,
		eEmissive,
		CASTOR_SCOPED_ENUM_BOUNDS( ePosition ),
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.7.0.0
	\date		12/11/2012
	\~english
	\brief		Deferred lighting Render technique class
	\remarks	Creates up to 6 render buffers (position, diffuse, normals, tangents, bitangents, specular)
				<br />it renders to these buffers (Geometry Pass)
				<br />then renders these buffers into the final one, adding the lights (Light Pass)
	\~french
	\brief		Classe de technique de rendu implémentant le Deferred lighting
	\remarks	On crée jusqu'à 6 tampons de rendu (position, diffuse, normales, tangentes, bitangentes, spéculaire)
				<br />on fait le rendu dans ces tampons (Geometry Pass)
				<br />puis on fait un rendu de ces tampons en ajoutant les lumières (Light Pass)
	*/
	class RenderTechnique
		: public Castor3D::RenderTechnique
	{
	protected:
		DECLARE_SMART_PTR( Point3rFrameVariable );

	protected:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_renderTarget	The render target for this technique
		 *\param[in]	p_renderSystem	The render system
		 *\param[in]	p_params		The technique parameters
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_renderTarget	La render target pour cette technique
		 *\param[in]	p_renderSystem	Le render system
		 *\param[in]	p_params		Les paramètres de la technique
		 */
		RenderTechnique( Castor3D::RenderTarget & p_renderTarget, Castor3D::RenderSystem & p_renderSystem, Castor3D::Parameters const & p_params );

	public:
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~RenderTechnique();
		/**
		 *\~english
		 *\brief		Instantiation function, used by the factory to create objects of a wanted type
		 *\param[in]	p_renderTarget	The technique render target
		 *\param[in]	p_renderSystem	The render system
		 *\param[in]	p_params		The technique parameters
		 *\return		A clone of this object
		 *\~french
		 *\brief		Fonction d'instanciation, utilisée par la fabrique pour créer des objets d'un type donné
		 *\param[in]	p_renderTarget	La cible de rendu de la technique
		 *\param[in]	p_renderSystem	Le render system
		 *\param[in]	p_params		Les paramètres de la technique
		 *\return		Un clône de cet objet
		 */
		static Castor3D::RenderTechniqueSPtr CreateInstance( Castor3D::RenderTarget & p_renderTarget, Castor3D::RenderSystem & p_renderSystem, Castor3D::Parameters const & p_params );

	protected:
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoGetDepthMaps
		 */
		void DoGetDepthMaps( Castor3D::DepthMapArray & p_depthMaps )override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoCreate
		 */
		bool DoCreate()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoDestroy
		 */
		void DoDestroy()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoInitialise
		 */
		bool DoInitialise( uint32_t & p_index )override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoCleanup
		 */
		void DoCleanup()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoBeginRender
		 */
		bool DoBeginRender()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoBeginOpaqueRendering
		 */
		bool DoBeginOpaqueRendering()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoEndOpaqueRendering
		 */
		void DoEndOpaqueRendering()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoBeginTransparentRendering
		 */
		bool DoBeginTransparentRendering()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoEndTransparentRendering
		 */
		void DoEndTransparentRendering()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoEndRender
		 */
		void DoEndRender()override;
		/**
		 *\copydoc		Castor3D::RenderTechnique::DoWriteInto
		 */
		bool DoWriteInto( Castor::TextFile & p_file )override;
		/**
		 *\copydoc		Castor3D::RenderPass::DoCompleteProgramFlags
		 */
		void DoCompleteProgramFlags( Castor::FlagCombination< Castor3D::ProgramFlag > & p_programFlags )const override;
		/**
		 *\copydoc		Castor3D::RenderPass::DoGetOpaquePixelShaderSource
		 */
		Castor::String DoGetOpaquePixelShaderSource(
			Castor::FlagCombination< Castor3D::TextureChannel > const & p_textureFlags,
			Castor::FlagCombination< Castor3D::ProgramFlag > const & p_programFlags,
			uint8_t p_sceneFlags )const override;
		/**
		 *\copydoc		Castor3D::RenderPass::DoUpdateOpaquePipeline
		 */
		void DoUpdateOpaquePipeline( Castor3D::RenderPipeline & p_pipeline, Castor3D::DepthMapArray & p_depthMaps )const override;
		/**
		 *\~english
		 *\brief		Retrieves the vertex shader source matching the given flags
		 *\param[in]	p_programFlags	Bitwise ORed ProgramFlag
		 *\~french
		 *\brief		Récupère le source du vertex shader correspondant aux flags donnés
		 *\param[in]	p_programFlags	Une combinaison de ProgramFlag
		 */
		Castor::String DoGetLightPassVertexShaderSource(
			Castor::FlagCombination< Castor3D::TextureChannel > const & p_textureFlags,
			Castor::FlagCombination< Castor3D::ProgramFlag > const & p_programFlags,
			uint8_t p_sceneFlags )const;
		/**
		 *\~english
		 *\brief		Retrieves the pixel shader source matching the given flags
		 *\param[in]	p_textureFlags	A combination of TextureChannel
		 *\~french
		 *\brief		Récupère le source du pixel shader correspondant aux flags donnés
		 *\param[in]	p_textureFlags	Une combinaison de TextureChannel
		 */
		Castor::String DoGetLightPassPixelShaderSource(
			Castor::FlagCombination< Castor3D::TextureChannel > const & p_textureFlags,
			Castor::FlagCombination< Castor3D::ProgramFlag > const & p_programFlags,
			uint8_t p_sceneFlags )const;
		/**
		 *\~english
		 *\brief		Binds the depth maps, beginning at p_startIndex.
		 *\param[in]	p_startIndex	The starting index.
		 *\~french
		 *\brief		Active les textures de profondeur, en commençant à p_startIndex.
		 *\param[in]	p_textureFlags	L'index de départ.
		 */
		void DoBindDepthMaps( uint32_t p_startIndex );
		/**
		 *\~english
		 *\brief		Unbinds the depth maps, beginning at p_startIndex.
		 *\param[in]	p_startIndex	The starting index.
		 *\~french
		 *\brief		Désactive les textures de profondeur, en commençant à p_startIndex.
		 *\param[in]	p_textureFlags	L'index de départ.
		 */
		void DoUnbindDepthMaps( uint32_t p_startIndex )const;

	private:
		struct LightPassProgram
		{
			//!\~english	The shader program used to render lights.
			//!\~french		Le shader utilisé pour rendre les lumières.
			Castor3D::ShaderProgramSPtr m_program;
			//!\~english	The framve variable buffer used to apply matrices.
			//!\~french		Le tampon de variables shader utilisé pour appliquer les matrices.
			Castor3D::FrameVariableBufferSPtr m_matrixUbo;
			//!\~english	The framve variable buffer used to transmit scene values.
			//!\~french		Le tampon de variables shader utilisé pour transmettre les variables de scène.
			Castor3D::FrameVariableBufferSPtr m_sceneUbo;
			//!\~english	The shader variable containing the camera position.
			//!\~french		La variable de shader contenant la position de la caméra.
			Castor3D::Point3rFrameVariableSPtr m_camera;
			//!\~english	Geometry buffers holder.
			//!\~french		Conteneur de buffers de géométries.
			Castor3D::GeometryBuffersSPtr m_geometryBuffers;
			//!\~english	The pipeline used by the light pass.
			//!\~french		Le pipeline utilisé par la passe lumières.
			Castor3D::RenderPipelineSPtr m_pipeline;
		};

	private:
		//!\~english	The various textures.
		//!\~french		Les diverses textures.
		std::array< Castor3D::TextureUnitUPtr, size_t( DsTexture::eCount ) > m_lightPassTextures;
		//!\~english	The depth buffer.
		//!\~french		Le tampon de profondeur.
		Castor3D::RenderBufferSPtr m_lightPassDepthBuffer;
		//!\~english	The deferred shading frame buffer.
		//!\~french		Le tampon d'image pour le deferred shading.
		Castor3D::FrameBufferSPtr m_geometryPassFrameBuffer;
		//!\~english	The attachments between textures and deferred shading frame buffer.
		//!\~french		Les attaches entre les textures et le tampon deferred shading.
		std::array< Castor3D::TextureAttachmentSPtr, size_t( DsTexture::eCount ) > m_geometryPassTexAttachs;
		//!\~english	The attachment between depth buffer and deferred shading frame buffer.
		//!\~french		L'attache entre le tampon de profondeur et le tampon deferred shading.
		Castor3D::RenderBufferAttachmentSPtr m_geometryPassDepthAttach;
		//!\~english	The shader program used to render lights.
		//!\~french		Le shader utilisé pour rendre les lumières.
		std::array< LightPassProgram, size_t( Castor3D::FogType::eCount ) > m_lightPassShaderPrograms;
		//!\~english	Buffer elements declaration.
		//!\~french		Déclaration des éléments d'un vertex.
		Castor3D::BufferDeclaration m_declaration;
		//!\~english	Vertex array (quad definition).
		//!\~french		Tableau de vertex (définition du quad).
		std::array< Castor3D::BufferElementGroupSPtr, 6 > m_arrayVertex;
		//!\~english	The vertex buffer.
		//!\~french		Le tampon de sommets.
		Castor3D::VertexBufferSPtr m_vertexBuffer;
		//!\~english	The viewport used when rendering is done.
		//!\~french		Le viewport utilisé pour rendre la cible sur sa cible (fenêtre ou texture).
		Castor3D::Viewport m_viewport;
	};
}

#endif
