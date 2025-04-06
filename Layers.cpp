#include "Layers.h"

Containers::Vector< Renderable* > Layers::layers[NUM_LAYERS];

void Layers::AddToLayer(Renderable* renderable, size_t layer)
{
	if (layer >= BACKGROUND && layer < NUM_LAYERS)
	{
		layers[layer].PushBack(renderable);
	}
}

void Layers::Update()
{
	for (size_t i = 0; i < NUM_LAYERS; i++)
	{
		auto& layer = layers[i];
		
		for (size_t j = 0; j < layer.Size(); j++)
		{
			auto& renderable = layer.At(j);
			renderable->Update();
			
			if (renderable->IsPendingDelete())
			{
				layer.Pop(j);
			}
		}
	}
}

void Layers::Render()
{
	for (size_t i = 0; i < NUM_LAYERS; i++)
	{
		auto& layer = layers[i];
		for (size_t j = 0; j < layer.Size(); j++)
		{
			layer.At(j)->Render();
		}
	}
}

void Layers::Clear()
{
	for (size_t i = 0; i < NUM_LAYERS; i++)
	{
		auto& layer = layers[i];
		layer.Clear();
	}
}
