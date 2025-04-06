#pragma once

#include <SDL.h>
#include <vector>

#include "Pointers.h"
#include "Containers.h"
#include "Renderable.h"

class Layers
{
public:
	enum
	{
		BACKGROUND,
		BACKGROUND_2,
		LEVEL,
		ABOVE,
		WEATHER,
		HUD,
		NUM_LAYERS
	};
	
	static void AddToLayer(Renderable* renderable, size_t layer);
	
	static void Update();
	static void Render();
	static void Clear();
	
private:
	static Containers::Vector< Renderable* > layers[NUM_LAYERS];
};