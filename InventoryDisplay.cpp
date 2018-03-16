#include "Utils.h"
#include "Layers.h"
#include "InventoryDisplay.h"

InventoryDisplay::InventoryDisplay( Inventoried* inventoried )
{
	int width;
	int height;
	
	int swidth;
	int sheight;
	
	this->wrapped = inventoried;
		
	this->texture = new SdlTexture();
	this->texture->Load( "res\\inventory.png" );
	this->texture->isRelative = false;
	
	SdlGlobals::GetScreenDimentions( swidth, sheight );
	
	this->texture->GetDimentions( &width, &height );
	this->texture->Move( ( swidth / 2 ) - ( width / 2 ), ( sheight / 2 ) - ( height / 2 ) );
	
	this->Hide();
	
	Layers::AddToLayer( this, Layers::HUD );
}

bool InventoryDisplay::Update()
{
	if ( this->isActive )
	{
		for ( KeySet::iterator it = this->keysPressed.begin(); it != this->keysPressed.end(); it++ )
		{
			switch ( *it )
			{
				case SDLK_ESCAPE:
				{
					this->Hide();
					this->ClearAsActiveConsumer();
					break;
				}
				default:
				{
					break;
				}
			}
		}
		
		for ( auto it = this->cycleButton.begin(); it != this->cycleButton.end(); it++ )
		{
			switch ( it->button )
			{
				case SDL_BUTTON_LEFT:
				{
					SDL_Point point = { it->x, it->y };
					
					Inventory& inv = this->wrapped->inventory;
					
					for ( auto it = inv.begin(); it != inv.end(); it++ )
					{
						SDL_Rect result;
						Item* item = ( *it );
						
						if ( item && SDL_EnclosePoints( &point, 1, &item->renderRect, &result ) )
						{
							if ( item->Use() )
							{
								int idx = std::distance( inv.begin(), it );
								this->wrapped->RemoveItem( idx );
							}
							break;
						}
					}
										
					break;
				}
			}
		}
	}
	
	return true;
}

void InventoryDisplay::Render()
{
	if ( this->IsDisplayed() )
	{
		int x;
		int y;
		
		this->texture->Render();
		this->texture->GetLocation( &x, &y );
		
		x += 207;
		y += 3;
		
		for ( auto it = this->wrapped->inventory.begin(); it != this->wrapped->inventory.end(); it++ )
		{
			int i = std::distance( this->wrapped->inventory.begin(), it );
			Item* item = ( *it );
			
			if ( item )
			{
				item->Move( x + ( ( i % 5 ) * 67 ), y + ( ( i / 5 ) * 67 ) );
				item->Render();
			}
		}
/*		
		this->texture->GetLocation( &x, &y );
		
		SDL_Point positions[] = {
			{ 70, 3 },
			{ 70, 70 },
			{ 70, 137 },
			{ 70, 204 },
			{ 137, 70 },
			{ 3, 70 }
		};
		
		this->texture->GetLocation( &x, &y );
		for ( int i = 0; i < Equipped::NUM_SLOTS; i++ )
		{
			Item* item = this->wrapped->GetSlot( i );
			if ( item )
			{
				const SDL_Point& point = positions[ i ];
				SDL_Point proxy = { x, y };
				
				proxy.x += point.x;
				proxy.y += point.y;
				
				item->Move( proxy );
				item->Render();
			}
		}
*/
	}
}