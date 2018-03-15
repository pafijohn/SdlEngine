#include "Layers.h"

std::vector< Renderable* > Layers::layers[ NUM_LAYERS ];

void Layers::AddToLayer( Renderable* renderable, int layer )
{
	if ( layer >= BACKGROUND and layer < NUM_LAYERS )
	{
		layers[ layer ].push_back( renderable );
	}
}

void Layers::Update()
{
	for ( int i = 0; i < NUM_LAYERS; i++ )
	{
		for ( auto it = layers[ i ].begin(); it != layers[ i ].end(); it++ )
		{
			( *it )->Update();
		}
	}
}

void Layers::Render()
{
	for ( int i = 0; i < NUM_LAYERS; i++ )
	{
		for ( auto it = layers[ i ].begin(); it != layers[ i ].end(); it++ )
		{
			( *it )->Render();
		}
	}
}