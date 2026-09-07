#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_HUDShowItemText)
{
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");

	if (lua_isstring(L, 2) | lua_isstring(L, 3)) {
		char* mainString = const_cast<char*>(luaL_optstring(L, 2, ""));
		char* secondaryString = const_cast<char*>(luaL_optstring(L, 3, ""));
		bool isCurseDisplay = lua::luaL_optboolean(L, 4, false);
		bool stackUpText = lua::luaL_optboolean(L, 5, true);

		if (stackUpText)
			hud->ClearStackedItemText();
		hud->ShowStackedItemTextCustomUTF8(mainString, secondaryString, false, isCurseDisplay);
	}
	else {
		Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
		ItemConfig_Item* item = lua::GetCData<ItemConfig_Item*>(L, 3, lua::ffi::CData[lua::ffi::CDataID::ITEM], "Item");
		bool stackUpText = lua::luaL_optboolean(L, 4, true);

		if (stackUpText)
			hud->ClearStackedItemText();
		hud->ShowItemText(player, item);
	}

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterFunction(_state, lua::Metatables::HUD, "ShowItemText", Lua_HUDShowItemText);
}