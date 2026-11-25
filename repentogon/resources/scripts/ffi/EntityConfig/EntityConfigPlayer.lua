ffi.cdef [[
    struct EntityConfigPlayer {
        private int ID : 0x0;
        private struct StdString Name : 0x4;
        private struct StdString SkinPath : 0x1c;
        private struct StdString NameImagePath : 0x34;
        private struct StdString PortraitPath : 0x4c;
        private struct StdString ExtraPortraitPath : 0x7c;
        private struct StdString CostumeSuffixName : 0x94;
        private int CostumeID : 0xac;
        private int Heart : 0xb0;
        private int Armor : 0xb4;
        private int BlackHeart : 0xb8;
        private int Coins : 0xbc;
        private int Bombs : 0xc0;
        private int Keys : 0xc4;
        private int Card : 0xc8;
        private int Pill : 0xcc;
        private bool Shooting : 0xd0;
        private int* CollectiblesFirst : 0xd4;
        private int* CollectiblesLast : 0xd8;
        private int Trinket : 0xe0;
        private int SkinColor : 0xe4;
        private int Achievement : 0xe8;
        private int BrokenHeart : 0xec;
        private uint32_t PocketActiveID : 0xf0;
        private struct StdString BirthrightDescription : 0xf4;
        private struct StdString BSkinParentName : 0x10c;
        private bool Hidden : 0x128;
        private struct Sprite* ModdedMenuBackgroundANM2 : 0x134;
        private struct Sprite* ModdedMenuPortraitANM2 : 0x138;
        private struct Sprite* ModdedGameOverANM2 : 0x13c;
        private struct Sprite* ModdedCoopMenuANM2 : 0x140;
        private struct Sprite* ModdedControlsANM2 : 0x144;
    } : 0x148;
    typedef struct EntityConfigPlayer* EntityConfigPlayerPtr;

    struct EntityConfigPlayer* L_EntityConfigPlayer_GetTaintedCounterpart(struct EntityConfigPlayer*);
]]

local repentogon = ffidll
local ffi = ffi

local HIDDEN_VANILLA_CHARACTERS = { [11] = true, [12] = true, [17] = true, [20] = true, [38] = true, [39] = true, [40] = true }

local EntityConfigPlayerMT
EntityConfigPlayerMT = {
    __type = "EntityConfigPlayer",
    CanShoot = function(self)
        local result = ffi.getprivate(self, "Shooting") return result
    end,
    GetAchievementID = function(self)
        local result = ffi.getprivate(self, "Achievement") return result
    end,
    GetBirthrightDescription = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BirthrightDescription"))
    end,
    GetBlackHearts = function(self)
        local result = ffi.getprivate(self, "BlackHeart") return result
    end,
    GetBombs = function(self)
        local result = ffi.getprivate(self, "Bombs") return result
    end,
    GetBrokenHearts = function(self)
        local result = ffi.getprivate(self, "BrokenHeart") return result
    end,
    GetCard = function(self)
        local result = ffi.getprivate(self, "Card") return result
    end,
    GetCoins = function(self)
        local result = ffi.getprivate(self, "Coins") return result
    end,
    GetCollectibles = function(self)
        return ffichecks.vectortotable(ffi.getprivate(self, "CollectiblesFirst"), ffi.getprivate(self, "CollectiblesLast"), 4)
    end,
    GetCostumeID = function(self)
        local result = ffi.getprivate(self, "CostumeID") return result
    end,
    GetCostumeSuffix = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "CostumeSuffixName"))
    end,
    GetExtraPortraitPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "ExtraPortraitPath"))
    end,
    GetKeys = function(self)
        local result = ffi.getprivate(self, "Keys") return result
    end,
    GetModdedControlsSprite = function(self)
        local result = ffi.getprivate(self, "ModdedControlsANM2") return result
    end,
    GetModdedCoopMenuSprite = function(self)
        local result = ffi.getprivate(self, "ModdedCoopMenuANM2") return result
    end,
    GetModdedGameOverSprite = function(self)
        local result = ffi.getprivate(self, "ModdedGameOverANM2") return result
    end,
    GetModdedMenuBackgroundSprite = function(self)
        local result = ffi.getprivate(self, "ModdedMenuBackgroundANM2") return result
    end,
    GetModdedMenuPortraitSprite = function(self)
        local result = ffi.getprivate(self, "ModdedMenuPortraitANM2") return result
    end,
    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Name"))
    end,
    GetNameImagePath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "NameImagePath"))
    end,
    GetPill = function(self)
        local result = ffi.getprivate(self, "Pill") return result
    end,
    GetPlayerType = function(self)
        local result = ffi.getprivate(self, "ID") return result
    end,
    GetPocketActive = function(self)
        local result = ffi.getprivate(self, "PocketActiveID") return result
    end,
    GetPortraitPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "PortraitPath"))
    end,
    GetRedHearts = function(self)
        local result = ffi.getprivate(self, "Heart") return result
    end,
    GetSkinColor = function(self)
        local result = ffi.getprivate(self, "SkinColor") return result
    end,
    GetSkinPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "SkinPath"))
    end,
    GetSoulHearts = function(self)
        local result = ffi.getprivate(self, "Armor") return result
    end,
    GetTaintedCounterpart = function(self)
        local result = repentogon.L_EntityConfigPlayer_GetTaintedCounterpart(self) return result
    end,
    GetTrinket = function(self)
        local result = ffi.getprivate(self, "Trinket") return result
    end,
    IsHidden = function(self)
        return ffi.getprivate(self, "Hidden") or HIDDEN_VANILLA_CHARACTERS[ffi.getprivate(self, "ID")] == true
    end,
    IsTainted = function(self)
        local id = ffi.getprivate(self, "ID")
        return (id >= 21 and id <= 40) or ffichecks.stdstring(ffi.getprivate(self, "BSkinParentName")) ~= ""
    end,
}

setmetatable(EntityConfigPlayerMT, { __index = function() end })
EntityConfigPlayerMT.__index = EntityConfigPlayerMT

ffi.metatype("struct EntityConfigPlayer", EntityConfigPlayerMT)

EntityConfigPlayer = setmetatable({}, {
    __class = EntityConfigPlayerMT,
})
