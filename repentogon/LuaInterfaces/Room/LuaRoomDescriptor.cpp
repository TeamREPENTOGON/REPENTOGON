#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "RoomPlacement.h"
#include "../LuaEntitySaveState.h"

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

LUA_FUNCTION(Lua_GetEntitiesSaveState) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	Lua_EntitiesSaveStateVector* ud = lua::place<Lua_EntitiesSaveStateVector>(L, lua::metatables::EntitiesSaveStateVectorMT);
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

LUA_FUNCTION(Lua_GetGridEntitiesSaveState) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	Lua_GridEntitiesSaveStateVector* ud = lua::place<Lua_GridEntitiesSaveStateVector>(L, lua::metatables::GridEntitiesSaveStateVectorMT);
	ud->data = &(descriptor->SavedGridEntities);
	return 1;
}

LUA_FUNCTION(Lua_GetBossDeathSeed) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	lua_pushinteger(L, descriptor->BossDeathSeed);
	return 1;
}

LUA_FUNCTION(Lua_InitSeeds) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	// rng userdata
	RNG* rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	descriptor->InitSeeds(rng);
	return 0;
}

LUA_FUNCTION(Lua_GetDimension) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	lua_pushinteger(L, descriptor->Dimension);
	return 1;
}

LUA_FUNCTION(Lua_GetNeighboringRooms) {
	RoomDescriptor* roomDesc = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");

	const std::map<int, RoomDescriptor*> neighbors = GetNeighboringRooms(roomDesc->GridIndex, roomDesc->Data->Shape, roomDesc->Dimension);

	lua_newtable(L);
	for (const auto& [doorSlot, neighborDesc] : neighbors) {
		lua::luabridge::UserdataPtr::push(L, neighborDesc, lua::GetMetatableKey(lua::Metatables::ROOM_DESCRIPTOR));
		lua_rawseti(L, -2, doorSlot);
	}

	return 1;
}

static void RegisterRoomDescriptorMethods(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetEntitiesSaveState", Lua_GetEntitiesSaveState },
		{ "GetRestrictedGridIndexes", Lua_GetRestrictedGridIndexes },
		{ "AddRestrictedGridIndex", Lua_AddRestrictedGridIndex },
		{ "GetGridEntitiesSaveState", Lua_GetGridEntitiesSaveState },
		{ "InitSeeds", Lua_InitSeeds },
		{ "GetDimension", Lua_GetDimension },
		{ "GetNeighboringRooms", Lua_GetNeighboringRooms },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::ROOM_DESCRIPTOR, functions);
	lua::RegisterVariableGetter(L, lua::Metatables::ROOM_DESCRIPTOR, "BossDeathSeed", Lua_GetBossDeathSeed);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	FixRoomDescriptorProperties(state);
	RegisterRoomDescriptorMethods(state);
}