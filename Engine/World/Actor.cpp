#include "Actor.h"

#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

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

const glm::vec3& Actor::getPosition() const
{
	return Position;
}

const glm::quat& Actor::getRotation() const
{
	return Rotation;
}

const glm::vec3& Actor::getScale() const
{
	return Scale;
}

glm::mat4x4 Actor::createMatrix() const
{
	return glm::translate(Position) * glm::scale(Scale) * glm::mat4_cast(Rotation);
}

void Actor::setPosition(const glm::vec3& Position)
{
	this->Position = Position;
}

void Actor::setRotation(const glm::quat& Rotation)
{
	this->Rotation = Rotation;
}

void Actor::setScale(const glm::vec3& Scale)
{
	this->Scale = Scale;
}

void Actor::Activated()
{
	
}

void Actor::Deactivated()
{
	
}