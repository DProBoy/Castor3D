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
#ifndef ___C3D_MATERIAL_H___
#define ___C3D_MATERIAL_H___

#include "Castor3DPrerequisites.hpp"

#include <Design/OwnedBy.hpp>
#include <Design/Resource.hpp>

namespace Castor3D
{
	/*!
	\author		Sylvain DOREMUS
	\version	0.1
	\date		09/02/2010
	\~english
	\brief		Definition of a material
	\remark		A material is composed of one or more passes.
	\~french
	\brief		Définition d'un matériau
	\remark		Un matériau est composé d'une ou plusieurs passes
	*/
	class Material
		: public Castor::Resource< Material >
		, public std::enable_shared_from_this< Material >
		, public Castor::OwnedBy< Engine >
	{
	public:
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Material loader
		\remark
		\~french
		\brief Loader de Material
		*/
		class TextWriter
			: public Castor::TextWriter< Material >
		{
		public:
			/**
			 *\~english
			 *\brief		Constructor
			 *\~french
			 *\brief		Constructeur
			 */
			C3D_API explicit TextWriter( Castor::String const & p_tabs );
			/**
			 *\~english
			 *\brief			Writes a material into a text file
			 *\param[in]		p_material	The material to save
			 *\param[in,out]	p_file		The file where to save the material
			 *\~french
			 *\brief			Ecrit un matériau dans un fichier texte
			 *\param[in]		p_material	Le matériau
			 *\param[in,out]	p_file		Le fichier
			 */
			C3D_API bool operator()( Material const & p_material, Castor::TextFile & p_file )override;
		};

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_name		The material name
		 *\param[in]	p_engine	The core engine
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_name		Le nom du matériau
		 *\param[in]	p_engine	Le moteur
		 */
		C3D_API Material( Castor::String const & p_name, Engine & p_engine );
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		C3D_API virtual ~Material();
		/**
		 *\~english
		 *\brief		Initialises the material and all it's passes
		 *\~french
		 *\brief		Initialise le matériau et toutes ses passes
		 */
		C3D_API void Initialise();
		/**
		 *\~english
		 *\brief		Flushes passes
		 *\~french
		 *\brief		Supprime les passes
		 */
		C3D_API void Cleanup();
		/**
		 *\~english
		 *\brief		Creates a pass
		 *\return		The created pass
		 *\~french
		 *\brief		Crée une passe
		 *\return		La passe créée
		 */
		C3D_API PassSPtr CreatePass();
		/**
		 *\~english
		 *\brief		Retrieves a pass and returns it
		 *\param[in]	p_index	The index of the wanted pass
		 *\return		The retrieved pass or nullptr if not found
		 *\~french
		 *\brief		Récupère une passe
		 *\param[in]	p_index	L'index de la passe voulue
		 *\return		La passe récupére ou nullptr si non trouvés
		 */
		C3D_API const PassSPtr GetPass( uint32_t p_index )const;
		/**
		 *\~english
		 *\brief		Retrieves a pass and returns it
		 *\param[in]	p_index	The index of the wanted pass
		 *\return		The retrieved pass or nullptr if not found
		 *\~french
		 *\brief		Récupère une passe
		 *\param[in]	p_index	L'index de la passe voulue
		 *\return		La passe récupére ou nullptr si non trouvés
		 */
		C3D_API PassSPtr GetPass( uint32_t p_index );
		/**
		 *\~english
		 *\brief		Destroys the pass at the given index
		 *\param[in]	p_index	The pass index
		 *\~french
		 *\brief		Destroys the pass at the given index
		 *\param[in]	p_index	L'index de la passe
		 */
		C3D_API void DestroyPass( uint32_t p_index );
		/**
		*\~english
		*\return		\p true if all passes needs alpha blending
		*\~french
		*\return		\p true si toutes les passes ont besoin d'alpha blending
		*/
		C3D_API bool HasAlphaBlending()const;
		/**
		 *\~english
		 *\brief		Retrieves the passes count
		 *\return		The count
		 *\~french
		 *\brief		Récupère le nombre de passes
		 *\return		Le nombre
		 */
		inline uint32_t GetPassCount()const
		{
			return uint32_t( m_passes.size() );
		}
		/**
		 *\~english
		 *\brief		Retrieves a constant iterator on the beginning of the passes array
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur constant sur le début du tableau de passes
		 *\return		L'itérateur
		 */
		inline PassPtrArrayConstIt begin()const
		{
			return m_passes.begin();
		}
		/**
		 *\~english
		 *\brief		Retrieves an iterator on the beginning of the passes array
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur sur le début du tableau de passes
		 *\return		L'itérateur
		 */
		inline PassPtrArrayIt begin()
		{
			return m_passes.begin();
		}
		/**
		 *\~english
		 *\brief		Retrieves a constant iterator on the end of the passes array
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur constant sur la fin du tableau de passes
		 *\return		L'itérateur
		 */
		inline PassPtrArrayConstIt end()const
		{
			return m_passes.end();
		}
		/**
		 *\~english
		 *\brief		Retrieves an iterator on the end of the passes array
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur sur la fin du tableau de passes
		 *\return		L'itérateur
		 */
		inline PassPtrArrayIt end()
		{
			return m_passes.end();
		}

	public:
		//!\~english The default material name	\~french Le nom du matériau par défaut
		static const Castor::String DefaultMaterialName;

	private:
		PassPtrArray m_passes;
	};
}

#endif
