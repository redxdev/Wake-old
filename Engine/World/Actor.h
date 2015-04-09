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

	const glm::vec3& getPosition() const;
	const glm::quat& getRotation() const;
	const glm::vec3& getScale() const;

	glm::mat4x4 createMatrix() const;

	void setPosition(const glm::vec3& Position);
	void setRotation(const glm::quat& Rotation);
	void setScale(const glm::vec3& Scale);

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