#include "CameraComponent.h"

#include "Actor.h"
#include "../Engine/GEngine.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

CameraComponent::CameraComponent(Actor* Parent, bool StartActive)
	: Component(Parent, StartActive)
{
	SetAsWindow();
}

void CameraComponent::Tick()
{
	Component::Tick();

	glm::mat4 ViewMatrix = glm::translate(-GetParent()->GetPosition()) * glm::mat4_cast(GetParent()->GetRotation()) * glm::scale(GetParent()->GetScale());
	glm::mat4 ProjectionMatrix = glm::perspective(FieldOfView, AspectRatio, NearPlane, FarPlane);

	W_ENGINE.SetViewMatrix(ViewMatrix);
	W_ENGINE.SetProjectionMatrix(ProjectionMatrix);
}

void CameraComponent::SetFieldOfView(float FieldOfView)
{
	this->FieldOfView = FieldOfView;
}

void CameraComponent::SetAspectRatio(float AspectRatio)
{
	this->AspectRatio = AspectRatio;
}

void CameraComponent::SetNearPlane(float NearPlane)
{
	this->NearPlane = NearPlane;
}

void CameraComponent::SetFarPlane(float FarPlane)
{
	this->FarPlane = FarPlane;
}

float CameraComponent::GetFieldOfView() const
{
	return FieldOfView;
}

float CameraComponent::GetAspectRatio() const
{
	return AspectRatio;
}

float CameraComponent::GetNearPlane() const
{
	return NearPlane;
}

float CameraComponent::GetFarPlane() const
{
	return FarPlane;
}

void CameraComponent::SetAsWindow(float FieldOfView, float NearPlane, float FarPlane)
{
	SetFieldOfView(FieldOfView);
	SetNearPlane(NearPlane);
	SetFarPlane(FarPlane);

	if (W_ENGINE.GetGameWindow().IsOpen())
	{
		SetAspectRatio((float)W_ENGINE.GetGameWindow().GetWidth() / (float)W_ENGINE.GetGameWindow().GetHeight());
	}
}