#pragma once

#include "../World/Actor.h"
#include "../World/StaticMeshComponent.h"
#include "../Logging/LogMacros.h"

class FallingObject : public Actor
{
	CLOG_LOGGER_DECL;

public:
	FallingObject(ActorID Id, bool StartActive, ShaderProgram* Program, StaticMesh* Mesh, ActorID Player, float InitialX);

	virtual ~FallingObject();

	virtual void Spawn() override;

	virtual void Tick() override;

	inline void SetSpeed(float Val) {Speed = Val;}

private:
	StaticMeshComponent* MeshComponent;

	ShaderProgram* Program;
	StaticMesh* Mesh;

	float Speed;

	ActorID PlayerID;
	Actor* Player;

	float InitialX = 0;
};