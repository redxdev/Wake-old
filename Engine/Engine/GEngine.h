#pragma once

#include <glm/mat4x4.hpp>
#include <SFML/System/Clock.hpp>

#include "../Logging/LogMacros.h"
#include "Window.h"

#define W_ENGINE (GEngine::Get())

class GEngine
{
	CLOG_LOGGER_DECL;
public:
	static GEngine& Get();

	bool Startup(const WindowOptions& Options = WindowOptions());
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
