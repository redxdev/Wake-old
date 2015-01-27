#include "EntityManager.h"

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
			CLOG_DEBUG("Starting EntityManager");
		}

		EntityManager::~EntityManager()
		{
			CLOG_DEBUG("Stopping EntityManager (last id = " << NextEntityId - 1 << ")");
		}


	}
}