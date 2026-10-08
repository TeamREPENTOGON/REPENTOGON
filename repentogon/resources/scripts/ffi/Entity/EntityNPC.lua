local Entity = require("ffi.Entity.Entity")

local NPC_FIELDS = [[
    int StateFrame : 0x410;
    private struct PathFinder PathfinderValue : 0x414;
    int State : 0xb64;
    int ProjectileCooldown : 0xba0;
    int ProjectileDelay : 0xba4;
    private struct Vector V1Value : 0xbb0;
    private struct Vector V2Value : 0xbb8;
    int I1 : 0xbc0;
    int I2 : 0xbc4;
    private uint32_t DeliriumBossTypeValue : 0xbc8;
    private uint32_t DeliriumBossVariantValue : 0xbcc;
    private uint16_t DeliriumTransformationTimerValue : 0xbd0;
    private uint8_t DeliriumRemainingAttacksValue : 0xbd2;
    private uint8_t DeliriumStateValue : 0xbd3;
    private uint32_t DeliriumAttackIdValue : 0xbd8;
    private uint8_t DeliriumAttackAngleValue : 0xbdc;
    private uint32_t DeliriumCycleValue : 0xbe0;
    private struct Entity* EntityRefValue : 0xbec;
    private float ScaleValue : 0xc38;
    private float ShieldStrengthValue : 0xc40;
    private bool IsChampionValue : 0xc63;
    private int ChampionColorIdxValue : 0xc64;
    private uint16_t ChampionRegenTimerValue : 0xc74;
    private int BossColorIdxValue : 0xc78;
    int GroupIdx : 0xc7c;
    private struct Color DirtColorValue : 0xcac;
    private int32_t ControllerIdValue : 0xf1c;
    private struct Entity* SirenPlayerValue : 0xf40;
]]

ffi.cdef("struct EntityNPC { " .. Entity.Fields .. NPC_FIELDS .. " } : 0xf60;\ntypedef struct EntityNPC* EntityNPCPtr;")

ffi.cdef [[
    void L_EntityNPC_Morph(struct EntityNPC*, int, int, int, int);
    void L_EntityNPC_KillUnique(struct EntityNPC*);
    void L_EntityNPC_SetCanShutDoors(struct EntityNPC*, bool);
    void L_EntityNPC_SetScale(struct EntityNPC*, float);
    void L_EntityNPC_ResetPathFinderTarget(struct EntityNPC*);
    bool L_EntityNPC_CanReroll(struct EntityNPC*);
    void L_EntityNPC_MakeChampion(struct EntityNPC*, unsigned int, int, bool);
    struct EntityEffect* L_EntityNPC_MakeSplat(struct EntityNPC*, float);
    int L_EntityNPC_GetAliveEnemyCount(struct EntityNPC*);
    void L_EntityNPC_AnimWalkFrame(struct EntityNPC*, const char*, const char*, float);
    int L_EntityNPC_QueryNPCsType(struct EntityNPC*, int, int);
    int L_EntityNPC_QueryNPCsSpawnerType(struct EntityNPC*, int, int, bool);
    int L_EntityNPC_QueryNPCsGroup(struct EntityNPC*, int);
    struct Entity* L_EntityNPC_GetPlayerTarget(struct EntityNPC*);
    void L_EntityNPC_CalcTargetPosition(struct EntityNPC*, float, struct Vector*);
    bool L_EntityNPC_CanBeDamagedFromVelocity(struct EntityNPC*, struct Vector*);
    struct EntityProjectile* L_EntityNPC_FireBossProjectiles(struct EntityNPC*, int, struct Vector*, float, struct ProjectileParams*);
    void L_EntityNPC_FireProjectiles(struct EntityNPC*, struct Vector*, struct Vector*, unsigned int, struct ProjectileParams*);
    int L_EntityNPC_FireBossProjectilesEx(struct EntityNPC*, int, struct Vector*, float, struct ProjectileParams*);
    int L_EntityNPC_FireProjectilesEx(struct EntityNPC*, struct Vector*, struct Vector*, unsigned int, struct ProjectileParams*);
    struct EntityProjectile* L_EntityNPC_GetProjectileResult(unsigned int);
    int L_EntityNPC_GetBackdropId();
    struct EntityProjectile* L_EntityNPC_FireGridEntity(struct EntityNPC*, struct Sprite*, struct GridEntityDesc*, struct Vector*, int);
    void L_EntityNPC_PlaySound(struct EntityNPC*, int, float, int, bool, float);
    struct EntityEffect* L_EntityNPC_MakeBloodCloud(struct EntityNPC*, struct Vector*, struct Color*);
    void L_EntityNPC_MakeBloodSplash(struct EntityNPC*);
    void L_EntityNPC_UpdateDirtColor(struct EntityNPC*, bool);
    bool L_EntityNPC_TryForceTarget(struct EntityNPC*, void*, int);
    unsigned int L_EntityNPC_GetHitListSize(struct EntityNPC*);
    unsigned int L_EntityNPC_GetHitListEntry(struct EntityNPC*, unsigned int);
    void L_EntityNPC_SetEntityRef(struct EntityNPC*, void*);
    int L_EntityNPC_GetFlyingOverride(struct EntityNPC*);
    void L_EntityNPC_SetFlyingOverride(struct EntityNPC*, bool);
    void L_EntityNPC_ClearFlyingOverride(struct EntityNPC*);
    void L_EntityNPC_ApplyTearflagEffects(struct EntityNPC*, struct Vector*, struct BitSet128*, void*, float);
    bool L_EntityNPC_IsBossColor(struct EntityNPC*);
    bool L_EntityNPC_TrySplit(struct EntityNPC*, float, struct EntityRef*, bool);
    bool L_EntityNPC_ReplaceSpritesheet(struct EntityNPC*, int, const char*, bool);
    void L_EntityNPC_UpdatePickupGhosts(struct EntityNPC*);
    void L_EntityNPC_GetLootList(struct EntityNPC*, bool, struct LootList*);
    void L_EntityNPC_GetFireplaceLoot(struct EntityNPC*, bool, struct LootList*);
    void L_EntityNPC_GetShopkeeperLoot(struct EntityNPC*, bool, struct LootList*);
    struct EntityNPC* L_EntityNPC_ThrowSpider(struct Vector*, void*, struct Vector*, bool, float);
    struct EntityNPC* L_EntityNPC_ThrowMaggot(struct Vector*, struct Vector*, float, float);
    struct EntityNPC* L_EntityNPC_ThrowMaggotAtPos(struct Vector*, struct Vector*, float);
    struct EntityNPC* L_EntityNPC_ShootMaggotProjectile(struct Vector*, struct Vector*, float, float);
    struct EntityNPC* L_EntityNPC_ThrowStrider(struct Vector*, void*, struct Vector*);
    struct EntityNPC* L_EntityNPC_ThrowRockSpider(struct Vector*, void*, struct Vector*, int, float);
    struct EntityNPC* L_EntityNPC_ThrowLeech(struct Vector*, void*, struct Vector*, float, bool);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local VectorGetter = helpers.VectorGetter
local VectorSetter = helpers.VectorSetter
local CopyStruct = helpers.CopyStruct
local EntityGetter = helpers.EntityGetter
local EntityToPointer = ffichecks.entitytopointer

local npcType = ffi.typeof("struct EntityNPC*")
local pathfinderSize = ffi.sizeof("struct PathFinder")

local SIREN = 904

local function QueryResults(count)
    local results = {}
    if count > 0 then
        for _, entity in ipairs(ffichecks.entityresults()) do
            local npc = entity:ToNPC()
            if npc ~= nil then
                results[#results + 1] = npc
            end
        end
    end
    return results
end

local function ProjectileResults(count)
    local results = {}
    for i = 0, count - 1 do
        results[i + 1] = repentogon.L_EntityNPC_GetProjectileResult(i)
    end
    return results
end

local getters = {
    CanShutDoors = function(self)
        local result = repentogon.L_Entity_CanShutDoors(self) return result
    end,
    ChildNPC = function(self)
        local child = self.Child
        return child ~= nil and child:ToNPC() or nil
    end,
    EntityRef = EntityGetter("EntityRefValue"),
    ParentNPC = function(self)
        local parent = self.Parent
        return parent ~= nil and parent:ToNPC() or nil
    end,
    Pathfinder = function(self)
        local copy = ffi.new("struct PathFinder")
        ffi.copy(copy, ffi.getprivate(self, "PathfinderValue"), pathfinderSize)
        return copy
    end,
    Scale = Getter("ScaleValue"),
    V1 = VectorGetter("V1Value"),
    V2 = VectorGetter("V2Value"),
}

local setters = {
    CanShutDoors = function(self, value)
        repentogon.L_EntityNPC_SetCanShutDoors(self, not not value)
    end,
    EntityRef = function(self, value)
        repentogon.L_EntityNPC_SetEntityRef(self, EntityToPointer(value))
    end,
    Pathfinder = function(self, value)
        ffichecks.checkcdata(1, value, "PathFinder")
        ffi.copy(ffi.getprivate(self, "PathfinderValue"), value, pathfinderSize)
    end,
    Scale = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_EntityNPC_SetScale(self, value)
    end,
    V1 = VectorSetter("V1Value"),
    V2 = VectorSetter("V2Value"),
}

local methods = {
    AnimWalkFrame = function(self, horizontalAnim, verticalAnim, threshold)
        horizontalAnim = ffichecks.checkstring(1, horizontalAnim)
        verticalAnim = ffichecks.checkstring(2, verticalAnim)
        threshold = ffichecks.checknumber(3, threshold)
        repentogon.L_EntityNPC_AnimWalkFrame(self, horizontalAnim, verticalAnim, threshold)
    end,
    ApplyTearflagEffects = function(self, position, flags, source, damage)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, flags, "BitSet128")
        damage = ffichecks.optnumber(damage, 3.5)
        if damage < 0 then
            damage = 0
        end
        repentogon.L_EntityNPC_ApplyTearflagEffects(self, position, flags, EntityToPointer(source), damage)
    end,
    CalcTargetPosition = function(self, distanceLimit)
        distanceLimit = ffichecks.checknumber(1, distanceLimit)
        local result = Vector(0, 0)
        repentogon.L_EntityNPC_CalcTargetPosition(self, distanceLimit, result)
        return result
    end,
    CanBeDamagedFromVelocity = function(self, velocity)
        ffichecks.checkcdata(1, velocity, "Vector")
        local result = repentogon.L_EntityNPC_CanBeDamagedFromVelocity(self, velocity) return result
    end,
    CanReroll = function(self)
        local result = repentogon.L_EntityNPC_CanReroll(self) return result
    end,
    ClearFlyingOverride = function(self)
        repentogon.L_EntityNPC_ClearFlyingOverride(self)
    end,
    FireBossProjectiles = function(self, numProjectiles, targetPosition, trajectoryModifier, params)
        numProjectiles = ffichecks.checkinteger(1, numProjectiles)
        if numProjectiles <= 0 then
            error(string.format("Invalid amount of projectiles %d\n", numProjectiles), 2)
        end
        ffichecks.checkcdata(2, targetPosition, "Vector")
        trajectoryModifier = ffichecks.checknumber(3, trajectoryModifier)
        ffichecks.checkcdata(4, params, "ProjectileParams")
        local result = repentogon.L_EntityNPC_FireBossProjectiles(self, numProjectiles, targetPosition, trajectoryModifier, params) return result
    end,
    FireBossProjectilesEx = function(self, numProjectiles, targetPosition, trajectoryModifier, params)
        numProjectiles = ffichecks.checkinteger(1, numProjectiles)
        if numProjectiles <= 0 then
            error(string.format("Invalid amount of projectiles %d\n", numProjectiles), 2)
        end
        ffichecks.checkcdata(2, targetPosition, "Vector")
        trajectoryModifier = ffichecks.checknumber(3, trajectoryModifier)
        ffichecks.checkcdata(4, params, "ProjectileParams")
        return ProjectileResults(repentogon.L_EntityNPC_FireBossProjectilesEx(self, numProjectiles, targetPosition, trajectoryModifier, params))
    end,
    FireGridEntity = function(self, sprite, desc, velocity, backdrop)
        ffichecks.checkcdata(1, sprite, "Sprite")
        ffichecks.checkcdata(2, desc, "GridEntityDesc")
        ffichecks.checkcdata(3, velocity, "Vector")
        if backdrop == nil then
            backdrop = repentogon.L_EntityNPC_GetBackdropId()
        else
            backdrop = ffichecks.checkinteger(4, backdrop)
        end
        local result = repentogon.L_EntityNPC_FireGridEntity(self, sprite, desc, velocity, math.min(backdrop, 1)) return result
    end,
    FireProjectiles = function(self, position, velocity, mode, params)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        mode = ffichecks.checkinteger(3, mode)
        if mode < 0 or mode > 9 then
            error(string.format("Invalid projectile mode %d\n", mode % 4294967296), 2)
        end
        ffichecks.checkcdata(4, params, "ProjectileParams")
        repentogon.L_EntityNPC_FireProjectiles(self, position, velocity, mode, params)
    end,
    FireProjectilesEx = function(self, position, velocity, mode, params)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        mode = ffichecks.checkinteger(3, mode)
        if mode < 0 or mode > 9 then
            error(string.format("Invalid projectile mode %d\n", mode % 4294967296), 2)
        end
        ffichecks.checkcdata(4, params, "ProjectileParams")
        return ProjectileResults(repentogon.L_EntityNPC_FireProjectilesEx(self, position, velocity, mode, params))
    end,
    GetAliveEnemyCount = function(self)
        local result = repentogon.L_EntityNPC_GetAliveEnemyCount(self) return result
    end,
    GetBossColorIdx = Getter("BossColorIdxValue"),
    GetChampionColorIdx = Getter("ChampionColorIdxValue"),
    GetControllerId = Getter("ControllerIdValue"),
    GetDarkRedChampionRegenTimer = Getter("ChampionRegenTimerValue"),
    GetDirtColor = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "DirtColorValue"))
    end,
    GetFireplaceLoot = function(self, shouldAdvance)
        local list = LootList()
        repentogon.L_EntityNPC_GetFireplaceLoot(self, ffichecks.optboolean(shouldAdvance, false), list)
        return list
    end,
    GetFlyingOverride = function(self)
        local override = repentogon.L_EntityNPC_GetFlyingOverride(self)
        if override < 0 then
            return nil
        end
        return override == 1
    end,
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityNPC_GetHitListSize(self) do
            result[i] = repentogon.L_EntityNPC_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetLootList = function(self, shouldAdvance)
        local list = LootList()
        repentogon.L_EntityNPC_GetLootList(self, ffichecks.optboolean(shouldAdvance, false), list)
        return list
    end,
    GetPathfinder = Getter("PathfinderValue"),
    GetPlayerTarget = function(self)
        local result = repentogon.L_EntityNPC_GetPlayerTarget(self) return result
    end,
    GetShieldStrength = Getter("ShieldStrengthValue"),
    GetShopkeeperLoot = function(self, shouldAdvance)
        local list = LootList()
        repentogon.L_EntityNPC_GetShopkeeperLoot(self, ffichecks.optboolean(shouldAdvance, false), list)
        return list
    end,
    GetSirenPlayerEntity = function(self)
        if ffi.getprivate(self, "TypeValue") == SIREN then
            return ffichecks.pointertoplayer(ffi.getprivate(self, "SirenPlayerValue"))
        end
        return nil
    end,
    IsBossColor = function(self)
        local result = repentogon.L_EntityNPC_IsBossColor(self) return result
    end,
    IsChampion = Getter("IsChampionValue"),
    KillUnique = function(self)
        repentogon.L_EntityNPC_KillUnique(self)
    end,
    MakeChampion = function(self, seed, championColorIdx, init)
        seed = ffichecks.checkinteger(1, seed)
        if championColorIdx == nil then
            championColorIdx = -1
        else
            championColorIdx = ffichecks.checkinteger(2, championColorIdx)
        end
        repentogon.L_EntityNPC_MakeChampion(self, seed, championColorIdx, not not init)
    end,
    MakeSplat = function(self, scale)
        scale = ffichecks.checknumber(1, scale)
        local result = repentogon.L_EntityNPC_MakeSplat(self, scale) return result
    end,
    Morph = function(self, entityType, variant, subType, championColorIdx)
        entityType = ffichecks.checkinteger(1, entityType)
        variant = ffichecks.checkinteger(2, variant)
        subType = ffichecks.checkinteger(3, subType)
        championColorIdx = ffichecks.checkinteger(4, championColorIdx)
        repentogon.L_EntityNPC_Morph(self, entityType, variant, subType, championColorIdx)
    end,
    PlaySound = function(self, id, volume, frameDelay, loop, pitch)
        id = ffichecks.checkinteger(1, id)
        repentogon.L_EntityNPC_PlaySound(self, id, ffichecks.optnumber(volume, 1.0), ffichecks.optnumber(frameDelay, 2),
            ffichecks.optboolean(loop, false), ffichecks.optnumber(pitch, 1.0))
    end,
    QueryNPCsGroup = function(self, groupIdx)
        groupIdx = ffichecks.checkinteger(1, groupIdx)
        return QueryResults(repentogon.L_EntityNPC_QueryNPCsGroup(self, groupIdx))
    end,
    QueryNPCsSpawnerType = function(self, entityType, variant, onlyEnemies)
        entityType = ffichecks.checkinteger(1, entityType)
        variant = ffichecks.checkinteger(2, variant)
        return QueryResults(repentogon.L_EntityNPC_QueryNPCsSpawnerType(self, entityType, variant, not not onlyEnemies))
    end,
    QueryNPCsType = function(self, entityType, variant)
        entityType = ffichecks.checkinteger(1, entityType)
        variant = ffichecks.checkinteger(2, variant)
        return QueryResults(repentogon.L_EntityNPC_QueryNPCsType(self, entityType, variant))
    end,
    ReplaceSpritesheet = function(self, layerId, spritesheet, loadGraphics)
        layerId = ffichecks.checkinteger(1, layerId)
        spritesheet = ffichecks.checkstring(2, spritesheet)
        local result = repentogon.L_EntityNPC_ReplaceSpritesheet(self, layerId, spritesheet, ffichecks.optboolean(loadGraphics, false)) return result
    end,
    ResetPathFinderTarget = function(self)
        repentogon.L_EntityNPC_ResetPathFinderTarget(self)
    end,
    SetControllerId = function(self, controllerId)
        controllerId = ffichecks.checknumber(1, controllerId)
        ffi.setprivate(self, "ControllerIdValue", controllerId)
    end,
    SetFlyingOverride = function(self, isFlying)
        isFlying = ffichecks.checkboolean(1, isFlying)
        repentogon.L_EntityNPC_SetFlyingOverride(self, isFlying)
    end,
    SetShieldStrength = function(self, strength)
        strength = ffichecks.checknumber(1, strength)
        ffi.setprivate(self, "ShieldStrengthValue", strength)
    end,
    SpawnBloodCloud = function(self, position, color)
        ffichecks.checkcdata(1, position, "Vector", true)
        ffichecks.checkcdata(2, color, "Color", true)
        local result = repentogon.L_EntityNPC_MakeBloodCloud(self, position, color) return result
    end,
    SpawnBloodSplash = function(self)
        repentogon.L_EntityNPC_MakeBloodSplash(self)
    end,
    TryForceTarget = function(self, target, duration)
        local pointer = EntityToPointer(target)
        if pointer == nil then
            ffichecks.argerror(1, "Entity expected, got " .. ffichecks.gettype(target), 3)
        end
        duration = ffichecks.checkinteger(2, duration)
        local result = repentogon.L_EntityNPC_TryForceTarget(self, pointer, duration) return result
    end,
    TrySplit = function(self, defaultDamage, source, doScreenEffects)
        defaultDamage = ffichecks.checknumber(1, defaultDamage)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_EntityNPC_TrySplit(self, defaultDamage, source, ffichecks.optboolean(doScreenEffects, true)) return result
    end,
    UpdateDirtColor = function(self, lerp)
        lerp = ffichecks.checkboolean(1, lerp)
        repentogon.L_EntityNPC_UpdateDirtColor(self, lerp)
    end,
    UpdatePickupGhosts = function(self)
        repentogon.L_EntityNPC_UpdatePickupGhosts(self)
    end,
}

-- Deprecated name
methods.GetPathFinder = methods.GetPathfinder

local NPCMT = Entity.Inherit("EntityNPC", methods, getters, setters)
ffi.metatype("struct EntityNPC", NPCMT)

ffichecks.pointertonpc = function(pointer)
    if pointer == nil then
        return nil
    end
    local result = ffi.cast(npcType, pointer) return result
end

EntityNPC = setmetatable({
    ShootMaggotProjectile = function(origin, target, velocity, yOffset)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(2, target, "Vector")
        local result = repentogon.L_EntityNPC_ShootMaggotProjectile(origin, target, ffichecks.optnumber(velocity, -8), ffichecks.optnumber(yOffset, -24)) return result
    end,
    ThrowLeech = function(origin, spawner, target, yPosOffset, big)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(3, target, "Vector")
        local result = repentogon.L_EntityNPC_ThrowLeech(origin, EntityToPointer(spawner), target, ffichecks.optnumber(yPosOffset, -10), ffichecks.optboolean(big, false)) return result
    end,
    ThrowMaggot = function(origin, target, yOffset, fallSpeed)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(2, target, "Vector")
        local result = repentogon.L_EntityNPC_ThrowMaggot(origin, target, ffichecks.optnumber(yOffset, -10), ffichecks.optnumber(fallSpeed, -8)) return result
    end,
    ThrowMaggotAtPos = function(origin, target, yOffset)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(2, target, "Vector")
        local result = repentogon.L_EntityNPC_ThrowMaggotAtPos(origin, target, ffichecks.optnumber(yOffset, -8)) return result
    end,
    ThrowRockSpider = function(origin, spawner, target, variant, yPosOffset)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(3, target, "Vector")
        local result = repentogon.L_EntityNPC_ThrowRockSpider(origin, EntityToPointer(spawner), target, ffichecks.optnumber(variant, 0), ffichecks.optnumber(yPosOffset, -10)) return result
    end,
    ThrowSpider = function(position, spawner, targetPosition, big, yOffset)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(3, targetPosition, "Vector")
        big = ffichecks.checkboolean(4, big)
        yOffset = ffichecks.checknumber(5, yOffset)
        local result = repentogon.L_EntityNPC_ThrowSpider(position, EntityToPointer(spawner), targetPosition, big, yOffset) return result
    end,
    ThrowStrider = function(origin, spawner, target)
        ffichecks.checkcdata(1, origin, "Vector")
        ffichecks.checkcdata(3, target, "Vector")
        local result = repentogon.L_EntityNPC_ThrowStrider(origin, EntityToPointer(spawner), target) return result
    end,
}, { __class = NPCMT })

return {
    Fields = NPC_FIELDS,
    Class = NPCMT,
}
