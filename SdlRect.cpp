#include "SdlRect.h"

SdlRect::SdlRect()
{
	this->Update( 0, 0, 0, 0 );
}

SdlRect::SdlRect( int x, int y, int w, int h )
{
	this->Update( x, y, w, h );
}

SdlRect& SdlRect::operator=( const SdlRect& other )
{
	this->x = other.x;
	this->y = other.y;
	this->w = other.w;
	this->h = other.h;
}

void SdlRect::Update( int x, int y, int w, int h )
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void SdlRect::Move( int x, int y )
{
	this->x = x;
	this->y = y;
}

void SdlRect::Scale( int w, int h )
{
	this->w = w;
	this->h = h;
}

bool SdlRect::CollidesWith( const SdlRect& other )
{
	return !!SDL_HasIntersection( this, &other );
}

bool SdlRect::CollidesWith( int x, int y )
{
	SdlRect proxy;
	proxy.Update( x, y, 1, 1 );
	return this->CollidesWith( proxy );
}