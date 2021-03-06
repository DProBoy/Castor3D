#include "BillboardUboPools.hpp"

#include "Engine.hpp"

#include "Event/Frame/FrameListener.hpp"
#include "Event/Frame/FunctorEvent.hpp"
#include "Material/Material.hpp"
#include "Material/Pass.hpp"
#include "Render/RenderPassTimer.hpp"
#include "Scene/BillboardList.hpp"
#include "Scene/Scene.hpp"

#include <Miscellaneous/Hash.hpp>

using namespace castor;

namespace castor3d
{
	namespace
	{
		size_t hash( BillboardBase const & billboard
			, Pass const & pass )
		{
			size_t result = std::hash< BillboardBase const * >{}( &billboard );
			castor::hashCombine( result, pass );
			return result;
		}
	}

	BillboardUboPools::BillboardUboPools( RenderSystem & renderSystem )
		: m_modelUboPool{ renderSystem }
		, m_modelMatrixUboPool{ renderSystem }
		, m_billboardUboPool{ renderSystem }
		, m_pickingUboPool{ renderSystem }
	{
		renderSystem.getEngine()->sendEvent( makeFunctorEvent( EventType::ePreRender
			, [this, &renderSystem]()
			{
				m_updateTimer = std::make_shared< RenderPassTimer >( *renderSystem.getEngine()
					, cuT( "Update" )
					, cuT( "Billboard UBOs" )
					, 4u );
			} ) );
	}

	void BillboardUboPools::update()
	{
		for ( auto & pair : m_entries )
		{
			auto & entry = pair.second;
			auto & modelData = entry.modelUbo.getData();
			modelData.shadowReceiver = entry.billboard.isShadowReceiver();
			modelData.materialIndex = entry.pass.getId();
			auto & modelMatrixData = entry.modelMatrixUbo.getData();
			modelMatrixData.prvModel = modelMatrixData.curModel;
			modelMatrixData.curModel = entry.billboard.getNode()->getDerivedTransformationMatrix();
			auto & billboardData = entry.billboardUbo.getData();
			billboardData.dimensions = entry.billboard.getDimensions();
		}
	}

	void BillboardUboPools::uploadUbos()
	{
		auto count = m_modelUboPool.getBufferCount()
			+ m_modelMatrixUboPool.getBufferCount()
			+ m_billboardUboPool.getBufferCount()
			+ m_pickingUboPool.getBufferCount();
		m_updateTimer->updateCount( std::max( count, 4u ) );

		if ( count )
		{
			auto timerBlock = m_updateTimer->start();
			uint32_t index = 0u;
			m_modelUboPool.upload( *m_updateTimer, index );
			index += std::max( m_modelUboPool.getBufferCount(), 1u );
			m_modelMatrixUboPool.upload( *m_updateTimer, index );
			index += std::max( m_modelMatrixUboPool.getBufferCount(), 1u );
			m_billboardUboPool.upload( *m_updateTimer, index );
			index += std::max( m_billboardUboPool.getBufferCount(), 1u );
			m_pickingUboPool.upload( *m_updateTimer, index );
		}
	}

	void BillboardUboPools::cleanupUbos()
	{
		m_modelUboPool.cleanup();
		m_modelMatrixUboPool.cleanup();
		m_billboardUboPool.cleanup();
		m_pickingUboPool.cleanup();
		m_entries.clear();
		m_connections.clear();
	}

	void BillboardUboPools::registerElement( BillboardBase & billboard )
	{
		m_connections.emplace( &billboard, billboard.onMaterialChanged.connect( [this]( BillboardBase const & billboard
			, MaterialSPtr oldMaterial
			, MaterialSPtr newMaterial )
		{
			if ( oldMaterial )
			{
				for ( auto & pass : *oldMaterial )
				{
					doRemoveEntry( billboard, *pass );
				}
			}

			if ( newMaterial )
			{
				for ( auto & pass : *newMaterial )
				{
					m_entries.emplace( hash( billboard, *pass )
						, doCreateEntry( billboard, *pass ) );
				}
			}
		} ) );

		for ( auto & pass : *billboard.getMaterial() )
		{
			m_entries.emplace( hash( billboard, *pass )
				, doCreateEntry( billboard, *pass ) );
		}
	}

	void BillboardUboPools::unregisterElement( BillboardBase & billboard )
	{
		auto it = m_connections.find( &billboard );

		if ( it != m_connections.end() )
		{
			m_connections.erase( it );

			for ( auto & pass : *billboard.getMaterial() )
			{
				doRemoveEntry( billboard, *pass );
			}
		}
	}

	BillboardUboPools::PoolsEntry BillboardUboPools::getUbos( BillboardBase const & billboard, Pass const & pass )const
	{
		return m_entries.at( hash( billboard, pass ) );
	}

	void BillboardUboPools::clear()
	{
		for ( auto & entry : m_entries )
		{
			m_modelUboPool.putBuffer( entry.second.modelUbo );
			m_modelMatrixUboPool.putBuffer( entry.second.modelMatrixUbo );
			m_billboardUboPool.putBuffer( entry.second.billboardUbo );
			m_pickingUboPool.putBuffer( entry.second.pickingUbo );
		}
	}
	
	BillboardUboPools::PoolsEntry BillboardUboPools::doCreateEntry( BillboardBase const & billboard
		, Pass const & pass )
	{
		return
		{
			billboard,
			pass,
			m_modelUboPool.getBuffer( renderer::MemoryPropertyFlag::eHostVisible ),
			m_modelMatrixUboPool.getBuffer( renderer::MemoryPropertyFlag::eHostVisible ),
			m_billboardUboPool.getBuffer( renderer::MemoryPropertyFlag::eHostVisible ),
			m_pickingUboPool.getBuffer( renderer::MemoryPropertyFlag::eHostVisible ),
		};
	}

	void BillboardUboPools::doRemoveEntry( BillboardBase const & billboard
		, Pass const & pass )
	{
		auto entry = getUbos( billboard, pass );
		m_entries.erase( hash( billboard, pass ) );
		m_modelUboPool.putBuffer( entry.modelUbo );
		m_modelMatrixUboPool.putBuffer( entry.modelMatrixUbo );
		m_billboardUboPool.putBuffer( entry.billboardUbo );
		m_pickingUboPool.putBuffer( entry.pickingUbo );
	}
}
