#include "CollisionLayers.h"

std::unordered_map<std::string, PooledQuadTree> CollisionLayers::layers;

void CollisionLayers::CreateLayer(const std::string& ident)
{
	layers[ident] = PooledQuadTree();
	layers[ident].Take();
}

QuadTree& CollisionLayers::GetLayer(const std::string& ident)
{
	return layers.at(ident).Get();
}

void CollisionLayers::DeleteLayer(const std::string& ident)
{
	layers.erase(ident);
}
