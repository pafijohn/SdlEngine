#include <SDL.h>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <SDL_Image.h>

#include "Hud.h"
#include "Map.h"
#include "Text.h"
#include "Utils.h"
#include "MainMenu.h"
#include "Character.h"
#include "Directions.h"
#include "SdlGlobals.h"
#include "TextManager.h"
#include "MixerManager.h"
#include "TextureManager.h"

#include <cmath>
#include <Python.h>

Character* character = nullptr;

void Character::OnSomething()
{
	puts("Character::OnSomething");
}

Character::Character()
{
	this->health = 50;
	this->mana = 100;
	this->stamina = 1000;

	this->staminaTickRegen = 1.35;
	this->moveRate = 5;

	this->armor = 0;

	this->timer.Start(0);
	Map::PushEntity(*this);

	this->actionHandlers["OnSomething"] = &Character::OnSomething;


	//auto handler = this->actionHandlers["OnSomething"];
	(this->*this->actionHandlers["OnSomething"])();
	//this->OnAction = &Character::OnSomething;
	//(this->*this->OnAction)();
}

void Character::OnForward()
{

}

void Character::OnBackward()
{

}

void Character::OnLeft()
{

}

void Character::OnRight()
{

}

void Character::OnAction1()
{

}

void Character::OnAction2()
{

}

void Character::OnAction3()
{

}

void Character::OnAction4()
{

}

void Character::OnAction5()
{

}

void Character::OnAction6()
{

}

void Character::OnAction7()
{

}

void Character::OnAction8()
{

}

void Character::OnAction9()
{

}

void Character::OnAction0()
{

}

void Character::OnRun()
{

}

void Character::OnOpenMainMenu()
{

}

void Character::OnOpenInventory()
{

}

void Character::OnInteract()
{

}

bool Character::AddHealth(int amount)
{
	bool canChange = (amount < 0);

	if (canChange)
	{
		Event event;
		event.type = Event::DAMAGE;

		event.damage.type = DamageTypes::SLASH;
		event.damage.amount = -amount;

		for (auto it = this->modifiers.begin(); it != this->modifiers.end(); it++)
		{
			(it->second)->OnEvent(event);
		}

		amount = -event.damage.amount;
	}

	// Recheck against < 0 since the value may be reduce to 0
	canChange = (amount < 0) || (amount > 0 && this->health < 100.0f);

	if (canChange)
	{
		this->health += amount;

		this->health = clamp(this->health, 0.0f, 100.0f);
	}

	return canChange;
}

void Character::AddStamina(int amount)
{
	this->stamina = clamp(this->stamina + amount, 0.0f, 1000.0f);
}

bool Character::SpendStamina(int amount)
{
	bool canSpend = this->stamina >= amount;

	if (canSpend)
	{
		this->stamina -= amount;
		this->staminaTimer.Start(3 * 1000);
	}

	return canSpend;
}

void Character::GetCollisionProxy(SdlRect& proxy)
{
	SDL_Point point = this->texture.GetPos();
	SdlRect rect;
	rect.x = point.x;
	rect.y = point.y;
	rect.w = CharacterTexture::RENDER_SIZE;
	rect.h = CharacterTexture::RENDER_SIZE;

	rect.y += rect.h * (3.0 / 4.0);
	rect.h /= 4;

	rect.x += rect.w * (2.0 / 5.0);
	rect.w /= 5;

	proxy = rect;
}

void Character::GetCenter(SDL_Point& point)
{
	point = this->texture.GetPos();

	point.x += CharacterTexture::RENDER_SIZE / 2;
	point.y += CharacterTexture::RENDER_SIZE / 2;
}

Modifier* Character::GetModifier(const std::string& name)
{
	Modifier* mod;

	if (this->modifiers.count(name))
	{
		mod = this->modifiers[ name ];
	}
	else
	{
		mod = nullptr;
	}

	return mod;
}

void Character::SetModifier(const std::string& name, Modifier* modifier)
{
	this->modifiers[name] = modifier;
}

void Character::OnCollision(const Entity& entity)
{
}

void Character::ProcessInputs()
{
	const static int _FORWARD = EnumToBit(Directions::FORWARD);
	const static int _LEFT = EnumToBit(Directions::LEFT);
	const static int _DOWN = EnumToBit(Directions::DOWN);
	const static int _RIGHT = EnumToBit(Directions::RIGHT);

	SdlRect proxy;

	int directions = 0;
	bool sprinting = false;

	for (KeySet::iterator it = this->keysPressed.begin(); it != this->keysPressed.end(); it++)
	{
		switch (*it)
		{
			case SDLK_w:
			{
				directions |= _FORWARD;
				break;
			}
			case SDLK_a:
			{
				directions |= _LEFT;
				break;
			}
			case SDLK_s:
			{
				directions |= _DOWN;
				break;
			}
			case SDLK_d:
			{
				directions |= _RIGHT;
				break;
			}
			case SDLK_LSHIFT:
			{
				sprinting = true;
				break;
			}
			case SDLK_RETURN:
			{
				this->chat.Activate();
				break;
			}
			case SDLK_e:
			{
				SDL_Point point;
				this->GetCenter(point);

				Tile* tile = Map::GetTile(point, this->texture.direction, .75);

				if (tile)
				{
					tile->OnInteract();
				}

				break;
			}
			case SDLK_1:
			{
				texture.SetAction(CharacterTexture::SPELLCAST);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	this->texture.SetMoving(directions != 0);

	for (auto it = this->cycleButton.begin(); it != this->cycleButton.end(); it++)
	{
		hud->OnClick((*it));
	}

	for (auto it = this->cycleKeys.begin(); it != this->cycleKeys.end(); it++)
	{
		switch (*it)
		{
			case SDLK_ESCAPE:
			{
				if (!hud->HideInventory())
				{
					mainMenu->SetAsActiveConsumer();
					mainMenu->Show();
				}
				break;
			}
		}
	}

	this->Clear();

	// If you are moving
	if (directions && this->texture.action == CharacterTexture::WALK)
	{
		this->texture.SetDirection(directions);

		EntitySet collisions;
		EntitySet revertCollisions;
		this->GetCollisionProxy(proxy);
		this->SetCollision(proxy);

		// If we start out colliding with an entity then we will:
		//  disable collision for that entity
		//  move around
		//  re-enable collision for that entity
		if(Map::CheckCollision(*this, collisions))
		{
			for (auto it = collisions.begin(); it != collisions.end(); it++)
			{
				auto id = (*it);
				EntityRegistry* registry = EntityRegistry::GetInst();
				Entity* entity = registry->GetEntity(id);

				if (entity && entity->GetHasCollision())
				{
					revertCollisions.insert(id);
					entity->SetHasCollision(false);
				}
			}
		}

		bool isColliding;

		SDL_Point loc = this->texture.GetPos();

		if (sprinting && this->SpendStamina(10))
		{
			this->moveRate = 10;
		}
		else
		{
			this->moveRate = 5;
		}

		if (directions & _LEFT)
		{
			loc.x -= this->moveRate;
		}

		if (directions & _RIGHT)
		{
			loc.x += this->moveRate;
		}

		// do the move && revert it back if need be
		do
		{
			this->texture.Move(loc.x, loc.y);
			this->GetCollisionProxy(proxy);
			this->SetCollision(proxy);

			isColliding = Map::CheckCollision(*this, collisions);

			if (isColliding)
			{
				if (directions & _LEFT)
				{
					loc.x++;
				}

				if (directions & _RIGHT)
				{
					loc.x--;
				}
			}
		} while(isColliding);

		if (directions & _FORWARD)
		{
			loc.y -= this->moveRate;
		}

		if (directions & _DOWN)
		{
			loc.y += this->moveRate;
		}

		do
		{
			this->texture.Move(loc.x, loc.y);
			this->GetCollisionProxy(proxy);
			this->SetCollision(proxy);

			isColliding = Map::CheckCollision(*this, collisions);

			if (isColliding)
			{
				if (directions & _FORWARD)
				{
					loc.y++;
				}

				if (directions & _DOWN)
				{
					loc.y--;
				}
			}
		} while(isColliding);

		this->texture.Move(loc);

		for (auto it = revertCollisions.begin(); it != revertCollisions.end(); it++)
		{
			auto id = (*it);
			EntityRegistry* registry = EntityRegistry::GetInst();
			Entity* entity = registry->GetEntity(id);

			if (entity)
			{
				entity->SetHasCollision(true);
			}
		}

		int swidth;
		int sheight;

		int width;
		int height;

		SdlGlobals::GetScreenDimentions(swidth, sheight);
		Map::GetMapSize(width, height);

		loc.x -= (swidth / 2.0);
		loc.y -= (sheight / 2.0);

		loc.x = std::max(loc.x, 0);
		loc.y = std::max(loc.y, 0);

		loc.x = std::min(loc.x, width - swidth);
		loc.y = std::min(loc.y, height - sheight);

		// camera is global
		camera.Move(loc);
	}
}

bool Character::Update()
{
	if (this->staminaTimer.IsExpired())
	{
		this->AddStamina(this->staminaTickRegen);
	}

	if (this->IsActiveConsumer())
	{
		this->ProcessInputs();
	}
	else
	{
		this->texture.SetMoving(false); // Cant be moving if we arent the active consumer
	}

	for (auto it = this->modifiers.begin(); it != this->modifiers.end(); it++)
	{
		Event event;
		event.type = Event::TICK;

		Modifier* modifier = (it->second);
		if (modifier)
		{
			modifier->OnEvent(event);
		}
	}

	this->texture.Update();

	return true;
}

void Character::Render()
{
	// Intentionally left blank
}
