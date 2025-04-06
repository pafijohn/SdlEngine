#include <map>
#include <SDL_Image.h>
#include <cstdio>
#include <iostream>

#include "SdlGlobals.h"
#include "TextureManager.h"
#include "Utils.h"
#include "SdlRect.h"

static std::map<std::string, SDL_Surface*> surfaces;
static std::map<std::string, SDL_Texture*> textures;

void TextureManager::Preload()
{
	const char* files[] = {
		"resources\\terrain.png",
		"resources\\CharacterSpriteSheet.png",
		"resources\\Armor.png",
		"resources\\clouds.png",
		"resources\\Arrow.png",
		"resources\\TextBackgrounds.png",
		"resources\\ItemPanel.png",
		"resources\\inventory.png",
		"resources\\items\\armor.png",
		"resources\\items\\bread.png",
		"resources\\items\\ring.png",
		"resources\\items\\chest.png",
		"resources\\Items\\backpack.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\tanned2.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\tanned.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\skeleton.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\red_orc.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\orc.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\light.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\darkelf2.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\darkelf.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\dark2.png",
		//"resources\\Universal-LPC-spritesheet-master\\body\\male\\dark.png",
	};
	
	for (size_t i = 0; i < lengthof(files); i++)
	{
		std::string filename = files[ i ];
		SDL_Surface* surface = IMG_Load(files[ i ]);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		
		if (surface == nullptr)
		{
			printf("IMG_Load Error: %s: %s\n", IMG_GetError(), filename.data());
		}
		
		surfaces[ filename ] = surface;
		textures[ filename ] = texture;
	}
}

SDL_Surface* TextureManager::GetSurface(const std::string& surfaceName)
{
	SDL_Surface* surface;
	
	if (surfaces.count(surfaceName))
	{
		surface = surfaces[ surfaceName ];
	}
	else
	{
		surface = nullptr;
	}
	
	return surface;
}

SDL_Texture* TextureManager::GetTexture(const std::string& textureName)
{
	SDL_Texture* texture;
	
	if (textures.count(textureName))
	{
		texture = textures[ textureName ];
	}
	else
	{
		texture = nullptr;
	}
	
	return texture;
}

void TextureManager::RenderSolid(const SdlColor& color, SdlRect& rect, SDL_BlendMode blendMode)
{
	SdlColor orig;
	SDL_BlendMode origBlend;
	
	SDL_GetRenderDrawBlendMode(renderer, &origBlend);
	
	SDL_SetRenderDrawBlendMode(renderer, blendMode);
	
	// Get the draw color
	SDL_GetRenderDrawColor(renderer, &orig.r, &orig.g, &orig.b, &orig.a);
	
	// Set the draw color
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	
	// Draw the rect
	SDL_RenderFillRect(renderer, &rect);
	
	// Restore the original color
	SDL_SetRenderDrawColor(renderer, orig.r, orig.g, orig.b, orig.a);
	
	SDL_SetRenderDrawBlendMode(renderer, origBlend);
}
