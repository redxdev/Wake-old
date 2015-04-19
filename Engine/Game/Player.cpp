#include "Player.h"

#include "../Engine/GEngine.h"
#include "../Engine/InputManager.h"

CLOG_LOGGER_DEF(Player);

Player::Player(ActorID Id, bool StartActive, ShaderProgram* Program, StaticMesh* Mesh)
	: Actor(Id, StartActive), Program(Program), Mesh(Mesh)
{
}

Player::~Player()
{
	
}

void Player::Spawn()
{
	Actor::Spawn();

	// Better to create components in Spawn() than in the constructor
	MeshComponent = CreateComponent<StaticMeshComponent>(true, Mesh, Program);
	MeshComponent->SetR(1.f);

	// W_INPUT.Bind creates a named binding if it doesn't exist, and adds
	// a set of keys to that binding.
	// The names themselves aren't used by the input system (they are only used when you actually
	// create the bindings), so there's no overhead (beyond setting up bindings) to using
	// strings for the names.
	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Pressed, Left));
	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Released, Left));
	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Pressed, Right));
	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Released, Right));
	W_INPUT.Bind("Jump", INPUT_BIND(Keyboard, Pressed, Up));
	W_INPUT.Bind("Jump", INPUT_BIND(Keyboard, Pressed, Space));

	// W_INPUT.Event returns the event object for a named binding.
	// Bind adds a function to that event.
	W_INPUT.Event("Left").Bind(this, &Player::Left);
	W_INPUT.Event("Right").Bind(this, &Player::Right);
	W_INPUT.Event("Jump").Bind(this, &Player::Jump);
}

void Player::Tick()
{
	Actor::Tick();

	// Gravity
	Velocity += glm::vec3(0, -2, 0) * W_ENGINE.GetDeltaTime();

	// Velocity -> Position
	SetPosition(GetPosition() + Velocity * W_ENGINE.GetDeltaTime());

	// Clamp to the bottom of the screen
	if (GetPosition()[1] < -1)
	{
		Velocity = glm::vec3(Velocity[0], 0, 0);
		SetPosition(glm::vec3(GetPosition()[0], -1, 0));
		HitGround = true;
	}

	SetPosition(glm::vec3(glm::clamp(GetPosition()[0], -1.f + GetScale()[0], 1.f - GetScale()[0]), GetPosition()[1], GetPosition()[2]));
}

void Player::Left(const Input& Input)
{
	float Direction = Input.Mode == EInputMode::Pressed ? 1.f : -1.f;
	Velocity += glm::vec3(Direction * -1.f, 0, 0);
}

void Player::Right(const Input& Input)
{
	float Direction = Input.Mode == EInputMode::Pressed ? 1.f : -1.f;
	Velocity += glm::vec3(Direction * 1.f, 0, 0);
}

void Player::Jump(const Input& Input)
{
	if (HitGround)
	{
		HitGround = false;
		Velocity += glm::vec3(0, 1, 0);
	}
}