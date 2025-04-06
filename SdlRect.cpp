#include "SdlRect.h"

#include <SDL.h>

SdlRect::SdlRect()
{
	this->Update(0, 0, 0, 0);
}

SdlRect::SdlRect(int x, int y, int w, int h)
{
	this->Update(x, y, w, h);
}

SdlRect& SdlRect::operator=(const SdlRect& other)
{
	this->x = other.x;
	this->y = other.y;
	this->w = other.w;
	this->h = other.h;
	
	return *this;
}

void SdlRect::Update(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void SdlRect::Move(int x, int y)
{
	this->x = x;
	this->y = y;
}

void SdlRect::Scale(int w, int h)
{
	this->w = w;
	this->h = h;
}

bool SdlRect::CollidesWith(const SdlRect& other) const
{
	bool intersects = !!SDL_HasIntersection(this, &other);
	
	return intersects;
}

bool SdlRect::CollidesWith(int x, int y) const
{
	SdlRect proxy;
	proxy.Update(x, y, 1, 1);
	return this->CollidesWith(proxy);
}

void SdlRect::GetPoints(SDL_Point& upperLeft, SDL_Point& upperRight, SDL_Point& bottomLeft, SDL_Point& bottomRight)
{
	upperLeft.y = this->y;
	upperRight.y = this->y;
	
	bottomLeft.y = this->y + this->h;
	bottomRight.y = this->y + this->h;
	
	upperLeft.x = this->x;
	bottomLeft.x = this->x;
	
	upperRight.x = this->x + this->w;
	bottomRight.x = this->x + this->w;
}

bool SdlRect::Contains(const SdlRect& other)
{
	return this->CollidesWith(other.x, other.y) &&
		this->CollidesWith(other.x + other.w, other.y + other.h);
}
