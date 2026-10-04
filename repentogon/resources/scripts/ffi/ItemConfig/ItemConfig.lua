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
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_CanRerollCollectible(id)
    end,
    GetCard = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_GetCard(self, id)
    end,
    GetCards = function(self)
        return ffi.getprivate(self, "CardList")
    end,
    GetCollectible = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_GetCollectible(self, id)
    end,
    GetCollectibles = function(self)
        return ffi.getprivate(self, "CollectibleList")
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
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_GetNullItem(self, id)
    end,
    GetNullItems = function(self)
        return ffi.getprivate(self, "NullItemList")
    end,
    GetPillEffect = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_GetPillEffect(self, id)
    end,
    GetPillEffects = function(self)
        return ffi.getprivate(self, "PillEffectList")
    end,
    GetTaggedItems = function(self, tags)
        ffichecks.checknumber(1, tags)
        local list = repentogon.L_ItemConfig_GetTaggedItems(self, tags)
        return ffichecks.vectortotable(ffi.getprivate(list, "Begin"), ffi.getprivate(list, "End"), 4)
    end,
    GetTrinket = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_ItemConfig_GetTrinket(self, id)
    end,
    GetTrinkets = function(self)
        return ffi.getprivate(self, "TrinketList")
    end,
}

setmetatable(ItemConfigMT, { __index = function() end })
ItemConfigMT.__index = ItemConfigMT

ffi.metatype("struct ItemConfig", ItemConfigMT)

ItemConfigConfig = setmetatable({}, {__class = ItemConfigMT})

rawset(Isaac, "GetItemConfig", function()
    return repentogon.L_ItemConfig_Get()
end)

rawset(ItemConfig.Config, "ShouldAddCostumeOnPickup", function(item)
    ffichecks.checkcdata(1, item, "ItemConfigItem")
    return item.Type ~= 0 and item.AddCostumeOnPickup
end)
