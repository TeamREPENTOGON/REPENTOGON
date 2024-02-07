ffi.cdef[[
typedef struct {
	unsigned int Type; // 0x0
	unsigned short int Variant; // 0x4
	unsigned short int Subtype; // 0x6
	float Weight; // 0x8
} RoomConfig_Entry;
typedef struct {
    uint32_t Size;
    RoomConfig_Entry* _items;
} RoomConfigEntry_List;
]]

local repentogon = ffidll
local lffi = ffi

local RoomEntryFuncs = {}

local RoomEntryMT = lffi.metatype("RoomConfig_Entry", {
    __tostring = function(self) return string.format("RoomConfigEntry(%u.%u.%u)", self.Type, self.Variant, self.Subtype) end,
    __index = RoomEntryFuncs
})

local RoomEntryListMT = CppContainer.ArrayProxyMT("RoomConfigEntry")