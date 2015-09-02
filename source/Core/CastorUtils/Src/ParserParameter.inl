﻿#include "Utils.hpp"
#include "Logger.hpp"
#include "PixelFormat.hpp"
#include "FileParserContext.hpp"
#include "ParserParameterTypeException.hpp"

namespace Castor
{
	namespace
	{
		/**
		 *\~english
		 *\brief		Parses a vector from a line
		 *\param[in]	p_strParams	The line containing the vector
		 *\param[out]	p_vResult	Receives the result
		 *\param[in]	p_pContext	The current parsing context
		 *\return		\p true if OK
		 *\~french
		 *\brief		Extrait un vecteur à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant le vecteur
		 *\param[out]	p_vResult	Reàoit le ràsultat
		 *\param[in]	p_pContext	Le context d'analyse
		 *\return		\p true si tout s'est bien passà
		 */
		inline bool ParseSize( String & p_strParams, Size & p_vResult )
		{
			bool l_bReturn = false;

			if ( p_strParams == cuT( "screen_size" ) )
			{
				l_bReturn = Castor::System::GetScreenSize( 0, p_vResult );
			}
			else
			{
				StringArray l_arrayValues = str_utils::split( p_strParams, cuT( " \t,;" ) );

				if ( l_arrayValues.size() >= 2 )
				{
					p_strParams.clear();
					std::for_each( l_arrayValues.begin() + 2, l_arrayValues.end(), [&]( String const & p_strParam )
					{
						p_strParams += p_strParam + cuT( " " );
					} );
					str_utils::trim( p_strParams );
					{
						std::basic_istringstream< xchar > l_stream( l_arrayValues[0] );
						l_stream >> p_vResult.ptr()[0];
					}
					{
						std::basic_istringstream< xchar > l_stream( l_arrayValues[1] );
						l_stream >> p_vResult.ptr()[1];
					}
					l_bReturn = true;
				}
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses a vector from a line
		 *\param[in]	p_strParams	The line containing the vector
		 *\param[out]	p_vResult	Receives the result
		 *\param[in]	p_pContext	The current parsing context
		 *\return		\p true if OK
		 *\~french
		 *\brief		Extrait un vecteur à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant le vecteur
		 *\param[out]	p_vResult	Reàoit le ràsultat
		 *\param[in]	p_pContext	Le context d'analyse
		 *\return		\p true si tout s'est bien passà
		 */
		inline bool ParsePosition( String & p_strParams, Position & p_vResult )
		{
			bool l_bReturn = false;

			StringArray l_arrayValues = str_utils::split( p_strParams, cuT( " \t,;" ) );

			if ( l_arrayValues.size() >= 2 )
			{
				p_strParams.clear();
				std::for_each( l_arrayValues.begin() + 2, l_arrayValues.end(), [&]( String const & p_strParam )
				{
					p_strParams += p_strParam + cuT( " " );
				} );
				str_utils::trim( p_strParams );
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[0] );
					l_stream >> p_vResult.ptr()[0];
				}
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[1] );
					l_stream >> p_vResult.ptr()[1];
				}
				l_bReturn = true;
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses a vector from a line
		 *\param[in]	p_strParams	The line containing the vector
		 *\param[out]	p_vResult	Receives the result
		 *\param[in]	p_pContext	The current parsing context
		 *\return		\p true if OK
		 *\~french
		 *\brief		Extrait un vecteur à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant le vecteur
		 *\param[out]	p_vResult	Reàoit le ràsultat
		 *\param[in]	p_pContext	Le context d'analyse
		 *\return		\p true si tout s'est bien passà
		 */
		inline bool ParseRectangle( String & p_strParams, Rectangle & p_vResult )
		{
			bool l_bReturn = false;

			StringArray l_arrayValues = str_utils::split( p_strParams, cuT( " \t,;" ) );

			if ( l_arrayValues.size() >= 4 )
			{
				p_strParams.clear();
				std::for_each( l_arrayValues.begin(), l_arrayValues.end(), [&]( String const & p_strParam )
				{
					p_strParams += p_strParam + cuT( " " );
				} );
				str_utils::trim( p_strParams );
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[0] );
					l_stream >> p_vResult.ptr()[0];
				}
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[1] );
					l_stream >> p_vResult.ptr()[1];
				}
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[2] );
					l_stream >> p_vResult.ptr()[2];
				}
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[3] );
					l_stream >> p_vResult.ptr()[3];
				}
				l_bReturn = true;
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses a vector from a line
		 *\param[in]	p_strParams	The line containing the vector
		 *\param[out]	p_vResult	Receives the result
		 *\param[in]	p_pContext	The current parsing context
		 *\return		\p true if OK
		 *\~french
		 *\brief		Extrait un vecteur à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant le vecteur
		 *\param[out]	p_vResult	Reàoit le ràsultat
		 *\param[in]	p_pContext	Le context d'analyse
		 *\return		\p true si tout s'est bien passà
		 */
		template< typename T, uint32_t Count >
		inline bool ParseVector( String & p_strParams, Point< T, Count > & p_vResult )
		{
			bool l_bReturn = false;
			StringArray l_arrayValues = str_utils::split( p_strParams, cuT( " \t,;" ), Count + 1, false );

			if ( l_arrayValues.size() >= Count )
			{
				p_strParams.clear();
				std::for_each( l_arrayValues.begin() + Count, l_arrayValues.end(), [&]( String const & p_strParam )
				{
					p_strParams += p_strParam + cuT( " " );
				} );
				str_utils::trim( p_strParams );

				for ( uint32_t i = 0; i < Count; i++ )
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[i] );
					l_stream >> p_vResult[i];
				}

				l_bReturn = true;
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses a colour from a line
		 *\param[in]	p_strParams	The line containing the vector
		 *\param[out]	p_colour	Receives the result
		 *\param[in]	p_pContext	The current parsing context
		 *\return		\p true if OK
		 *\~french
		 *\brief		Extrait une couleur à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant le vecteur
		 *\param[out]	p_colour	Reàoit le ràsultat
		 *\param[in]	p_pContext	Le context d'analyse
		 *\return		\p true si tout s'est bien passà
		 */
		inline bool ParseColour( String & p_strParams, Colour & p_colour )
		{
			bool l_bReturn = false;
			StringArray l_arrayValues = str_utils::split( p_strParams, cuT( " \t,;" ) );

			if ( l_arrayValues.size() >= Colour::eCOMPONENT_COUNT )
			{
				p_strParams.clear();
				std::for_each( l_arrayValues.begin() + Colour::eCOMPONENT_COUNT, l_arrayValues.end(), [&]( String const & p_strParam )
				{
					p_strParams += p_strParam + cuT( " " );
				} );
				str_utils::trim( p_strParams );

				for ( uint8_t i = 0; i < Colour::eCOMPONENT_COUNT; i++ )
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[i] );
					double l_dComponent;
					l_stream >> l_dComponent;
					p_colour[Colour::eCOMPONENT( i )] = l_dComponent;
				}

				l_bReturn = true;
			}
			else if ( l_arrayValues.size() == 3 )
			{
				p_strParams.clear();

				for ( uint8_t i = 0; i < 3; i++ )
				{
					std::basic_istringstream< xchar > l_stream( l_arrayValues[i] );
					double l_dComponent;
					l_stream >> l_dComponent;
					p_colour[Colour::eCOMPONENT( i )] = l_dComponent;
				}

				p_colour[Colour::eCOMPONENT_ALPHA] = 1.0;
				l_bReturn = true;
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses a signed integer from a line
		 *\param[in]	p_strParams	The line containing the value
		 *\param[out]	p_tResult	Receives the result
		 *\~french
		 *\brief		Extrait un entier signà à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant la valeur
		 *\param[out]	p_tResult	Reàoit le ràsultat
		 */
		template< typename T >
		inline bool ParseInteger( String & p_strParams, T & p_tResult, typename std::enable_if < !std::is_unsigned< T >::value >::type * = 0 )
		{
			bool l_bReturn = false;
			StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );

			if ( l_array.size() )
			{
				l_bReturn = str_utils::is_integer( l_array[0] );
				int64_t l_i64Tmp = 0;

				if ( l_bReturn )
				{
					l_i64Tmp = str_utils::to_long_long( l_array[0] );
					l_bReturn = ( l_i64Tmp > std::numeric_limits< T >::lowest() ) && ( l_i64Tmp < std::numeric_limits< T >::max() );
				}

				if ( l_bReturn )
				{
					p_tResult = static_cast< T >( l_i64Tmp );
				}

				p_strParams.clear();

				if ( l_array.size() > 1 )
				{
					p_strParams = l_array[1];
				}
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses an unsigned integer from a line
		 *\param[in]	p_strParams	The line containing the value
		 *\param[out]	p_tResult	Receives the result
		 *\~french
		 *\brief		Extrait un entier non signà à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant la valeur
		 *\param[out]	p_tResult	Reàoit le ràsultat
		 */
		template< typename T >
		inline bool ParseInteger( String & p_strParams, T & p_tResult, typename std::enable_if< std::is_unsigned< T >::value >::type * = 0 )
		{
			bool l_bReturn = false;
			StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );

			if ( l_array.size() )
			{
				l_bReturn = str_utils::is_integer( l_array[0] );
				uint64_t l_ui64Tmp = 0;

				if ( l_bReturn )
				{
					l_ui64Tmp = str_utils::to_long_long( l_array[0] );
					l_bReturn = ( l_ui64Tmp > std::numeric_limits< T >::lowest() ) && ( l_ui64Tmp < std::numeric_limits< T >::max() );
				}

				if ( l_bReturn )
				{
					p_tResult = static_cast< T >( l_ui64Tmp );
				}

				p_strParams.clear();

				if ( l_array.size() > 1 )
				{
					p_strParams = l_array[1];
				}
			}

			return l_bReturn;
		}
		/**
		 *\~english
		 *\brief		Parses an floating number from a line
		 *\param[in]	p_strParams	The line containing the value
		 *\param[out]	p_tResult	Receives the result
		 *\~french
		 *\brief		Extrait un flottant à partir d'une ligne
		 *\param[in]	p_strParams	La ligne contenant la valeur
		 *\param[out]	p_tResult	Reàoit le ràsultat
		 */
		template< typename T >
		inline bool ParseFloat( String & p_strParams, T & p_tResult )
		{
			bool l_bReturn = false;
			StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );

			if ( l_array.size() )
			{
				l_bReturn = str_utils::is_floating( l_array[0] );
				long double l_ldTmp = 0;

				if ( l_bReturn )
				{
					l_ldTmp = str_utils::to_long_double( l_array[0] );
					l_bReturn = ( l_ldTmp > std::numeric_limits< T >::lowest() ) && ( l_ldTmp < std::numeric_limits< T >::max() );
				}

				if ( l_bReturn )
				{
					p_tResult = static_cast< T >( l_ldTmp );
				}

				p_strParams.clear();

				if ( l_array.size() > 1 )
				{
					p_strParams = l_array[1];
				}
			}

			return l_bReturn;
		}
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_TEXT >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_TEXT >::GetType()
	{
		return ePARAMETER_TYPE_TEXT;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_TEXT >::GetStrType()
	{
		return cuT( "text" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_TEXT >::Parse( String & p_strParams )
	{
		m_value = p_strParams;

		if ( !m_value.empty() )
		{
			if ( m_value[0] == cuT( '\"' ) )
			{
				m_value = m_value.substr( 1 );

				if ( !m_value.empty() )
				{
					if ( m_value[m_value.size() - 1] == cuT( '\"' ) )
					{
						m_value = m_value.substr( 0, m_value.size() - 1 );
					}
				}
			}
		}

		p_strParams.clear();
		return true;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_NAME >::ParserParameter( FileParserContext & p_context )
		: ParserParameter< ePARAMETER_TYPE_TEXT >( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_NAME >::GetType()
	{
		return ePARAMETER_TYPE_NAME;
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_NAME >::GetBaseType()
	{
		return ePARAMETER_TYPE_TEXT;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_NAME >::GetStrType()
	{
		return cuT( "name" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_NAME >::Parse( String & p_strParams )
	{
		StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );
		p_strParams.clear();

		if ( !l_array.empty() )
		{
			m_value = l_array[0];
			str_utils::trim( m_value );

			if ( !m_value.empty() )
			{
				if ( m_value[0] == cuT( '\"' ) )
				{
					m_value = m_value.substr( 1 );

					if ( !m_value.empty() )
					{
						std::size_t l_uiIndex = m_value.find( cuT( '\"' ) );

						if ( l_uiIndex != String::npos )
						{
							if ( l_uiIndex != m_value.size() - 1 )
							{
								l_array[1] = m_value.substr( l_uiIndex + 1 ) + l_array[1];
							}

							m_value = m_value.substr( 0, l_uiIndex );
						}
					}
				}
			}

			if ( l_array.size() > 1 )
			{
				p_strParams = l_array[1];
			}
		}

		return !m_value.empty();
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_PATH >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_PATH >::GetType()
	{
		return ePARAMETER_TYPE_PATH;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_PATH >::GetStrType()
	{
		return cuT( "path" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_PATH >::Parse( String & p_strParams )
	{
		m_value = p_strParams;
		p_strParams.clear();

		if ( !m_value.empty() )
		{
			if ( m_value[0] == cuT( '\"' ) )
			{
				m_value = m_value.substr( 1 );

				if ( !m_value.empty() )
				{
					std::size_t l_uiIndex = m_value.find( cuT( '\"' ) );

					if ( l_uiIndex != String::npos )
					{
						if ( l_uiIndex != m_value.size() - 1 )
						{
							p_strParams = m_value.substr( l_uiIndex + 1 );
							str_utils::trim( p_strParams );
						}

						m_value = m_value.substr( 0, l_uiIndex );
					}
				}
			}
		}

		return !m_value.empty();
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_BOOL >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_BOOL >::GetType()
	{
		return ePARAMETER_TYPE_BOOL;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_BOOL >::GetStrType()
	{
		return cuT( "boolean" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_BOOL >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );
		p_strParams.clear();

		if ( !l_array.empty() )
		{
			m_value = str_utils::to_lower_case( l_array[0] ) == cuT( "true" );
			l_bReturn = l_array[0] == cuT( "true" ) || l_array[0] == cuT( "false" );

			if ( l_array.size() > 1 )
			{
				p_strParams = l_array[1];
			}
		}

		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_INT8 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_INT8 >::GetType()
	{
		return ePARAMETER_TYPE_INT8;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_INT8 >::GetStrType()
	{
		return cuT( "8 bits signed integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_INT8 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_INT16 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_INT16 >::GetType()
	{
		return ePARAMETER_TYPE_INT16;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_INT16 >::GetStrType()
	{
		return cuT( "16 bits signed integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_INT16 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_INT32 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_INT32 >::GetType()
	{
		return ePARAMETER_TYPE_INT32;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_INT32 >::GetStrType()
	{
		return cuT( "32 bits signed integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_INT32 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_INT64 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_INT64 >::GetType()
	{
		return ePARAMETER_TYPE_INT64;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_INT64 >::GetStrType()
	{
		return cuT( "64 bits signed integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_INT64 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_UINT8 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_UINT8 >::GetType()
	{
		return ePARAMETER_TYPE_UINT8;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_UINT8 >::GetStrType()
	{
		return cuT( "8 bits unsigned integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_UINT8 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_UINT16 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_UINT16 >::GetType()
	{
		return ePARAMETER_TYPE_UINT16;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_UINT16 >::GetStrType()
	{
		return cuT( "16 bits unsigned integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_UINT16 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_UINT32 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_UINT32 >::GetType()
	{
		return ePARAMETER_TYPE_UINT32;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_UINT32 >::GetStrType()
	{
		return cuT( "32 bits unsigned integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_UINT32 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_CHECKED_TEXT >::ParserParameter( FileParserContext & p_context, UIntStrMap const & p_mapValues )
		: ParserParameter< ePARAMETER_TYPE_UINT32 >( p_context )
		, m_mapValues( p_mapValues )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_CHECKED_TEXT >::GetType()
	{
		return ePARAMETER_TYPE_CHECKED_TEXT;
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_CHECKED_TEXT >::GetBaseType()
	{
		return ePARAMETER_TYPE_UINT32;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_CHECKED_TEXT >::GetStrType()
	{
		return cuT( "checked text" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_CHECKED_TEXT >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );
		p_strParams.clear();

		if ( !l_array.empty() )
		{
			UIntStrMapIt l_it = m_mapValues.find( l_array[0] );

			if ( l_it != m_mapValues.end() )
			{
				m_value = l_it->second;
				l_bReturn = true;
			}

			if ( l_array.size() > 1 )
			{
				p_strParams = l_array[1];
			}
		}

		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT >::ParserParameter( FileParserContext & p_context, UIntStrMap const & p_mapValues )
		: ParserParameter< ePARAMETER_TYPE_UINT32 >( p_context )
		, m_mapValues( p_mapValues )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT >::GetType()
	{
		return ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT;
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT >::GetBaseType()
	{
		return ePARAMETER_TYPE_UINT32;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT >::GetStrType()
	{
		return cuT( "bitwise ORed checked texts" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_BITWISE_ORED_CHECKED_TEXT >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		m_value = 0;
		StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );
		p_strParams.clear();

		if ( !l_array.empty() )
		{
			StringArray l_values = str_utils::split( l_array[0], cuT( "|" ), std::count( l_array[0].begin(), l_array[0].end(), cuT( '|' ) ) + 1, false );

			for ( auto && l_value: l_values )
			{
				UIntStrMapIt l_it = m_mapValues.find( l_value );

				if ( l_it != m_mapValues.end() )
				{
					m_value |= l_it->second;
					l_bReturn = true;
				}
			}

			if ( l_array.size() > 1 )
			{
				p_strParams = l_array[1];
			}
		}

		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_UINT64 >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_UINT64 >::GetType()
	{
		return ePARAMETER_TYPE_UINT64;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_UINT64 >::GetStrType()
	{
		return cuT( "64 bits unsigned integer" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_UINT64 >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseInteger( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_FLOAT >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_FLOAT >::GetType()
	{
		return ePARAMETER_TYPE_FLOAT;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_FLOAT >::GetStrType()
	{
		return cuT( "simple precision floating point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_FLOAT >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseFloat( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_DOUBLE >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_DOUBLE >::GetType()
	{
		return ePARAMETER_TYPE_DOUBLE;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_DOUBLE >::GetStrType()
	{
		return cuT( "double precision floating point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_DOUBLE >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseFloat( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_LONGDOUBLE >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_LONGDOUBLE >::GetType()
	{
		return ePARAMETER_TYPE_LONGDOUBLE;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_LONGDOUBLE >::GetStrType()
	{
		return cuT( "long double precision floating point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_LONGDOUBLE >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseFloat( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_PIXELFORMAT >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_PIXELFORMAT >::GetType()
	{
		return ePARAMETER_TYPE_PIXELFORMAT;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_PIXELFORMAT >::GetStrType()
	{
		return cuT( "pixel format" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_PIXELFORMAT >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		StringArray l_array = str_utils::split( p_strParams, cuT( " \t,;" ), 1, false );
		p_strParams.clear();

		if ( l_array.size() )
		{
			m_value = PF::GetFormatByName( l_array[0] );
			l_bReturn = m_value != ePIXEL_FORMAT_COUNT;

			if ( l_array.size() > 1 )
			{
				p_strParams = l_array[1];
			}
		}

		return l_bReturn;
	}

	//*************************************************************************************************

  inline ParserParameter< ePARAMETER_TYPE_POINT2I >::ParserParameter( FileParserContext & p_context )
	  : ParserParameterBase( p_context )
  {
  }

  inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT2I >::GetType()
  {
	  return ePARAMETER_TYPE_POINT2I;
  }

  inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT2I >::GetStrType()
  {
	  return cuT( "2 integers point" );
  }

  inline bool ParserParameter< ePARAMETER_TYPE_POINT2I >::Parse( String & p_strParams )
  {
	  bool l_bReturn = false;
	  l_bReturn = ParseVector( p_strParams, m_value );
	  return l_bReturn;
  }

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT3I >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT3I >::GetType()
	{
		return ePARAMETER_TYPE_POINT3I;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT3I >::GetStrType()
	{
		return cuT( "3 integers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT3I >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT4I >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT4I >::GetType()
	{
		return ePARAMETER_TYPE_POINT4I;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT4I >::GetStrType()
	{
		return cuT( "4 integers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT4I >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT2F >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT2F >::GetType()
	{
		return ePARAMETER_TYPE_POINT2F;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT2F >::GetStrType()
	{
		return cuT( "2 simple precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT2F >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT3F >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT3F >::GetType()
	{
		return ePARAMETER_TYPE_POINT3F;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT3F >::GetStrType()
	{
		return cuT( "3 simple precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT3F >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT4F >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT4F >::GetType()
	{
		return ePARAMETER_TYPE_POINT4F;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT4F >::GetStrType()
	{
		return cuT( "4 simple precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT4F >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT2D >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT2D >::GetType()
	{
		return ePARAMETER_TYPE_POINT2D;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT2D >::GetStrType()
	{
		return cuT( "2 double precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT2D >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT3D >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT3D >::GetType()
	{
		return ePARAMETER_TYPE_POINT3D;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT3D >::GetStrType()
	{
		return cuT( "3 double precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT3D >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POINT4D >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POINT4D >::GetType()
	{
		return ePARAMETER_TYPE_POINT4D;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POINT4D >::GetStrType()
	{
		return cuT( "4 double precision floating point numbers point" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POINT4D >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseVector( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_SIZE >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_SIZE >::GetType()
	{
		return ePARAMETER_TYPE_SIZE;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_SIZE >::GetStrType()
	{
		return cuT( "size" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_SIZE >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseSize( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_POSITION >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_POSITION >::GetType()
	{
		return ePARAMETER_TYPE_POSITION;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_POSITION >::GetStrType()
	{
		return cuT( "position" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_POSITION >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParsePosition( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_RECTANGLE >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_RECTANGLE >::GetType()
	{
		return ePARAMETER_TYPE_RECTANGLE;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_RECTANGLE >::GetStrType()
	{
		return cuT( "rectangle" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_RECTANGLE >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseRectangle( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	inline ParserParameter< ePARAMETER_TYPE_COLOUR >::ParserParameter( FileParserContext & p_context )
		: ParserParameterBase( p_context )
	{
	}

	inline ePARAMETER_TYPE ParserParameter< ePARAMETER_TYPE_COLOUR >::GetType()
	{
		return ePARAMETER_TYPE_COLOUR;
	}

	inline xchar const * ParserParameter< ePARAMETER_TYPE_COLOUR >::GetStrType()
	{
		return cuT( "colour" );
	}

	inline bool ParserParameter< ePARAMETER_TYPE_COLOUR >::Parse( String & p_strParams )
	{
		bool l_bReturn = false;
		l_bReturn = ParseColour( p_strParams, m_value );
		return l_bReturn;
	}

	//*************************************************************************************************

	template< typename T >
	T const & ParserParameterBase::Get( T & p_value )
	{
		static const ePARAMETER_TYPE l_given = ParserValueTyper< T >::Type;
		ePARAMETER_TYPE l_expected = GetBaseType();

		if ( l_given == l_expected )
		{
			p_value = static_cast< ParserParameter< l_given >* >( this )->m_value;
		}
		else
		{
			throw ParserParameterTypeException( m_context.strFunctionName, l_given, l_expected );
		}

		return p_value;
	}
}

