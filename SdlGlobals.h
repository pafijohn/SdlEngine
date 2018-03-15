#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "Positionable.h"
#include "SdlColor.h"

class SdlGlobals
{
public:
	static bool Initialize();
	static void GetScreenDimentions( int& width, int& height );
	static void Destroy();
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_GLContext context;
extern Positionable camera;


extern SdlColor black;
extern SdlColor white;
extern SdlColor red;
extern SdlColor green;
extern SdlColor blue;
