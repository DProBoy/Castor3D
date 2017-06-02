﻿#include "TowerCategory.hpp"

using namespace Castor;
using namespace Castor3D;

namespace castortd
{
	LongRangeTower::LongRangeTower()
		: Category{ Tower::Category::Kind::eLongRange }
	{
		auto l_costIncrement = []( uint32_t p_inc
			, uint32_t p_value
			, uint32_t p_level )
		{
			return p_value + p_inc + p_inc * ( p_level / 5 );
		};
		auto l_uintIncrement = []( uint32_t p_inc
			, uint32_t p_value
			, uint32_t p_level )
		{
			return p_value + p_inc;
		};
		auto l_floatIncrement = []( float p_inc
			, float p_value
			, uint32_t p_level )
		{
			return p_value + p_inc;
		};
		auto l_decrement = []( std::chrono::milliseconds p_inc
			, std::chrono::milliseconds p_value
			, uint32_t p_level )
		{
			p_value -= p_inc;

			if ( p_value < p_inc )
			{
				p_value = p_inc;
			}

			return p_value;
		};
		m_damage.Initialise( 5u
			, std::bind( l_uintIncrement, 9u, std::placeholders::_1, std::placeholders::_2 )
			, 400u
			, std::bind( l_costIncrement, 30u, std::placeholders::_1, std::placeholders::_2 )
			, 15u );

		m_cooldown.Initialise( std::chrono::milliseconds{ 1000u }
			, std::bind( l_decrement, std::chrono::milliseconds{ 40u }, std::placeholders::_1, std::placeholders::_2 )
			, 200u
			, std::bind( l_costIncrement, 20u, std::placeholders::_1, std::placeholders::_2 ) );

		m_range.Initialise( 100.0f
			, std::bind( l_floatIncrement, 20.0f, std::placeholders::_1, std::placeholders::_2 )
			, 150u
			, std::bind( l_costIncrement, 10u, std::placeholders::_1, std::placeholders::_2 ) );

		m_bulletSpeed = 96.0f;
		m_towerCost = 250u;
		m_material = cuT( "OrangeTowerCube" );
		m_colour = Colour::from_components( 1.0f, 1.0f, 0.0f, 1.0f );
	}

	ShortRangeTower::ShortRangeTower()
		: Category{ Tower::Category::Kind::eShortRange }
	{
		auto l_costIncrement = []( uint32_t p_inc
			, uint32_t p_value
			, uint32_t p_level )
		{
			return p_value + p_inc + p_inc * ( p_level / 5 );
		};
		auto l_uintIncrement = []( uint32_t p_inc
			, uint32_t p_value
			, uint32_t p_level )
		{
			return p_value + p_inc;
		};
		auto l_floatIncrement = []( float p_inc
			, float p_value
			, uint32_t p_level )
		{
			return p_value + p_inc;
		};
		auto l_decrement = []( std::chrono::milliseconds p_inc
			, std::chrono::milliseconds p_value
			, uint32_t p_level )
		{
			p_value -= p_inc;

			if ( p_value < p_inc )
			{
				p_value = p_inc;
			}

			return p_value;
		};
		m_damage.Initialise( 3u
			, std::bind( l_uintIncrement, 5u, std::placeholders::_1, std::placeholders::_2 )
			, 400u
			, std::bind( l_costIncrement, 30u, std::placeholders::_1, std::placeholders::_2 ) );

		m_cooldown.Initialise( std::chrono::milliseconds{ 500u }
			, std::bind( l_decrement, std::chrono::milliseconds{ 30u }, std::placeholders::_1, std::placeholders::_2 )
			, 150u
			, std::bind( l_costIncrement, 10u, std::placeholders::_1, std::placeholders::_2 ) );

		m_range.Initialise( 40.0f
			, std::bind( l_floatIncrement, 4.0f, std::placeholders::_1, std::placeholders::_2 )
			, 200u
			, std::bind( l_costIncrement, 20u, std::placeholders::_1, std::placeholders::_2 ) );

		m_bulletSpeed = 120.0f;
		m_towerCost = 170u;
		m_material = cuT( "BlueTowerCube" );
		m_colour = Colour::from_components( 0.0f, 0.0f, 1.0f, 1.0f );
	}
}
