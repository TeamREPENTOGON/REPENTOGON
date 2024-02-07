ffi.cdef[[
// stubs
typedef struct {
    int StageID; // 0x0
	int Mode; // 0x4
	int Type; // 0x8
	unsigned int Variant; // 0xc
	int Subtype; // 0x10
	char _[0x18]; // std_string Name 0x14
	int Difficulty; // 0x2c
	float InitialWeight; // 0x30
	float Weight; // 0x34
	int Doors; // 0x38
	int _; // 0x3c
	void* _; // RoomSpawn* Spawns 0x40
	uint16_t SpawnCount; // 0x44
	char Width; // 0x46
	char Height; // 0x47
	int Shape; // 0x48
	int _; // 0x4c
	int _; // 0x50
	int _; // 0x54
	int OriginalVariant; // 0x58
} RoomConfig;

const char* L_RoomConfig_GetName(RoomConfig*);
RoomConfigSpawn_List* L_RoomConfig_GetSpawns(RoomConfig*);
]]

local repentogon = ffidll
local lffi = ffi

local RoomConfigFuncs = {}

local function GetStrFunc(func, shouldGC)
    if shouldGC == true then
        return function(self)
            return GCString(func(self))
        end
    else 
        return function(self)
            return lffi.string(func(self))
        end
    end
end

local function GetGCCDataFunc(func)
    return function(self)
        return GC(func(self))
    end
end

local getkeys = {
    Name = GetStrFunc(repentogon.L_RoomConfig_GetName),
    Spawns = GetGCCDataFunc(repentogon.L_RoomConfig_GetSpawns)
}

local RoomConfigMT = lffi.metatype("RoomConfig", {
    __tostring = function(self) return "RoomConfig(" .. self.Name .. ")" end,
    __index = function(self, key)
		if getkeys[key] ~= nil then
			return getkeys[key](self)
		end
	end,
    __newindex = function() end
})