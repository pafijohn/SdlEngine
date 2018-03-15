#pragma once

#include <SDL.h>

#include "Layers.h"
#include "Directions.h"
#include "Renderable.h"
#include "Positionable.h"
#include "AnimatedTexture.h"

class CharacterTexture: public Renderable, public Positionable
{
public:
	CharacterTexture();
	void SetMoving( bool moving );
	void SetAction( int action, bool ignoreChecks = false );
	void SetDirection( int direction );
	
	AnimatedTexture* GetAnimation();
	
	bool Update();
	void Render();
	
	enum
	{
		SPELLCAST,
		THRUST,
		WALK,
		WAVE,
		SHOOT,
		NUM_ACTIONS
	};
	
	const static int SPRITE_SIZE = 64;
	const static int RENDER_SIZE = 64;
	
	int action;
	int direction;
	bool isMoving;
	
	Timer timer;
	
private:
	AnimatedTexture* top[ NUM_ACTIONS ][ Directions::NUM_DIRECTIONS ];
};
