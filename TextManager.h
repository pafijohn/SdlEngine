#pragma once

#include <SDL.h>
#include <string>

#include "SdlColor.h"

class TextManager
{
public:
	enum
	{
		COURIER_NEW,
		COURIER_NEW_B, // B == Bold
		COURIER_NEW_I, // I == Italic
		COURIER_NEW_BI,
	};
	
	static void Preload();
	static std::string MakeIdentifier( int font, const SdlColor& color, int size );
	static void ParseIdentifier( const std::string& ident, int& font, SdlColor& color, int& size );
	static int FontToEnum( const std::string& font );
	static void GetDimentions( const std::string& text, const std::string& identifier, int* w, int* h );
	static void RenderText( const std::string& text,
		const std::string& identifier = "",
		int x = 0,
		int y = 0 );
};
