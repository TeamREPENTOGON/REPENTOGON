#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static RoomDescriptor* GetLeftRoom(RoomDescriptor* source) {
	return nullptr;
}
static void RoomDescriptorGetAllowedDoors(lua_State* L, RoomDescriptor* descriptor) {
	lua_pushinteger(L, descriptor->AllowedDoors);
}

LUA_FUNCTION(Lua_RoomDescriptorGetAllowedDoors) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

LUA_FUNCTION(Lua_RoomDescriptorGetAllowedDoorsConst) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

LUA_FUNCTION(Lua_RoomDescriptorSetAllowedDoors) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	int doors = (int)luaL_checkinteger(L, 2);
	descriptor->AllowedDoors = doors;
	return 0;
}

static void RoomDescriptorGetDoors(lua_State* L, RoomDescriptor* descriptor, bool allowSet) {
	RoomDescriptor** ptr = (RoomDescriptor**)lua_newuserdata(L, sizeof(RoomDescriptor*));
	*ptr = descriptor;
	if (allowSet) {
		luaL_setmetatable(L, lua::metatables::RoomDescriptorDoors);
	}
	else {
		luaL_setmetatable(L, lua::metatables::RoomDescriptorDoorsConst);
	}
}

LUA_FUNCTION(Lua_RoomDescriptorGetDoors) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, true);
	return 1;
}

LUA_FUNCTION(Lua_RoomDescriptorGetDoorsConst) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, false);
	return 1;
}

static void _RoomDescriptorDoorsGet(lua_State* L, RoomDescriptor* descriptor) {
	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		luaL_error(L, "Invalid door slot %d\n", slot);
		return;
	}

	lua_pushinteger(L, descriptor->Doors[slot]);
}

LUA_FUNCTION(Lua_RoomDescriptorDoorsGet) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, lua::metatables::RoomDescriptorDoors);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

LUA_FUNCTION(Lua_RoomDescriptorDoorsGetConst) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, lua::metatables::RoomDescriptorDoorsConst);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

LUA_FUNCTION(Lua_RoomDescriptorDoorsSet) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, lua::metatables::RoomDescriptorDoors);
	RoomDescriptor* descriptor = *ptr;

	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "Invalid door slot %d\n", slot);
	}

	int value = (int)luaL_checkinteger(L, 3);
	if (value < -1 || value > 255) {
		return luaL_error(L, "Invalid door mask %d\n", value);
	}

	descriptor->Doors[slot] = value;
	return 0;
}

static void FixRoomDescriptorProperties(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	int type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoors);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoors);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::CONST_ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoorsConst);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoorsConst);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propset");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propset is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorSetAllowedDoors);

	lua_rawset(L, -3);
	lua_pop(L, 2);

	luaL_newmetatable(L, lua::metatables::RoomDescriptorDoors);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGet);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsSet);
	lua_rawset(L, -5);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::RoomDescriptorDoorsConst);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGetConst);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

struct Lua_EntitySaveState {
	std::vector<EntitySaveState>* vec;
	int index;

	static luaL_Reg methods[];

	static Lua_EntitySaveState* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<Lua_EntitySaveState*>(L, idx, lua::metatables::EntitySaveStateMT);
	}

	LUA_FUNCTION(Lua_GetType) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.type);

		return 1;
	}

	LUA_FUNCTION(Lua_SetType) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.type = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetVariant) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.variant);

		return 1;
	}

	LUA_FUNCTION(Lua_SetVariant) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.variant = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetSubtype) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.subtype);

		return 1;
	}

	LUA_FUNCTION(Lua_SetSubtype) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.subtype = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
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
		Lua_EntitySaveState* ud = GetData(L, 1);
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
		Lua_EntitySaveState* ud = GetData(L, 1);
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
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage2 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI3) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage3);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI3) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage3 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI4) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage4);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI4) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage4 = (int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI5) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage5);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI5) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage5 = (int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetGridSpawnIdx) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.gridSpawnIdx);

		return 1;
	}

	LUA_FUNCTION(Lua_SetGridSpawnIdx) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.gridSpawnIdx = (short)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetB1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushboolean(L, st.boolStorage1);

		return 1;
	}

	LUA_FUNCTION(Lua_SetB1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.boolStorage1 = lua::luaL_checkboolean(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetB2) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushboolean(L, st.boolStorage2);

		return 1;
	}

	LUA_FUNCTION(Lua_SetB2) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.boolStorage2 = lua::luaL_checkboolean(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI6) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage6);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI6) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage6 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetPos) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
		*toLua = st.targetPosition;

		return 1;
	}

	LUA_FUNCTION(Lua_SetPos) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.targetPosition = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

		return 0;
	}

	LUA_FUNCTION(Lua_GetI7) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._intStorage7);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI7) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st._intStorage7 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetInitSeed) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._initSeed);

		return 1;
	}

	LUA_FUNCTION(Lua_GetDropSeed) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st._dropSeed);

		return 1;
	}

	LUA_FUNCTION(Lua_GetSpawnerType) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.spawnerType);

		return 1;
	}

	LUA_FUNCTION(Lua_GetSpawnerVariant) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.spawnerVariant);

		return 1;
	}

	LUA_FUNCTION(Lua_GetF1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushnumber(L, st.floatStorage1);

		return 1;
	}

	LUA_FUNCTION(Lua_SetF1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.floatStorage1 = (float)luaL_checknumber(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetF2) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushnumber(L, st.floatStorage2);

		return 1;
	}

	LUA_FUNCTION(Lua_SetF2) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.floatStorage2 = (float)luaL_checknumber(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetI8) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		lua_pushinteger(L, st.intStorage8);

		return 1;
	}

	LUA_FUNCTION(Lua_SetI8) {
		Lua_EntitySaveState* ud = GetData(L, 1);
		EntitySaveState& st = (ud->vec->data()[ud->index]);
		st.intStorage8 = (unsigned int)luaL_checkinteger(L, 2);

		return 0;
	}

	LUA_FUNCTION(Lua_GetU1) {
		Lua_EntitySaveState* ud = GetData(L, 1);
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
		Lua_EntitySaveState* ud = GetData(L, 1);
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

luaL_Reg Lua_EntitySaveState::methods[] = {
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

struct RoomDescriptor_SavedEntities {
	std::vector<EntitySaveState>* data;

	static RoomDescriptor_SavedEntities* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<RoomDescriptor_SavedEntities*>(L, idx, lua::metatables::EntitiesSaveStateVectorMT);
	}

	LUA_FUNCTION(Lua_Get) {
		RoomDescriptor_SavedEntities* ud = GetData(L, 1);
		size_t index = (size_t)luaL_checkinteger(L, 2);

		if (index < 0 || index >= ud->data->size()) {
			return luaL_error(L, "Invalid index for Get(): %d\n", index);
		}

		Lua_EntitySaveState* result = lua::place<Lua_EntitySaveState>(L, lua::metatables::EntitySaveStateMT);
		result->vec = ud->data;
		result->index = index;
		return 1;
	}

	LUA_FUNCTION(Lua_GetByType) {
		RoomDescriptor_SavedEntities* ud = GetData(L, 1);
		int type = (int)luaL_checkinteger(L, 2);
		int variant = (int)luaL_optinteger(L, 3, 0);
		int subtype = (int)luaL_optinteger(L, 4, 0);

		lua_newtable(L);

		int j = 1;
		for (size_t i = 0; i < ud->data->size(); ++i) {
			EntitySaveState const& st = (*ud->data)[i];
			if (st.type == type && st.variant == variant && st.subtype == subtype) {
				lua_pushinteger(L, j);
				Lua_EntitySaveState* result = lua::place<Lua_EntitySaveState>(L, lua::metatables::EntitySaveStateMT);
				result->vec = ud->data;
				result->index = j;
				lua_rawset(L, -3);

				++j;
			}
		}

		return 1;
	}

	LUA_FUNCTION(Lua_Clear) {
		RoomDescriptor_SavedEntities* ud = GetData(L, 1);
		ud->data->clear();

		return 0;
	}

	LUA_FUNCTION(Lua_MetaLen) {
		RoomDescriptor_SavedEntities* ud = GetData(L, 1);
		lua_pushinteger(L, ud->data->size());
		return 1;
	}

	static luaL_Reg methods[];
};

luaL_Reg RoomDescriptor_SavedEntities::methods[] = {
	{ "Get", RoomDescriptor_SavedEntities::Lua_Get },
	{ "GetByType", RoomDescriptor_SavedEntities::Lua_GetByType },
	{ "Clear", RoomDescriptor_SavedEntities::Lua_Clear },
	{ "__len", RoomDescriptor_SavedEntities::Lua_MetaLen },
	{ NULL, NULL }
};

LUA_FUNCTION(Lua_GetEntitiesSaveState) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptor_SavedEntities* ud = lua::place<RoomDescriptor_SavedEntities>(L, lua::metatables::EntitiesSaveStateVectorMT);
	ud->data = &(descriptor->SavedEntities);
	return 1;
}

LUA_FUNCTION(Lua_GetRestrictedGridIndexes) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	std::set<int>& gridIndexes = descriptor->RestrictedGridIndexes;
	lua_newtable(L);
	int idx = 1;
	for (int gridIdx : gridIndexes) {
		lua_pushinteger(L, idx);
		lua_pushinteger(L, gridIdx);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_AddRestrictedGridIndex) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	const int gridIdx = (int)luaL_checkinteger(L, 2);
	std::set<int>& ents = descriptor->RestrictedGridIndexes;
	
	ents.insert(gridIdx);

	return 0;
}

struct RoomDescriptor_SavedGridEntities {
	std::vector<GridEntityDesc>* data;

	static RoomDescriptor_SavedGridEntities* GetData(lua_State* L, int idx) {
		return lua::GetUserdata<RoomDescriptor_SavedGridEntities*>(L, idx, lua::metatables::GridEntitiesSaveStateVectorMT);
	}

	LUA_FUNCTION(Lua_Get) {
		RoomDescriptor_SavedGridEntities* ud = GetData(L, 1);
		size_t index = (size_t)luaL_checkinteger(L, 2);

		if (index < 0 || index >= ud->data->size()) {
			return luaL_error(L, "Invalid index for Get(): %d\n", index);
		}

		lua::luabridge::UserdataPtr::push(L, &ud->data->data()[index], lua::Metatables::GRID_ENTITY_DESC);
		return 1;
	}

	LUA_FUNCTION(Lua_GetByType) {
		RoomDescriptor_SavedGridEntities* ud = GetData(L, 1);
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

	/*LUA_FUNCTION(Lua_Clear) {
		RoomDescriptor_SavedGridEntities* ud = GetData(L, 1);
		ud->data->clear();

		return 0;
	}
	*/

	LUA_FUNCTION(Lua_MetaLen) {
		RoomDescriptor_SavedGridEntities* ud = GetData(L, 1);
		lua_pushinteger(L, ud->data->size());
		return 1;
	}

	static luaL_Reg methods[];
};

luaL_Reg RoomDescriptor_SavedGridEntities::methods[] = {

	{ "Get", RoomDescriptor_SavedGridEntities::Lua_Get },
	{ "GetByType", RoomDescriptor_SavedGridEntities::Lua_GetByType },
	//{ "Clear", RoomDescriptor_SavedGridEntities::Lua_Clear },
	{ "__len", RoomDescriptor_SavedGridEntities::Lua_MetaLen },
	{ NULL, NULL }
};

LUA_FUNCTION(Lua_GetGridEntitiesSaveState) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptor_SavedGridEntities* ud = lua::place<RoomDescriptor_SavedGridEntities>(L, lua::metatables::GridEntitiesSaveStateVectorMT);
	ud->data = &(descriptor->SavedGridEntities);
	return 1;
}

static void RegisterRoomDescriptorMethods(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetEntitiesSaveState", Lua_GetEntitiesSaveState },
		{ "GetRestrictedGridIndexes", Lua_GetRestrictedGridIndexes },
		{ "AddRestrictedGridIndex", Lua_AddRestrictedGridIndex },
		{ "GetGridEntitiesSaveState", Lua_GetGridEntitiesSaveState },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::ROOM_DESCRIPTOR, functions);
}

static void RegisterEntitiesSaveStateMetatables(lua_State* L) {
	lua::RegisterNewClass(L, "EntitiesSaveStateVector", lua::metatables::EntitiesSaveStateVectorMT, RoomDescriptor_SavedEntities::methods);
	lua::RegisterNewClass(L, "EntitySaveState", lua::metatables::EntitySaveStateMT, Lua_EntitySaveState::methods);
	lua::RegisterNewClass(L, "GridEntitiesSaveStateVector", lua::metatables::GridEntitiesSaveStateVectorMT, RoomDescriptor_SavedGridEntities::methods);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	FixRoomDescriptorProperties(state);
	RegisterRoomDescriptorMethods(state);
	RegisterEntitiesSaveStateMetatables(state);
}