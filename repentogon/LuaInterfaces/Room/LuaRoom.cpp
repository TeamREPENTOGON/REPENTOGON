#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Room.h"
#include "../../LuaClasses.h"

#include "../../Patches/ItemPoolManager.h"

RoomASM roomASM;
extern uint32_t hookedbackdroptype;

// TODO: see what we can reimplement in Lualand
extern "C" {

	__declspec(dllexport) bool L_Room_CanPickupGridEntity(Room* room, int gridIndex) {
		return room->CanPickupGridEntity(gridIndex);
	}

	__declspec(dllexport) bool L_Room_CanSpawnObstacleAtPosition(Room* room, int gridIndex, bool force) {
		return room->CanSpawnObstacleAtPosition(gridIndex, force);
	}

	__declspec(dllexport) bool L_Room_CheckLine(Room* room, Vector* pos1, Vector* pos2, int mode, int gridPathThreshold, bool ignoreWalls, bool ignoreCrushable, Vector* hitPos) {
		return room->CheckLine(pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable, hitPos);
	}

	__declspec(dllexport) void L_Room_ClearBossHazards(Room* room, bool excludeNPCs, Entity* source) {
		// this function only uses [this] to create an EntityRef, and the constructor cleanly handles cases where Entity is nullptr
		source->ClearBossHazards(excludeNPCs);
	}

	__declspec(dllexport) bool L_Room_DamageGrid(Room* room, int index, float damage) {
		return room->DamageGrid(index, damage);
	}

	__declspec(dllexport) bool L_Room_DamageGridWithSource(Room* room, int index, float damage, EntityRef* source) {
		return room->DamageGrid(index, damage, source);
	}

	__declspec(dllexport) bool L_Room_DestroyGrid(Room* room, int index, bool immediate) {
		return room->DestroyGrid(index, immediate);
	}

	__declspec(dllexport) bool L_Room_DestroyGridWithSource(Room* room, int index, bool immediate, EntityRef* source) {
		return room->DestroyGrid(index, immediate, source);
	}

	//TODO: we can reimplement this in lua once RNG, Game, SFXManager are moved over
	__declspec(dllexport) void L_Room_DoLightningStrike(Room* room, unsigned int seed) {
		RNG rng; // oppa tyrone style
		rng.game_constructor(seed, 35);
		float intensity = 1.3f + rng.RandomFloat() * .6f;

		g_Game->_lightningIntensity = intensity;
		g_Manager->_sfxManager.Play(472, 1.0, 90, false, 0.9f + rng.RandomFloat() * 0.2f, 0);
	}

	__declspec(dllexport) void L_Room_FindFreePickupSpawnPosition(Room* room, Vector* pos, float initialStep, bool avoidActiveEntities, bool allowPits, Vector* out) {
		room->FindFreePickupSpawnPosition(out, pos, initialStep, avoidActiveEntities, allowPits, false);
	}

	__declspec(dllexport) void L_Room_FindFreeTilePosition(Room* room, Vector* pos, float distanceThreshold, Vector* out) {
		room->FindFreeTilePosition(out, pos, distanceThreshold);
	}
	

	__declspec(dllexport) unsigned int L_Room_GetBossVictoryJingle(Room* room) {
		return room->GetBossVictoryJingle();
	}

	//ditto
	__declspec(dllexport) int L_Room_GetFrameCount(Room* room) {
		return room->GetFrameCount();
	}

	__declspec(dllexport) void L_Room_GetCenterPos(Room* room, Vector* out) {
		room->GetCenterPos(out);
	}

	__declspec(dllexport) float L_Room_GetChampionBossChance(Room* room) {
		return room->GetChampionBossChance();
	}

	__declspec(dllexport) int L_Room_GetClampedGridIndex(Room* room, Vector* pos) {
		return room->GetClampedGridIndex(pos);
	}

	__declspec(dllexport) void L_Room_GetClampedPosition(Room* room, Vector* pos, float margin, Vector* out) {
		room->GetClampedPosition(out, pos, margin, margin, margin, margin);
	}

	__declspec(dllexport) float L_Room_GetDevilRoomChance(Room* room) {
		return room->GetDevilRoomChance();
	}

	__declspec(dllexport) GridEntity_Door* L_Room_GetDoor(Room* room, int slot) {
		return room->GetDoor(slot);
	}

	__declspec(dllexport) void L_Room_GetDoorSlotPosition(Room* room, int slot, Vector* out) {
		room->GetDoorSlotPosition(out, slot);
	}

	__declspec(dllexport) unsigned int L_Room_GetGridCollision(Room* room, int index) {
		return room->GetGridCollision(index);
	}

	__declspec(dllexport) int L_Room_GetGridCollisionAtPos(Room* room, Vector* pos) {
		return room->GetGridCollisionAtPos(*pos);
	}

	__declspec(dllexport) GridEntity* L_Room_GetGridEntity(Room* room, int index) {
		return room->GetGridEntity(index);
	}

	__declspec(dllexport) GridEntity* L_Room_GetGridEntityFromPos(Room* room, Vector* pos) {
		return room->GetGridEntityFromPos(pos);
	}

	__declspec(dllexport) int L_Room_GetGridIndex(Room* room, Vector* pos) {
		return room->GetGridIndex(pos);
	}

	__declspec(dllexport) int L_Room_GetGridIndexByTile(Room* room, int gridRow, int gridColumn) {
		return room->GetGridIndexByTile(gridRow, gridColumn);
	}

	__declspec(dllexport) int L_Room_GetGridPathFromPos(Room* room, Vector* pos) {
		return room->GetGridPathFromPos(pos);
	}

	__declspec(dllexport) void L_Room_GetGridPosition(Room* room, int index, Vector* out) {
		room->GetGridPosition(out, index);
	}

	__declspec(dllexport) int L_Room_GetItemPool(Room* room, unsigned int seed, bool raw) {
		if (raw) {
			return roomASM.ItemPool;
		}
		return Room::GetItemPool(seed, room->_descriptor, 0);
	}

	__declspec(dllexport) void L_Room_GetLaserTarget(Room* room, Vector* pos, Vector* dir, Vector* out) {
		room->GetLaserTarget(out, pos, dir);
	}

	__declspec(dllexport) float L_Room_GetLightingAlpha(Room* room) {
		return room->GetLightingAlpha();
	}

	//TODO: Once we FFI Game, this can live in LuaJIT. I have these going through C because it doesn't actually use Room at all, it would go through Luabridge if I were to do them in Lua now.
	__declspec(dllexport) float L_Room_GetLightningIntensity(Room* room) {
		return g_Game->_lightningIntensity;
	}

	__declspec(dllexport) void L_Room_GetLRoomAreaDesc(Room* room, LRoomAreaDesc* out) {
		room->GetLRoomAreaDesc(out);
	}

	__declspec(dllexport) void L_Room_GetLRoomTileDesc(Room* room, LRoomTileDesc* out) {
		room->GetLRoomTileDesc(out);
	}

	__declspec(dllexport) int L_Room_GetRail(Room* room, int index) {

		if (!room->_descriptor->Data->IsAllowedGridIndex(index)) {
			return -1;
		}
		else {
			uint8_t railType = room->GetRailType((uint8_t)index);
			if (railType == 255) {
				return -1;
			}
			else {
				return railType;
			}
		}
	}

	__declspec(dllexport) void L_Room_GetRandomPosition(Room* room, float margin, Vector* out) {
		room->GetRandomPosition(out, margin);
	}

	__declspec(dllexport) int L_Room_GetRandomTileIndex(Room* room, unsigned int seed) {
		return room->GetRandomTileIndex(seed);
	}

	__declspec(dllexport) int L_Room_GetRoomConfigStage(Room* room) {
		return room->GetRoomConfigStage();
	}

	__declspec(dllexport) int L_Room_GetSeededCollectible(Room* room, unsigned int seed, float noDecrease) {
		return ItemPool::GetSeededCollectible(seed, noDecrease, room->_descriptor);
	}

	__declspec(dllexport) int L_Room_GetShopItemPrice(Room* room, unsigned int entityVariant, unsigned int entitySubType, int shopItemID) {
		return room->GetShopItemPrice(entityVariant, entitySubType, shopItemID);
	}

	//ditto
	__declspec(dllexport) bool L_Room_IsAmbushActive() {
		return g_Game->_ambush.active;
	}

	__declspec(dllexport) bool L_Room_IsChampionBossSeed(Room* room) {
		return room->IsChampionBossSeed();
	}

	__declspec(dllexport) bool L_Room_IsCurrentRoomLastBoss() {
		return Room::IsCurrentRoomLastBoss();
	}

	//ditto
	__declspec(dllexport) bool L_Room_IsMirrorWorld() {
		return Room::IsMirrorWorld();
	}

	__declspec(dllexport) bool L_Room_IsPersistentRoomEntity(Room* room, int type, int variant) {
		return room->IsPersistentRoomEntity(type, variant, 0);
	}
	__declspec(dllexport) bool L_Room_IsPositionInRoom(Room* room, Vector* pos, float margin) {
		return room->IsPositionInRoom(pos, margin);
	}

	__declspec(dllexport) bool L_Room_IsValidGridIndex(Room* room, int gridIndex, bool includeWalls) {
		return room->IsValidGridIndex(gridIndex, includeWalls);
	}

	__declspec(dllexport) void L_Room_PlayMusic(Room* room) {
		room->PlayMusic();
	}

	__declspec(dllexport) void L_Room_RemoveDoor(Room* room, int slot) {
		room->RemoveDoor(slot);
	}

	__declspec(dllexport) void L_Room_RemoveGridEntity(Room* room, int gridIndex, int pathTrail, bool keepDecoration) {
		room->RemoveGridEntity(gridIndex, pathTrail, keepDecoration);
	}

	__declspec(dllexport) void L_Room_RemoveGridEntityImmediate(Room* room, int gridIndex, int pathTrail, bool keepDecoration) {
		room->RemoveGridEntityImmediate(gridIndex, pathTrail, keepDecoration);
	}

	__declspec(dllexport) void L_Room_Render(Room* room) {
		room->Render();
	}
	
	__declspec(dllexport) void L_Room_RespawnEnemies(Room* room) {
		room->RespawnEnemies();
	}

	__declspec(dllexport) void L_Room_SaveState(Room* room) {
		room->SaveState();
	}

	__declspec(dllexport) void L_Room_ScreenWrapPosition(Room* room, Vector* pos, float margin, Vector* out) {
		room->ScreenWrapPosition(out, pos, margin, margin, margin, margin);
	}

	__declspec(dllexport) void L_Room_SetBackdropType(Room* room, int id, int changeDecoration) {
		room->_backdrop.Init(id, (bool)changeDecoration);
	}

	__declspec(dllexport) void L_Room_SetItemPool(Room* room, int poolType) {
		roomASM.ItemPool = poolType;
	}

	__declspec(dllexport) void L_Room_SetLightningIntensity(float intensity) {
		g_Game->_lightningIntensity = intensity;
	}

	__declspec(dllexport) void L_Room_SetPauseTimer(Room* room, int duration) {
		room->SetPauseTimer(duration);
	}

	__declspec(dllexport) void L_Room_SetRailType(Room* room, int gridIndex, int railVariant) {
		room->SetRailType(gridIndex, (RailType)railVariant);
	}

	__declspec(dllexport) void L_Room_ShopReshuffle(Room* room, bool keepCollectibleIdx, bool reselectSaleItem) {
		room->ShopReshuffle(keepCollectibleIdx, reselectSaleItem);
	}

	__declspec(dllexport) void L_Room_ShopRestockFull(Room* room) {
		room->ShopRestockFull();
	}

	__declspec(dllexport) void L_Room_ShopRestockPartial(Room* room) {
		room->ShopRestockPartial();
	}

	__declspec(dllexport) void L_Room_SpawnClearAward(Room* room) {
		room->SpawnClearAward();
	}

	__declspec(dllexport) bool L_Room_SpawnGridEntity(Room* room, int gridIndex, unsigned int gridType, unsigned int variant, unsigned int seed, unsigned int varData) {
		return room->SpawnGridEntity(gridIndex, gridType, variant, seed, varData);
	}

	__declspec(dllexport) bool L_Room_SpawnGridEntityDesc(Room* room, int gridIndex, GridEntityDesc* desc) {
		return room->SpawnGridEntityDesc(gridIndex, desc);
	}

	__declspec(dllexport) void L_Room_TriggerClear(Room* room, bool silent) {
		room->TriggerClear(silent);
	}

	__declspec(dllexport) void L_Room_TriggerOutput(Room* room, int output) {
		room->TriggerOutput(output);
	}

	__declspec(dllexport) void L_Room_TriggerRestock(Room* room, int gridIdx, int shopIdx) {
		room->TriggerRestock(gridIdx, shopIdx);
	}

	__declspec(dllexport) int L_Room_TryGetShopDiscount(Room* room, int shopItemIdx, int price) {
		return room->TryGetShopDiscount(shopItemIdx, price);
	}

	__declspec(dllexport) bool L_Room_TryMakeBridge(Room* room, GridEntity_Pit* pit, GridEntity_Rock* rock) {
		return room->TryMakeBridge(pit, rock);
	}

	__declspec(dllexport) bool L_Room_TrySpawnBlueWombDoor(Room* room, bool firstTime, bool ignoreTime, bool force) {
		return room->TrySpawnBlueWombDoor(firstTime, ignoreTime, force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnBossRushDoor(Room* room, bool ignoreTime, bool force) {
		return room->TrySpawnBossRushDoor(ignoreTime, force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnDevilRoomDoor(Room* room, bool animate, bool force) {
		return room->TrySpawnDevilRoomDoor(animate, force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnMegaSatanRoomDoor(Room* room, bool force) {
		return room->TrySpawnMegaSatanRoomDoor(force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnSecretExit(Room* room, bool animate, bool force) {
		return room->TrySpawnSecretExit(animate, force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnSecretShop(Room* room, bool force) {
		return room->TrySpawnSecretShop(force);
	}

	__declspec(dllexport) bool L_Room_TrySpawnSpecialQuestDoor(Room* room, bool ignoreStageType) {
		roomASM.ForceSpecialQuestDoor = ignoreStageType;
		return room->TrySpawnSpecialQuestDoor();
	}

	__declspec(dllexport) bool L_Room_TrySpawnTheVoidDoor(Room* room, bool force) {
		return room->TrySpawnTheVoidDoor(force);
	}

	__declspec(dllexport) void L_Room_TurnGold(Room* room) {
		room->TurnGold();
	}

	__declspec(dllexport) void L_Room_Update(Room* room) {
		room->Update();
	}

	//ditto
	__declspec(dllexport) void L_Room_ColorModifierUpdate(Room* room, bool process, bool lerp, bool rate) {
		ColorModState pColor;
		if (process) {
			pColor = room->ComputeColorModifier();

		}
		else {
			// It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
			// This logic provides backwards compatability.
			FXParams* fx = room->GetFXParams();
			KColor* c = &fx->roomColor;
			pColor = ColorModState(c->_red, c->_green, c->_blue, c->_alpha, fx->brightness, fx->contrast);
		}

		g_Game->SetColorModifier(&pColor, lerp, rate);
	}

	__declspec(dllexport) void L_Room_WorldToScreenPosition(Room* room, Vector* worldPos, Vector* out) {
		room->WorldToScreenPosition(out, *worldPos);
	}

	__declspec(dllexport) unsigned int L_Room_TMP_GetSpawnGridEntitySeed(int gridIndex) {
		return gridIndex + g_Game->_frameCount + 1;
	}

	__declspec(dllexport) int L_Room_TMP_GetNumItemPools() {
		return ItemPoolManager::GetNumItemPools();
	}
}

LUA_FUNCTION(Lua_RoomGetEntities) {
	Room* room = LuaRoom::Get(L, 1);

	LuaEntityList::PushPtr(L, &room->_entityList._updateEL);

	return 1;
}

LUA_FUNCTION(Lua_RoomMamaMegaExplosion) {
	Room* room = LuaRoom::Get(L, 1);
	Vector position = Vector(0, 0);
	if (LuaVector::IsUnderlyingType(L, 2)) {
		position = *LuaVector::Get(L, 2);
	}
	Entity_Player* player = LuaEntityPlayer::GetOpt(L, 3);

	if (player) {
		room->MamaMegaExplosion(&position, player);
	}
	else {
		room->MamaMegaExplosion(&position);
	}

	return 0;
}

LUA_FUNCTION(Lua_RoomPickupGridEntity)
{
	Room* room = LuaRoom::Get(L, 1);
	int gridIndex = (int)luaL_checkinteger(L, 2);
	Entity_Effect* ent = room->PickupGridEntity(gridIndex);

	LuaEntityEffect::PushPtr(L, ent);

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {

	lua_register(_state, "__Lua_Room_GetEntities", Lua_RoomGetEntities);
	lua_register(_state, "__Lua_Room_MamaMegaExplosion", Lua_RoomMamaMegaExplosion);
	lua_register(_state, "__Lua_Room_PickupGridEntity", Lua_RoomPickupGridEntity);

	super();
}