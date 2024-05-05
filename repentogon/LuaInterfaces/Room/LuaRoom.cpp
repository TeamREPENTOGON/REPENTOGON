#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Room.h"

RoomASM roomASM;
extern uint32_t hookedbackdroptype;

LUA_FUNCTION(Lua_SpawnGridEntity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	bool ret = false;
	int GridIndex = (int)luaL_checkinteger(L, 2);
	if (GridIndex < 0 || GridIndex > 447) {
		return luaL_error(L, "Grid index %d invalid", GridIndex);
	}
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		GridEntityDesc* desc = lua::GetUserdata<GridEntityDesc*>(L, 3, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
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
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	int PathTrail = (int)luaL_checkinteger(L, 3);
	bool KeepDecoration = lua_toboolean(L, 4);
	room->RemoveGridEntityImmediate(GridIndex, PathTrail, KeepDecoration);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetShopItemPrice) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	unsigned int entVariant = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int entSubtype = (unsigned int)luaL_checkinteger(L, 3);
	int shopItemID = (int)luaL_checkinteger(L, 4);

	lua_pushinteger(L, room->GetShopItemPrice(entVariant, entSubtype, shopItemID));
	return 1;
}

LUA_FUNCTION(Lua_RoomSetBackdrop) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
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
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	bool Force = lua_toboolean(L, 3);
	lua_pushboolean(L, room->CanSpawnObstacleAtPosition(GridIndex, Force));
	return 1;
}

LUA_FUNCTION(Lua_RoomGetWaterAmount)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, *room->GetWaterAmount());

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterAmount)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	float amount = (float)luaL_checknumber(L, 2);
	*room->GetWaterAmount() = amount;

	return 0;
}

LUA_FUNCTION(Lua_RoomGetFloorColor)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *room->GetFloorColor();

	return 1;
}

LUA_FUNCTION(Lua_RoomGetWaterColor)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColor();

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterColor)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetUnknownWaterInt() = 1; // See Room.zhl for more info
	*room->GetWaterColor() = *color;

	return 0;
}

LUA_FUNCTION(Lua_RoomGetWaterColorMultiplier)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColorMultiplier();

	return 1;
}

LUA_FUNCTION(Lua_RoomSetWaterColorMultiplier)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetUnknownWaterInt() = 1;
	*room->GetWaterColorMultiplier() = *color;

	return 0;
}

LUA_FUNCTION(Lua_RoomSetWaterCurrent)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	*room->GetWaterCurrent() = *vector;

	return 0;
}

LUA_FUNCTION(Lua_RoomGetEffects)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);

	TemporaryEffects* effects = room->GetTemporaryEffects();
	lua::luabridge::UserdataPtr::push(L, effects, lua::GetMetatableKey(lua::Metatables::_TEMPORARY_EFFECTS));

	return 1;
}

LUA_FUNCTION(lua_RoomGetRail) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_Integer index = luaL_checkinteger(L, 2);

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
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_Integer index = luaL_checkinteger(L, 2);

	if (!room->IsValidGridIndex(index, false)) {
		return luaL_error(L, "Invalid grind index %lld\n", index);
	}

	lua_Integer rail = luaL_checkinteger(L, 3);
	if (!Room::IsValidRailType(rail)) {
		return luaL_error(L, "Invalid rail type %lld\n", index);
	}

	room->SetRailType((uint8_t)index, (RailType)rail);

	return 0;
}

LUA_FUNCTION(Lua_RoomCanPickupGridEntity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, room->CanPickupGridEntity(gridIndex));
	return 1;
}

LUA_FUNCTION(Lua_RoomPickupGridEntity)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	Entity_Effect* ent = room->PickupGridEntity(gridIndex);

	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

LUA_FUNCTION(Lua_RoomGetGridIndexByTile)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	int gridRow = (int)luaL_checkinteger(L, 2);
	int gridColumn = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, room->GetGridIndexByTile(gridRow, gridColumn));

	return 1;
}

LUA_FUNCTION(Lua_RoomSetPauseTimer)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->SetPauseTimer((int)luaL_checkinteger(L, 2));

	return 0;
}

LUA_FUNCTION(Lua_Room_GetBossChampionChance) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, room->GetChampionBossChance());
	return 1;
}

LUA_FUNCTION(Lua_Room_IsChampionBossSeed) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushboolean(L, room->IsChampionBossSeed());
	return 1;
}

LUA_FUNCTION(Lua_RoomColorModifierUpdate)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	bool process = lua::luaL_optboolean(L, 2, true);
	bool lerp = lua::luaL_optboolean(L, 3, true);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	ColorModState* pColor;
	if (process) {
		pColor = &room->ComputeColorModifier();
	}
	else {
		pColor = room->GetFXParams()->GetColorModifier();
	}

	g_Game->SetColorModifier(pColor, lerp, rate);
	return 0;
}

LUA_FUNCTION(Lua_RoomTryGetShopDiscount) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const int shopItemIdx = (int)luaL_checkinteger(L, 2);
	const int price = (int)luaL_checkinteger(L, 3);

	lua_pushinteger(L, room->TryGetShopDiscount(shopItemIdx, price));
	return 1;
}

LUA_FUNCTION(Lua_RoomGetRoomClearDelay) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_roomClearDelay);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetRoomClearDelay) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_roomClearDelay = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetGreedWaveTimer) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_greedWaveTimer);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetGreedWaveTimer) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_greedWaveTimer = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomIsPersistentRoomEntity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	const int type = (int)luaL_checkinteger(L, 2);
	const int varinat = (int)luaL_optinteger(L, 3, 0);
	lua_pushboolean(L, room->IsPersistentRoomEntity(type, varinat, 0));
	return 1;
}

LUA_FUNCTION(Lua_RoomTrySpawnSpecialQuestDoor) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	roomASM.ForceSpecialQuestDoor = lua::luaL_optboolean(L, 2, false);
	lua_pushboolean(L, room->TrySpawnSpecialQuestDoor());
	return 1;
}

LUA_FUNCTION(Lua_RoomSetLavaIntensity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_lavaIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetLightningIntensity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, g_Game->_lightningIntensity);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetLightningIntensity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	g_Game->_lightningIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomDoLightningStrike) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	unsigned int seed = (unsigned int)luaL_optinteger(L, 2, Isaac::genrand_int32());
	RNG rng; // oppa tyrone style
	rng.game_constructor(seed, 35);
	float intensity = 1.3f + rng.RandomFloat()*.6f;
	
	g_Game->_lightningIntensity = intensity;
	g_Manager->_sfxManager.Play(472, 1.0, 90, false, 0.9f + rng.RandomFloat() * 0.2f, 0);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetRainIntensity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushnumber(L, room->_rainIntensity);
	return 1;
}

LUA_FUNCTION(Lua_RoomSetRainIntensity) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->_rainIntensity = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomGetNumRainSpawners) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->_numRainSpawners);
	return 1;
}

LUA_FUNCTION(Lua_RoomGetBackdropTypeHui) { //this is a bad way to replace room.GetBackdropType, I think
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
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
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	room->SaveState();

	return 0;
}

LUA_FUNCTION(Lua_GetBossVictoryJingle) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
	lua_pushinteger(L, room->GetBossVictoryJingle());

	return 1;
}

HOOK_METHOD(Room, Init, (int param_1, RoomDescriptor * desc) -> void) {
	roomASM.WaterDisabled = false;
	super(param_1, desc);
	//printf("WaterDisabled is %s, stage is %d\n", roomASM.WaterDisabled ? "TRUE" : "FALSE", g_Game->_stage);
	if (g_Game->_stage == 12 && !roomASM.WaterDisabled && (this->_descriptor->Data->StageId == 27 || this->_descriptor->Data->StageId == 28)) {
		this->_waterAmount = 1.0f;
		//printf("setting water\n");
	}
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
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ROOM, functions);
}
