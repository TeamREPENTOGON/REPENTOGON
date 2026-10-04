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
    return ffi.getprivate(self, "DefaultSkinColor")
end

local function GetSkinColor(self)
    return ffi.getprivate(self, "SkinColor")
end

local CostumeSpriteDescMT
CostumeSpriteDescMT = {
    __type = "CostumeSpriteDesc",
    CanOverwriteColor = function(self)
        return ffi.getprivate(self, "OverwriteColor")
    end,
    GetBodyColor = GetSkinColor,
    GetDefaultSkinColor = GetDefaultSkinColor,
    GetHeadColor = GetDefaultSkinColor,
    GetItemConfig = function(self)
        return ffi.getprivate(self, "Item")
    end,
    GetPlayerType = function(self)
        return ffi.getprivate(self, "PlayerType")
    end,
    GetPriority = function(self)
        return ffi.getprivate(self, "Priority")
    end,
    GetSkinColor = GetSkinColor,
    GetSprite = function(self)
        return ffi.getprivate(self, "Sprite")
    end,
    HasOverlay = function(self)
        return ffi.getprivate(self, "Overlay")
    end,
    HasSkinAlt = function(self)
        return ffi.getprivate(self, "SkinAlt")
    end,
    IsFlying = function(self)
        return ffi.getprivate(self, "Flying")
    end,
    IsItemAnimPlaying = function(self)
        return ffi.getprivate(self, "ItemAnimPlay")
    end,
    IsItemStateOnly = function(self)
        return ffi.getprivate(self, "ItemStateOnly")
    end,
}

setmetatable(CostumeSpriteDescMT, { __index = function() end })
CostumeSpriteDescMT.__index = CostumeSpriteDescMT

ffi.metatype("struct CostumeSpriteDesc", CostumeSpriteDescMT)

CostumeSpriteDesc = setmetatable({}, {
    __class = CostumeSpriteDescMT,
})
