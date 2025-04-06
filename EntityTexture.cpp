#include "EntityTexture.h"

EntityTexture::EntityTexture()
{
	
}

void EntityTexture::Move(int x, int y)
{
	SdlTexture::Move(x, y);
	this->SetCollision(this->renderRect);
}
