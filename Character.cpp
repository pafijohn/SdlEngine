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

#include <Python.h>

Character* character = nullptr;

Character::Character()
{
	this->health = 50;
	this->mana = 100;
	this->stamina = 1000;
	
	this->staminaTickRegen = 1.35;
	this->moveRate = 5;
	
	this->armor = 0;
	
	this->timer.Start( 0 );
}

void Character::AddHealth( int amount )
{
	if ( amount < 0 )
	{
		Event event;
		event.type = Event::DAMAGE;
		
		event.damage.type = DamageTypes::SLASH;
		event.damage.amount = -amount;
		
		for ( auto it = this->modifiers.begin(); it != this->modifiers.end(); it++ )
		{
			( it->second )->OnEvent( event );
		}
		
		amount = -event.damage.amount;
	}
	
	this->health += amount;
	
	if ( this->health > 100 )
	{
		this->health = 100;
	}
}

void Character::AddStamina( int amount )
{	
	this->stamina += amount;
	
	if ( this->stamina > 1000 )
	{
		this->stamina = 1000;
	}
}

bool Character::SpendStamina( int amount )
{
	bool canSpend = this->stamina >= amount;
	
	if ( canSpend )
	{
		this->stamina -= amount;
		this->staminaTimer.Start( 3 * 1000 );
	}
	
	return canSpend;
}

void Character::GetCollisionProxy( SdlRect& proxy )
{
	SDL_Point point = this->texture.GetPos();
	SdlRect rect;
	rect.x = point.x;
	rect.y = point.y;
	rect.w = CharacterTexture::RENDER_SIZE;
	rect.h = CharacterTexture::RENDER_SIZE;
	
	rect.y += rect.h * ( 3.0 / 4.0 );
	rect.h /= 4;
	
	rect.x += rect.w * ( 2.0 / 5.0 );
	rect.w /= 5;
	
	proxy = rect;
}

void Character::GetCenter( SDL_Point& point )
{
	point = this->texture.GetPos();

	point.x += CharacterTexture::RENDER_SIZE / 2;
	point.y += CharacterTexture::RENDER_SIZE / 2;					
}

Modifier* Character::GetModifier( const std::string& name )
{
	Modifier* mod;
	
	if ( this->modifiers.count( name ) )
	{
		mod = this->modifiers[ name ];
	}
	else
	{
		mod = nullptr;
	}
	
	return mod;
}

void Character::SetModifier( const std::string& name, Modifier* modifier )
{
	this->modifiers[ name ] = modifier;
}

bool Character::Update()
{
	SdlRect proxy;
	
	int directions = 0;
	bool sprinting = false;
	
	const static int _FORWARD = EnumToBit( Directions::FORWARD );
	const static int _LEFT = EnumToBit( Directions::LEFT );
	const static int _DOWN = EnumToBit( Directions::DOWN );
	const static int _RIGHT = EnumToBit( Directions::RIGHT );
	
	if ( this->stamina < 1000 and this->staminaTimer.IsExpired() )
	{
		this->stamina += this->staminaTickRegen;
		
		if ( this->stamina > 1000 )
		{
			this->stamina = 1000;
		}
	}
	
	if ( this->isActive )
	{
		for ( KeySet::iterator it = this->keysPressed.begin(); it != this->keysPressed.end(); it++ )
		{
			switch ( *it )
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
					this->GetCenter( point );
					
					Tile* tile = Map::GetTile( point, this->texture.direction, .75 );
					
					if ( tile )
					{
						tile->OnInteract();
					}
					
					break;
				}
				case SDLK_1:
				{
					texture.SetAction( CharacterTexture::SPELLCAST );
					break;
				}
				case SDLK_ESCAPE:
				{
					mainMenu->SetAsActiveConsumer();
					mainMenu->display = true;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		
		for ( auto it = this->cycleButton.begin(); it != this->cycleButton.end(); it++ )
		{
			switch ( it->button )
			{
				case SDL_BUTTON_LEFT:
				{
					SDL_Point point = { it->x, it->y };
					
					int idx = hud->OnClick( point );
					
					if ( isBounded( idx, 0, this->inventory.size() ) )
					{
						this->Use( idx );
					}
					
					break;
				}
			}
		}
		
		this->Clear();
	}
	
	this->texture.SetDirection( directions );
	this->texture.SetMoving( directions != 0 );
	
	// If you are moving
	if ( directions and this->texture.action == CharacterTexture::WALK )
	{
		SDL_Point loc = this->texture.GetPos();
		
		if ( sprinting and this->SpendStamina( 10 ) )
		{
			this->moveRate = 10;
		}
		else
		{
			this->moveRate = 5;
		}
		
		bool blockedX = false;
		bool blockedY = false;
		
		for ( int i = 0; i < this->moveRate; i++ )
		{
			if ( !blockedX )
			{
				int preX = loc.x;
				
				if ( directions & _LEFT )
				{
					loc.x--;
				}
				
				if ( directions & _RIGHT )
				{
					loc.x++;
				}
				
				this->texture.Move( loc.x, loc.y );
				
				this->GetCollisionProxy( proxy );
				blockedX = Map::HasCollision( proxy );
				
				if ( blockedX )
				{
					loc.x = preX;
				}
			}
			
			if ( !blockedY )
			{
				int preY = loc.y;
				
				if ( directions & _FORWARD )
				{
					loc.y--;
				}
				
				if ( directions & _DOWN )
				{
					loc.y++;
				}
				
				this->texture.Move( loc.x, loc.y );
				
				this->GetCollisionProxy( proxy );
				blockedY = Map::HasCollision( proxy );
				
				if ( blockedY )
				{
					loc.y = preY;
				}
			}
		}
		
		this->texture.Move( loc );
		
		int swidth;
		int sheight;
				
		int width;
		int height;
		
		SdlGlobals::GetScreenDimentions( swidth, sheight );
		Map::GetMapSize( width, height );
		
		loc.x -= ( swidth / 2.0 );
		loc.y -= ( sheight / 2.0 );
		
		loc.x = std::max( loc.x, 0 );
		loc.y = std::max( loc.y, 0 );
		
		loc.x = std::min( loc.x, width - swidth );
		loc.y = std::min( loc.y, height - sheight );
		
		camera.Move( loc );
	}
	
	Event event;
	event.type = Event::TICK;
	
	for ( auto it = this->modifiers.begin(); it != this->modifiers.end(); it++ )
	{
		( it->second )->OnEvent( event );
	}
	
	this->texture.Update();
	
	return true;
}

void Character::Render()
{
	// Intentionally left blank
}
