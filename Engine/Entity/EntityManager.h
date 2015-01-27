#pragma once

#include "../Util/Types.h"
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
				static_assert(std::is_base_of<Entity, T>(), "T must subclass Entity");
				T* Ent = new T(NextEntityId);
				NextEntityId++;

				return Ent;
			}

		private:
			EntityManager();
			~EntityManager();

			uint32 NextEntityId;
		};
	}
}