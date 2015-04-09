#include "World.h"

CLOG_LOGGER_DEF(World);

World& World::Get()
{
	static World Instance;
	return Instance;
}

World::World()
{
	NextActorID = 0;
}

World::~World()
{
	
}

bool World::Startup()
{
	CLOG_DEBUG("Starting world");

	NextActorID = 1;

	return true;
}

bool World::Shutdown()
{
	CLOG_DEBUG("Shutting down world");

	CLOG_DEBUG("Cleaning up " << Actors.size() << " actors");
	for (auto Itr = Actors.begin(); Itr != Actors.end(); ++Itr)
	{
		Itr->second->Deactivate();
		Itr->second->Destroy();
		delete Itr->second;
	}

	Actors.clear();

	return true;
}

ActorID World::CreateActorID()
{
	if (NextActorID == 0)
	{
		CLOG_ERROR("Unable to generate new actor id (next is 0/ERR)");
		return 0;
	}

	if (NextActorID == std::numeric_limits<ActorID>::max())
	{
		CLOG_ERROR("Unable to generate new actor id (limit reached)");
		return 0;
	}

	return NextActorID++;
}


bool World::Destroy(Actor* Act)
{
	if (!Act)
		return false;

	return Destroy(Act->GetActorID());
}

bool World::Destroy(ActorID Id)
{
	Actor* Act = GetActor(Id);
	if (!Act)
		return false;

	Act->Deactivate();
	Act->Destroy();
	Actors.erase(Id);

	delete Act;

	return true;
}

Actor* World::GetActor(ActorID Id)
{
	auto found = Actors.find(Id);
	if (found == Actors.end())
	{
		return nullptr;
	}

	return found->second;
}


void World::Draw()
{
	for (auto Itr = Actors.begin(); Itr != Actors.end(); ++Itr)
	{
		if (!Itr->second->IsActive())
			continue;

		Itr->second->Draw();
	}
}

void World::Tick()
{
	for (auto Itr = Actors.begin(); Itr != Actors.end(); ++Itr)
	{
		if (!Itr->second->IsActive())
			continue;

		Itr->second->Tick();
	}
}