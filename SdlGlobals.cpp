#define GLEW_STATIC
#include <GL/glew.h>

#include "SdlGlobals.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#include <glm/glm.hpp>
#include "MixerManager.h"

static bool initSuccess = false;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_GLContext context = 0;
Positionable camera;

SdlColor black(0, 0, 0);
SdlColor red(0xFF, 0, 0);
SdlColor green(0, 0xFF, 0);
SdlColor blue(0, 0, 0xFF);
SdlColor white(0xFF, 0xFF, 0xFF);

bool SdlGlobals::Initialize()
{
	const int DRIVER_INDEX = -1;
	window = nullptr;
	renderer = nullptr;
	bool success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0;
	
	if (success)
	{	
		window = SDL_CreateWindow(
			"SdlGame",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0,
			0,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
		);
		
		success = window != nullptr;
		if (success)
		{
			renderer = SDL_CreateRenderer(window, DRIVER_INDEX, SDL_RENDERER_ACCELERATED);
			context = SDL_GL_CreateContext(window);
			
			success = renderer != nullptr && context != 0;
			
			if (success)
			{				
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
				SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
				
				SDL_GL_MakeCurrent(window, context);
				
				success = TTF_Init() >= 0;
				
				if (success)
				{
					initAudio();
					//SDL_ShowCursor(SDL_DISABLE);
					SDL_ShowCursor(SDL_ENABLE);
					//SDL_SetRelativeMouseMode(SDL_TRUE);
					
					glewExperimental = GL_TRUE;
					glewInit();
				}
			}
			else
			{
				printf("Could not init render || context: %d, %d\n", renderer == nullptr ? 0 : 1, context == nullptr ? 0 : 1);
				puts(SDL_GetError());
			}
		}
	}
	
	initSuccess = success;
	return success;
}

void SdlGlobals::GetScreenDimentions(int& width, int& height)
{
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	width = surface->w;
	height = surface->h;
}

void SdlGlobals::Destroy()
{
	if (initSuccess)
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_GL_DeleteContext(context);
		SDL_CloseAudio();
		window = nullptr;
		renderer = nullptr;
	}
}
