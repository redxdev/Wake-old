#pragma once

class Actor;

class Component
{
public:
	Component(Actor* Parent, bool StartActive = true);
	virtual ~Component();

	Actor* GetParent();

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