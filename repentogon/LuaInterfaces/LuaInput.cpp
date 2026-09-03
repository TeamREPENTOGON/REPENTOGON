#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

extern float WINMouseWheelMove_Vert;
extern float WINMouseWheelMove_Hori;

MOD_EXPORT float L_Input_GetActionValue(int action, int controllerId) {
	return g_InputManagerBase.GetActionValue(action, controllerId, 0);
}

MOD_EXPORT float L_Input_GetButtonValue(int button, int controllerId) {
	return LuaEngine::GetButtonValue(button, controllerId);
}

MOD_EXPORT char* L_Input_GetDeviceNameByIdx(int controllerId) {
	return g_InputManagerBase.GetDeviceNameByIdx(controllerId);
}

MOD_EXPORT void L_Input_GetMousePosition(bool gameCoords, Vector* out) {
	g_LuaEngine->GetMousePosition(out, gameCoords);
}

MOD_EXPORT void L_Input_GetMouseWheel(Vector* out) {
	*out = Vector(WINMouseWheelMove_Hori, WINMouseWheelMove_Vert);
}

MOD_EXPORT bool L_Input_IsActionPressed(int action, int controllerId) {
	return g_InputManagerBase.IsActionPressed(action, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsActionTriggered(int action, int controllerId) {
	return g_InputManagerBase.IsActionTriggered(action, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsButtonPressed(int button, int controllerId) {
	return g_InputManagerBase.IsButtonPressed(button, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsButtonTriggered(int button, int controllerId) {
	return g_InputManagerBase.IsButtonTriggered(button, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsMouseBtnPressed(uint32_t button) {
	return LuaEngine::IsMouseButtonPressed(button);
}

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
