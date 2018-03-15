#include <cstdio>

#include "SdlTexture.h"
#include "TextureManager.h"

SdlTexture::SdlTexture()
{
	this->surface = nullptr;
	this->texture = nullptr;
	this->isRelative = true;
	
	this->rotation = 0.0;
	this->center.x = 0;
	this->center.y = 0;
	this->flip = SDL_FLIP_NONE;
}

SdlTexture::~SdlTexture()
{
	this->Clean();
}

bool SdlTexture::Load( const std::string& filename )
{
	this->surface = TextureManager::GetSurface( filename );
	this->texture = TextureManager::GetTexture( filename );
	
	this->ResetRects();
}

void SdlTexture::Clean()
{
	if ( this->texture )
	{
		this->texture = nullptr;
	}
	
	if ( this->surface )
	{
		this->surface = nullptr;
	}
}

void SdlTexture::FlipVertical()
{	
	if ( flip & SDL_FLIP_VERTICAL )
	{
		this->flip &= ~SDL_FLIP_VERTICAL;
	}
	else
	{
		this->flip |= SDL_FLIP_VERTICAL;
	}
}

void SdlTexture::FlipHorizontal()
{
	if ( flip & SDL_FLIP_HORIZONTAL )
	{
		this->flip &= ~SDL_FLIP_HORIZONTAL;
	}
	else
	{
		this->flip |= SDL_FLIP_HORIZONTAL;
	}
}

void SdlTexture::SetRotatationCenter( int x, int y )
{
	this->center.x = x;
	this->center.y = y;
}

void SdlTexture::Rotate( double rotation )
{
	this->rotation = rotation;
}

void SdlTexture::SetAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( this->texture, alpha );
}

void SdlTexture::SetTextureColorMod( const SdlColor& color )
{
	SDL_SetTextureColorMod( this->texture, color.r, color.g, color.b );
}

void SdlTexture::Crop( int x, int y, int w, int h )
{
	this->clippingRect.Update( x, y, w, h );
}

void SdlTexture::Move( const SDL_Point& point )
{
	this->Move( point.x, point.y );
}

void SdlTexture::Move( int x, int y )
{
	this->renderRect.Move( x, y );
}

void SdlTexture::Scale( int w, int h )
{
	this->renderRect.Scale( w, h );
}

bool SdlTexture::CreateRGBTexture( int width, int height, const SdlColor& color )
{
	Uint32 amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	amask = 0x000000ff;
#else
	amask = 0xff000000;
#endif
	
	SDL_Surface* surface_ = SDL_CreateRGBSurface( 0, width, height, 32, 0, 0, 0, amask );
	
	if ( surface_ )
	{
		SDL_FillRect( surface_, NULL, SDL_MapRGBA( surface_->format, color.r, color.g, color.b, color.a ) );
	}
	
	this->SetSurface( surface_ );
	
	return !!this->texture;
}

void SdlTexture::SetSurface( SDL_Surface* surface )
{
	this->Clean();
	this->surface = surface;
	
	if ( this->surface )
	{
		this->texture = SDL_CreateTextureFromSurface( renderer, this->surface );
		
		this->ResetRects();
	}
}

void SdlTexture::ResetRects()
{
	if ( this->surface )
	{
		this->clippingRect.w = this->surface->w;
		this->clippingRect.h = this->surface->h;
		
		this->renderRect.w = this->clippingRect.w;
		this->renderRect.h = this->clippingRect.h;
	}
}

bool SdlTexture::IsValid()
{
	return !!this->texture;
}

bool SdlTexture::CollidesWith( SdlTexture* other )
{
	return this->renderRect.CollidesWith( other->renderRect );
}

bool SdlTexture::RenderBoundingBox()
{
	int x = this->renderRect.x;
	int y = this->renderRect.y;
	int xw = x + this->renderRect.w;
	int yh = y + this->renderRect.h;
	
	// Top Left to Top Right
	SDL_RenderDrawLine( renderer, x, y, xw, y );
	// Top Right to Bottom Right
	SDL_RenderDrawLine( renderer, xw, y, xw, yh );
	// Bottom Right to Bottom Left
	SDL_RenderDrawLine( renderer, xw, yh, x, yh );
	// Bottom Left to Top Left
	SDL_RenderDrawLine( renderer, x, yh, x, y );
}

bool SdlTexture::Update()
{
	return true;
}

void SdlTexture::Render()
{
	SdlRect modRender = this->renderRect;
	
	if ( this->isRelative )
	{
		SDL_Point pos = camera.GetPos();
		
		modRender.x -= pos.x;
		modRender.y -= pos.y;
	}
	
	SDL_RenderCopyEx( renderer, this->texture, &this->clippingRect, &modRender, this->rotation, &this->center, ( SDL_RendererFlip )this->flip );
}

void SdlTexture::GetDimentions( int* width, int* height )
{
	if ( width )
	{
		*width = this->renderRect.w;
	}
	
	if ( height )
	{
		*height = this->renderRect.h;
	}
}

void SdlTexture::GetLocation( int* x, int* y )
{
	if ( x )
	{
		*x = this->renderRect.x;
	}
	
	if ( y )
	{
		*y = this->renderRect.y;
	}
}
