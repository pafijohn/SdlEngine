#include <cstdio>

#include "Hud.h"
#include "Text.h"
#include "Layers.h"
#include "Character.h"
#include "SdlGlobals.h"
#include "TextManager.h"

Hud* hud = nullptr;

Hud::Hud()
{
	Layers::AddToLayer( this, Layers::HUD );
	
	int width;
	int height;
	
	int swidth;
	int sheight;
	
	this->inventoryDisplay = new SdlTexture();
	this->inventoryDisplay->Load( "res\\inventory.png" );
	this->inventoryDisplay->isRelative = false;
	
	SdlGlobals::GetScreenDimentions( swidth, sheight );
	
	this->inventoryDisplay->GetDimentions( &width, &height );
	this->inventoryDisplay->Move( ( swidth / 2 ) - ( width / 2 ), ( sheight / 2 ) - ( height / 2 ) );
	
	this->backpack = new SdlTexture();
	this->backpack->isRelative = false;
	this->backpack->Load( "res\\Items\\backpack.png" );
	this->backpack->GetDimentions( &width, &height );
	this->backpack->Move( swidth - width, sheight - height );
}

int Hud::OnClick( const SDL_Point& point )
{
	int idx;
	SDL_Rect result;
	
	if ( SDL_EnclosePoints( &point, 1, &this->backpack->renderRect, &result ) )
	{
		this->SetDisplayed( !this->IsDisplayed() );
	}
	else if ( this->IsDisplayed() )
	{
		for ( auto it = character->inventory.begin(); it != character->inventory.end(); it++ )
		{
			Item* item = ( *it );
			
			if ( item )
			{
				if ( SDL_EnclosePoints( &point, 1, &item->renderRect, &result ) )
				{
					idx = std::distance( character->inventory.begin(), it );
				}
			}
		}
	}
	else
	{
		idx = -1;
	}
	
	return idx;
}

bool Hud::Update()
{
	return true;
}

void Hud::Render()
{
	char buff[256];
	
	Text status;
	
	sprintf( buff, "Health: %.02f\r\n", character->health );
	status.AddText( buff, TextManager::MakeIdentifier( TextManager::COURIER_NEW_B, SdlColor( 0xFF, 0, 0 ), 36 ) );
	status.SetBackground( Text::GREY );
	
	sprintf( buff, "Stamina: %.02f\r\n", character->stamina );
	status.AddText( buff, TextManager::MakeIdentifier( TextManager::COURIER_NEW_B, SdlColor( 0, 0xFF, 0 ), 36 ) );
	
	sprintf( buff, "Mana: %.02f\r\n", character->mana );
	status.AddText( buff, TextManager::MakeIdentifier( TextManager::COURIER_NEW_B, SdlColor( 0, 0, 0xFF ), 36 ) );
	
	status.Move( 50, ( 4.0 / 5.0 ) * 1080 );
	
	status.Render();
	
	this->backpack->Render();
	
	if ( this->IsDisplayed() )
	{
		int x;
		int y;
		
		this->inventoryDisplay->Render();
		this->inventoryDisplay->GetLocation( &x, &y );
		
		x += 207;
		y += 3;
		
		for ( auto it = character->inventory.begin(); it != character->inventory.end(); it++ )
		{
			int i = std::distance( character->inventory.begin(), it );
			Item* item = ( *it );
			
			if ( item )
			{
				item->Move( x + ( ( i % 5 ) * 67 ), y + ( ( i / 5 ) * 67 ) );
				item->Render();
			}
		}
		
		this->inventoryDisplay->GetLocation( &x, &y );
		
		SDL_Point positions[] = {
			{ 70, 3 },
			{ 70, 70 },
			{ 70, 137 },
			{ 70, 204 },
			{ 137, 70 },
			{ 3, 70 }
		};
		
		this->inventoryDisplay->GetLocation( &x, &y );
		for ( int i = 0; i < Equipped::NUM_SLOTS; i++ )
		{
			Item* item = character->GetSlot( i );
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
	}
}