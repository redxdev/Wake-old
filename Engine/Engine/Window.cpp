#include "Window.h"

namespace Engine
{
	Window::Window()
	{
		RenderWindow = nullptr;
	}

	Window::~Window()
	{
		if (RenderWindow != nullptr)
		{
			if (RenderWindow->isOpen())
			{
				RenderWindow->close();
			}

			delete RenderWindow;
			RenderWindow = nullptr;
		}
	}

	bool Window::Initialize(const WindowOptions& Options)
	{
		sf::ContextSettings CtxSettings;
		CtxSettings.antialiasingLevel = Options.AntiAliasing;
		CtxSettings.depthBits = Options.DepthBits;
		CtxSettings.stencilBits = Options.StencilBits;
		CtxSettings.majorVersion = Options.OGL_Major;
		CtxSettings.minorVersion = Options.OGL_Minor;

		if (!RenderWindow)
		{
			RenderWindow = new sf::Window();
		}

		RenderWindow->create(sf::VideoMode(Options.Width, Options.Height, Options.BitsPerPixel), Options.Title, Options.Fullscreen ? sf::Style::Fullscreen : sf::Style::Default, CtxSettings);

		return true;
	}

	void Window::Deinitialize()
	{
		if (RenderWindow != nullptr)
		{
			if (RenderWindow->isOpen())
			{
				RenderWindow->close();
			}

			delete RenderWindow;
			RenderWindow = nullptr;
		}
	}
}