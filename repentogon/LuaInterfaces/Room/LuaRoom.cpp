#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Room.h"
#include "Log.h"

#include "../../Patches/ItemPoolManager.h"

RoomASM roomASM;
extern uint32_t hookedbackdroptype;

LUA_FUNCTION(Lua_SpawnGridEntity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	bool ret = false;
	int GridIndex = (int)luaL_checkinteger(L, 2);
	if (GridIndex < 0 || GridIndex > 447) {
		return luaL_error(L, "Grid index %d invalid", GridIndex);
	}
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		GridEntityDesc* desc = lua::GetLuabridgeUserdata<GridEntityDesc*>(L, 3, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
		ret = room->SpawnGridEntityDesc(GridIndex, desc);
	}
	else
	{
		int Type = (int)luaL_checkinteger(L, 3);
		int Variant = (int)luaL_optinteger(L, 4, 0);
		unsigned int Seed = (unsigned int)luaL_optinteger(L, 5, GridIndex + g_Game->_frameCount + 1);
		int VarData = (int)luaL_optinteger(L, 6, 0);
		ret = room->SpawnGridEntity(GridIndex, Type, Variant, Seed, VarData);
	}
	lua_pushboolean(L, ret);
	return 1;
}

LUA_FUNCTION(Lua_RemoveGridEntityImmediate) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	int PathTrail = (int)luaL_checkinteger(L, 3);
	bool KeepDecoration = lua_toboolean(L, 4);
	room->RemoveGridEntityImmediate(GridIndex, PathTrail, KeepDecoration);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetShopItemPrice) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	unsigned int entVariant = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int entSubtype = (unsigned int)luaL_checkinteger(L, 3);
	int shopItemID = (int)luaL_checkinteger(L, 4);

	lua_pushinteger(L, room->GetShopItemPrice(entVariant, entSubtype, shopItemID));
	return 1;
}

LUA_FUNCTION(Lua_RoomSetBackdrop) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_Integer id = luaL_checkinteger(L, 2);
	if (id < 0) {
		luaL_error(L, "Invalid backdrop id %d (min = 0, max = 61)", id);
	}
	lua_Integer changeDecoration = luaL_checkinteger(L, 3);
	Backdrop* backdrop = room->GetBackdrop();
	backdrop->Init((unsigned int)id, (bool)changeDecoration);
	return 0;
}

LUA_FUNCTION(Lua_RoomCanSpawnObstacleAtPosition) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	bool Force = lua_toboolean(L, 3);
	lua_pushboolean(L, room->CanSpawnObstacleAtPosition(GridIndex, Force));
	return 1;
}

LUA_FUNCTION(Lua_RoomGetWaterAmount)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, *room->GetWaterAmount());

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterAmount)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	float amount = (float)luaL_checknumber(L, 2);
	*room->GetWaterAmount() = amount;

	return 0;
}

LUA_FUNCTION(Lua_RoomGetFloorColor)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *room->GetFloorColor();

	return 1;
}

//[get/set] not actual color
LUA_FUNCTION(Lua_RoomGetWaterColor)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColor();

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterColor)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	KColor* color = lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetWaterLerpColorMultiplier() = 1; // See Room.zhl for more info
	room->_waterLerpTargetColor = *color;

	return 0;
}

//[get/set] not actual color mult
LUA_FUNCTION(Lua_RoomGetWaterColorMultiplier)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColorMultiplier();

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterColorMultiplier)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	KColor* color = lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetWaterLerpColorMultiplier() = 1;
	room->_waterLerpTargetColorMult = *color;

	return 0;
}

LUA_FUNCTION(Lua_RoomSetWaterCurrent)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	Vector* vector = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	*room->GetWaterCurrent() = *vector;

	return 0;
}

LUA_FUNCTION(Lua_RoomGetEffects)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	TemporaryEffects* effects = room->GetTemporaryEffects();
	lua::luabridge::UserdataPtr::push(L, effects, lua::GetMetatableKey(lua::Metatables::_TEMPORARY_EFFECTS));

	return 1;
}

LUA_FUNCTION(lua_RoomGetRail) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int index = (int)luaL_checkinteger(L, 2);

	if (!room->_descriptor->Data->IsAllowedGridIndex(index)) {
		// return luaL_error(L, "Invalid grid index %I\n", index);
		lua_pushnil(L);
	} else {
		uint8_t railType = room->GetRailType((uint8_t)index);
		if (railType == 255) {
			lua_pushnil(L);
		}
		else {
			lua_pushinteger(L, railType);
		}
	}

	return 1;
}

LUA_FUNCTION(lua_RoomSetRail) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int index = (int)luaL_checkinteger(L, 2);

	/* if (!room->IsValidGridIndex(index, false)) {
		return luaL_error(L, "Invalid grid index %d\n", index);
	} */
	if (index < 0 || (index >= (room->_gridHeight * room->_gridWidth))) {
		return luaL_error(L, "Invalid grid index %d\n", index);
	}

	int rail = (int)luaL_checkinteger(L, 3);
	if (!Room::IsValidRailType(rail)) {
		return luaL_error(L, "Invalid rail type %d\n", rail);
	}

	room->SetRailType(index, (RailType)rail);

	return 0;
}

LUA_FUNCTION(Lua_RoomCanPickupGridEntity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, room->CanPickupGridEntity(gridIndex));
	return 1;
}

LUA_FUNCTION(Lua_RoomPickupGridEntity)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	Entity_Effect* ent = room->PickupGridEntity(gridIndex);

	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

LUA_FUNCTION(Lua_RoomGetGridIndexByTile)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridRow, gridColumn;
	if (lua_type(L, 2) == LUA_TTABLE) {
		size_t length = (size_t)lua_rawlen(L, 2);
		if (length != 2)
			return luaL_argerror(L, 2, "expected table length of 2!");

		lua_rawgeti(L, 2, 1);
		gridRow = (int)luaL_checkinteger(L, -1);
		lua_pop(L, 1);
		lua_rawgeti(L, 2, 2);
		gridColumn = (int)luaL_checkinteger(L, -1);
		lua_pop(L, 1);
	}
	else
	{
		gridRow = (int)luaL_checkinteger(L, 2);
		gridColumn = (int)luaL_checkinteger(L, 3);
	}

	lua_pushinteger(L, room->GetGridIndexByTile(gridRow, gridColumn));

	return 1;
}

LUA_FUNCTION(Lua_RoomSetPauseTimer)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->SetPauseTimer((int)luaL_checkinteger(L, 2));

	return 0;
}

LUA_FUNCTION(Lua_Room_GetBossChampionChance) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, room->GetChampionBossChance());
	return 1;
}

LUA_FUNCTION(Lua_Room_IsChampionBossSeed) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushboolean(L, room->IsChampionBossSeed());
	return 1;
}

LUA_FUNCTION(Lua_RoomColorModifierUpdate)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	bool process = lua::luaL_optboolean(L, 2, true);
	bool lerp = lua::luaL_optboolean(L, 3, true);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	ColorModState pColor;
	if (process) {
		pColor = room->ComputeColorModifier();
		
	}
	else {
		// It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
		// This logic provides backwards compatability.
		FXParams* fx = room->GetFXParams();
		KColor* c = &fx->roomColor;
		pColor = ColorModState(c->_red, c->_green, c->_blue, fx->roomColor._alpha, fx->brightness, fx->contrast);
	}

	g_Game->SetColorModifier(&pColor, lerp, rate);
	return 0;
}

LUA_FUNCTION(Lua_RoomTryGetShopDiscount) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const int shopItemIdx = (int)luaL_checkinteger(L, 2);
	const int price = (int)luaL_checkinteger(L, 3);

	lua_pushinteger(L, room->TryGetShopDiscount(shopItemIdx, price));
	return 1;
}

LUA_FUNCTION(Lua_RoomGetRoomClearDelay) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_roomClearDelay);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetRoomClearDelay) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_roomClearDelay = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetGreedWaveTimer) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_greedWaveTimer);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetGreedWaveTimer) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_greedWaveTimer = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomIsPersistentRoomEntity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const int type = (int)luaL_checkinteger(L, 2);
	const int varinat = (int)luaL_optinteger(L, 3, 0);
	lua_pushboolean(L, room->IsPersistentRoomEntity(type, varinat, 0));
	return 1;
}

LUA_FUNCTION(Lua_RoomTrySpawnSpecialQuestDoor) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	roomASM.ForceSpecialQuestDoor = lua::luaL_optboolean(L, 2, false);
	lua_pushboolean(L, room->TrySpawnSpecialQuestDoor());
	return 1;
}

LUA_FUNCTION(Lua_RoomSetLavaIntensity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_lavaIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetLightningIntensity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, g_Game->_lightningIntensity);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetLightningIntensity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	g_Game->_lightningIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomDoLightningStrike) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	unsigned int seed = (unsigned int)luaL_optinteger(L, 2, Isaac::genrand_int32());
	RNG rng; // oppa tyrone style
	rng.game_constructor(seed, 35);
	float intensity = 1.3f + rng.RandomFloat()*.6f;
	
	g_Game->_lightningIntensity = intensity;
	g_Manager->_sfxManager.Play(472, 1.0, 90, false, 0.9f + rng.RandomFloat() * 0.2f, 0);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetRainIntensity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, room->_rainIntensity);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetRainIntensity) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_rainIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetNumRainSpawners) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_numRainSpawners);
	return 1;
}

LUA_FUNCTION(Lua_RoomGetBackdropTypeHui) { //this is a bad way to replace room.GetBackdropType, I think
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	if (hookedbackdroptype != 0) {
		lua_pushinteger(L, hookedbackdroptype);
		return 1;
	}
	else {
		Backdrop* bg = room->GetBackdrop();
		lua_pushinteger(L, bg->backdropId);
		return 1;
	}
}

LUA_FUNCTION(Lua_RoomSaveState) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->SaveState();

	return 0;
}

LUA_FUNCTION(Lua_GetBossVictoryJingle) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->GetBossVictoryJingle());

	return 1;
}

LUA_FUNCTION(Lua_RoomSetItemPool) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const int poolType = (int)luaL_checkinteger(L, 2);

	if (poolType < POOL_NULL || (uint32_t)poolType >= ItemPoolManager::GetNumItemPools()) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	roomASM.ItemPool = poolType;

	return 0;
}

inline int TrySpecialPool(Room* room) {
	const uint32_t roomType = room->_roomType;

	if (room->_bossId == BOSS_FALLEN) {
		return ROOM_DEVIL;
	}

	if (roomType == ROOM_BOSS) {
		if (*g_Game->GetLevelStateFlags() & (1 << 17)) {
			return ROOM_DEVIL;
		}
		return roomType;
	}

	if (roomType == ROOM_TREASURE) {
		if (g_Game->IsGreedMode() && (room->_descriptor->GridIndex != g_Game->_greedModeTreasureRoomIdx)) {
			return ROOM_BOSS;
		}
		if (room->_descriptor->Flags & (1 << 11)) {
			return ROOM_DEVIL;
		}
		return roomType;
	}

	if (roomType == ROOM_CHALLENGE && (room->_descriptor->Data->Subtype == 1)) {
		return ROOM_BOSS;
	}

	return roomType;
}

LUA_FUNCTION(Lua_RoomGetItemPool) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	uint32_t seed = (unsigned int)luaL_optinteger(L, 2, Isaac::genrand_int32());
	seed = seed != 0 ? seed : 1;
	bool raw = lua::luaL_optboolean(L, 3, false);

	const RoomConfig_Room* roomData = room->_descriptor->Data;
	if (g_Manager->_starting || roomData == nullptr)
	{
		lua_pushinteger(L, POOL_NULL);
		return 1;
	}

	if (roomASM.ItemPool != POOL_NULL || raw) {
		lua_pushinteger(L, roomASM.ItemPool);
		return 1;
	}

	if ((roomData->Type == ROOM_DEFAULT) && (roomData->StageId == STB_HOME) && roomData->Subtype == 2) {
		lua_pushinteger(L, POOL_MOMS_CHEST);
		return 1;
	}

	uint32_t roomType = TrySpecialPool(room);
	int poolType = g_Game->_itemPool.GetPoolForRoom(roomType, seed);
	poolType = poolType != POOL_NULL ? poolType : POOL_TREASURE;

	lua_pushinteger(L, poolType);
	return 1;
}

HOOK_METHOD(Room, Init, (int param_1, RoomDescriptor * desc) -> void) {
	roomASM.WaterDisabled = false;
	roomASM.ItemPool = POOL_NULL;
	super(param_1, desc);
	ZHL::Logger logger;
	//logger.Log("WaterDisabled is %s, stage is %d, desc stage is %d\n", roomASM.WaterDisabled ? "TRUE" : "FALSE", g_Game->_stage, this->_descriptor->Data->StageId);
	if (g_Game->_stage == 12 && !roomASM.WaterDisabled && (this->_descriptor->Data->StageId == 27 || this->_descriptor->Data->StageId == 28)) {
		//__debugbreak();
		this->_waterAmount = 1.0f;
		//logger.Log("setting water\n");
	}
}

HOOK_STATIC(ItemPool, GetSeededCollectible, (uint32_t seed, bool noDecrease, RoomDescriptor* roomDesc) -> int) {
	if (roomASM.ItemPool != POOL_NULL) {
		return g_Game->_itemPool.GetCollectible(roomASM.ItemPool, seed, noDecrease, COLLECTIBLE_NULL);
	}
	return super(seed, noDecrease, roomDesc);
}

LUA_FUNCTION(Lua_RoomGetWallColor) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua::luabridge::UserdataPtr::push(L, &room->_wallColor, lua::Metatables::COLOR);
	return 1;
}

LUA_FUNCTION(Lua_RoomTriggerOutput) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const unsigned int output = (unsigned int)luaL_checkinteger(L, 2);

	if (output < 0 || output > 9) {
		return luaL_argerror(L, 2, "Invalid output index");
	}

	room->TriggerOutput(output);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetShopItemPrice", Lua_RoomGetShopItemPrice},
		{ "SpawnGridEntity", Lua_SpawnGridEntity},
		{ "RemoveGridEntityImmediate", Lua_RemoveGridEntityImmediate},
		{ "CanSpawnObstacleAtPosition", Lua_RoomCanSpawnObstacleAtPosition},
		{ "GetWaterAmount", Lua_RoomGetWaterAmount},
		{ "SetWaterAmount", Lua_RoomSetWaterAmount},
		{ "GetFloorColor", Lua_RoomGetFloorColor},
		{ "GetWallColor", Lua_RoomGetWallColor},
		{ "GetWaterColor", Lua_RoomGetWaterColor},
		{ "SetWaterColor", Lua_RoomSetWaterColor},
		{ "SetWaterCurrent", Lua_RoomSetWaterCurrent},
		{ "GetWaterColorMultiplier", Lua_RoomGetWaterColorMultiplier},
		{ "SetWaterColorMultiplier", Lua_RoomSetWaterColorMultiplier},
		{ "SetBackdropType", Lua_RoomSetBackdrop},
		{ "GetEffects", Lua_RoomGetEffects},
		{ "GetRail", lua_RoomGetRail},
		{ "SetRail", lua_RoomSetRail},
		{ "CanPickupGridEntity", Lua_RoomCanPickupGridEntity},
		{ "PickupGridEntity", Lua_RoomPickupGridEntity},
		{ "GetGridIndexByTile", Lua_RoomGetGridIndexByTile},
		{ "SetPauseTimer", Lua_RoomSetPauseTimer},
		{ "GetChampionBossChance", Lua_Room_GetBossChampionChance},
		{ "IsChampionBossSeed", Lua_Room_IsChampionBossSeed},
		{ "UpdateColorModifier", Lua_RoomColorModifierUpdate},
		{ "TryGetShopDiscount", Lua_RoomTryGetShopDiscount},
		{ "GetRoomClearDelay", Lua_RoomGetRoomClearDelay},
		{ "SetRoomClearDelay", Lua_RoomSetRoomClearDelay},
		{ "GetGreedWaveTimer", Lua_RoomGetGreedWaveTimer},
		{ "SetGreedWaveTimer", Lua_RoomSetGreedWaveTimer},
		{ "IsPersistentRoomEntity", Lua_RoomIsPersistentRoomEntity},
		{ "TrySpawnSpecialQuestDoor", Lua_RoomTrySpawnSpecialQuestDoor},
		{ "SetLavaIntensity", Lua_RoomSetLavaIntensity},
		{ "GetLightningIntensity", Lua_RoomGetLightningIntensity},
		{ "SetLightningIntensity", Lua_RoomSetLightningIntensity},
		{ "DoLightningStrike", Lua_RoomDoLightningStrike},
		{ "GetRainIntensity", Lua_RoomGetRainIntensity},
		{ "SetRainIntensity", Lua_RoomSetRainIntensity},
		{ "GetNumRainSpawners", Lua_RoomGetNumRainSpawners},
		{ "GetBackdropType", Lua_RoomGetBackdropTypeHui},
		{ "SaveState", Lua_RoomSaveState},
		{ "GetBossVictoryJingle", Lua_GetBossVictoryJingle},
		{ "SetItemPool", Lua_RoomSetItemPool },
		{ "GetItemPool", Lua_RoomGetItemPool },
		{ "TriggerOutput", Lua_RoomTriggerOutput },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ROOM, functions);
}
