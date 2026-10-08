ffi.cdef [[
    struct HistoryHUDItemData {
        private struct HistoryItem HistoryItem : 0xc;
    } : 0x28;

    struct HistoryHUDPlayer {
        private struct EntityPlayer* Player : 0x0;
        private struct HistoryHUDItemData* ItemsFirst : 0x8;
        private struct HistoryHUDItemData* ItemsLast : 0xc;
    } : 0x14;

    struct HistoryHUD {
        private struct HistoryHUDPlayer Players[2] : 0x0;
    } : 0x2c;
    typedef struct HistoryHUD* HistoryHUDPtr;

    struct HistoryHUDItem {
        private int PlayerSlot;
        private int Index;
        private struct HistoryHUDItemData Data;
    };

    struct HistoryHUD* L_HistoryHUD_Get();
    void L_HistoryHUD_GetPosition(struct HistoryHUD*, struct Vector*);
    int L_HistoryHUD_GetNumVisibleItems(struct HistoryHUD*);
    void L_HistoryHUD_GetItemRenderOffset(struct HistoryHUD*, int, int, struct Vector*);
]]

local repentogon = ffidll
local ffi = ffi

local TRINKET_ID_MASK = 0x7fff

local HistoryItemT = ffi.typeof("struct HistoryItem")

local function GetRenderOffset(historyHUD, playerSlot, index)
    local offset = Vector(0, 0)
    repentogon.L_HistoryHUD_GetItemRenderOffset(historyHUD, playerSlot, index, offset)
    return offset
end

local function GetHistoryItem(self)
    return ffi.getprivate(ffi.getprivate(self, "Data"), "HistoryItem")
end

local HistoryHUDItemMT
HistoryHUDItemMT = {
    __type = "HistoryHUDItem",
    GetHistoryItem = function(self)
        return HistoryItemT(GetHistoryItem(self))
    end,
    GetItemID = function(self)
        return ffi.getprivate(GetHistoryItem(self), "ItemID")
    end,
    GetRenderOffset = function(self)
        return GetRenderOffset(repentogon.L_HistoryHUD_Get(), ffi.getprivate(self, "PlayerSlot"), ffi.getprivate(self, "Index"))
    end,
    GetTime = function(self)
        return ffi.getprivate(GetHistoryItem(self), "Time")
    end,
    IsTrinket = function(self)
        return ffi.getprivate(GetHistoryItem(self), "Trinket")
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Index") < repentogon.L_HistoryHUD_GetNumVisibleItems(repentogon.L_HistoryHUD_Get())
    end,
}

setmetatable(HistoryHUDItemMT, { __index = function() end })
HistoryHUDItemMT.__index = HistoryHUDItemMT

local HistoryHUDItemT = ffi.metatype("struct HistoryHUDItem", HistoryHUDItemMT)

HistoryHUDItem = setmetatable({}, {
    __class = HistoryHUDItemMT,
})

local function CheckPlayerIndex(playerIdx, level)
    playerIdx = ffichecks.checkinteger(1, playerIdx, level)
    if playerIdx < 0 or playerIdx > 1 then
        ffichecks.argerror(1, string.format("invalid HistoryHUDPlayer index %d, must be 0 or 1", playerIdx), level)
    end
    return playerIdx
end

local function GetHistoryHUDItems(self, playerIdx, includeCollectibles, includeTrinkets, includeNotVisible, offsetsOnly, idFilter)
    local player = ffi.getprivate(self, "Players")[playerIdx]
    local numVisibleItems = repentogon.L_HistoryHUD_GetNumVisibleItems(self)
    local result = {}

    if ffi.getprivate(player, "Player") ~= nil then
        local first = ffi.getprivate(player, "ItemsFirst")
        local count = ffichecks.vectorsize(first, ffi.getprivate(player, "ItemsLast"), ffi.sizeof("struct HistoryHUDItemData"))
        for i = 0, count - 1 do
            if not includeNotVisible and i >= numVisibleItems then
                break
            end

            local item = first[i]
            local historyItem = ffi.getprivate(item, "HistoryItem")
            local isTrinket = ffi.getprivate(historyItem, "Trinket")
            if (isTrinket and includeTrinkets) or (not isTrinket and includeCollectibles) then
                local id = ffi.getprivate(historyItem, "ItemID")
                if isTrinket then
                    id = id & TRINKET_ID_MASK
                end

                if not idFilter or next(idFilter) == nil or idFilter[id] then
                    if offsetsOnly then
                        result[#result + 1] = GetRenderOffset(self, playerIdx, i)
                    else
                        result[#result + 1] = HistoryHUDItemT(playerIdx, i, item)
                    end
                end
            end
        end
    end

    return result
end

local function GetFilteredItems(self, trinkets, offsetsOnly, playerIdx, ids, includeNotVisible)
    playerIdx = CheckPlayerIndex(playerIdx, 4)

    local filter = {}
    if type(ids) == "table" then
        for _, v in pairs(ids) do
            if math.type(v) == "integer" then
                filter[trinkets and (v & TRINKET_ID_MASK) or v] = true
            end
        end
    elseif ids ~= nil then
        ids = ffichecks.checkinteger(2, ids, 3)
        filter[trinkets and (ids & TRINKET_ID_MASK) or ids] = true
    end

    return GetHistoryHUDItems(self, playerIdx, not trinkets, trinkets, ffichecks.optboolean(includeNotVisible, false), offsetsOnly, filter)
end

local HistoryHUDMT
HistoryHUDMT = {
    __type = "HistoryHUD",
    GetCollectibleOffsets = function(self, playerIdx, ids, includeNotVisible)
        return GetFilteredItems(self, false, true, playerIdx, ids, includeNotVisible)
    end,
    GetCollectibles = function(self, playerIdx, ids, includeNotVisible)
        return GetFilteredItems(self, false, false, playerIdx, ids, includeNotVisible)
    end,
    GetItems = function(self, playerIdx, includeNotVisible)
        playerIdx = CheckPlayerIndex(playerIdx, 3)
        return GetHistoryHUDItems(self, playerIdx, true, true, ffichecks.optboolean(includeNotVisible, false), false, nil)
    end,
    GetPlayer = function(self, playerIdx)
        playerIdx = CheckPlayerIndex(playerIdx, 3)
        return ffi.getprivate(ffi.getprivate(self, "Players")[playerIdx], "Player")
    end,
    GetPosition = function(self)
        local position = Vector(0, 0)
        repentogon.L_HistoryHUD_GetPosition(self, position)
        return position
    end,
    GetTrinketOffsets = function(self, playerIdx, ids, includeNotVisible)
        return GetFilteredItems(self, true, true, playerIdx, ids, includeNotVisible)
    end,
    GetTrinkets = function(self, playerIdx, ids, includeNotVisible)
        return GetFilteredItems(self, true, false, playerIdx, ids, includeNotVisible)
    end,
}

setmetatable(HistoryHUDMT, { __index = function() end })
HistoryHUDMT.__index = HistoryHUDMT

ffi.metatype("struct HistoryHUD", HistoryHUDMT)

HistoryHUD = setmetatable({}, {
    __class = HistoryHUDMT,
})

