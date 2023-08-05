#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RoomMT = "Room";

static int Lua_RemoveGridEntityImmediate(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	int PathTrail = (int)luaL_checkinteger(L, 3);
	bool KeepDecoration = lua_toboolean(L, 4);
	room->RemoveGridEntityImmediate(GridIndex, PathTrail, KeepDecoration);
	return 0;
}

static int Lua_RoomGetShopItemPrice(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	unsigned int entVariant = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int entSubtype = (unsigned int)luaL_checkinteger(L, 3);
	int shopItemID = (int)luaL_checkinteger(L, 4);

	lua_pushinteger(L, room->GetShopItemPrice(entVariant, entSubtype, shopItemID));
	return 1;
}

static int Lua_RoomSetBackdrop(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	lua_Integer id = luaL_checkinteger(L, 2);
	if (id < 0 ) {
		luaL_error(L, "Invalid backdrop id %d (min = 0, max = 61)", id);
	}
	lua_Integer changeDecoration = luaL_checkinteger(L, 3);
	Backdrop* backdrop = room->GetBackdrop();
	backdrop->Init((unsigned int)id, (bool)changeDecoration);
	return 0;
}

static int Lua_RoomCanSpawnObstacleAtPosition(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	int GridIndex = (int)luaL_checkinteger(L, 2);
	bool Force = lua_toboolean(L, 3);
	lua_pushboolean(L, room->CanSpawnObstacleAtPosition(GridIndex, Force));
	return 1;
}

int Lua_RoomGetWaterAmount(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	lua_pushnumber(L, *room->GetWaterAmount());

	return 1;
}

int Lua_RoomSetWaterAmount(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	float amount = (float)luaL_checknumber(L, 2);
	*room->GetWaterAmount() = amount;

	return 0;
}

int Lua_RoomGetFloorColor(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *room->GetFloorColor();

	return 1;
}

int Lua_RoomGetWaterColor(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColor();

	return 1;
}

int Lua_RoomSetWaterColor(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetUnknownWaterInt() = 1; // See Room.zhl for more info
	*room->GetWaterColor() = *color;

	return 0;
}

int Lua_RoomGetWaterColorMultiplier(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);

	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *room->GetWaterColorMultiplier();

	return 1;
}

int Lua_RoomSetWaterColorMultiplier(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	*room->GetUnknownWaterInt() = 1;
	*room->GetWaterColorMultiplier() = *color;

	return 0;
}

int Lua_RoomSetWaterCurrent(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	*room->GetWaterCurrent() = *vector;

	return 0;
}

int Lua_RoomGetEffects(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);

	TemporaryEffects* effects = room->GetTemporaryEffects();
	lua::luabridge::UserdataPtr::push(L, effects, lua::GetMetatableKey(lua::Metatables::_TEMPORARY_EFFECTS));

	return 1;
}

static int lua_RoomGetRail(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	lua_Integer index = luaL_checkinteger(L, 2);

	if (!room->IsValidGridIndex(index, false)) {
		return luaL_error(L, "Invalid grind index %lld\n", index);
	}

	lua_pushinteger(L, room->GetRailType(index));

	return 1;
}

static int lua_RoomSetRail(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	lua_Integer index = luaL_checkinteger(L, 2);

	if (!room->IsValidGridIndex(index, false)) {
		return luaL_error(L, "Invalid grind index %lld\n", index);
	}

	lua_Integer rail = luaL_checkinteger(L, 3);
	if (!Room::IsValidRailType(rail)) {
		return luaL_error(L, "Invalid rail type %lld\n", index);
	}

	room->SetRailType(index, (RailType)rail);

	return 0;
}

static int Lua_RoomCanPickupGridEntity(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, room->CanPickupGridEntity(gridIndex));
	return 1;
}

int Lua_RoomPickupGridEntity(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	Entity_Effect* ent = room->PickupGridEntity(gridIndex);

	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

int Lua_RoomGetGridIndexByTile(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	int gridRow  = (int)luaL_checkinteger(L, 2);
	int gridColumn = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, room->GetGridIndexByTile(gridRow, gridColumn));

	return 1;
}

int Lua_RoomSetPauseTimer(lua_State* L)
{
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, RoomMT);
	room->SetPauseTimer(luaL_checkinteger(L, 2));

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ROOM;

	lua::RegisterFunction(state, mt, "GetShopItemPrice", Lua_RoomGetShopItemPrice);
	lua::RegisterFunction(state, mt, "RemoveGridEntityImmediate", Lua_RemoveGridEntityImmediate);
	lua::RegisterFunction(state, mt, "CanSpawnObstacleAtPosition", Lua_RoomCanSpawnObstacleAtPosition);
	lua::RegisterFunction(state, mt, "GetWaterAmount", Lua_RoomGetWaterAmount);
	lua::RegisterFunction(state, mt, "SetWaterAmount", Lua_RoomSetWaterAmount);
	lua::RegisterFunction(state, mt, "GetFloorColor", Lua_RoomGetFloorColor);
	lua::RegisterFunction(state, mt, "GetWaterColor", Lua_RoomGetWaterColor);
	lua::RegisterFunction(state, mt, "SetWaterColor", Lua_RoomSetWaterColor);
	lua::RegisterFunction(state, mt, "SetWaterCurrent", Lua_RoomSetWaterCurrent);
	lua::RegisterFunction(state, mt, "GetWaterColorMultiplier", Lua_RoomGetWaterColorMultiplier);
	lua::RegisterFunction(state, mt, "SetWaterColorMultiplier", Lua_RoomSetWaterColorMultiplier);
	lua::RegisterFunction(state, mt, "SetBackdropType", Lua_RoomSetBackdrop);
	lua::RegisterFunction(state, mt, "GetEffects", Lua_RoomGetEffects);
	lua::RegisterFunction(state, mt, "GetRail", lua_RoomGetRail);
	lua::RegisterFunction(state, mt, "SetRail", lua_RoomSetRail);
	lua::RegisterFunction(state, mt, "CanPickupGridEntity", Lua_RoomCanPickupGridEntity);
	lua::RegisterFunction(state, mt, "PickupGridEntity", Lua_RoomPickupGridEntity);
	lua::RegisterFunction(state, mt, "GetGridIndexByTile", Lua_RoomGetGridIndexByTile);
	lua::RegisterFunction(state, mt, "SetPauseTimer", Lua_RoomSetPauseTimer);
}