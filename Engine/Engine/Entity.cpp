#include "Entity.h"

namespace Engine
{
	Entity::Entity(EID Id)
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

	void Entity::Draw()
	{
	}

	void Entity::Tick(float DeltaTime)
	{
	}
}
