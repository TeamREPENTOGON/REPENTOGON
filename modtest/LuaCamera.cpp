#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* CameraMT = "Camera";

static int Lua_GetCamera(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)room + 0x11F8;
	luaL_setmetatable(L, CameraMT);
	return 1;
}

int Lua_CameraSetFocusPosition(lua_State* L)
{
	//Camera* camera = *lua::GetUserdata<Camera**>(L, 1, CameraMT); // this is crashing and i'm not sure why, nor am i in the right mindset to try to debug this :upside_down: we'll cheat for now
	Vector* vector = lua::UserdataToData<Vector*>(lua_touserdata(L, 2));
	Room* room = *g_Game->GetCurrentRoom();
	Camera* camera = room->GetCamera();
	camera->SetFocusPosition(*vector);
	return 0;
}

int Lua_SnapToPosition(lua_State* L)
{
	Vector* vector = lua::UserdataToData<Vector*>(lua_touserdata(L, 2));
	Room* room = *g_Game->GetCurrentRoom();
	Camera* camera = room->GetCamera();
	camera->SnapToPosition(*vector);
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
	RegisterCamera(state);
}