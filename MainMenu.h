#pragma once

#include "Timer.h"
#include "SolidRect.h"
#include "Renderable.h"
#include "SdlTexture.h"
#include "EventConsumer.h"

class MainMenu: public Renderable, public EventConsumer
{
	int currentSelection;
	SolidRect* solidRect;
	SdlTexture texture;
	
	Timer timer;
	
	enum
	{
		SINGLEPLAYER,
		MULTIPLAYER,
		SETTINGS,
		EXIT,
		NUM_SETTINGS
	};
	
public:
	MainMenu();
	
	virtual bool Update();
	virtual void Render();
	
	bool display;
};

extern MainMenu* mainMenu;