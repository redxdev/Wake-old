#pragma once

#include <SFML/Window.hpp>

#include "../Utility/Types.h"
#include "../Utility/Event.h"

namespace Engine
{
	struct WindowOptions
	{
		uint32 Width = 800;
		uint32 Height = 600;
		uint32 DepthBits = 24;
		uint32 StencilBits = 8;
		uint32 AntiAliasing = 0;
		uint32 BitsPerPixel = 32;
		uint32 OGL_Major = 3;
		uint32 OGL_Minor = 0;
		const char* Title = "Wake Engine";
		bool Fullscreen = false;
	};

	class Window
	{
	public:
		Window();
		~Window();

		bool Initialize(const WindowOptions& Options);

		void Deinitialize();

		void PollEvents();

		W_EVENT(Closed);
		W_EVENT(Resized, uint32, uint32);
		W_EVENT(LostFocus);
		W_EVENT(GainedFocus);

		W_EVENT(TextEntered, uint32);

		W_EVENT(MouseMoved, int32, int32);
		W_EVENT(MouseEntered);
		W_EVENT(MouseLeft);

	private:
		sf::Window* RenderWindow;
	};
}