#pragma once

#include "Containers.h"
#include "Positionable.h"
#include "Component.h"

class PositionHistogramComponent: public Component
{
	Containers::Vector<Positionable> positions;
public:
	PositionHistogramComponent();
	virtual void OnTick();
};