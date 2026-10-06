local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityKnife { " .. Entity.Fields .. [[
    private struct BitSet128 TearFlagsValue : 0x410;
    float Scale : 0x430;
    float Rotation : 0x438;
    float RotationOffset : 0x43c;
    private float KnifeDistanceValue : 0x444;
    private float KnifeVelocityValue : 0x448;
    float MaxDistance : 0x45c;
    float Charge : 0x460;
    private bool IsFlyingValue : 0x464;
    private bool IsSwingingValue : 0x465;
    float PathOffset : 0x4c4;
    float PathFollowSpeed : 0x4c8;
    private bool PrismAppliedValue : 0x4e5;
    private bool MultidimensionalAppliedValue : 0x4e6;
    private bool IsSpinAttackValue : 0x4ea;
} : 0xe78;
typedef struct EntityKnife* EntityKnifePtr;
]])

ffi.cdef [[
    void L_EntityKnife_Shoot(struct EntityKnife*, float, float);
    void L_EntityKnife_Reset(struct EntityKnife*);
    int L_EntityKnife_GetRenderZ(struct EntityKnife*);
    unsigned int L_EntityKnife_GetHitListSize(struct EntityKnife*);
    unsigned int L_EntityKnife_GetHitListEntry(struct EntityKnife*, unsigned int);
    void L_EntityKnife_RemoveFromHitList(struct EntityKnife*, void*);
    void L_EntityKnife_AddToHitList(struct EntityKnife*, void*);
    bool L_EntityKnife_InHitList(struct EntityKnife*, void*);
    void L_EntityKnife_InitHomingPath(struct EntityKnife*, struct Vector*, void*);
    struct EntityKnife* L_EntityKnife_GetHitboxParentKnife(struct EntityKnife*);
    void L_EntityKnife_SetHitboxParentKnife(struct EntityKnife*, struct EntityKnife*);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local BooleanSetter = helpers.BooleanSetter
local CopyStruct = helpers.CopyStruct
local EntityToPointer = ffichecks.entitytopointer

local TYPE_KNIFE = 8

local function CheckEntity(index, value)
    local pointer = EntityToPointer(value)
    if pointer == nil then
        ffichecks.argerror(index, "Entity expected, got " .. ffichecks.gettype(value), 3)
    end
    return pointer
end

local function NumberSetter(field)
    return function(self, value)
        ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local getters = {
    TearFlags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlagsValue"))
    end,
}

local setters = {
    TearFlags = function(self, value)
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "TearFlagsValue", value)
    end,
}

local methods = {
    AddTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags | flags
    end,
    AddToHitList = function(self, entity)
        repentogon.L_EntityKnife_AddToHitList(self, CheckEntity(1, entity))
    end,
    ClearTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags & ~flags
    end,
    FireSplitTear = Entity.Helpers.FireSplitTear,
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityKnife_GetHitListSize(self) do
            result[i] = repentogon.L_EntityKnife_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetHitboxParentKnife = function(self)
        return repentogon.L_EntityKnife_GetHitboxParentKnife(self)
    end,
    GetIsSpinAttack = Getter("IsSpinAttackValue"),
    GetIsSwinging = Getter("IsSwingingValue"),
    GetKnifeDistance = Getter("KnifeDistanceValue"),
    GetKnifeVelocity = Getter("KnifeVelocityValue"),
    GetRenderZ = function(self)
        return repentogon.L_EntityKnife_GetRenderZ(self)
    end,
    HasTearFlags = function(self, flags)
        return (self.TearFlags & flags) ~= TearFlags.TEAR_NORMAL
    end,
    InHitList = function(self, entity)
        return repentogon.L_EntityKnife_InHitList(self, CheckEntity(1, entity))
    end,
    InitHomingPath = function(self, direction, source)
        ffichecks.checkcdata(1, direction, "Vector")
        repentogon.L_EntityKnife_InitHomingPath(self, direction, EntityToPointer(source))
    end,
    IsFlying = Getter("IsFlyingValue"),
    IsMultidimensionalTouched = Getter("MultidimensionalAppliedValue"),
    IsPrismTouched = Getter("PrismAppliedValue"),
    RemoveFromHitList = function(self, entity)
        repentogon.L_EntityKnife_RemoveFromHitList(self, CheckEntity(1, entity))
    end,
    Reset = function(self)
        repentogon.L_EntityKnife_Reset(self)
    end,
    SetHitboxParentKnife = function(self, parent)
        ffichecks.checkcdata(1, parent, "EntityKnife", true)
        repentogon.L_EntityKnife_SetHitboxParentKnife(self, parent)
    end,
    SetIsSpinAttack = BooleanSetter("IsSpinAttackValue"),
    SetIsSwinging = BooleanSetter("IsSwingingValue"),
    SetKnifeDistance = NumberSetter("KnifeDistanceValue"),
    SetKnifeVelocity = NumberSetter("KnifeVelocityValue"),
    SetMultidimensionalTouched = BooleanSetter("MultidimensionalAppliedValue"),
    SetPathFollowSpeed = function(self, speed)
        ffichecks.checknumber(1, speed)
        self.PathFollowSpeed = speed
    end,
    SetPrismTouched = BooleanSetter("PrismAppliedValue"),
    Shoot = function(self, percent, range)
        ffichecks.checknumber(1, percent)
        ffichecks.checknumber(2, range)
        repentogon.L_EntityKnife_Shoot(self, percent, range)
    end,
}

local KnifeMT = Entity.Inherit("EntityKnife", methods, getters, setters)
ffi.metatype("struct EntityKnife", KnifeMT)
Entity.SetClassType(TYPE_KNIFE, ffi.typeof("struct EntityKnife*"))

EntityKnife = setmetatable({}, { __class = KnifeMT })
