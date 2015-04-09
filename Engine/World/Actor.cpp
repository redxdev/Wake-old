#include "Actor.h"

Actor::Actor(ActorID Id, bool StartActive)
{
	AID = Id;
	Active = StartActive;
}

Actor::~Actor()
{
	
}


ActorID Actor::GetActorID() const
{
	return AID;
}

bool Actor::IsActive() const
{
	return Active;
}

void Actor::Activate()
{
	if (!Active)
	{
		Active = true;
		Activated();
	}
}

void Actor::Deactivate()
{
	if (Active)
	{
		Active = false;
		Deactivated();
	}
}

void Actor::ToggleActive()
{
	if (Active)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}
}

void Actor::Spawn()
{
	
}

void Actor::Destroy()
{
	
}

void Actor::Draw()
{
	
}

void Actor::Tick()
{
	
}

void Actor::Activated()
{
	
}

void Actor::Deactivated()
{
	
}