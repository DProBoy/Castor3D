﻿#include "Benchmark.hpp"

#include <Logger.hpp>
#include <File.hpp>

#include "BenchManager.hpp"
#include "OpenClBench.hpp"
#include "CastorUtilsMatrixTest.hpp"
#include "CastorUtilsPixelFormatTest.hpp"
#include "CastorUtilsStringTest.hpp"
#include "CastorUtilsZipTest.hpp"
#include "CastorUtilsUniqueTest.hpp"

int main( int argc, char const * argv[] )
{
	int l_iReturn = EXIT_SUCCESS;
	int l_iCount = 1;

	if ( argc == 2 )
	{
		l_iCount = std::max< int >( 1, atoi( argv[2] ) );
	}

#if defined( NDEBUG )
	Castor::Logger::Initialise( Castor::ELogType_INFO );
#else
	Castor::Logger::Initialise( Castor::ELogType_DEBUG );
#endif

	Castor::Logger::SetFileName( Castor::File::GetWorkingDirectory() / cuT( "Tests.log" ) );
#if defined( CASTOR_USE_OCL )
	Testing::Register( std::make_shared< Testing::OpenCLBench >() );
#endif
	Testing::Register( std::make_shared< Testing::CastorUtilsMatrixBench >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsStringBench >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsUniqueTest >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsMatrixTest >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsPixelFormatTest >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsStringTest >() );
	Testing::Register( std::make_shared< Testing::CastorUtilsZipTest >() );
	BENCHLOOP( l_iCount, l_iReturn );
	Castor::Logger::Cleanup();
	return l_iReturn;
}
