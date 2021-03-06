/*
See LICENSE file in root folder
*/
#ifndef ___CastorUtils_RangedValue_HPP___
#define ___CastorUtils_RangedValue_HPP___
#pragma once

#include "Range.hpp"

namespace castor
{
	/*!
	\author		Sylvain DOREMUS
	\date		16/02/2017
	\~english
	\brief		A value inside a range.
	\remarks	The value can't get out of its range.
	\~french
	\brief		Représente une valeur dans un intervalle.
	\remarks	La valeur ne peut pas sortir de son intervalle.
	*/
	template< typename T >
	class RangedValue
	{
	public:
		/**
		 *\~english
		 *\brief		Constructor.
		 *\param[in]	p_value	The value.
		 *\param[in]	p_range	The range.
		 *\~french
		 *\brief		Constructeur.
		 *\param[in]	p_value	La valeur.
		 *\param[in]	p_range	Son intervalle.
		 */
		inline RangedValue( T const & p_value
			, Range< T > const & p_range )noexcept
			: m_range{ p_range }
			, m_value{ m_range.clamp( p_value ) }
		{
		}
		/**
		 *\~english
		 *\brief		Assignment from a value.
		 *\param[in]	p_value	The value.
		 *\~french
		 *\brief		Affectation depuis une valeur.
		 *\param[in]	p_value	La valeur.
		 */
		inline RangedValue & operator=( T const & p_value )noexcept
		{
			m_value = m_range.clamp( p_value );
			return *this;
		}
		/**
		 *\~english
		 *\brief		Redefines the range.
		 *\param[in]	p_range	The range.
		 *\~french
		 *\brief		Redéfinit l'intervalle.
		 *\param[in]	p_range	Le nouvel intervalle.
		 */
		void updateRange( Range< T > const & p_range )noexcept
		{
			m_range = p_range;
			m_range.clamp( m_value );
		}
		/**
		 *\~english
		 *\return		0.0 if \p m_value <= \p m_range.m_min.\n
		 *				1.0 if \p m_value >= \p m_range.m_max.\n
		 *				A value from 0.0 to 1.0, depending on how close the value is
		 *				from \p m_range.m_min or from \p m_range.m_max.
		 *\~french
		 *\return		0.0 si \p m_value <= \p m_range.m_min.\n
		 *				1.0 si \p m_value >= \p m_range.m_max.\n
		 *				Une valeur allant de 0.0 à 1.0, selon que la valeur est plus proche
		 *				de \p m_range.m_min ou de \p m_range.m_max.
		 */
		float percent()const noexcept
		{
			return m_range.percent( m_value );
		}
		/**
		 *\~english
		 *\return		The value.
		 *\~french
		 *\return		La valeur.
		 */
		inline T const & value()const noexcept
		{
			return m_value;
		}
		/**
		 *\~english
		 *\return		The range.
		 *\~french
		 *\return		L'intervalle.
		 */
		inline Range< T > const & range()const noexcept
		{
			return m_range;
		}
		/**
		 *\~english
		 *\name Arithmetic operators.
		 *\~french
		 *\name Opérateurs arithmétiques.
		 */
		/**@{*/
		inline RangedValue< T > & operator+=( RangedValue< T > const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value + p_rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator-=( RangedValue< T > const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value - p_rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator*=( RangedValue< T > const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value * p_rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator/=( RangedValue< T > const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value / p_rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator+=( T const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value + p_rhs );
			return *this;
		}

		inline RangedValue< T > & operator-=( T const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value - p_rhs );
			return *this;
		}

		inline RangedValue< T > & operator*=( T const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value * p_rhs );
			return *this;
		}

		inline RangedValue< T > & operator/=( T const & p_rhs )noexcept
		{
			m_value = m_range.clamp( m_value / p_rhs );
			return *this;
		}
		/**@}*/

	private:
		//!\~english	The range.
		//!\~french		L'intervalle.
		Range< T > m_range;
		//!\~english	The value.
		//!\~french		La valeur.
		T m_value;
	};
	/**
	 *\~english
	 *\brief		Helper function to create a ranged value.
	 *\param[in]	p_value			The value.
	 *\param[in]	p_min, p_max	The range.
	 *\return		The created ranged value.
	 *\~french
	 *\brief		Fonction d'aide à la construction d'une valeur dans un intervalle.
	 *\param[in]	p_value			La valeur.
	 *\param[in]	p_min, p_max	Les bornes de l'intervalle.
	 *\return		L'objet créé.
	 */
	template< typename T >
	inline RangedValue< T > makeRangedValue( T const & p_value
		, T const & p_min
		, T const & p_max )noexcept
	{
		return RangedValue< T >{ p_value, makeRange( p_min, p_max ) };
	}
	/**
	 *\~english
	 *\name Logic operators.
	 *\~french
	 *\name Opérateurs logiques.
	 */
	/**@{*/
	namespace details
	{
		template< typename T >
		struct Equal
		{
			inline bool operator()( RangedValue< T > const & p_lhs
				, RangedValue< T > const & p_rhs )const noexcept
			{
				static constexpr auto eps = std::numeric_limits< T >::epsilon();
				return std::abs( p_lhs.value() - p_rhs.value() ) < eps;
			}
		};

		template<>
		struct Equal< uint32_t >
		{
			inline bool operator()( RangedValue< uint32_t > const & p_lhs
				, RangedValue< uint32_t > const & p_rhs )const noexcept
			{
				return p_lhs.value() == p_rhs.value();
			}
		};
	}

	template< typename T >
	inline bool operator==( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		static constexpr auto eps = std::numeric_limits< T >::epsilon();
		return std::abs( p_lhs.value() - p_rhs ) < eps;
	}

	template< typename T >
	inline bool operator!=( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		return !( p_lhs == p_rhs );
	}

	template< typename T >
	inline bool operator>( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		return p_lhs.value() > p_rhs;
	}

	template< typename T >
	inline bool operator<( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		return p_lhs.value() < p_rhs;
	}

	template< typename T >
	inline bool operator>=( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}

	template< typename T >
	inline bool operator<=( RangedValue< T > const & p_lhs
		, T const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}

	template< typename T >
	inline bool operator==( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		static constexpr auto eps = std::numeric_limits< T >::epsilon();
		return std::abs( p_lhs - p_rhs.value() ) < eps;
	}

	template< typename T >
	inline bool operator!=( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs == p_rhs );
	}

	template< typename T >
	inline bool operator>( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs > p_rhs.value();
	}

	template< typename T >
	inline bool operator<( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs < p_rhs.value();
	}

	template< typename T >
	inline bool operator>=( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}

	template< typename T >
	inline bool operator<=( T const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}

	template< typename T >
	inline bool operator==( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		static const details::Equal< T > equals;
		return equals( p_lhs, p_rhs );
	}

	template< typename T >
	inline bool operator!=( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs == p_rhs );
	}

	template< typename T >
	inline bool operator>( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() > p_rhs.value();
	}

	template< typename T >
	inline bool operator<( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() < p_rhs.value();
	}

	template< typename T >
	inline bool operator>=( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}

	template< typename T >
	inline bool operator<=( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return !( p_lhs < p_rhs );
	}
	/**@}*/
	/**
	 *\~english
	 *\name Arithmetic operators.
	 *\~french
	 *\name Opérateurs arithmétiques.
	 */
	/**@{*/
	template< typename T >
	inline T operator+( T const & p_lhs, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs + p_rhs.value();
	}

	template< typename T >
	inline T operator-( T const & p_lhs, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs - p_rhs.value();
	}

	template< typename T >
	inline T operator*( T const & p_lhs, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs * p_rhs.value();
	}

	template< typename T >
	inline T operator/( T const & p_lhs, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs / p_rhs.value();
	}

	template< typename T >
	inline T operator+( RangedValue< T > const & p_lhs, T const & p_rhs )noexcept
	{
		return p_lhs.value() + p_rhs;
	}

	template< typename T >
	inline T operator-( RangedValue< T > const & p_lhs, T const & p_rhs )noexcept
	{
		return p_lhs.value() - p_rhs;
	}

	template< typename T >
	inline T operator*( RangedValue< T > const & p_lhs, T const & p_rhs )noexcept
	{
		return p_lhs.value() * p_rhs;
	}

	template< typename T >
	inline T operator/( RangedValue< T > const & p_lhs, T const & p_rhs )noexcept
	{
		return p_lhs.value() / p_rhs;
	}

	template< typename T >
	inline T operator+( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() + p_rhs.value();
	}

	template< typename T >
	inline T operator-( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() - p_rhs.value();
	}

	template< typename T >
	inline T operator*( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() * p_rhs.value();
	}

	template< typename T >
	inline T operator/( RangedValue< T > const & p_lhs
		, RangedValue< T > const & p_rhs )noexcept
	{
		return p_lhs.value() / p_rhs.value();
	}
	/**@}*/
}

#endif
