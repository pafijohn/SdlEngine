#pragma once

#include "Tile.h"
#include "Timer.h"
#include "Layers.h"
#include "QuadTree.h"
#include "Pointers.h"
#include "Renderable.h"
#include "Positionable.h"

#include <SDL.h>
#include <vector>
#include <string>

class MapEntity: public Renderable
{
	int layer;
public:
	void SetLayer(int i);
	virtual bool Update();
	virtual void Render();
};

class Map
{
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
		
	static std::vector< std::vector< Pointers::SmartPointer< Tile > >* > tiles[ NUM_LAYERS ];
	
	static void Init();
	static void LoadLayer(const std::string& mapFile, int layer, int width, int height);
	
	// Entity Based
	static void PushEntity(Entity& entity);
	static void PopEntity(Entity& entity);
	static bool CheckCollision(Entity& entity, EntitySet& collisions);
	// End of Entity Based
	
	static SDL_Point GetPosition(const SDL_Point& point, int direction, float distance);
	static Tile* GetTile(size_t layer, size_t x, size_t y);
	static Tile* GetTile(const SDL_Point& point, int direction, float distance);
	static bool PutTile(Tile* tile, size_t layer, size_t x, size_t y, bool overwrite = false);
	static void GetMapSize(int& w, int& h);
	
	static void UpdateLayer(int layer);
	static void RenderLayer(int layer);
};
