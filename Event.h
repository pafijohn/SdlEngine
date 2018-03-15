#pragma once

// Maybe put this somewhere else
namespace DamageTypes
{
	enum
	{
		SLASH,
		BLUNT,
		EARTH,
		ICE,
		FIRE,
		STORM,
		DARK,
		LIGHT,
	};
}

struct Event
{
	int type;
	
	enum
	{
		DAMAGE,
		INVENTORY_CHANGE,
		TICK,
		NUM_EVENTS
	};
	
	union
	{
		struct
		{
			int type;
			int amount;
		} damage;
	};
};