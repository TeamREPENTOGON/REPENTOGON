#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_InputGetDeviceNameByIdx)
{
	int cidx=(int)luaL_checkinteger(L, 1);
	char* device_name=g_InputManagerBase.GetDeviceNameByIdx(cidx);
	if (device_name){
		lua_pushstring(L,device_name);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

extern float WINMouseWheelMove_Vert;
extern float WINMouseWheelMove_Hori;

LUA_FUNCTION(Lua_InputGetMouseWheel)
{
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	Vector vec = Vector(WINMouseWheelMove_Hori, WINMouseWheelMove_Vert);
	memcpy(toLua, &vec, sizeof(Vector));
	return 1;
}

LUA_FUNCTION(Lua_InputGetActionButtons) {
	int action = (int)luaL_checkinteger(L, 1);
	int controlleridx = (int)luaL_checkinteger(L, 2);
	int buttons[2]{ -1, -1 };
	g_InputManagerBase.GetActionButtons(action, controlleridx, buttons);
	lua_pushinteger(L, buttons[0]);
	lua_pushinteger(L, buttons[1]);
	return 2;
}

LUA_FUNCTION(Lua_InputGetButtonFrame) {
	int button = (int)luaL_checkinteger(L, 1);
	int controlleridx = (int)luaL_checkinteger(L, 2);
	if (controlleridx == 0) {
		lua_pushinteger(L, button);
	} else {
		lua_pushinteger(L, InputManager::GetControllerButtonFrame(button));
	}
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetDeviceNameByIdx", Lua_InputGetDeviceNameByIdx);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetMouseWheel", Lua_InputGetMouseWheel);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetActionButtons", Lua_InputGetActionButtons);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetButtonFrame", Lua_InputGetButtonFrame);
}
