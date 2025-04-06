#include "Positionable.h"

Positionable::Positionable()
{
	this->point.x = 0;
	this->point.y = 0;
}

void Positionable::Move( int x, int y )
{
	SDL_Point point = { x, y };
	
	this->Move( point );
}

void Positionable::Move( const SDL_Point& point )
{
	this->point = point;
	this->OnMove();
}

const SDL_Point& Positionable::GetPos()
{
	return this->point;
}

void Positionable::OnMove()
{
	
}
