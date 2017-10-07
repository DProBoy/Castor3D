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
#ifndef ___CU_FIXED_SIZE_MEMORY_DATA_H___
#define ___CU_FIXED_SIZE_MEMORY_DATA_H___

#include "MemoryDataTyper.hpp"

#include <cstddef>

namespace castor
{
	/*!
	\author		Sylvain DOREMUS
	\version	0.8.0
	\date		08/01/2016
	\~english
	\brief		Memory allocation policy.
	\remarks	Fixed size, basic memory leaks check.
				Holds the memory buffers, free chunks and currently allocated objects count.
	\~french
	\brief		Politique d'allocation de mémoire.
	\remarks	Taille fixe, check basique des fuites mémoire.
				Contient le tampon mémoire, les chunks libres, et le nombre d'objets actuellement alloués.
	*/
	template< typename Object, typename MemoryAllocator >
	class FixedSizeMemoryData
	{
		using Namer = MemoryDataNamer< MemoryDataType::eFixed >;

	protected:
		/**
		 *\~english
		 *\brief		Initialises the pool with given objects count.
		 *\param[in]	p_count	The object's count.
		 *\~french
		 *\brief		Initialise le pool avec le nombre d'objets donné.
		 *\param[in]	p_count	Le compte des objets.
		 */
		void initialise( size_t p_count )noexcept
		{
			m_total = p_count;
			m_buffer = MemoryAllocator::allocate( m_total * sizeof( Object ) );
			m_free = new Object * [m_total];
			m_freeIndex = m_free;
			m_bufferEnd = m_buffer;

			for ( size_t i = 0; i < m_total; ++i )
			{
				*m_freeIndex++ = reinterpret_cast< Object * >( m_bufferEnd );
				m_bufferEnd += sizeof( Object );
			}

			m_freeEnd = m_freeIndex;
		}
		/**
		 *\~english
		 *\brief		Cleans the pool up, reports memory leaks.
		 *\~french
		 *\brief		Nettoie le pool, rapporte les fuites de mémoire.
		 */
		void cleanup()noexcept
		{
			if ( m_freeIndex != m_freeEnd )
			{
				reportError< PoolErrorType::eCommonMemoryLeaksDetected >( Namer::Name, size_t( ( m_freeEnd - m_freeIndex ) * sizeof( Object ) ) );
			}

			delete [] m_free;
			MemoryAllocator::deallocate( m_buffer );
			m_free = nullptr;
			m_buffer = nullptr;
			m_freeIndex = m_free;
			m_bufferEnd = m_buffer;
		}
		/**
		 *\~english
		 *\brief		Gives the address an available chunk.
		 *\return		nullptr if no memory available, the memory address if not.
		 *\~french
		 *\brief		donne un chunk mémoire disponible.
		 *\return		nullptr s'il n'y a plus de place disponible, l'adresse mémoire sinon.
		 */
		Object * allocate()noexcept
		{
			if ( m_freeIndex == m_free )
			{
				reportError< PoolErrorType::eCommonOutOfMemory >( Namer::Name );
				return nullptr;
			}

			return *--m_freeIndex;
		}
		/**
		 *\~english
		 *\brief		Frees the given memory.
		 *\remarks		Checks if the given address comes from the pool.
		 *\param[in]	p_space	The memory to free.
		 *\return		nullptr if no memory available, the memory address if not.
		 *\~french
		 *\brief		Libère la mémoire donnée.
		 *\remarks		Vérifie si la mémoire fait bien partie du pool.
		 *\param[in]	p_space	La mémoire à libérer.
		 *\return		true si la mémoire faisait partie du pool.
		 */
		bool deallocate( void * p_space )noexcept
		{
			if ( p_space )
			{
				if ( m_freeIndex == m_freeEnd )
				{
					reportError< PoolErrorType::eCommonPoolIsFull >( Namer::Name, ( void * )p_space );
					return false;
				}

				if ( ptrdiff_t( p_space ) < ptrdiff_t( m_buffer ) || ptrdiff_t( p_space ) >= ptrdiff_t( m_bufferEnd ) )
				{
					reportError< PoolErrorType::eCommonNotFromRange >( Namer::Name, ( void * )p_space );
					return false;
				}

				*m_freeIndex++ = reinterpret_cast< Object * >( p_space );
				return true;
			}
			else
			{
				return true;
			}
		}

	private:
		//!\~english The buffer.	\~french Le tampon.
		uint8_t * m_buffer = nullptr;
		//!\~english Pointer to the buffer's end.	\~french Pointeur sur la fin du tampon.
		uint8_t * m_bufferEnd = nullptr;
		//!\~english The free chunks.	\~french Les chunks libres.
		Object ** m_free = nullptr;
		//!\~english The free chunks' end.	\~french La fin des chunks libres.
		Object ** m_freeEnd = nullptr;
		//!\~english The las allocated chunk.	\~french Le dernier chunk alloué.
		Object ** m_freeIndex = nullptr;
		//!\~english The total pool capacity.	\~french Le nombre total possible d'éléments.
		size_t m_total = 0;
	};
}

#endif