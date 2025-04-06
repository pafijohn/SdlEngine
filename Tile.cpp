#include "Tile.h"
#include "Utils.h"
#include "Character.h"
#include "SdlGlobals.h"
#include "MixerManager.h"

Tile::Tile()
{
	this->Load("resources\\terrain.png");
	this->SetId(-1);
	this->Init();
}

void Tile::Init()
{
	int xIdx = this->id % SPRITE_SIZE;
	int yIdx = this->id / SPRITE_SIZE;
	
	xIdx *= SPRITE_SIZE;
	yIdx *= SPRITE_SIZE;
	
	this->Crop(xIdx, yIdx, SPRITE_SIZE, SPRITE_SIZE);
	this->Scale(RENDER_SIZE, RENDER_SIZE);
}

void Tile::SetId(int id)
{
	this->id = id;
}

void Tile::OnInteract()
{
}

Wheat::Wheat()
{
	this->toggle = true;
	this->timer.Start(0);
	this->available = true;
}

void Wheat::OnInteract()
{
	bool canPickup = character->inventory.HasSpace() && this->available;
	
	if (canPickup)
	{
		if (this->toggle)
		{
			playSound("sounds\\knifeSlice.wav");
		}
		else
		{
			playSound("sounds\\knifeSlice2.wav");
		}
		
		character->AddItem(new Bread());
		this->toggle = !this->toggle;
		this->timer.Start(10000);
		this->available = false;
		this->SetHasCollision(false);
	}
}

bool Wheat::Update()
{
	if (!this->available && this->timer.IsExpired())
	{
		this->available = true;
		this->SetHasCollision(true);
	}

	return true;
}

void Wheat::Render()
{
	if (!this->available)
	{
		this->SetAlpha(0x80);
	}
	
	Tile::Render();
	
	this->SetAlpha(0xFF);
}

Ore::Ore()
{
	this->timer.Start(0);
	this->available = true;
}

void Ore::OnInteract()
{
	if (this->available)
	{
		character->AddItem(new OreItem());
		this->timer.Start(10000);
		this->available = false;
	}
}

bool Ore::Update()
{
	if (!this->available && this->timer.IsExpired())
	{
		this->available = true;
	}
	
	return true;
}

void Ore::Render()
{
	if (!this->available)
	{
		this->SetAlpha(0x80);
	}
	
	Tile::Render();
	
	this->SetAlpha(0xFF);
}

ChestTile::ChestTile()
{
	this->SetHasCollision(true);
	this->SetId(590);
	this->Init();
	
	// TODO
	// initializing here causes crash for some reason
	// not sure why but its ok to initialize it on the 
	// first interaction since the displaying of the
	// intentory does not prevent the existance of it
	this->inventoryDisplay = nullptr;
	
	this->AddItem(new Bread());
}

bool ChestTile::Update()
{	
	return true;
}

void ChestTile::OnInteract()
{
	if (this->inventoryDisplay.IsNull())
	{
		this->inventoryDisplay = new InventoryDisplay(*this);
	}
	
	this->inventoryDisplay->SetAsActiveConsumer();
	this->inventoryDisplay->Show();
}

void ChestTile::Render()
{
	Tile::Render();
	
	//if (!this->inventoryDisplay.IsNull())
	if (this->inventoryDisplay)
	{
		this->inventoryDisplay->Render();
	}
}