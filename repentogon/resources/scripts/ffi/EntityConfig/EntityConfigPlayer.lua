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
        return ffi.getprivate(self, "Shooting")
    end,
    GetAchievementID = function(self)
        return ffi.getprivate(self, "Achievement")
    end,
    GetBirthrightDescription = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BirthrightDescription"))
    end,
    GetBlackHearts = function(self)
        return ffi.getprivate(self, "BlackHeart")
    end,
    GetBombs = function(self)
        return ffi.getprivate(self, "Bombs")
    end,
    GetBrokenHearts = function(self)
        return ffi.getprivate(self, "BrokenHeart")
    end,
    GetCard = function(self)
        return ffi.getprivate(self, "Card")
    end,
    GetCoins = function(self)
        return ffi.getprivate(self, "Coins")
    end,
    GetCollectibles = function(self)
        return ffichecks.vectortotable(ffi.getprivate(self, "CollectiblesFirst"), ffi.getprivate(self, "CollectiblesLast"), 4)
    end,
    GetCostumeID = function(self)
        return ffi.getprivate(self, "CostumeID")
    end,
    GetCostumeSuffix = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "CostumeSuffixName"))
    end,
    GetExtraPortraitPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "ExtraPortraitPath"))
    end,
    GetKeys = function(self)
        return ffi.getprivate(self, "Keys")
    end,
    GetModdedControlsSprite = function(self)
        return ffi.getprivate(self, "ModdedControlsANM2")
    end,
    GetModdedCoopMenuSprite = function(self)
        return ffi.getprivate(self, "ModdedCoopMenuANM2")
    end,
    GetModdedGameOverSprite = function(self)
        return ffi.getprivate(self, "ModdedGameOverANM2")
    end,
    GetModdedMenuBackgroundSprite = function(self)
        return ffi.getprivate(self, "ModdedMenuBackgroundANM2")
    end,
    GetModdedMenuPortraitSprite = function(self)
        return ffi.getprivate(self, "ModdedMenuPortraitANM2")
    end,
    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Name"))
    end,
    GetNameImagePath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "NameImagePath"))
    end,
    GetPill = function(self)
        return ffi.getprivate(self, "Pill")
    end,
    GetPlayerType = function(self)
        return ffi.getprivate(self, "ID")
    end,
    GetPocketActive = function(self)
        return ffi.getprivate(self, "PocketActiveID")
    end,
    GetPortraitPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "PortraitPath"))
    end,
    GetRedHearts = function(self)
        return ffi.getprivate(self, "Heart")
    end,
    GetSkinColor = function(self)
        return ffi.getprivate(self, "SkinColor")
    end,
    GetSkinPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "SkinPath"))
    end,
    GetSoulHearts = function(self)
        return ffi.getprivate(self, "Armor")
    end,
    GetTaintedCounterpart = function(self)
        return repentogon.L_EntityConfigPlayer_GetTaintedCounterpart(self)
    end,
    GetTrinket = function(self)
        return ffi.getprivate(self, "Trinket")
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
