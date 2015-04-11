#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* Parent, bool StartActive = true);

	virtual void Tick() override;

	void SetFieldOfView(float FieldOfView);
	void SetAspectRatio(float AspectRatio);
	void SetNearPlane(float NearPlane);
	void SetFarPlane(float FarPlane);

	float GetFieldOfView() const;
	float GetAspectRatio() const;
	float GetNearPlane() const;
	float GetFarPlane() const;

	void SetAsWindow(float FieldOfView = 45.f, float NearPlane = 0.1f, float FarPlane = 100.f);

private:
	float FieldOfView;
	float AspectRatio;
	float NearPlane;
	float FarPlane;
};