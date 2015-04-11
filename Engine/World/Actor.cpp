#include "Actor.h"

#include "Component.h"

#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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

const std::list<Component*>& Actor::GetComponents() const
{
	return Components;
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
	if (Active)
	{
		Activate();
	}
}

void Actor::Destroy()
{
	for (auto Comp : Components)
	{
		Comp->Deactivate();
		Comp->Destroy();
		delete Comp;
	}

	Components.clear();
}

void Actor::Draw()
{
	for (auto Comp : Components)
	{
		if (Comp->IsActive())
		{
			Comp->Draw();
		}
	}
}

void Actor::Tick()
{
	for (auto Comp : Components)
	{
		if (Comp->IsActive())
		{
			Comp->Tick();
		}
	}
}

const glm::vec3& Actor::GetPosition() const
{
	return Position;
}

const glm::quat& Actor::GetRotation() const
{
	return Rotation;
}

const glm::vec3& Actor::GetScale() const
{
	return Scale;
}

glm::mat4x4 Actor::CreateMatrix() const
{
	return glm::translate(Position) * glm::mat4_cast(Rotation) * glm::scale(Scale);
}

void Actor::SetPosition(const glm::vec3& Position)
{
	this->Position = Position;
}

void Actor::SetRotation(const glm::quat& Rotation)
{
	this->Rotation = Rotation;
}

void Actor::SetScale(const glm::vec3& Scale)
{
	this->Scale = Scale;
}

void Actor::LookAt(const glm::vec3& Point)
{
	glm::mat4 transform = glm::lookAt(Position, Point, glm::vec3(0, 1, 0));
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transform, scale, rotation, translation, skew, perspective);

	SetRotation(rotation);
}


void Actor::Activated()
{
	
}

void Actor::Deactivated()
{
	
}