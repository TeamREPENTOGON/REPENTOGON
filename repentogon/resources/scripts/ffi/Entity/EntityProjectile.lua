local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityProjectile { " .. Entity.Fields .. [[
    float Height : 0x410;
    float FallingSpeed : 0x414;
    float FallingAccel : 0x418;
    float Damage : 0x41c;
    float Scale : 0x420;
    float HomingStrength : 0x424;
    float CurvingStrength : 0x428;
    float Acceleration : 0x42c;
    private uint64_t ProjectileFlagsValue : 0x438;
    int WiggleFrameOffset : 0x448;
    private uint64_t ChangeFlagsValue : 0x450;
    float ChangeVelocity : 0x458;
    int ChangeTimeout : 0x45c;
} : 0x470;
typedef struct EntityProjectile* EntityProjectilePtr;
]])

ffi.cdef [[
    void L_EntityProjectile_Deflect(struct EntityProjectile*, struct Vector*);
]]

local ffi = ffi
local repentogon = ffidll

local MAX_EXACT = 9007199254740992LL
local function Flags64(value)
    local signed = ffi.cast("int64_t", value)
    if signed >= -MAX_EXACT and signed <= MAX_EXACT then
        return tonumber(signed)
    end
    return signed
end

local function CheckFlags64(index, value)
    if ffichecks.isnumber(value) or ffi.istype("int64_t", value) or ffi.istype("uint64_t", value) then
        return value
    end
    ffichecks.argerror(index, "integer expected, got " .. ffichecks.gettype(value), 3)
end

local function FlagsSetter(field)
    return function(self, value)
        value = CheckFlags64(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function Adder(field)
    return function(self, amount)
        ffichecks.checknumber(1, amount)
        self[field] = self[field] + amount
    end
end

local function FlagsAdder(field)
    return function(self, flags)
        flags = CheckFlags64(1, flags)
        ffi.setprivate(self, field, ffi.getprivate(self, field) | flags)
    end
end

local getters = {
    ProjectileFlags = function(self)
        return Flags64(ffi.getprivate(self, "ProjectileFlagsValue"))
    end,
    ChangeFlags = function(self)
        return Flags64(ffi.getprivate(self, "ChangeFlagsValue"))
    end,
}

local setters = {
    ProjectileFlags = FlagsSetter("ProjectileFlagsValue"),
    ChangeFlags = FlagsSetter("ChangeFlagsValue"),
}

local methods = {
    AddChangeFlags = FlagsAdder("ChangeFlagsValue"),
    AddFallingAccel = Adder("FallingAccel"),
    AddFallingSpeed = Adder("FallingSpeed"),
    AddHeight = Adder("Height"),
    AddProjectileFlags = FlagsAdder("ProjectileFlagsValue"),
    AddScale = Adder("Scale"),
    ClearProjectileFlags = function(self, flags)
        flags = CheckFlags64(1, flags)
        ffi.setprivate(self, "ProjectileFlagsValue", ffi.getprivate(self, "ProjectileFlagsValue") & ~ffi.cast("uint64_t", flags))
    end,
    Deflect = function(self, velocity)
        ffichecks.checkcdata(1, velocity, "Vector")
        repentogon.L_EntityProjectile_Deflect(self, velocity)
    end,
    HasProjectileFlags = function(self, flags)
        flags = CheckFlags64(1, flags)
        return (ffi.getprivate(self, "ProjectileFlagsValue") & ffi.cast("uint64_t", flags)) ~= 0
    end,
}

local ProjectileMT = Entity.Inherit("EntityProjectile", methods, getters, setters)
ffi.metatype("struct EntityProjectile", ProjectileMT)
Entity.SetProjectileType(ffi.typeof("struct EntityProjectile*"))

EntityProjectile = setmetatable({}, { __class = ProjectileMT })
