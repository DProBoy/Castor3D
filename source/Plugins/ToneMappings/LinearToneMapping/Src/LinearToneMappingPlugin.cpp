﻿#include "LinearToneMapping.hpp"

#include <Log/Logger.hpp>

#include <Engine.hpp>
#include <Cache/TargetCache.hpp>

#include <Miscellaneous/Version.hpp>

using namespace Linear;

#ifndef CASTOR_PLATFORM_WINDOWS
#	define C3D_LinearToneMapping_API
#else
#	ifdef LinearToneMapping_EXPORTS
#		define C3D_LinearToneMapping_API __declspec( dllexport )
#	else
#		define C3D_LinearToneMapping_API __declspec( dllimport )
#	endif
#endif

extern "C"
{
	C3D_LinearToneMapping_API void GetRequiredVersion( Castor3D::Version * p_version )
	{
		*p_version = Castor3D::Version();
	}

	C3D_LinearToneMapping_API void GetType( Castor3D::PluginType * p_type )
	{
		*p_type = Castor3D::PluginType::eToneMapping;
	}

	C3D_LinearToneMapping_API void GetName( char const ** p_name )
	{
		static Castor::String const Name = cuT( "Linear Tone Mapping" );
		*p_name = Name.c_str();
	}

	C3D_LinearToneMapping_API void OnLoad( Castor3D::Engine * p_engine, Castor3D::Plugin * p_plugin )
	{
		p_engine->GetRenderTargetCache().GetToneMappingFactory().Register( ToneMapping::Name
			, &ToneMapping::Create );
	}

	C3D_LinearToneMapping_API void OnUnload( Castor3D::Engine * p_engine )
	{
		p_engine->GetRenderTargetCache().GetToneMappingFactory().Unregister( ToneMapping::Name );
	}
}