#include "Utils.h"

#include "Map.h"
#include "Modifier.h"
#include "Character.h"
#include "Inventoried.h"
#include "MixerManager.h"

Item::Item()
{
	this->isRelative = false;
}

bool Item::Use()
{
	return false;
}

void Item::OnPickUp()
{
	
}

void Item::OnDrop()
{
	
}

Bread::Bread()
{
	this->Load( "res\\items\\bread.png" );
}

bool Bread::Use()
{
	character->AddHealth( 10 );
	return true;
}

Armor::Armor()
{
	this->Load( "res\\items\\armor.png" );
}

bool Armor::Use()
{
	return false;
}

void Armor::OnPickUp()
{
	Modifier* mod = character->GetModifier( "ArmorModifier" );
	
	if ( mod == nullptr )
	{
		ArmorModifier* modifier = new ArmorModifier();
		
		character->SetModifier( "ArmorModifier", modifier );
	}
}

void Armor::OnDrop()
{
	Modifier* mod = character->GetModifier( "ArmorModifier" );
	
	if ( mod )
	{
		character->SetModifier( "ArmorModifier", nullptr );		
		
		delete mod;
	}
}

Ring::Ring()
{
	this->Load( "res\\items\\ring.png" );
}

bool Ring::Use()
{
	return false;
}

void Ring::OnPickUp()
{
	Modifier* mod = character->GetModifier( "RingModifier" );
	
	if ( mod == nullptr )
	{
		RingModifier* modifier = new RingModifier();
		
		character->SetModifier( "RingModifier", modifier );
	}
}

void Ring::OnDrop()
{
	Modifier* mod = character->GetModifier( "RingModifier" );
	
	if ( mod )
	{
		character->SetModifier( "RingModifier", nullptr );		
		
		delete mod;
	}
}

OreItem::OreItem()
{
	this->Load( "res\\items\\ring.png" );
}

Chest::Chest()
{
	this->Load( "res\\items\\chest.png" );
}

bool Chest::Use()
{
	SDL_Point point;
	character->GetCenter( point );
	
	point = Map::GetPosition( point, character->texture.direction, 1.0 );
	
	return Map::PutTile( new ChestTile(), Layers::LEVEL, point.x, point.y );
}

Inventory::Inventory()
{
	this->itemCount = 0;
}

bool Inventory::PushBack( Item* item )
{
	bool pushable = this->size() < 20;
	
	if ( pushable )
	{
		this->push_back( item );
	}
	else
	{
		int idx;
		
		for ( auto it = this->begin(); it != this->end() && !pushable; it++ )
		{
			pushable = ( *it ) == nullptr;
			
			if ( pushable )
			{
				idx = std::distance( this->begin(), it );
				this->at( idx ) = item;
			}
		}
	}
	
	if ( pushable )
	{
		this->itemCount++;
	}
	
	return pushable;
}

bool Inventory::HasSpace()
{
	return this->itemCount < 20;
}

void Inventory::ClearItem( int i )
{
	this->at( i ) = nullptr;
	this->itemCount--;
}

void Inventoried::Use( int i )
{
	Item* item = this->GetItem( i );
	
	if ( item )
	{
		if ( item->Use() )
		{
			this->RemoveItem( i );
		}
	}
}

void Inventoried::AddItem( Item* item )
{
	if ( this->inventory.PushBack( item ) )
	{
		item->OnPickUp();
	}
	else
	{
		// TODO maybe return bool later?
		delete item;
	}
}

Item* Inventoried::GetItem( int i )
{
	return ( isBounded( i, 0, this->inventory.size() ) ) ? this->inventory[ i ] : nullptr;
}

Item* Inventoried::GetItem( const std::string& itemName, int* i )
{
	Item* item = nullptr;
	
	for ( int j = 0; j < this->inventory.size() and item == nullptr; j++ )
	{
		if ( this->inventory[ j ]->name == itemName )
		{
			if ( i )
			{
				*i = j;
			}
			
			item = this->inventory[ j ];
		}
	}
	
	return item;
}

void Inventoried::RemoveItem( int i )
{
	Item* item = GetItem( i );
	item->OnDrop();
	delete item;
	
	this->inventory.ClearItem( i );
}

Equipped::Equipped()
{
	this->head = nullptr;
	this->body = nullptr;
	this->legs = nullptr;
	this->feet = nullptr;
	this->mainHand = nullptr;
	this->offHand = nullptr;
}

void Equipped::SetSlot( int slot, Item* item )
{
	switch ( slot )
	{
		case HEAD:
		{
			this->head = item;
			break;
		}
		case BODY:
		{
			this->body = item;
			break;
		}
		case LEGS:
		{
			this->legs = item;
			break;
		}
		case FEET:
		{
			this->feet = item;
			break;
		}
		case MAIN_HAND:
		{
			this->mainHand = item;
			break;
		}
		case OFF_HAND:
		{
			this->offHand = item;
			break;
		}
	}
}


Item* Equipped::GetSlot( int slot )
{
	Item* item;
	
	switch ( slot )
	{
		case HEAD:
		{
			item = this->head;
			break;
		}
		case BODY:
		{
			item = this->body;
			break;
		}
		case LEGS:
		{
			item = this->legs;
			break;
		}
		case FEET:
		{
			item = this->feet;
			break;
		}
		case MAIN_HAND:
		{
			item = this->mainHand;
			break;
		}
		case OFF_HAND:
		{
			item = this->offHand;
			break;
		}
		default:
		{
			item = nullptr;
			break;
		}
	}
	
	return item;
}