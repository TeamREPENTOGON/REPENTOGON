ffi.cdef [[
    struct HistoryItem {
        private int Time : 0x0;
        private bool Trinket : 0x4;
        private int ItemID : 0x8;
        private int LevelStage : 0xc;
        private int StageType : 0x10;
        private int RoomType : 0x14;
        private int ItemPoolType : 0x18;
    } : 0x1c;
    typedef struct HistoryItem* HistoryItemPtr;

    struct History {
        private struct HistoryItem* First : 0x0;
        private struct HistoryItem* Last : 0x4;
        private struct HistoryItem* End : 0x8;
    } : 0x14;
    typedef struct History* HistoryPtr;

    bool L_History_RemoveHistoryItemByIndex(struct History*, int);
]]

local repentogon = ffidll
local ffi = ffi

local HistoryItemMT
HistoryItemMT = {
    __type = "HistoryItem",
    GetItemID = function(self)
        local result = ffi.getprivate(self, "ItemID") return result
    end,
    GetItemPoolType = function(self)
        local result = ffi.getprivate(self, "ItemPoolType") return result
    end,
    GetLevelStage = function(self)
        local result = ffi.getprivate(self, "LevelStage") return result
    end,
    GetRoomType = function(self)
        local result = ffi.getprivate(self, "RoomType") return result
    end,
    GetStageType = function(self)
        local result = ffi.getprivate(self, "StageType") return result
    end,
    GetTime = function(self)
        local result = ffi.getprivate(self, "Time") return result
    end,
    IsTrinket = function(self)
        local result = ffi.getprivate(self, "Trinket") return result
    end,
}

setmetatable(HistoryItemMT, { __index = function() end })
HistoryItemMT.__index = HistoryItemMT

local HistoryItemT = ffi.metatype("struct HistoryItem", HistoryItemMT)

HistoryItem = setmetatable({}, {
    __class = HistoryItemMT,
})

----------
local TRINKET_ID_MASK = 0x7fff

local function CopyItems(self, filter)
    local first = ffi.getprivate(self, "First")
    local result = {}
    for i = 0, ffichecks.vectorsize(first, ffi.getprivate(self, "Last"), ffi.sizeof(HistoryItemT)) - 1 do
        local item = first[i]
        if not filter or filter(item) then
            result[#result + 1] = HistoryItemT(item)
        end
    end
    return result
end

local function SearchHistory(self, trinkets, ids)
    local filterIDs = {}
    if type(ids) == "table" then
        for _, v in pairs(ids) do
            if math.type(v) == "integer" then
                filterIDs[v] = true
                filterIDs[trinkets and (v & TRINKET_ID_MASK) or v] = true
            end
        end
    elseif ids ~= nil then
        ids = ffichecks.checkinteger(1, ids, 3)
        filterIDs[trinkets and (ids & TRINKET_ID_MASK) or ids] = true
    end
    local anyFilter = next(filterIDs) ~= nil

    return CopyItems(self, function(item)
        local isTrinket = ffi.getprivate(item, "Trinket")
        if isTrinket ~= trinkets then
            return false
        end
        local id = ffi.getprivate(item, "ItemID")
        if isTrinket then
            id = id & TRINKET_ID_MASK
        end
        return not anyFilter or filterIDs[id] == true
    end)
end

local HistoryMT
HistoryMT = {
    __type = "History",
    GetCollectiblesHistory = function(self)
        return CopyItems(self)
    end,
    RemoveHistoryItemByIndex = function(self, index)
        index = ffichecks.checkinteger(1, index)
        local result = repentogon.L_History_RemoveHistoryItemByIndex(self, index) return result
    end,
    SearchCollectibles = function(self, ids)
        return SearchHistory(self, false, ids)
    end,
    SearchTrinkets = function(self, ids)
        return SearchHistory(self, true, ids)
    end,
}

setmetatable(HistoryMT, { __index = function() end })
HistoryMT.__index = HistoryMT

ffi.metatype("struct History", HistoryMT)

History = setmetatable({}, {
    __class = HistoryMT,
})
