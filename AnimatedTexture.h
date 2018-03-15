#pragma once

#include <SDL.h>
#include <vector>

#include "Timer.h"
#include "SdlTexture.h"
#include "Positionable.h"

struct FrameInfo
{
	std::string sprite;
	int numFrames;
	
	int frameWidth;
	int frameHeight;
	
	int startX;
	int startY;
	
	int incX;
	int incY;
	
	int scaleW;
	int scaleH;
};

class AnimatedTexture: public Renderable, public Positionable
{
	int frame;
	std::vector< SdlTexture* > frames;
	
	const static int FRAME_DELAY = 125;
	
	Timer timer;
	FrameInfo frameInfo;
public:
	AnimatedTexture( const FrameInfo& frameInfo );
	
	SdlTexture* GetCurrentFrame();
	
	void Play();
	void Stop();
	
	virtual bool Update();
	virtual void Render();
	
	bool repeat;
};