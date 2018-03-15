#pragma once

#include <SDL.h>

#include "SdlRect.h"
#include "SdlColor.h"
#include "Renderable.h"

class SolidRect: public Renderable
{
	SdlRect rect;
	SdlColor col;
public:
	SolidRect( const SdlRect& rect, const SdlColor& col );
	
	virtual bool Update();
	virtual void Render();
};