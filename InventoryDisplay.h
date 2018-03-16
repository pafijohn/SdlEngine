#pragma once

#include "Renderable.h"
#include "Inventoried.h"
#include "Displayable.h"
#include "EventConsumer.h"

class InventoryDisplay: public Renderable, public Displayable, public EventConsumer
{
	SdlTexture* texture;
	Inventoried* wrapped;
	
public:
	InventoryDisplay( Inventoried* inventoried );
	
	virtual bool Update();
	virtual void Render();
};