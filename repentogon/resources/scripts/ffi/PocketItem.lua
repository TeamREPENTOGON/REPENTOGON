ffi.cdef [[
    struct PocketItem {
        private int Slot : 0x0;
        private uint32_t Type : 0x4;
    } : 0x8;
    typedef struct PocketItem* PocketItemPtr;
]]

local ffi = ffi

local PocketItemMT
PocketItemMT = {
    __type = "PocketItem",
    GetSlot = function(self)
        local result = ffi.getprivate(self, "Slot") return result
    end,
    GetType = function(self)
        local result = ffi.getprivate(self, "Type") return result
    end,
}

setmetatable(PocketItemMT, { __index = function() end })
PocketItemMT.__index = PocketItemMT

ffi.metatype("struct PocketItem", PocketItemMT)

PocketItem = setmetatable({}, {
    __class = PocketItemMT,
})
