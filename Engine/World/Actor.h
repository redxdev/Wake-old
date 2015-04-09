#pragma once

#include "../Utility/Types.h"

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
};