ffi.cdef [[
    struct CostumeSpriteDesc {
        private struct Sprite Sprite : 0x0;
        private struct ItemConfigItem* Item : 0x114;
        private int Priority : 0x118;
        private bool ItemAnimPlay : 0x11c;
        private bool Flying : 0x11d;
        private bool Overlay : 0x11e;
        private bool SkinAlt : 0x11f;
        private int DefaultSkinColor : 0x120;
        private int SkinColor : 0x124;
        private bool OverwriteColor : 0x128;
        private bool ItemStateOnly : 0x129;
        private int PlayerType : 0x12c;
    } : 0x130;
    typedef struct CostumeSpriteDesc* CostumeSpriteDescPtr;
]]

local ffi = ffi

local function GetDefaultSkinColor(self)
    local result = ffi.getprivate(self, "DefaultSkinColor") return result
end

local function GetSkinColor(self)
    local result = ffi.getprivate(self, "SkinColor") return result
end

local CostumeSpriteDescMT
CostumeSpriteDescMT = {
    __type = "CostumeSpriteDesc",
    CanOverwriteColor = function(self)
        local result = ffi.getprivate(self, "OverwriteColor") return result
    end,
    GetBodyColor = GetSkinColor,
    GetDefaultSkinColor = GetDefaultSkinColor,
    GetHeadColor = GetDefaultSkinColor,
    GetItemConfig = function(self)
        local result = ffi.getprivate(self, "Item") return result
    end,
    GetPlayerType = function(self)
        local result = ffi.getprivate(self, "PlayerType") return result
    end,
    GetPriority = function(self)
        local result = ffi.getprivate(self, "Priority") return result
    end,
    GetSkinColor = GetSkinColor,
    GetSprite = function(self)
        local result = ffi.getprivate(self, "Sprite") return result
    end,
    HasOverlay = function(self)
        local result = ffi.getprivate(self, "Overlay") return result
    end,
    HasSkinAlt = function(self)
        local result = ffi.getprivate(self, "SkinAlt") return result
    end,
    IsFlying = function(self)
        local result = ffi.getprivate(self, "Flying") return result
    end,
    IsItemAnimPlaying = function(self)
        local result = ffi.getprivate(self, "ItemAnimPlay") return result
    end,
    IsItemStateOnly = function(self)
        local result = ffi.getprivate(self, "ItemStateOnly") return result
    end,
}

setmetatable(CostumeSpriteDescMT, { __index = function() end })
CostumeSpriteDescMT.__index = CostumeSpriteDescMT

ffi.metatype("struct CostumeSpriteDesc", CostumeSpriteDescMT)

CostumeSpriteDesc = setmetatable({}, {
    __class = CostumeSpriteDescMT,
})
