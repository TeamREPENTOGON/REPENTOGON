#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_HUDShowItemText)
{
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");

	if (lua_isstring(L, 2)) {
		char* mainString = const_cast<char*>(luaL_checkstring(L, 2));
		char* secondaryString = const_cast<char*>(luaL_checkstring(L, 3));
		bool isCurseDisplay = lua::luaL_optboolean(L, 4, false);
		bool stackUpText = lua::luaL_optboolean(L, 5, false);

		if (stackUpText)
			hud->ShowStackedItemTextCustomUTF8(mainString, secondaryString, false, isCurseDisplay);
		else {
			hud->_messageMain.Show(mainString, secondaryString, true, isCurseDisplay);
		}	
	}
	else {
		Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
		ItemConfig_Item* item = lua::GetCData<ItemConfig_Item*>(L, 3, lua::ffi::CData[lua::ffi::CDataID::ITEM], "Item");
		bool stackUpText = lua::luaL_optboolean(L, 4, true);

		if (stackUpText)
			hud->ShowStackedItemTextCustomUTF8(item->name.data(), item->desc.data(), false, false);
		else
			hud->ShowItemText(player, item);
	}

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterFunction(_state, lua::Metatables::HUD, "ShowItemText", Lua_HUDShowItemText);
}