#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//AddCollectible Callback (id: 1004 enum pending)
HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	printf("item get");
	lua_State *L = g_LuaEngine->_state;
	
	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1004); //parameters (1004 is the callback id)
	lua_pushinteger(L, type); 
	lua_pushinteger(L, charge);
	lua_pushboolean(L, firsttime);
	lua_pushinteger(L, slot);
	lua_pushinteger(L, vardata);

	if ((!lua_pcall(L, 6, 1, 0)) && (lua_istable(L, -1))) { // 6 params 
		printf("Additem callback run \n");
		int tablesize = lua_rawlen(L, -1);
		if (tablesize == 5){ 
		int* result = new int[tablesize];
		printf("tablesize: %d\n",tablesize );
		for (int i = 1; i <= tablesize; i++) {
			lua_pushinteger(L, i); 
			lua_gettable(L, -2);
			result[i-1] = lua_tointeger(L, -1); //I only need ints here, otherwise I'd need to check the type
			printf("V: %d\n", result[i-1]);
			lua_pop(L, 1);
		}
		super(result[0], result[1], result[2], result[3], result[4]);
		return;
		}
	}
	super(type, charge, firsttime, slot, vardata);
}
//AddCollectible Callback (id: 1004 enum pending)

//1005 RESERVED - POST_ADD_COLLECTIBLE

//POST_TAKE_DMG callback (id: 1006 enum pending)
void ProcessPostDamageCallback(Entity ent, float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) {

	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushnumber(L, 1006);
	
	Entity* luaEnt = lua::luabridge::UserdataValue<Entity>::place(L, lua::GetMetatableKey(lua::Metatables::ENTITY));
	*luaEnt = ent;

	lua_pushnumber(L, damage);
	lua_pushnumber(L, damageFlags);

	EntityRef* ref = lua::luabridge::UserdataValue<EntityRef>::place(L, lua::GetMetatableKey(lua::Metatables::ENTITY_REF));
	*ref = *source;

	lua_pushnumber(L, damageCountdown);

	lua_pcall(L, 6, 1, 0);

};

HOOK_METHOD(Entity, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if (result) ProcessPostDamageCallback(*ent, damage, damageFlags, source, damageCountdown);
	return result;
}

HOOK_METHOD(Entity_Player, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if(result) ProcessPostDamageCallback(*ent, damage, damageFlags, source, damageCountdown);
	return result;
}
//POST_TAKE_DMG callback end