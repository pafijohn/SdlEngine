#pragma once

#include <SDL.h>

struct SdlRect : SDL_Rect
{
	SdlRect();
	SdlRect( int x, int y, int w, int h );
	SdlRect& operator=( const SdlRect& other );
	
	void Update( int x, int y, int w, int h );
	void Move( int x, int y );
	void Scale( int w, int h );
	bool CollidesWith( const SdlRect& other );
	bool CollidesWith( int x, int y );
};
