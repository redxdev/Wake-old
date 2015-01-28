#pragma once

#include "../Utility/Types.h"
#include "../Logging/LogMacros.h"
#include "Entity.h"

namespace Wake
{
	namespace Entity
	{
		class EntityManager
		{
			CLOG_LOGGER_DECL;

		public:
			static EntityManager& Get();

			template<typename T, typename... V>
			T* CreateEntity(V... Args)
			{
				static_assert(std::is_base_of<Entity, T>(), "T must subclass Entity");
				T* Ent = new T(CreateEntityId(), Args);

				return Ent;
			}

		private:
			EntityManager();
			~EntityManager();

			inline uint32 CreateEntityId()
			{
				return NextEntityId++;
			}

			uint32 NextEntityId;
		};
	}
}