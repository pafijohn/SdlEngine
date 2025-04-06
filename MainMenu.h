#pragma once

#include "Timer.h"
#include "SolidRect.h"
#include "Renderable.h"
#include "SdlTexture.h"
#include "Displayable.h"
#include "EventConsumer.h"

class MainMenu:
	public Renderable,
	public EventConsumer,
	public Displayable
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
};

extern MainMenu* mainMenu;