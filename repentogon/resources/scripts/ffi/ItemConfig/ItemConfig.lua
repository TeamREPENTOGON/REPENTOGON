ffi.cdef [[
    struct ItemConfig {
        private struct ItemConfigList CollectibleList : 0x0;
        private struct ItemConfigList TrinketList : 0xc;
        private struct ItemConfigList NullItemList : 0x18;
        private struct CardConfigList CardList : 0x24;
        private struct PillConfigList PillEffectList : 0x30;
    } : 0x124;
    typedef struct ItemConfig* ItemConfigPtr;

    struct ItemConfig* L_ItemConfig_Get();
    struct ItemConfigItem* L_ItemConfig_GetCollectible(struct ItemConfig*, int);
    struct ItemConfigItem* L_ItemConfig_GetNullItem(struct ItemConfig*, int);
    struct ItemConfigItem* L_ItemConfig_GetTrinket(struct ItemConfig*, int);
    struct ItemConfigCard* L_ItemConfig_GetCard(struct ItemConfig*, int);
    struct ItemConfigPillEffect* L_ItemConfig_GetPillEffect(struct ItemConfig*, int);
    struct ItemConfigList* L_ItemConfig_GetTaggedItems(struct ItemConfig*, uint64_t);
    int L_ItemConfig_GetItemsWithCustomTag(struct ItemConfig*, const char*, struct ItemConfigItem**);
    bool L_ItemConfig_CanRerollCollectible(int);
]]

local ffi = ffi
local repentogon = ffidll

local ItemConfigMT
ItemConfigMT = {
    __type = "ItemConfig",

    CanRerollCollectible = function(self, id)
        if id == nil then
            id = self
        end
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_CanRerollCollectible(id) return result
    end,
    GetCard = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_GetCard(self, id) return result
    end,
    GetCards = function(self)
        local result = ffi.getprivate(self, "CardList") return result
    end,
    GetCollectible = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_GetCollectible(self, id) return result
    end,
    GetCollectibles = function(self)
        local result = ffi.getprivate(self, "CollectibleList") return result
    end,
    GetItemsWithCustomTag = function(self, tag)
        tag = ffichecks.checkstring(1, tag)
        local result = {}
        local count = repentogon.L_ItemConfig_GetItemsWithCustomTag(self, tag, nil)
        if count > 0 then
            local items = ffi.new("struct ItemConfigItem*[?]", count)
            repentogon.L_ItemConfig_GetItemsWithCustomTag(self, tag, items)
            for i = 1, count do
                result[i] = items[i - 1]
            end
        end
        return result
    end,
    GetNullItem = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_GetNullItem(self, id) return result
    end,
    GetNullItems = function(self)
        local result = ffi.getprivate(self, "NullItemList") return result
    end,
    GetPillEffect = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_GetPillEffect(self, id) return result
    end,
    GetPillEffects = function(self)
        local result = ffi.getprivate(self, "PillEffectList") return result
    end,
    GetTaggedItems = function(self, tags)
        tags = ffichecks.checknumber(1, tags)
        local list = repentogon.L_ItemConfig_GetTaggedItems(self, tags)
        return ffichecks.vectortotable(ffi.getprivate(list, "Begin"), ffi.getprivate(list, "End"), 4)
    end,
    GetTrinket = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_ItemConfig_GetTrinket(self, id) return result
    end,
    GetTrinkets = function(self)
        local result = ffi.getprivate(self, "TrinketList") return result
    end,
}

setmetatable(ItemConfigMT, { __index = function() end })
ItemConfigMT.__index = ItemConfigMT

ffi.metatype("struct ItemConfig", ItemConfigMT)

ItemConfigConfig = setmetatable({}, {__class = ItemConfigMT})

rawset(Isaac, "GetItemConfig", function()
    local result = repentogon.L_ItemConfig_Get() return result
end)

ItemConfig = {
    Card = ItemConfigCard,
    Config = ItemConfigConfig,
    Costume = ItemConfigCostume,
    Item = ItemConfigItem,
    PillEffect = ItemConfigPillEffect,
}

local REMOVED_COLLECTIBLES = { [43] = true, [61] = true, [235] = true }

rawset(ItemConfigConfig, "GetCollectible", function(id)
    id = ffichecks.checkinteger(1, id)
    local result = repentogon.L_ItemConfig_GetCollectible(repentogon.L_ItemConfig_Get(), id) return result
end)
rawset(ItemConfigConfig, "GetNullItem", function(id)
    id = ffichecks.checkinteger(1, id)
    local result = repentogon.L_ItemConfig_GetNullItem(repentogon.L_ItemConfig_Get(), id) return result
end)
rawset(ItemConfigConfig, "GetTrinket", function(id)
    id = ffichecks.checkinteger(1, id)
    local result = repentogon.L_ItemConfig_GetTrinket(repentogon.L_ItemConfig_Get(), id) return result
end)
rawset(ItemConfigConfig, "IsValidCollectible", function(id)
    id = ffichecks.checkinteger(1, id)
    if id <= 0 or REMOVED_COLLECTIBLES[id] then
        return false
    end
    local config = repentogon.L_ItemConfig_Get()
    if id >= #ffi.getprivate(config, "CollectibleList") then
        return false
    end
    return repentogon.L_ItemConfig_GetCollectible(config, id) ~= nil
end)
rawset(ItemConfigConfig, "ShouldAddCostumeOnPickup", function(item)
    ffichecks.checkcdata(1, item, "ItemConfigItem")
    return item.Type ~= 0 and item.AddCostumeOnPickup
end)
