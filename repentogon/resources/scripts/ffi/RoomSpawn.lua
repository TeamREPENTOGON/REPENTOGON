ffi.cdef[[
// stubs
typedef struct {
    uint16_t X; // 0x0
	uint16_t Y; // 0x2
	RoomConfig_Entry* _; // Entries 0x4
	uint16_t CountEntries; // 0x8
	char _[0x2]; // 0xa
	float SumWeights; // 0xc
} RoomConfig_Spawn; 
typedef struct {
    uint32_t Size;
    RoomConfig_Spawn* _items;
} RoomConfigSpawn_List;

RoomConfig_Entry* L_RoomConfigSpawn_PickEntry(RoomConfig_Spawn*, float);

RoomConfigEntry_List* L_RoomConfigSpawn_GetEntries(RoomConfig_Spawn*);
]]

local repentogon = ffidll
local lffi = ffi

local RoomSpawnFuncs = {}

function RoomSpawnFuncs:PickEntry(r)
    ffichecks.checknumber(2, r)

    return repentogon.L_RoomConfigSpawn_PickEntry(r)
end

local function GetGCCDataFunc(func)
    return function(self)
        return GC(func(self))
    end
end

local getkeys = {
    Entries = GetGCCDataFunc(repentogon.L_RoomConfigSpawn_GetEntries)
}

local RoomSpawnMT = lffi.metatype("RoomConfig_Spawn", {
    __tostring = function(self) return string.format("RoomConfigSpawn(%d, %d)", self.X, self.Y) end,
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        end
        return RoomSpawnFuncs[key]
    end
})

local RoomSpawnListMT = CppContainer.ArrayProxyMT("RoomConfigSpawn")