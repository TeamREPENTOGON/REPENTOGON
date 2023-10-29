#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

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

	if (!room->IsValidGridIndex(index, false)) {
		return luaL_error(L, "Invalid grind index %lld\n", index);
	}

	lua_pushinteger(L, room->GetRailType((uint8_t)index));

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

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetShopItemPrice", Lua_RoomGetShopItemPrice},
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
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ROOM, functions);
}