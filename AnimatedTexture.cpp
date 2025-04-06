#include "Utils.h"
#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture(const FrameInfo& frameInfo)
{
	this->frameInfo = frameInfo;
	
	SDL_Point pos = this->GetPos();
	
	int fx = this->frameInfo.startX;
	int fy = this->frameInfo.startY;
	
	this->frame = 0;
	this->repeat = false;
	
	for (size_t i = 0; i < this->frameInfo.numFrames; i++)
	{
		Pointers::SmartPointer<SdlTexture> frame_(new SdlTexture());
		
		frame_->Load(this->frameInfo.sprite);
		
		frame_->Move(pos.x, pos.y);
		frame_->Crop(fx, fy, this->frameInfo.frameWidth, this->frameInfo.frameHeight);
		frame_->Scale(this->frameInfo.scaleW, this->frameInfo.scaleH);
		
		this->frames.PushBack(frame_);
		
		fx += this->frameInfo.incX;
		fy += this->frameInfo.incY;
	}
}

AnimatedTexture::~AnimatedTexture()
{
	this->frames.Clear();
}

SdlTexture* AnimatedTexture::GetCurrentFrame()
{
	return this->frames.At(this->frame).Get();
}

void AnimatedTexture::Play()
{
	this->frame = 0;
	this->timer.Start(FRAME_DELAY);
}

void AnimatedTexture::Stop()
{
	this->frame = 0;
	this->timer.Invalidate();
}

void AnimatedTexture::Repeat()
{
	this->repeat = true;
}

bool AnimatedTexture::Update()
{
	bool valid = this->timer.IsValid();
	if (valid)
	{
		if (this->timer.IsExpired())
		{
			this->frame++;
			
			if (this->frame >= this->frameInfo.numFrames)
			{
				valid = this->repeat;
				if (this->repeat)
				{
					this->frame %= this->frameInfo.numFrames;
					this->timer.Start(FRAME_DELAY);
				}
				else
				{
					this->Stop();
				}
			}
			else
			{
				this->timer.Start(FRAME_DELAY);
			}
		}
	}
	
	return valid;
}

void AnimatedTexture::Render()
{
	SDL_Point point = this->GetPos();
	this->GetCurrentFrame()->Move(point.x, point.y);
	this->GetCurrentFrame()->Render();
}
