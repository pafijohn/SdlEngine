#include "Utils.h"

#include "Map.h"
#include "Modifier.h"
#include "Character.h"
#include "Inventoried.h"
#include "MixerManager.h"

Item::Item()
{
	this->isRelative = false;
	this->SetHasCollision(true); // Collision for clicking purposes
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
	this->Load("resources\\items\\bread.png");
	this->name = "Bread";
}

bool Bread::Use()
{
	return character->AddHealth(10);
}

Armor::Armor()
{
	this->Load("resources\\items\\armor.png");
	this->name = "Armor";
}

bool Armor::Use()
{
	return false;
}

void Armor::OnPickUp()
{
	Modifier* mod = character->GetModifier("ArmorModifier");
	
	if (mod == nullptr)
	{
		ArmorModifier* modifier = new ArmorModifier();
		
		character->SetModifier("ArmorModifier", modifier);
	}
}

void Armor::OnDrop()
{
	Modifier* mod = character->GetModifier("ArmorModifier");
	
	if (mod)
	{
		character->SetModifier("ArmorModifier", nullptr);		
		
		delete mod;
	}
}

Ring::Ring()
{
	this->Load("resources\\items\\ring.png");
	this->name = "Ring";
}

bool Ring::Use()
{
	return false;
}

void Ring::OnPickUp()
{
	Modifier* mod = character->GetModifier("RingModifier");
	
	if (mod == nullptr)
	{
		RingModifier* modifier = new RingModifier();
		
		character->SetModifier("RingModifier", modifier);
	}
}

void Ring::OnDrop()
{
	Modifier* mod = character->GetModifier("RingModifier");
	
	if (mod)
	{
		character->SetModifier("RingModifier", nullptr);		
		
		delete mod;
	}
}

OreItem::OreItem()
{
	this->Load("resources\\items\\ring.png");
	this->name = "Ore";
}

Chest::Chest()
{
	this->Load("resources\\items\\chest.png");
	this->name = "Chest";
}

bool Chest::Use()
{
	SDL_Point point;
	character->GetCenter(point);
	
	point = Map::GetPosition(point, character->texture.direction, 1.0);
	ChestTile* tile = new ChestTile();
	bool put = Map::PutTile(tile, Layers::LEVEL, point.x, point.y);
	
	return put;
}

Inventory::Inventory()
{
	this->itemCount = 0;
	for (size_t i = 0; i < Inventory::MAX_ITEMS; i++)
	{
		this->PushBack(nullptr);
	}
}

bool Inventory::PushItem(Item* item)
{
	for (size_t i = 0; i < this->Size(); i++)
	{
		Item* elem = this->At(i);
		
		if (elem == nullptr)
		{
			this->Set(i, item);
			this->itemCount++;
			
			return true;
		}
	}
	
	return false;
}

bool Inventory::HasSpace()
{
	return this->itemCount < Inventory::MAX_ITEMS;
}

void Inventory::ClearItem(size_t i)
{
	auto* elem = this->Set(i, nullptr);
	this->itemCount--;
	
	if (elem)
	{
		delete elem;
	}
}

void Inventoried::Use(size_t i)
{
	Item* item = this->GetItem(i);
	
	if (item && item->Use())
	{
		this->RemoveItem(i);
	}
}

void Inventoried::AddItem(Item* item)
{
	if (this->inventory.PushItem(item))
	{
		item->OnPickUp();
	}
	else
	{
		// TODO maybe return bool later?
		delete item;
	}
}

Item* Inventoried::GetItem(size_t i)
{
	return (isBounded(i, 0, this->inventory.Size())) ? this->inventory.At(i) : nullptr;
}

Item* Inventoried::GetItem(const std::string& itemName, size_t* i)
{
	Item* item = nullptr;
	
	for (size_t j = 0; j < this->inventory.Size() && item == nullptr; j++)
	{
		Item* _item = this->inventory.At(j);
		
		if (_item && _item->name == itemName)
		{
			if (i)
			{
				*i = j;
			}
			
			item = _item;
			break;
		}
	}
	
	return item;
}

void Inventoried::RemoveItem(size_t i)
{
	Item* item = GetItem(i);
	item->OnDrop();
	
	this->inventory.ClearItem(i);
}

void Inventoried::RemoveItem(Item* item)
{
	size_t idx = this->inventory.IndexOf(item);
	
	if (idx >= 0)
	{
		this->RemoveItem(idx);
	}
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

void Equipped::SetSlot(int slot, Item* item)
{
	switch (slot)
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


Item* Equipped::GetSlot(int slot)
{
	Item* item;
	
	switch (slot)
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