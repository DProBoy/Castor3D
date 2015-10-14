﻿#include "BlendStateManager.hpp"

#include "RenderSystem.hpp"

using namespace Castor;

namespace Castor3D
{
	BlendStateManager::BlendStateManager( Engine & p_engine )
		: Manager< String, BlendState >( p_engine )
	{
	}

	BlendStateManager::~BlendStateManager()
	{
	}

	BlendStateSPtr BlendStateManager::Create( String const & p_name )
	{
		m_elements.lock();
		BlendStateSPtr l_pReturn = m_elements.find( p_name );

		if ( !l_pReturn )
		{
			l_pReturn = m_renderSystem->CreateBlendState();
			m_elements.insert( p_name, l_pReturn );
			GetOwner()->PostEvent( MakeInitialiseEvent( *l_pReturn ) );
			Logger::LogInfo( cuT( "BlendStateManager::Create - Created BlendState: " ) + p_name + cuT( "" ) );
		}
		else
		{
			Logger::LogWarning( cuT( "BlendStateManager::Create - Duplicate BlendState: " ) + p_name );
		}

		m_elements.unlock();
		return l_pReturn;
	}
}
