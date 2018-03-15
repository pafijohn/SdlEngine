#pragma once

#include "SdlTexture.h"
#include "Renderable.h"
#include "Displayable.h"

class Hud: public Renderable, public Displayable
{
public:
	SdlTexture* backpack;
	SdlTexture* inventoryDisplay;
	
	Hud();
	
	int OnClick( const SDL_Point& point );
	
	void Show();
	void Hide();
	virtual bool Update();
	virtual void Render();
};

extern Hud* hud;