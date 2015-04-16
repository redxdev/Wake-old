#pragma once

#include "../Utility/Types.h"

#include <list>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

class Component;

/**
 * The most basic type of object in the game world. All objects in the world subclass Actor.
 */
class Actor
{
public:
	Actor(ActorID Id, bool StartActive = true);

	virtual ~Actor();

	/**
	 * Get the internal id for this actor. Use this id to keep references to actors unless
	 * you really need the speed of direct access and know what you're doing.
	 */
	ActorID GetActorID() const;

	/**
	 * Get the list of components on this actor.
	 */
	const std::list<Component*>& GetComponents() const;

	/**
	 * Create a component on this actor.
	 */
	template<typename T>
	T* CreateComponent(bool StartActive = true)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must subclass Component");

		T* Comp = new T(this, StartActive);
		Components.push_back(Comp);

		Comp->Spawn();

		return Comp;
	}

	/**
	 * Create a component on this actor.
	 */
	template<typename T, typename... V>
	T* CreateComponent(V... Args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must subclass Component");

		T* Comp = new T(this, Args...);
		Components.push_back(Comp);

		Comp->Spawn();

		return Comp;
	}

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

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

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

	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat();
	glm::vec3 Scale = glm::vec3(1, 1, 1);

	std::list<Component*> Components;
};