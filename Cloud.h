#pragma once

#include <SDL.h>

#include "Timer.h"
#include "SdlTexture.h"

class Cloud: public SdlTexture
{
	Timer timer;
	Timer deleteTimer;
	
public:
	Cloud();
	virtual bool Update();
};