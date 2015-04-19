#pragma once

#include "../World/Actor.h"
#include "../World/StaticMeshComponent.h"
#include "../Logging/LogMacros.h"
#include "../Engine/Input.h"

class Player : public Actor
{
	CLOG_LOGGER_DECL;

public:
	Player(ActorID Id, bool StartActive, ShaderProgram* Program, StaticMesh* Mesh);

	virtual ~Player();

	virtual void Spawn() override;

	virtual void Tick() override;

private:
	void Left(const Input& Input);
	void Right(const Input& Input);
	void Jump(const Input& Input);

	StaticMeshComponent* MeshComponent;

	ShaderProgram* Program;
	StaticMesh* Mesh;

	glm::vec3 Velocity;

	bool HitGround = false;
};