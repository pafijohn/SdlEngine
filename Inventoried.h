#pragma once

#include "Event.h"
#include "SdlTexture.h"

#include <vector>

class Item: public SdlTexture
{
public:
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

// Inherited so as to have swap
class Inventory: public std::vector<Item*>
{
	int itemCount;
public:
	Inventory();
	
	bool PushBack( Item* item );
	bool HasSpace();
	void ClearItem( int i );
};

class Inventoried
{
public:
	Inventory inventory;
	
	void Use( int i );
	void AddItem( Item* item );
	Item* GetItem( int i );
	Item* GetItem( const std::string& itemName, int* i = nullptr );
	void RemoveItem( int i );
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
	void SetSlot( int slot, Item* item );
	Item* GetSlot( int slot );
};