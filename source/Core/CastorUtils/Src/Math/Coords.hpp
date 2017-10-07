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
#ifndef ___CASTOR_COORDS_H___
#define ___CASTOR_COORDS_H___

#include "Data/TextLoader.hpp"
#include "Data/TextWriter.hpp"

#include <cmath>
#include <iostream>

namespace castor
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\~english
	\brief		Templated static dimensions point representation
	\remark		Can hold any type which has a defined Policy
	\~french
	\brief		Classe de points à dimensions statiques
	\remark		Peut recevoir les types de données qui ont une Policy créée
	*/
	template< typename T, uint32_t Count >
	class Coords
	{
	public:
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 03/01/2011
		\~english
		\brief Coords< T, Count > loader
		\~french
		\brief Loader de Coords< T, Count >
		*/
		class TextLoader
			: public castor::TextLoader< Coords< T, Count > >
		{
		public:
			/**
			 *\~english
			 *\brief		Constructor
			 *\~french
			 *\brief		Constructeur
			 */
			inline TextLoader();
			/**
			 *\~english
			 *\brief		Loads a Coords< T, Count > object from a text file
			 *\param[out]	p_object	The Coords< T, Count > object to read
			 *\param[in]	p_file		The file
			 *\return		\p true if ok
			 *\~french
			 *\brief		Charge un objet Coords< T, Count > à partir d'un fichier texte
			 *\param[out]	p_object	L'objet Coords< T, Count >
			 *\param[in]	p_file		Le fichier
			 *\return		\p true si ok
			 */
			inline bool operator()( Coords< T, Count > & p_object, TextFile & p_file )override;
		};
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 03/01/2011
		\~english
		\brief Coords< T, Count > Writer
		\~french
		\brief Writer de Coords< T, Count >
		*/
		class TextWriter
			: public castor::TextWriter< Coords< T, Count > >
		{
		public:
			/**
			 *\~english
			 *\brief		Constructor
			 *\~french
			 *\brief		Constructeur
			 */
			inline TextWriter( String const & p_tabs );
			/**
			 *\~english
			 *\brief		Writes a Point2f object into a text file
			 *\param[in]	p_object	The Point2f object to write
			 *\param[out]	p_file		The file
			 *\return		\p true if ok
			 *\~french
			 *\brief		Ecrit un objet Point2f dans un fichier texte
			 *\param[in]	p_object	L'objet Point2f
			 *\param[out]	p_file		Le fichier
			 *\return		\p true si ok
			 */
			inline bool operator()( Coords< T, Count > const & p_object, TextFile & p_file )override;
		};

	private:
		template< typename U, uint32_t _Count > friend class Coords;

		typedef T value_type;
		typedef value_type & reference;
		typedef value_type * pointer;
		typedef value_type const & const_reference;
		typedef value_type const * const_pointer;
		typedef Coords< value_type, Count > & point_reference;
		typedef Coords< value_type, Count > * point_pointer;
		typedef Coords< value_type, Count > const & const_point_reference;
		typedef Coords< value_type, Count > const * const_point_pointer;
		static const std::size_t binary_size = sizeof( T ) * Count;

	public:
		//!\~english Typedef over a pointer on data	\~french Typedef d'un pointeur sur les données
		typedef value_type * iterator;
		//!\~english Typedef over a constant pointer on data	\~french Typedef d'un pointeur constant sur les données
		typedef value_type const * const_iterator;

	public:
		/**
		 *\~english
		 *\brief		Constructor
		 *\~french
		 *\brief		Constructeur
		 */
		Coords();
		/**
		 *\~english
		 *\brief		Copy Constructor
		 *\param[in]	p_point	The Coords object to copy
		 *\~french
		 *\brief		Constructeur par copie
		 *\param[in]	p_point	L'objet Coords à copier
		 */
		Coords( Coords< T, Count > const & p_point );
		/**
		 *\~english
		 *\brief		Move assignment Constructor
		 *\param[in]	p_point	The Coords object to move
		 *\~french
		 *\brief		Constructeur par déplacement
		 *\param[in]	p_point	L'objet Coords à déplacer
		 */
		Coords( Coords< T, Count > && p_point );
		/**
		 *\~english
		 *\brief		Constructor
		 *\param[in]	p_pValues	The data buffer
		 *\~french
		 *\brief		Constructeur
		 *\param[in]	p_pValues	Le buffer de données
		 */
		explicit Coords( T * p_pValues );
		/**
		 *\~english
		 *\brief		Destructor
		 *\remarks		Not virtual but it should be ok to derive Coords because destructor is dummy
		 *\~french
		 *\brief		Destructeur
		 *\remarks		Non virtuel mais il devrait être possible de dériver Coords car le destructeur ne fait rien
		 */
		~Coords();
		/**
		 *\~english
		 *\brief		Assignment operator
		 *\param[in]	p_pValues	The values to affect
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation
		 *\param[in]	p_pValues	Les valeurs à affecter
		 *\return		Une référence sur cet objet Coords
		 */
		Coords< T, Count > & operator=( T * p_pValues );
		/**
		 *\~english
		 *\brief		Copy assignment operator
		 *\param[in]	p_pt	The Coords object to copy
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par copie
		 *\param[in]	p_pt	L'objet Coords à copier
		 *\return		Une référence sur cet objet Coords
		 */
		Coords< T, Count > & operator=( Coords< T, Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Move assignment operator
		 *\param[in]	p_pt	The Coords object to move
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par déplacement
		 *\param[in]	p_pt	L'objet Coords à déplacer
		 *\return		Une référence sur cet objet Coords
		 */
		Coords< T, Count > & operator=( Coords< T, Count > && p_pt );
		/**
		 *\~english
		 *\brief		Copy assignment operator
		 *\param[in]	p_pt	The Point object to copy
		 *\return		A reference to this Point object
		 *\~french
		 *\brief		Opérateur d'affectation par copie
		 *\param[in]	p_pt	L'objet Point à copier
		 *\return		Une référence sur cet objet Point
		 */
		Coords< T, Count > & operator=( Point< T, Count > const & p_pt );
		/**
		 *\~english
		 *\brief		addition assignment operator
		 *\param[in]	p_pt	The Coords object to add
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par addition
		 *\param[in]	p_pt	L'objet Coords à ajouter
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator+=( Coords< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Substraction assignment operator
		 *\param[in]	p_pt	The Coords object to subtract
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par soustraction
		 *\param[in]	p_pt	L'objet Coords à soustraire
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator-=( Coords< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_pt	The Coords object to multiply
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par multiplication
		 *\param[in]	p_pt	L'objet Coords à multiplier
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator*=( Coords< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Division assignment operator
		 *\param[in]	p_pt	The Coords object to divide
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par division
		 *\param[in]	p_pt	L'objet Coords à diviser
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator/=( Coords< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		addition assignment operator
		 *\param[in]	p_pt	The Coords object to add
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par addition
		 *\param[in]	p_pt	L'objet Coords à ajouter
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator+=( Point< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Substraction assignment operator
		 *\param[in]	p_pt	The Coords object to subtract
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par soustraction
		 *\param[in]	p_pt	L'objet Coords à soustraire
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator-=( Point< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_pt	The Coords object to multiply
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par multiplication
		 *\param[in]	p_pt	L'objet Coords à multiplier
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator*=( Point< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		Division assignment operator
		 *\param[in]	p_pt	The Coords object to divide
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par division
		 *\param[in]	p_pt	L'objet Coords à diviser
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U, uint32_t _Count > Coords< T, Count > & operator/=( Point< U, _Count > const & p_pt );
		/**
		 *\~english
		 *\brief		addition assignment operator
		 *\param[in]	p_coords	The values to add
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par addition
		 *\param[in]	p_coords	Les valeurs à additionner
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U >	Coords< T, Count > & operator+=( U const * p_coords );
		/**
		 *\~english
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coords	The values to subtract
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par soustraction
		 *\param[in]	p_coords	Les valeurs à soustraire
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U >	Coords< T, Count > & operator-=( U const * p_coords );
		/**
		 *\~english
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coords	The values to multiply
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par multiplication
		 *\param[in]	p_coords	Les valeurs à multiplier
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U >	Coords< T, Count > & operator*=( U const * p_coords );
		/**
		 *\~english
		 *\brief		Division assignment operator
		 *\param[in]	p_coords	The values to divide
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par division
		 *\param[in]	p_coords	Les valeurs à diviser
		 *\return		Une référence sur cet objet Coords
		 */
		template< typename U >	Coords< T, Count > & operator/=( U const * p_coords );
		/**
		 *\~english
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coord	The value to multiply
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par multiplication
		 *\param[in]	p_coord	La valeur à multiplier
		 *\return		Une référence sur cet objet Coords
		 */
		Coords< T, Count > & operator*=( T const & p_coord );
		/**
		 *\~english
		 *\brief		Division assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this Coords object
		 *\~french
		 *\brief		Opérateur d'affectation par division
		 *\param[in]	p_coord	La valeur à diviser
		 *\return		Une référence sur cet objet Coords
		 */
		Coords< T, Count > & operator/=( T const & p_coord );
		/**
		 *\~english
		 *\brief		Swaps this pont data with the parameter ones
		 *\param[in]	p_pt	The point to swap
		 *\~french
		 *\brief		Echange les données de c epoint avec celles du point donné
		 *\param[in]	p_pt	Le point à échanger
		 */
		void swap( Coords< T, Count > & p_pt );
		/**
		 *\~english
		 *\brief		Inverts data in the point
		 *\remarks		The first becomes last and so on
		 *\~french
		 *\brief		Inverse les données dans ce point
		 *\remarks		Les premiers deviennent les derniers
		 */
		void flip();
		/**
		 *\~english
		 *\brief		Retrieves the total size of the point
		 *\return		count() * elemSize()
		 *\~french
		 *\brief		Récupère la taille totale du point
		 *\return		count() * elemSize()
		 */
		inline uint32_t size()const
		{
			return binary_size;
		}
		/**
		 *\~english
		 *\brief			Retrieves data from the point and put it into an array
		 *\param[in,out]	p_pResult	Receives the point data, needs to be allocated by the caller
		 *\~french
		 *\brief			Récupère les données et les place dans un tableau
		 *\param[in,out]	p_pResult	Reçoit les données, doit être alloué par l'appelant
		 */
		void toValues( T * p_pResult )const;
		/**
		 *\~english
		 *\brief		Retrieves the number of coordinates
		 *\return		The number of coordinates
		 *\~french
		 *\brief		Récupère le nombre de coordonnées
		 *\return		Le nombre de coordonnées
		 */
		inline uint32_t count()const
		{
			return Count;
		}
		/**
		 *\~english
		 *\brief		Retrieves the coordinate type size
		 *\return		The data type size
		 *\~french
		 *\brief		Récupère la taille du type données
		 *\return		La taille du type de données
		 */
		inline std::size_t elemSize()const
		{
			return sizeof( T );
		}
		/**
		 *\~english
		 *\brief		Retrieves the data at given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\return		A constant reference on data at wanted index
		 *\~french
		 *\brief		Récupère la donnée à l'index donné
		 *\remarks		Aucun check n'est fait, s'il y a une erreur d'index, attendez-vous à un crash
		 *\return		Une référence constante sur la donnée à l'index voulu
		 */
		inline T const & operator[]( uint32_t p_pos )const
		{
			return m_coords[p_pos];
		}
		/**
		 *\~english
		 *\brief		Retrieves the data at given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\return		A reference on data at wanted index
		 *\~french
		 *\brief		Récupère la donnée à l'index donné
		 *\remarks		Aucun check n'est fait, s'il y a une erreur d'index, attendez-vous à un crash
		 *\return		Une référence sur la donnée à l'index voulu
		 */
		inline T & operator[]( uint32_t p_pos )
		{
			return m_coords[p_pos];
		}
		/**
		 *\~english
		 *\brief		Retrieves the data at given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\return		A constant reference on data at wanted index
		 *\~french
		 *\brief		Récupère la donnée à l'index donné
		 *\remarks		Cette fonction vérifie l'index et lance une exception s'il est hors bornes
		 *\return		Une référence constante sur la donnée à l'index voulu
		 */
		T const & at( uint32_t p_pos )const;
		/**
		 *\~english
		 *\brief		Retrieves the data at given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\return		A reference on data at wanted index
		 *\~french
		 *\brief		Récupère la donnée à l'index donné
		 *\remarks		Cette fonction vérifie l'index et lance une exception s'il est hors bornes
		 *\return		Une référence sur la donnée à l'index voulu
		 */
		T & at( uint32_t p_pos );
		/**
		 *\~english
		 *\brief		Retrieves a non constant pointer on datas
		 *\return		The pointer
		 *\~french
		 *\brief		Récupère un pointeur non constant sur les données
		 *\return		Le pointeur
		 */
		inline T * ptr()
		{
			return m_coords;
		}
		/**
		 *\~english
		 *\brief		Retrieves a constant pointer on datas
		 *\return		The pointer
		 *\~french
		 *\brief		Récupère un pointeur constant sur les données
		 *\return		Le pointeur
		 */
		inline T const * constPtr()const
		{
			return m_coords;
		}
		/**
		 *\~english
		 *\brief		Retrieves an iterator to the first element
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur sur le premier élément
		 *\return		L'itérateur
		 */
		inline iterator begin()
		{
			return m_coords;
		}
		/**
		 *\~english
		 *\brief		Retrieves a constant iterator to the first element
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur constant sur le premier élément
		 *\return		L'itérateur
		 */
		inline const_iterator begin()const
		{
			return m_coords;
		}
		/**
		 *\~english
		 *\brief		Retrieves an iterator to the last element
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur sur le dernier élément
		 *\return		L'itérateur
		 */
		inline iterator end()
		{
			return m_coords + Count;
		}
		/**
		 *\~english
		 *\brief		Retrieves a constant iterator to the last element
		 *\return		The iterator
		 *\~french
		 *\brief		Récupère un itérateur constant sur le dernier élément
		 *\return		L'itérateur
		 */
		inline const_iterator end()const
		{
			return m_coords + Count;
		}

	private:
		T * m_coords;
	};
	/**
	 *\~english
	 *\brief		Equality operator
	 *\param[in]	lhs, rhs	The points to compare
	 *\return		\p true if points have same dimensions and same values
	 *\~french
	 *\brief		Opérateur d'égalité
	 *\param[in]	lhs, rhs	Les points à comparer
	 *\return		\p true si les points ont les mêmes dimensions et les mêmes valeurs
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> bool operator==( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		Difference operator
	 *\param[in]	lhs, rhs	The points to compare
	 *\return		\p true if points have different dimensions or at least one different value
	 *\~french
	 *\brief		Opérateur d'inégalité
	 *\param[in]	lhs, rhs	Les points à comparer
	 *\return		\p true si les points ont des dimensions différentes ou au moins une valeur différente
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> bool operator!=( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		addition operator
	 *\param[in]	lhs, rhs	The points to add
	 *\return		The addition result
	 *\~french
	 *\brief		Opérateur d'addition
	 *\param[in]	lhs, rhs	Les points à additionner
	 *\return		Le résultat de l'addition
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> Point< T, Count > operator+( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		Substraction operator
	 *\param[in]	lhs, rhs	The points to subtract
	 *\return		The substraction result
	 *\~french
	 *\brief		Opérateur de soustraction
	 *\param[in]	lhs, rhs	Les points à soustraire
	 *\return		Le résultat de la soustraction
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> Point< T, Count > operator-( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		Multiplication operator
	 *\param[in]	lhs, rhs	The points to multiply
	 *\return		The multiplication result
	 *\~french
	 *\brief		Opérateur de multiplication
	 *\param[in]	lhs, rhs	Les points à multiplier
	 *\return		Le résultat de la multiplication
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> Point< T, Count > operator*( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		Division operator
	 *\param[in]	lhs, rhs	The points to divide
	 *\return		The division result
	 *\~french
	 *\brief		Opérateur de division
	 *\param[in]	lhs, rhs	Les points à diviser
	 *\return		Le résultat de la division
	 */
	template< typename T, uint32_t Count, typename U, uint32_t _Count	> Point< T, Count > operator/( Coords< T, Count > const & lhs, Coords< U, _Count > const & rhs );
	/**
	 *\~english
	 *\brief		addition operator
	 *\param[in]	p_pt		The point
	 *\param[in]	p_coords	The values to add
	 *\return		The addition result
	 *\~french
	 *\brief		Opérateur d'addition
	 *\param[in]	p_pt		Le point
	 *\param[in]	p_coords	Les valeurs à additionner
	 *\return		Le résultat de l'addition
	 */
	template< typename T, uint32_t Count, typename U > Point< T, Count > operator+( Coords< T, Count > const & p_pt, U const * p_coords );
	/**
	 *\~english
	 *\brief		Substraction operator
	 *\param[in]	p_pt		The point
	 *\param[in]	p_coords	The values to subtract
	 *\return		The substraction result
	 *\~french
	 *\brief		Opérateur de soustraction
	 *\param[in]	p_pt		Le point
	 *\param[in]	p_coords	Les valeurs à soustraire
	 *\return		Le résultat de la soustraction
	 */
	template< typename T, uint32_t Count, typename U > Point< T, Count > operator-( Coords< T, Count > const & p_pt, U const * p_coords );
	/**
	 *\~english
	 *\brief		Multiplication operator
	 *\param[in]	p_pt		The point
	 *\param[in]	p_coords	The values to multiply
	 *\return		The multiplication result
	 *\~french
	 *\brief		Opérateur de multiplication
	 *\param[in]	p_pt		Le point
	 *\param[in]	p_coords	Les valeurs à mulitplier
	 *\return		Le résultat de la multiplication
	 */
	template< typename T, uint32_t Count, typename U > Point< T, Count > operator*( Coords< T, Count > const & p_pt, U const * p_coords );
	/**
	 *\~english
	 *\brief		Division operator
	 *\param[in]	p_pt		The point to divide
	 *\param[in]	p_coords	The values
	 *\return		The division result
	 *\~french
	 *\brief		Opérateur de division
	 *\param[in]	p_pt		Le point à diviser
	 *\param[in]	p_coords	Les valeurs
	 *\return		Le résultat de la division
	 */
	template< typename T, uint32_t Count, typename U > Point< T, Count > operator/( Coords< T, Count > const & p_pt, U const * p_coords );
	/**
	 *\~english
	 *\brief		Multiplication operator
	 *\param[in]	p_pt	The point to multiply
	 *\param[in]	p_coord	The value
	 *\return		The multiplication result
	 *\~french
	 *\brief		Opérateur de multiplication
	 *\param[in]	p_pt	Le point à multiplier
	 *\param[in]	p_coord	La valeur
	 *\return		Le résultat de la multiplication
	 */
	template< typename T, uint32_t Count > Point< T, Count > operator*( Coords< T, Count > const & p_pt, T const & p_coord );
	/**
	 *\~english
	 *\brief		Division operator
	 *\param[in]	p_pt	The point to divide
	 *\param[in]	p_coord	The value
	 *\return		The division result
	 *\~french
	 *\brief		Opérateur de division
	 *\param[in]	p_pt	Le point à diviser
	 *\param[in]	p_coord	La valeur
	 *\return		Le résultat de la division
	 */
	template< typename T, uint32_t Count > Point< T, Count > operator/( Coords< T, Count > const & p_pt, T const & p_coord );
	/*!
	\author 	Sylvain DOREMUS
	\date 		10/07/2012
	\version	0.7.0
	\~english
	\brief		Point helper functions
	\~french
	\brief		Fonctions d'aide pour les Point
	*/
	namespace point
	{
		/**
		 *\~english
		 *\brief		Negates every value of a point's data
		 *\param[in]	p_point	The point
		 *\~french
		 *\brief		Négative chaque donnée du point donné
		 *\param[in]	p_point	Le point
		 */
		template< typename T, uint32_t Count > static void negate( Coords< T, Count > & p_point );
		/**
		 *\~english
		 *\brief		Normalises the point
		 *\remarks		A point is normalised when it's length equals 1.0
						<br />So each data is divided by the point's length
		 *\param[in]	p_point	The point
		 *\~french
		 *\brief		Normalise le point
		 *\remarks		Un point est normalisé quand sa longueur vaut 1.0
						<br />Chaque donnée est donc divisée par la longueur du point
		 *\param[in]	p_point	Le point
						*/
		template< typename T, uint32_t Count > static void normalise( Coords< T, Count > & p_point );
		/**
		 *\~english
		 *\brief		Computes the scalar product of 2 points
		 *\param[in]	lhs, rhs	The points
		 *\return		The scalar product
		 *\~french
		 *\brief		Calcule le produit scalaire entre 2 points
		 *\param[in]	lhs, rhs	Les points
		 *\return		Le produit scalaire
		 */
		template< typename T, uint32_t Count > static T dot( Coords< T, Count > const & lhs, Coords< T, Count > const & rhs );
		/**
		 *\~english
		 *\brief		Cross product operator
		 *\param[in]	lhs, rhs	The points to multiply
		 *\return		The cross product
		 *\~french
		 *\brief		Opérateur de produit vectoriel
		 *\param[in]	lhs, rhs	Les points à multiplier
		 *\return		Le résultat du produit vectoriel
		 */
		template< typename T, typename U > static Point< T, 3 > cross( Coords< T, 3 > const & lhs, Coords< U, 3 > const & rhs );
		/**
		 *\~english
		 *\brief		Computes the trigonometric cosine of the angle between 2 points
		 *\param[in]	lhs, rhs	The points
		 *\return		The cosine
		 *\~french
		 *\brief		Calcule le cosinus trigonométrique de l'angle entre 2 points
		 *\param[in]	lhs, rhs	Les points
		 *\return		Le cosinus
		 */
		template< typename T, uint32_t Count > static double cosTheta( Coords< T, Count > const & lhs, Coords< T, Count > const & rhs );
		/**
		 *\~english
		 *\brief		Computes the squared Euclidian length of the vector
		 *\param[in]	p_point	The point
		 *\return		The squared Euclidian distance
		 *\~french
		 *\brief		Calcule le carré de la longueur Euclidienne du vecteur
		 *\param[in]	p_point	Le point
		 *\return		Le carré de la norme Euclidienne
		 */
		template< typename T, uint32_t Count > static double lengthSquared( Coords< T, Count > const & p_point );
		/**
		 *\~english
		 *\brief		Computes the Euclidian length of the vector
		 *\param[in]	p_point	The point
		 *\return		The Euclidian distance
		 *\~french
		 *\brief		Calcule la longueur Euclidienne du vecteur
		 *\param[in]	p_point	Le point
		 *\return		La norme Euclidienne
		 */
		template< typename T, uint32_t Count > static double length( Coords< T, Count > const & p_point );
		/**
		 *\~english
		 *\brief		Computes the Manhattan length of the vector
		 *\param[in]	p_point	The point
		 *\return		The Manhattan distance
		 *\~french
		 *\brief		Calcule la longueur de Manhattan du vecteur
		 *\param[in]	p_point	Le point
		 *\return		La distance de Manhattan
		 */
		template< typename T, uint32_t Count > static double lengthManhattan( Coords< T, Count > const & p_point );
		/**
		 *\~english
		 *\brief		Computes the Minkowski length of the vector
		 *\param[in]	p_point	The point
		 *\param[in]	p_dOrder	The Minkowski order
		 *\return		The Minkowski distance
		 *\~french
		 *\brief		Calcule la longueur de Minkowski du vecteur
		 *\param[in]	p_point	Le point
		 *\param[in]	p_dOrder	L'ordre Minkownski
		 *\return		La distance de Minkowski
		 */
		template< typename T, uint32_t Count > static double lengthMinkowski( Coords< T, Count > const & p_point, double p_dOrder );
		/**
		 *\~english
		 *\brief		Computes the Chebychev length of the vector
		 *\param[in]	p_point	The point
		 *\return		The Chebychev distance
		 *\~french
		 *\brief		Calcule la longueur de Chebychev du vecteur
		 *\param[in]	p_point	Le point
		 *\return		La distance de Chebychev
		 */
		template< typename T, uint32_t Count > static double lengthChebychev( Coords< T, Count > const & p_point );
		/**
		 *\~english
		 *\brief		Computes the squared Euclidian distance between two points.
		 *\param[in]	p_lhs, p_rhs	The points.
		 *\return		The squared Euclidian distance.
		 *\~french
		 *\brief		Calcule le carré de la distance Euclidienne entre deux points.
		 *\param[in]	p_lhs, p_rhs	Les points.
		 *\return		Le carré de la norme Euclidienne.
		 */
		template< typename T, uint32_t Count > static double distanceSquared( Coords< T, Count > const & p_lhs, Coords< T, Count > const & p_rhs );
		/**
		 *\~english
		 *\brief		Computes the Euclidian distance between two points.
		 *\param[in]	p_lhs, p_rhs	The points.
		 *\return		The Euclidian distance.
		 *\~french
		 *\brief		Calcule la norme Euclidienne entre deux points.
		 *\param[in]	p_lhs, p_rhs	Les points.
		 *\return		La norme Euclidienne.
		 */
		template< typename T, uint32_t Count > static double distance( Coords< T, Count > const & p_lhs, Coords< T, Count > const & p_rhs );
		/**
		 *\~english
		 *\brief		Computes the Manhattan distance between two points.
		 *\param[in]	p_lhs, p_rhs	The points.
		 *\return		The Manhattan distance.
		 *\~french
		 *\brief		Calcule la distance de Manhattan entre deux points.
		 *\param[in]	p_lhs, p_rhs	Les points.
		 *\return		La distance de Manhattan.
		 */
		template< typename T, uint32_t Count > static double distanceManhattan( Coords< T, Count > const & p_lhs, Coords< T, Count > const & p_rhs );
		/**
		 *\~english
		 *\brief		Computes the Minkowski distance between two points.
		 *\param[in]	p_lhs, p_rhs	The points.
		 *\param[in]	p_dOrder	The Minkowski order.
		 *\return		The Minkowski distance.
		 *\~french
		 *\brief		Calcule la distance de Minkowski entre deux points.
		 *\param[in]	p_lhs, p_rhs	Les points.
		 *\param[in]	p_dOrder	L'ordre Minkownski.
		 *\return		La distance de Minkowski.
		 */
		template< typename T, uint32_t Count > static double distanceMinkowski( Coords< T, Count > const & p_lhs, Coords< T, Count > const & p_rhs, double p_dOrder );
		/**
		 *\~english
		 *\brief		Computes the Chebychev distance between two points.
		 *\param[in]	p_lhs, p_rhs	The points.
		 *\return		The Chebychev distance.
		 *\~french
		 *\brief		Calcule la distance de Chebychev entre deux points.
		 *\param[in]	p_lhs, p_rhs	Les points.
		 *\return		La distance de Chebychev.
		 */
		template< typename T, uint32_t Count > static double distanceChebychev( Coords< T, Count > const & p_lhs, Coords< T, Count > const & p_rhs );
	}
}
/**
 *\~english
 *\brief			Stream operator
 *\param[in,out]	p_streamOut	The stream receiving point's data
 *\param[in]		p_pt		The input point
 *\return			A reference to the stream
 *\~french
 *\brief			Opérateur de flux
 *\param[in,out]	p_streamOut	Le flux qui reçoit les données du point
 *\param[in]		p_pt		Le point entré
 *\return			Une référence sur le flux
 */
template< typename T, uint32_t Count > castor::String & operator<<( castor::String & p_streamOut, castor::Coords< T, Count > const & p_pt );
/**
 *\~english
 *\brief			Stream operator
 *\param[in,out]	p_streamIn	The stream holding point's data
 *\param[in,out]	p_pt		The output point
 *\return			A reference to the stream
 *\~french
 *\brief			Opérateur de flux
 *\param[in,out]	p_streamIn	Le flux qui contient les données du point
 *\param[in,out]	p_pt		Le point sortie
 *\return			Une référence sur le flux
 */
template< typename T, uint32_t Count > castor::String & operator>>( castor::String & p_streamIn, castor::Coords< T, Count > & p_pt );
/**
 *\~english
 *\brief			Stream operator
 *\param[in,out]	p_streamOut	The stream receiving point's data
 *\param[in]		p_pt		The input point
 *\return			A reference to the stream
 *\~french
 *\brief			Opérateur de flux
 *\param[in,out]	p_streamOut	Le flux qui reçoit les données du point
 *\param[in]		p_pt		Le point entré
 *\return			Une référence sur le flux
 */
template< typename T, uint32_t Count, typename CharType > std::basic_ostream< CharType > & operator<<( std::basic_ostream< CharType > & p_streamOut, castor::Coords< T, Count > const & p_pt );
/**
 *\~english
 *\brief			Stream operator
 *\param[in,out]	p_streamIn	The stream holding point's data
 *\param[in,out]	p_pt		The output point
 *\return			A reference to the stream
 *\~french
 *\brief			Opérateur de flux
 *\param[in,out]	p_streamIn	Le flux qui contient les données du point
 *\param[in,out]	p_pt		Le point sortie
 *\return			Une référence sur le flux
 */
template< typename T, uint32_t Count, typename CharType > std::basic_istream< CharType > & operator>>( std::basic_istream< CharType > & p_streamIn, castor::Coords< T, Count > & p_pt );

#include "Coords.inl"

#endif