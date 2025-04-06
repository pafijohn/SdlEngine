#pragma once

#include <unordered_map>

class Entity;

class EntityRegistry
{
private:
	std::unordered_map<uint64_t, Entity*> entities;
	
	EntityRegistry();
	
public:
	static EntityRegistry* GetInst();
	Entity* GetEntity(uint64_t id);
	bool Register(Entity& entity);
	bool Unregister(const Entity& entity);
};
