#pragma once
#include <string>

#include "SdlTexture.h"

class TextureManager
{
public:
	static void Preload();
	
	static SDL_Surface* GetSurface( const std::string& surfaceName );
	static SDL_Texture* GetTexture( const std::string& textureName );
	
	static void RenderSolid( const SdlColor& color, SdlRect& rect, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE );
};
