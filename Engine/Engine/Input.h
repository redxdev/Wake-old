#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../Utility/Types.h"

namespace Engine
{
	enum class EKeyboardInput : uint8
	{
		Unknown,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Escape,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShift,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		SemiColon,
		Comma,
		Period,
		Quote,
		Slash,
		BackSlash,
		BackTick,
		Equal,
		Dash,
		Space,
		Return,
		BackSpace,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause
	};

	EKeyboardInput ConvertSFKeyToInput(sf::Keyboard::Key Key);

	enum class EMouseInput : uint8
	{
		Unknown,
		Left,
		Middle,
		Right,
		X1,
		X2,
		WheelUp,
		WheelDown
	};

	EMouseInput ConvertSFMouseToInput(sf::Mouse::Button Button);

	enum class EInputType : uint8
	{
		Mouse,
		Keyboard,
		Axis
	};

	enum class EInputMode : uint8
	{
		Pressed,
		Released,
		Value
	};

	struct Input
	{
		EInputType Type;
		EInputMode Mode;
		float Value;

		union
		{
			EMouseInput Mouse;
			EKeyboardInput Keyboard;
		};
	};
}