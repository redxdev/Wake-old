#pragma once

#include "../Util/Types.h"

namespace Wake
{
	namespace Entity
	{
		/**
		 * Base entity class.
		 */
		class Entity
		{
		public:
			virtual ~Entity()
			{
			}

			inline uint32 GetEntityId() const
			{
				return EntityId;
			}

			inline bool IsActive() const
			{
				return IsActive;
			}

			inline void Activate()
			{
				Active = true;
			}

			inline void Deactivate()
			{
				Active = false;
			}

			inline void ToggleActive()
			{
				Active = !Active;
			}

			virtual void Tick(float DeltaTime);

		protected:
			explicit Entity(uint32 Id);

		private:
			uint32 EntityId;
			bool Active;
		};
	}
}