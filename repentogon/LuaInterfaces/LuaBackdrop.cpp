#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "LuaRender.h"

static Backdrop* GetBackdrop(lua_State* L, int idx = 1);

// ============================================================================
// Room

LUA_FUNCTION(lua_Room_GetBackdrop) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	Backdrop** ud = (Backdrop**)lua_newuserdata(L, sizeof(Backdrop*));
	*ud = room->GetBackdrop();
	luaL_setmetatable(L, lua::metatables::BackdropMT);
	return 1;
}

// ============================================================================
// Backdrop

Backdrop* GetBackdrop(lua_State* L, int idx) {
	return *(Backdrop**)luaL_checkudata(L, idx, lua::metatables::BackdropMT);
}

LUA_FUNCTION(lua_Backdrop_GetWallImage) {
	Backdrop* backdrop = GetBackdrop(L);
	LuaRender::LuaImage* image = new (lua_newuserdata(L, sizeof(LuaRender::LuaImage))) LuaRender::LuaImage;
	image->image = backdrop->wallsBuffer;
	luaL_setmetatable(L, LuaRender::ImageMT);
	return 1;
}

LUA_FUNCTION(lua_Backdrop_GetFloorImage) {
	Backdrop* backdrop = GetBackdrop(L);
	LuaRender::LuaImage* image = new (lua_newuserdata(L, sizeof(LuaRender::LuaImage))) LuaRender::LuaImage;
	image->image = backdrop->floorBuffer;
	luaL_setmetatable(L, LuaRender::ImageMT);
	return 1;
}

LUA_FUNCTION(lua_Backdrop_GetFloorANM2) {
	Backdrop* backdrop = GetBackdrop(L);
	ANM2* sprite = &backdrop->floorANM2;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(lua_Backdrop_GetDetailANM2) {
	Backdrop* backdrop = GetBackdrop(L);
	ANM2* sprite = &backdrop->_wallDetailANM2;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(lua_Backdrop_GetControlsANM2) {
	Backdrop* backdrop = GetBackdrop(L);
	ANM2* sprite = &backdrop->_controlsANM2;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

/*    //it has something to do with the size of the backdrop image. Maybe it can be helped to make the custom sized backdrop?
LUA_FUNCTION(lua_Backdrop_GetunkVec1) {
	Backdrop* backdrop = GetBackdrop(L);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = backdrop->unkVec1;
	return 1;
}

LUA_FUNCTION(lua_Backdrop_SetunkVec1) {
	Backdrop* backdrop = GetBackdrop(L);
	backdrop->unkVec1 = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(lua_Backdrop_GetunkVec2) {
	Backdrop* backdrop = GetBackdrop(L);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = backdrop->unkVec2;
	return 1;
}

LUA_FUNCTION(lua_Backdrop_SetunkVec2) {
	Backdrop* backdrop = GetBackdrop(L);
	backdrop->unkVec2 = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}
*/

LUA_FUNCTION(lua_Backdrop_GetControlsButtonsANM2) {
	Backdrop* backdrop = GetBackdrop(L);
	ANM2* sprite = &backdrop->_controlsButtonsANM2;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

static void RegisterBackdropClass(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetBackdrop", lua_Room_GetBackdrop);

	luaL_Reg functions[] = {
		{ "GetWallImage", lua_Backdrop_GetWallImage },
		{ "GetFloorImage", lua_Backdrop_GetFloorImage },
		{ "GetFloorANM2", lua_Backdrop_GetFloorANM2 },
		{ "GetDetailANM2", lua_Backdrop_GetDetailANM2 },
		{ "GetControlsANM2", lua_Backdrop_GetControlsANM2 },
		{ "GetControlsButtonsANM2", lua_Backdrop_GetControlsButtonsANM2 },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::BackdropMT, lua::metatables::BackdropMT, functions);
}

// ============================================================================
// Lua Engine

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBackdropClass(_state);
}
