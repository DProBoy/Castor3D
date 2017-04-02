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
#ifndef ___C3D_BINARY_PARSER_H___
#define ___C3D_BINARY_PARSER_H___

#include "ChunkParser.hpp"

namespace Castor3D
{
	/*!
	\author 	Sylvain DOREMUS
	\version	0.7.0.0
	\date 		15/04/2013
	\~english
	\brief		Binary data chunk writer/reader base class
	\~french
	\brief		Classe de base de lecture/écriture d'un chunk de données binaires
	*/
	template< class TParsed >
	class BinaryParserBase
	{
	public:
		/**
		 *\~english
		 *\brief		From file reader function
		 *\param[out]	p_obj	The object to read
		 *\param[in]	p_file	The file containing the chunk
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Fonction de lecture à partir d'un fichier
		 *\param[out]	p_obj	L'objet à lire
		 *\param[in]	p_file	Le fichier qui contient le chunk
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		inline bool Parse( TParsed & p_obj, Castor::BinaryFile & p_file )
		{
			BinaryChunk l_header;
			bool l_return = l_header.Read( p_file );

			if ( l_header.GetChunkType() != ChunkType::eCmshFile )
			{
				Castor::Logger::LogError( cuT( "Not a valid CMSH file." ) );
				l_return = false;
			}

			if ( l_return )
			{
				l_return = DoParseHeader( l_header );
			}

			if ( l_return )
			{
				l_return = l_header.CheckAvailable( 1 );
			}

			BinaryChunk l_chunk;

			if ( l_return )
			{
				l_return = l_header.GetSubChunk( l_chunk );
			}

			if ( l_return )
			{
				l_return = Parse( p_obj, l_chunk );
			}

			return l_return;
		}
		/**
		 *\~english
		 *\brief		From chunk reader function
		 *\param[out]	p_obj	The object to read
		 *\param[in]	p_chunk	The chunk
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Fonction de lecture à partir d'un chunk
		 *\param[out]	p_obj	L'objet à lire
		 *\param[in]	p_chunk	Le chunk
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		inline bool Parse( TParsed & p_obj, BinaryChunk & p_chunk )
		{
			bool l_return = true;

			if ( p_chunk.GetChunkType() == ChunkTyper< TParsed >::Value )
			{
				m_chunk = &p_chunk;
			}
			else
			{
				Castor::Logger::LogError( cuT( "Not a valid chunk for parsed type." ) );
				l_return = false;
			}

			if ( l_return )
			{
				l_return = DoParse( p_obj );

				if ( !l_return )
				{
					m_chunk->EndParse();
				}
			}

			return l_return;
		}

	protected:
		/**
		 *\~english
		 *\brief			Parses the header chunk.
		 *\param[in,out]	p_chunk	The parent chunk.
		 *\return			\p false if any error occured.
		 *\~french
		 *\brief			Lit le chunk d'en-tête.
		 *\param[in,out]	p_chunk	Le chunk.
		 *\return			\p false si une erreur quelconque est arrivée.
		 */
		inline bool DoParseHeader( BinaryChunk & p_chunk )const
		{
			BinaryChunk l_chunk;
			bool l_return = p_chunk.GetSubChunk( l_chunk );

			if ( l_chunk.GetChunkType() != ChunkType::eCmshHeader )
			{
				Castor::Logger::LogError( cuT( "Missing header chunk." ) );
				l_return = false;
			}

			Castor::String l_name;
			uint32_t l_version{ 0 };

			while ( l_return && l_chunk.CheckAvailable( 1 ) )
			{
				BinaryChunk l_subchunk;
				l_return = l_chunk.GetSubChunk( l_subchunk );

				switch ( l_subchunk.GetChunkType() )
				{
				case ChunkType::eName:
					l_return = DoParseChunk( l_name, l_subchunk );
					break;

				case ChunkType::eCmshVersion:
					l_return = DoParseChunk( l_version, l_subchunk );
					break;
				}
			}

			if ( !l_return )
			{
				p_chunk.EndParse();
			}

			return l_return;
		}
		/**
		 *\~english
		 *\brief		Retrieves a value array from a chunk
		 *\param[out]	p_values	Receives the parsed values
		 *\param[out]	p_count		The values count
		 *\param[in]	p_chunk		The chunk containing the values
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Récupère un tableau de valeurs à partir d'un chunk
		 *\param[out]	p_values	Reçoit les valeurs
		 *\param[out]	p_count		Le compte des valeurs
		 *\param[in]	p_chunk		Le chunk contenant les valeurs
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		template< typename T >
		inline bool DoParseChunk( T * p_values, size_t p_count, BinaryChunk & p_chunk )const
		{
			return ChunkParser< T >::Parse( p_values, p_count, p_chunk );
		}
		/**
		 *\~english
		 *\brief		Retrieves a value array from a chunk
		 *\param[out]	p_values	Receives the parsed values
		 *\param[in]	p_chunk		The chunk containing the values
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Récupère un tableau de valeurs à partir d'un chunk
		 *\param[out]	p_values	Reçoit les valeurs
		 *\param[in]	p_chunk		Le chunk contenant les valeurs
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		template< typename T, size_t Count >
		inline bool DoParseChunk( T( &p_values )[Count], BinaryChunk & p_chunk )const
		{
			return ChunkParser< T >::Parse( p_values, Count, p_chunk );
		}
		/**
		 *\~english
		 *\brief		Retrieves a value array from a chunk
		 *\param[out]	p_values	Receives the parsed values
		 *\param[in]	p_chunk		The chunk containing the values
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Récupère un tableau de valeurs à partir d'un chunk
		 *\param[out]	p_values	Reçoit les valeurs
		 *\param[in]	p_chunk		Le chunk contenant les valeurs
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		template< typename T, size_t Count >
		inline bool DoParseChunk( std::array< T, Count > & p_values, BinaryChunk & p_chunk )const
		{
			return ChunkParser< T >::Parse( p_values.data(), Count, p_chunk );
		}
		/**
		 *\~english
		 *\brief		Retrieves a value array from a chunk
		 *\param[out]	p_values	Receives the parsed values
		 *\param[in]	p_chunk		The chunk containing the values
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Récupère un tableau de valeurs à partir d'un chunk
		 *\param[out]	p_values	Reçoit les valeurs
		 *\param[in]	p_chunk		Le chunk contenant les valeurs
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		template< typename T >
		inline bool DoParseChunk( std::vector< T > & p_values, BinaryChunk & p_chunk )const
		{
			return ChunkParser< T >::Parse( p_values.data(), p_values.size(), p_chunk );
		}
		/**
		 *\~english
		 *\brief		Retrieves a value from a chunk
		 *\param[out]	p_value	Receives the parsed value
		 *\param[in]	p_chunk		The chunk containing the value
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Récupère une valeur à partir d'un chunk
		 *\param[out]	p_value	Reçoit la valeur
		 *\param[in]	p_chunk		Le chunk contenant la valeur
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		template< typename T >
		inline bool DoParseChunk( T & p_value, BinaryChunk & p_chunk )const
		{
			return ChunkParser< T >::Parse( p_value, p_chunk );
		}
		/**
		 *\~english
		 *\brief		Retrieves a subchunk.
		 *\param[out]	p_chunk	Receives the subchunk.
		 *\return		\p false if any error occured.
		 *\~french
		 *\brief		Récupère un sous-chunk.
		 *\param[out]	p_chunk	Reçoit le sous-chunk.
		 *\return		\p false si une erreur quelconque est arrivée.
		 */
		inline bool DoGetSubChunk( BinaryChunk & p_chunk )
		{
			REQUIRE( m_chunk );
			bool l_return = m_chunk->CheckAvailable( 1 );

			if ( l_return )
			{
				l_return = m_chunk->GetSubChunk( p_chunk );
			}

			return l_return;
		}

	private:
		/**
		 *\~english
		 *\brief		From chunk reader function
		 *\param[out]	p_obj	The object to read
		 *\param[in]	p_chunk	The chunk
		 *\return		\p false if any error occured
		 *\~french
		 *\brief		Fonction de lecture à partir d'un chunk
		 *\param[out]	p_obj	L'objet à lire
		 *\param[in]	p_chunk	Le chunk
		 *\return		\p false si une erreur quelconque est arrivée
		 */
		C3D_API virtual bool DoParse( TParsed & p_obj ) = 0;

	protected:
		//!\~english	The writer's chunk.
		//!\~french		Le chunk du writer.
		BinaryChunk * m_chunk{ nullptr };
	};
	template< class TParsed >
	class BinaryParser;
}

#endif