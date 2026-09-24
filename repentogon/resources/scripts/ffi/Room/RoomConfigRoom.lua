ffi.cdef [[
    struct RoomConfigRoom {
        const int32_t StageID : 0x0;
        const int32_t Mode : 0x4;
        const int32_t Type : 0x8;
        const uint32_t Variant : 0xC;
        const int32_t Subtype : 0x10;
        const int32_t Difficulty : 0x2C;
        const float InitialWeight : 0x30;
        const float Weight : 0x34;
        const uint32_t Doors : 0x38;
        private struct RoomConfigSpawn* _Spawns : 0x40;
        const uint16_t SpawnCount : 0x44;
        const int8_t Width : 0x46;
        const int8_t Height : 0x47;
        const int32_t Shape : 0x48;
        const uint32_t OriginalVariant : 0x58;
    } : 0x5C;

    typedef struct RoomConfigRoom* RoomConfigRoomPtr;

    const char* L_RoomConfigRoom_GetName(struct RoomConfigRoom*);
]];

local ffi = ffi
local repentogon = ffidll

local RoomConfigRoomMT
RoomConfigRoomMT = {
    __type = "RoomConfigRoom",
}

setmetatable(RoomConfigRoomMT, { __index = function() end })

RoomConfigRoomMT.__index = function(self, key)
    if key == "Name" then
        local name = repentogon.L_RoomConfigRoom_GetName(self)
        if name == nil then return "" end
        return ffi.string(name)
    elseif key == "Spawns" then
        return __RoomConfigSpawns_wrap(ffi.getprivate(self, "_Spawns"), self.SpawnCount)
    end
    return RoomConfigRoomMT[key]
end


local RoomConfigRoomT = ffi.metatype("struct RoomConfigRoom", RoomConfigRoomMT)

RoomConfigRoom = setmetatable({}, {
    __class = RoomConfigRoomMT,
})
