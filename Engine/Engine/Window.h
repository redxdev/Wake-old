#pragma once

#include <SFML/Window.hpp>

#include "../Utility/Types.h"

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
		const char* Title;
		bool Fullscreen = false;
	};

	class Window
	{
	public:
		Window();
		~Window();

		void Initialize(const WindowOptions& Options);

	private:
		sf::Window* RenderWindow;
	};
}