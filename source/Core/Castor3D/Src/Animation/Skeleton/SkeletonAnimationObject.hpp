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
#ifndef ___C3D_SKELETON_ANIMATION_OBJECT_H___
#define ___C3D_SKELETON_ANIMATION_OBJECT_H___

#include "Binary/BinaryParser.hpp"
#include "Binary/BinaryWriter.hpp"
#include "Animation/Interpolator.hpp"

#include <Math/SquareMatrix.hpp>
#include <Math/Quaternion.hpp>

namespace castor3d
{
	/*!
	\author 	Sylvain DOREMUS
	\version	0.1
	\date		09/02/2010
	\~english
	\brief		Class which represents the skeleton animation objects.
	\remark		Manages translation, scaling, rotation of the object.
	\~french
	\brief		Classe de représentation d'objets d'animations de squelette.
	\remark		Gère les translations, mises à l'échelle, rotations de l'objet.
	*/
	class SkeletonAnimationObject
		: public castor::OwnedBy< SkeletonAnimation >
		, public std::enable_shared_from_this< SkeletonAnimationObject >
	{
	protected:
		/**
		 *\~english
		 *\brief		Constructor.
		 *\param[in]	p_animation	The parent animation.
		 *\param[in]	p_type		The skeleton animation object type.
		 *\~french
		 *\brief		Constructeur.
		 *\param[in]	p_animation	L'animation parente.
		 *\param[in]	p_type		Le type d'objet d'animation de squelette.
		 */
		C3D_API SkeletonAnimationObject( SkeletonAnimation & p_animation, SkeletonAnimationObjectType p_type );
		/**
		 *\~english
		 *\brief		Copy constructor.
		 *\~french
		 *\brief		Constructeur par copie.
		 */
		C3D_API SkeletonAnimationObject( SkeletonAnimationObject const & p_rhs ) = delete;

	public:
		/**
		 *\~english
		 *\brief		Destructor.
		 *\~french
		 *\brief		Destructeur.
		 */
		C3D_API virtual ~SkeletonAnimationObject();
		/**
		 *\~english
		 *\brief		adds a child to this object.
		 *\remarks		The child's transformations are affected by this object's ones.
		 *\param[in]	p_object	The child.
		 *\~french
		 *\brief		Ajoute un objet enfant à celui-ci.
		 *\remarks		Les transformations de l'enfant sont affectées par celles de cet objet.
		 *\param[in]	p_object	L'enfant.
		 */
		C3D_API void addChild( SkeletonAnimationObjectSPtr p_object );
		/**
		 *\~english
		 *\brief		Retrieves the object name.
		 *\return		The name.
		 *\~french
		 *\brief		Récupère le nom de l'objet.
		 *\return		Le nom.
		 */
		C3D_API virtual castor::String const & getName()const = 0;
		/**
		 *\~english
		 *\brief		Creates a scaling key frame and adds it to the list.
		 *\remarks		If a key frame with the same starting time already exists, it is returned, but not modified.
		 *\param[in]	p_from		The starting time.
		 *\param[in]	p_translate	The translation at start time.
		 *\param[in]	p_rotate	The rotation at start time.
		 *\param[in]	p_scale		The scaling at start time.
		 *\~french
		 *\brief		Crée une key frame de mise à l'échelle et l'ajoute à la liste.
		 *\remarks		Si une key frame avec le même index de temps de début existe, elle est retournée sans être modifiée.
		 *\param[in]	p_from		L'index de temps de début.
		 *\param[in]	p_translate	La translation au temps de début.
		 *\param[in]	p_rotate	La rotation au temps de début.
		 *\param[in]	p_scale		L'échelle au temps de début.
		 */
		C3D_API KeyFrame & addKeyFrame( castor::Milliseconds const & p_from
			, castor::Point3r const & p_translate = castor::Point3r{}
		, castor::Quaternion const & p_rotate = castor::Quaternion{}
		, castor::Point3r const & p_scale = castor::Point3r{ 1.0_r, 1.0_r, 1.0_r } );
		/**
		 *\~english
		 *\brief		Deletes the scaling key frame at time index p_time.
		 *\param[in]	p_time	The time index.
		 *\~french
		 *\brief		Supprime la key frame de mise à l'échelle à l'index de temps donné.
		 *\param[in]	p_time	L'index de temps.
		 */
		C3D_API void removeKeyFrame( castor::Milliseconds const & p_time );
		/**
		 *\~english
		 *\return		The scaling key frames interpolation mode.
		 *\~french
		 *\return		Le mode d'interpolation des key frames de mise à l'échelle.
		 */
		inline InterpolatorType getInterpolationMode()const
		{
			return m_mode;
		}
		/**
		 *\~english
		 *\return		The key frames.
		 *\~french
		 *\return		Les key frames.
		 */
		inline KeyFrameArray const & getKeyFrames()const
		{
			return m_keyframes;
		}
		/**
		 *\~english
		 *\return		The animation length.
		 *\~french
		 *\return		La durée de l'animation.
		 */
		inline castor::Milliseconds const & getLength()const
		{
			return m_length;
		}
		/**
		 *\~english
		 *\return		The moving object type.
		 *\~french
		 *\return		Le type d'objet mouvant.
		 */
		inline SkeletonAnimationObjectType getType()const
		{
			return m_type;
		}
		/**
		 *\~english
		 *\brief		sets the animation length.
		 *\param[in]	p_length	The new value.
		 *\~french
		 *\brief		Définit la durée de l'animation.
		 *\param[in]	p_length	La nouvelle valeur.
		 */
		inline void	setLength( castor::Milliseconds const & p_length )
		{
			m_length = p_length;
		}
		/**
		 *\~english
		 *\brief		Retrieves the animation node transformation.
		 *\return		The value.
		 *\~french
		 *\brief		Récupère les animations du noeud de transformation.
		 *\return		La valeur.
		 */
		inline castor::Matrix4x4r const & getNodeTransform()const
		{
			return m_nodeTransform;
		}
		/**
		 *\~english
		 *\brief		sets the animation node transformation.
		 *\param[in]	p_transform	The new value.
		 *\~french
		 *\brief		Définit les animations du noeud de transformation.
		 *\param[in]	p_transform	La nouvelle valeur.
		 */
		inline void setNodeTransform( castor::Matrix4x4r const & p_transform )
		{
			m_nodeTransform = p_transform;
		}
		/**
		 *\~english
		 *\brief		Tells whether or not the object has keyframes.
		 *\return		\p false if no keyframes.
		 *\~french
		 *\brief		Dit si l'objet a des keyframes.
		 *\return		\p false si pas de keyframes.
		 */
		inline bool hasKeyFrames()const
		{
			return !m_keyframes.empty();
		}
		/**
		 *\~english
		 *\return		The children array.
		 *\~french
		 *\return		Le tableau d'enfants.
		 */
		inline SkeletonAnimationObjectPtrArray const & getChildren()const
		{
			return m_children;
		}
		/**
		 *\~english
		 *\return		The parent object.
		 *\~french
		 *\return		L'objet parent.
		 */
		inline SkeletonAnimationObjectSPtr getParent()const
		{
			return m_parent.lock();
		}

	protected:
		//!\~english	The interpolation mode.
		//!\~french		Le mode d'interpolation.
		InterpolatorType m_mode{ InterpolatorType::eCount };
		//!\~english	The animation length.
		//!\~french		La durée de l'animation.
		castor::Milliseconds m_length{ 0 };
		//!\~english	The moving thing type.
		//!\~french		Le type du machin mouvant.
		SkeletonAnimationObjectType m_type;
		//!\~english	The key frames.
		//!\~french		Les keyframes.
		KeyFrameArray m_keyframes;
		//!\~english	Animation node transformations.
		//!\~french		Transformations du noeud d'animation.
		castor::Matrix4x4r m_nodeTransform;
		//!\~english	The objects depending on this one.
		//!\~french		Les objets dépendant de celui-ci.
		SkeletonAnimationObjectPtrArray m_children;
		//!\~english	The parent object.
		//!\~french		L'objet parent.
		SkeletonAnimationObjectWPtr m_parent;

		friend class BinaryWriter< SkeletonAnimationObject >;
		friend class BinaryParser< SkeletonAnimationObject >;
		friend class SkeletonAnimationInstanceObject;
	};
	/*!
	\author 	Sylvain DOREMUS
	\version	0.9.0
	\date 		28/05/2016
	\~english
	\brief		Helper structure to find ChunkType from a type.
	\remarks	Specialisation for SkeletonAnimationObject.
	\~french
	\brief		Classe d'aide pour récupéer un ChunkType depuis un type.
	\remarks	Spécialisation pour SkeletonAnimationObject.
	*/
	template<>
	struct ChunkTyper< SkeletonAnimationObject >
	{
		static ChunkType const Value = ChunkType::eAnimationObject;
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.8.0
	\date		26/01/2016
	\~english
	\brief		SkeletonAnimationObject binary loader.
	\~english
	\brief		Loader binaire de SkeletonAnimationObject.
	*/
	template<>
	class BinaryWriter< SkeletonAnimationObject >
		: public BinaryWriterBase< SkeletonAnimationObject >
	{
	protected:
		/**
		 *\~english
		 *\brief		Function used to fill the chunk from specific data.
		 *\param[in]	p_obj	The object to write.
		 *\return		\p false if any error occured.
		 *\~french
		 *\brief		Fonction utilisée afin de remplir le chunk de données spécifiques.
		 *\param[in]	p_obj	L'objet à écrire.
		 *\return		\p false si une erreur quelconque est arrivée.
		 */
		C3D_API bool doWrite( SkeletonAnimationObject const & p_obj )override;
	};
	/*!
	\author		Sylvain DOREMUS
	\version	0.8.0
	\date		26/01/2016
	\~english
	\brief		SkeletonAnimationObject binary loader.
	\~english
	\brief		Loader binaire de SkeletonAnimationObject.
	*/
	template<>
	class BinaryParser< SkeletonAnimationObject >
		: public BinaryParserBase< SkeletonAnimationObject >
	{
	private:
		/**
		 *\~english
		 *\brief		Function used to retrieve specific data from the chunk.
		 *\param[out]	p_obj	The object to read.
		 *\param[in]	p_chunk	The chunk containing data.
		 *\return		\p false if any error occured.
		 *\~french
		 *\brief		Fonction utilisée afin de récupérer des données spécifiques à partir d'un chunk.
		 *\param[out]	p_obj	L'objet à lire.
		 *\param[in]	p_chunk	Le chunk contenant les données.
		 *\return		\p false si une erreur quelconque est arrivée.
		 */
		C3D_API bool doParse( SkeletonAnimationObject & p_obj )override;
	};
}

#endif