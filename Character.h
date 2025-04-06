#pragma once

#include <map>

#include "Timer.h"
#include "Entity.h"
#include "SdlRect.h"
#include "Modifier.h"
#include "SdlTexture.h"
#include "Renderable.h"
#include "Inventoried.h"
#include "EventConsumer.h"
#include "CharacterChat.h"
#include "CharacterTexture.h"
#include "InventoryDisplay.h"

class Character:
	public Renderable,
	public EventConsumer,
	public Inventoried,
	public Equipped,
	public Entity
{
	std::map<std::string, Modifier*> modifiers;
	Timer timer;
	CharacterChat chat;
	
	std::map<std::string, void (Character::*)()> actionHandlers;
	
	// Action handlers
	void OnSomething();
	void OnForward();
	void OnBackward();
	void OnLeft();
	void OnRight();
	void OnAction1();
	void OnAction2();
	void OnAction3();
	void OnAction4();
	void OnAction5();
	void OnAction6();
	void OnAction7();
	void OnAction8();
	void OnAction9();
	void OnAction0();
	void OnRun();
	void OnOpenMainMenu();
	void OnOpenInventory();
	void OnInteract();
	
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
	bool AddHealth(int amount);
	void AddStamina(int amount);
	
	bool SpendStamina(int amount);
	void GetCollisionProxy(SdlRect& proxy);
	void GetCenter(SDL_Point& point);
	
	Modifier* GetModifier(const std::string& name);
	void SetModifier(const std::string& name, Modifier* modifier);
	void ProcessInputs();
	
	virtual bool Update();
	virtual void Render();
	
	virtual void OnCollision(const Entity& entity);
};

extern Character* character;