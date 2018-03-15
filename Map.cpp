#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

#include "Map.h"
#include "Utils.h"
#include "Directions.h"
#include "SdlGlobals.h"
#include "TextureManager.h"
#include "CharacterTexture.h"

std::vector< std::vector< Tile* >* > Map::tiles[ NUM_LAYERS ];

static MapEntity mapEntities[ Map::WEATHER ];

void Map::Init()
{
	int width;
	int height;
	
	char buff[ 256 ];
	
	std::ifstream metadata( "res\\Map_Meta.txt" );
	metadata.getline( buff, sizeof( buff ) );
	
	sscanf( buff, "%d,%d", &width, &height );
	
	std::string files[] = {
		"res\\Map_Background.csv.txt",
		"res\\Map_Background_2.csv.txt",
		"res\\Map_Level.csv.txt",
		"res\\Map_Above.csv.txt",
	};
	
	for ( int i = 0; i < lengthof( files ); i++ )
	{
		LoadLayer( files[ i ], i, width, height );
	}
	
	for ( int i = 0; i < Map::WEATHER; i++ )
	{
		mapEntities[ i ].SetLayer( i );
	}
}

void Map::LoadLayer( const std::string& mapFile, int layer, int width, int height )
{
	int w;
	int h;
	
	SdlGlobals::GetScreenDimentions( w, h );
	std::ifstream map( mapFile );
	
	unsigned long long counter = 0;
	
	for ( int i = 0; i < height; i++ )
	{
		std::vector< Tile* >* line = new std::vector< Tile* >();
		
		tiles[ layer ].push_back( line );
		
		for ( int j = 0; j < width; j++ )
		{
			int id;
			Tile* tile;
			
			map.read( ( char* )&id, sizeof( id ) );
			
			switch ( id )
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
			
			if ( tile )
			{
				int xPos = ( counter % width ) * Tile::RENDER_SIZE;
				int yPos = ( counter / height ) * Tile::RENDER_SIZE;
				
				tile->SetId( id );
				tile->Move( xPos, yPos );
				tile->Init();
			}
			
			line->push_back( tile );
			
			counter++;
		}
	}
}

bool Map::HasCollision( SdlRect& other )
{
	int width;
	int height;
	
	SdlGlobals::GetScreenDimentions( width, height );
	
	bool collided = ( other.x < 0 or other.y < 0 );
	
	if ( !collided )
	{
		std::vector< bool > hasCollisions;
		std::vector< Tile* > temp;
		
		Map::GetCollision( other, hasCollisions, temp );
		
		for ( auto it = hasCollisions.begin(); it != hasCollisions.end() and !collided; it++ )
		{
			collided = ( *it );
		}
	}
	
	return collided;
}

SDL_Point Map::GetPosition( const SDL_Point& point, int direction, float distance )
{
	const int amount = Tile::RENDER_SIZE * distance;
	SDL_Point proxy = point;
	SDL_Point output;
	
	switch ( direction )
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

Tile* Map::GetTile( int layer, int x, int y )
{
	Tile* val;
	
	if ( ( layer >= 0 and layer < NUM_LAYERS ) and ( y >= 0 and y < Map::tiles[ layer ].size() ) and ( x >= 0 and x < Map::tiles[ layer ].at( y )->size() ) )
	{
		val = Map::tiles[ layer ].at( y )->at( x );
	}
	else
	{
		val = nullptr;
	}
	
	return val;
}

Tile* Map::GetTile( const SDL_Point& point, int direction, float distance )
{
	SDL_Point mapPos = Map::GetPosition( point, direction, distance );
	
	return Map::GetTile( LEVEL, mapPos.x, mapPos.y );
}

bool Map::PutTile( Tile* tile, int layer, int x, int y, bool overwrite )
{
	int xPos;
	int yPos;
	
	// TODO Should check bounds, and if tile is not nullptr
	Tile* existingTile = Map::GetTile( layer, x, y );
	
	bool placeable = ( existingTile && overwrite ) || ( existingTile == nullptr );
	
	if ( placeable )
	{
		if ( existingTile )
		{
			existingTile->GetLocation( &xPos, &yPos );
			delete existingTile;
			
			Map::tiles[ layer ].at( y )->at( x ) = tile;
		}
		else
		{
			xPos = x * Tile::RENDER_SIZE;
			yPos = y * Tile::RENDER_SIZE;
			
			Map::tiles[ layer ].at( y )->at( x ) = tile;
		}
		
		if ( tile )
		{
			tile->Move( xPos, yPos );			
		}
	}
	
	return placeable;
}

void Map::GetMapSize( int& w, int& h )
{
	int maxH = tiles[ BACKGROUND ].size();
	int maxW = tiles[ BACKGROUND ].at( maxH - 1 )->size();
	
	maxW *= Tile::RENDER_SIZE;
	maxH *= Tile::RENDER_SIZE;
	
	w = maxW;
	h = maxH;
}

void Map::GetCollision( SdlRect& other, std::vector< bool >& collided, std::vector< Tile* >& collidedTiles )
{
	Tile* proxy;
	
	int left_ = other.x / Tile::RENDER_SIZE;
	int right_ = ( other.x + other.w ) / Tile::RENDER_SIZE;
	int top_ = other.y / Tile::RENDER_SIZE;
	int bot_ = ( other.y + other.h ) / Tile::RENDER_SIZE;
	
	Tile* tileCollisions[ 4 ] = { 0, 0, 0, 0 };
	bool areTilesColliding[ 4 ] = { true, true, true, true };
	
	tileCollisions[ 0 ] = Map::GetTile( LEVEL, left_, top_ );
	tileCollisions[ 1 ] = Map::GetTile( LEVEL, right_, top_ );
	tileCollisions[ 2 ] = Map::GetTile( LEVEL, left_, bot_ );
	tileCollisions[ 3 ] = Map::GetTile( LEVEL, right_, bot_ );
	
	for ( int i = 0; i < 4; i++ )
	{
		Tile* temp = tileCollisions[ i ];
		areTilesColliding[ i ] = ( temp and !temp->IsPassible() );
	}
	
	for ( int i = 0; i < 4; i++ )
	{
		collidedTiles.push_back( tileCollisions[ i ] );
		collided.push_back( areTilesColliding[ i ] );
	}
}

int Map::GetCollisionId( SdlRect& other )
{
	int mostMaterial = -1;
	int mostMaterialAmount = 0;
	
	std::map< int, int > amounts;
	std::vector< bool > hasCollisions;
	std::vector< Tile* > collisions;
	
	Map::GetCollision( other, hasCollisions, collisions );
	
	if ( hasCollisions.size() == 4 and collisions.size() == 4 )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( hasCollisions[ i ] )
			{
				Tile* tile = collisions[ i ];
				if ( tile )
				{
					int id = tile->id;
					
					if ( amounts.count( id ) )
					{
						amounts[ id ]++;
					}
					else
					{
						amounts[ id ] = 1;
					}
				}
			}
		}
	}
	
	for ( auto it = amounts.begin(); it != amounts.end(); it++ )
	{
		if ( it->second > mostMaterialAmount )
		{
			mostMaterialAmount = it->second;
			mostMaterial = it->first;
		}
	}
	
	return mostMaterial;
}

void Map::UpdateLayer( int layer )
{
	int w;
	int h;
	double dw;
	double dh;
	
	int tilesPerX;
	int tilesPerY;
	
	SDL_Point proxy = camera.GetPos();
	SdlGlobals::GetScreenDimentions( w, h );
	
	dw = w;
	dh = h;
	
	tilesPerX = ( dh / Tile::RENDER_SIZE ) + 3;
	tilesPerY = ( dw / Tile::RENDER_SIZE ) + 3;
	
	proxy.x -= Tile::RENDER_SIZE;
	proxy.y -= Tile::RENDER_SIZE;
	
	int origJ = proxy.x / Tile::RENDER_SIZE;
	int origK = proxy.y / Tile::RENDER_SIZE;
	
	origJ = std::max( origJ, 0 );
	origK = std::max( origK, 0 );
	
	int jj = origJ + tilesPerY;
	int kk = origK + tilesPerX;
	
	for ( int j = origJ; j < jj; j++ )
	{
		for ( int k = origK; k < kk; k++ )
		{
			Tile* tile = Map::GetTile( layer, j, k );
			
			if ( tile )
			{
				tile->Update();
			}
		}
	}
}

void Map::RenderLayer( int layer )
{
	int w;
	int h;
	double dw;
	double dh;
	
	int tilesPerX;
	int tilesPerY;
	
	SDL_Point proxy = camera.GetPos();
	SdlGlobals::GetScreenDimentions( w, h );
	
	dw = w;
	dh = h;
	
	tilesPerX = ( dh / Tile::RENDER_SIZE ) + 3;
	tilesPerY = ( dw / Tile::RENDER_SIZE ) + 3;
	
	proxy.x -= Tile::RENDER_SIZE;
	proxy.y -= Tile::RENDER_SIZE;
	
	int origJ = proxy.x / Tile::RENDER_SIZE;
	int origK = proxy.y / Tile::RENDER_SIZE;
	
	origJ = std::max( origJ, 0 );
	origK = std::max( origK, 0 );
	
	int jj = origJ + tilesPerY;
	int kk = origK + tilesPerX;
	
	for ( int j = origJ; j < jj; j++ )
	{
		for ( int k = origK; k < kk; k++ )
		{
			Tile* tile = Map::GetTile( layer, j, k );
			
			if ( tile )
			{
				tile->Render();
			}
		}
	}
}

void MapEntity::SetLayer( int i )
{
	this->layer = i;
	Layers::AddToLayer( this, this->layer );
}

bool MapEntity::Update()
{
	Map::UpdateLayer( this->layer );
	return true;
}

void MapEntity::Render()
{
	Map::RenderLayer( this->layer );
}
