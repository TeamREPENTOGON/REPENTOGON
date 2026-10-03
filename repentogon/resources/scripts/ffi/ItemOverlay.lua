ffi.cdef[[
	struct ItemOverlay {
        int State : 0x0;
        int OverlayID : 0x4;
        struct Sprite Sprite : 0x8;
        int Delay : 0x1144;
        struct Sprite MegaMushPlayerSprite : 0x114c;
	} : 0x1260;

    struct ItemOverlay* L_ItemOverlay_Get();
]]

local cfuncs = {
    GetPlayer = __Lua_ItemOverlay_GetPlayer,
    Show = __Lua_ItemOverlay_Show,
}

local repentogon = ffidll

local ItemOverlayMT
ItemOverlayMT = {
    __type = "ItemOverlay",
}

setmetatable(ItemOverlayMT, { __index = function() end })
ItemOverlayMT.__index = ItemOverlayMT

local ItemOverlayT = ffi.metatype("struct ItemOverlay", ItemOverlayMT)

local function GetItemOverlay()
    return repentogon.L_ItemOverlay_Get()
end

ItemOverlay = {
    GetDelay = function()
        return GetItemOverlay().Delay
    end,
    GetMegaMushPlayerSprite = function()
        return GetItemOverlay().MegaMushPlayerSprite
    end,
    GetOverlayID = function()
        return GetItemOverlay().OverlayID
    end,
    GetPlayer = function()
        return cfuncs.GetPlayer()
    end,
    GetSprite = function()
        return GetItemOverlay().Sprite
    end,
    Show = function(giantbookID, delay, player)
        cfuncs.Show(giantbookID, delay, player)
    end,
}

__Lua_ItemOverlay_GetPlayer = nil
__Lua_ItemOverlay_Show = nil