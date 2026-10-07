ffi.cdef[[
	struct ItemOverlay {
        int State : 0x0;
        int OverlayID : 0x4;
        struct Sprite Sprite : 0x8;
        int Delay : 0x1144;
        private struct EntityPlayer* Player : 0x1148;
        struct Sprite MegaMushPlayerSprite : 0x114c;
	} : 0x1260;

    struct ItemOverlay* L_ItemOverlay_Get();
    void L_ItemOverlay_Show(int, int, struct EntityPlayer*);
]]

local ffi = ffi
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
        return ffi.getprivate(GetItemOverlay(), "Player")
    end,
    GetSprite = function()
        return GetItemOverlay().Sprite
    end,
    Show = function(giantbookID, delay, player)
        ffichecks.checkinteger(1, giantbookID)
        delay = delay or 0
        ffichecks.checkinteger(2, delay)
        ffichecks.checkcdata(3, player, "EntityPlayer", true)
        repentogon.L_ItemOverlay_Show(giantbookID, delay, player)
    end,
}
