#pragma once

#include "SdlTexture.h"
#include "Renderable.h"
#include "Displayable.h"
#include "InventoryDisplay.h"

class Hud: public Renderable
{
	InventoryDisplay* inventoryDisplay;
	
public:
	SdlTexture* backpack;
	
	Hud();
	
	void OnClick( const SDL_Point& point );
	
	virtual bool Update();
	virtual void Render();
};

extern Hud* hud;