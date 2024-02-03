#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Room.h"

extern "C" {
	bool L_Room_CheckLine(Vector* pos1, Vector* pos2, int mode, int gridPathThreshold, bool ignoreWalls, bool ignoreCrushable, Vector* buffer) {
		return g_Game->_level._room->CheckLine(pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable, buffer);
	}

	bool L_Room_DamageGrid(int index, int damage) {
		return g_Game->_level._room->DamageGrid(index, damage);
	}

	bool L_Room_DestroyGrid(int index, bool immediate, bool dontExplodeBombRocks) {
		return g_Game->_level._room->DestroyGrid(index, immediate, dontExplodeBombRocks);
	}

	void L_Room_EmitBloodFromWalls(int duration, int count) {
		g_Game->_level._room->_wallBloodDuration = duration;
		g_Game->_level._room->_wallBloodCount = count;
	}

	Vector* L_Room_FindFreePickupSpawnPosition(Vector* pos, float initialStep, bool avoidActiveEntities, bool allowPits) {
		return new Vector(g_Game->_level._room->FindFreePickupSpawnPosition(pos, initialStep, avoidActiveEntities, allowPits));
	}

	Vector* L_Room_FindFreeTilePosition(Vector* pos, float distanceThreshold) {
		return new Vector(g_Game->_level._room->FindFreeTilePosition(pos, distanceThreshold));
	}

	int L_Room_GetAliveBossesCount() {
		return g_Game->_level._room->GetAliveBossesCount();
	}

	int L_Room_GetAliveEnemiesCount() {
		return g_Game->_level._room->GetAliveEnemiesCount();
	}

	int L_Room_GetAwardSeed() {
		if (g_Game->_level._room->_descriptor) return g_Game->_level._room->_descriptor->AwardSeed;
		return 0;
	}

	int L_Room_GetBackdropType() {
		return g_Game->_level._room->_backdrop.backdropId;
	}

	int L_Room_GetBossID() {
		return g_Game->_level._room->GetBossID();
	}

	Vector* L_Room_GetBottomRightPos() {
		return new Vector(g_Game->_level._room->GetBottomRightPos());
	}
	
	int L_Room_GetBrokenWatchState() {
		return g_Game->_level._room->GetBrokenWatchState();
	}

	Vector* L_Room_GetCenterPos() {
		return new Vector(g_Game->_level._room->GetCenterPos());
	}

	int L_Room_GetClampedGridIndex(Vector* pos) {
		return g_Game->_level._room->GetClampedGridIndex(pos);
	}

	Vector* L_Room_GetClampedPosition(Vector* pos, float margin) {
		return new Vector(g_Game->_level._room->GetClampedPosition(pos, margin));
	}

	int L_Room_GetDecorationSeed() {
		if (g_Game->_level._room->_descriptor) return g_Game->_level._room->_descriptor->DecorationSeed;
		return 0;
	}

	int L_Room_GetDeliriumDistance() {
		if (g_Game->_level._room->_descriptor) return g_Game->_level._room->_descriptor->DeliriumDistance;
		return 0;
	}

	float L_Room_GetDevilRoomChance() {
		return g_Game->_level._room->GetDevilRoomChance();
	}

	GridEntity_Door* L_Room_GetDoor(int doorSlot) {
		return g_Game->_level._room->GetDoor(doorSlot);
	}

	Vector* L_Room_GetDoorSlotPosition(int doorSlot) {
		return new Vector(g_Game->_level._room->GetDoorSlotPosition(doorSlot));
	}

	int L_Room_GetDungeonRockIdx() {
		return g_Game->_level._room->GetDungeonRockIdx();
	}

	float L_Room_GetEnemyDamageInflicted() {
		return g_Game->_level._room->_enemyDamageInflicted;
	}

	EntityList* L_Room_GetEntities() {
		return &g_Game->_level._room->_entityList;
	}

	int L_Room_GetFrameCount() {
		if (!g_Game->_level._room->_isInitialized) return -1;
		return g_Game->_level._room->_frameCount;
	}

	int L_Room_GetGridCollision(int gridIndex) {
		return g_Game->_level._room->GetGridCollision(gridIndex);
	}

	int L_Room_GetGridCollisionAtPos(Vector* pos) {
		return g_Game->_level._room->GetGridCollisionAtPos(pos);
	}

	GridEntity* L_Room_GetGridEntity(int index) {
		return g_Game->_level._room->GetGridEntity(index);
	}

	GridEntity* L_Room_GetGridEntityFromPos(Vector* pos) {
		return g_Game->_level._room->GetGridEntityFromPos(pos);
	}

	int L_Room_GetGridHeight() {
		return g_Game->_level._room->_gridWidth;
	}

	int L_Room_GetGridIndex(Vector* pos) {
		return g_Game->_level._room->GetGridIndex(pos);
	}

	int L_Room_GetGridPath(int index) {
		return g_Game->_level._room->GetGridPath(index);
	}

	int L_Room_GetGridPathFromPos(Vector* pos) {
		return g_Game->_level._room->GetGridPathFromPos(pos);
	}

	Vector* L_Room_GetGridPosition(int index) {
		return new Vector(g_Game->_level._room->GetGridPos(index));
	}

	int L_Room_GetGridSize() {
		return g_Game->_level._room->_gridWidth * g_Game->_level._room->_gridHeight;
	}

	int L_Room_GetGridWidth() {
		return g_Game->_level._room->_gridWidth;
	}

	Vector* L_Room_GetLaserTarget(Vector* pos, Vector* dir) {
		return new Vector(g_Game->_level._room->GetLaserTarget(pos, dir));
	}

	float L_Room_GetLavaIntensity() {
		return g_Game->_level._room->_lavaIntensity;
	}

	float L_Room_GetLightingAlpha() {
		return g_Game->_level._room->GetLightingAlpha();
	}

	// LRoomAreaDesc (what are those structs??)
	void* L_Room_GetLRoomAreaDesc() {
		auto desc = new Vector[4];
		g_Game->_level._room->GetLRoomAreaDesc(desc);
		return desc;
	}

	// LRoomTileDesc (what are those structs??)
	void* L_Room_GetLRoomTileDesc() {
		auto desc = new Vector[4];
		g_Game->_level._room->GetLRoomTileDesc(desc);
		return desc;
	}

	Vector* L_Room_GetRandomPosition(float margin) {
		return new Vector(g_Game->_level._room->GetRandomPosition(margin));
	}

	int L_Room_GetRandomTileIndex(int seed) {
		return g_Game->_level._room->GetRandomTileIndex(seed);
	}

	bool L_Room_GetRedHeartDamage() {
		return g_Game->_level._room->_redHeartDamage;
	}

	int L_Room_GetRenderMode() {
		return g_Game->_level._room->_renderMode;
	}

	Vector* L_Room_GetRenderScrollOffset() {
		return new Vector(g_Game->_level._room->_renderScrollOffset);
	}

	Vector* L_Room_GetRenderSurfaceTopLeft() {
		return new Vector(g_Game->_level._room->_renderSurfaceTopLeft);
	}

	int L_Room_GetRoomConfigStage() {
		return g_Game->_level._room->GetRoomConfigStage();
	}

	int L_Room_GetRoomShape() {
		return g_Game->_level._room->GetRoomShape();
	}
	
	int L_Room_GetSecondBossID() {
		return g_Game->_level._room->_secondBossId;
	}

	uint32_t L_Room_GetSeededCollectible(int seed, bool noDecrease) {
		return g_Game->_level._room->GetSeededCollectible(seed, noDecrease);
	}

	int L_Room_GetShopLevel() {
		return g_Game->_level._room->_shopLevel;
	}

	int L_Room_GetSpawnSeed() {
		return g_Game->_level._room->_descriptor->SpawnSeed;
	}

	int L_Room_GetTintedRockIdx() {
		return g_Game->_level._room->_tintedRockIndex;
	}

	Vector* L_Room_GetTopLeftPos() {
		return new Vector(g_Game->_level._room->GetTopLeftPos());
	}

	//// RoomType
	int L_Room_GetType() {
		return g_Game->_level._room->_roomType;
	}

	Vector* L_Room_GetWaterCurrent() {
		return g_Game->_level._room->GetWaterCurrent();
	}

	bool L_Room_HasCurseMist() {
		return g_Game->_level._room->HasCurseMist();
	}

	bool L_Room_HasLava() {
		return g_Game->_level._room->HasLava();
	}

	bool L_Room_HasSlowDown() {
		return g_Game->_level._room->_slowdownDuration > 0;
	}

	bool L_Room_HasTriggerPressurePlates() {
		return g_Game->_level._room->_hasTriggerPressurePlates;
	}

	bool L_Room_HasWater() {
		return g_Game->_level._room->_waterAmount > 0.0;
	}

	bool L_Room_HasWaterPits() {
		return g_Game->_level._room->HasWaterPits();
	}

	void L_Room_InvalidatePickupVision() {
		g_Game->_level._room->_invalidatePickupVision = true;
	}

	bool L_Room_IsAmbushActive() {
		return g_Game->_ambush.active;
	}

	bool L_Room_IsAmbushDone() {
		return g_Game->_level._room->IsAmbushDone();
	}

	bool L_Room_IsClear() {
		return g_Game->_level._room->IsClear();
	}

	bool L_Room_IsCurrentRoomLastBoss() {
		return g_Game->_level._room->IsCurrentRoomLastBoss();
	}

	bool L_Room_IsDoorSlotAllowed(int slot) {
		return g_Game->_level._room->IsDoorSlotAllowed(slot);
	}

	bool L_Room_IsFirstEnemyDead() {
		return g_Game->_level._room->_isFirstEnemyDead;
	}

	bool L_Room_IsFirstVisit() {
		return g_Game->_level._room->_isFirstVisit;
	}

	bool L_Room_IsInitialized() {
		return g_Game->_level._room->_isInitialized;
	}

	bool L_Room_IsLShapedRoom() {
		return g_Game->_level._room->IsLShapedRoom();
	}

	bool L_Room_IsMirrorWorld() {
		return g_Game->_level._room->IsMirrorWorld();
	}

	bool L_Room_IsPositionInRoom(Vector* pos, float margin) {
		return g_Game->_level._room->IsPositionInRoom(pos, margin);
	}

	bool L_Room_IsSacrificeDone() {
		return g_Game->_level._room->IsSacrificeDone();
	}

	void L_Room_KeepDoorsClosed() {
		g_Game->_level._room->_roomClearDelay = 10;
	}

	void L_Room_MamaMegaExplosion(Vector* pos) {
		g_Game->_level._room->MamaMegaExplosion(pos);
	}

	void L_Room_PlayMusic() {
		g_Game->_level._room->PlayMusic();
	}

	void L_Room_RemoveDoor(int slot) {
		g_Game->_level._room->RemoveDoor(slot);
	}

	void L_Room_RemoveGridEntity(int gridIndex, int pathTrail, bool keepDecoration) {
		g_Game->_level._room->RemoveGridEntity(gridIndex, pathTrail, keepDecoration);
	}

	void L_Room_Render() {
		g_Game->_level._room->Render();
	}

	void L_Room_RespawnEnemies() {
		g_Game->_level._room->RespawnEnemies();
	}

	Vector* L_Room_ScreenWrapPosition(Vector* pos, float margin) {
		return new Vector(g_Game->_level._room->ScreenWrapPosition(pos, margin));
	}

	void L_Room_SetAmbushDone(bool value) {
		g_Game->_level._room->SetAmbushDone(value);
	}

	void L_Room_SetBrokenWatchState(int state) {
		g_Game->_level._room->SetBrokenWatchState(state);
	}

	void L_Room_SetCardAgainstHumanity() {
		g_Game->_level._room->_cardAgainstHumanity = true;
	}

	void L_Room_SetClear(bool clear) {
		g_Game->_level._room->SetClear(clear);
	}

	void L_Room_SetFirstEnemyDead(bool value) {
		g_Game->_level._room->_isFirstEnemyDead = value;
	}

	void L_Room_SetFloorColor(ColorMod* floorColor) {
		if (g_Game->_level._room->GetFloorColor() != floorColor) {
			*g_Game->_level._room->GetFloorColor() = *floorColor;
		}
	}

	bool L_Room_SetGridPath(int index, int value) {
		return g_Game->_level._room->SetGridPath(index, value);
	}

	void L_Room_SetRedHeartDamage() {
		g_Game->_level._room->_redHeartDamage = true;
	}

	void L_Room_SetSacrificeDone(bool done) {
		g_Game->_level._room->SetSacrificeDone(done);
	}

	void L_Room_SetSlowDown(int duration) {
		g_Game->_level._room->SetSlowDown(duration);
	}

	void L_Room_SetWallColor(ColorMod* wallColor) {
		if (&g_Game->_level._room->_wallColor != wallColor) {
			g_Game->_level._room->_wallColor = *wallColor;
		}
	}

	void L_Room_ShopReshuffle(bool keepCollectibleIdx, bool reselectSaleItem) {
		g_Game->_level._room->ShopReshuffle(keepCollectibleIdx, reselectSaleItem);
	}

	void L_Room_ShopRestockFull() {
		g_Game->_level._room->ShopRestockFull();
	}

	void L_Room_ShopRestockPartial() {
		g_Game->_level._room->ShopRestockPartial();
	}

	void L_Room_SpawnClearAward() {
		g_Game->_level._room->SpawnClearAward();
	}

	bool L_Room_SpawnGridEntity(int gridIndex, uint32_t type, uint32_t variant, uint32_t seed, uint32_t varData) {
		return g_Game->_level._room->SpawnGridEntity(gridIndex, type, variant, seed, varData);
	}

	void L_Room_StopRain() {
		g_Game->_level._room->_stopRain = true;
	}

	void L_Room_TriggerClear(bool silent) {
		g_Game->_level._room->TriggerClear(silent);
	}

	bool L_Room_TryMakeBridge(GridEntity* pit, GridEntity* rock) {
		return g_Game->_level._room->TryMakeBridge(pit, rock);
	}

	bool L_Room_TrySpawnBlueWombDoor(bool firstTime, bool ignoreTime, bool force) {
		return g_Game->_level._room->TrySpawnBlueWombDoor(firstTime, ignoreTime, force);
	}

	bool L_Room_TrySpawnBossRushDoor(bool ignoreTime, bool force) {
		return g_Game->_level._room->TrySpawnBossRushDoor(ignoreTime, force);
	}

	bool L_Room_TrySpawnBrokenShovel() {
		return g_Game->_level._room->TrySpawnBrokenShovel();
	}

	bool L_Room_TrySpawnDevilRoomDoor(bool animate, bool force) {
		return g_Game->_level._room->TrySpawnDevilRoomDoor(animate, force);
	}

	bool L_Room_TrySpawnMegaSatanRoomDoor(bool force) {
		return g_Game->_level._room->TrySpawnMegaSatanDoor(force);
	}

	bool L_Room_TrySpawnSecretExit(bool animate, bool force) {
		return g_Game->_level._room->TrySpawnSecretExit(animate, force);
	}
	bool L_Room_TrySpawnSecretShop(bool force) {
		return g_Game->_level._room->TrySpawnSecretShop(force);
	}
	bool L_Room_TrySpawnSpecialQuestDoor() {
		return g_Game->_level._room->TrySpawnSpecialQuestDoor();
	}

	bool L_Room_TrySpawnTheVoidDoor(bool force) {
		return g_Game->_level._room->TrySpawnTheVoidDoor(force);
	}

	void L_Room_TurnGold() {
		g_Game->_level._room->TurnGold();
	}

	void L_Room_Update() {
		g_Game->_level._room->Update();
	}

	Vector* L_Room_WorldToScreenPosition(Vector* worldPos) {
		return new Vector(g_Game->_level._room->WorldToScreenPosition(worldPos));
	}
}

RoomASM roomASM;

//LUA_FUNCTION(Lua_SpawnGridEntity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	bool ret = false;
//	int GridIndex = (int)luaL_checkinteger(L, 2);
//	if (GridIndex < 0 || GridIndex > 447) {
//		return luaL_error(L, "Grid index %d invalid", GridIndex);
//	}
//	if (lua_type(L, 3) == LUA_TUSERDATA) {
//		GridEntityDesc* desc = lua::GetUserdata<GridEntityDesc*>(L, 3, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
//		ret = room->SpawnGridEntityDesc(GridIndex, desc);
//	}
//	else
//	{
//		int Type = (int)luaL_checkinteger(L, 3);
//		int Variant = (int)luaL_optinteger(L, 4, 0);
//		unsigned int Seed = (unsigned int)luaL_optinteger(L, 5, GridIndex + g_Game->_frameCount + 1);
//		int VarData = (int)luaL_optinteger(L, 6, 0);
//		ret = room->SpawnGridEntity(GridIndex, Type, Variant, Seed, VarData);
//	}
//	lua_pushboolean(L, ret);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RemoveGridEntityImmediate) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	int GridIndex = (int)luaL_checkinteger(L, 2);
//	int PathTrail = (int)luaL_checkinteger(L, 3);
//	bool KeepDecoration = lua_toboolean(L, 4);
//	room->RemoveGridEntityImmediate(GridIndex, PathTrail, KeepDecoration);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetShopItemPrice) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	unsigned int entVariant = (unsigned int)luaL_checkinteger(L, 2);
//	unsigned int entSubtype = (unsigned int)luaL_checkinteger(L, 3);
//	int shopItemID = (int)luaL_checkinteger(L, 4);
//
//	lua_pushinteger(L, room->GetShopItemPrice(entVariant, entSubtype, shopItemID));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetBackdrop) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_Integer id = luaL_checkinteger(L, 2);
//	if (id < 0) {
//		luaL_error(L, "Invalid backdrop id %d (min = 0, max = 61)", id);
//	}
//	lua_Integer changeDecoration = luaL_checkinteger(L, 3);
//	Backdrop* backdrop = room->GetBackdrop();
//	backdrop->Init((unsigned int)id, (bool)changeDecoration);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomCanSpawnObstacleAtPosition) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	int GridIndex = (int)luaL_checkinteger(L, 2);
//	bool Force = lua_toboolean(L, 3);
//	lua_pushboolean(L, room->CanSpawnObstacleAtPosition(GridIndex, Force));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomGetWaterAmount)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushnumber(L, *room->GetWaterAmount());
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetWaterAmount)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	float amount = (float)luaL_checknumber(L, 2);
//	*room->GetWaterAmount() = amount;
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetFloorColor)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//
//	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
//	*toLua = *room->GetFloorColor();
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomGetWaterColor)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//
//	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
//	*toLua = *room->GetWaterColor();
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetWaterColor)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
//	*room->GetUnknownWaterInt() = 1; // See Room.zhl for more info
//	*room->GetWaterColor() = *color;
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetWaterColorMultiplier)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//
//	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
//	*toLua = *room->GetWaterColorMultiplier();
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetWaterColorMultiplier)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
//	*room->GetUnknownWaterInt() = 1;
//	*room->GetWaterColorMultiplier() = *color;
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomSetWaterCurrent)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
//	*room->GetWaterCurrent() = *vector;
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetEffects)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//
//	TemporaryEffects* effects = room->GetTemporaryEffects();
//	lua::luabridge::UserdataPtr::push(L, effects, lua::GetMetatableKey(lua::Metatables::_TEMPORARY_EFFECTS));
//
//	return 1;
//}
//
//LUA_FUNCTION(lua_RoomGetRail) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_Integer index = luaL_checkinteger(L, 2);
//
//	if (!room->_descriptor->Data->IsAllowedGridIndex(index)) {
//		// return luaL_error(L, "Invalid grid index %I\n", index);
//		lua_pushnil(L);
//	} else {
//		uint8_t railType = room->GetRailType((uint8_t)index);
//		if (railType == 255) {
//			lua_pushnil(L);
//		}
//		else {
//			lua_pushinteger(L, railType);
//		}
//	}
//
//	return 1;
//}
//
//LUA_FUNCTION(lua_RoomSetRail) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_Integer index = luaL_checkinteger(L, 2);
//
//	if (!room->IsValidGridIndex(index, false)) {
//		return luaL_error(L, "Invalid grind index %lld\n", index);
//	}
//
//	lua_Integer rail = luaL_checkinteger(L, 3);
//	if (!Room::IsValidRailType(rail)) {
//		return luaL_error(L, "Invalid rail type %lld\n", index);
//	}
//
//	room->SetRailType((uint8_t)index, (RailType)rail);
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomCanPickupGridEntity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	int gridIndex = (int)luaL_checkinteger(L, 2);
//	lua_pushboolean(L, room->CanPickupGridEntity(gridIndex));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomPickupGridEntity)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	int gridIndex = (int)luaL_checkinteger(L, 2);
//	Entity_Effect* ent = room->PickupGridEntity(gridIndex);
//
//	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomGetGridIndexByTile)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	int gridRow = (int)luaL_checkinteger(L, 2);
//	int gridColumn = (int)luaL_checkinteger(L, 3);
//	lua_pushinteger(L, room->GetGridIndexByTile(gridRow, gridColumn));
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetPauseTimer)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	room->SetPauseTimer((int)luaL_checkinteger(L, 2));
//
//	return 0;
//}
//
//LUA_FUNCTION(Lua_Room_GetBossChampionChance) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushnumber(L, room->GetChampionBossChance());
//	return 1;
//}
//
//LUA_FUNCTION(Lua_Room_IsChampionBossSeed) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushboolean(L, room->IsChampionBossSeed());
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomColorModifierUpdate)
//{
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	bool process = lua::luaL_optboolean(L, 2, true);
//	bool lerp = lua::luaL_optboolean(L, 3, true);
//	float rate = (float)luaL_optnumber(L, 4, 0.015);
//
//	ColorModState* pColor;
//	if (process) {
//		pColor = &room->ComputeColorModifier();
//	}
//	else {
//		pColor = room->GetFXParams()->GetColorModifier();
//	}
//
//	g_Game->SetColorModifier(pColor, lerp, rate);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomTryGetShopDiscount) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	const int shopItemIdx = (int)luaL_checkinteger(L, 2);
//	const int price = (int)luaL_checkinteger(L, 3);
//
//	lua_pushinteger(L, room->TryGetShopDiscount(shopItemIdx, price));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomGetRoomClearDelay) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushinteger(L, room->_roomClearDelay);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetRoomClearDelay) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	room->_roomClearDelay = (int)luaL_checkinteger(L, 2);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetGreedWaveTimer) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushinteger(L, room->_greedWaveTimer);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetGreedWaveTimer) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	room->_greedWaveTimer = (int)luaL_checkinteger(L, 2);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomIsPersistentRoomEntity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	const int type = (int)luaL_checkinteger(L, 2);
//	const int varinat = (int)luaL_optinteger(L, 3, 0);
//	lua_pushboolean(L, room->IsPersistentRoomEntity(type, varinat, 0));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomTrySpawnSpecialQuestDoor) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	roomASM.ForceSpecialQuestDoor = lua::luaL_optboolean(L, 2, false);
//	lua_pushboolean(L, room->TrySpawnSpecialQuestDoor());
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetLavaIntensity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	room->_lavaIntensity = (float)luaL_checknumber(L, 2);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetLightningIntensity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushnumber(L, g_Game->_lightningIntensity);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetLightningIntensity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	g_Game->_lightningIntensity = (float)luaL_checknumber(L, 2);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomDoLightningStrike) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	unsigned int seed = (unsigned int)luaL_optinteger(L, 2, Isaac::genrand_int32());
//	RNG rng; // oppa tyrone style
//	rng.game_constructor(seed, 35);
//	float intensity = 1.3f + rng.RandomFloat()*.6f;
//	
//	g_Game->_lightningIntensity = intensity;
//	g_Manager->_sfxManager.Play(472, 1.0, 90, false, 0.9f + rng.RandomFloat() * 0.2f, 0);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetRainIntensity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushnumber(L, room->_rainIntensity);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_RoomSetRainIntensity) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	room->_rainIntensity = (float)luaL_checknumber(L, 2);
//	return 0;
//}
//
//LUA_FUNCTION(Lua_RoomGetNumRainSpawners) {
//	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, lua::metatables::RoomMT);
//	lua_pushinteger(L, room->_numRainSpawners);
//	return 1;
//}
//
//HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
//	super();
//
//	lua::LuaStackProtector protector(_state);
//
//	luaL_Reg functions[] = {
//		{ "GetShopItemPrice", Lua_RoomGetShopItemPrice},
//		{ "SpawnGridEntity", Lua_SpawnGridEntity},
//		{ "RemoveGridEntityImmediate", Lua_RemoveGridEntityImmediate},
//		{ "CanSpawnObstacleAtPosition", Lua_RoomCanSpawnObstacleAtPosition},
//		{ "GetWaterAmount", Lua_RoomGetWaterAmount},
//		{ "SetWaterAmount", Lua_RoomSetWaterAmount},
//		{ "GetFloorColor", Lua_RoomGetFloorColor},
//		{ "GetWaterColor", Lua_RoomGetWaterColor},
//		{ "SetWaterColor", Lua_RoomSetWaterColor},
//		{ "SetWaterCurrent", Lua_RoomSetWaterCurrent},
//		{ "GetWaterColorMultiplier", Lua_RoomGetWaterColorMultiplier},
//		{ "SetWaterColorMultiplier", Lua_RoomSetWaterColorMultiplier},
//		{ "SetBackdropType", Lua_RoomSetBackdrop},
//		{ "GetEffects", Lua_RoomGetEffects},
//		{ "GetRail", lua_RoomGetRail},
//		{ "SetRail", lua_RoomSetRail},
//		{ "CanPickupGridEntity", Lua_RoomCanPickupGridEntity},
//		{ "PickupGridEntity", Lua_RoomPickupGridEntity},
//		{ "GetGridIndexByTile", Lua_RoomGetGridIndexByTile},
//		{ "SetPauseTimer", Lua_RoomSetPauseTimer},
//		{ "GetChampionBossChance", Lua_Room_GetBossChampionChance},
//		{ "IsChampionBossSeed", Lua_Room_IsChampionBossSeed},
//		{ "UpdateColorModifier", Lua_RoomColorModifierUpdate},
//		{ "TryGetShopDiscount", Lua_RoomTryGetShopDiscount},
//		{ "GetRoomClearDelay", Lua_RoomGetRoomClearDelay},
//		{ "SetRoomClearDelay", Lua_RoomSetRoomClearDelay},
//		{ "GetGreedWaveTimer", Lua_RoomGetGreedWaveTimer},
//		{ "SetGreedWaveTimer", Lua_RoomSetGreedWaveTimer},
//		{ "IsPersistentRoomEntity", Lua_RoomIsPersistentRoomEntity},
//		{ "TrySpawnSpecialQuestDoor", Lua_RoomTrySpawnSpecialQuestDoor},
//		{ "SetLavaIntensity", Lua_RoomSetLavaIntensity},
//		{ "GetLightningIntensity", Lua_RoomGetLightningIntensity},
//		{ "SetLightningIntensity", Lua_RoomSetLightningIntensity},
//		{ "DoLightningStrike", Lua_RoomDoLightningStrike},
//		{ "GetRainIntensity", Lua_RoomGetRainIntensity},
//		{ "SetRainIntensity", Lua_RoomSetRainIntensity},
//		{ "GetNumRainSpawners", Lua_RoomGetNumRainSpawners},
//		{ NULL, NULL }
//	};
//	lua::RegisterFunctions(_state, lua::Metatables::ROOM, functions);
//}