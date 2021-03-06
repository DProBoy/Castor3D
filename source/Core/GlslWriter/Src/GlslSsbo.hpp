/*
See LICENSE file in root folder
*/
#ifndef ___GLSL_Ssbo_H___
#define ___GLSL_Ssbo_H___

#include "GlslBoInfo.hpp"
#include "GlslIndentBlock.hpp"

namespace glsl
{
	class Ssbo
	{
	public:
		enum class Layout
		{
			eStd140,
			eStd430,
			ePacked,
			eShared
		};

		using Info = BoInfo< Ssbo::Layout >;

		GlslWriter_API Ssbo( GlslWriter & writer
			, castor::String const & name
			, uint32_t bind
			, uint32_t set = 0u
			, Layout layout = Layout::eStd430 );
		GlslWriter_API void end();

		template< typename T >
		inline T declMember( castor::String const & name );
		template< typename T >
		inline Array< T > declMember( castor::String const & name
			, uint32_t dimension );
		template< typename T >
		inline Array< T > declMemberArray( castor::String const & name );
		template< typename T >
		inline Optional< T > declMember( castor::String const & name
			, bool enabled );
		template< typename T >
		inline Optional< Array< T > > declMember( castor::String const & name
			, uint32_t dimension
			, bool enabled );
		template< typename T >
		inline Optional< Array< T > > declMemberArray( castor::String const & name
			, bool enabled );
		template< typename T >
		inline T getMember( castor::String const & name );
		template< typename T >
		inline Array< T > getMember( castor::String const & name
			, uint32_t dimension );
		template< typename T >
		inline Array< T > getMemberArray( castor::String const & name );
		template< typename T >
		inline Optional< T > getMember( castor::String const & name
			, bool enabled );
		template< typename T >
		inline Optional< Array< T > > getMember( castor::String const & name
			, uint32_t dimension
			, bool enabled );
		template< typename T >
		inline Optional< Array< T > > getMemberArray( castor::String const & name
			, bool enabled );

	private:
		GlslWriter & m_writer;
		castor::StringStream m_stream;
		std::unique_ptr< IndentBlock > m_block;
		castor::String m_name;
		uint32_t m_count{ 0u };
		Info m_info;
	};
}

#include "GlslSsbo.inl"

#endif
