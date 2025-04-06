#pragma once

#include <SDL.h>
#include <string>

#include "SdlRect.h"
#include "SdlGlobals.h"
#include "Renderable.h"

class SdlTexture: public Renderable
{
public:
	SDL_Texture* texture;
	SDL_Surface* surface;
	SdlRect clippingRect;
	SdlRect renderRect;
	
	double rotation;
	SDL_Point center;
	int flip;
	
	bool isRelative;
	
	SdlTexture();
	~SdlTexture();
	
	void Clean();
	
	void FlipVertical();
	void FlipHorizontal();
	void SetRotatationCenter(int x, int y);
	void Rotate(double rotation);
	void SetAlpha(Uint8 alpha);
	void SetTextureColorMod(const SdlColor& color);
	
	bool Load(const std::string& filename);
	void Crop(int x, int y, int w, int h);
	void Scale(int w, int h);
	bool CreateRGBTexture(int width, int height, const SdlColor& color);
	void SetSurface(SDL_Surface* surface);
	void ResetRects();
	bool IsValid();
	bool RenderBoundingBox();
	
	void Move(const SDL_Point& point);
	virtual void Move(int x, int y);
	
	virtual bool Update();
	virtual void Render();
	
	bool CollidesWith(SdlTexture* other);
	
	void GetDimentions(int* width, int* height);
	void GetLocation(int* x, int* y);
};
