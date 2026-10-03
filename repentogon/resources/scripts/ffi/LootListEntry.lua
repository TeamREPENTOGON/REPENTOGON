ffi.cdef [[
    struct LootListEntry {
        private uint32_t Type : 0x0;
        private uint32_t Variant : 0x4;
        private uint32_t SubType : 0x8;
        private uint32_t Seed : 0xc;
        private struct RNG* RNG : 0x10;
    } : 0x14;
]]
    
local repentogon = ffidll
local ffi = ffi

local LootListEntryMT
LootListEntryMT = {
    __type = "LootListEntry",
    GetRNG = function(self)
        return ffi.getprivate(self, "RNG")
    end,
    GetSeed = function(self)
        return ffi.getprivate(self, "Seed")
    end,
    GetSubType = function(self)
        return ffi.getprivate(self, "SubType")
    end,
    GetType = function(self)
        return ffi.getprivate(self, "Type")
    end,
    GetVariant = function(self)
        return ffi.getprivate(self, "Variant")
    end
}

setmetatable(LootListEntryMT, { __index = function() end })
LootListEntryMT.__index = LootListEntryMT

local LootListEntryT = ffi.metatype("struct LootListEntry", LootListEntryMT)

LootListEntry = setmetatable({}, {
    __class = LootListEntryMT,
})