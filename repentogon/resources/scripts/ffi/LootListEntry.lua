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
        local result = ffi.getprivate(self, "RNG") return result
    end,
    GetSeed = function(self)
        local result = ffi.getprivate(self, "Seed") return result
    end,
    GetSubType = function(self)
        local result = ffi.getprivate(self, "SubType") return result
    end,
    GetType = function(self)
        local result = ffi.getprivate(self, "Type") return result
    end,
    GetVariant = function(self)
        local result = ffi.getprivate(self, "Variant") return result
    end
}

setmetatable(LootListEntryMT, { __index = function() end })
LootListEntryMT.__index = LootListEntryMT

local LootListEntryT = ffi.metatype("struct LootListEntry", LootListEntryMT)

LootListEntry = setmetatable({}, {
    __class = LootListEntryMT,
})