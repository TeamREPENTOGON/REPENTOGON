ffi.cdef[[
    struct Minimap {
        int State : 0x0;
        uint32_t HoldTime : 0x8;
        struct Sprite ItemIconsSprite : 0x2d8;
        struct Sprite IconsSprite : 0x500;
        int ShakeDuration : 0x61c;
        struct Vector ShakeOffset : 0x620;
    } : 0x628;

    struct Minimap* L_Minimap_Get();
    void L_Minimap_GetDisplayedSize(struct Vector*);
    void L_Minimap_Refresh();
]]

local repentogon = ffidll

local MinimapMT
MinimapMT = {
    __type = "Minimap",
}

setmetatable(MinimapMT, { __index = function() end })
MinimapMT.__index = MinimapMT

local MinimapT = ffi.metatype("struct Minimap", MinimapMT)

local function GetMinimap()
    return repentogon.L_Minimap_Get()
end

Minimap = {
    GetDisplayedSize = function()
        local v = Vector(0, 0)
            repentogon.L_Minimap_GetDisplayedSize(v)
        return v
    end,
    GetHoldTime = function()
        return GetMinimap().HoldTime
    end,
    GetIconsSprite = function()
        return GetMinimap().IconsSprite
    end,
    GetItemIconsSprite = function()
        return GetMinimap().ItemIconsSprite
    end,
    GetShakeDuration = function()
        return GetMinimap().ShakeDuration
    end,
    GetShakeOffset = function()
        local v = Vector(GetMinimap().ShakeOffset.X, GetMinimap().ShakeOffset.Y)
        return v
    end,
    GetState = function()
        return GetMinimap().State
    end,
    Refresh = function()
        repentogon.L_Minimap_Refresh()
    end,
    SetHoldTime = function(time)
        ffichecks.checkinteger(1, time)
        GetMinimap().HoldTime = time
    end,
    SetShakeDuration = function(duration)
        ffichecks.checkinteger(1, duration)
        GetMinimap().ShakeDuration = duration
    end,
    SetShakeOffset = function(offset)
        ffichecks.checkcdata(1, offset, "Vector")
        GetMinimap().ShakeOffset = offset
    end,
    SetState = function(state)
        ffichecks.checkinteger(1, state)
        GetMinimap().State = state
    end,
}