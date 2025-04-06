#pragma once

#include "Event.h"
#include "SdlTexture.h"

#include "Entity.h"
#include "Containers.h"
#include "EntityTexture.h"

class Item:
	public EntityTexture
{
public:
	static const int SPRITE_SIZE = 64;

	std::string name;
	
	Item();
	virtual bool Use();
	
	virtual void OnPickUp();
	virtual void OnDrop();
};

class Bread: public Item
{
public:
	Bread();
	virtual bool Use();
};

class Armor: public Item
{
public:
	Armor();
	virtual bool Use();
	
	virtual void OnPickUp();
	virtual void OnDrop();
};

class Ring: public Item
{
public:
	Ring();
	virtual bool Use();
	
	virtual void OnPickUp();
	virtual void OnDrop();
};

class OreItem: public Item
{
public:
	OreItem();
};

class Chest: public Item
{
public:
	Chest();
	virtual bool Use();
};

class Inventory:
	public Containers::Vector<Item*>
{
	size_t itemCount;
public:
	static const int MAX_ITEMS = 20;
	
	Inventory();
	
	bool PushItem(Item* item);
	bool HasSpace();
	void ClearItem(size_t i);
};

class Inventoried
{
public:
	Inventory inventory;
	
	void Use(size_t i);
	void AddItem(Item* item);
	Item* GetItem(size_t i);
	Item* GetItem(const std::string& itemName, size_t* i = nullptr);
	void RemoveItem(size_t i);
	void RemoveItem(Item* item);
};

class Equipped
{
	Item* head;
	Item* body;
	Item* legs;
	Item* feet;
	Item* mainHand;
	Item* offHand;
public:
	enum
	{
		HEAD,
		BODY,
		LEGS,
		FEET,
		MAIN_HAND,
		OFF_HAND,
		NUM_SLOTS
	};
	
	Equipped();
	void SetSlot(int slot, Item* item);
	Item* GetSlot(int slot);
};