#include "Utils.h"
#include "Object.h"
#include "QuadTree.h"

QuadTree::QuadTree()
{
	
}

void QuadTree::SetDepth(int depth)
{
	this->depth = depth;
}

void QuadTree::SetRect(const SdlRect& rect_)
{
	this->rect = rect_;
}

const SdlRect& QuadTree::GetRect()
{
	return this->rect;
}

bool QuadTree::HasDeeper()
{
	return this->subnodes.size() > 0;
}

void QuadTree::SetDeeper(Quadrant which)
{
	int half_width = this->rect.w / 2;
	int half_height = this->rect.h / 2;
	
	int idx = -1;
	SdlRect rectangle;
	rectangle.Scale(half_width, half_height);
	
	switch (which)
	{
		case Quadrant::TOP_LEFT:
		{
			idx = 0;
			rectangle.Move(this->rect.x, this->rect.y);
			break;
		}
		case Quadrant::TOP_RIGHT:
		{
			idx = 1;
			rectangle.Move(this->rect.x + half_width, this->rect.y);
			break;
		}
		case Quadrant::BOTTOM_LEFT:
		{
			idx = 2;
			rectangle.Move(this->rect.x, this->rect.y + half_height);
			break;
		}
		case Quadrant::BOTTOM_RIGHT:
		{
			idx = 3;
			rectangle.Move(this->rect.x + half_width, this->rect.y + half_height);
			break;
		}
		default:
		{
			idx = -1;
			break;
		}
	}
	
	if (idx != -1)
	{
		QuadTree& quadtree = this->subnodes[idx].Take();
		
		quadtree.SetDepth(this->depth + 1);
		quadtree.SetRect(rectangle);
	}
}

void QuadTree::MakeDeeper()
{
	if (!this->HasDeeper() && this->depth < MAX_DEPTH)
	{
		this->subnodes.resize(4);
		this->SetDeeper(Quadrant::TOP_LEFT);
		this->SetDeeper(Quadrant::TOP_RIGHT);
		this->SetDeeper(Quadrant::BOTTOM_LEFT);
		this->SetDeeper(Quadrant::BOTTOM_RIGHT);
	}
}

bool QuadTree::Contains(Entity& entity)
{
	return !!this->values.count(entity.GetId());
}

void QuadTree::Insert(Entity& entity)
{
	SdlRect rect = entity.GetCollision();
	if ((this->rect.CollidesWith(rect) || this->rect.Contains(rect)) && this->values.find(entity.GetId()) == this->values.end())
	{
		this->values.insert(entity.GetId());
		this->MakeDeeper();
		
		if (this->HasDeeper())
		{
			for (auto it = this->subnodes.begin(); it != this->subnodes.end(); it++)
			{
				QuadTree& tree = it->Get();
				tree.Insert(entity);
			}
		}
	}
}

void QuadTree::Remove(Entity& entity)
{
	if (this->Contains(entity))
	{
		this->values.erase(entity.GetId());
		if (this->HasDeeper())
		{
			for (auto it = this->subnodes.begin(); it != this->subnodes.end(); it++)
			{
				QuadTree& tree = it->Get();
				tree.Remove(entity);
			}
		}
	}
}

void QuadTree::RemoveAll()
{
	EntitySet set = this->GetValues();
	EntityRegistry* registry = EntityRegistry::GetInst();
	
	for (auto it = set.begin(); it != set.end(); it++)
	{
		Entity* e = registry->GetEntity(*it);
		
		if (e)
		{
			this->Remove(*e);
		}
		
	}
}

const EntitySet& QuadTree::GetValues()
{
	return this->values;
}

QuadTree* QuadTree::GetDeepest(Entity& entity)
{
	if (this->rect.Contains(entity.GetCollision()))
	{
		if (this->HasDeeper())
		{
			for (auto it = this->subnodes.begin(); it != this->subnodes.end(); it++)
			{
				QuadTree* _deepest = it->Get().GetDeepest(entity);
				
				if (_deepest)
				{
					return _deepest;
				}
			}
		}
		
		return this;
	}
	
	return nullptr;
}

bool QuadTree::CheckCollision(Entity& entity, EntitySet& collisions)
{
	bool collided = false;
	QuadTree* deepest = this->GetDeepest(entity);
	
	if (deepest)
	{
		const SdlRect& entCollision = entity.GetCollision();
		const EntitySet& values_ = deepest->GetValues();
		
		for (auto it = values_.begin(); it != values_.end(); it++)
		{
			uint64_t id = (*it);
			// If the ID is not the id of the current entity
			if (id != entity.GetId())
			{
				EntityRegistry* registry = EntityRegistry::GetInst();
				Entity* e = registry->GetEntity(id);
				
				if (e)
				{
					Entity& e_ = *e;
					const SdlRect& eCollision = e_.GetCollision();
					bool e_Collision = e_.GetHasCollision() && entCollision.CollidesWith(eCollision);
					
					collided = collided || e_Collision;
					
					if (e_Collision)
					{
						collisions.insert(id);
						entity.OnCollision(e_);
						e_.OnCollision(entity);
					}
				}
			}
		}
	}
	
	return collided;
}
