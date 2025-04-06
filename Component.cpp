#include "Component.h"

Component::Component()
{
	this->updateModulo = 1;
}

Component::Component(int updateModulo)
{
	this->updateModulo = updateModulo;
}

void Component::Tick(int tick)
{
	if ((tick % this->updateModulo) == 0)
	{
		this->OnTick();
	}
}