#include "Entity.h"

namespace Engine
{
	Entity::Entity(uint32 Id)
	{
		EntityId = Id;
		Active = true;
	}

	void Entity::Spawn()
	{
	}

	void Entity::Destroy()
	{
	}

	void Entity::Tick(float DeltaTime)
	{
	}
}