#pragma once

#include <SFML/Window.hpp>

#include "../Utility/Types.h"
#include "../Utility/Event.h"
#include "Input.h"

struct WindowOptions
{
	uint32 Width = 800;
	uint32 Height = 600;

	uint32 DepthBits = 24;
	uint32 StencilBits = 8;
	uint32 AntiAliasing = 0;
	uint32 BitsPerPixel = 32;

	uint32 OGL_Major = 4;
	uint32 OGL_Minor = 3;

	const char* Title = "Wake Engine";

	bool Fullscreen = false;
	bool VerticalSync = false;
};

/**
 * Wrapper for SFML Window.
 */
class Window
{
public:
	Window();
	~Window();

	/**
	 * Create (or re-create) a window with specified options.
	 * \returns true on success, false on failure.
	 */
	bool Initialize(const WindowOptions& Options);

	void Deinitialize();

	void PollEvents();
	void Display();

	bool IsOpen() const;

	uint32 GetWidth() const;
	uint32 GetHeight() const;

	void SetTitle(const char* Title);

	/**
	 * Get the SFML window.
	 */
	sf::Window* GetRenderWindow();

	// Window Events

	W_EVENT(Closed);
	W_EVENT(Resized, uint32, uint32);
	W_EVENT(LostFocus);
	W_EVENT(GainedFocus);

	W_EVENT(TextEntered, uint32);
	W_EVENT(KeyPressed, EKeyboardInput);
	W_EVENT(KeyReleased, EKeyboardInput);

	W_EVENT(MouseButtonPressed, EMouseInput);
	W_EVENT(MouseButtonReleased, EMouseInput);
	W_EVENT(MouseWheelMoved, int32);

	W_EVENT(MouseMoved, int32, int32);
	W_EVENT(MouseEntered);
	W_EVENT(MouseLeft);

private:
	sf::Window* RenderWindow;
};