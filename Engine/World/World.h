#pragma once

#include <unordered_map>

#include "../Utility/Types.h"
#include "../Logging/LogMacros.h"

#include "Actor.h"

#define W_WORLD (World::Get())

class World
{
	CLOG_LOGGER_DECL;
public:
	static World& Get();

	bool Startup();
	bool Shutdown();

	template<typename T>
	T* SpawnActor()
	{
		static_assert(std::is_base_of<Actor, T>::value, "T must subclass Actor");

		ActorID Id = CreateActorID();
		if (Id == 0)
		{
			CLOG_ERROR("Unable to create actor (CreateEntityID() == 0)");
			return nullptr;
		}

		T* Act = new T(CreateActorID());
		Actors.emplace(Id, Act);

		return Act;
	}

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

		T* Act = new T(CreateActorID(), Args);
		Actors.emplace(Id, Act);

		return Act;
	}

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