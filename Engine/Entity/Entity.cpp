#include "Entity.h"

namespace Wake
{
	namespace Entity
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
}