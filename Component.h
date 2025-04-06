#pragma once

#include "Pointers.h"
#include "Entity.h"

using namespace Pointers;

class Component
{
private:
	int updateModulo;
	SP<Entity> parent;
	
protected:
    Component();
	Component(int updateModulo);

public:
	void Tick(int tick);
	virtual void OnTick() = 0;
};