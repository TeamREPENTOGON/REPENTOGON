ffi.cdef[[
typedef struct {
    int32_t GridIndex; // 0x0
    int32_t SafeGridIndex; // 0x4
    int32_t ListIndex; // 0x8
    int32_t _; // unk0: 0xC
    RoomConfig* Data; // 0x10
    RoomConfig* OverrideData; // 0x14
    int32_t AllowedDoors; // 0x18
    int32_t _[8]; // Doors: 0x1C
    int32_t DisplayFlags; // 0x3C
    int32_t VisitedCount; // 0x40
    int32_t Flags; // 0x44
    int16_t _; // unk9: 0x48
    int16_t ClearCount; // 0x4A
    int32_t _; // unk10: 0x4C
    int32_t PoopCount; // 0x50
    int32_t PitsCount; // 0x54
    int32_t DecorationSeed; // 0x58
    int32_t SpawnSeed; // 0x5C
    int32_t AwardSeed; // 0x60
    char _[0x4]; // pad0: 0x64
    char _[0xc]; // SavedGridEntities: std::vector 0x68
    char _[0xc]; // SavedEntities: std::vector 0x74
    char _[0x8]; // RestrictedGridIndexes: std::set<int> 0x80;
    char _[0xc]; // SavedEntities2: std::vector 0x88
    char _[0x18]; // pad1: 0x94
    int16_t ShopItemIdx; // 0xAC
    int16_t ShopItemDiscountIdx; // 0xAE
    int32_t DeliriumDistance; // 0xB0
    int32_t _; // unk29: 0xB4
} RoomDescriptor;

typedef struct {
    uint32_t Size;
    RoomDescriptor* Rooms;
} RoomDescriptor_List;

bool L_RoomDescriptor_GetChallengeDone(RoomDescriptor* self);
bool L_RoomDescriptor_GetClear(RoomDescriptor* self);
bool L_RoomDescriptor_GetHasWater(RoomDescriptor* self);
bool L_RoomDescriptor_GetNoReward(RoomDescriptor* self);
bool L_RoomDescriptor_GetPressurePlatesTriggered(RoomDescriptor* self);
bool L_RoomDescriptor_GetSacrificeDone(RoomDescriptor* self);
bool L_RoomDescriptor_GetSurpriseMiniboss(RoomDescriptor* self);

void L_RoomDescriptor_SetChallengeDone(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetClear(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetHasWater(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetNoReward(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetPressurePlatesTriggered(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetSacrificeDone(RoomDescriptor* self, bool value);
void L_RoomDescriptor_SetSurpriseMiniboss(RoomDescriptor* self, bool value);
]]

local repentogon = ffidll
local lffi = ffi

local RoomDescriptorFuncs = {}

local function SetBoolFunc(func)
    return function(self, value)
        ffichecks.checkboolean(2, value)
        func(self, value)
    end
end

local getkeys = {
    ChallengeDone = repentogon.L_RoomDescriptor_GetChallengeDone,
    Clear = repentogon.L_RoomDescriptor_GetClear,
    HasWater = repentogon.L_RoomDescriptor_GetHasWater,
    NoReward = repentogon.L_RoomDescriptor_GetNoReward,
    PressurePlatesTriggered = repentogon.L_RoomDescriptor_GetPressurePlatesTriggered,
    SacrificeDone = repentogon.L_RoomDescriptor_GetSacrificeDone,
    SurpriseMiniboss = repentogon.L_RoomDescriptor_GetSurpriseMiniboss  
}

local setkeys = {
    ChallengeDone = SetBoolFunc(repentogon.L_RoomDescriptor_SetChallengeDone),
    Clear = SetBoolFunc(repentogon.L_RoomDescriptor_SetClear),
    HasWater = SetBoolFunc(repentogon.L_RoomDescriptor_SetHasWater),
    NoReward = SetBoolFunc(repentogon.L_RoomDescriptor_SetNoReward),
    PressurePlatesTriggered = SetBoolFunc(repentogon.L_RoomDescriptor_SetPressurePlatesTriggered),
    SacrificeDone = SetBoolFunc(repentogon.L_RoomDescriptor_SetSacrificeDone),
    SurpriseMiniboss = SetBoolFunc(repentogon.L_RoomDescriptor_SetSurpriseMiniboss)
}

local RoomDescriptorMT = lffi.metatype("RoomDescriptor", {
    -- todo: add roomconfig name to __tostring
    __tostring = function(self) return "RoomDescriptor" end,
    __index = function(self, key)
        if getkeys[key] ~= nil then
			return getkeys[key](self)
		end
		return RoomDescriptorFuncs[key]
    end,
    __newindex = function(self, key, value)
		if setkeys[key] ~= nil then
			return setkeys[key](self, value)
		else
			error(string.format("no writable variable '%s'", key))
		end
	end,
})

local RoomDescriptorListFuncs = {}

function RoomDescriptorFuncs:Get(index)
    ffichecks.checknumber(1, index)

    return self.Rooms[index]
end

local RoomDescriptorListMT = lffi.metatype("RoomDescriptor_List", {
    __tostring = function(self) return "RoomDescriptor[" .. self.Size .. "]" end,
    __len = function(self) return self.Size end,
    __index = RoomDescriptorFuncs
})