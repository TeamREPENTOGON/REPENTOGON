#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetFXLayers) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	FXLayers** ud = (FXLayers**)lua_newuserdata(L, sizeof(FXLayers*));
	*ud = &room->_fxLayers;
	luaL_setmetatable(L, lua::metatables::FXLayersMT);
	return 1;
}

LUA_FUNCTION(Lua_FXLayersAddPoopFx)
{
	FXLayers* fxLayers = *lua::GetRawUserdata<FXLayers**>(L, 1, lua::metatables::FXLayersMT);
	ColorMod* color = lua::GetLuabridgeUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");
	fxLayers->AddPoopFx(color);

	return 0;
}

static void RegisterFXLayers(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetFXLayers", Lua_GetFXLayers);

	luaL_Reg functions[] = {
		{ "AddPoopFx", Lua_FXLayersAddPoopFx },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::FXLayersMT, lua::metatables::FXLayersMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterFXLayers(_state);
}