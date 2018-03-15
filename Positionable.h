#pragma once

#include <SDL.h>

class Positionable
{
	SDL_Point point;
	
public:
	Positionable();
	
	virtual void Move( int x, int y );
	virtual void Move( const SDL_Point& point );
	const SDL_Point& GetPos();
};