/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.html)

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
#ifndef ___CastorTest_UnitTest___
#define ___CastorTest_UnitTest___

#include "TestPrerequisites.hpp"

#include <cmath>

namespace Testing
{
	template< typename T >
	inline std::string to_string( T const & p_value )
	{
		std::stringstream l_stream;
		l_stream << p_value;
		return l_stream.str();
	}

	template<>
	inline std::string to_string< std::wstring >( std::wstring const & p_value )
	{
		std::stringstream l_stream;
		l_stream << Castor::string::string_cast< char >( p_value );
		return l_stream.str();
	}

	template< typename T, typename U >
	inline bool compare( T const & p_a, U const & p_b )
	{
		return p_a == p_b;
	}

	template<>
	inline bool compare< float, float >( float const & p_a, float const & p_b )
	{
		float l_epsilon = float( 0.0001 );
		return std::abs( float( p_a - p_b ) ) < l_epsilon;
	}

	template<>
	inline bool compare< float, double >( float const & p_a, double const & p_b )
	{
		float l_epsilon = float( 0.0001 );
		return std::abs( float( p_a - p_b ) ) < l_epsilon;
	}

	template<>
	inline bool compare< double, double >( double const & p_a, double const & p_b )
	{
		double l_epsilon = double( 0.0001 );
		return std::abs( double( p_a - p_b ) ) < l_epsilon;
	}

	template<>
	inline bool compare< double, float >( double const & p_a, float const & p_b )
	{
		double l_epsilon = double( 0.0001 );
		return std::abs( double( p_a - p_b ) ) < l_epsilon;
	}

	class TestCase
	{
	public:
		using TestFunction = std::function< void() >;

	public:
		TestCase( std::string const & p_name );
		virtual ~TestCase();
		void Execute( uint32_t & p_errCount, uint32_t & p_testCount );
		inline std::string const & GetName()const
		{
			return m_name;
		}

	protected:
		void DoRegisterTest( std::string const & p_name, TestFunction p_test );

	private:
		virtual void DoRegisterTests() = 0;

	protected:
		void DoReportFailure()
		{
			( *m_errorCount )++;
		}

		void DoAddTest()
		{
			( *m_testCount )++;
		}

	private:
		uint32_t * m_errorCount{ nullptr };
		uint32_t * m_testCount{ nullptr };
		std::string m_name;
		std::map< std::string, TestFunction > m_tests;
	};

	DECLARE_SMART_PTR( TestCase );

	class TestFailed
		:	public std::exception
	{
	public:
		TestFailed( std::string const & p_what, std::string const & p_file, std::string const & p_function, int p_line );
		virtual ~TestFailed() throw();
		const char * what()
		{
			return m_what.c_str();
		}

	private:
		std::string m_what;
	};

#	define TEST_CHECK( x )\
	DoAddTest();\
	if( !(x) )\
	{\
		DoReportFailure();\
		Castor::Logger::LogWarning( std::stringstream() << "Failure at " << __FILE__ << " - " << __FUNCTION__ << ", line " << __LINE__ << ": " << #x );\
	}

#	define TEST_CHECK_THROW( x )\
	DoAddTest();\
	try\
	{\
		( x ); \
		DoReportFailure();\
		Castor::Logger::LogWarning( std::stringstream() << "Failure at " << __FILE__ << " - " << __FUNCTION__ << ", line " << __LINE__ << ": " << #x );\
	}\
	catch ( ... )\
	{\
	}

#	define TEST_CHECK_NOTHROW( x )\
	DoAddTest();\
	try\
	{\
		( x ); \
	}\
	catch ( ... )\
	{\
		DoReportFailure();\
		Castor::Logger::LogWarning( std::stringstream() << "Failure at " << __FILE__ << " - " << __FUNCTION__ << ", line " << __LINE__ << ": " << #x );\
	}

#	define TEST_EQUAL( x, y )\
	DoAddTest();\
	if( !compare( x, y ) )\
	{\
		DoReportFailure();\
		Castor::Logger::LogWarning( std::stringstream() << "Failure at " << __FILE__ << " - " << __FUNCTION__ << ", line " << __LINE__ << ": " << #x << " == " << #y << " (" << Testing::to_string( x ) << " != " << Testing::to_string( y ) << ")" );\
	}

#	define TEST_REQUIRE( x )\
	DoAddTest();\
	if( !(x) )\
	{\
		throw TestFailed( #x, __FILE__, __FUNCTION__, __LINE__ );\
	}
}

#endif
