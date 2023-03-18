#include <lua.hpp>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int QueryRadiusRef = -1;

int Lua_IsaacFindByTypeFix(lua_State* L)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	int type = luaL_checkinteger(L, 1);
	int variant = luaL_optinteger(L, 2, -1);
	int subtype = luaL_optinteger(L, 3, -1);
	bool cache = false;
	if lua_isboolean(L, 4)
		cache = lua_toboolean(L, 4);
	bool ignoreFriendly = false;
	if lua_isboolean(L, 5)
		ignoreFriendly = lua_toboolean(L, 5);
	lua_newtable(L, 0);
	EntityList_EL res(*list->GetUpdateEL());

	list->QueryType(&res, type, variant, subtype, cache, ignoreFriendly);

	unsigned int size = res._size;

	if (size) {
		Entity** data = res._data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}

		res.Destroy();
	}

	return 1;
}


int Lua_IsaacGetRoomEntitiesFix(lua_State* L)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList_EL* res = room->GetEntityList()->GetUpdateEL();
	lua_newtable(L, 0);
	unsigned int size = res->_size;

	if (size) {
		Entity** data = res->_data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}
	}
	return 1;
}

static void DummyQueryRadius(EntityList_EL* el, void* pos, int partition) {
	el->_data = nullptr;
	el->_size = 0;
	el->_sublist = 1;
	el->_capacity = 0;
}

int Lua_IsaacFindInRadiusFix(lua_State* L)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float radius = luaL_checknumber(L, 2);
	unsigned int partition = luaL_optinteger(L, 3, -1);

	EntityList_EL res;
	EntityList_EL* resPtr = &res;
	lua_newtable(L, 0);

	lua_rawgeti(g_LuaEngine->_state, LUA_REGISTRYINDEX, QueryRadiusRef);
	const void* queryRadius = lua_topointer(g_LuaEngine->_state, -1);
	lua_pop(g_LuaEngine->_state, 1);

	__asm {
		push ecx;
		mov ecx, list;
		push partition;
		push pos;
		push resPtr;
		movss xmm3, radius;
		call queryRadius;
		pop ecx;
	}

	unsigned int size = res._size;

	if (size) {
		Entity** data = res._data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}

		res.Destroy();
	}

	return 1;
}

static void RegisterFindByTypeFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "FindByType");
	lua_pushcfunction(L, Lua_IsaacFindByTypeFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetRoomEntitiesFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetRoomEntities");
	lua_pushcfunction(L, Lua_IsaacGetRoomEntitiesFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterFindInRadiusFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "FindInRadius");
	lua_pushcfunction(L, Lua_IsaacFindInRadiusFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterFindByTypeFix(state);
	RegisterGetRoomEntitiesFix(state);
	RegisterFindInRadiusFix(state);

	SigScan scanner("558bec83e4f883ec14535657f3");
	bool result = scanner.Scan();
	if (!result) {
		lua_pushlightuserdata(state, &DummyQueryRadius);
	}
	else {
		lua_pushlightuserdata(state, scanner.GetAddress());
	}
	QueryRadiusRef = luaL_ref(state, LUA_REGISTRYINDEX);
}