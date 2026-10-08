ffi.cdef [[
    struct Seeds {
        private bool CustomRun : 0x0;
        private uint32_t GameStartSeed : 0x4;
        private uint32_t StageSeeds[14] : 0x18;
        private uint32_t PlayerInitSeed : 0x50;
        private int SeedEffectsCount : 0x58;
    } : 0x5c;
    typedef struct Seeds* SeedsPtr;

    void L_Seeds_AddSeedEffect(struct Seeds*, int);
    bool L_Seeds_CanAddSeedEffect(struct Seeds*, int);
    void L_Seeds_ClearSeedEffects(struct Seeds*);
    void L_Seeds_ForgetStageSeed(struct Seeds*, int);
    unsigned int L_Seeds_GetNextSeed(struct Seeds*);
    bool L_Seeds_HasSeedEffect(struct Seeds*, int);
    bool L_Seeds_IsSeedComboBanned(struct Seeds*, int, int);
    void L_Seeds_RemoveBlockingSeedEffects(struct Seeds*, int);
    void L_Seeds_RemoveSeedEffect(struct Seeds*, int);
    void L_Seeds_Reset(struct Seeds*);
    void L_Seeds_Restart(struct Seeds*, int);
    void L_Seeds_SetStartSeed(struct Seeds*, const char*);

    const char* L_Seeds_Seed2String(unsigned int);
    unsigned int L_Seeds_String2Seed(const char*);
    int L_Seeds_GetSeedEffect(const char*);
    int L_Seeds_CountUnlockedSeedEffects();
    void L_Seeds_InitSeedInfo();
]]

local repentogon = ffidll
local ffi = ffi

local NUM_STAGES = 14

local function Seed2String(seed)
    local result = ffi.string(repentogon.L_Seeds_Seed2String(seed)) return result
end

local SeedsMT
SeedsMT = {
    __type = "Seeds",
    AddSeedEffect = function(self, seedEffect)
        seedEffect = ffichecks.checkinteger(1, seedEffect)
        repentogon.L_Seeds_AddSeedEffect(self, seedEffect)
    end,
    CanAddSeedEffect = function(self, seedEffect)
        seedEffect = ffichecks.checkinteger(1, seedEffect)
        local result = repentogon.L_Seeds_CanAddSeedEffect(self, seedEffect) return result
    end,
    ClearSeedEffects = function(self)
        repentogon.L_Seeds_ClearSeedEffects(self)
    end,
    ClearStartSeed = function(self)
        ffi.setprivate(self, "GameStartSeed", 0)
    end,
    CountSeedEffects = function(self)
        local result = ffi.getprivate(self, "SeedEffectsCount") return result
    end,
    ForgetStageSeed = function(self, stage)
        stage = ffichecks.checkinteger(1, stage)
        repentogon.L_Seeds_ForgetStageSeed(self, stage)
    end,
    GetNextSeed = function(self)
        local result = repentogon.L_Seeds_GetNextSeed(self) return result
    end,
    GetPlayerInitSeed = function(self)
        local result = ffi.getprivate(self, "PlayerInitSeed") return result
    end,
    GetStageSeed = function(self, stage)
        stage = ffichecks.checkinteger(1, stage)
        if stage < 0 or stage >= NUM_STAGES then
            stage = NUM_STAGES - 1
        end
        return ffi.getprivate(self, "StageSeeds")[stage]
    end,
    GetStartSeed = function(self)
        local result = ffi.getprivate(self, "GameStartSeed") return result
    end,
    GetStartSeedString = function(self)
        return Seed2String(ffi.getprivate(self, "GameStartSeed"))
    end,
    HasSeedEffect = function(self, seedEffect)
        seedEffect = ffichecks.checkinteger(1, seedEffect)
        local result = repentogon.L_Seeds_HasSeedEffect(self, seedEffect) return result
    end,
    IsCustomRun = function(self)
        local result = ffi.getprivate(self, "CustomRun") return result
    end,
    IsInitialized = function(self)
        return ffi.getprivate(self, "GameStartSeed") ~= 0
    end,
    IsSeedComboBanned = function(self, seedEffect1, seedEffect2)
        seedEffect1 = ffichecks.checkinteger(1, seedEffect1)
        seedEffect2 = ffichecks.checkinteger(2, seedEffect2)
        local result = repentogon.L_Seeds_IsSeedComboBanned(self, seedEffect1, seedEffect2) return result
    end,
    RemoveBlockingSeedEffects = function(self, seedEffect)
        seedEffect = ffichecks.checkinteger(1, seedEffect)
        repentogon.L_Seeds_RemoveBlockingSeedEffects(self, seedEffect)
    end,
    RemoveSeedEffect = function(self, seedEffect)
        seedEffect = ffichecks.checkinteger(1, seedEffect)
        repentogon.L_Seeds_RemoveSeedEffect(self, seedEffect)
    end,
    Reset = function(self)
        repentogon.L_Seeds_Reset(self)
    end,
    Restart = function(self, challenge)
        challenge = ffichecks.checkinteger(1, challenge)
        repentogon.L_Seeds_Restart(self, challenge)
    end,
    SetStageSeed = function(self, stage, seed)
        stage = ffichecks.checkinteger(1, stage)
        seed = ffichecks.checkinteger(2, seed)
        if stage < 0 or stage >= NUM_STAGES then
            ffichecks.argerror(1, "Invalid LevelStage (must be between 0 and 13)")
        end
        if seed == 0 then
            seed = 1
        end
        ffi.getprivate(self, "StageSeeds")[stage] = seed
    end,
    SetStartSeed = function(self, seed)
        seed = ffichecks.checkstring(1, seed)
        repentogon.L_Seeds_SetStartSeed(self, seed)
    end,
}

setmetatable(SeedsMT, { __index = function() end })
SeedsMT.__index = SeedsMT

ffi.metatype("struct Seeds", SeedsMT)

Seeds = setmetatable({
    CountUnlockedSeedEffects = function()
        local result = repentogon.L_Seeds_CountUnlockedSeedEffects() return result
    end,
    GetSeedEffect = function(seed)
        seed = ffichecks.checkstring(1, seed)
        local result = repentogon.L_Seeds_GetSeedEffect(seed) return result
    end,
    InitSeedInfo = function()
        repentogon.L_Seeds_InitSeedInfo()
    end,
    IsSpecialSeed = function(seed)
        seed = ffichecks.checkstring(1, seed)
        return repentogon.L_Seeds_GetSeedEffect(seed) ~= 0
    end,
    IsStringValidSeed = function(seed)
        seed = ffichecks.checkstring(1, seed)
        return repentogon.L_Seeds_String2Seed(seed) ~= 0
    end,
    Seed2String = function(seed)
        seed = ffichecks.checkinteger(1, seed)
        return Seed2String(seed)
    end,
    String2Seed = function(seed)
        seed = ffichecks.checkstring(1, seed)
        local result = repentogon.L_Seeds_String2Seed(seed) return result
    end,
}, {
    __class = SeedsMT,
})
