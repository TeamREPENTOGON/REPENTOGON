#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

LUA_FUNCTION(Lua_GridToGridTNT) {
	GridEntity* grid = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	if (grid->GetDesc()->_type == 23) { //TODO: enum
		GridEntity_TNT* newGrid = lua::UserdataToData<GridEntity_TNT*>(lua_touserdata(L, 1));

		luaL_setmetatable(L, lua::metatables::GridTNT_MT);
	}
	else
		lua_pushnil(L);
	return 1;
}

void RegisterTNTMetatable(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GRID_ENTITY, "ToTNT", Lua_GridToGridTNT);

	luaL_newmetatable(L, lua::metatables::GridTNT_MT); // meta

	lua_pushstring(L, "__index"); // meta __index
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod); // meta __index metamethod
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__newindex"); // meta __newindex
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod); // meta __newindex metamethod
	lua_rawset(L, -3); // meta

	lua_pushboolean(L, true); // meta true
	lua_rawsetp(L, -2, lua::luabridge::getIdentityKey()); // meta

	lua_pushstring(L, "__parent"); // meta __parent
	lua::PushMetatable(L, lua::Metatables::GRID_ENTITY); // meta __parent meta_entity
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
	lua_pushstring(L, "GridEntityTNT"); // meta __type GridEntityTNT
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__class"); // meta __class 
	lua_pushstring(L, "GridEntityTNT"); // meta __class GridEntityTNT
	lua_rawset(L, -3); // meta

	luaL_Reg funcs[] = {
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1); // clean stack
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterTNTMetatable(_state);
}