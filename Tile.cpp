#include "Tile.h"
#include "Utils.h"
#include "Character.h"
#include "SdlGlobals.h"
#include "MixerManager.h"

Tile::Tile()
{
	this->Load( "res\\terrain.png" );
	this->SetId( -1 );
	this->Init();
	this->isPassible = false;
}

bool Tile::IsPassible()
{
	return this->isPassible;
}

void Tile::Init()
{
	int xIdx = this->id % SPRITE_SIZE;
	int yIdx = this->id / SPRITE_SIZE;
	
	xIdx *= SPRITE_SIZE;
	yIdx *= SPRITE_SIZE;
	
	this->Crop( xIdx, yIdx, SPRITE_SIZE, SPRITE_SIZE );
	this->Scale( RENDER_SIZE, RENDER_SIZE );
}

void Tile::SetId( int id )
{
	this->id = id;
}

void Tile::OnInteract()
{
}

Wheat::Wheat()
{
	this->toggle = true;
	this->timer.Start( 0 );
	this->available = true;
	this->isPassible = false;
}

void Wheat::OnInteract()
{
	bool canPickup = character->inventory.HasSpace() && this->available;
	
	if ( canPickup )
	{
		if ( this->toggle )
		{
			playSound( "sounds\\knifeSlice.wav" );
		}
		else
		{
			playSound( "sounds\\knifeSlice2.wav" );
		}
		
		character->AddItem( new Bread() );
		this->toggle = !this->toggle;
		this->timer.Start( 10000 );
		this->available = false;
		this->isPassible = true;
	}
}

bool Wheat::Update()
{
	if ( !this->available and this->timer.IsExpired() )
	{
		this->available = true;
	}
	
	if ( this->available and this->isPassible )
	{
		SdlRect charColl;
		character->GetCollisionProxy( charColl );
		
		if ( !this->renderRect.CollidesWith( charColl ) )
		{
			this->isPassible = false;
		}
	}
	
	return true;
}

void Wheat::Render()
{
	if ( !this->available )
	{
		this->SetAlpha( 0x80 );
	}
	
	Tile::Render();
	
	this->SetAlpha( 0xFF );
}

Ore::Ore()
{
	this->timer.Start( 0 );
	this->available = true;
}

void Ore::OnInteract()
{
	if ( this->available )
	{
		character->AddItem( new OreItem() );
		this->timer.Start( 10000 );
		this->available = false;
	}
}

bool Ore::Update()
{
	if ( !this->available and this->timer.IsExpired() )
	{
		this->available = true;
	}
	
	return true;
}

void Ore::Render()
{
	if ( !this->available )
	{
		this->SetAlpha( 0x80 );
	}
	
	Tile::Render();
	
	this->SetAlpha( 0xFF );
}

ChestTile::ChestTile()
{
	this->isPassible = true;
	this->SetId( 590 );
	this->Init();
}

bool ChestTile::Update()
{
	if ( this->isPassible )
	{
		SdlRect temp;
		character->GetCollisionProxy( temp );
		
		if ( !this->renderRect.CollidesWith( temp ) )
		{
			this->isPassible = false;
		}
	}
	
	return true;
}

void ChestTile::OnInteract()
{
	// TODO display an inventory
}