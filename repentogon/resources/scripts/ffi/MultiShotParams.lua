ffi.cdef [[
    struct MultiShotParams {
        private int16_t NumTears : 0x0;
        private int16_t NumLanesPerEye : 0x2;
        private float SpreadAngleTears : 0x4;
        private float SpreadAngleLaser : 0x8;
        private float SpreadAngleTechX : 0xc;
        private float SpreadAngleKnife : 0x10;
        private int NumEyesActive : 0x14;
        private float MultiEyeAngle : 0x18;
        private bool CrossEyed : 0x1c;
        private bool ShootingBackwards : 0x1d;
        private bool ShootingSideways : 0x1e;
        private int16_t NumRandomDirTears : 0x20;
    } : 0x24;
    typedef struct MultiShotParams* MultiShotParamsPtr;
]]

local ffi = ffi

local spreadAngleFields = {
    [1] = "SpreadAngleTears",  -- WEAPON_TEARS
    [5] = "SpreadAngleTears",  -- WEAPON_BOMBS
    [2] = "SpreadAngleLaser",  -- WEAPON_BRIMSTONE
    [3] = "SpreadAngleLaser",  -- WEAPON_LASER
    [4] = "SpreadAngleKnife",  -- WEAPON_KNIFE
    [9] = "SpreadAngleTechX",  -- WEAPON_TECH_X
    [14] = "SpreadAngleTechX", -- WEAPON_FETUS (C Section)
    [10] = "SpreadAngleKnife", -- WEAPON_BONE
    [11] = "SpreadAngleKnife", -- WEAPON_NOTCHED_AXE
}
local spreadAngleScale = { [10] = 3, [11] = 3 }

local MultiShotParamsMT
MultiShotParamsMT = {
    __type = "MultiShotParams",
    GetMultiEyeAngle = function(self)
        local result = ffi.getprivate(self, "MultiEyeAngle") return result
    end,
    GetNumEyesActive = function(self)
        local result = ffi.getprivate(self, "NumEyesActive") return result
    end,
    GetNumLanesPerEye = function(self)
        local result = ffi.getprivate(self, "NumLanesPerEye") return result
    end,
    GetNumRandomDirTears = function(self)
        local result = ffi.getprivate(self, "NumRandomDirTears") return result
    end,
    GetNumTears = function(self)
        local result = ffi.getprivate(self, "NumTears") return result
    end,
    GetSpreadAngle = function(self, weaponType)
        weaponType = ffichecks.checkinteger(1, weaponType)
        local field = spreadAngleFields[weaponType]
        if field then
            return ffi.getprivate(self, field) * (spreadAngleScale[weaponType] or 1)
        end
        if weaponType >= 1 and weaponType <= 15 then
            return 0
        end
        ffichecks.argerror(1, "WeaponTypes bigger than 15 are not supported")
    end,
    IsCrossEyed = function(self)
        local result = ffi.getprivate(self, "CrossEyed") return result
    end,
    IsShootingBackwards = function(self)
        local result = ffi.getprivate(self, "ShootingBackwards") return result
    end,
    IsShootingSideways = function(self)
        local result = ffi.getprivate(self, "ShootingSideways") return result
    end,
    SetIsCrossEyed = function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "CrossEyed", value)
    end,
    SetIsShootingBackwards = function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "ShootingBackwards", value)
    end,
    SetIsShootingSideways = function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "ShootingSideways", value)
    end,
    SetMultiEyeAngle = function(self, angle)
        angle = ffichecks.checknumber(1, angle)
        ffi.setprivate(self, "MultiEyeAngle", angle)
    end,
    SetNumEyesActive = function(self, count)
        count = ffichecks.checkinteger(1, count)
        ffi.setprivate(self, "NumEyesActive", count)
    end,
    SetNumLanesPerEye = function(self, count)
        count = ffichecks.checkinteger(1, count)
        ffi.setprivate(self, "NumLanesPerEye", count)
    end,
    SetNumRandomDirTears = function(self, count)
        count = ffichecks.checkinteger(1, count)
        ffi.setprivate(self, "NumRandomDirTears", count)
    end,
    SetNumTears = function(self, count)
        count = ffichecks.checkinteger(1, count)
        ffi.setprivate(self, "NumTears", count)
    end,
    SetSpreadAngle = function(self, weaponType, angle)
        weaponType = ffichecks.checkinteger(1, weaponType)
        angle = ffichecks.checknumber(2, angle)
        local field = spreadAngleFields[weaponType]
        if not field then
            if weaponType >= 1 and weaponType <= 15 then
                ffichecks.argerror(1, "the given WeaponType can't change its spread angle")
            end
            ffichecks.argerror(1, "WeaponTypes bigger than 15 are not supported")
        end
        ffi.setprivate(self, field, angle / (spreadAngleScale[weaponType] or 1))
        if ffi.getprivate(self, "NumLanesPerEye") < 2 then
            ffi.setprivate(self, "NumLanesPerEye", 2)
        end
    end,
}

setmetatable(MultiShotParamsMT, { __index = function() end })
MultiShotParamsMT.__index = MultiShotParamsMT

ffi.metatype("struct MultiShotParams", MultiShotParamsMT)

MultiShotParams = setmetatable({}, {
    __class = MultiShotParamsMT,
})
