#include "Utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

uint32_t GetRandom( int limit )
{
	static uint32_t x = time( nullptr );
	static uint32_t y = x * 2;
	static uint32_t z = x / 3;
	static uint32_t w = x ^ z;
	
	uint32_t t = x;
	
	t ^= t << 11;
	t ^= t >> 8;
	x = y; y = z; z = w;
	w ^= w >> 19;
	w ^= t;
	
	return w % limit;
}

void splitString( const std::string& str, std::vector<std::string>& output )
{
	std::string pushing;
	for ( auto it = str.begin(); it != str.end(); it++ )
	{
		if ( ( *it ) == ' ' )
		{
			if ( pushing.size() > 0 )
			{
				output.push_back( pushing );
			}
			
			pushing = "";
		}
		else
		{
			pushing += ( *it );
		}
	}
	
	if ( pushing.size() > 0 )
	{
		output.push_back( pushing );
	}
}