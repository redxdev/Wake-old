#include "LuaInput.h"

#include "../Engine/InputManager.h"
#include "LuaEvent.h"
#include "LuaLibRegistry.h"

static int l_bind(lua_State* L)
{
	const char* Name = luaL_checkstring(L, 1);

	InputBinding Binding;
	Binding.Type = (EInputType)(uint8)luaL_checkinteger(L, 2);
	Binding.Mode = (EInputMode)(uint8)luaL_checkinteger(L, 3);
	switch (Binding.Type)
	{
	default:
		break;

	case EInputType::Keyboard:
		Binding.Code.Keyboard = (EKeyboardInput)(uint8)luaL_checkinteger(L, 4);
		break;

	case EInputType::Mouse:
		Binding.Code.Mouse = (EMouseInput)(uint8)luaL_checkinteger(L, 4);
		break;
	}

	W_INPUT.Bind(Name, Binding);

	return 0;
}

static int l_event(lua_State* L)
{
	const char* Name = luaL_checkstring(L, 1);

	InputManager::InputEvent& Event = W_INPUT.Event(Name);
	PushLuaValue<const Input&>(L, Event);

	return 1;
}

static const struct luaL_reg inputlib_f[] = {
	{ "bind", l_bind },
	{ "event", l_event },
	{NULL, NULL}
};

static void add_constant(lua_State* L, const char* Name, int32 Value)
{
	lua_pushstring(L, Name);
	lua_pushnumber(L, Value);
	lua_settable(L, -3);
}

#define C_AUTO(KeyName) add_constant(L, #KeyName, (int32)EKeyboardInput::KeyName)
static void push_keyboard_constants(lua_State* L)
{
	lua_newtable(L);
	C_AUTO(Unknown);
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
#undef C_AUTO

static void push_mouse_constants(lua_State* L)
{
	lua_newtable(L);
	add_constant(L, "Unknown", (int32)EMouseInput::Unknown);
	add_constant(L, "Left", (int32)EMouseInput::Left);
	add_constant(L, "Right", (int32)EMouseInput::Right);
	add_constant(L, "X1", (int32)EMouseInput::X1);
	add_constant(L, "X2", (int32)EMouseInput::X2);
}

static void push_type_constants(lua_State* L)
{
	lua_newtable(L);
	add_constant(L, "Unknown", (int32)EInputType::Unknown);
	add_constant(L, "Mouse", (int32)EInputType::Mouse);
	add_constant(L, "Keyboard", (int32)EInputType::Keyboard);
}

static void push_mode_constants(lua_State* L)
{
	lua_newtable(L);
	add_constant(L, "Unknown", (int32)EInputMode::Unknown);
	add_constant(L, "Pressed", (int32)EInputMode::Pressed);
	add_constant(L, "Released", (int32)EInputMode::Released);
	add_constant(L, "Value", (int32)EInputMode::Value);
}

int luaopen_input(lua_State* L)
{
	luaL_register(L, "input", inputlib_f);
	
	lua_pushstring(L, "key");
	push_keyboard_constants(L);
	lua_settable(L, -3);

	lua_pushstring(L, "mouse");
	push_mouse_constants(L);
	lua_settable(L, -3);

	lua_pushstring(L, "type");
	push_type_constants(L);
	lua_settable(L, -3);

	lua_pushstring(L, "mode");
	push_mode_constants(L);
	lua_settable(L, -3);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_input);