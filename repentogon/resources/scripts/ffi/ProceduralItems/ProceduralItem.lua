ffi.cdef [[
    struct ProceduralItem {
        private struct ItemConfigItem* Item : 0x0;
        private int ID : 0x4;
        private struct ItemConfigItem* TargetItem : 0x10;
        private float Damage : 0x14;
        private float FireDelay : 0x18;
        private float Speed : 0x1c;
        private float Range : 0x20;
        private float ShotSpeed : 0x24;
        private float Luck : 0x28;
        private struct ProceduralEffect** EffectsFirst : 0x2c;
        private struct ProceduralEffect** EffectsLast : 0x30;
    } : 0x38;
    typedef struct ProceduralItem* ProceduralItemPtr;
]]

local ffi = ffi

local function GetEffectCount(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "EffectsFirst"), ffi.getprivate(self, "EffectsLast"), ffi.sizeof("void*"))
end

local ProceduralItemMT
ProceduralItemMT = {
    __type = "ProceduralItem",
    GetDamage = function(self)
        return ffi.getprivate(self, "Damage")
    end,
    GetEffect = function(self, index)
        ffichecks.checkinteger(1, index)
        if index >= 0 and index < GetEffectCount(self) then
            return ffi.getprivate(self, "EffectsFirst")[index]
        end
        return nil
    end,
    GetEffectCount = GetEffectCount,
    GetFireDelay = function(self)
        return ffi.getprivate(self, "FireDelay")
    end,
    GetID = function(self)
        return ffi.getprivate(self, "ID")
    end,
    GetItem = function(self)
        return ffi.getprivate(self, "Item")
    end,
    GetLuck = function(self)
        return ffi.getprivate(self, "Luck")
    end,
    GetRange = function(self)
        return ffi.getprivate(self, "Range")
    end,
    GetShotSpeed = function(self)
        return ffi.getprivate(self, "ShotSpeed")
    end,
    GetSpeed = function(self)
        return ffi.getprivate(self, "Speed")
    end,
    GetTargetItem = function(self)
        return ffi.getprivate(self, "TargetItem")
    end,
}

setmetatable(ProceduralItemMT, { __index = function() end })
ProceduralItemMT.__index = ProceduralItemMT

ffi.metatype("struct ProceduralItem", ProceduralItemMT)

ProceduralItem = setmetatable({}, {
    __class = ProceduralItemMT,
})
