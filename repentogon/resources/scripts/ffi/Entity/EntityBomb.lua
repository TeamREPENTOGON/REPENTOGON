local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityBomb { " .. Entity.Fields .. [[
    private int ExplosionCountdownValue : 0x410;
    private int ExplosionCountdownCopyValue : 0x414;
    float ExplosionDamage : 0x418;
    private float FallSpeedValue : 0x41c;
    private float FallAccelerationValue : 0x420;
    private float RocketAngleValue : 0x428;
    private float RocketSpeedValue : 0x42c;
    private float ScaleValue : 0x434;
    private struct BitSet128 FlagsValue : 0x438;
    private bool IsFetusValue : 0x448;
    float RadiusMultiplier : 0x44c;
    private bool PrismTouchedValue : 0x45c;
    private bool LoadCostumesValue : 0x463;
} : 0x9d8;
typedef struct EntityBomb* EntityBombPtr;
]])

ffi.cdef [[
    void L_EntityBomb_UpdateDirtColor(struct EntityBomb*);
    struct Sprite* L_EntityBomb_GetCostumeLayerSprite(struct EntityBomb*, int);
    unsigned int L_EntityBomb_GetHitListSize(struct EntityBomb*);
    unsigned int L_EntityBomb_GetHitListEntry(struct EntityBomb*, unsigned int);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local BooleanSetter = helpers.BooleanSetter
local CopyStruct = helpers.CopyStruct

local TYPE_BOMB = 4

local function NumberSetter(field)
    return function(self, value)
        ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function Deprecated(message, field)
    local printed = false
    local function warn()
        if not printed then
            Console.PrintWarning(message)
            printed = true
        end
    end
    return function(self)
        warn()
        return ffi.getprivate(self, field)
    end, function(self, value)
        warn()
        ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local getters = {
    Flags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "FlagsValue"))
    end,
    IsFetus = Getter("IsFetusValue"),
}

local setters = {
    Flags = function(self, value)
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "LoadCostumesValue", true) -- nicalis brand lunacy
        ffi.setprivate(self, "FlagsValue", value)
    end,
    IsFetus = function(self, value)
        ffi.setprivate(self, "IsFetusValue", not not value)
    end,
}

local getHeight, setHeight = Deprecated("[WARN] EntityBomb:Get/SetHeight is deprecated - It was mislabeled and actually accesses the FallSpeed. For bomb \"height\", use `PositionOffset.Y`. For falling speed, use EntityBomb:Get/SetFallSpeed.", "FallSpeedValue")
local getFallingSpeed, setFallingSpeed = Deprecated("[WARN] EntityBomb:Get/SetFallingSpeed is deprecated - It was mislabeled and actually accesses the FallAcceleration. Please use EntityBomb:Get/SetFallSpeed or EntityBomb:Get/SetFallAcceleration instead.", "FallAccelerationValue")

local methods = {
    AddTearFlags = function(self, flags)
        self.Flags = self.Flags | flags
    end,
    ClearTearFlags = function(self, flags)
        self.Flags = self.Flags & ~flags
    end,
    GetCostumeLayerSprite = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 0 or index >= 5 then
            error(string.format("Invalid index %d, value must be between 0 and 4", index), 2)
        end
        return repentogon.L_EntityBomb_GetCostumeLayerSprite(self, index)
    end,
    GetExplosionCountdown = Getter("ExplosionCountdownValue"),
    GetFallAcceleration = Getter("FallAccelerationValue"),
    GetFallSpeed = Getter("FallSpeedValue"),
    GetFallingSpeed = getFallingSpeed,
    GetHeight = getHeight,
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityBomb_GetHitListSize(self) do
            result[i] = repentogon.L_EntityBomb_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetRocketAngle = Getter("RocketAngleValue"),
    GetRocketSpeed = Getter("RocketSpeedValue"),
    GetScale = Getter("ScaleValue"),
    HasTearFlags = function(self, flags)
        return (self.Flags & flags) ~= TearFlags.TEAR_NORMAL
    end,
    IsLoadingCostumes = Getter("LoadCostumesValue"),
    IsPrismTouched = Getter("PrismTouchedValue"),
    SetExplosionCountdown = function(self, countdown)
        ffichecks.checkinteger(1, countdown)
        ffi.setprivate(self, "ExplosionCountdownValue", countdown)
        ffi.setprivate(self, "ExplosionCountdownCopyValue", countdown)
    end,
    SetFallAcceleration = NumberSetter("FallAccelerationValue"),
    SetFallSpeed = NumberSetter("FallSpeedValue"),
    SetFallingSpeed = setFallingSpeed,
    SetHeight = setHeight,
    SetLoadCostumes = function(self, load)
        ffi.setprivate(self, "LoadCostumesValue", ffichecks.optboolean(load, true))
    end,
    SetPrismTouched = BooleanSetter("PrismTouchedValue"),
    SetRocketAngle = NumberSetter("RocketAngleValue"),
    SetRocketSpeed = NumberSetter("RocketSpeedValue"),
    SetScale = NumberSetter("ScaleValue"),
    UpdateDirtColor = function(self)
        repentogon.L_EntityBomb_UpdateDirtColor(self)
    end,
}

local BombMT = Entity.Inherit("EntityBomb", methods, getters, setters)
ffi.metatype("struct EntityBomb", BombMT)
Entity.SetClassType(TYPE_BOMB, ffi.typeof("struct EntityBomb*"))

EntityBomb = setmetatable({}, { __class = BombMT })
