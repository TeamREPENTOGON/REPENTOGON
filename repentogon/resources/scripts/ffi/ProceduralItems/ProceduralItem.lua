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
        local result = ffi.getprivate(self, "Damage") return result
    end,
    GetEffect = function(self, index)
        index = ffichecks.checkinteger(1, index)
        if index >= 0 and index < GetEffectCount(self) then
            return ffi.getprivate(self, "EffectsFirst")[index]
        end
        return nil
    end,
    GetEffectCount = GetEffectCount,
    GetFireDelay = function(self)
        local result = ffi.getprivate(self, "FireDelay") return result
    end,
    GetID = function(self)
        local result = ffi.getprivate(self, "ID") return result
    end,
    GetItem = function(self)
        local result = ffi.getprivate(self, "Item") return result
    end,
    GetLuck = function(self)
        local result = ffi.getprivate(self, "Luck") return result
    end,
    GetRange = function(self)
        local result = ffi.getprivate(self, "Range") return result
    end,
    GetShotSpeed = function(self)
        local result = ffi.getprivate(self, "ShotSpeed") return result
    end,
    GetSpeed = function(self)
        local result = ffi.getprivate(self, "Speed") return result
    end,
    GetTargetItem = function(self)
        local result = ffi.getprivate(self, "TargetItem") return result
    end,
}

setmetatable(ProceduralItemMT, { __index = function() end })
ProceduralItemMT.__index = ProceduralItemMT

ffi.metatype("struct ProceduralItem", ProceduralItemMT)

ProceduralItem = setmetatable({}, {
    __class = ProceduralItemMT,
})
