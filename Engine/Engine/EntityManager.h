#pragma once

#include <unordered_map>

#include "../Utility/Types.h"
#include "../Logging/LogMacros.h"
#include "Entity.h"

#define W_ENTITY (Engine::EntityManager::Get())

namespace Engine
{
	class EntityManager
	{
		CLOG_LOGGER_DECL;

	public:
		static EntityManager& Get();

		bool Startup();
		bool Shutdown();

		template<typename T>
		T* CreateEntity()
		{
			static_assert(std::is_base_of<Entity, T>::value, "T must subclass Entity");

			EID Id = CreateEntityId();
			if (Id == 0)
			{
				CLOG_ERROR("Unable to create entity (CreateEntityId() == 0)");
				return nullptr;
			}

			T* Ent = new T(CreateEntityId());
			Entities.emplace(Id, Ent);

			return Ent;
		}

		template<typename T, typename... V>
		T* CreateEntity(V... Args)
		{
			static_assert(std::is_base_of<Entity, T>::value, "T must subclass Entity");

			EID Id = CreateEntityId();
			if (Id == 0)
			{
				CLOG_ERROR("Unable to create entity (CreateEntityId() == 0)");
				return nullptr;
			}

			T* Ent = new T(CreateEntityId(), Args);
			Entities.emplace(Id, Ent);

			return Ent;
		}

		Entity* GetEntity(EID Id)
		{
			auto found = Entities.find(Id);
			if (found != Entities.end())
			{
				return found->second;
			}

			return nullptr;
		}

		bool Destroy(Entity* Ent);

		bool Destroy(EID EntId);

		void Draw();

		void Tick(float DeltaTime);

	private:
		EntityManager();
		~EntityManager();

		EntityManager(const EntityManager& Copy) {}

		EID CreateEntityId();

		EID NextEntityId;

		std::unordered_map<EID, Entity*> Entities;
	};
}