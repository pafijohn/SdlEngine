#include "Entity.h"
#include "EntityRegistry.h"

static EntityRegistry* registry = nullptr;

EntityRegistry::EntityRegistry()
{
	// Here just to prevent external creations
}

EntityRegistry* EntityRegistry::GetInst()
{
	if (registry == nullptr)
	{
		registry = new EntityRegistry();
	}
	
	return registry;
}

Entity* EntityRegistry::GetEntity(uint64_t id)
{
	// Members of a class are accessed via the "this" keyword
	// The "this->entities" is a member of this class with a type of map
	// A map(or dictionary) is set of key -> value pairs
	// This maps keys are a 64 bit integer -> Entity pointer
	// The key is an id that has been assigned to an Entity
	// The id gives a method to uniquely identify an Entity
	
	Entity* ent;
	
	// If the id of the entity we are trying to get is a key
	if (this->entities.count(id))
	{
		// Pull the value using that key
		ent = this->entities[id];
	}
	// Otherwise (there is no mapping for that particular id -> Entity*)
	else
	{
		// There is no entity
		ent = nullptr;
	}
	
	// Return the entity if it was in our map or nullptr otherwise
	return ent;
}

bool EntityRegistry::Register(Entity& entity)
{
	uint64_t id = entity.GetId();
	bool canAdd = this->entities.count(id) == 0;
	
	if (canAdd)
	{
		this->entities.insert({{id, &entity}});
	}
	
	return canAdd;
}

bool EntityRegistry::Unregister(const Entity& entity)
{
	uint64_t id = entity.GetId();
	bool contains = !!this->entities.count(id);
	
	if (contains)
	{
		this->entities.erase(id);
	}
	
	return contains;
}