#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../ImGuiFeatures/LogViewer.h"

LUA_FUNCTION(Lua_GetCamera) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	Camera** ud = (Camera**)lua_newuserdata(L, sizeof(Camera*));
	*ud = room->GetCamera();
	luaL_setmetatable(L, lua::metatables::CameraMT);
	return 1;
}

LUA_FUNCTION(Lua_CameraSetFocusPosition)
{
	Camera* camera = *lua::GetUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SetFocusPosition(vector);
	//*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

LUA_FUNCTION(Lua_SnapToPosition)
{
	Camera* camera = *lua::GetUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SnapToPosition(*vector);
	*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

LUA_FUNCTION(Lua_IsPosVisible) {
	Camera* camera = *lua::GetUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, camera->IsPosVisible(vector));
	return 1;
}

static void RegisterCamera(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetCamera", Lua_GetCamera);

	luaL_Reg functions[] = {
		{ "SetFocusPosition", Lua_CameraSetFocusPosition },
		{ "SnapToPosition", Lua_SnapToPosition },
		{ "IsPosVisible", Lua_IsPosVisible },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::CameraMT, lua::metatables::CameraMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCamera(_state);
}