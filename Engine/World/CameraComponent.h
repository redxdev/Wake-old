#pragma once

#include "Component.h"

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

	void SetAsWindow(float FieldOfView = 90.f, float NearPlane = 0.001f, float FarPlane = 1000.f);

private:
	float FieldOfView;
	float AspectRatio;
	float NearPlane;
	float FarPlane;
};