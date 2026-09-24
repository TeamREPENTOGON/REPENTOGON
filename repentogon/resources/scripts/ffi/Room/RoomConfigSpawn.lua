ffi.cdef [[
    struct RoomConfigSpawn {
        const uint16_t X : 0x0;
        const uint16_t Y : 0x2;
        private struct RoomConfigEntry* _Entries : 0x4;
        const uint8_t EntryCount : 0x8;
        const float SumWeights : 0xC;
    } : 0x10;

    struct RoomConfigSpawns {
        private struct RoomConfigSpawn* _begin;
        private uint32_t _size;
    };

    struct RoomConfigEntry* L_RoomConfigSpawn_PickEntry(struct RoomConfigSpawn*, float);
]]

local ffi = ffi
local repentogon = ffidll

local RoomConfigSpawnsMT; RoomConfigSpawnsMT = { __type = "RoomConfigSpawns" }

local function el_size(self)
    return ffi.getprivate(self, "_size")
end

RoomConfigSpawnsMT.__len = function(self) return el_size(self) end

RoomConfigSpawnsMT.__index = function(self, k)
    if k == "Size" then return el_size(self) end
    return RoomConfigSpawnsMT[k]
end

function RoomConfigSpawnsMT:Get(idx)
    ffichecks.checkinteger(1, idx)
    if idx < 0 or idx >= el_size(self) then return nil end
    return ffi.getprivate(self, "_begin") + idx
end

ffi.metatype("struct RoomConfigSpawns", RoomConfigSpawnsMT)
RoomConfigSpawns = setmetatable({}, { __class = RoomConfigSpawnsMT })

function __RoomConfigSpawns_wrap(ptr, count)
    local s = ffi.new("struct RoomConfigSpawns")
    ffi.setprivate(s, "_begin", ptr)
    ffi.setprivate(s, "_size", count)
    return s
end

------------------

local RoomConfigSpawnMT
RoomConfigSpawnMT = {
    __type = "RoomConfigSpawn",
    PickEntry = function(self, r)
        ffichecks.checknumber(1, r)
        local entry = repentogon.L_RoomConfigSpawn_PickEntry(self, r)
        if entry == nil then return nil end
        return entry
    end,
}

RoomConfigSpawnMT.__index = function(self, key)
    if key == "Entries" then
        return __RoomConfigEntries_wrap(ffi.getprivate(self, "_Entries"), self.EntryCount)
    end
    return RoomConfigSpawnMT[key]
end

local RoomConfigSpawnT = ffi.metatype("struct RoomConfigSpawn", RoomConfigSpawnMT)

RoomConfigSpawn = setmetatable({}, {
    __class = RoomConfigSpawnMT,
})
