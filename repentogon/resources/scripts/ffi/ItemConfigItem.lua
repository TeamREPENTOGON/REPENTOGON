ffi.cdef[[
struct ItemConfigItem
{
    int Type;
    int ID;
    char _pad_name[0x18]; // Name
    char _pad_desc[0x18]; // Desc
    char _pad_gfx_file_name[0x18]; // GfxFileName
    int AchievementID;
    int CacheFlags;
    int AddMaxHearts;
    int AddHearts;
    int AddSoulHearts;
    int AddBlackHearts;
    int AddBombs;
    int AddKeys;
    int AddCoins;
    unsigned int MaxCharges;
    unsigned int MaxCooldown;
    short DevilPrice;
    short ShopPrice;
    struct ItemConfigCostume Costume;
    unsigned int ChargeType;
    bool Special;
    bool PassiveCache;
    bool AddCostumeOnPickup;
    bool Hidden;
    bool PersistentEffect;
    bool ClearEffectsOnRemove;
    char _pad[0x2];
    uint64_t Tags;
    int Quality;
    int CraftingQuality;
};

typedef struct ItemConfigItem* ItemConfigItemPtr;

const char* L_ItemConfigItem_GetName(struct ItemConfigItem*);
void L_ItemConfigItem_SetName(struct ItemConfigItem*, const char*);
const char* L_ItemConfigItem_GetDesc(struct ItemConfigItem*);
void L_ItemConfigItem_SetDesc(struct ItemConfigItem*, const char*);
const char* L_ItemConfigItem_GetGfxFileName(struct ItemConfigItem*);
void L_ItemConfigItem_SetGfxFileName(struct ItemConfigItem*, const char*);
bool L_ItemConfigItem_IsAvailable(struct ItemConfigItem*);
int L_ItemConfigItem_GetCustomTags(struct ItemConfigItem*, const char**);   
bool L_ItemConfigItem_HasCustomTag(struct ItemConfigItem*, const char*);
void L_ItemConfigItem_AddCustomTag(struct ItemConfigItem*, const char*);
void L_ItemConfigItem_RemoveCustomTag(struct ItemConfigItem*, const char*);
int L_ItemConfigItem_GetCustomCacheTags(struct ItemConfigItem*, const char**);
int L_ItemConfigItem_HasCustomCacheTag(struct ItemConfigItem*, const char*);
void L_ItemConfigItem_AddCustomCacheTag(struct ItemConfigItem*, const char*);
void L_ItemConfigItem_RemoveCustomCacheTag(struct ItemConfigItem*, const char*);
//uint64_t L_ItemConfigItem_GetTags(struct ItemConfigItem* item);
//void L_ItemConfigItem_SetTags(struct ItemConfigItem* item, uint64_t tags);
]]

local lffi = ffi
local repentogon = ffidll


local getkeys = {
    Name = repentogon.L_ItemConfigItem_GetName,
	Description = repentogon.L_ItemConfigItem_GetDesc,
	GfxFileName = repentogon.L_ItemConfigItem_GetGfxFileName,
    --Tags = repentogon.L_ItemConfigItem_GetTags,
}

local setkeys = {
    Name = repentogon.L_ItemConfigItem_SetName,
	Description = repentogon.L_ItemConfigItem_SetDesc,
	GfxFileName = repentogon.L_ItemConfigItem_SetGfxFileName,
    --Tags = repentogon.L_ItemConfigItem_SetTags,
}

local ItemConfigItemMT
ItemConfigItemMT = {
    __type = "ItemConfigItem",

    __index = function(self, key)
        if getkeys[key] ~= nil then
            return lffi.string(getkeys[key](self))
        end
        return ItemConfigItemMT[key]
    end,

    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        end
        error("cannot set '" .. tostring(key) .. "'")
    end,

    -- Some of these were trivial reimplementations, so I didn't bother with sigs or functions for them on the C side.
    AddCustomCacheTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        repentogon.L_ItemConfigItem_AddCustomCacheTag(self, tag)
    end,
    AddCustomTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        repentogon.L_ItemConfigItem_AddCustomTag(self, tag)
    end,
    GetCustomCacheTags = function(self)
        local n = repentogon.L_ItemConfigItem_GetCustomCacheTags(self, nil)
        local t = {}
        if n > 0 then
            local arr = lffi.new("const char*[?]", n)
            repentogon.L_ItemConfigItem_GetCustomCacheTags(self, arr)
            for i = 0, n - 1 do
                t[i + 1] = lffi.string(arr[i])
            end
        end
        return t
    end,
    GetCustomTags = function(self)
        local n = repentogon.L_ItemConfigItem_GetCustomTags(self, nil)
        local t = {}
        if n > 0 then
            local arr = lffi.new("const char*[?]", n)
            repentogon.L_ItemConfigItem_GetCustomTags(self, arr)
            for i = 0, n - 1 do
                t[i + 1] = lffi.string(arr[i])
            end
        end
        return t
    end,
    HasCustomCacheTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        return repentogon.L_ItemConfigItem_HasCustomCacheTag(self, tag)
    end,
    HasCustomTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        return repentogon.L_ItemConfigItem_HasCustomTag(self, tag)
    end,
    HasTags = function(self, tags)
        ffichecks.checknumber(2, tags)
        --if tags & repentogon.L_ItemConfigItem_GetTags(self) ~= 0 then 
        if tags & self.Tags ~= 0 then
            return true
        end
        return false
    end,
    IsAvailable = function(self)
        return repentogon.L_ItemConfigItem_IsAvailable(self)
    end,
    IsCollectible = function(self)
        local type = self.Type
        if type ~= 1 and type ~= 3 and type ~= 4 then
            return false
        end
        return true
    end,
    IsNull = function(self)
        if self.Type == 0 then
            return true
        end
        return false
    end,
    IsTrinket = function(self)
        if self.Type == 2 then
            return true
        end
        return false
    end,
    RemoveCustomCacheTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        repentogon.L_ItemConfigItem_RemoveCustomCacheTag(self, tag)
    end,
    RemoveCustomTag = function(self, tag)
        ffichecks.checkstring(2, tag)
        repentogon.L_ItemConfigItem_RemoveCustomTag(self, tag)
    end,
}

ffi.metatype("struct ItemConfigItem", ItemConfigItemMT)

ItemConfigItem = setmetatable({}, {__class = ItemConfigItemMT})