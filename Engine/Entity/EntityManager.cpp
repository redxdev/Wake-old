#include "EntityManager.h"
#include <assert.h>

namespace Wake
{
	namespace Entity
	{
		CLOG_LOGGER_DEF(EntityManager);

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

		void EntityManager::Startup()
		{
			CLOG_DEBUG("Starting entity manager");
			NextEntityId = 1;
		}

		void EntityManager::Shutdown()
		{
			CLOG_DEBUG("Shutting down entity manager");
		}

		uint32 EntityManager::CreateEntityId()
		{
			if (NextEntityId == std::numeric_limits<uint32>::max())
			{
				CLOG_ERROR("Unable to generate new entity id (limit reached)");
				return 0;
			}

			return NextEntityId++;
		}

	}
}