#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <stdint.h>
#include <algorithm>

#define lengthof(x) (sizeof(x) / sizeof(*x))
#define isBounded(x, a, b) (x >= a && x < b)

#define EnumToBit(x) (1 << x)

#define DEBUG_PRINT() \
do \
{ \
	printf("Debug print: File: %s, Line: %d\n", __FILE__, __LINE__); \
} while(0)

#define MAX(x, y) x>y?x:y
#define MIN(x, y) x>y?y:x


uint32_t GetRandom(uint32_t limit = 0xFFFFFFFF);
uint64_t GetRandom64(uint64_t limit = 0xFFFFFFFFFFFFFFFF);

void splitString(const std::string& str, std::vector<std::string>& output);

template <class T>
T clamp(const T& val, const T& min, const T& max)
{
	T newVal = val;
	newVal = std::min(val, max);
	newVal = std::max(newVal, min);
	
	return newVal;
}