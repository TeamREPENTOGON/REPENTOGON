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
};

luaL_Reg Lua_EntitySaveState::methods[] = {
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

static void RegisterRoomDescriptorMethods(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetEntitiesSaveState", Lua_GetEntitiesSaveState },
		{ "GetRestrictedGridIndexes", Lua_GetRestrictedGridIndexes },
		{ "AddRestrictedGridIndex", Lua_AddRestrictedGridIndex },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::ROOM_DESCRIPTOR, functions);
}

static void RegisterEntitiesSaveStateMetatables(lua_State* L) {
	lua::RegisterNewClass(L, "EntitiesSaveStateVector", lua::metatables::EntitiesSaveStateVectorMT, RoomDescriptor_SavedEntities::methods);
	lua::RegisterNewClass(L, "EntitySaveState", lua::metatables::EntitiesSaveStateVectorMT, Lua_EntitySaveState::methods);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	FixRoomDescriptorProperties(state);
	RegisterRoomDescriptorMethods(state);
	RegisterEntitiesSaveStateMetatables(state);
}