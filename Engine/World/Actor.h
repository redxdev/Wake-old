#pragma once

#include "../Utility/Types.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

/**
 * The most basic type of object in the game world. All objects in the world subclass Actor.
 */
class Actor
{
public:
	Actor(ActorID Id, bool StartActive = true);

	virtual ~Actor();

	ActorID GetActorID() const;

	bool IsActive() const;
	void Activate();
	void Deactivate();
	void ToggleActive();

	virtual void Spawn();
	virtual void Destroy();
	virtual void Draw();
	virtual void Tick();

	const glm::vec3& GetPosition() const;
	const glm::quat& GetRotation() const;
	const glm::vec3& GetScale() const;

	glm::mat4x4 CreateMatrix() const;

	void SetPosition(const glm::vec3& Position);
	void SetRotation(const glm::quat& Rotation);
	void SetScale(const glm::vec3& Scale);

protected:
	/**
	 * Called when the actor has been activated.
	 */
	virtual void Activated();

	/**
	 * Called when the actor has been deactivated.
	 */
	virtual void Deactivated();

private:
	ActorID AID;
	bool Active;

	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;
};