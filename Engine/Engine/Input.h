#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../Utility/Types.h"
#include "../Utility/LuaExt.h"

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

const char* ConvertKeyboardToString(EKeyboardInput Key);

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

const char* ConvertMouseToString(EMouseInput Mouse);

enum class EInputType : uint8
{
	Unknown,
	Mouse,
	Keyboard
};

const char* ConvertInputTypeToString(EInputType Type);

enum class EInputMode : uint8
{
	Unknown,
	Pressed,
	Released,
	Value
};

const char* ConvertInputModeToString(EInputMode Mode);

struct InputCode
{
	InputCode()
	{
	}

	InputCode(EMouseInput Mouse)
	{
		this->Mouse = Mouse;
	}

	InputCode(EKeyboardInput Keyboard)
	{
		this->Keyboard = Keyboard;
	}

	union
	{
		EMouseInput Mouse;
		EKeyboardInput Keyboard;
	};
};

struct Input
{
	EInputType Type;
	EInputMode Mode;
	float Value;
	InputCode Code;
};

void PushLuaValue(lua_State* L, EKeyboardInput Key);
void PushLuaValue(lua_State* L, EMouseInput Button);
void PushLuaValue(lua_State* L, EInputMode Mode);
void PushLuaValue(lua_State* L, EInputType Type);
void PushLuaValue(lua_State* L, const Input& Input);