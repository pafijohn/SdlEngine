#include "Utils.h"
#include "Layers.h"
#include "Character.h"
#include "CharacterTexture.h"

using namespace Pointers;

const static int frameCounts[] = {
	7,
	8,
	9,
	6,
	13
};

CharacterTexture::CharacterTexture()
{
	FrameInfo frameInfo = {
		"resources\\CharacterSpriteSheet.png", // sprite
		0,                               // numFrames
		SPRITE_SIZE,                     // frameWidth
		SPRITE_SIZE,                     // frameHeight
		0,                               // startX
		0,                               // startY
		SPRITE_SIZE,                     // incX
		0,                               // incY
		SPRITE_SIZE,                     // scaleW
		SPRITE_SIZE                      // scaleH
	};
	
	for (uint32_t i = 0; i < NUM_ACTIONS; i++)
	{
		frameInfo.numFrames = frameCounts[i];
		
		for (uint32_t k = 0; k < Directions::NUM_DIRECTIONS; k++)
		{	
			auto animation = create<AnimatedTexture>(frameInfo);
			
			this->top[i][k] = animation;
			
			frameInfo.startY += SPRITE_SIZE;
		}
	}
	
	this->isMoving = false;
	this->action = WALK;
	this->direction = Directions::FORWARD;
	
	this->Move(0, 0);
	
	Layers::AddToLayer(this, Layers::LEVEL);
	
	for (uint32_t k = 0; k < Directions::NUM_DIRECTIONS; k++)
	{
		this->top[WALK][this->direction]->Repeat();
	}
}

void CharacterTexture::SetMoving(bool moving)
{
	if (moving != this->isMoving)
	{
		if (this->action == WALK)
		{
			this->isMoving = moving;
			
			if (this->isMoving)
			{
				this->GetAnimation()->Play();
			}
			else
			{
				this->GetAnimation()->Stop();
			}
		}
	}
}

void CharacterTexture::SetAction(int action, bool ignoreChecks)
{
	if (action != this->action)
	{
		if (ignoreChecks || (this->action == WALK))
		{
			this->GetAnimation()->Stop();
			
			this->action = action;
			
			this->GetAnimation()->Play();
		}
	}
}

void CharacterTexture::SetDirection(int direction)
{
	if ((direction & (1 << Directions::LEFT)) && (direction & (1 << Directions::RIGHT)))
	{
		direction &= ~((1 << Directions::LEFT) | (1 << Directions::RIGHT));
	}
	
	if ((direction & (1 << Directions::FORWARD)) && (direction & (1 << Directions::DOWN)))
	{
		direction &= ~((1 << Directions::FORWARD) | (1 << Directions::DOWN));
	}
	
	if (direction == 0)
	{
		this->SetMoving(false);
	}
	else
	{
		if ((direction & (1 << this->direction)) == 0)
		{
			for (size_t i = 0; i < Directions::NUM_DIRECTIONS; i++)
			{
				if (direction & (1 << i))
				{
					this->GetAnimation()->Stop();
					
					this->direction = i;
					
					this->GetAnimation()->Play();
					
					break;
				}
			}
		}
	}
	
}

SP<AnimatedTexture> CharacterTexture::GetAnimation()
{
	return this->top[this->action][this->direction];
}

bool CharacterTexture::Update()
{
	SDL_Point point = this->GetPos();
	this->GetAnimation()->Move(point.x, point.y);
	
	if (!this->GetAnimation()->Update())
	{
		this->SetAction(WALK, true);
		this->SetMoving(false);
		this->GetAnimation()->Stop();
	}
	
	return true;
}

void CharacterTexture::Render()
{
	this->GetAnimation()->Render();
}
