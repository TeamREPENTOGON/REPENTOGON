#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../ImGuiFeatures/LogViewer.h"
#include "../Patches/ASMPatches/ASMCamera.h"

LUA_FUNCTION(Lua_GetCamera) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	Camera** ud = (Camera**)lua_newuserdata(L, sizeof(Camera*));
	*ud = room->GetCamera();
	luaL_setmetatable(L, lua::metatables::CameraMT);
	return 1;
}

LUA_FUNCTION(Lua_CameraSetFocusPosition)
{
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SetFocusPosition(vector);
	//*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

LUA_FUNCTION(Lua_SnapToPosition)
{
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	camera->SnapToPosition(*vector);
	*camera->ShouldOverride() = true;
	logViewer.AddLog("[REPENTOGON]", "camera override is %s\n", *camera->ShouldOverride() ? "TRUE" : "FALSE");
	return 0;
}

LUA_FUNCTION(Lua_CameraUpdate) {
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	bool flag = lua::luaL_optboolean(L, 2, false);
	camera->DoUpdate(flag);
	return 0;
}

LUA_FUNCTION(Lua_IsPosVisible) {
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	Vector* vector = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, camera->IsPosVisible(vector));
	return 1;
}

LUA_FUNCTION(Lua_IsClampEnabled) {
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	lua_pushboolean(L, roomCameraData.Clamped);
	return 1;
}

LUA_FUNCTION(Lua_SetClampEnabled) {
	Camera* camera = *lua::GetRawUserdata<Camera**>(L, 1, lua::metatables::CameraMT);
	bool clamped = lua::luaL_checkboolean(L, 2);
	roomCameraData.Clamped = clamped;
	return 0;
}

static void RegisterCamera(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetCamera", Lua_GetCamera);

	luaL_Reg functions[] = {
		{ "SetFocusPosition", Lua_CameraSetFocusPosition },
		{ "SnapToPosition", Lua_SnapToPosition },
		{ "IsPosVisible", Lua_IsPosVisible },
		{ "Update", Lua_CameraUpdate },
		{ "IsClampEnabled", Lua_IsClampEnabled },
		{ "SetClampEnabled", Lua_SetClampEnabled },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::CameraMT, lua::metatables::CameraMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCamera(_state);
}