#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "LuaEntitySaveState.h"

struct Lua_EntitySaveStateAPI : Lua_EntitySaveState {
	static luaL_Reg methods[];

	static Lua_EntitySaveStateAPI* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<Lua_EntitySaveStateAPI*>(L, idx, lua::metatables::EntitySaveStateMT);
	}

	LUA_FUNCTION(Lua_GetType) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.type);

		return 1;
	}

	LUA_FUNCTION(Lua_SetType) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.type = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetVariant) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.variant);

		return 1;
	}

	LUA_FUNCTION(Lua_SetVariant) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.variant = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetSubtype) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.subtype);

		return 1;
	}

	LUA_FUNCTION(Lua_SetSubtype) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.subtype = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		if (st.type == 4 || st.type == 33 || st.type == 292) {
			lua_pushnumber(L, *(float*)(&st.intStorage1));
		}
		else {
			lua_pushinteger(L, st.intStorage1);
		}
		
		return 1;
	}

	LUA_FUNCTION(Lua_SetI1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);

		if (st.type == 4 || st.type == 33 || st.type == 292) {
			//lua_pushnumber(L, *(float*)(&st.intStorage1));
			*(float*)st.intStorage1 = (float)luaL_checknumber(L, 2);
		}
		else {
			st.intStorage1 = (unsigned int)luaL_checkinteger(L, 2);
		}
		

		return 0;
	}

	LUA_FUNCTION(Lua_GetI2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		/*if (st.type == 4) {
			lua::luabridge::UserdataValue<BitSet128>::push(L, lua::GetMetatableKey(lua::Metatables::BITSET_128), st.intStorage2);
		}
		else {
			lua_pushinteger(L, st.intStorage2);
		}
		*/

		lua_pushinteger(L, st.intStorage2);
		return 1;
	}

	LUA_FUNCTION(Lua_SetI2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage2 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI3) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage3);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI3) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage3 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI4) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage4);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI4) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage4 = (int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI5) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage5);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI5) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage5 = (int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetGridSpawnIdx) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.gridSpawnIdx);

		return 1;
	}

	LUA_FUNCTION(Lua_SetGridSpawnIdx) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.gridSpawnIdx = (short)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetB1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushboolean(L, st.boolStorage1);

		return 1;
	}

	LUA_FUNCTION(Lua_SetB1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.boolStorage1 = lua::luaL_checkboolean(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetB2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushboolean(L, st.boolStorage2);

		return 1;
	}

	LUA_FUNCTION(Lua_SetB2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.boolStorage2 = lua::luaL_checkboolean(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI6) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage6);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI6) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage6 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetPos) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
		*toLua = st.targetPosition;

		return 1;
	}

	LUA_FUNCTION(Lua_SetPos) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.targetPosition = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

		return 0;
	}

	LUA_FUNCTION(Lua_GetI7) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._intStorage7);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI7) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st._intStorage7 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetInitSeed) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._initSeed);

		return 1;
	}

	LUA_FUNCTION(Lua_GetDropSeed) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._dropSeed);

		return 1;
	}

	LUA_FUNCTION(Lua_GetSpawnerType) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.spawnerType);

		return 1;
	}

	LUA_FUNCTION(Lua_GetSpawnerVariant) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.spawnerVariant);

		return 1;
	}

	LUA_FUNCTION(Lua_GetF1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushnumber(L, st.floatStorage1);

		return 1;
	}

	LUA_FUNCTION(Lua_SetF1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.floatStorage1 = (float)luaL_checknumber(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetF2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushnumber(L, st.floatStorage2);

		return 1;
	}

	LUA_FUNCTION(Lua_SetF2) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.floatStorage2 = (float)luaL_checknumber(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI8) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage8);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI8) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage8 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetU1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);

		if (st.type == 5) {
			lua_pushboolean(L, st.byteStorage);
		}
		else {
			lua_pushinteger(L, st.byteStorage);
		}

		return 1;
	}

	LUA_FUNCTION(Lua_SetU1) {
		Lua_EntitySaveStateAPI* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		if (st.type == 5) {
			st.byteStorage = lua::luaL_checkboolean(L, 2);
		}
		else {
			st.byteStorage = (int8_t)luaL_checkinteger(L, 2);
		}

		return 0;
	}
};

luaL_Reg Lua_EntitySaveStateAPI::methods[] = {
	{ "GetType", Lua_GetType },
	{ "SetType", Lua_SetType },
	{ "GetVariant", Lua_GetVariant },
	{ "SetVariant", Lua_SetVariant },
	{ "GetSubType", Lua_GetSubtype },
	{ "SetSubType", Lua_SetSubtype },
	{ "GetI1", Lua_GetI1 },
	{ "SetI1", Lua_SetI1 },
	{ "GetI2", Lua_GetI2 },
	{ "SetI2", Lua_SetI2 },
	{ "GetI3", Lua_GetI3 },
	{ "SetI3", Lua_SetI3 },
	{ "GetI4", Lua_GetI4 },
	{ "SetI4", Lua_SetI4 },
	{ "GetI5", Lua_GetI5 },
	{ "SetI5", Lua_SetI5 },
	{ "GetGridSpawnIdx", Lua_GetGridSpawnIdx },
	//{ "SetGridSpawnIdx", Lua_SetGridSpawnIdx },
	{ "GetB1", Lua_GetB1 },
	{ "SetB1", Lua_SetB1 },
	{ "GetB2", Lua_GetB2 },
	{ "SetB2", Lua_SetB2 },
	{ "GetI6", Lua_GetI6 },
	{ "SetI6", Lua_SetI6 },
	{ "GetPos", Lua_GetPos },
	{ "SetPos", Lua_SetPos },
	{ "GetI7", Lua_GetI7 },
	{ "SetI7", Lua_SetI7 },
	{ "GetInitSeed", Lua_GetInitSeed },
	{ "GetDropSeed", Lua_GetDropSeed },
	{ "GetSpawnerType", Lua_GetSpawnerType },
	{ "GetSpawnerVariant", Lua_GetSpawnerVariant },
	{ "GetF1", Lua_GetF1 },
	{ "SetF1", Lua_SetF1 },
	{ "GetF2", Lua_GetF2 },
	{ "SetF2", Lua_SetF2 },
	{ "GetI8", Lua_GetI8 },
	{ "SetI8", Lua_SetI8 },
	{ "GetU1", Lua_GetU1 },
	{ "SetU1", Lua_SetU1 },

	{ NULL, NULL }
};

struct Lua_EntitiesSaveStateVectorAPI : Lua_EntitiesSaveStateVector {
	static Lua_EntitiesSaveStateVectorAPI* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<Lua_EntitiesSaveStateVectorAPI*>(L, idx, lua::metatables::EntitiesSaveStateVectorMT);
	}

	LUA_FUNCTION(Lua_Get) {
		Lua_EntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		size_t index = (size_t)luaL_checkinteger(L, 2);

		if (index < 0 || index >= ud->data->size()) {
			return luaL_error(L, "Invalid index for Get(): %d\n", index);
		}

		Lua_EntitySaveStateAPI* result = lua::place<Lua_EntitySaveStateAPI>(L, lua::metatables::EntitySaveStateMT);
		result->vec = ud->data;
		result->index = index;
		return 1;
	}

	LUA_FUNCTION(Lua_GetByType) {
		Lua_EntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		int type = (int)luaL_checkinteger(L, 2);
		int variant = (int)luaL_optinteger(L, 3, 0);
		int subtype = (int)luaL_optinteger(L, 4, 0);

		lua_newtable(L);

		int j = 1;
		for (size_t i = 0; i < ud->data->size(); ++i) {
			EntitySaveState const& st = (*ud->data)[i];
			if (st.type == type && st.variant == variant && st.subtype == subtype) {
				lua_pushinteger(L, j);
				Lua_EntitySaveStateAPI* result = lua::place<Lua_EntitySaveStateAPI>(L, lua::metatables::EntitySaveStateMT);
				result->vec = ud->data;
				result->index = j;
				lua_rawset(L, -3);

				++j;
			}
		}

		return 1;
	}

	LUA_FUNCTION(Lua_Clear) {
		Lua_EntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		//for (size_t i = 0; i < ud->data->capacity(); ++i) {
		//	EntitySaveState& st = (*ud->data)[i];			//not 100% sure whether all that's needed, if issues arise, uncomment this block
		//	st.Clear();
		//};
		ud->data->clear();

		return 0;
	}

	LUA_FUNCTION(Lua_MetaLen) {
		Lua_EntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		lua_pushinteger(L, ud->data->size());
		return 1;
	}

	static luaL_Reg methods[];
};

luaL_Reg Lua_EntitiesSaveStateVectorAPI::methods[] = {
	{ "Get", Lua_EntitiesSaveStateVectorAPI::Lua_Get },
	{ "GetByType", Lua_EntitiesSaveStateVectorAPI::Lua_GetByType },
	{ "Clear", Lua_EntitiesSaveStateVectorAPI::Lua_Clear },
	{ "__len", Lua_EntitiesSaveStateVectorAPI::Lua_MetaLen },
	{ NULL, NULL }
};

struct Lua_GridEntitiesSaveStateVectorAPI : Lua_GridEntitiesSaveStateVector {
	static Lua_GridEntitiesSaveStateVectorAPI* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<Lua_GridEntitiesSaveStateVectorAPI*>(L, idx, lua::metatables::GridEntitiesSaveStateVectorMT);
	}

	LUA_FUNCTION(Lua_Get) {
		Lua_GridEntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		size_t index = (size_t)luaL_checkinteger(L, 2);

		if (index < 0 || index >= ud->data->size()) {
			return luaL_error(L, "Invalid index for Get(): %d\n", index);
		}

		lua::luabridge::UserdataPtr::push(L, &ud->data->data()[index], lua::Metatables::GRID_ENTITY_DESC);
		return 1;
	}

	LUA_FUNCTION(Lua_GetByType) {
		Lua_GridEntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		int type = (int)luaL_checkinteger(L, 2);

		lua_newtable(L);

		int j = 1;
		for (size_t i = 0; i < ud->data->size(); ++i) {
			GridEntityDesc& st = (*ud->data)[i];
			if (st._type == type) {
				lua_pushinteger(L, j);
				lua::luabridge::UserdataPtr::push(L, &st, lua::Metatables::GRID_ENTITY_DESC);
				lua_rawset(L, -3);

				++j;
			}
		}

		return 1;
	}

	LUA_FUNCTION(Lua_Clear) {
		Lua_GridEntitiesSaveStateVectorAPI* ud = GetData(L, 1);
//		ud->data->clear();
		for (size_t i = 0; i < ud->data->size(); ++i) {
			GridEntityDesc& st = (*ud->data)[i];
			st._type = 0;
		};

		return 0;
	}

	LUA_FUNCTION(Lua_MetaLen) {
		Lua_GridEntitiesSaveStateVectorAPI* ud = GetData(L, 1);
		lua_pushinteger(L, ud->data->size());
		return 1;
	}

	static luaL_Reg methods[];
};

luaL_Reg Lua_GridEntitiesSaveStateVectorAPI::methods[] = {

	{ "Get", Lua_GridEntitiesSaveStateVectorAPI::Lua_Get },
	{ "GetByType", Lua_GridEntitiesSaveStateVectorAPI::Lua_GetByType },
	{ "Clear", Lua_GridEntitiesSaveStateVectorAPI::Lua_Clear },
	{ "__len", Lua_GridEntitiesSaveStateVectorAPI::Lua_MetaLen },
	{ NULL, NULL }
};

static void RegisterEntitiesSaveStateMetatables(lua_State* L) {
	lua::RegisterNewClass(L, "EntitiesSaveStateVector", lua::metatables::EntitiesSaveStateVectorMT, Lua_EntitiesSaveStateVectorAPI::methods);
	lua::RegisterNewClass(L, "EntitySaveState", lua::metatables::EntitySaveStateMT, Lua_EntitySaveStateAPI::methods);
	lua::RegisterNewClass(L, "GridEntitiesSaveStateVector", lua::metatables::GridEntitiesSaveStateVectorMT, Lua_GridEntitiesSaveStateVectorAPI::methods);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterEntitiesSaveStateMetatables(state);
}