#include "Text.h"
#include "Utils.h"
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
	this->chunks.clear();
}

void Text::SetBackground(int background)
{
	this->background = background;
}

void Text::GetRect(SdlRect& rect)
{
	int w = 0;
	int h = 0;
	SDL_Point pos = this->GetPos();
	
	rect.x = pos.x;
	rect.y = pos.y;
	
	for (auto it = this->chunks.begin(); it != this->chunks.end(); it++)
	{
		int width;
		int heighth;
		
		TextManager::GetDimentions((*it)->text, (*it)->identifier, &width, &heighth);
		h += heighth;
		w = MAX(w, width);
	}
	
	rect.h = h;
	rect.w = w;
}

bool Text::Update()
{
	return true;
}

void Text::Render()
{
	SDL_Point pos = this->GetPos();
	
	int _x = pos.x;
	int _y = pos.y;
	
	SdlTexture texture;
	texture.Load("resources\\TextBackgrounds.png");
	texture.isRelative = false;
	
	for (auto it = this->chunks.rbegin(); it != this->chunks.rend(); it++)
	{
		int width;
		int heighth;
		
		TextManager::GetDimentions((*it)->text, (*it)->identifier, &width, &heighth);
		
		if (this->background != NONE)
		{
			texture.Crop(0, 49 * this->background, 190, 49);
			texture.Scale(width + 20, heighth);
			texture.Move(_x - 10, _y);
			texture.Render();
		}
		
		TextManager::RenderText((*it)->text, (*it)->identifier, _x, _y);
		_y -= heighth;
	}
}

void Text::AddText(const std::string& text, const std::string& identifier)
{
	TextChunk* chunk = new TextChunk();
	
	chunk->text = text;
	chunk->identifier = identifier;
	
	this->chunks.push_back(chunk);
}
