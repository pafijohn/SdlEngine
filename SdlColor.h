#pragma once

#include <SDL.h>

struct SdlColor : public SDL_Color
{
	SdlColor( int r = 0, int g = 0, int b = 0, int a = 0 );
};