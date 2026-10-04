ffi.cdef [[
    struct Weapon {
        private float FireDelay : 0xc;
        private float MaxFireDelay : 0x10;
        private float Charge : 0x14;
        private struct Vector Direction : 0x20;
        private int WeaponType : 0x30;
        private uint32_t Modifiers : 0x34;
        private int NumFired : 0x38;
    } : 0x40;
    typedef struct Weapon* WeaponPtr;

    float L_Weapon_GetMaxCharge(struct Weapon*);
    void L_Weapon_PlayItemAnim(struct Weapon*, unsigned int, int, struct Vector*, float);
    bool L_Weapon_IsAxisAligned(struct Weapon*);
    bool L_Weapon_IsItemAnimFinished(struct Weapon*, unsigned int);
    void L_Weapon_ClearItemAnim(struct Weapon*, unsigned int);
    void L_Weapon_SetHeadLockTime(struct Weapon*, int);
]]

local repentogon = ffidll
local ffi = ffi

local cfuncs = {
    GetOwner = __Lua_Weapon_GetOwner,
    GetMainEntity = __Lua_Weapon_GetMainEntity,
}

local VectorT = ffi.typeof("struct Vector")

local WeaponMT
WeaponMT = {
    __type = "Weapon",
    ClearItemAnim = function(self, item)
        ffichecks.checkinteger(1, item)
        repentogon.L_Weapon_ClearItemAnim(self, item)
    end,
    GetCharge = function(self)
        return ffi.getprivate(self, "Charge")
    end,
    GetDirection = function(self)
        local direction = ffi.getprivate(self, "Direction")
        return Vector(direction.X, direction.Y)
    end,
    GetFireDelay = function(self)
        return ffi.getprivate(self, "FireDelay")
    end,
    GetMainEntity = function(self)
        return cfuncs.GetMainEntity(self)
    end,
    GetMaxCharge = function(self)
        return repentogon.L_Weapon_GetMaxCharge(self)
    end,
    GetMaxFireDelay = function(self)
        return ffi.getprivate(self, "MaxFireDelay")
    end,
    GetModifiers = function(self)
        return ffi.getprivate(self, "Modifiers")
    end,
    GetNumFired = function(self)
        return ffi.getprivate(self, "NumFired")
    end,
    GetOwner = function(self)
        return cfuncs.GetOwner(self)
    end,
    GetWeaponType = function(self)
        return ffi.getprivate(self, "WeaponType")
    end,
    IsAxisAligned = function(self)
        return repentogon.L_Weapon_IsAxisAligned(self)
    end,
    IsItemAnimFinished = function(self, item)
        ffichecks.checkinteger(1, item)
        return repentogon.L_Weapon_IsItemAnimFinished(self, item)
    end,
    PlayItemAnim = function(self, item, anim, position, charge)
        ffichecks.checkinteger(1, item)
        ffichecks.checkinteger(2, anim)
        ffichecks.checkcdata(3, position, "Vector")
        ffichecks.checknumber(4, charge)
        repentogon.L_Weapon_PlayItemAnim(self, item, anim, position, charge)
    end,
    SetCharge = function(self, charge)
        ffichecks.checknumber(1, charge)
        ffi.setprivate(self, "Charge", charge)
    end,
    SetFireDelay = function(self, delay)
        ffichecks.checknumber(1, delay)
        ffi.setprivate(self, "FireDelay", delay)
    end,
    SetHeadLockTime = function(self, time)
        ffichecks.checkinteger(1, time)
        repentogon.L_Weapon_SetHeadLockTime(self, time)
    end,
    SetModifiers = function(self, modifiers)
        ffichecks.checkinteger(1, modifiers)
        ffi.setprivate(self, "Modifiers", ffi.getprivate(self, "Modifiers") | modifiers)
    end,
}

setmetatable(WeaponMT, { __index = function() end })
WeaponMT.__index = WeaponMT

ffi.metatype("struct Weapon", WeaponMT)

Weapon = setmetatable({}, {
    __class = WeaponMT,
})

__Lua_Weapon_GetOwner = nil
__Lua_Weapon_GetMainEntity = nil
