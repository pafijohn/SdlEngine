#include "Utils.h"
#include "Text.h"
#include "SdlRect.h"
#include "SdlColor.h"
#include "MainMenu.h"
#include "SdlGlobals.h"
#include "TextManager.h"

MainMenu* mainMenu;

MainMenu::MainMenu()
{
	int width;
	int height;
	
	this->texture.Load( "res\\Arrow.png" );
	this->texture.Scale( 26, 26 );
	this->texture.isRelative = false;
	
	this->currentSelection = 0;
	SdlGlobals::GetScreenDimentions( width, height );
	SdlRect rect( 0, 0, width, height );
	SdlColor color( 0x80, 0x80, 0x80, 0x80 );
	
	this->solidRect = new SolidRect( rect, color );
	
	this->timer.Start( 0 );
	
	this->display = true;
}

bool MainMenu::Update()
{	
	if ( !this->isActive )
	{
		return false;
	}
	
	const bool hitEnter = this->keysPressed.find( SDLK_RETURN ) != this->keysPressed.end();
	
	// Enter doesnt have to wait for the timer to expire
	if ( hitEnter )
	{
		switch ( this->currentSelection )
		{
			case SINGLEPLAYER:
			{
				this->display = false;
				EventConsumer::Pop();
				break;
			}
			case MULTIPLAYER:
			{
				break;
			}
			case SETTINGS:
			{
				// Settings stub
				break;
			}
			case EXIT:
			{
				SDL_Event quitEvent;
				quitEvent.type = SDL_QUIT;
				SDL_PushEvent( &quitEvent );
				break;
			}
		}
		
	}
	
	if ( this->timer.IsExpired() )
	{
		for ( KeySet::iterator it = this->keysPressed.begin(); it != this->keysPressed.end(); it++ )
		{
			switch ( *it )
			{
				case SDLK_UP:
				{
					if ( this->currentSelection > 0 )
					{
						this->currentSelection--;
						this->timer.Start( 150 );
					}
					break;
				}
				case SDLK_DOWN:
				{
					if ( this->currentSelection < EXIT )
					{
						this->currentSelection++;
						this->timer.Start( 150 );
					}
					break;
				}
			}
		}
	}
	
	return true;
}

void MainMenu::Render()
{
	if ( this->display )
	{
		int width;
		int height;
		
		SdlColor color( 0xFF, 0xFF, 0xFF );
		
		std::string ident = TextManager::MakeIdentifier( TextManager::COURIER_NEW_B, color, 36 );
		
		SdlGlobals::GetScreenDimentions( width, height );
		
		Text text;
		
		text.Move( 100, 100 );
		text.AddText( "Play Singleplayer", ident );
		text.Render();
		text.Clear();
		
		text.Move( 100, 150 );
		text.AddText( "Play Multiplayer", ident );
		text.Render();
		text.Clear();
		
		text.Move( 100, 200 );
		text.AddText( "Settings", ident );
		text.Render();
		text.Clear();
		
		text.Move( 100, 250 );
		text.AddText( "Exit", ident );
		text.Render();
		text.Clear();
		
		this->texture.Move( 50, 105 + ( this->currentSelection * 50 ) );
		
		this->solidRect->Render();
		this->texture.Render();
	}
}
