#pragma once

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

private:
	GEngine();
	~GEngine();

	Window GameWindow;

	bool Running;
};
