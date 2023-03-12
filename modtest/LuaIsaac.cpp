#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_IsaacFindByTypeFix(lua_State* L)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	//(EntityType Type, int Variant = -1, int SubType = -1, boolean Cache = false, boolean IgnoreFriendly = false)

	int type = luaL_checkinteger(L, 1);
	int variant = -1;
	// try doing lua_tonumber to let -1 through
	if (lua_isinteger(L, 2))
		variant = lua_tointeger(L, 2);
	int subtype = -1;
	if (lua_isinteger(L, 3))
		subtype = lua_tointeger(L, 3);
	bool cache = lua_toboolean(L, 4);
	bool ignoreFriendly = lua_toboolean(L, 5);

	lua_newtable(L, 0);

	EntityList_EL* res = &EntityList_EL();

	list->QueryType(res, type, variant, subtype, cache, ignoreFriendly);

	unsigned int size = res->_size;

	if (size) {
		Entity** data = res->_data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, &ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}
	}

	/*for (int i = 0; i < size; ++i) {
		Entity* ent = *data[i];
		
		printf("%d %d %d", ent.GetType(), ent.GetVariant(), ent.GetSubType());
	}*/

	return 1;
}

static void RegisterFindByTypeFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "FindByType2");
	lua_pushcfunction(L, Lua_IsaacFindByTypeFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterFindByTypeFix(state);
}