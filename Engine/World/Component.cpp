#include "Component.h"

#include "Actor.h"

Component::Component(Actor* Parent, bool StartActive)
{
	this->Parent = Parent;
	this->Active = StartActive;
}

Component::~Component()
{
}

Actor* Component::GetParent() const
{
	return Parent;
}

bool Component::IsActive() const
{
	return Active;
}

void Component::Activate()
{
	if (!Active)
	{
		Active = true;
		Activated();
	}
}

void Component::Deactivate()
{
	if (Active)
	{
		Active = false;
		Deactivated();
	}
}

void Component::ToggleActive()
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

void Component::Spawn()
{
	if (Active)
	{
		Activate();
	}
}

void Component::Destroy()
{

}

void Component::Draw()
{

}

void Component::Tick()
{

}

void Component::Activated()
{
	
}

void Component::Deactivated()
{
	
}

