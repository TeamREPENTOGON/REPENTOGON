local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityFamiliar { " .. Entity.Fields .. [[
    private struct Entity* PlayerValue : 0x410;
    private int MoveDelayNumValue : 0x5f8;
    int FireCooldown : 0xd4c;
    int HeadFrameDelay : 0xd50;
    int MoveDirection : 0xd54;
    int ShootDirection : 0xd58;
    int LastDirection : 0xd5c;
    float OrbitAngleOffset : 0xd60;
    struct Vector OrbitDistance : 0xd64;
    float OrbitSpeed : 0xd6c;
    int OrbitLayer : 0xd70;
    private bool IsFollowerValue : 0xd74;
    private bool IsDelayedValue : 0xd75;
    int Coins : 0xd7c;
    int Hearts : 0xd80;
    int Keys : 0xd84;
    int State : 0xd88;
    int RoomClearCount : 0xd8c;
    private struct Weapon* WeaponValue : 0xdb0;
    private bool IsLilDeliriumValue : 0xdb4;
    private struct ItemConfigItem* ItemValue : 0xed4;
    private struct Color DirtColorValue : 0xedc;
} : 0xf30;
typedef struct EntityFamiliar* EntityFamiliarPtr;
]])

ffi.cdef [[
    void L_EntityFamiliar_SetPlayer(struct EntityFamiliar*, void*);
    void L_EntityFamiliar_AddCoins(struct EntityFamiliar*, int);
    void L_EntityFamiliar_FollowParent(struct EntityFamiliar*);
    void L_EntityFamiliar_FollowPosition(struct EntityFamiliar*, struct Vector*);
    void L_EntityFamiliar_Shoot(struct EntityFamiliar*);
    void L_EntityFamiliar_PlayChargeAnim(struct EntityFamiliar*, int);
    void L_EntityFamiliar_PlayShootAnim(struct EntityFamiliar*, int);
    void L_EntityFamiliar_PlayFloatAnim(struct EntityFamiliar*, int);
    void L_EntityFamiliar_MoveDelayed(struct EntityFamiliar*, int);
    void L_EntityFamiliar_MoveDiagonally(struct EntityFamiliar*, float);
    int L_EntityFamiliar_RecalculateOrbitOffset(struct EntityFamiliar*, int, bool);
    void L_EntityFamiliar_AddToFollowers(struct EntityFamiliar*);
    void L_EntityFamiliar_AddToDelayed(struct EntityFamiliar*);
    void L_EntityFamiliar_AddToOrbit(struct EntityFamiliar*, int);
    void L_EntityFamiliar_RemoveFromFollowers(struct EntityFamiliar*);
    void L_EntityFamiliar_RemoveFromDelayed(struct EntityFamiliar*);
    void L_EntityFamiliar_RemoveFromOrbit(struct EntityFamiliar*);
    void L_EntityFamiliar_GetOrbitDistance(int, struct Vector*);
    void L_EntityFamiliar_GetOrbitPosition(struct EntityFamiliar*, struct Vector*, struct Vector*);
    struct EntityTear* L_EntityFamiliar_FireProjectile(struct EntityFamiliar*, struct Vector*);
    void L_EntityFamiliar_PickEnemyTarget(struct EntityFamiliar*, float, int, int, struct Vector*, float);
    int L_EntityFamiliar_GetFollowerPriority(struct EntityFamiliar*);
    struct PathFinder* L_EntityFamiliar_GetPathfinder(struct EntityFamiliar*);
    bool L_EntityFamiliar_TryAimAtMarkedTarget(struct EntityFamiliar*, struct Vector*, int*, struct Vector*);
    void L_EntityFamiliar_TriggerRoomClear(struct EntityFamiliar*);
    void L_EntityFamiliar_UpdateDirtColor(struct EntityFamiliar*);
    void L_EntityFamiliar_RemoveFromPlayer(struct EntityFamiliar*);
    bool L_EntityFamiliar_CanCharm(struct EntityFamiliar*);
    bool L_EntityFamiliar_IsCharmed(struct EntityFamiliar*);
    bool L_EntityFamiliar_CanBeDamagedByEnemies(struct EntityFamiliar*);
    bool L_EntityFamiliar_CanBeDamagedByProjectiles(struct EntityFamiliar*);
    bool L_EntityFamiliar_CanBeDamagedByLasers(struct EntityFamiliar*);
    bool L_EntityFamiliar_CanBlockProjectiles(struct EntityFamiliar*);
    float L_EntityFamiliar_GetMultiplier(struct EntityFamiliar*);
    void L_EntityFamiliar_InvalidateCachedMultiplier(struct EntityFamiliar*);
    void L_EntityFamiliar_SetLilDelirium(struct EntityFamiliar*, bool);
    int L_EntityFamiliar_GetRandomWisp(struct RNG*);
    struct Entity* L_EntityFamiliar_GetActiveWeaponEntity(struct EntityFamiliar*);
    int L_EntityFamiliar_GetActiveWeaponNumFired(struct EntityFamiliar*);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local CopyStruct = helpers.CopyStruct

local TYPE_FAMILIAR = 3

local function IntegerMethod(export)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        export(self, value)
    end
end

local function VoidMethod(export)
    return function(self)
        export(self)
    end
end

local function BoolMethod(export)
    return function(self)
        return export(self)
    end
end

local function GetRandomWisp(rng)
    ffichecks.checkcdata(1, rng, "RNG")
    local result = repentogon.L_EntityFamiliar_GetRandomWisp(rng) return result
end

local function GetPathfinder(self)
    local result = repentogon.L_EntityFamiliar_GetPathfinder(self) return result
end

local getters = {
    Player = function(self)
        return ffichecks.pointertoplayer(ffi.getprivate(self, "PlayerValue"))
    end,
    IsDelayed = Getter("IsDelayedValue"),
    IsFollower = Getter("IsFollowerValue"),
}

local setters = {
    Player = function(self, value)
        repentogon.L_EntityFamiliar_SetPlayer(self, ffichecks.playertopointer(value))
    end,
    IsDelayed = function(self, value)
        ffi.setprivate(self, "IsDelayedValue", not not value)
    end,
    IsFollower = function(self, value)
        ffi.setprivate(self, "IsFollowerValue", not not value)
    end,
}

local methods = {
    AddCoins = IntegerMethod(repentogon.L_EntityFamiliar_AddCoins),
    AddHearts = function(self, hearts)
        hearts = ffichecks.checkinteger(1, hearts)
        self.Hearts = self.Hearts + hearts
    end,
    AddKeys = function(self, keys)
        keys = ffichecks.checkinteger(1, keys)
        self.Keys = self.Keys + keys
    end,
    AddToDelayed = VoidMethod(repentogon.L_EntityFamiliar_AddToDelayed),
    AddToFollowers = VoidMethod(repentogon.L_EntityFamiliar_AddToFollowers),
    AddToOrbit = IntegerMethod(repentogon.L_EntityFamiliar_AddToOrbit),
    CanBeDamagedByEnemies = BoolMethod(repentogon.L_EntityFamiliar_CanBeDamagedByEnemies),
    CanBeDamagedByLasers = BoolMethod(repentogon.L_EntityFamiliar_CanBeDamagedByLasers),
    CanBeDamagedByProjectiles = BoolMethod(repentogon.L_EntityFamiliar_CanBeDamagedByProjectiles),
    CanBlockProjectiles = BoolMethod(repentogon.L_EntityFamiliar_CanBlockProjectiles),
    CanCharm = BoolMethod(repentogon.L_EntityFamiliar_CanCharm),
    FireProjectile = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        local result = repentogon.L_EntityFamiliar_FireProjectile(self, direction) return result
    end,
    FollowParent = VoidMethod(repentogon.L_EntityFamiliar_FollowParent),
    FollowPosition = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_EntityFamiliar_FollowPosition(self, position)
    end,
    GetActiveWeaponEntity = function(self)
        local result = repentogon.L_EntityFamiliar_GetActiveWeaponEntity(self) return result
    end,
    GetActiveWeaponNumFired = function(self)
        if ffi.getprivate(self, "WeaponValue") == nil then
            return nil
        end
        local result = repentogon.L_EntityFamiliar_GetActiveWeaponNumFired(self) return result
    end,
    GetDirtColor = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "DirtColorValue"))
    end,
    GetFollowerPriority = function(self)
        local result = repentogon.L_EntityFamiliar_GetFollowerPriority(self) return result
    end,
    GetItemConfig = Getter("ItemValue"),
    GetMoveDelayNum = Getter("MoveDelayNumValue"),
    GetMultiplier = function(self)
        local result = repentogon.L_EntityFamiliar_GetMultiplier(self) return result
    end,
    GetOrbitPosition = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        local result = Vector(0, 0)
        repentogon.L_EntityFamiliar_GetOrbitPosition(self, offset, result)
        return result
    end,
    GetPathFinder = GetPathfinder, -- deprecated
    GetPathfinder = GetPathfinder,
    GetRandomWisp = function(self, rng)
        return GetRandomWisp(rng)
    end,
    GetWeapon = Getter("WeaponValue"),
    InvalidateCachedMultiplier = VoidMethod(repentogon.L_EntityFamiliar_InvalidateCachedMultiplier),
    IsCharmed = BoolMethod(repentogon.L_EntityFamiliar_IsCharmed),
    IsLilDelirium = Getter("IsLilDeliriumValue"),
    MoveDelayed = IntegerMethod(repentogon.L_EntityFamiliar_MoveDelayed),
    MoveDiagonally = function(self, speed)
        speed = ffichecks.checknumber(1, speed)
        repentogon.L_EntityFamiliar_MoveDiagonally(self, speed)
    end,
    PickEnemyTarget = function(self, maxDistance, frameInterval, flags, coneDirection, coneAngle)
        maxDistance = ffichecks.checknumber(1, maxDistance)
        frameInterval = frameInterval == nil and 13 or frameInterval
        flags = flags == nil and 0 or flags
        frameInterval = ffichecks.checkinteger(2, frameInterval)
        flags = ffichecks.checkinteger(3, flags)
        if type(coneDirection) == "cdata" then
            ffichecks.checkcdata(4, coneDirection, "Vector")
        else
            coneDirection = Vector(0, 0)
        end
        repentogon.L_EntityFamiliar_PickEnemyTarget(self, maxDistance, frameInterval, flags, coneDirection, ffichecks.optnumber(coneAngle, 15))
    end,
    PlayChargeAnim = IntegerMethod(repentogon.L_EntityFamiliar_PlayChargeAnim),
    PlayFloatAnim = IntegerMethod(repentogon.L_EntityFamiliar_PlayFloatAnim),
    PlayShootAnim = IntegerMethod(repentogon.L_EntityFamiliar_PlayShootAnim),
    RecalculateOrbitOffset = function(self, layer, add)
        layer = ffichecks.checkinteger(1, layer)
        local result = repentogon.L_EntityFamiliar_RecalculateOrbitOffset(self, layer, not not add) return result
    end,
    RemoveFromDelayed = VoidMethod(repentogon.L_EntityFamiliar_RemoveFromDelayed),
    RemoveFromFollowers = VoidMethod(repentogon.L_EntityFamiliar_RemoveFromFollowers),
    RemoveFromOrbit = VoidMethod(repentogon.L_EntityFamiliar_RemoveFromOrbit),
    RemoveFromPlayer = VoidMethod(repentogon.L_EntityFamiliar_RemoveFromPlayer),
    SetLilDelirium = function(self, isLilDelirium)
        repentogon.L_EntityFamiliar_SetLilDelirium(self, not not isLilDelirium)
    end,
    SetMoveDelayNum = function(self, frames)
        frames = ffichecks.checkinteger(1, frames)
        ffi.setprivate(self, "MoveDelayNumValue", frames)
    end,
    Shoot = VoidMethod(repentogon.L_EntityFamiliar_Shoot),
    TryAimAtMarkedTarget = function(self, ...)
        local legacyOverload = select("#", ...) == 2
        local aimDirection, direction = ...
        ffichecks.checkcdata(1, aimDirection, "Vector", true)
        if direction == nil then
            direction = -1
        else
            direction = ffichecks.checkinteger(2, direction)
        end

        local aim = aimDirection ~= nil and Vector(aimDirection.X, aimDirection.Y) or Vector(0, 0)
        local directionBuffer = ffi.new("int[1]", direction)
        local targetPosition = Vector(0, 0)
        local success = repentogon.L_EntityFamiliar_TryAimAtMarkedTarget(self, aim, directionBuffer, targetPosition)

        if legacyOverload then
            if success then
                return targetPosition
            end
            return nil
        end
        return success, { aim, directionBuffer[0], targetPosition }
    end,
    TriggerRoomClear = VoidMethod(repentogon.L_EntityFamiliar_TriggerRoomClear),
    UpdateDirtColor = VoidMethod(repentogon.L_EntityFamiliar_UpdateDirtColor),
}

local FamiliarMT = Entity.Inherit("EntityFamiliar", methods, getters, setters)
ffi.metatype("struct EntityFamiliar", FamiliarMT)
Entity.SetClassType(TYPE_FAMILIAR, ffi.typeof("struct EntityFamiliar*"))

EntityFamiliar = setmetatable({
    GetOrbitDistance = function(layer)
        layer = ffichecks.checkinteger(1, layer)
        local result = Vector(0, 0)
        repentogon.L_EntityFamiliar_GetOrbitDistance(layer, result)
        return result
    end,
    GetRandomWisp = GetRandomWisp,
}, { __class = FamiliarMT })
