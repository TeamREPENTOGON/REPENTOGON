#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../ImGuiFeatures/LogViewer.h"

static constexpr const char* CameraMT = "Camera";

static int Lua_GetCamera(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	Camera** ud = (Camera**)lua_newuserdata(L, sizeof(Camera*));
	*ud = room->GetCamera();
	luaL_setmetatable(L, CameraMT);
	return 1;
}

int Lua_CameraSetFocusPosition(lua_State* L)
{
	Camera* camera = *lua::GetUserdata<Camera**>(L, 1, CameraMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SetFocusPosition(vector);
	//*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

int Lua_SnapToPosition(lua_State* L)
{
	
	Camera* camera = *lua::GetUserdata<Camera**>(L, 1, CameraMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SnapToPosition(*vector);
	*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

static void RegisterCamera(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM);
	lua_pushstring(L, "GetCamera");
	lua_pushcfunction(L, Lua_GetCamera);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, CameraMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "SetFocusPosition", Lua_CameraSetFocusPosition },
		{ "SnapToPosition", Lua_SnapToPosition },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCamera(state);
}