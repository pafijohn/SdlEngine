#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

#include "Map.h"
#include "Utils.h"
#include "SdlRect.h"
#include "Directions.h"
#include "SdlGlobals.h"
#include "TextureManager.h"
#include "CollisionLayers.h"
#include "CharacterTexture.h"

std::vector< std::vector< Pointers::SmartPointer< Tile > >* > Map::tiles[NUM_LAYERS];

static MapEntity mapEntities[Map::WEATHER];

void Map::Init()
{
	int width;
	int height;
	
	char buff[256];
	
	std::ifstream metadata("resources\\Map_Meta.txt");
	metadata.getline(buff, sizeof(buff));
	
	sscanf_s(buff, "%d,%d", &width, &height);
	
	std::string files[] = {
		"resources\\Map_Background.csv.txt",
		"resources\\Map_Background_2.csv.txt",
		"resources\\Map_Level.csv.txt",
		"resources\\Map_Above.csv.txt",
	};
	
	SdlRect mapRect;
	mapRect.Move(0, 0);
	mapRect.Scale(width * Tile::RENDER_SIZE, height * Tile::RENDER_SIZE);
	CollisionLayers::CreateLayer("Map");
	auto& collision = CollisionLayers::GetLayer("Map");
	collision.SetRect(mapRect);
	
	for (size_t i = 0; i < lengthof(files); i++)
	{
		LoadLayer(files[i], i, width, height);
	}
	
	for (size_t i = 0; i < Map::WEATHER; i++)
	{
		mapEntities[i].SetLayer(i);
	}
}

void Map::LoadLayer(const std::string& mapFile, int layer, int width, int height)
{
	int w;
	int h;
	
	SdlGlobals::GetScreenDimentions(w, h);
	std::ifstream map(mapFile);
	
	unsigned long long counter = 0;
	
	for (int i = 0; i < height; i++)
	{
		std::vector< Pointers::SmartPointer< Tile > >* line = new std::vector< Pointers::SmartPointer< Tile > >();
		
		tiles[layer].push_back(line);
		
		for (int j = 0; j < width; j++)
		{
			int id;
			Tile* tile;
			
			map.read((char*)&id, sizeof(id));
			
			switch (id)
			{
				case -1:
				{
					tile = nullptr;
					break;
				}
				case 181:
				case 182:
				case 183:
				{
					tile = new Ore();
					break;
				}
				case 271:
				case 272:
				case 273:
				case 303:
				case 304:
				case 305:
				case 335:
				case 336:
				case 337:
				{
					tile = new Wheat();
					break;
				}
				default:
				{
					tile = new Tile();
					break;
				}
			}
			
			if (tile)
			{
				int xPos = (counter % width) * Tile::RENDER_SIZE;
				int yPos = (counter / height) * Tile::RENDER_SIZE;
				
				tile->SetId(id);
				tile->Move(xPos, yPos);
				tile->Init();
				
				if (layer == Map::LEVEL)
				{
					SdlRect collision(xPos, yPos, Tile::RENDER_SIZE, Tile::RENDER_SIZE);
					tile->SetCollision(collision);
					tile->SetHasCollision(true);
					Map::PushEntity(*tile);
				}
			}
			
			line->push_back(Pointers::SmartPointer< Tile >(tile));
			
			counter++;
		}
	}
}

SDL_Point Map::GetPosition(const SDL_Point& point, int direction, float distance)
{
	const int amount = Tile::RENDER_SIZE * distance;
	SDL_Point proxy = point;
	SDL_Point output;
	
	switch (direction)
	{
		case Directions::LEFT:
		{
			proxy.x -= amount;
			break;
		}
		case Directions::RIGHT:
		{
			proxy.x += amount;
			break;
		}
		case Directions::FORWARD:
		{
			proxy.y -= amount;
			break;
		}
		case Directions::DOWN:
		{
			proxy.y += amount;
			break;
		}
	}
	
	output.x = proxy.x / Tile::RENDER_SIZE;
	output.y = proxy.y / Tile::RENDER_SIZE;
	
	return output;
}

Tile* Map::GetTile(size_t layer, size_t x, size_t y)
{
	Tile* val;
	
	if ((layer >= 0 && layer < NUM_LAYERS) && (y >= 0 && y < Map::tiles[layer].size()) && (x >= 0 && x < Map::tiles[layer].at(y)->size()))
	{
		val = Map::tiles[layer].at(y)->at(x).Get();
	}
	else
	{
		val = nullptr;
	}
	
	return val;
}

Tile* Map::GetTile(const SDL_Point& point, int direction, float distance)
{
	SDL_Point mapPos = Map::GetPosition(point, direction, distance);
	
	return Map::GetTile(LEVEL, mapPos.x, mapPos.y);
}

void Map::PushEntity(Entity& entity)
{
	auto& collision = CollisionLayers::GetLayer("Map");
	collision.Insert(entity);
}

void Map::PopEntity(Entity& entity)
{
	auto& collision = CollisionLayers::GetLayer("Map");
	collision.Remove(entity);
}

bool Map::CheckCollision(Entity& entity, EntitySet& collisions)
{
	auto& collision = CollisionLayers::GetLayer("Map");
	SdlRect mapRect = collision.GetRect();
	bool edgeCollide = !mapRect.Contains(entity.GetCollision());
	bool ordinaryCollision = collision.CheckCollision(entity, collisions);
	
	return edgeCollide || ordinaryCollision;
}

bool Map::PutTile(Tile* tile, size_t layer, size_t x, size_t y, bool overwrite)
{
	int xPos;
	int yPos;
	
	// TODO Should check bounds, and if tile is not nullptr
	Tile* existingTile = Map::GetTile(layer, x, y);
	
	bool placeable = (existingTile && overwrite) || (existingTile == nullptr);
	
	if (placeable)
	{
		if (existingTile)
		{
			EntityRegistry* registry = EntityRegistry::GetInst();
			
			existingTile->GetLocation(&xPos, &yPos);
			PopEntity(*existingTile);
			registry->Unregister(*existingTile);
			delete existingTile;
			
			Map::tiles[layer].at(y)->at(x) = tile;
		}
		else
		{
			xPos = x * Tile::RENDER_SIZE;
			yPos = y * Tile::RENDER_SIZE;
			
			Map::tiles[layer].at(y)->at(x) = tile;
		}
		
		if (tile)
		{
			SdlRect collision(xPos, yPos, Tile::RENDER_SIZE, Tile::RENDER_SIZE);
			tile->SetCollision(collision);
			tile->SetHasCollision(true);
			
			PushEntity(*tile);
			tile->Move(xPos, yPos);			
		}
	}
	
	return placeable;
}

void Map::GetMapSize(int& w, int& h)
{
	int maxH = tiles[BACKGROUND].size();
	int maxW = tiles[BACKGROUND].at(maxH - 1)->size();
	
	maxW *= Tile::RENDER_SIZE;
	maxH *= Tile::RENDER_SIZE;
	
	w = maxW;
	h = maxH;
}

void Map::UpdateLayer(int layer)
{
	int w;
	int h;
	
	int tilesPerX;
	int tilesPerY;
	
	SDL_Point proxy = camera.GetPos();
	SdlGlobals::GetScreenDimentions(w, h);
	
	tilesPerX = ((double)h / Tile::RENDER_SIZE) + 3;
	tilesPerY = ((double)w / Tile::RENDER_SIZE) + 3;
	
	proxy.x -= Tile::RENDER_SIZE;
	proxy.y -= Tile::RENDER_SIZE;
	
	int origI = proxy.x / Tile::RENDER_SIZE;
	int origJ = proxy.y / Tile::RENDER_SIZE;
	
	origI = std::max(origI, 0);
	origJ = std::max(origJ, 0);
	
	int ii = origI + tilesPerY;
	int jj = origJ + tilesPerX;
	
	for (int i = origI; i < ii; i++)
	{
		for (int j = origJ; j < jj; j++)
		{
			Tile* tile = Map::GetTile(layer, i, j);
			
			if (tile)
			{
				tile->Update();
			}
		}
	}
}

void Map::RenderLayer(int layer)
{
	int w;
	int h;
	double dw;
	double dh;
	
	int tilesPerX;
	int tilesPerY;
	
	SDL_Point proxy = camera.GetPos();
	SdlGlobals::GetScreenDimentions(w, h);
	
	dw = w;
	dh = h;
	
	tilesPerX = (dh / Tile::RENDER_SIZE) + 3;
	tilesPerY = (dw / Tile::RENDER_SIZE) + 3;
	
	proxy.x -= Tile::RENDER_SIZE;
	proxy.y -= Tile::RENDER_SIZE;
	
	int origJ = proxy.x / Tile::RENDER_SIZE;
	int origK = proxy.y / Tile::RENDER_SIZE;
	
	origJ = std::max(origJ, 0);
	origK = std::max(origK, 0);
	
	int jj = origJ + tilesPerY;
	int kk = origK + tilesPerX;
	
	for (int j = origJ; j < jj; j++)
	{
		for (int k = origK; k < kk; k++)
		{
			Tile* tile = Map::GetTile(layer, j, k);
			
			if (tile)
			{
				tile->Render();
			}
		}
	}
}

void MapEntity::SetLayer(int i)
{
	this->layer = i;
	Layers::AddToLayer(this, this->layer);
}

bool MapEntity::Update()
{
	Map::UpdateLayer(this->layer);
	return true;
}

void MapEntity::Render()
{
	Map::RenderLayer(this->layer);
}
