#include "FallingObject.h"

#include "../Engine/GEngine.h"
#include "../World/World.h"

CLOG_LOGGER_DEF(FallingObject);

FallingObject::FallingObject(ActorID Id, bool StartActive, ShaderProgram* Program, StaticMesh* Mesh, ActorID Player, float InitialX)
	: Actor(Id, StartActive), Program(Program), Mesh(Mesh), PlayerID(Player), InitialX(InitialX)
{
	
}

FallingObject::~FallingObject()
{
	
}

void FallingObject::Spawn()
{
	Actor::Spawn();

	MeshComponent = CreateComponent<StaticMeshComponent>(true, Mesh, Program);
	MeshComponent->SetB(0.8f);

	Player = W_WORLD.GetActor(PlayerID);
	SetPosition(GetPosition() + glm::vec3(InitialX, 0, 0));
}

void FallingObject::Tick()
{
	Actor::Tick();

	SetPosition(GetPosition() + glm::vec3(0, -Speed, 0) * W_ENGINE.GetDeltaTime());

	if (GetPosition()[1] < -1.5)
	{
		SetPosition(glm::vec3(InitialX, 1.5f, GetPosition()[2]));
	}

	float Distance = glm::distance(GetPosition(), Player->GetPosition());
	if (Distance < 0.7f)
	{
		MeshComponent->SetR(Distance);
		float Dir = GetPosition()[0] < Player->GetPosition()[0] ? -1.f : 1.f;
		SetPosition(GetPosition() + glm::vec3(Dir * 0.5, 0.f, 0.f) * W_ENGINE.GetDeltaTime());
	}
	else
	{
		MeshComponent->SetB(0.8f);
		MeshComponent->SetR(0.0f);
	}
}