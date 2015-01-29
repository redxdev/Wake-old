#pragma once

#include "../Utility/Types.h"

namespace Engine
{
	typedef uint32 EID;

	/**
	 * Base entity class.
	 */
	class Entity
	{
	public:
		explicit Entity(EID Id);

		virtual ~Entity()
		{
		}

		inline EID GetEntityId() const
		{
			return EntityId;
		}

		inline bool IsActive() const
		{
			return Active;
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

		virtual void Spawn();

		virtual void Destroy();

		virtual void Tick(float DeltaTime);

	private:
		EID EntityId;
		bool Active;
	};
}