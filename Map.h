#pragma once

#include "Tile.h"
#include "Timer.h"
#include "Layers.h"
#include "Renderable.h"
#include "Positionable.h"

#include <SDL.h>
#include <vector>
#include <string>

class MapEntity: public Renderable
{
	int layer;
public:
	void SetLayer( int i );
	virtual bool Update();
	virtual void Render();
};

class Map
{
	static int chunkX;
	static int chunkY;
public:
	
	enum
	{
		BACKGROUND,
		BACKGROUND_2,
		LEVEL,
		ABOVE,
		WEATHER,
		HUD,
		NUM_LAYERS
	};
	
	static std::vector< std::vector< Tile* >* > tiles[ NUM_LAYERS ];
	
	static void Init();
	static void LoadLayer( const std::string& mapFile, int layer, int width, int height );
	
	static bool HasCollision( SdlRect& other );
	static SDL_Point GetPosition( const SDL_Point& point, int direction, float distance );
	static Tile* GetTile( int layer, int x, int y );
	static Tile* GetTile( const SDL_Point& point, int direction, float distance );
	static bool PutTile( Tile* tile, int layer, int x, int y, bool overwrite = false );
	static void GetMapSize( int& w, int& h );
	
	static void GetCollision( SdlRect& other, std::vector< bool >& collided, std::vector< Tile* >& collidedTiles );
	
	static int GetCollisionId( SdlRect& other );
	
	static void UpdateLayer( int layer );
	static void RenderLayer( int layer );
};
