#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

extern "C" {
	const char* L_RoomConfig_GetName(RoomConfig* self) {
		return self->Name.c_str();
	}

	RoomSpawn_List* L_RoomConfig_GetSpawns(RoomConfig* self) {
		return new RoomSpawn_List{ self->SpawnCount, self->Spawns };
	}

	// RoomConfig::Spawn
	RoomEntry* L_RoomConfigSpawn_PickEntry(RoomSpawn* self, float r) {
		return self->PickEntry(r);
	}

	RoomEntry_List* L_RoomConfigSpawn_GetEntries(RoomSpawn* self) {
		printf("%p\n", self);
		return new RoomEntry_List{ self->CountEntries, self->Entries };
	}
}

LUA_FUNCTION(Lua_RoomConfigGetMode)
{
	RoomConfig* roomConfig = lua::GetUserdata<RoomConfig*>(L, 1, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfigRoom");
	lua_pushinteger(L, roomConfig->Mode);

	return 1;
}

static void FixRoomConfigModeGet(lua_State* L) {
	lua::RegisterVariableGetter(L, lua::Metatables::ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigGetMode);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigGetMode);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	FixRoomConfigModeGet(_state);
}