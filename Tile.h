#pragma once

#include "Timer.h"
#include "Entity.h"
#include "Pointers.h"
#include "SdlTexture.h"
#include "Renderable.h"
#include "Inventoried.h"
#include "Displayable.h"
#include "EventConsumer.h"
#include "InventoryDisplay.h"

using namespace Pointers;

class Tile:
	public SdlTexture,
	public Entity
{
public:
	int id;
	
	const static int SPRITE_SIZE = 32;
	const static int RENDER_SIZE = 64;
	
	Tile();
	
	virtual void SetId(int id);
	virtual void Init();
	virtual void OnInteract();
};

class Wheat:
	public Tile
{
	bool toggle;
	Timer timer;
	
	bool available;
public:
	Wheat();
	virtual void OnInteract();
	
	virtual bool Update();
	virtual void Render();
};

class Ore:
	public Tile
{
	bool available;
	Timer timer;
public:
	Ore();
	virtual void OnInteract();
	
	virtual bool Update();
	virtual void Render();
};

class ChestTile:
	public Tile,
	public Inventoried
{
	SP<InventoryDisplay> inventoryDisplay;
public:
	ChestTile();
	virtual void OnInteract();
	
	virtual bool Update();
	virtual void Render();
};