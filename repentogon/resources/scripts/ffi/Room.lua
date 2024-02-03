ffi.cdef[[
bool L_Room_CheckLine(Vector* pos1, Vector* pos2, int mode, int gridPathThreshold, bool ignoreWalls, bool ignoreCrushable, Vector* buffer);
bool L_Room_DamageGrid(int index, int damage);
bool L_Room_DestroyGrid(int index, bool immediate, bool dontExplodeBombRocks);
void L_Room_EmitBloodFromWalls(int duration, int count);
Vector* L_Room_FindFreePickupSpawnPosition(Vector* pos, float initialStep, bool avoidActiveEntities, bool allowPits);
Vector* L_Room_FindFreeTilePosition(Vector* pos, float distanceThreshold);
int L_Room_GetAliveBossesCount();
int L_Room_GetAliveEnemiesCount();
int L_Room_GetAwardSeed();
int L_Room_GetBackdropType();
int L_Room_GetBossID();
Vector* L_Room_GetBottomRightPos();
int L_Room_GetBrokenWatchState();
Vector* L_Room_GetCenterPos();
int L_Room_GetClampedGridIndex(Vector* pos);
Vector* L_Room_GetClampedPosition(Vector* pos, float margin);
int L_Room_GetDecorationSeed();
int L_Room_GetDeliriumDistance();
float L_Room_GetDevilRoomChance();
GridEntityDoor* L_Room_GetDoor(int doorSlot);
Vector* L_Room_GetDoorSlotPosition(int doorSlot);
int L_Room_GetDungeonRockIdx();
float L_Room_GetEnemyDamageInflicted();
EntityList* L_Room_GetEntities();
int L_Room_GetFrameCount();
int L_Room_GetGridCollision(int gridIndex);
int L_Room_GetGridCollisionAtPos(Vector* pos);
GridEntity* L_Room_GetGridEntity(int index);
GridEntity* L_Room_GetGridEntityFromPos(Vector* pos);
int L_Room_GetGridHeight();
int L_Room_GetGridIndex(Vector* pos);
int L_Room_GetGridPath(int index);
int L_Room_GetGridPathFromPos(Vector* pos);
Vector* L_Room_GetGridPosition(int index);
int L_Room_GetGridSize();
int L_Room_GetGridWidth();
Vector* L_Room_GetLaserTarget(Vector* pos, Vector* dir);
float L_Room_GetLavaIntensity();
float L_Room_GetLightingAlpha();
// LRoomAreaDesc (what are those structs??)
void* L_Room_GetLRoomAreaDesc();
// LRoomTileDesc (what are those structs??)
void* L_Room_GetLRoomTileDesc();
Vector* L_Room_GetRandomPosition(float margin);
int L_Room_GetRandomTileIndex(int seed);
bool L_Room_GetRedHeartDamage();
int L_Room_GetRenderMode();
Vector* L_Room_GetRenderScrollOffset();
Vector* L_Room_GetRenderSurfaceTopLeft();
int L_Room_GetRoomConfigStage();
int L_Room_GetRoomShape();
int L_Room_GetSecondBossID();
unsigned int L_Room_GetSeededCollectible(int seed, bool noDecrease);
int L_Room_GetShopLevel();
int L_Room_GetSpawnSeed();
int L_Room_GetTintedRockIdx();
Vector* L_Room_GetTopLeftPos();
int L_Room_GetType();
Vector* L_Room_GetWaterCurrent();
bool L_Room_HasCurseMist();
bool L_Room_HasLava();
bool L_Room_HasSlowDown();
bool L_Room_HasTriggerPressurePlates();
bool L_Room_HasWater();
bool L_Room_HasWaterPits();
void L_Room_InvalidatePickupVision();
bool L_Room_IsAmbushActive();
bool L_Room_IsAmbushDone();
bool L_Room_IsClear();
bool L_Room_IsCurrentRoomLastBoss();
bool L_Room_IsDoorSlotAllowed(int slot);
bool L_Room_IsFirstEnemyDead();
bool L_Room_IsFirstVisit();
bool L_Room_IsInitialized();
bool L_Room_IsLShapedRoom();
bool L_Room_IsMirrorWorld();
bool L_Room_IsPositionInRoom(Vector* pos, float margin);
bool L_Room_IsSacrificeDone();
void L_Room_KeepDoorsClosed();
void L_Room_MamaMegaExplosion(Vector* pos);
void L_Room_PlayMusic();
void L_Room_RemoveDoor(int slot);
void L_Room_RemoveGridEntity(int gridIndex, int pathTrail, bool keepDecoration);
void L_Room_Render();
void L_Room_RespawnEnemies();
Vector* L_Room_ScreenWrapPosition(Vector* pos, float margin);
void L_Room_SetAmbushDone(bool value);
void L_Room_SetBrokenWatchState(int state);
void L_Room_SetCardAgainstHumanity();
void L_Room_SetClear(bool clear);
void L_Room_SetFirstEnemyDead(bool value);
void L_Room_SetFloorColor(ColorMod* floorColor);
bool L_Room_SetGridPath(int index, int value);
void L_Room_SetRedHeartDamage();
void L_Room_SetSacrificeDone(bool done);
void L_Room_SetSlowDown(int duration);
void L_Room_SetWallColor(ColorMod* wallColor);
void L_Room_ShopReshuffle(bool keepCollectibleIdx, bool reselectSaleItem);
void L_Room_ShopRestockFull();
void L_Room_ShopRestockPartial();
void L_Room_SpawnClearAward();
bool L_Room_SpawnGridEntity(int gridIndex, unsigned int type, unsigned int variant, unsigned int seed, unsigned int varData);
void L_Room_StopRain();
void L_Room_TriggerClear(bool silent);
bool L_Room_TryMakeBridge(GridEntity* pit, GridEntity* rock);
bool L_Room_TrySpawnBlueWombDoor(bool firstTime, bool ignoreTime, bool force);
bool L_Room_TrySpawnBossRushDoor(bool ignoreTime, bool force);
bool L_Room_TrySpawnBrokenShovel();
bool L_Room_TrySpawnDevilRoomDoor(bool animate, bool force);
bool L_Room_TrySpawnMegaSatanRoomDoor(bool force);
bool L_Room_TrySpawnSecretExit(bool animate, bool force);
bool L_Room_TrySpawnSecretShop(bool force);
bool L_Room_TrySpawnSpecialQuestDoor();
bool L_Room_TrySpawnTheVoidDoor(bool force);
void L_Room_TurnGold();
void L_Room_Update();
Vector* L_Room_WorldToScreenPosition(Vector* worldPos);
]]

local repentogon = ffidll
local lffi = ffi

local RoomFuncs = {}

function RoomFuncs:CheckLine(pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable)
    ffichecks.checkcdata(1, pos1, "Vector")
    ffichecks.checkcdata(2, pos2, "Vector")
    ffichecks.checknumber(3, mode)
    gridPathThreshold = ffichecks.optnumber(gridPathThreshold, 0)
    ignoreWalls = ffichecks.optboolean(ignoreWalls, false)
    ignoreCrushable = ffichecks.optboolean(ignoreCrushable, false)

    local resultPos = lffi.new("Vector[1]")
    return repentogon.L_Room_CheckLine(pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable, resultPos), resultPos
end

function RoomFuncs:DamageGrid(index, damage)
    ffichecks.checknumber(1, index)
    ffichecks.checknumber(2, damage)

    return repentogon.L_Room_DamageGrid(index, damage)
end

function RoomFuncs:DestroyGrid(index, immediate, dontExplodeBombRocks)
    ffichecks.checknumber(1, index)
    ffichecks.checkboolean(2, immediate)
    dontExplodeBombRocks = ffichecks.optboolean(dontExplodeBombRocks, false)

    return repentogon.L_Room_DestroyGrid(index, immediate, dontExplodeBombRocks)
end

function RoomFuncs:EmitBloodFromWalls(duration, count)
    ffichecks.checknumber(1, duration)
    ffichecks.checknumber(2, count)

    repentogon.L_Room_EmitBloodFromWalls(duration, count)
end

function RoomFuncs:FindFreePickupSpawnPosition(pos, initialStep, avoidActiveEntities, allowPits)
    ffichecks.checkcdata(1, pos, "Vector")
    initialStep = ffichecks.optnumber(initialStep, 0)
    avoidActiveEntities = ffichecks.optboolean(avoidActiveEntities, false)
    allowPits = ffichecks.optboolean(allowPits, false)

    return lffi.gc(repentogon.L_Room_FindFreePickupSpawnPosition(pos, initialStep, avoidActiveEntities, allowPits), repentogon.L_Free)
end

function RoomFuncs:FindFreeTilePosition(pos, distanceThreshold)
    ffichecks.checkcdata(1, pos, "Vector")
    ffichecks.checknumber(2, distanceThreshold)

    return lffi.gc(repentogon.L_Room_FindFreeTilePosition(pos, distanceThreshold), repentogon.L_Free)
end 

function RoomFuncs:GetAliveBossesCount()
    return repentogon.L_Room_FindFreePickupSpawnPosition()
end

function RoomFuncs:GetAliveEnemiesCount()
    return repentogon.L_Room_GetAliveEnemiesCount()
end

function RoomFuncs:GetAwardSeed()
    return repentogon.L_Room_GetAwardSeed()
end

function RoomFuncs:GetBackdropType()
    return repentogon.L_Room_GetBackdropType()
end

function RoomFuncs:GetBossID()
    return repentogon.L_Room_GetBossID()
end

function RoomFuncs:GetBottomRightPos()
    return lffi.gc(repentogon.L_Room_GetBottomRightPos(), repentogon.L_Free)
end

function RoomFuncs:GetBrokenWatchState()
    return repentogon.L_Room_GetBrokenWatchState()
end 

function RoomFuncs:GetCenterPos()
    return lffi.gc(repentogon.L_Room_GetCenterPos(), repentogon.L_Free)
end

function RoomFuncs:GetClampedGridIndex(pos)
    ffichecks.checkcdata(1, pos, "Vector")
    return repentogon.L_Room_GetClampedGridIndex(pos)
end

function RoomFuncs:GetClampedPosition(pos, margin)
    ffichecks.checkcdata(1, pos, "Vector")
    ffichecks.checknumber(2, margin)

    return lffi.gc(repentogon.L_Room_GetClampedPosition(pos, margin), repentogon.L_Free)
end

function RoomFuncs:GetDecorationSeed()
    return repentogon.L_Room_GetDecorationSeed()
end

function RoomFuncs:GetDeliriumDistance()
    return repentogon.L_Room_GetDeliriumDistance()
end

function RoomFuncs:GetDevilRoomChance()
    return repentogon.L_Room_GetDevilRoomChance()
end

function RoomFuncs:GetDoor(doorSlot)
    ffichecks.checknumber(1, doorSlot)

    return repentogon.L_Room_GetDoor(doorSlot)
end

function RoomFuncs:GetDoorSlotPosition(doorSlot)
    ffichecks.checknumber(1, doorSlot)

    return lffi.gc(repentogon.L_Room_GetDoorSlotPosition(doorSlot), repentogon.L_Free)
end

function RoomFuncs:GetDungeonRockIdx()
    return repentogon.L_Room_GetDungeonRockIdx()
end

function RoomFuncs:GetEnemyDamageInflicted()
    return repentogon.L_Room_GetEnemyDamageInflicted()
end

function RoomFuncs:GetEntities()
    return repentogon.L_Room_GetEntities()
end

function RoomFuncs:GetFrameCount()
    return repentogon.L_Room_GetFrameCount()
end

function RoomFuncs:GetGridCollision(gridIndex)
    ffichecks.checknumber(1, gridIndex)
    
    return repentogon.L_Room_GetGridCollision()
end

function RoomFuncs:GetGridCollisionAtPos(pos)
    ffichecks.checkcdata(1, pos "Vector")

    return repentogon.L_Room_GetGridCollisionAtPos(pos)
end

function RoomFuncs:GetGridEntity(index)
    ffichecks.checknumber(1, index)

    return repentogon.L_Room_GetGridEntity()
end

function RoomFuncs:GetGridEntityFromPos(pos)
    ffichecks.checkcdata(1, pos, "Vector")

    return repentogon.L_Room_GetGridEntityFromPos(pos)
end

function RoomFuncs:GetGridHeight()
    return repentogon.L_Room_GetGridHeight()
end

function RoomFuncs:GetGridIndex(pos)
    ffichecks.checkcdata(1, pos, "Vector")

    return repentogon.L_Room_GetGridIndex(pos)
end

function RoomFuncs:GetGridPath(index)
    ffichecks.checknumber(1, index)

    return repentogon.L_Room_GetGridPath(index)
end

function RoomFuncs:GetGridPathFromPos(pos)
    ffichecks.checkcdata(1, pos, "Vector")

    return repentogon.L_Room_GetGridPathFromPos(pos)
end

function RoomFuncs:GetGridPosition(index)
    ffichecks.checknumber(1, index)

    return lffi.gc(repentogon.L_Room_GetGridPosition(), repentogon.L_Free)
end

function RoomFuncs:GetGridSize()
    return repentogon.L_Room_GetGridSize()
end

function RoomFuncs:GetGridWidth()
    return repentogon.L_Room_GetGridWidth()
end

function RoomFuncs:GetLaserTarget(pos, dir)
    ffichecks.checkcdata(1, pos, "Vector")
    ffichecks.checkcdata(2, dir, "Vector")

    return lffi.gc(repentogon.L_Room_GetLaserTarget(pos, dir), repentogon.L_Free)
end

function RoomFuncs:GetLavaIntensity()
    return repentogon.L_Room_GetLavaIntensity()
end

function RoomFuncs:GetLightingAlpha()
    return repentogon.L_Room_GetLightingAlpha()
end 

function RoomFuncs:GetLRoomAreaDesc()
    return lffi.gc(repentogon.L_Room_GetLRoomAreaDesc(), repentogon.L_Free)
end

function RoomFuncs:GetLRoomTileDesc()
    return lffi.gc(repentogon.L_Room_GetLRoomTileDesc(), repentogon.L_Free)
end

function RoomFuncs:GetRandomPosition(margin)
    ffichecks.checknumber(1, margin)

    return lffi.gc(repentogon.GetRandomPosition(margin), repentogon.L_Free)
end

function RoomFuncs:GetRandomTileIndex(seed)
    ffichecks.checknumber(1, seed)

    return repentogon.GetRandomTileIndex(seed)
end

function RoomFuncs:GetRedHeartDamage()
    return repentogon.L_Room_GetRedHeartDamage()
end

function RoomFuncs:GetRenderMode()
    return repentogon.L_Room_GetRenderMode()
end

function RoomFuncs:GetRenderScrollOffset()
    return lffi.gc(repentogon.L_Room_GetRenderScrollOffset(), repentogon.L_Free)
end

function RoomFuncs:GetRenderSurfaceTopLeft()
    return lffi.gc(repentogon.L_Room_GetRenderSurfaceTopLeft(), repentogon.L_Free)
end

function RoomFuncs:GetRoomConfigStage()
    return repentogon.L_Room_GetRoomConfigStage()
end

function RoomFuncs:GetRoomShape()
    return repentogon.L_Room_GetRoomShape()
end

function RoomFuncs:GetSecondBossID()
    return repentogon.L_Room_GetSecondBossID()
end

function RoomFuncs:GetSeededCollectible(seed, noDecrease)
    ffichecks.checknumber(1, seed)
    noDecrease = ffichecks.optboolean(noDecrease, false)

    return repentogon.L_Room_GetSeededCollectible(seed, noDecrease)
end

function RoomFuncs:GetShopLevel()
    return repentogon.L_Room_GetShopLevel()
end

function RoomFuncs:GetSpawnSeed()
    return repentogon.L_Room_GetSpawnSeed()
end

function RoomFuncs:GetTintedRockIdx()
    return repentogon.L_Room_GetTintedRockIdx()
end

function RoomFuncs:GetTopLeftPos()
    return lffi.gc(repentogon.L_Room_GetTopLeftPos(), repentogon.L_Free)
end

function RoomFuncs:GetType()
    return repentogon.L_Room_GetType()
end

function RoomFuncs:GetWaterCurrent()
    return lffi.gc(repentogon.L_Room_GetWaterCurrent(), repentogon.L_Free)
end

function RoomFuncs:HasCurseMist()
    return repentogon.L_Room_HasCurseMist()
end

function RoomFuncs:HasLava()
    return repentogon.L_Room_HasLava()
end

function RoomFuncs:HasSlowDown()
    return repentogon.L_Room_HasSlowDown()
end

function RoomFuncs:HasTriggerPressurePlates()
    return repentogon.L_Room_HasTriggerPressurePlates()
end

function RoomFuncs:HasWater()
    return repentogon.L_Room_HasWater()
end

function RoomFuncs:HasWaterPits()
    return repentogon.L_Room_HasWaterPits()
end

function RoomFuncs:InvalidatePickupVision()
    repentogon.L_Room_InvalidatePickupVision()
end

function RoomFuncs:IsAmbushActive()
    return repentogon.L_Room_IsAmbushActive()
end

function RoomFuncs:IsAmbushDone()
    return repentogon.L_Room_IsAmbushDone()
end

function RoomFuncs:IsClear()
    return repentogon.L_Room_IsClear()
end

function RoomFuncs:IsCurrentRoomLastBoss()
    return repentogon.L_Room_IsCurrentRoomLastBoss()
end

function RoomFuncs:IsDoorSlotAllowed(slot)
    ffichecks.checknumber(1, slot)

    return repentogon.L_Room_IsDoorSlotAllowed(slot)
end

function RoomFuncs:IsFirstEnemyDead()
    return repentogon.L_Room_IsFirstEnemyDead()
end

function RoomFuncs:IsFirstVisit()
    return repentogon.L_Room_IsFirstVisit()
end

function RoomFuncs:IsInitialized()
    return repentogon.L_Room_IsInitialized()
end

function RoomFuncs:IsLShapedRoom()
    return repentogon.L_Room_IsLShapedRoom()
end

function RoomFuncs:IsMirrorWorld()
    return repentogon.L_Room_IsMirrorWorld()
end

function RoomFuncs:IsPositionInRoom(pos, margin)
    ffichecks.checkcdata(1, pos, "Vector")
    ffichecks.checknumber(2, margin)

    return repentogon.L_Room_IsPositionInRoom(pos, margin)
end

function RoomFuncs:IsSacrificeDone()
    return repentogon.L_Room_IsSacrificeDone()
end

function RoomFuncs:KeepDoorsClosed()
    repentogon.L_Room_KeepDoorsClosed()
end

function RoomFuncs:MamaMegaExplosion(pos)
    ffichecks.checkcdata(1, pos, "Vector")

    repentogon.L_Room_MamaMegaExplosion(pos)
end

function RoomFuncs:PlayMusic()
    repentogon.L_Room_PlayMusic()
end

function RoomFuncs:RemoveDoor(slot)
    ffichecks.checknumber(1, slot)

    repentogon.L_Room_RemoveDoor(slot)
end

function RoomFuncs:RemoveGridEntity(gridIndex, pathTrail, keepDecoration)
    ffichecks.checknumber(1, gridIndex)
    ffichecks.checknumber(2, pathTrail)
    ffichecks.checkboolean(3, keepDecoration)

    return repentogon.L_Room_RemoveGridEntity(gridIndex, pathTrail, keepDecoration)
end

function RoomFuncs:Render()
    repentogon.L_Room_Render()
end

function RoomFuncs:RespawnEnemies()
    repentogon.L_Room_RespawnEnemies()
end

function RoomFuncs:ScreenWrapPosition(pos, margin)
    ffichecks.checkcdata(1, pos, "Vector")
    ffichecks.checknumber(2, margin)

    return lffi.gc(repentogon.L_Room_ScreenWrapPosition(pos, margin), repentogon.L_Free)
end

function RoomFuncs:SetAmbushDone(value)
    ffichecks.checkboolean(1, value)

    repentogon.L_Room_SetAmbushDone(value)
end

function RoomFuncs:SetBrokenWatchState(value)
    ffichecks.checkboolean(1, value)

    repentogon.L_Room_SetBrokenWatchState(value)
end

function RoomFuncs:SetCardAgainstHumanity()
    repentogon.L_Room_SetCardAgainstHumanity()
end

function RoomFuncs:SetClear(clear)
    ffichecks.checkboolean(1, clear)

    repentogon.L_Room_SetClear(clear)
end

function RoomFuncs:SetFirstEnemyDead(value)
    ffichecks.checkboolean(1, value)

    repentogon.L_Room_SetFirstEnemyDead(value)
end

function RoomFuncs:SetFloorColor(color)
    ffichecks.checkcdata(1, color, "ColorMod")

    repentogon.L_Room_SetFloorColor(color)
end

function RoomFuncs:SetGridPath(index, value)
    ffichecks.checknumber(1, index)
    ffichecks.checknumber(2, value)

    return repentogon.L_Room_SetFloorColor(index, value)
end

function RoomFuncs:SetRedHeartDamage()
    repentogon.L_Room_SetRedHeartDamage()
end

function RoomFuncs:SetSacrificeDone(done)
    ffichecks.checkboolean(1, done)

    repentogon.L_Room_SetSacrificeDone(done)
end

function RoomFuncs:SetSlowDown(duration)
    ffichecks.checknumber(1, duration)

    repentogon.L_Room_SetSlowDown(duration)
end

function RoomFuncs:SetWallColor(color)
    ffichecks.checkcdata(1, color, "ColorMod")

    repentogon.L_Room_SetWallColor(color)
end

function RoomFuncs:ShopReshuffle(keepCollectibleIdx, reselectSaleItem)
    ffichecks.checkboolean(1, keepCollectibleIdx)
    ffichecks.checkboolean(2, reselectSaleItem)

    repentogon.L_Room_ShopReshuffle(keepCollectibleIdx, reselectSaleItem)
end

function RoomFuncs:ShopRestockFull()
    repentogon.L_Room_ShopRestockFull()
end

function RoomFuncs:ShopRestockPartial()
    repentogon.L_Room_ShopRestockPartial()
end

function RoomFuncs:SpawnClearAward()
    repentogon.L_Room_SpawnClearAward()
end

function RoomFuncs:SpawnGridEntity(gridIndex, type, variant, seed, vardata)
    ffichecks.checknumber(1, gridIndex)
    ffichecks.checknumber(2, type)
    ffichecks.checknumber(3, variant)
    ffichecks.checknumber(4, seed) 
    ffichecks.checknumber(5, vardata)

    return repentogon.L_Room_SpawnGridEntity(gridIndex, type, variant, seed, vardata)
end

function RoomFuncs:StopRain()
    repentogon.L_Room_StopRain()
end

function RoomFuncs:TriggerClear(silent)
    ffichecks.checkboolean(1, silent)

    repentogon.L_Room_TriggerClear(silent)
end

function RoomFuncs:TryMakeBridge(pit, rock)
    ffichecks.checkcdata(1, pit, "GridEntity")
    ffichecks.checkcdata(2, rock, "GridEntity")

    return repentogon.L_Room_TryMakeBridge(pit, rock)
end

function RoomFuncs:TrySpawnBlueWombDoor(firstTime, ignoreTime, force)
    firstTime = ffichecks.optboolean(firstTime, true)
    ignoreTime = ffichecks.optboolean(ignoreTime, false)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnBlueWombDoor(firstTime, ignoreTime, force)
end

function RoomFuncs:TrySpawnBossRushDoor(ignoreTime, force)
    ignoreTime = ffichecks.optboolean(ignoreTime, false)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnBossRushDoor(ignoreTime, force)
end

function RoomFuncs:TrySpawnBrokenShovel()
    return repentogon.L_Room_TrySpawnBrokenShovel()
end

function RoomFuncs:TrySpawnDevilRoomDoor(animate, force)
    animate = ffichecks.optboolean(animate, false)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnDevilRoomDoor(animate, force)
end

function RoomFuncs:TrySpawnMegaSatanRoomDoor(force)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnMegaSatanRoomDoor(force)
end

function RoomFuncs:TrySpawnSecretExit(animate, force)
    animate = ffichecks.optboolean(animate, false)
    force = ffichecks.optboolean(force, false)

    return repentogon.TrySpawnSecretExit(animate, force)
end

function RoomFuncs:TrySpawnSecretShop(force)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnSecretShop(force)
end

function RoomFuncs:TrySpawnSpecialQuestDoor()
    return repentogon.L_Room_TrySpawnSpecialQuestDoor()
end

function RoomFuncs:TrySpawnTheVoidDoor(force)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_Room_TrySpawnTheVoidDoor(force)
end

function RoomFuncs:TurnGold()
    repentogon.L_Room_TurnGold()
end

function RoomFuncs:Update()
    repentogon.L_Room_Update()
end

function RoomFuncs:WorldToScreenPosition(worldPos)
    ffichecks.checkcdata(1, worldPos, "Vector")

    return lffi.gc(repentogon.L_Room_WorldToScreenPosition(worldPos), repentogon.L_Free)
end

local RoomMT = {
    __index = function(self, key)
        return RoomFuncs[key]
    end
}

Room = setmetatable({}, RoomMT)