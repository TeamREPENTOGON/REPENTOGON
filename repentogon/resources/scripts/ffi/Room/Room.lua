ffi.cdef [[
    struct EntityList {
        int RenderMode : 0xa4;
        int EnemyCount : 0xa8;
        int EnemyCount2 : 0xac;
        int BossCount : 0xb0;
        int BossCount2 : 0xb4;
        float EnemyDamageInflicted : 0xbc;
    } : 0x400;

    struct Room {
        private bool Initialized : 0x0;
        private bool FirstVisit : 0x1;
        private struct RoomDescriptor* RoomDescriptor : 0x4;
        private int Type : 0x8;
        private int GridWidth : 0xc;
        private int GridHeight : 0x10;
        private struct Vector TopLeftPos : 0x14;
        private struct Vector BottomRightPos : 0x1c;
        private struct GridEntity* GridEntities[448] : 0x24;
        private int TintedRockIdx : 0x764;
        private int DungeonRockIdx : 0x768;
        private int GridPaths[456] : 0x76c;
        private struct Camera* Camera : 0x11f8;
        private int RoomClearDelay : 0x11ec;
        private bool CardAgainstHumanity : 0x11f4;
        private struct Vector RenderSurfaceTopLeft : 0x11fc;
        private struct Vector RenderScrollOffset : 0x1204;
        private bool RedHeartDamage : 0x120c;
        private int BrokenWatchState : 0x1210;
        private struct EntityList EntityList : 0x1218;
        private struct FXLayers FXLayers : 0x1618;
        private int BossID : 0x1bb0;
        private int SecondBossID : 0x1bb4;
        private struct Backdrop Backdrop : 0x1bc8;
        private int ShopLevel : 0x7068;
        private bool _HasTriggerPressurePlates : 0x7229;
        private bool FirstEnemyDead : 0x72a0;
        private int WallBloodDuration : 0x7214;
	    private int WallBloodCount : 0x7218;
        private int SlowDownDuration : 0x722c;
        private int GreedWaveTimer : 0x723c;
        private float WaterAmount : 0x7240;
        private struct KColor WaterColor : 0x7244;
	    private struct KColor WaterColorMultiplier : 0x7258;
        private struct KColor WaterLerpTargetColor : 0x726c;
        private struct KColor WaterLerpTargetColorMult : 0x7280;
        private int WaterLerpColorDuration : 0x7294;
        private struct Color FloorColor : 0x72a4;
        private struct Color WallColor : 0x72d0;
        private struct TemporaryEffects TemporaryEffects : 0x7310;
        private bool _StopRain : 0x745c;
        private int NumRainSpawners : 0x7454;
        private float RainIntensity : 0x7458;
        private struct Vector WaterCurrent : 0x7460;
        private struct RailManager RailManager : 0x7468;
        private float LavaIntensity : 0x7740;
        private bool InvalidatePickupVision : 0x7894;
    } : 0x7898;

    typedef struct Room* RoomPtr;

    bool L_Room_CanPickupGridEntity(struct Room*, int);
    bool L_Room_CanSpawnObstacleAtPosition(struct Room*, int, bool);
    bool L_Room_CheckLine(struct Room*, struct Vector*, struct Vector*, int, int, bool, bool, struct Vector*);
    void L_Room_ClearBossHazards(struct Room*, bool, struct Entity*);
    bool L_Room_DamageGrid(struct Room*, int, float);
    bool L_Room_DamageGridWithSource(struct Room*, int, float, struct EntityRef*);
    bool L_Room_DestroyGrid(struct Room*, int, bool);
    bool L_Room_DestroyGridWithSource(struct Room*, int, bool, struct EntityRef*);
    void L_Room_DoLightningStrike(struct Room*, unsigned int);
    void L_Room_FindFreePickupSpawnPosition(struct Room*, struct Vector*, float, bool, bool, struct Vector*);
    void L_Room_FindFreeTilePosition(struct Room*, struct Vector*, float, struct Vector*);
    unsigned int L_Room_GetBossVictoryJingle(struct Room*);
    int L_Room_GetFrameCount(struct Room*);
    void L_Room_GetCenterPos(struct Room*, struct Vector*);
    float L_Room_GetChampionBossChance(struct Room*);
    int L_Room_GetClampedGridIndex(struct Room*, struct Vector*);
    void L_Room_GetClampedPosition(struct Room*, struct Vector*, float, struct Vector*);
    float L_Room_GetDevilRoomChance(struct Room*);
    struct GridEntityDoor* L_Room_GetDoor(struct Room*, int);
    void L_Room_GetDoorSlotPosition(struct Room*, int, struct Vector*);
    unsigned int L_Room_GetGridCollision(struct Room*, int);
    int L_Room_GetGridCollisionAtPos(struct Room*, struct Vector*);
    struct GridEntity* L_Room_GetGridEntity(struct Room*, int);
    struct GridEntity* L_Room_GetGridEntityFromPos(struct Room*, struct Vector*);
    int L_Room_GetGridIndex(struct Room*, struct Vector*);  
    int L_Room_GetGridIndexByTile(struct Room*, int, int);
    int L_Room_GetGridPathFromPos(struct Room*, struct Vector*);
    void L_Room_GetGridPosition(struct Room*, int, struct Vector*);
    int L_Room_GetItemPool(struct Room*, unsigned int, bool);
    void L_Room_GetLaserTarget(struct Room*, struct Vector*, struct Vector*, struct Vector*);
    float L_Room_GetLightingAlpha(struct Room*);
    float L_Room_GetLightningIntensity();
    void L_Room_GetLRoomAreaDesc(struct Room*, struct LRoomAreaDesc*);
    void L_Room_GetLRoomTileDesc(struct Room*, struct LRoomTileDesc*);
    int L_Room_GetRail(struct Room*, int);
    void L_Room_GetRandomPosition(struct Room*, float, struct Vector*);
    int L_Room_GetRandomTileIndex(struct Room*, unsigned int seed);
    int L_Room_GetRoomConfigStage(struct Room*);
    int L_Room_GetSeededCollectible(struct Room*, float, bool);
    int L_Room_GetShopItemPrice(struct Room*, unsigned int, unsigned int, int);
    bool L_Room_IsAmbushActive();   
    bool L_Room_IsChampionBossSeed(struct Room*);
    bool L_Room_IsCurrentRoomLastBoss();
    bool L_Room_IsValidGridIndex(struct Room*, int, bool);
    bool L_Room_IsMirrorWorld();
    bool L_Room_IsPersistentRoomEntity(struct Room*, int, int);
    bool L_Room_IsPositionInRoom(struct Room*, struct Vector*, float);
    void L_Room_PlayMusic(struct Room*);
    void L_Room_RemoveDoor(struct Room*, int);
    void L_Room_RemoveGridEntity(struct Room*, int, int, bool);
    void L_Room_RemoveGridEntityImmediate(struct Room*, int, int, bool);
    void L_Room_Render(struct Room*);
    void L_Room_RespawnEnemies(struct Room*);
    void L_Room_SaveState(struct Room*);
    void L_Room_ScreenWrapPosition(struct Room*, struct Vector*, float, struct Vector*);
    void L_Room_SetBackdropType(struct Room*, int, int);
    void L_Room_SetItemPool(struct Room*, int);
    void L_Room_SetLightningIntensity(int);
    void L_Room_SetPauseTimer(struct Room*, int);
    void L_Room_SetRailType(struct Room*, int, int);
    void L_Room_ShopReshuffle(struct Room*, bool, bool);
    void L_Room_ShopRestockFull(struct Room*);
    void L_Room_ShopRestockPartial(struct Room*);
    void L_Room_SpawnClearAward(struct Room*);
    bool L_Room_SpawnGridEntity(struct Room*, int, unsigned int, unsigned int, unsigned int, unsigned int);
    bool L_Room_SpawnGridEntityDesc(struct Room*, int, struct GridEntityDesc*);
    void L_Room_TriggerClear(struct Room*, bool);
    void L_Room_TriggerOutput(struct Room*, int);
    void L_Room_TriggerRestock(struct Room*, int, int);
    int L_Room_TryGetShopDiscount(struct Room*, int, int);
    bool L_Room_TryMakeBridge(struct Room*, struct GridEntityPit*, struct GridEntityRock*);
    bool L_Room_TrySpawnBlueWombDoor(struct Room*, bool, bool, bool);
    bool L_Room_TrySpawnBossRushDoor(struct Room*, bool, bool);
    bool L_Room_TrySpawnDevilRoomDoor(struct Room*, bool, bool);
    bool L_Room_TrySpawnMegaSatanRoomDoor(struct Room*, bool);
    bool L_Room_TrySpawnSecretExit(struct Room*, bool, bool);
    bool L_Room_TrySpawnSecretShop(struct Room*, bool);
    bool L_Room_TrySpawnSpecialQuestDoor(struct Room*, bool);
    bool L_Room_TrySpawnTheVoidDoor(struct Room*, bool);
    void L_Room_TurnGold(struct Room*);
    void L_Room_Update(struct Room*);
    void L_Room_UpdateColorModifier(struct Room*, bool, bool, float);
    void L_Room_WorldToScreenPosition(struct Room*, struct Vector*, struct Vector*);

    unsigned int L_Room_TMP_GetSpawnGridEntitySeed(int);
    int L_Room_TMP_GetNumItemPools();
]]

local cfuncs = {
    GetEntities = __Lua_Room_GetEntities,
    MamaMegaExplosion = __Lua_Room_MamaMegaExplosion,
    PickupGridEntity = __Lua_Room_PickupGridEntity,
}

local ffi = ffi
local repentogon = ffidll

local RoomMT
RoomMT = {
    __type = "Room",
    CanPickupGridEntity = function(self, gridIndex)
        ffichecks.checkinteger(1, gridIndex)
        return repentogon.L_Room_CanPickupGridEntity(self, gridIndex)
    end,
    CanSpawnObstacleAtPosition = function(self, gridIndex, force)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkboolean(2, force)
        return repentogon.L_Room_CanSpawnObstacleAtPosition(self, gridIndex, force)
    end,
    ClearBossHazards = function(self, ignoreNPCs, source)
        ignoreNPCs = ffichecks.optboolean(ignoreNPCs, false);
        source = ffichecks.optcdata(source, nil);
        return repentogon.L_Room_ClearBossHazards(self, ignoreNPCs, source)
    end,
    CheckLine = function(self, pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable)
        ffichecks.checkcdata(1, pos1, "Vector")
        ffichecks.checkcdata(2, pos2, "Vector")
        ffichecks.checkinteger(3, mode)
        gridPathThreshold = ffichecks.optnumber(gridPathThreshold, 0)
        ignoreWalls = ffichecks.optboolean(ignoreWalls, false)
        ignoreCrushable = ffichecks.optboolean(ignoreCrushable, false)
        
        local hitPos = Vector()
        local res = repentogon.L_Room_CheckLine(self, pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable, hitPos)
        return res, hitPos
    end,
    DamageGrid = function(self, index, damage)
        ffichecks.checkinteger(1, index)
        ffichecks.checknumber(2, damage)

        return repentogon.L_Room_DamageGrid(self, index, damage)
    end,
    DamageGridWithSource = function(self, index, damage, source)
        ffichecks.checkinteger(1, index)
        ffichecks.checknumber(2, damage)
        ffichecks.checkcdata(3, source, "EntityRef")

        return repentogon.L_Room_DamageGridWithSource(self, index, damage, source)
    end,
    DestroyGrid = function(self, index, immediate)
        ffichecks.checkinteger(1, index)
        ffichecks.checkboolean(2, immediate)

        return repentogon.L_Room_DestroyGrid(self, index, damage)
    end,
    DestroyGridWithSource = function(self, index, immediate, source)
        ffichecks.checkinteger(1, index)
        ffichecks.checkboolean(2, immediate)
        ffichecks.checkcdata(3, source, "EntityRef")

        return repentogon.L_Room_DestroyGridWithSource(self, index, immediate, source)
    end,
    DoLightningStrike = function(seed)
        seed = ffichecks.optnumber(seed, math.random(0, 4294967295))
        repentogon.L_Room_DoLightningStrike(self, seed)
    end,
    EmitBloodFromWalls = function(self, duration, count)
        ffichecks.checkinteger(1, duration)
        ffichecks.checkinteger(2, count)
        ffi.setprivate(self, "WallBloodDuration", duration)
        ffi.setprivate(self, "WallBloodCount", count)
    end,
    FindFreePickupSpawnPosition = function(self, pos, initialStep, avoidActiveEntities, allowPits)
        ffichecks.checkcdata(1, pos, "Vector")
        initialStep = ffichecks.optnumber(initialStep, 0)
        avoidActiveEntities = ffichecks.optboolean(avoidActiveEntities, false)
        allowPits = ffichecks.optboolean(allowPits, false)

        local out = Vector()
        repentogon.L_Room_FindFreePickupSpawnPosition(self, pos, initialStep, avoidActiveEntities, allowPits, out)
        return out
    end,
    FindFreeTilePosition = function(self, pos, distanceThreshold)
        ffichecks.checkcdata(1, pos, "Vector")
        ffichecks.checknumber(2, distanceThreshold)

        local out = Vector()
        repentogon.L_Room_FindFreeTilePosition(self, pos, distanceThreshold, out)
        return out
    end,
    GetAliveBossesCount = function(self)
        return ffi.getprivate(self, "EntityList").BossCount + ffi.getprivate(self, "EntityList").BossCount2
    end,
    GetAliveEnemiesCount = function(self)
        return ffi.getprivate(self, "EntityList").EnemyCount + ffi.getprivate(self, "EntityList").EnemyCount2
    end,
    GetAwardSeed = function(self)
        if ffi.getprivate(self, "RoomDescriptor") then
            return ffi.getprivate(self, "RoomDescriptor").AwardSeed
        end
        return 0
    end,
    GetBackdrop = function(self)
        return ffi.getprivate(self, "Backdrop")
    end,
    GetBackdropType = function(self)
        return ffi.getprivate(ffi.getprivate(self, "Backdrop"), "Type")
    end,
    GetBossID = function(self)
        return ffi.getprivate(self, "BossID")
    end,
    GetBossVictoryJingle = function(self)
        return repentogon.L_Room_GetBossVictoryJingle(self)
    end,
    GetBottomRightPos = function(self)
        return ffi.getprivate(self, "BottomRightPos")
    end,
    GetBrokenWatchState = function(self)
        return ffi.getprivate(self, "BrokenWatchState")
    end,
    GetCenterPos = function(self)
        local out = Vector()
        repentogon.L_Room_GetCenterPos(self, out)
        return out
    end,
    GetCamera = function(self)
        return ffi.getprivate(self, "Camera")
    end,
    GetChampionBossChance = function(self)
        return repentogon.L_Room_GetChampionBossChance(self)
    end,
    GetClampedGridIndex = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        return repentogon.L_Room_GetClampedGridIndex(self, pos)
    end,
    GetClampedPosition = function(self, pos, margin)
        ffichecks.checkcdata(1, pos, "Vector")
        ffichecks.checknumber(2, margin)
        local out = Vector()
        repentogon.L_Room_GetClampedPosition(self, pos, margin, out)
        return out
    end,
    GetDecorationSeed = function(self)
        if ffi.getprivate(self, "RoomDescriptor") then
            return ffi.getprivate(self, "RoomDescriptor").DecorationSeed
        end
        return 0
    end,
    GetDeliriumDistance = function(self)
        if ffi.getprivate(self, "RoomDescriptor") then
            return ffi.getprivate(self, "RoomDescriptor").DeliriumDistance
        end
        return 0
    end,
    GetDevilRoomChance = function(self)
        return repentogon.L_Room_GetDevilRoomChance(self)
    end,
    GetDoor = function(self, slot)
        ffichecks.checkinteger(1, slot)
        local door = repentogon.L_Room_GetDoor(self, slot)
        if door == nil then return nil end
        return door
    end,
    GetDoorSlotPosition = function(self, slot)
        ffichecks.checkinteger(1, slot)
        local out = Vector()
        repentogon.L_Room_GetDoorSlotPosition(self, slot, out)
        return out
    end,
    GetDungeonRockIdx = function(self)
        return ffi.getprivate(self, "DungeonRockIdx")
    end,
    GetEffects = function(self)
        return ffi.getprivate(self, "TemporaryEffects")
    end,
    GetEnemyDamageInflicted = function(self)
        return ffi.getprivate(self, "EntityList").EnemyDamageInflicted
    end,
    GetEntities = function(self)
        return cfuncs.GetEntities(self)
    end,
    GetFloorColor = function(self)
        return ffi.getprivate(self, "FloorColor")
    end,
    GetFrameCount = function(self)
        return repentogon.L_Room_GetFrameCount(self)
    end,
    GetFXLayers = function(self)
        return ffi.getprivate(self, "FXLayers")
    end,
    GetFXParams = function(self)
        return ffi.getprivate(ffi.getprivate(self, "FXLayers"), "FXParams")
    end,
    GetGreedWaveTimer = function(self)
        return ffi.getprivate(self, "GreedWaveTimer")
    end,
    GetGridCollision = function(self, index)
        ffichecks.checkinteger(1, index)
        return repentogon.L_Room_GetGridCollision(self, index)
    end,
    GetGridCollisionAtPos = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        return repentogon.L_Room_GetGridCollisionAtPos(self, pos)
    end,
    GetGridEntity = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 0 or index > 447 then return nil end
        local grid = ffi.getprivate(self, "GridEntities")[index]
        if grid == nil then return nil end
        return grid
    end,
    GetGridEntityFromPos = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        local grid = repentogon.L_Room_GetGridEntityFromPos(self, pos)
        if grid == nil then return nil end
        return grid
    end,
    GetGridHeight = function(self)
        return ffi.getprivate(self, "GridHeight")
    end,
    GetGridIndex = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        return repentogon.L_Room_GetGridIndex(self, pos)
    end,
    GetGridIndexByTile = function(self, gridRow, gridColumn)
        local row, column
        if ffichecks.istable(gridRow) then
            if #gridRow ~= 2 then
                ffichecks.argerror(1, "expected table length of 2!")
            end
            row = gridRow[1]
            column = gridRow[2]
        else
            ffichecks.checkinteger(1, gridRow)
            ffichecks.checkinteger(2, gridColumn)
            row = gridRow
            column = gridColumn
        end
        return repentogon.L_Room_GetGridIndexByTile(self, row, column);
    end,
    GetGridPath = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 448 then
            return ffi.getprivate(self, "GridPaths")[index]
        end
        return 0
    end,
    GetGridPathFromPos = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        return repentogon.L_Room_GetGridPathFromPos(self, pos)
    end,
    GetGridPosition = function(self, index)
        if self:GetGridWidth() == 0 then
            return Vector(0, 0)
        end
        ffichecks.checkinteger(1, index)
        local out = Vector()
        repentogon.L_Room_GetGridPosition(self, index, out)
        return out
    end,
    GetGridSize = function(self)
        return ffi.getprivate(self, "GridHeight") * ffi.getprivate(self, "GridWidth")
    end,
    GetGridWidth = function(self)
        return ffi.getprivate(self, "GridWidth")
    end,
    GetItemPool = function(self, seed, raw)
        seed = ffichecks.optnumber(seed, math.random(0, 4294967295))
        raw = ffichecks.optboolean(raw, false)
        return repentogon.L_Room_GetItemPool(self, seed, raw)
    end,
    GetLaserTarget = function(self, pos, dir)
        ffichecks.checkcdata(1, pos, "Vector")
        ffichecks.checkcdata(2, dir, "Vector")
        local out = Vector()
        repentogon.L_Room_GetLaserTarget(self, pos, dir, out)
        return out
    end,
    GetLavaIntensity = function(self)
        return ffi.getprivate(self, "LavaIntensity")
    end,
    GetLightingAlpha = function(self)
        return repentogon.L_Room_GetLightingAlpha(self)
    end,
    GetLightningIntensity = function(self)
        return repentogon.L_Room_GetLightningIntensity();
    end,
    GetLRoomAreaDesc = function(self)
        local out = ffi.new("struct LRoomAreaDesc")
        repentogon.L_Room_GetLRoomAreaDesc(self, out)
        return out
    end,
    GetLRoomTileDesc = function(self)
        local out = ffi.new("struct LRoomTileDesc")
        repentogon.L_Room_GetLRoomTileDesc(self, out)
        return out
    end,
    GetNumRainSpawners = function(self)
        return ffi.getprivate(self, "NumRainSpawners")
    end,
    GetRail = function(self, index)
        ffichecks.checkinteger(1, index)
        local ret = repentogon.L_Room_GetRail(self, index)
        if ret == -1 then
            return nil
        else
            return ret
        end
    end,
    GetRailManager = function(self)
        return ffi.getprivate(self, "RailManager")
    end,
    GetRainIntensity = function(self)
        return ffi.getprivate(self, "RainIntensity")
    end,
    GetRandomPosition = function(self, margin)
        ffichecks.checknumber(1, margin)
        local out = Vector()
        repentogon.L_Room_GetRandomPosition(self, margin, out)
        return out
    end,
    GetRandomTileIndex = function(self, seed)
        ffichecks.checkinteger(1, seed)
        return repentogon.L_Room_GetRandomTileIndex(self, seed)
    end,
    GetRedHeartDamage = function(self)
        return ffi.getprivate(self, "RedHeartDamage")
    end,
    GetRenderMode = function(self)
        return ffi.getprivate(self, "EntityList").RenderMode
    end,
    GetRenderScrollOffset = function(self)
        return ffi.getprivate(self, "RenderScrollOffset")
    end,
    GetRenderSurfaceTopLeft = function(self)
        return ffi.getprivate(self, "RenderSurfaceTopLeft")
    end,
    GetRoomClearDelay = function(self)
        return ffi.getprivate(self, "RoomClearDelay")
    end,
    GetRoomConfigStage = function(self)
        return repentogon.L_Room_GetRoomConfigStage(self)
    end,
    GetRoomShape = function(self)
        if not ffichecks.isnullptr(ffi.getprivate(self, "RoomDescriptor")) and not ffichecks.isnullptr(ffi.getprivate(self, "RoomDescriptor").Data) then
            return ffi.getprivate(self, "RoomDescriptor").Data.Shape
        end
        return 1
    end,
    GetSecondBossID = function(self)
        return ffi.getprivate(self, "SecondBossID")
    end,
    GetSeededCollectible = function(self, seed, noDecrease)
        ffichecks.checkinteger(1, seed)
        noDecrease = ffichecks.optboolean(noDecrease, false)
        return repentogon.L_Room_GetSeededCollectible(self, seed, noDecrease)
    end,
    GetShopItemPrice = function(entityVariant, entitySubType, shopItemID)
        ffichecks.checkinteger(1, entityVariant)
        ffichecks.checkinteger(2, entitySubType)
        ffichecks.checkinteger(3, shopItemID)
    end,
    GetShopLevel = function(self)
        return ffi.getprivate(self, "ShopLevel")
    end,
    GetSpawnSeed = function(self)
        if ffi.getprivate(self, "RoomDescriptor") then
            return ffi.getprivate(self, "RoomDescriptor").SpawnSeed
        end
        return 0
    end,
    GetTintedRockIdx = function(self)
        return ffi.getprivate(self, "TintedRockIdx")
    end,
    GetTopLeftPos = function(self)
        return ffi.getprivate(self, "TopLeftPos")
    end,
    GetType = function(self)
        return ffi.getprivate(self, "Type")
    end,
    GetWallColor = function(self)
        return ffi.getprivate(self, "WallColor")
    end,
    GetWaterAmount = function(self)
        return ffi.getprivate(self, "WaterAmount")
    end,
    GetWaterColor = function(self)
        return ffi.getprivate(self, "WaterColor")
    end,
    GetWaterColorMultiplier = function(self)
        return ffi.getprivate(self, "WaterColorMultiplier")
    end,
    GetWaterCurrent = function()
        return ffi.getprivate(self, "GetWaterCurrent")
    end,
    HasCurseMist = function(self)   
        return (ffi.getprivate(self, "RoomDescriptor").Flags >> 13) & 1 ~= 0
    end,
    HasLava = function(self)
        if ffi.getprivate(ffi.getprivate(self, "Backdrop"), "Type") == 32 and (ffi.getprivate(self, "RoomDescriptor").Flags >> 5) & 1 ~= 0 then
            return true
        end
        return false
    end,
    HasSlowDown = function(self)
        return ffi.getprivate(self, "SlowDownDuration") > 0
    end,
    HasTriggerPressurePlates = function(self)
        return ffi.getprivate(self, "_HasTriggerPressurePlates")
    end,
    HasWater = function(self)
        return ffi.getprivate(self, "WaterAmount") > 0
    end,
    HasWaterPits = function(self)
        return (ffi.getprivate(self, "RoomDescriptor").Flags >> 5) & 1 ~= 0
    end,
    InvalidatePickupVision = function(self)
        ffi.setprivate(self, "InvalidatePickupVision", true)
    end,
    IsAmbushActive = function(self)
        return repentogon.L_Room_IsAmbushActive()
    end,
    IsAmbushDone = function(self)
        return (ffi.getprivate(self, "RoomDescriptor").Flags >> 3) & 1 ~= 0
    end,
    IsChampionBossSeed = function(self)
        return repentogon.L_Room_IsChampionBossSeed(self)
    end,
    IsClear = function(self)
        return ffi.getprivate(self, "RoomDescriptor").Flags & 1 ~= 0
    end,
    IsCurrentRoomLastBoss = function(self)
        return repentogon.L_Room_IsCurrentRoomLastBoss()
    end,
    IsDoorSlotAllowed = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot >= 0 and slot < 8 then
            return (ffi.getprivate(self, "RoomDescriptor").Data.Doors & (1 << slot)) ~= 0
        end
        return false
    end,
    IsFirstEnemyDead = function(self)
        return ffi.getprivate(self, "FirstEnemyDead")
    end,
    IsFirstVisit = function(self)
        return ffi.getprivate(self, "FirstVisit")
    end,
    IsInitialized = function(self)
        return ffi.getprivate(self, "Initialized")
    end,
    IsLShapedRoom = function(self)
        return ffi.getprivate(self, "RoomDescriptor").Data.Shape - 9 < 4;
    end,
    IsMirrorWorld = function(self)
        return repentogon.L_Room_IsMirrorWorld()
    end,
    IsPersistentRoomEntity = function(self, entityType, entityVariant)
        ffichecks.checkinteger(1, entityType)
        ffichecks.checkinteger(2, entityVariant)
        return repentogon.L_Room_IsPersistentRoomEntity(self, entityType, entityVariant)
    end,
    IsPositionInRoom = function(self, pos, margin)
        ffichecks.checkcdata(1, pos, "Vector")
        ffichecks.checknumber(2, margin)
        return repentogon.L_Room_IsPositionInRoom(self, pos, margin)
    end,
    IsSacrificeDone = function(self)
        return (ffi.getprivate(self, "RoomDescriptor").Flags >> 2) & 1 ~= 0
    end,
    KeepDoorsClosed = function(self)
        ffi.setprivate(self, "RoomClearDelay", 10)
    end,
    MamaMegaExplosion = function(self, pos, player)
        cfuncs.MamaMegaExplosion(self, pos, player)
    end,
    PickupGridEntity = function(self, index)
        return cfuncs.PickupGridEntity(self, index)
    end,
    PlayMusic = function(self)
        repentogon.L_Room_PlayMusic(self)
    end,
    RemoveDoor = function(self, slot)
        ffichecks.checkinteger(1, slot)
        repentogon.L_Room_RemoveDoor(self, slot)
    end,
    RemoveGridEntity = function(self, gridIndex, pathTrail, keepDecoration)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkinteger(2, pathTrail)
        ffichecks.checkboolean(3, keepDecoration)
        repentogon.L_Room_RemoveGridEntity(self, gridIndex, pathTrail, keepDecoration)
    end,        
    RemoveGridEntityImmediate = function(self, gridIndex, pathTrail, keepDecoration)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkinteger(2, pathTrail)
        ffichecks.checkboolean(3, keepDecoration)
        repentogon.L_Room_RemoveGridEntityImmediate(self, gridIndex, pathTrail, keepDecoration)
    end, 
    Render = function(self) 
        repentogon.L_Room_Render(self)
    end,
    RespawnEnemies = function(self) 
        repentogon.L_Room_RespawnEnemies(self)
    end,
    SaveState = function(self)
        repentogon.L_Room_SaveState(self)
    end,
    ScreenWrapPosition = function(self, pos, margin)
        ffichecks.checkcdata(1, pos, "Vector")
        ffichecks.checknumber(2, margin)
        local out = Vector()
        repentogon.L_Room_ScreenWrapPosition(self, pos, margin, out)
        return out
    end,
    SetAmbushDone = function(self, value)
        ffichecks.checkboolean(1, value)
        ffi.getprivate(self, "RoomDescriptor").ChallengeDone = value
    end,
    SetBackdropType = function(self, backdropType, changeDecoration)
        ffichecks.checkinteger(1, backdropType)
        ffichecks.checkinteger(2, changeDecoration)
        if backdropType < 0 or backdropType > 61 then
            ffichecks.argerror(1, string.format("Invalid backdrop id %d (min = 0, max = 61)", backdropType))
        end
        repentogon.L_Room_SetBackdropType(self, backdropType, changeDecoration)
    end,
    SetBrokenWatchState = function(self, state)
        ffichecks.checkinteger(1, state)
        ffi.setprivate(self, "BrokenWatchState", state)
    end,
    SetCardAgainstHumanity = function(self)
        ffi.setprivate(self, "CardAgainstHumanity", true)
    end,
    SetClear = function(self, clear)
        ffichecks.checkboolean(1, clear)
        ffi.getprivate(self, "RoomDescriptor").Clear = clear
    end,
    SetFirstEnemyDead = function(self, value)
        ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "FirstEnemyDead", value)
    end,
    SetFloorColor = function(self, floorColor)
        ffichecks.checkcdata(1, floorColor, "Color")
        ffi.setprivate(self, "FloorColor", floorColor)
    end,
    SetGreedWaveTimer = function(self, time)
        ffichecks.checkinteger(1, time)
        ffi.setprivate(self, "GreedWaveTimer", time)
    end,
    SetItemPool = function(self, type)
        if type < -1 or type >= repentogon.L_Room_TMP_GetNumItemPools() then -- TODO get rid of tmp getter
            ffichecks.argerror(2, "Invalid ItemPoolType")
        end
        repentogon.L_Room_SetItemPool(self, type)
    end,
    SetGridPath = function(self, index, value)
        ffichecks.checkinteger(1, index)
        ffichecks.checkinteger(2, value)
        
        if index < 448 then
            ffi.getprivate(self, "GridPaths")[index] = value
            return true
        end
        return false
    end,
    SetLavaIntensity = function(self, intensity)
        ffichecks.checknumber(1, intensity)
        ffi.setprivate(self, "LavaIntensity", intensity)
    end,
    SetLightningIntensity = function(self, intensity)
        ffichecks.checknumber(1, intensity)
        repentogon.L_Room_SetLightningIntensity(intensity)
    end,
    SetRedHeartDamage = function(self, value)
        value = ffichecks.optboolean(value, true)
        ffi.setprivate(self, "RedHeartDamage", true)
    end,
    SetPauseTimer = function(self, duration)
        ffichecks.checkinteger(1, duration)
        repentogon.L_Room_SetPauseTimer(self, duration)
    end,
    SetRail = function(self, gridIndex, railVariant)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkinteger(2, railVariant)

        if not repentogon.L_Room_IsValidGridIndex(self, gridIndex, false) or gridIndex < 0 or gridIndex >= self:GetGridSize() then
            ffichecks.argerror(1, string.format("Invalid grid index %d", gridIndex))
        end

        repentogon.L_Room_SetRailType(self, gridIndex, railVariant)
    end,
    SetRainIntensity = function(self, intensity)
        ffichecks.checknumber(1, intensity)
        ffi.setprivate(self, "RainIntensity", intensity)
    end,
    SetRoomClearDelay = function(self, delay)
        ffichecks.checkinteger(1, delay)
        ffi.setprivate(self, "RoomClearDelay", delay)
    end,
    SetSacrificeDone = function(self, done)
        ffichecks.checkboolean(1, done)
        ffi.getprivate(self, "RoomDescriptor").SacrificeDone = done
    end,
    SetSlowDown = function(self, duration)
        ffichecks.checkinteger(1, duration)
        ffi.setprivate(self, "SlowDownDuration", duration)
    end,
    SetWallColor = function(self, wallColor)
        ffichecks.checkcdata(1, wallColor, "Color")
        ffi.setprivate(self, "WallColor", wallColor)
    end,
    SetWaterAmount = function(self, amount)
        ffichecks.checknumber(1, amount)
        ffi.setprivate(self, "WaterAmount", amount)
    end,
    SetWaterColor = function(self, waterColor)
        ffichecks.checkcdata(1, waterColor, "KColor")
        ffi.setprivate(self, "WaterLerpColorDuration", 1)
        ffi.setprivate(self, "WaterLerpTargetColor", waterColor)
    end,
    SetWaterColorMultiplier = function(self, waterColor)
        ffichecks.checkcdata(1, waterColor, "KColor")
        ffi.setprivate(self, "WaterLerpColorDuration", 1)
        ffi.setprivate(self, "WaterLerpTargetColorMult", waterColor)
    end,
    SetWaterCurrent = function(self, waterCurrent)
        ffichecks.checkcdata(1, waterCurrent, "Vector")
        ffi.setprivate(self, "WaterCurrent", waterCurrent)
    end,
    ShopReshuffle = function(self, keepCollectibleIdx, reselectSaleItem)
        ffichecks.checkboolean(1, keepCollectibleIdx)
        ffichecks.checkboolean(2, reselectSaleItem)
        repentogon.L_Room_ShopReshuffle(self, keepCollectibleIdx, reselectSaleItem)
    end,
    ShopRestockFull = function(self)
        repentogon.L_Room_ShopRestockFull(self)
    end,
    ShopRestockPartial = function(self)
        repentogon.L_Room_ShopRestockPartial(self)
    end,
    SpawnClearAward = function(self)
        repentogon.L_Room_SpawnClearAward(self)
    end,
    SpawnGridEntity = function(self, gridIndex, gridType, variant, seed, varData)
        ffichecks.checkinteger(1, gridIndex)
        if ffichecks.iscdata(gridType, "GridEntityDesc") then
            return repentogon.L_Room_SpawnGridEntityDesc(self, gridIndex, gridType)
        else
            ffichecks.checkinteger(2, gridType)
            variant = ffichecks.optnumber(variant, 0)
            seed = ffichecks.optnumber(seed, repentogon.L_Room_TMP_GetSpawnGridEntitySeed(gridIndex)) -- suboptimal, but it stays fast this way
            varData = ffichecks.optnumber(varData, 0)
            return repentogon.L_Room_SpawnGridEntity(self, gridIndex, gridType, variant, seed, varData)
        end
    end,
    StopRain = function(self)
        ffi.setprivate(self, "_StopRain", true)
    end,
    TriggerClear = function(self, silent)
        silent = ffichecks.optboolean(silent, false)
        repentogon.L_Room_TriggerClear(self, silent)
    end,
    TriggerOutput = function(self, groupIdx)
        ffichecks.checkinteger(1, groupIdx)
        if groupIdx < 0 or groupIdx > 9 then
            ffichecks.argerror(2, "Invalid output index")
        end
        repentogon.L_Room_TriggerOutput(self, groupIdx)
    end,
    TriggerRestock = function(self, gridIndex, shopItemIdx)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkinteger(2, shopItemIdx)
        repentogon.L_Room_TriggerRestock(self, gridIndex, shopItemIdx)
    end,
    TryGetShopDiscount = function(self, shopItemIdx, price) 
        ffichecks.checkinteger(1, shopItemIdx)
        ffichecks.checkinteger(2, price)
        return repentogon.L_Room_TryGetShopDiscount(self, shopItemIdx, price)
    end,
    TryMakeBridge = function(self, pit, rock)
        pit = pit:ToPit()
        rock = rock:ToRock()
        ffichecks.checkcdata(1, pit, "GridEntityPit")
        ffichecks.checkcdata(2, rock, "GridEntityRock")
        return repentogon.L_Room_TryMakeBridge(self, pit, rock)
    end,
    TrySpawnBlueWombDoor = function(self, firstTime, ignoreTime, force)
        firstTime = ffichecks.optboolean(firstTime, true)
        ignoreTime = ffichecks.optboolean(ignoreTime, true)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnBlueWombDoor(self, firstTime, ignoreTime, force)
    end,
    TrySpawnBossRushDoor = function(self, ignoreTime, force)
        ignoreTime = ffichecks.optboolean(ignoreTime, true)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnBossRushDoor(self, ignoreTime, force)
    end,
    TrySpawnDevilRoomDoor = function(self, animate, force)
        animate = ffichecks.optboolean(animate, true)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnDevilRoomDoor(self, animate, force)
    end,
    TrySpawnMegaSatanRoomDoor = function(self, force)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnMegaSatanRoomDoor(self, force)
    end,
    TrySpawnSecretExit = function(self, animate, force)
        animate = ffichecks.optboolean(animate, true)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnSecretExit(self, animate, force)
    end,
    TrySpawnSecretShop = function(self, force)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnSecretShop(self, force)
    end,
    TrySpawnSpecialQuestDoor = function(self, ignoreStageType)
        ffichecks.optboolean(ignoreStageType, false)
        return repentogon.L_Room_TrySpawnSpecialQuestDoor(self, ignoreStageType)
    end,
    TrySpawnTheVoidDoor = function(self, force)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_Room_TrySpawnTheVoidDoor(self, force)
    end,
    TurnGold = function(self)
        repentogon.L_Room_TurnGold(self)
    end,
    Update = function(self)
        repentogon.L_Room_Update(self)
    end,
    UpdateColorModifier = function(self, process, lerp, rate)
        process = ffichecks.optboolean(process, true)
        lerp = ffichecks.optboolean(lerp, true)
        rate = ffichecks.optnumber(rate, 0.015)
        repentogon.L_Room_UpdateColorModifier(self, process, lerp, rate)
    end,
    WorldToScreenPosition = function(self, worldPos)
        ffichecks.checkcdata(1, worldPos, "Vector")
        local out = Vector()
        repentogon.L_Room_WorldToScreenPosition(self, worldPos, out)
        return out
    end,
}

RoomMT.__index = function(self, key)
    return RoomMT[key]
end

local RoomT = ffi.metatype("struct Room", RoomMT)

Room = setmetatable({}, {
    __class = RoomMT,
})

__Lua_Room_GetEntities = nil
__Lua_Room_MamaMegaExplosion = nil
__Lua_Room_PickupGridEntity = nil
