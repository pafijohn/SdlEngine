#pragma once

#include "SdlTexture.h"
#include "Entity.h"

class EntityTexture:
	public Entity,
	public SdlTexture
{
public:
	EntityTexture();
	
	virtual void Move(int x, int y) override;
};