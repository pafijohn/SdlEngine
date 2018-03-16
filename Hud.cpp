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
	
	SdlGlobals::GetScreenDimentions( swidth, sheight );

	this->backpack = new SdlTexture();
	this->backpack->isRelative = false;
	this->backpack->Load( "res\\Items\\backpack.png" );
	this->backpack->GetDimentions( &width, &height );
	this->backpack->Move( swidth - width, sheight - height );
	
	this->inventoryDisplay = new InventoryDisplay( character );
}

void Hud::OnClick( const SDL_Point& point )
{
	SDL_Rect result;
	
	if ( SDL_EnclosePoints( &point, 1, &this->backpack->renderRect, &result ) )
	{
		this->inventoryDisplay->Show();
		this->inventoryDisplay->SetAsActiveConsumer();
	}
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
}