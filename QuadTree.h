#pragma once

#include <vector>
#include <unordered_set>

#include "Object.h"
#include "Entity.h"
#include "SdlRect.h"
#include "Pointers.h"

using namespace Pointers;

typedef std::unordered_set<uint64_t> EntitySet;

class QuadTree
{
private:
	std::vector< PooledQuadTree > subnodes;
	EntitySet values;
	SdlRect rect;
	int depth;
	
	enum Quadrant
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		NUM_QUADRANTS
	};
	
	static const int MAX_DEPTH = 5;
	
public:
	QuadTree();
	
	void SetDepth(int depth = 0);
	void SetRect(const SdlRect& rect_);
	const SdlRect& GetRect();
	bool HasDeeper();
	void SetDeeper(Quadrant which);
	void MakeDeeper();	
	bool Contains(Entity& entity);
	void Insert(Entity& entity);
	void Remove(Entity& entity);
	void RemoveAll();
	const EntitySet& GetValues();
	QuadTree* GetDeepest(Entity& entity);	
	bool CheckCollision(Entity& entity, EntitySet& collisions);
};