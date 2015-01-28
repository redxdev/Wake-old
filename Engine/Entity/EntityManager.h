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

			template<typename T>
			T* CreateEntity()
			{
				static_assert(std::is_base_of<Entity, T>::value, "T must subclass Entity");

				uint32 Id = CreateEntityId();
				if (Id == 0)
				{
					CLOG_ERROR("Unable to create entity (CreateEntityId() == 0)");
					return nullptr;
				}

				T* Ent = new T(CreateEntityId());
				return Ent;
			}

			template<typename T, typename... V>
			T* CreateEntity(V... Args)
			{
				static_assert(std::is_base_of<Entity, T>::value, "T must subclass Entity");

				uint32 Id = CreateEntityId();
				if (Id == 0)
				{
					CLOG_ERROR("Unable to create entity (CreateEntityId() == 0)");
					return nullptr;
				}

				T* Ent = new T(CreateEntityId(), Args);
				return Ent;
			}

			void Startup();
			void Shutdown();

		private:
			EntityManager();
			~EntityManager();

			EntityManager(const EntityManager& Copy) {}

			uint32 CreateEntityId();

			uint32 NextEntityId;
		};
	}
}