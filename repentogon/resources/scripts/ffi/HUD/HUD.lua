ffi.cdef [[
    struct HUD {
        private struct PlayerHUD PlayerHUDs[8] : 0x0;
        private struct Sprite ChargeBarSprite : 0x3798;
        private struct Sprite HeartsSprite : 0x39c0;
        private struct Sprite PickupHUDSprite : 0x3ad4;
        private struct Sprite CardsPillsSprite : 0x3be8;
        private struct HUDMessage MessageMain : 0x3cfc;
        private struct HUDMessage MessageStack[6] : 0x3ef0;
        private struct HUDMessage MessagePlayerHUD[4] : 0x4aa8;
        private struct Sprite FortuneSprite : 0x5278;
        private float BossHPBarFill : 0x54b0;
        private bool Visible : 0x54e4;
        private struct Sprite CoopMenuSprite : 0x54f4;
        private struct Sprite InventorySprite : 0x5608;
        private struct Sprite CraftingTableSprite : 0x571c;
        private struct Sprite PoopSpellsSprite : 0x5830;
        private struct HistoryHUD HistoryHUD : 0x5c64;
    } : 0x5c90;
    typedef struct HUD* HUDPtr;

    void L_HUD_AssignPlayerHUDs(struct HUD*);
    void L_HUD_Update(struct HUD*);
    void L_HUD_PostUpdate(struct HUD*);
    void L_HUD_Render(struct HUD*);
    void L_HUD_ShowFortuneText(struct HUD*, const char**, int);
    void L_HUD_ShowStackedItemText(struct HUD*, const char*, const char*, bool, bool);
]]

local repentogon = ffidll
local ffi = ffi

local cfuncs = {
    FlashChargeBar = __Lua_HUD_FlashChargeBar,
    FlashRedHearts = __Lua_HUD_FlashRedHearts,
    InvalidateActiveItem = __Lua_HUD_InvalidateActiveItem,
    InvalidateCraftingItem = __Lua_HUD_InvalidateCraftingItem,
    ShowItemTextPlayer = __Lua_HUD_ShowItemTextPlayer,
}

local MAX_FORTUNE_LINES = 32

local function IsString(value)
    return type(value) == "string" or type(value) == "number"
end

local function OptString(value)
    if value == nil then
        return ""
    end
    return tostring(value)
end

local function GetMessage(self, field, index, max)
    index = ffichecks.optnumber(index, 0)
    if index < 0 or index > max then
        ffichecks.argerror(1, string.format("invalid HUD message index %d", index), 3)
    end
    return ffi.getprivate(self, field)[index]
end

local HUDMT
HUDMT = {
    __type = "HUD",
    AssignPlayerHUDs = function(self)
        repentogon.L_HUD_AssignPlayerHUDs(self)
    end,
    FlashChargeBar = function(self, player, slot)
        cfuncs.FlashChargeBar(self, player, slot or 0)
    end,
    FlashRedHearts = function(self, player)
        cfuncs.FlashRedHearts(self, player)
    end,
    GetBossHPBarFill = function(self)
        return ffi.getprivate(self, "BossHPBarFill")
    end,
    GetCardsPillsSprite = function(self)
        return ffi.getprivate(self, "CardsPillsSprite")
    end,
    GetChargeBarSprite = function(self)
        return ffi.getprivate(self, "ChargeBarSprite")
    end,
    GetCoopMenuSprite = function(self)
        return ffi.getprivate(self, "CoopMenuSprite")
    end,
    GetCoopPlayerMessage = function(self, index)
        return GetMessage(self, "MessagePlayerHUD", index, 3)
    end,
    GetCraftingSprite = function(self)
        return ffi.getprivate(self, "CraftingTableSprite")
    end,
    GetFortuneSprite = function(self)
        return ffi.getprivate(self, "FortuneSprite")
    end,
    GetHeartsSprite = function(self)
        return ffi.getprivate(self, "HeartsSprite")
    end,
    GetHistoryHUD = function(self)
        return ffi.getprivate(self, "HistoryHUD")
    end,
    GetInventorySprite = function(self)
        return ffi.getprivate(self, "InventorySprite")
    end,
    GetMainMessage = function(self)
        return ffi.getprivate(self, "MessageMain")
    end,
    GetPickupsHUDSprite = function(self)
        return ffi.getprivate(self, "PickupHUDSprite")
    end,
    GetPlayerHUD = function(self, index)
        index = ffichecks.optnumber(index, 0)
        if index < 0 or index > 7 then
            ffichecks.argerror(1, string.format("invalid index %d, value must be between 0 and 7", index))
        end
        return ffi.getprivate(self, "PlayerHUDs")[index]
    end,
    GetPoopSpellSprite = function(self)
        return ffi.getprivate(self, "PoopSpellsSprite")
    end,
    GetStackedMessage = function(self, index)
        return GetMessage(self, "MessageStack", index, 5)
    end,
    GetStreakSprite = function(self)
        return ffi.getprivate(ffi.getprivate(self, "MessageMain"), "Sprite")
    end,
    InvalidateActiveItem = function(self, player, slot)
        cfuncs.InvalidateActiveItem(self, player, slot or 0)
    end,
    InvalidateCraftingItem = function(self, player)
        cfuncs.InvalidateCraftingItem(self, player)
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Visible")
    end,
    PostUpdate = function(self)
        repentogon.L_HUD_PostUpdate(self)
    end,
    Render = function(self)
        repentogon.L_HUD_Render(self)
    end,
    SetBossHPBarFill = function(self, fill)
        ffichecks.checknumber(1, fill)
        ffi.setprivate(self, "BossHPBarFill", fill)
    end,
    SetVisible = function(self, visible)
        ffi.setprivate(self, "Visible", not not visible)
    end,
    ShowFortuneText = function(self, ...)
        local lines = {}
        for i = 1, math.min(select("#", ...), MAX_FORTUNE_LINES) do
            local line = select(i, ...)
            if type(line) ~= "string" then
                break
            end
            lines[i] = line
        end
        local buffer = ffi.new("const char*[?]", #lines + 1, lines)
        repentogon.L_HUD_ShowFortuneText(self, buffer, #lines)
    end,
    ShowItemText = function(self, ...)
        local first, second, third, fourth = ...
        if IsString(first) or IsString(second) then
            repentogon.L_HUD_ShowStackedItemText(self, OptString(first), OptString(second),
                ffichecks.optboolean(third, false), ffichecks.optboolean(fourth, true))
        else
            cfuncs.ShowItemTextPlayer(self, first, second, ffichecks.optboolean(third, true))
        end
    end,
    Update = function(self)
        repentogon.L_HUD_Update(self)
    end,
}

setmetatable(HUDMT, {
    __index = function() end,
    __call = function(t, _, k) return t[k] end,
})
HUDMT.__index = HUDMT

ffi.metatype("struct HUD", HUDMT)

HUD = setmetatable({}, {
    __class = HUDMT,
})

__Lua_HUD_FlashChargeBar = nil
__Lua_HUD_FlashRedHearts = nil
__Lua_HUD_InvalidateActiveItem = nil
__Lua_HUD_InvalidateCraftingItem = nil
__Lua_HUD_ShowItemTextPlayer = nil