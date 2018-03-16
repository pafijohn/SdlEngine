#pragma once

#include <map>

#include "Timer.h"
#include "SdlRect.h"
#include "Modifier.h"
#include "SdlTexture.h"
#include "Renderable.h"
#include "Inventoried.h"
#include "EventConsumer.h"
#include "CharacterChat.h"
#include "CharacterTexture.h"
#include "InventoryDisplay.h"

class Character: public Renderable, public EventConsumer, public Inventoried, public Equipped
{
	std::map<std::string, Modifier*> modifiers;
	Timer timer;
	CharacterChat chat;
	
public:
	CharacterTexture texture;
	
	Timer staminaTimer;
	float staminaTickRegen;
	
	int moveRate;
	int armor;
	
	float health;
	float stamina;
	float mana;
	
	const static int FRAME_DELAY = 125;
	
	Character();
	void AddHealth( int amount );
	void AddStamina( int amount );
	
	bool SpendStamina( int amount );
	void GetCollisionProxy( SdlRect& proxy );
	void GetCenter( SDL_Point& point );
	
	Modifier* GetModifier( const std::string& name );
	void SetModifier( const std::string& name, Modifier* modifier );
	
	virtual bool Update();
	virtual void Render();
};

extern Character* character;