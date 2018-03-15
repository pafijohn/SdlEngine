#include "SolidRect.h"
#include "TextureManager.h"

SolidRect::SolidRect( const SdlRect& rect, const SdlColor& col )
{
	this->rect = rect;
	this->col = col;
}

bool SolidRect::Update()
{
	return true;
}

void SolidRect::Render()
{
	TextureManager::RenderSolid( this->col, this->rect, SDL_BLENDMODE_BLEND );
}
