ffi.cdef [[
    struct Weapon {
        private struct Entity* Owner : 0x4;
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
    struct Entity* L_Weapon_GetMainEntity(struct Weapon*);
]]

local repentogon = ffidll
local ffi = ffi

local VectorT = ffi.typeof("struct Vector")

local WeaponMT
WeaponMT = {
    __type = "Weapon",
    ClearItemAnim = function(self, item)
        item = ffichecks.checkinteger(1, item)
        repentogon.L_Weapon_ClearItemAnim(self, item)
    end,
    GetCharge = function(self)
        local result = ffi.getprivate(self, "Charge") return result
    end,
    GetDirection = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "Direction"))
    end,
    GetFireDelay = function(self)
        local result = ffi.getprivate(self, "FireDelay") return result
    end,
    GetMainEntity = function(self)
        local result = repentogon.L_Weapon_GetMainEntity(self) return result
    end,
    GetMaxCharge = function(self)
        local result = repentogon.L_Weapon_GetMaxCharge(self) return result
    end,
    GetMaxFireDelay = function(self)
        local result = ffi.getprivate(self, "MaxFireDelay") return result
    end,
    GetModifiers = function(self)
        local result = ffi.getprivate(self, "Modifiers") return result
    end,
    GetNumFired = function(self)
        local result = ffi.getprivate(self, "NumFired") return result
    end,
    GetOwner = function(self)
        local result = ffi.getprivate(self, "Owner") return result
    end,
    GetWeaponType = function(self)
        local result = ffi.getprivate(self, "WeaponType") return result
    end,
    IsAxisAligned = function(self)
        local result = repentogon.L_Weapon_IsAxisAligned(self) return result
    end,
    IsItemAnimFinished = function(self, item)
        item = ffichecks.checkinteger(1, item)
        local result = repentogon.L_Weapon_IsItemAnimFinished(self, item) return result
    end,
    PlayItemAnim = function(self, item, anim, position, charge)
        item = ffichecks.checkinteger(1, item)
        anim = ffichecks.checkinteger(2, anim)
        ffichecks.checkcdata(3, position, "Vector")
        charge = ffichecks.checknumber(4, charge)
        repentogon.L_Weapon_PlayItemAnim(self, item, anim, position, charge)
    end,
    SetCharge = function(self, charge)
        charge = ffichecks.checknumber(1, charge)
        ffi.setprivate(self, "Charge", charge)
    end,
    SetFireDelay = function(self, delay)
        delay = ffichecks.checknumber(1, delay)
        ffi.setprivate(self, "FireDelay", delay)
    end,
    SetHeadLockTime = function(self, time)
        time = ffichecks.checkinteger(1, time)
        repentogon.L_Weapon_SetHeadLockTime(self, time)
    end,
    SetModifiers = function(self, modifiers)
        modifiers = ffichecks.checkinteger(1, modifiers)
        ffi.setprivate(self, "Modifiers", ffi.getprivate(self, "Modifiers") | modifiers)
    end,
}

setmetatable(WeaponMT, { __index = function() end })
WeaponMT.__index = WeaponMT

ffi.metatype("struct Weapon", WeaponMT)

Weapon = setmetatable({}, {
    __class = WeaponMT,
})

