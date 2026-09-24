ffi.cdef [[
    struct RoomConfigEntry {
        const uint32_t Type : 0x0;
        const uint16_t Variant : 0x4;
        const uint16_t Subtype : 0x6;
        const float Weight : 0x8;
    } : 0xc;

    struct RoomConfigEntries {
        private struct RoomConfigEntry* _begin;
        private uint32_t _size;
    };
]];


local ffi = ffi

local RoomConfigEntriesMT; RoomConfigEntriesMT = { __type = "RoomConfigEntries" }

local function el_size(self)
    return ffi.getprivate(self, "_size")
end

RoomConfigEntriesMT.__len = function(self) return el_size(self) end

RoomConfigEntriesMT.__index = function(self, k)
    if k == "Size" then return el_size(self) end
    return RoomConfigEntriesMT[k]
end

function RoomConfigEntriesMT:Get(idx)
    ffichecks.checkinteger(1, idx)
    if idx < 0 or idx >= el_size(self) then return nil end
    return ffi.getprivate(self, "_begin") + idx
end

ffi.metatype("struct RoomConfigEntries", RoomConfigEntriesMT)
RoomConfigEntries = setmetatable({}, { __class = RoomConfigEntriesMT })

function __RoomConfigEntries_wrap(ptr, count)
    local e = ffi.new("struct RoomConfigEntries")
    ffi.setprivate(e, "_begin", ptr)
    ffi.setprivate(e, "_size", count)
    return e
end

------------------

local RoomConfigEntryMT
RoomConfigEntryMT = {
    __type = "RoomConfigEntry",
}

setmetatable(RoomConfigEntryMT, {
    __index = function() end,
})
RoomConfigEntryMT.__index = RoomConfigEntryMT

local RoomConfigEntryT = ffi.metatype("struct RoomConfigEntry", RoomConfigEntryMT)

RoomConfigEntry = setmetatable({}, {
    __class = RoomConfigEntryMT,
})
