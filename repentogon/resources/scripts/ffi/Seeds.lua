ffi.cdef[[
struct Seeds {
    const bool _isCustomRun;
    const char _pad1[0x03];
    const unsigned int _gameStartSeed;
    struct RNG _rng;
    const unsigned int _stageSeeds[14];
    const unsigned int _playerInitSeed;
    const char _pad2[0x04];
    const int _seedEffectsCount;
};

typedef struct Seeds* Seeds;

void L_Seeds_AddSeedEffect(struct Seeds*, unsigned int);
bool L_Seeds_CanAddSeedEffect(struct Seeds*, unsigned int);
void L_Seeds_ClearSeedEffects(struct Seeds*);
void L_Seeds_ClearStartSeed(struct Seeds*);
int L_Seeds_CountUnlockedSeedEffects(void);
void L_Seeds_ForgetStageSeed(struct Seeds*, unsigned int);
void L_Seeds_Seed2String(unsigned int seed, char* output);
bool L_Seeds_HasSeedEffect(struct Seeds*, int);
void L_Seeds_InitSeedInfo(void);
bool L_Seeds_IsSeedComboBanned(unsigned int, unsigned int);
void L_Seeds_RemoveBlockingSeedEffects(struct Seeds*, unsigned int);
void L_Seeds_RemoveSeedEffect(struct Seeds*, int);
void L_Seeds_Reset(struct Seeds*);
int L_Seeds_String2Seed(const char*);
void L_Seeds_SetStartSeed(struct Seeds*, unsigned int);
void L_Seeds_SetStageSeed(struct Seeds*, int, unsigned int);
]]
local GAME_START_SEED_NOT_SET_ERR = "Error: Game Start Seed was not set!"
local seedStringBuffer = ffi.new("char[9]")

-- Lua dictionary lookup will be faster. For optimization, all eastereggs here should be exactly 9 characters.
-- That way all we have to do on our end before doing a lookup is adding padding to input strings and trimming
-- them.
local SEED_EFFECT_BY_CODE = {
    ["SL0W 4ME2"] = 1,
    ["HART BEAT"] = 2,
    ["CAM0 K1DD"] = 3,
    ["CAM0 F0ES"] = 4,
    ["CAM0 DR0P"] = 5,
    ["WH0A WHAT"] = 6,
    ["FART SNDS"] = 7,
    ["B00B T00B"] = 8,
    ["DYSL EX1A"] = 9,
    ["KEEP TRAK"] = 10,
    ["KEEP AWAY"] = 11,
    ["DRAW KCAB"] = 12,
    ["CHAM P10N"] = 13,
    ["1MN0 B0DY"] = 14,
    ["BL1N DEYE"] = 15,
    ["BASE MENT"] = 16,
    ["C0CK FGHT"] = 17,
    ["C0NF ETT1"] = 18,
    ["FEAR M1NT"] = 19,
    ["FRA1 DN0T"] = 20,
    ["CLST RPH0"] = 21,
    ["BL00 00DY"] = 23,
    ["BRWN SNKE"] = 24,
    ["PAC1 F1SM"] = 25,
    ["D0NT ST0P"] = 26,
    ["THEG H0ST"] = 27,
    ["30M1 N1TS"] = 28,
    ["MED1 C1NE"] = 29,
    ["FACE D0WN"] = 30,
    ["C0ME BACK"] = 32,
    ["FREE 2PAY"] = 33,
    ["T0PH EAVY"] = 35,
    ["T1NY D0ME"] = 36,
    ["PTCH BLCK"] = 37,
    ["TEAR GL0W"] = 38,
    ["ANDA NTE "] = 41,
    ["LARG HET0"] = 42,
    ["ALLE GR0 "] = 43,
    ["PRES T0  "] = 44,
    ["THEB LANK"] = 46,
    ["HARD HARD"] = 47,
    ["BRTL B0NS"] = 48,
    ["1CES KATE"] = 52,
    ["KAPP A   "] = 53,
    ["H0H0 H0H0"] = 54,
    ["K1DS M0DE"] = 55,
    ["DARK NESS"] = 56,
    ["LABY RNTH"] = 57,
    ["L0ST     "] = 58,
    ["VNKN 0WN "] = 59,
    ["MAZE     "] = 60,
    ["BL1N D   "] = 61,
    ["CVRS ED  "] = 62,
    ["N1TE L1TE"] = 63,
    ["THRE AD  "] = 64,
    ["F0VN D   "] = 65,
    ["N0W1 KN0W"] = 66,
    ["PATH F1ND"] = 67,
    ["BRA1 LLE "] = 68,
    ["BLCK CNDL"] = 70,
    ["N0RE TVRN"] = 71,
    ["G0NE S00N"] = 72,
    ["ALM1 GHTY"] = 73,
    ["BRAV ERY "] = 74,
    ["C0WR D1CE"] = 75,
    ["AX1S ALGN"] = 76,
    ["SVPE RH0T"] = 77,
    ["M0DE SEVN"] = 78,
    ["GFVE LLLL"] = 79,
}

local ffi = ffi
local ffidll = ffidll
local sformat = string.format
local L_Seeds_AddSeedEffect = ffidll.L_Seeds_AddSeedEffect
local L_Seeds_CanAddSeedEffect = ffidll.L_Seeds_CanAddSeedEffect
local L_Seeds_ClearSeedEffects = ffidll.L_Seeds_ClearSeedEffects
local L_Seeds_ClearStartSeed = ffidll.L_Seeds_ClearStartSeed
local L_Seeds_CountUnlockedSeedEffects = ffidll.L_Seeds_CountUnlockedSeedEffects
local L_Seeds_ForgetStageSeed = ffidll.L_Seeds_ForgetStageSeed
local L_Seeds_Seed2String = ffidll.L_Seeds_Seed2String
local L_Seeds_HasSeedEffect = ffidll.L_Seeds_HasSeedEffect
local L_Seeds_InitSeedInfo = ffidll.L_Seeds_InitSeedInfo
local L_Seeds_IsSeedComboBanned = ffidll.L_Seeds_IsSeedComboBanned
local L_Seeds_RemoveBlockingSeedEffects = ffidll.L_Seeds_RemoveBlockingSeedEffects
local L_Seeds_RemoveSeedEffect = ffidll.L_Seeds_RemoveSeedEffect
local L_Seeds_Reset = ffidll.L_Seeds_Reset
local L_Seeds_String2Seed = ffidll.L_Seeds_String2Seed
local L_Seeds_SetStartSeed = ffidll.L_Seeds_SetStartSeed
local L_Seeds_SetStageSeed = ffidll.L_Seeds_SetStageSeed

local SeedsMT
SeedsMT = {
    __type = "Seeds",

    AddSeedEffect = function(self, value)
        ffichecks.checkinteger(1, value)
        L_Seeds_AddSeedEffect(self, value)
    end,

    CanAddSeedEffect = function(self, value)
        ffichecks.checkinteger(1, value)
        return L_Seeds_CanAddSeedEffect(self, value)
    end,

    ClearSeedEffects = function(self)
        L_Seeds_ClearSeedEffects(self)
    end,

    ClearStartSeed = function(self)
        L_Seeds_ClearStartSeed(self)
    end,

    CountSeedEffects = function(self)
        return self._seedEffectsCount
    end,

    ForgetStageSeed = function(self, stage)
        ffichecks.checkinteger(1, stage)
        L_Seeds_ForgetStageSeed(self, stage)
    end,

    GetNextSeed = function(self)
        if self._gameStartSeed == 0 then
            error(GAME_START_SEED_NOT_SET_ERR)
        end

        return self._rng:Next()
    end,

    GetPlayerInitSeed = function(self)
        if self._gameStartSeed == 0 then
            error(GAME_START_SEED_NOT_SET_ERR)
        end

        return self._playerInitSeed
    end,

    GetStageSeed = function(self, stage)
        ffichecks.checkinteger(1, stage)

        if self._gameStartSeed == 0 then
            error(GAME_START_SEED_NOT_SET_ERR)
        end

        if stage > 13 then
            stage = 13
        elseif stage < 1 then
            stage = 1
        end

        return self._stageSeeds[stage]
    end,

    GetStartSeed = function(self)
        local seed = self._gameStartSeed

        if seed == 0 then
            error("Game Start Seed was not set!")
        end

        return seed
    end,

    GetStartSeedString = function(self)
        local seed = self._gameStartSeed

        if seed == 0 then
            -- Well-documented default string used in main menu.
            return "B911 99JA"
        end

        return SeedsF.Seed2String(seed)
    end,

    HasSeedEffect = function(self, value)
        ffichecks.checkinteger(1, value)
        return L_Seeds_HasSeedEffect(self, value)
    end,

    IsCustomRun = function(self)
        return self._isCustomRun
    end,

    IsInitialized = function(self)
        return self._gameStartSeed ~= 0
    end,

    IsSeedComboBanned = function(self, effect1, effect2)
        ffichecks.checkinteger(1, effect1)
        ffichecks.checkinteger(2, effect2)
        return L_Seeds_IsSeedComboBanned(effect1, effect2)
    end,

    RemoveBlockingSeedEffects = function(self, effect)
        ffichecks.checkinteger(1, effect)
        L_Seeds_RemoveBlockingSeedEffects(self, effect)
    end,

    RemoveSeedEffect = function(self, effect)
        ffichecks.checkinteger(1, effect)
        L_Seeds_RemoveSeedEffect(self, effect)
    end,

    Reset = function(self)
        L_Seeds_Reset(self)
    end,

    SetStartSeed = function(self, seedString)
        ffichecks.checkstring(1, seedString)
        L_Seeds_SetStartSeed(self, L_Seeds_String2Seed(seedString))
    end,

    Restart = function(self, challenge)
        ffichecks.checkinteger(1, challenge)

        local seed = 0

        if self._isCustomRun and challenge == 0 then
            seed = self._gameStartSeed
        end

        L_Seeds_SetStartSeed(self, seed)
    end,

    SetStageSeed = function(self, stage, seed)
        ffichecks.checkinteger(1, stage)
        ffichecks.checkinteger(2, seed)

        if stage < 0 or stage > 13 then
            error("Invalid LevelStage (must be between 0 and 13)")
        end

        L_Seeds_SetStageSeed(self, stage, seed)
    end,
}

SeedsMT.__index = SeedsMT

local SeedsT = ffi.metatype("struct Seeds", SeedsMT)

SeedsF = {
    CountUnlockedSeedEffects = function()
        return L_Seeds_CountUnlockedSeedEffects()
    end,

    GetSeedEffect = function(str)
        ffichecks.checkstring(1, str)
        --  Trim string to be within 9 characters (and padding with spaces)
        return SEED_EFFECT_BY_CODE[sformat("%-9.9s", str)] or 0
    end,
    
    Seed2String = function(seed)
        ffichecks.checkinteger(1, seed)
        L_Seeds_Seed2String(seed, seedStringBuffer)
        return ffi.string(seedStringBuffer, 9)
    end,

    InitSeedInfo = function()
        L_Seeds_InitSeedInfo()
    end,

    String2Seed = function(seed)
        ffichecks.ffichecks.checkstring(1, seed)
        return L_Seeds_String2Seed(seed)
    end,

    IsStringValidSeed = function(seed)
        ffichecks.checkstring(1, seed)
        return L_Seeds_String2Seed(seed) ~= 0
    end,

    IsSpecialSeed = function(seed)
        return SeedsF.GetSeedEffect(seed) ~= 0
    end,
}