#pragma once

#include <glm/mat4x4.hpp>
#include <SFML/System/Clock.hpp>

#include "../Logging/LogMacros.h"
#include "../Utility/luaext.h"
#include "Window.h"

#define W_ENGINE (GEngine::Get())

/**
 * Handles the main game loop, the render window, and render/tick events.
 */
class GEngine
{
	CLOG_LOGGER_DECL;
public:
	static GEngine& Get();

public:
	bool Startup();
	bool Shutdown();

	void Run();
	void Stop();

	Window& GetGameWindow();

	bool IsRunning();

	void SetViewMatrix(const glm::mat4& Matrix);
	void SetProjectionMatrix(const glm::mat4& Matrix);

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	float GetDeltaTime() const;

	W_EVENT(Tick);
	W_EVENT(Draw);

private:
	GEngine();
	~GEngine();

	void Render();

	Window GameWindow;

	bool Running;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	
	sf::Clock DeltaClock;
	float DeltaTime;
};
