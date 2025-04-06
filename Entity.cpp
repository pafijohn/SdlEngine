#include "Utils.h"
#include "Entity.h"

Entity::Entity()
{
	this->hasCollision = false;
	this->GenId();
}

Entity::~Entity()
{
	EntityRegistry* registry = EntityRegistry::GetInst();
	registry->Unregister(*this);
}

void Entity::GenId()
{
	EntityRegistry* registry = EntityRegistry::GetInst();
	do
	{
		this->entityId = GetRandom64();
	} while (!registry->Register(*this));
}

uint64_t Entity::GetId() const
{
	return this->entityId;
}

void Entity::SetCollision(const SdlRect& rect)
{
	this->collisionBox = rect;
}

const SdlRect& Entity::GetCollision() const
{
	return this->collisionBox;
}


void Entity::SetHasCollision(bool collision)
{
	this->hasCollision = collision;
}

bool Entity::GetHasCollision()
{
	return this->hasCollision;
}

void Entity::OnCollision(const Entity& entity)
{
	
}
