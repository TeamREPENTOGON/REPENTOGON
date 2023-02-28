#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

// Test function
static int lua_EntitySlot_REPENTOGOING(lua_State* L) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushstring(L, "REPENTOGOING !");
	lua_pushinteger(L, *(slot->GetType()));
	lua_pushinteger(L, *(slot->GetVariant()));
	return 3;
}

void RegisterSlotMetatable(lua_State* L) {
	lua::LuaStackProtector protector(L);

	luaL_newmetatable(L, lua::metatables::EntitySlotMT); // meta

	lua_pushstring(L, "__index"); // meta __index
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod); // meta __index metamethod
	lua_rawset(L, -3); // meta

	lua_pushboolean(L, true); // meta true
	lua_rawsetp(L, -2, lua::luabridge::getIdentityKey()); // meta

	lua_pushstring(L, "__parent"); // meta __parent
	lua::PushMetatable(L, lua::Metatables::ENTITY); // meta __parent meta_entity
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__propget"); // meta __propget
	lua_newtable(L); // meta __proget table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__propset"); // meta __propset
	lua_newtable(L); // meta __propset table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__const"); // meta __const
	lua_newtable(L); // meta __const table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__type"); // meta __type
	lua_pushstring(L, "EntitySlot"); // meta __type EntitySlot
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__class"); // meta __class 
	lua_pushstring(L, "EntitySlot"); // meta __class EntitySlot
	lua_rawset(L, -3); // meta

	luaL_Reg funcs[] = {
		{ "REPENTOGO", lua_EntitySlot_REPENTOGOING },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1); // clean stack
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterSlotMetatable(g_LuaEngine->_state);
}