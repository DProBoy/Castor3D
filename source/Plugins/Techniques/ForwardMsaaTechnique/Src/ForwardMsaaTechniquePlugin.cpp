﻿#include "ForwardMsaaRenderTechnique.hpp"

#if defined( VLD_AVAILABLE )
#	include <vld.h>
#endif

#include <Log/Logger.hpp>

#include <Engine.hpp>
#include <Cache/TechniqueCache.hpp>

#include <Miscellaneous/Version.hpp>

#ifndef CASTOR_PLATFORM_WINDOWS
#	define C3D_ForwardMsaaTechnique_API
#else
#	ifdef ForwardMsaaTechnique_EXPORTS
#		define C3D_ForwardMsaaTechnique_API __declspec(dllexport)
#	else
#		define C3D_ForwardMsaaTechnique_API __declspec(dllimport)
#	endif
#endif

extern "C"
{
	C3D_ForwardMsaaTechnique_API void GetRequiredVersion( Castor3D::Version * p_version )
	{
		*p_version = Castor3D::Version();
	}

	C3D_ForwardMsaaTechnique_API void GetType( Castor3D::PluginType * p_type )
	{
		*p_type = Castor3D::PluginType::eTechnique;
	}

	C3D_ForwardMsaaTechnique_API void GetName( char const ** p_name )
	{
		*p_name = forward_msaa::RenderTechnique::Name.c_str();
	}

	C3D_ForwardMsaaTechnique_API void OnLoad( Castor3D::Engine * p_engine, Castor3D::Plugin * p_plugin )
	{
		p_engine->GetTechniqueFactory().Register( forward_msaa::RenderTechnique::Type
			, &forward_msaa::RenderTechnique::CreateInstance );
	}

	C3D_ForwardMsaaTechnique_API void OnUnload( Castor3D::Engine * p_engine )
	{
		p_engine->GetTechniqueFactory().Unregister( forward_msaa::RenderTechnique::Type );
	}
}