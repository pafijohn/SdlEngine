#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <stdint.h>

#define lengthof( x ) ( sizeof( x ) / sizeof( *x ) )
#define isBounded( x, a, b ) ( x >= a and x < b )

#define EnumToBit(x) (1 << x)

#define DEBUG_PRINT() \
do                                                                   \
{                                                                    \
	printf("Debug print: File: %s, Line: %d\n", __FILE__, __LINE__); \
} while(0)

uint32_t GetRandom( int limit );

void splitString( const std::string& str, std::vector<std::string>& output );