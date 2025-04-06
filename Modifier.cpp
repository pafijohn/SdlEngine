#include "Modifier.h"
#include "Character.h"

Modifier::~Modifier()
{
	
}

void Modifier::OnStart()
{
	
}

void Modifier::OnStop()
{
	
}

void Modifier::OnEvent( Event& event )
{
	
}

void ArmorModifier::OnEvent( Event& event )
{
	switch ( event.type )
	{
		case Event::DAMAGE:
		{
			switch ( event.damage.type )
			{
				case DamageTypes::SLASH:
				case DamageTypes::BLUNT:
				{
					event.damage.amount *= .85;
					break;
				}
			}
			break;
		}
		case Event::INVENTORY_CHANGE:
		{
			if ( !character->GetModifier( "ArmorModifier" ) )
			{
				character->SetModifier( "ArmorModifier", this );
			}
			break;
		}
	}
}

void RingModifier::OnEvent( Event& event )
{
	static int tickCount = 0;
	switch ( event.type )
	{
		case Event::TICK:
		{
			if ( ( tickCount % 20 ) == 0 )
			{
				// Should make stamina regen faster
				character->AddStamina( 10 );
			}
			break;
		}
	}
	tickCount++;
}
