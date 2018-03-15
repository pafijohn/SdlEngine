#include "Text.h"
#include "TextManager.h"
#include "TextureManager.h"

#include <cstdio>
#include <algorithm>

Text::Text()
{
	this->background = NONE;
}

Text::~Text()
{
	this->Clear();
}

void Text::Clear()
{
	auto it = this->chunks.begin();
	
	for ( ;it != this->chunks.end(); it++ )
	{
		delete *it;
	}
	
	this->chunks.clear();
}

void Text::SetBackground( int background )
{
	this->background = background;
}

bool Text::Update()
{
	return true;
}

void Text::Render()
{
	auto it = this->chunks.begin();
	SDL_Point pos = this->GetPos();
	
	int _x = pos.x;
	int _y = pos.y;
	
	SdlTexture texture;
	texture.Load( "res\\TextBackgrounds.png" );
	texture.isRelative = false;
	
	for ( ;it != this->chunks.end(); it++ )
	{
		int width;
		int heighth;
		
		TextManager::GetDimentions( ( *it )->text, ( *it )->identifier, &width, &heighth );
		
		if ( this->background != NONE )
		{
			texture.Crop( 0, 49 * this->background, 190, 49 );
			texture.Scale( width + 20, heighth );
			texture.Move( _x - 10, _y );
			texture.Render();
		}
		
		TextManager::RenderText( ( *it )->text, ( *it )->identifier, _x, _y );
		
		_y += heighth;
	}
}
	
void Text::AddText( const std::string& text, const std::string& identifier )
{
	TextChunk* chunk = new TextChunk();
	
	chunk->text = text;
	chunk->identifier = identifier;
	
	this->chunks.push_back( chunk );
}
