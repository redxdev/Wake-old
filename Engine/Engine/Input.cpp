#include "Input.h"

#define C_AUTO(KeyName) case sf::Keyboard::KeyName: return EKeyboardInput::KeyName
#define C_MAN(KeyName, Input) case sf::Keyboard::KeyName: return EKeyboardInput::Input

EKeyboardInput ConvertSFKeyToInput(sf::Keyboard::Key Key)
{
	switch (Key)
	{
	default:
		return EKeyboardInput::Unknown;

		C_AUTO(A);
		C_AUTO(B);
		C_AUTO(C);
		C_AUTO(D);
		C_AUTO(E);
		C_AUTO(F);
		C_AUTO(G);
		C_AUTO(H);
		C_AUTO(I);
		C_AUTO(J);
		C_AUTO(K);
		C_AUTO(L);
		C_AUTO(M);
		C_AUTO(N);
		C_AUTO(O);
		C_AUTO(P);
		C_AUTO(Q);
		C_AUTO(R);
		C_AUTO(S);
		C_AUTO(T);
		C_AUTO(U);
		C_AUTO(V);
		C_AUTO(W);
		C_AUTO(X);
		C_AUTO(Y);
		C_AUTO(Z);
		C_AUTO(Num0);
		C_AUTO(Num1);
		C_AUTO(Num2);
		C_AUTO(Num3);
		C_AUTO(Num4);
		C_AUTO(Num5);
		C_AUTO(Num6);
		C_AUTO(Num7);
		C_AUTO(Num8);
		C_AUTO(Num9);
		C_AUTO(Escape);
		C_AUTO(LControl);
		C_AUTO(LShift);
		C_AUTO(LAlt);
		C_AUTO(LSystem);
		C_AUTO(RControl);
		C_AUTO(RShift);
		C_AUTO(RAlt);
		C_AUTO(RSystem);
		C_AUTO(Menu);
		C_AUTO(LBracket);
		C_AUTO(RBracket);
		C_AUTO(SemiColon);
		C_AUTO(Comma);
		C_AUTO(Period);
		C_AUTO(Quote);
		C_AUTO(Slash);
		C_AUTO(BackSlash);
		C_MAN(Tilde, BackTick);
		C_AUTO(Equal);
		C_AUTO(Dash);
		C_AUTO(Space);
		C_AUTO(Return);
		C_AUTO(BackSpace);
		C_AUTO(PageUp);
		C_AUTO(PageDown);
		C_AUTO(End);
		C_AUTO(Home);
		C_AUTO(Insert);
		C_AUTO(Delete);
		C_AUTO(Add);
		C_AUTO(Subtract);
		C_AUTO(Multiply);
		C_AUTO(Divide);
		C_AUTO(Left);
		C_AUTO(Right);
		C_AUTO(Up);
		C_AUTO(Down);
		C_AUTO(Numpad0);
		C_AUTO(Numpad1);
		C_AUTO(Numpad2);
		C_AUTO(Numpad3);
		C_AUTO(Numpad4);
		C_AUTO(Numpad5);
		C_AUTO(Numpad6);
		C_AUTO(Numpad7);
		C_AUTO(Numpad8);
		C_AUTO(Numpad9);
		C_AUTO(F1);
		C_AUTO(F2);
		C_AUTO(F3);
		C_AUTO(F4);
		C_AUTO(F5);
		C_AUTO(F6);
		C_AUTO(F7);
		C_AUTO(F8);
		C_AUTO(F9);
		C_AUTO(F10);
		C_AUTO(F11);
		C_AUTO(F12);
		C_AUTO(F13);
		C_AUTO(F14);
		C_AUTO(F15);
		C_AUTO(Pause);
	}
}

#undef C_MAN
#undef C_AUTO

#define C_AUTO(KeyName) case EKeyboardInput::KeyName: return #KeyName

const char* ConvertKeyboardToString(EKeyboardInput Key)
{
	switch (Key)
	{
	default:
		return "Unknown";

		C_AUTO(A);
		C_AUTO(B);
		C_AUTO(C);
		C_AUTO(D);
		C_AUTO(E);
		C_AUTO(F);
		C_AUTO(G);
		C_AUTO(H);
		C_AUTO(I);
		C_AUTO(J);
		C_AUTO(K);
		C_AUTO(L);
		C_AUTO(M);
		C_AUTO(N);
		C_AUTO(O);
		C_AUTO(P);
		C_AUTO(Q);
		C_AUTO(R);
		C_AUTO(S);
		C_AUTO(T);
		C_AUTO(U);
		C_AUTO(V);
		C_AUTO(W);
		C_AUTO(X);
		C_AUTO(Y);
		C_AUTO(Z);
		C_AUTO(Num0);
		C_AUTO(Num1);
		C_AUTO(Num2);
		C_AUTO(Num3);
		C_AUTO(Num4);
		C_AUTO(Num5);
		C_AUTO(Num6);
		C_AUTO(Num7);
		C_AUTO(Num8);
		C_AUTO(Num9);
		C_AUTO(Escape);
		C_AUTO(LControl);
		C_AUTO(LShift);
		C_AUTO(LAlt);
		C_AUTO(LSystem);
		C_AUTO(RControl);
		C_AUTO(RShift);
		C_AUTO(RAlt);
		C_AUTO(RSystem);
		C_AUTO(Menu);
		C_AUTO(LBracket);
		C_AUTO(RBracket);
		C_AUTO(SemiColon);
		C_AUTO(Comma);
		C_AUTO(Period);
		C_AUTO(Quote);
		C_AUTO(Slash);
		C_AUTO(BackSlash);
		C_AUTO(BackTick);
		C_AUTO(Equal);
		C_AUTO(Dash);
		C_AUTO(Space);
		C_AUTO(Return);
		C_AUTO(BackSpace);
		C_AUTO(PageUp);
		C_AUTO(PageDown);
		C_AUTO(End);
		C_AUTO(Home);
		C_AUTO(Insert);
		C_AUTO(Delete);
		C_AUTO(Add);
		C_AUTO(Subtract);
		C_AUTO(Multiply);
		C_AUTO(Divide);
		C_AUTO(Left);
		C_AUTO(Right);
		C_AUTO(Up);
		C_AUTO(Down);
		C_AUTO(Numpad0);
		C_AUTO(Numpad1);
		C_AUTO(Numpad2);
		C_AUTO(Numpad3);
		C_AUTO(Numpad4);
		C_AUTO(Numpad5);
		C_AUTO(Numpad6);
		C_AUTO(Numpad7);
		C_AUTO(Numpad8);
		C_AUTO(Numpad9);
		C_AUTO(F1);
		C_AUTO(F2);
		C_AUTO(F3);
		C_AUTO(F4);
		C_AUTO(F5);
		C_AUTO(F6);
		C_AUTO(F7);
		C_AUTO(F8);
		C_AUTO(F9);
		C_AUTO(F10);
		C_AUTO(F11);
		C_AUTO(F12);
		C_AUTO(F13);
		C_AUTO(F14);
		C_AUTO(F15);
		C_AUTO(Pause);
	}
}

EMouseInput ConvertSFMouseToInput(sf::Mouse::Button Button)
{
	switch (Button)
	{
	default:
		return EMouseInput::Unknown;

	case sf::Mouse::Left:
		return EMouseInput::Left;

	case sf::Mouse::Middle:
		return EMouseInput::Middle;

	case sf::Mouse::Right:
		return EMouseInput::Right;

	case sf::Mouse::XButton1:
		return EMouseInput::X1;

	case sf::Mouse::XButton2:
		return EMouseInput::X2;
	}
}

const char* ConvertMouseToString(EMouseInput Mouse)
{
	switch (Mouse)
	{
	default:
		return "Unknown";

	case EMouseInput::Left:
		return "Left";

	case EMouseInput::Middle:
		return "Middle";

	case EMouseInput::Right:
		return "Right";

	case EMouseInput::WheelUp:
		return "WheelUp";

	case EMouseInput::WheelDown:
		return "WheelDown";

	case EMouseInput::X1:
		return "X1";

	case EMouseInput::X2:
		return "X2";
	}
}

const char* ConvertInputTypeToString(EInputType Type)
{
	switch (Type)
	{
	default:
		return "Unknown";

	case EInputType::Keyboard:
		return "Keyboard";

	case EInputType::Mouse:
		return "Mouse";
	}
}

const char* ConvertInputModeToString(EInputMode Mode)
{
	switch (Mode)
	{
	default:
		return "Unknown";

	case EInputMode::Pressed:
		return "Pressed";

	case EInputMode::Released:
		return "Released";

	case EInputMode::Value:
		return "Value";
	}
}