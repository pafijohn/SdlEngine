#include <SDL.h>
#include <cmath>
#include <cstdio>
#include <time.h>
#include <cstdint>
#include <stdlib.h>
#include <Python.h>
#include <filesystem>

#include "Hud.h"
#include "Map.h"
#include "Timer.h"
#include "Utils.h"
#include "Cloud.h"
#include "Object.h"
#include "Layers.h"
#include "MainMenu.h"
#include "Character.h"
#include "SolidRect.h"
#include "SdlTexture.h"
#include "SdlGlobals.h"
#include "TextManager.h"
#include "EngineModule.h"
#include "EventConsumer.h"
#include "MixerManager.h"
#include "TextureManager.h"

#include "HashMap.h"
#include "KeyHash.h"

using namespace Pointers;

int main(int argc, char* args[])
{
	bool quit = false;
	
	QuadTreePool::Init();
	
	SdlGlobals::Initialize();
	TextureManager::Preload();
	TextManager::Preload();
	
	Map::Init();
	
	character = new Character();
	
	character->SetSlot(Equipped::BODY, new Armor());
	character->AddItem(new Bread());
	character->AddItem(new Ring());
	character->SetAsActiveConsumer();
	
	mainMenu = new MainMenu();
	
	hud = new Hud();
	
	Layers::AddToLayer(character, Layers::LEVEL);
	Layers::AddToLayer(mainMenu, Layers::HUD);
	
	const wchar_t* program = L"SdlTest";
	
	//Py_SetProgramName(program);  /* optional but recommended */
	InitModules();
	Py_Initialize();

	std::filesystem::path currentPath = std::filesystem::current_path();
	std::string modulePath = currentPath.string();
	//modulePath += "\\python_scripts";

	PyObject* sys_path = PySys_GetObject("path");
	PyObject* new_path = PyUnicode_FromString(modulePath.c_str());
	int result = PyList_Append(sys_path, new_path);
	
	PyObject* pModule = PyImport_ImportModule("scripts");
	PyErr_Print();
	PyObject* pFunc = PyObject_GetAttrString(pModule, "init");
	
	if (pFunc && PyCallable_Check(pFunc))
	{
		PyObject_CallObject(pFunc, NULL);
	}
	
	SDL_Event event;
	
	uint32_t tick = SDL_GetTicks();
	
	const int fpsRate = 1000.0 / 100.0;
	uint32_t nextFps = tick;
	
	const int tickRate = 1000.0 / 50.0;
	uint32_t nextTick = tick;
		
	while(!quit)
	{
		tick = SDL_GetTicks();
				
		if (tick >= nextTick)
		{
			nextTick += tickRate;
			while(SDL_PollEvent(&event) != 0 && !quit)
			{
				// https://wiki.libsdl.org/SDL_EventType
				// https://wiki.libsdl.org/SDL_Keycode
				switch(event.type)
				{
					case SDL_QUIT:
					{
						quit = true;
						break;
					}
					case SDL_KEYDOWN:
					{
						EventConsumer::KeyPressed(event.key.keysym.sym);
						break;
					}
					case SDL_KEYUP:
					{
						EventConsumer::KeyReleased(event.key.keysym.sym);
						break;
					}
					case SDL_MOUSEBUTTONUP:
					{
						EventConsumer::ButtonPressed(event.button);
						break;
					}
				}
			}
			
			Layers::Update();
		}
		
		if (tick >= nextFps)
		{
			nextFps += fpsRate;
			
			SDL_RenderClear(renderer);
			
			Layers::Render();
			
			SDL_RenderPresent(renderer);
		}
		
		while (nextTick > SDL_GetTicks() + 1 && nextFps > SDL_GetTicks() + 1)
		{
			SDL_Delay(1);
		}
	}
	
	Layers::Clear();
	SdlGlobals::Destroy();
	SDL_Quit();
	
	if (Py_FinalizeEx() < 0)
	{
		printf("Couldn't finalize Python properly.");
	}
	
	return 0;
}