#pragma once

#include "SdlRect.h"
#include "EntityRegistry.h"

class Entity
{
private:
	uint64_t entityId;
	SdlRect collisionBox;
	bool hasCollision;
	
public:
	Entity();
	virtual ~Entity();
	void GenId();
	uint64_t GetId() const;
	
	void SetCollision(const SdlRect& rect);
	const SdlRect& GetCollision() const;
	
	void SetHasCollision(bool collision);
	bool GetHasCollision();
	
	virtual void OnCollision(const Entity& entity);
};