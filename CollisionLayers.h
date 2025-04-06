#pragma once

#include "Object.h"
#include "QuadTree.h"

#include <string>
#include <unordered_map>

using namespace Pointers;

class CollisionLayers
{
private:
	static std::unordered_map<std::string, PooledQuadTree> layers;

public:
	static void CreateLayer(const std::string& ident);
	static QuadTree& GetLayer(const std::string& ident);
	static void DeleteLayer(const std::string& ident);
};
