#include <map>
#include <vector>
#include <cstdio>
#include <cctype>
#include <SDL_ttf.h>

#include "SdlGlobals.h"
#include "SdlTexture.h"
#include "TextManager.h"


static std::map<std::string, SdlTexture*> textures;

// Identifier = font|r|g|b|a|size

void TextManager::Preload()
{
	std::vector< std::string > fonts;
	std::vector< SDL_Color > colors;
	std::vector< int > sizes;
	
	fonts.push_back( "C:\\WINDOWS\\Fonts\\cour.ttf" );
	fonts.push_back( "C:\\WINDOWS\\Fonts\\courbd.ttf" );
	fonts.push_back( "C:\\WINDOWS\\Fonts\\courbi.ttf" );
	fonts.push_back( "C:\\WINDOWS\\Fonts\\couri.ttf" );
	
	colors.push_back( ( SDL_Color ){ 0, 0, 0 } );
	colors.push_back( ( SDL_Color ){ 255, 255, 255 } );
	colors.push_back( ( SDL_Color ){ 255, 0, 0 } );
	colors.push_back( ( SDL_Color ){ 0, 255, 0 } );
	colors.push_back( ( SDL_Color ){ 0, 0, 255 } );
	
	sizes.push_back( 12 );
	sizes.push_back( 24 );
	sizes.push_back( 36 );
	sizes.push_back( 256 );
	
	for ( int i = 0; i < fonts.size(); i++ )
	{
		//std::string fullPath = "\%SYSTEMROOT\%\\Fonts\\" + fonts.at( i );
		const char* fontName = fonts.at( i ).data();
		for ( int j = 0; j < colors.size(); j++ )
		{
			SDL_Color& color = colors.at( j );
			
			for ( int k = 0; k < sizes.size(); k++ )
			{
				int size = sizes.at( k );
				
				TTF_Font* font = TTF_OpenFont( fontName, size );
				
				if( font == nullptr )
				{
					printf( "TTF_OpenFont: %s\n", TTF_GetError() );
					continue;
				}
				
				for ( int l = 0; l < 0xFF; l++ )
				{
					char c = ( char )l;
					char identifier[ 256 ];
					char character[ 2 ];
					character[ 0 ] = c;
					character[ 1 ] = '\0';
					
					if ( !( c == '\r' or c == '\n' ) and !isprint( l ) )
					{
						continue;
					}
					
					SDL_Surface* surface = TTF_RenderText_Blended( font, character, color );
					SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
					
					sprintf( identifier, "%s|%d|%d|%d|%d|%d|%c", fontName, color.r, color.g, color.b, color.a, size, c );
					
					if ( surface == nullptr or texture == nullptr )
					{
						printf( "Text loading error: %s\n", identifier );
					}
					else
					{
						SdlTexture* myTexture = new SdlTexture();
						myTexture->isRelative = false;
						myTexture->surface = surface;
						myTexture->texture = texture;
						myTexture->ResetRects();
						
						textures[ identifier ] = myTexture;
					}
				}
			}
		}
	}
}

std::string TextManager::MakeIdentifier( int font, const SdlColor& color, int size )
{
	std::string ident;
	char identifier[ 256 ] = { 0 };
	char fontFace[ 256 ] = { 0 };
	
	switch ( font )
	{
		case COURIER_NEW:
		{
			sprintf( fontFace, "C:\\WINDOWS\\Fonts\\cour.ttf" );
			break;
		}
		case COURIER_NEW_B:
		{
			sprintf( fontFace, "C:\\WINDOWS\\Fonts\\courbd.ttf" );
			break;
		}
		case COURIER_NEW_I:
		{
			sprintf( fontFace, "C:\\WINDOWS\\Fonts\\couri.ttf" );
			break;
		}
		case COURIER_NEW_BI:
		{
			sprintf( fontFace, "C:\\WINDOWS\\Fonts\\courbi.ttf" );
			break;
		}
	}
	
	sprintf( identifier, "%s|%d|%d|%d|%d|%d", fontFace, color.r, color.g, color.b, color.a, size );
	
	ident = identifier;
	
	return ident;
}

void TextManager::ParseIdentifier( const std::string& ident, int& font, SdlColor& color, int& size )
{
	char buff[ 255 ];
	sscanf( ident.data(), "%s|%d|%d|%d|%d|%d", &buff, &color.r, &color.g, &color.b, &color.a, &size );
	
	font = FontToEnum( buff );
}

int TextManager::FontToEnum( const std::string& font )
{
	int enum_;
	static std::map<std::string, int> fonts;
	
	if ( fonts.size() == 0 )
	{
		fonts[ "C:\\WINDOWS\\Fonts\\cour.ttf" ] = COURIER_NEW;
		fonts[ "C:\\WINDOWS\\Fonts\\courbd.ttf" ] = COURIER_NEW_B;
		fonts[ "C:\\WINDOWS\\Fonts\\couri.ttf" ] = COURIER_NEW_I;
		fonts[ "C:\\WINDOWS\\Fonts\\courbi.ttf" ] = COURIER_NEW_BI;
	}
	
	if ( fonts.count( font ) )
	{
		enum_ = fonts[ font ];
	}
	else
	{
		enum_ = -1;
	}
	
	return enum_;
}

void TextManager::GetDimentions( const std::string& text, const std::string& identifier, int* w, int* h )
{
	int maxWidth = 0;
	int width = 0;
	int height = 0;
	
	int charHeight = 0;
	
	std::string ident = identifier.empty() ? "C:\\WINDOWS\\Fonts\\cour.ttf|255|255|255|0|12" : identifier;
	
	for ( int i = 0; i < text.size(); i++ )
	{
		char c = text.at( i );
		std::string fullIdent = ident + "|" + c;
		
		if ( c == '\n' )
		{
			height += charHeight;
		}
		else if ( c == '\r' )
		{
			width = 0;
		}
		else if ( textures.count( fullIdent ) == 0 )
		{
			printf( "Cannot find ident: %s\n", fullIdent.data() );
		}
		else
		{
			int charWidth;
			
			textures[ fullIdent ]->GetDimentions( &charWidth, &charHeight );
			
			width += charWidth;
			maxWidth = std::max( maxWidth, width );
		}
	}
	
	*w = maxWidth;
	*h = height;
}

void TextManager::RenderText( const std::string& text, const std::string& identifier, int x, int y )
{
	int origX = x;
	int OrigY = y;
	
	int height;
	
	std::string ident = identifier.empty() ? "C:\\WINDOWS\\Fonts\\cour.ttf|255|255|255|0|12" : identifier;
	
	for ( int i = 0; i < text.size(); i++ )
	{
		char c = text.at( i );
		std::string fullIdent = ident + "|" + c;
		
		if ( textures.count( fullIdent ) == 0 )
		{
			printf( "Cannot render ident: %s\n", fullIdent.data() );
			continue;
		}
		
		if ( c == '\n' )
		{
			textures[ ident + "|A" ]->GetDimentions( nullptr, &height );
			y += height;
		}
		else if ( c == '\r' )
		{
			x = origX;
		}
		else
		{
			int charWidth;
			
			textures[ fullIdent ]->Move( x, y );
			textures[ fullIdent ]->Render();
			textures[ fullIdent ]->GetDimentions( &charWidth, nullptr );
			
			x += charWidth;
		}
	}
}
