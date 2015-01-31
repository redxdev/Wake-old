#include "EntityManager.h"
#include <assert.h>

namespace Engine
{
	CLOG_LOGGER_DEF(Engine::EntityManager);

	EntityManager& EntityManager::Get()
	{
		static EntityManager Instance;
		return Instance;
	}

	EntityManager::EntityManager()
	{
	}

	EntityManager::~EntityManager()
	{
	}

	bool EntityManager::Startup()
	{
		CLOG_DEBUG("Starting entity manager");
		NextEntityId = 1;

		return true;
	}

	bool EntityManager::Shutdown()
	{
		CLOG_DEBUG("Shutting down entity manager");

		CLOG_DEBUG("Cleaning up " << Entities.size() << " entities");
		for (auto Itr = Entities.begin(); Itr != Entities.end(); ++Itr)
		{
			Itr->second->Deactivate();
			Itr->second->Destroy();
			delete Itr->second;
		}

		Entities.clear();

		return true;
	}

	uint32 EntityManager::CreateEntityId()
	{
		if (NextEntityId == std::numeric_limits<EID>::max())
		{
			CLOG_ERROR("Unable to generate new entity id (limit reached)");
			return 0;
		}

		return NextEntityId++;
	}

	void EntityManager::Destroy(Entity* Ent)
	{
		if (!Ent)
			return;

		Destroy(Ent->GetEntityId());
	}

	void EntityManager::Destroy(EID EntId)
	{
		Entity* Ent = GetEntity(EntId);
		if (!Ent)
			return;

		Ent->Deactivate();
		Ent->Destroy();
		Entities.erase(EntId);

		delete Ent;
	}

}