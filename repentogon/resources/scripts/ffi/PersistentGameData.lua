ffi.cdef [[
    struct PersistentGameData {
        private bool ItemsCollection[733] : 0xae8;
        private bool Bosses[104] : 0xe07;
    } : 0xf90;

    struct PersistentGameData* L_PersistentGameData_Get();
    bool L_PersistentGameData_AddBestiaryKill(struct PersistentGameData*, int, int);
    void L_PersistentGameData_AddBossKilled(struct PersistentGameData*, int);
    int L_PersistentGameData_GetBestiaryDeathCount(struct PersistentGameData*, int, int);
    int L_PersistentGameData_GetBestiaryEncounterCount(struct PersistentGameData*, int, int);
    int L_PersistentGameData_GetBestiaryKillCount(struct PersistentGameData*, int, int);
    int L_PersistentGameData_GetEventCounter(struct PersistentGameData*, int);
    void L_PersistentGameData_IncreaseEventCounter(struct PersistentGameData*, int, int);
    bool L_PersistentGameData_IsChallengeCompleted(struct PersistentGameData*, int);
    bool L_PersistentGameData_TryUnlock(struct PersistentGameData*, int, bool);
    bool L_PersistentGameData_Unlock(struct PersistentGameData*, int, bool);
    bool L_PersistentGameData_Unlocked(struct PersistentGameData*, int);
]]
local repentogon = ffidll
local ffi = ffi

rawset(Isaac, "GetPersistentGameData", function()
    local result = repentogon.L_PersistentGameData_Get() return result
end)

local function CheckBossID(arg, id)
    if id > 103 or id < 1 then
        ffichecks.argerror(arg, string.format("expected BossType between 1 and 103 inclusive, got %d", id), 3)
    end
end

local function CheckEventCounter(arg, counter)
    if counter >= EventCounter.NUM_EVENT_COUNTERS then
        ffichecks.argerror(arg, string.format("EventCounter cannot be higher than %d", EventCounter.NUM_EVENT_COUNTERS), 3)
    end
end

local PersistentGameDataMT
PersistentGameDataMT = {
    __type = "PersistentGameData",
    AddBestiaryKill = function(self, entType, variant)
        entType = ffichecks.checkinteger(1, entType)
        variant = ffichecks.optnumber(variant, 0)
        local result = repentogon.L_PersistentGameData_AddBestiaryKill(self, entType, variant) return result
    end,
    AddBossKilled = function(self, boss)
        boss = ffichecks.checkinteger(1, boss)
        CheckBossID(1, boss)
        repentogon.L_PersistentGameData_AddBossKilled(self, boss)
    end,
    GetBestiaryDeathCount = function(self, entType, variant)
        entType = ffichecks.checkinteger(1, entType)
        variant = ffichecks.checkinteger(2, variant)
        local result = repentogon.L_PersistentGameData_GetBestiaryDeathCount(self, entType, variant) return result
    end,
    GetBestiaryEncounterCount = function(self, entType, variant)
        entType = ffichecks.checkinteger(1, entType)
        variant = ffichecks.checkinteger(2, variant)
        local result = repentogon.L_PersistentGameData_GetBestiaryEncounterCount(self, entType, variant) return result
    end,
    GetBestiaryKillCount = function(self, entType, variant)
        entType = ffichecks.checkinteger(1, entType)
        variant = ffichecks.checkinteger(2, variant)
        local result = repentogon.L_PersistentGameData_GetBestiaryKillCount(self, entType, variant) return result
    end,
    GetEventCounter = function(self, counter)
        counter = ffichecks.checkinteger(1, counter)
        CheckEventCounter(1, counter)
        local result = repentogon.L_PersistentGameData_GetEventCounter(self, counter) return result
    end,
    IncreaseEventCounter = function(self, counter, count)
        counter = ffichecks.checkinteger(1, counter)
        count = ffichecks.checkinteger(2, count)
        CheckEventCounter(1, counter)
        repentogon.L_PersistentGameData_IncreaseEventCounter(self, counter, count)
    end,
    IsBossKilled = function(self, boss)
        boss = ffichecks.checkinteger(1, boss)
        CheckBossID(1, boss)
        return ffi.getprivate(self, "Bosses")[boss]
    end,
    IsChallengeCompleted = function(self, challenge)
        challenge = ffichecks.checkinteger(1, challenge)
        local result = repentogon.L_PersistentGameData_IsChallengeCompleted(self, challenge) return result
    end,
    IsItemInCollection = function(self, item)
        item = ffichecks.checkinteger(1, item)
        if item >= CollectibleType.NUM_COLLECTIBLES then
            ffichecks.argerror(1, string.format("CollectibleType cannot be higher than %d", CollectibleType.NUM_COLLECTIBLES - 1))
        end
        return ffi.getprivate(self, "ItemsCollection")[item]
    end,
    TryUnlock = function(self, unlock, blockPaperPopup)
        unlock = ffichecks.checkinteger(1, unlock)
        blockPaperPopup = ffichecks.optboolean(blockPaperPopup, false)
        local result = repentogon.L_PersistentGameData_TryUnlock(self, unlock, blockPaperPopup) return result
    end,
    Unlock = function(self, unlock, blockPaperPopup)
        unlock = ffichecks.checkinteger(1, unlock)
        blockPaperPopup = ffichecks.optboolean(blockPaperPopup, false)
        local result = repentogon.L_PersistentGameData_Unlock(self, unlock, blockPaperPopup) return result
    end,
    Unlocked = function(self, unlock)
        unlock = ffichecks.checkinteger(1, unlock)
        local result = repentogon.L_PersistentGameData_Unlocked(self, unlock) return result
    end,
}

setmetatable(PersistentGameDataMT, { __index = function() end })
PersistentGameDataMT.__index = PersistentGameDataMT

local PersistentGameDataT = ffi.metatype("struct PersistentGameData", PersistentGameDataMT)

PersistentGameData = setmetatable({}, {
    __class = PersistentGameDataMT,
})

