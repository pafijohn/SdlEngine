#pragma once

#include <SDL.h>
#include <vector>

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
	
	static void AddToLayer( Renderable* renderable, int layer );
	
	static void Update();
	static void Render();
	
private:
	static std::vector< Renderable* > layers[ NUM_LAYERS ];
};