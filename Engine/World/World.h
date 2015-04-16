#pragma once

#include <unordered_map>

#include "../Utility/Types.h"
#include "../Logging/LogMacros.h"

#include "Actor.h"

#define W_WORLD (World::Get())

/**
 * The game world.
 */
class World
{
	CLOG_LOGGER_DECL;
public:
	static World& Get();

	bool Startup();
	bool Shutdown();

	/**
	 * Spawn an actor.
	 */
	template<typename T>
	T* SpawnActor(bool StartActive = true)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T must subclass Actor");

		ActorID Id = CreateActorID();
		if (Id == 0)
		{
			CLOG_ERROR("Unable to create actor (CreateEntityID() == 0)");
			return nullptr;
		}

		T* Act = new T(Id, StartActive);
		Actors.emplace(Id, Act);

		Act->Spawn();

		return Act;
	}

	/**
	 * Spawn an actor.
	 */
	template<typename T, typename... V>
	T* SpawnActor(V... Args)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T must subclass Actor");

		ActorID Id = CreateActorID();
		if (Id == 0)
		{
			CLOG_ERROR("Unable to create actor (CreateActorID() == 0)");
			return nullptr;
		}

		T* Act = new T(Id, Args...);
		Actors.emplace(Id, Act);

		Act->Spawn();

		return Act;
	}

	/**
	 * Get the actor represented by an id.
	 */
	Actor* GetActor(ActorID Id);

	bool Destroy(Actor* Act);
	bool Destroy(ActorID Id);

	void Draw();
	void Tick();

private:
	World();
	~World();

	World(const World& Copy) {}

	ActorID CreateActorID();

	ActorID NextActorID;

	std::unordered_map<ActorID, Actor*> Actors;
};