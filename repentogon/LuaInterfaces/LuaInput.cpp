#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_InputGetMousePosition)
{
	bool gameCoords = lua::luaL_checkboolean(L, 1);

	Vector* toLua = lua::ffi::placeCdata<Vector>(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR]);
	g_LuaEngine->GetMousePosition(toLua, gameCoords);

	return 1;
}

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
	Vector* toLua = lua::ffi::placeCdata<Vector>(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR]);
	Vector vec = Vector(WINMouseWheelMove_Hori, WINMouseWheelMove_Vert);
	memcpy(toLua, &vec, sizeof(Vector));
	return 1;
}



HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetMousePosition", Lua_InputGetMousePosition);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetDeviceNameByIdx", Lua_InputGetDeviceNameByIdx);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Input, "GetMouseWheel", Lua_InputGetMouseWheel);
}
