#pragma once

#include <SDL.h>
#include <vector>

#include "Timer.h"
#include "Pointers.h"
#include "Containers.h"
#include "SdlTexture.h"
#include "Positionable.h"

struct FrameInfo
{
	std::string sprite;
	size_t numFrames;
	
	size_t frameWidth;
	size_t frameHeight;
	
	size_t startX;
	size_t startY;
	
	size_t incX;
	size_t incY;
	
	size_t scaleW;
	size_t scaleH;
};

class AnimatedTexture:
	public Renderable,
	public Positionable
{
	size_t frame;
	Containers::Vector<Pointers::SmartPointer<SdlTexture> > frames;
	
	const static int FRAME_DELAY = 125;
	
	Timer timer;
	FrameInfo frameInfo;
public:
	AnimatedTexture(const FrameInfo& frameInfo);
	~AnimatedTexture();
	
	SdlTexture* GetCurrentFrame();
	
	void Play();
	void Stop();
	void Repeat();
	
	virtual bool Update();
	virtual void Render();
	
	bool repeat;
};