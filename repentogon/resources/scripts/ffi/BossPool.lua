ffi.cdef [[
    struct BossPoolEntry {
        int ID;
        float InitialWeight;
        float Weight;
        int AchievementID;
        int RoomVariantStart;
    };

    struct BossPool {
        private struct BossPoolEntry* EntriesFirst : 0x18;
        private struct BossPoolEntry* EntriesLast : 0x1c;
        private float TotalWeight : 0x24;
        private struct RNG RNG : 0x28;
        private int DoubleTroubleRoomVariantStart : 0x38;
    } : 0x3c;
    typedef struct BossPool* BossPoolPtr;

    struct BossPoolBoolVector {
        uint32_t* WordsFirst;
        uint32_t* WordsLast;
        uint32_t* WordsEnd;
        uint32_t Size;
    };

    struct BossPoolManager {
        private struct BossPool Pools[37] : 0x0;
        private struct BossPoolBoolVector RemovedBosses : 0x8ac;
        private struct BossPoolBoolVector LevelBlacklist : 0x8bc;
    } : 0x8cc;

    struct BossPoolManager* L_BossPoolManager_Get();
    const char* L_BossPool_GetName(struct BossPool*);
]]

local repentogon = ffidll
local ffi = ffi

local NUM_STB = 37

local function GetDoubleTroubleRoomVariantStart(self)
    return ffi.getprivate(self, "DoubleTroubleRoomVariantStart")
end

local BossPoolMT
BossPoolMT = {
    __type = "BossPool",
    GetDoubleTroubleRoomID = GetDoubleTroubleRoomVariantStart,
    GetDoubleTroubleRoomVariantStart = GetDoubleTroubleRoomVariantStart,
    GetEntries = function(self)
        local first = ffi.getprivate(self, "EntriesFirst")
        local entries = {}
        for i = 0, ffichecks.vectorsize(first, ffi.getprivate(self, "EntriesLast"), ffi.sizeof("struct BossPoolEntry")) - 1 do
            local entry = first[i]
            entries[i + 1] = {
                bossID = entry.ID,
                initialWeight = entry.InitialWeight,
                weight = entry.Weight,
                weightAlt = entry.Weight,
                achievementID = entry.AchievementID,
            }
        end
        return entries
    end,
    GetName = function(self)
        return ffi.string(repentogon.L_BossPool_GetName(self))
    end,
    GetRNG = function(self)
        return ffi.getprivate(self, "RNG")
    end,
    GetWeight = function(self)
        return ffi.getprivate(self, "TotalWeight")
    end,
}

setmetatable(BossPoolMT, { __index = function() end })
BossPoolMT.__index = BossPoolMT

ffi.metatype("struct BossPool", BossPoolMT)

BossPool = setmetatable({}, {
    __class = BossPoolMT,
})

local function BoolVectorToTable(vector)
    local words = vector.WordsFirst
    local result = {}
    for i = 1, vector.Size - 1 do
        result[i] = ((words[i >> 5] >> (i & 31)) & 1) ~= 0
    end
    return result
end

local function GetLevelBlacklist()
    return BoolVectorToTable(ffi.getprivate(repentogon.L_BossPoolManager_Get(), "LevelBlacklist"))
end

BossPoolManager = {
    GetLevelBlacklist = GetLevelBlacklist,
    GetPool = function(stbType)
        ffichecks.checkinteger(1, stbType)
        if stbType < 0 or stbType >= NUM_STB then
            ffichecks.argerror(1, "invalid STB type")
        end
        return ffi.getprivate(repentogon.L_BossPoolManager_Get(), "Pools") + stbType
    end,
    GetRemovedBosses = function()
        return BoolVectorToTable(ffi.getprivate(repentogon.L_BossPoolManager_Get(), "RemovedBosses"))
    end,
    GetRemovedSpecialBosses = GetLevelBlacklist,
}
