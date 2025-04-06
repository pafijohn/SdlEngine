#pragma once

#include "Pointers.h"
#include "SdlTexture.h"
#include "Renderable.h"
#include "Displayable.h"
#include "ContextMenu.h"
#include "InventoryDisplay.h"

using namespace Pointers;

class Hud: public Renderable
{
	InventoryDisplay* inventoryDisplay;
	
public:
	SP<SdlTexture> backpack;
	
	Hud();
	~Hud();
	
	void OnClick(const SDL_MouseButtonEvent& point);
	bool HideInventory();
	
	virtual bool Update();
	virtual void Render();
};

extern Hud* hud;