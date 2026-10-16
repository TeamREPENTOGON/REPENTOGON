ffi.cdef[[
    struct ChallengeParamIntVector {
        int* First;
        int* Last;
        int* End;
    };

    struct ChallengeParam {
        private struct StdString Name : 0x0;
        private struct ChallengeParamIntVector CollectibleList : 0x20;
        private struct ChallengeParamIntVector TrinketList : 0x2c;
        private int StartingPill : 0x38;
        private int StartingCard : 0x3c;
        private int EndStage : 0x40;
        private int PlayerType : 0x44;
        private struct ChallengeParamIntVector AchievementList : 0x48;
        private int SoulHearts : 0x54;
        private int BlackHearts : 0x58;
        private int Hearts : 0x5c;
        private int MaxHearts : 0x60;
        private int Coins : 0x64;
        private float AddDamage : 0x68;
        private bool ShootingEnabled : 0x6c;
        private int Difficulty : 0x70;
        private bool IsMegaSatan : 0x74;
        private float MinFireRate : 0x78;
        private bool MaxDamage : 0x7c;
        private bool MinShotSpeed : 0x7d;
        private bool BigRange : 0x7e;
        private int PathType : 0x80;
        private unsigned int Curse : 0x84;
        private unsigned int CurseFilter : 0x88;
        private struct ChallengeParamIntVector CollectibleTwinList : 0x8c;
    } : 0xa4;

    typedef struct ChallengeParam* ChallengeParamPtr;

    int L_ChallengeParam_GetRoomFilterSize(struct ChallengeParam*);
    void L_ChallengeParam_GetRoomFilter(struct ChallengeParam*, int*);
]]

local ffi = ffi
local repentogon = ffidll

local function VectorToTable(vec)
    return ffichecks.vectortotable(vec.First, vec.Last, ffi.sizeof("int"))
end

local ChallengeParamMT
ChallengeParamMT = {
    __type = "ChallengeParam",
    CanShoot = function(self)
        local result = ffi.getprivate(self, "ShootingEnabled") return result
    end,
    GetAchievementList = function(self)
        return VectorToTable(ffi.getprivate(self, "AchievementList"))
    end,
    GetAddDamage = function(self)
        local result = ffi.getprivate(self, "AddDamage") return result
    end,
    GetBlackHearts = function(self)
        local result = ffi.getprivate(self, "BlackHearts") return result
    end,
    GetCoins = function(self)
        local result = ffi.getprivate(self, "Coins") return result
    end,
    GetCollectibleList = function(self)
        return VectorToTable(ffi.getprivate(self, "CollectibleList"))
    end,
    GetCollectibleTwinList = function(self)
        return VectorToTable(ffi.getprivate(self, "CollectibleTwinList"))
    end,
    GetCurse = function(self)
        local result = ffi.getprivate(self, "Curse") return result
    end,
    GetCurseFilter = function(self)
        local result = ffi.getprivate(self, "CurseFilter") return result
    end,
    GetDifficulty = function(self)
        local result = ffi.getprivate(self, "Difficulty") return result
    end,
    GetEndStage = function(self)
        local result = ffi.getprivate(self, "EndStage") return result
    end,
    GetHearts = function(self)
        local result = ffi.getprivate(self, "Hearts") return result
    end,
    GetMaxHearts = function(self)
        local result = ffi.getprivate(self, "MaxHearts") return result
    end,
    GetMinFireRate = function(self)
        local result = ffi.getprivate(self, "MinFireRate") return result
    end,
    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Name"))
    end,
    GetPlayerType = function(self)
        local result = ffi.getprivate(self, "PlayerType") return result
    end,
    GetRoomFilter = function(self)
        local size = repentogon.L_ChallengeParam_GetRoomFilterSize(self)
        local result = {}
        if size == 0 then return result end
        local rooms = ffi.new("int[?]", size)
        repentogon.L_ChallengeParam_GetRoomFilter(self, rooms)
        for i = 0, size - 1 do
            result[i + 1] = rooms[i]
        end
        return result
    end,
    GetSoulHearts = function(self)
        local result = ffi.getprivate(self, "SoulHearts") return result
    end,
    GetStartingCard = function(self)
        local result = ffi.getprivate(self, "StartingCard") return result
    end,
    GetStartingPill = function(self)
        local result = ffi.getprivate(self, "StartingPill") return result
    end,
    GetTrinketList = function(self)
        return VectorToTable(ffi.getprivate(self, "TrinketList"))
    end,
    IsAltPath = function(self)
        return ffi.getprivate(self, "PathType") == 1
    end,
    IsBeastPath = function(self)
        return ffi.getprivate(self, "PathType") == 3
    end,
    IsBigRangeEnabled = function(self)
        local result = ffi.getprivate(self, "BigRange") return result
    end,
    IsMaxDamageEnabled = function(self)
        local result = ffi.getprivate(self, "MaxDamage") return result
    end,
    IsMegaSatanRun = function(self)
        local result = ffi.getprivate(self, "IsMegaSatan") return result
    end,
    IsMinShotSpeedEnabled = function(self)
        local result = ffi.getprivate(self, "MinShotSpeed") return result
    end,
    IsSecretPath = function(self)
        return ffi.getprivate(self, "PathType") == 2
    end,
}

setmetatable(ChallengeParamMT, { __index = function() end })
ChallengeParamMT.__index = ChallengeParamMT

ffi.metatype("struct ChallengeParam", ChallengeParamMT)

ChallengeParam = setmetatable({}, { __class = ChallengeParamMT })
