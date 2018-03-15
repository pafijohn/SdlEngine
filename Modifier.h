#pragma once

#include "Event.h"

class Modifier
{
public:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnEvent( Event& event );
};


class ArmorModifier: public Modifier
{
public:
	virtual void OnEvent( Event& event );
};

class RingModifier: public Modifier
{
public:
	virtual void OnEvent( Event& event );
};