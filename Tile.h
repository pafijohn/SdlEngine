#pragma once

#import "Timer.h"
#import "SdlTexture.h"
#import "Renderable.h"
#import "Inventoried.h"
#import "Displayable.h"

class Tile: public SdlTexture
{
protected:
	bool isPassible;
public:
	int id;
	
	const static int SPRITE_SIZE = 32;
	const static int RENDER_SIZE = 64;
	
	Tile();
	
	virtual bool IsPassible();
	virtual void SetId( int id );
	virtual void Init();
	virtual void OnInteract();
};

class Wheat: public Tile
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

class Ore: public Tile
{
	bool available;
	Timer timer;
public:
	Ore();
	virtual void OnInteract();
	
	virtual bool Update();
	virtual void Render();
};

class ChestTile: public Tile, public Inventoried, public Displayable
{
public:
	ChestTile();
	
	virtual bool Update();	
	virtual void OnInteract();
};