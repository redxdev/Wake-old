#pragma once

class Actor;

/**
 * A reusable class that can be attached to an Actor.
 */
class Component
{
public:
	Component(Actor* Parent, bool StartActive = true);
	virtual ~Component();

	/**
	 * Gets the parent actor (the actor this component is attached to).
	 */
	Actor* GetParent() const;

	bool IsActive() const;
	void Activate();
	void Deactivate();
	void ToggleActive();

	virtual void Spawn();
	virtual void Destroy();
	virtual void Draw();
	virtual void Tick();

protected:
	virtual void Activated();
	virtual void Deactivated();

private:
	Actor* Parent;
	bool Active;
};