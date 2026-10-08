ffi.cdef [[
    struct ItemPool {
        private int LastPool : 0x650;
        private int NumAvailableTrinkets : 0x7a0;
    } : 0x830;
    typedef struct ItemPool* ItemPoolPtr;

    struct ItemPoolItem {
        uint32_t ItemID : 0x0;
        float InitialWeight : 0x4;
        float Weight : 0x8;
        float DecreaseBy : 0xc;
        float RemoveOn : 0x10;
        bool IsUnlocked : 0x14;
        bool IsSpecial : 0x15;
    } : 0x18;

    struct ItemPoolItemList {
        private struct ItemPoolItem* Begin : 0x0;
        private struct ItemPoolItem* End : 0x4;
        private struct ItemPoolItem* Capacity : 0x8;
    } : 0xc;

    bool L_ItemPool_IsPoolValid(int);
    int L_ItemPool_GetPillEffect(struct ItemPool*, unsigned int, struct EntityPlayer*);
    int L_ItemPool_GetNumItemPools();
    bool L_ItemPool_GetCollectible(struct ItemPool*, int, bool, uint32_t, int, uint32_t, int*);
    int L_ItemPool_GetTrinket(struct ItemPool*, bool);
    int L_ItemPool_GetCardEx(struct ItemPool*, unsigned int, int, int, int, bool);
    void L_ItemPool_AddRoomBlacklist(unsigned int);
    int L_ItemPool_GetRandomPool(struct ItemPool*, struct RNG*, bool, const int*, int, bool);
    struct ItemPoolItem* L_ItemPool_PickCollectible(struct ItemPool*, int, bool, struct RNG*, uint32_t);
    int L_ItemPool_GetCollectibleFromList(struct ItemPool*, const int*, int, unsigned int, unsigned int, bool, bool);
    bool L_ItemPool_HasCollectible(struct ItemPool*, int);
    int L_ItemPool_GetCollectibleBits(struct ItemPool*, bool, bool*);
    struct ItemPoolItemList* L_ItemPool_GetPoolList(int);
    bool L_ItemPool_HasTrinket(struct ItemPool*, unsigned int);
    int L_ItemPool_CanSpawnCollectible(struct ItemPool*, int, bool);
    void L_ItemPool_UnidentifyPill(struct ItemPool*, int);
    int L_ItemPool_GetPillColor(struct ItemPool*, int);
    bool L_ItemPool_AddBibleUpgrade(struct ItemPool*, int, int);
    bool L_ItemPool_GetBibleUpgrades(int, int*);
    bool L_ItemPool_ResetCollectible(struct ItemPool*, int);
    int L_ItemPool_GetCollectibleByName(const char*);
    void L_ItemPool_PrintWarning(const char*);
    void L_ItemPool_AddVirtualItem(int, int, float, float, float);
    const char* L_ItemPool_AddTemporaryItem(int, int, float, float, float);
    const char* L_ItemPool_RemoveTemporaryItem(int, int, float, float, float);
    bool L_ItemPool_RemoveCollectible(struct ItemPool*, int, bool, bool);
    bool L_ItemPool_RemoveTrinket(struct ItemPool*, int);
    void L_ItemPool_ResetTrinkets(struct ItemPool*);
    int L_ItemPool_GetCard(struct ItemPool*, unsigned int, bool, bool, bool);
    int L_ItemPool_GetPill(struct ItemPool*, unsigned int);
    void L_ItemPool_ResetRoomBlacklist(struct ItemPool*);
    void L_ItemPool_IdentifyPill(struct ItemPool*, unsigned int);
    bool L_ItemPool_IsPillIdentified(struct ItemPool*, unsigned int);
    int L_ItemPool_ForceAddPillEffect(struct ItemPool*, int);
    int L_ItemPool_GetPoolForRoom(struct ItemPool*, unsigned int, unsigned int);
]]

local ffi = ffi
local repentogon = ffidll

local ITEM_SIZE = ffi.sizeof("struct ItemPoolItem")
local intOut = ffi.new("int[1]")

local function CheckValidPool(index, poolType)
    if not repentogon.L_ItemPool_IsPoolValid(poolType) then
        ffichecks.argerror(index, "Invalid ItemPoolType", 3)
    end
end

local function PoolItemToTable(item)
    return {
        itemID = item.ItemID,
        initialWeight = item.InitialWeight,
        weight = item.Weight,
        decreaseBy = item.DecreaseBy,
        removeOn = item.RemoveOn,
        isUnlocked = item.IsUnlocked,
    }
end

local function IntArray(list, count)
    local array = ffi.new("int[?]", count)
    for i = 1, count do
        local value = list[i]
        value = ffichecks.checkinteger(i, value)
        array[i - 1] = value
    end
    return array
end

local POOL_ITEM_FIELDS = {
    itemid = "ItemID",
    name = "Name",
    weight = "Weight",
    decreaseby = "DecreaseBy",
    removeon = "RemoveOn",
}
local NUM_POOL_ITEM_FIELDS = 5

local function ParsePoolItem(tbl, warnings)
    local desc = { itemId = 0, weight = 1.0, decreaseBy = 0.5, removeOn = 0.1 }
    if type(tbl) ~= "table" then
        warnings[#warnings + 1] = "argument is not a lua table"
        return desc
    end

    local entered = {}
    local enteredCount = 0
    local function enter(field)
        entered[field] = true
        enteredCount = enteredCount + 1
    end

    for key, value in pairs(tbl) do
        if enteredCount == NUM_POOL_ITEM_FIELDS then
            break
        end

        local keyType = type(key)
        local field = (keyType == "string" or keyType == "number") and POOL_ITEM_FIELDS[string.lower(tostring(key))]
        if field and not entered[field] then
            if field == "ItemID" then
                if math.type(value) == "integer" then
                    desc.itemId = value
                    enter("ItemID")
                    enter("Name")
                else
                    warnings[#warnings + 1] = "invalid parameter for \"ItemID\" (integer expected got " .. type(value) .. ")"
                end
            elseif field == "Name" then
                local valueType = type(value)
                if valueType == "string" or valueType == "number" then
                    desc.itemId = repentogon.L_ItemPool_GetCollectibleByName(tostring(value))
                    enter("ItemID")
                    enter("Name")
                else
                    warnings[#warnings + 1] = "invalid parameter for \"Name\" (string expected got " .. valueType .. ")"
                end
            else
                local number = tonumber(value)
                if number ~= nil then
                    if field == "Weight" then
                        desc.weight = number
                    elseif field == "DecreaseBy" then
                        desc.decreaseBy = number
                    else
                        desc.removeOn = number
                    end
                    enter(field)
                else
                    warnings[#warnings + 1] = "invalid parameter for \"" .. field .. "\" (number expected got " .. type(value) .. ")"
                end
            end
        end
    end
    return desc
end

local function AppendConversionWarnings(warnings, prefix, conversionWarnings)
    if #conversionWarnings > 0 then
        warnings[#warnings + 1] = prefix
        for _, warning in ipairs(conversionWarnings) do
            warnings[#warnings + 1] = warning
        end
    end
end

local function ParsePoolItems(tbl, warnings)
    local descs = {}
    local length = #tbl
    if length > 0 then
        for i = 1, length do
            local conversionWarnings = {}
            descs[i] = ParsePoolItem(rawget(tbl, i), conversionWarnings)
            AppendConversionWarnings(warnings, "Something went wrong when building PoolItem " .. i .. ":\n", conversionWarnings)
        end
    else
        local conversionWarnings = {}
        descs[1] = ParsePoolItem(tbl, conversionWarnings)
        AppendConversionWarnings(warnings, "Something went wrong when building PoolItem :\n", conversionWarnings)
    end
    return descs
end

local function PrintWarnings(warnings)
    for _, warning in ipairs(warnings) do
        repentogon.L_ItemPool_PrintWarning(warning)
    end
end

local function BitsToTable(self, roomBlacklist)
    local count = repentogon.L_ItemPool_GetCollectibleBits(self, roomBlacklist, nil)
    local result = {}
    if count > 1 then
        local bits = ffi.new("bool[?]", count)
        repentogon.L_ItemPool_GetCollectibleBits(self, roomBlacklist, bits)
        for i = 1, count - 1 do
            result[i] = bits[i]
        end
    end
    return result
end

local ItemPoolMT
ItemPoolMT = {
    __type = "ItemPool",

    AddBibleUpgrade = function(self, add, poolType)
        add = ffichecks.checkinteger(1, add)
        poolType = ffichecks.checkinteger(2, poolType)
        if not repentogon.L_ItemPool_AddBibleUpgrade(self, add, poolType) then
            ffichecks.argerror(2, "Invalid ItemPoolType")
        end
    end,
    AddCollectible = function(self, poolType, items)
        poolType = ffichecks.checkinteger(1, poolType)
        CheckValidPool(1, poolType)
        ffichecks.checktable(2, items)

        local warnings = {}
        for _, desc in ipairs(ParsePoolItems(items, warnings)) do
            repentogon.L_ItemPool_AddVirtualItem(poolType, desc.itemId, desc.weight, desc.decreaseBy, desc.removeOn)
        end
        PrintWarnings(warnings)
    end,
    AddRoomBlacklist = function(self, item)
        item = ffichecks.checkinteger(1, item)
        repentogon.L_ItemPool_AddRoomBlacklist(item)
    end,
    AddTemporaryCollectible = function(self, poolType, items)
        poolType = ffichecks.checkinteger(1, poolType)
        CheckValidPool(1, poolType)
        ffichecks.checktable(2, items)

        local warnings = {}
        for _, desc in ipairs(ParsePoolItems(items, warnings)) do
            local err = repentogon.L_ItemPool_AddTemporaryItem(poolType, desc.itemId, desc.weight, desc.decreaseBy, desc.removeOn)
            if err ~= nil then
                error(ffi.string(err), 0)
            end
        end
        PrintWarnings(warnings)
    end,
    CanSpawnCollectible = function(self, id, unkFlag)
        id = ffichecks.checkinteger(1, id)
        unkFlag = ffichecks.checkboolean(2, unkFlag)
        local result = repentogon.L_ItemPool_CanSpawnCollectible(self, id, unkFlag)
        if result < 0 then
            ffichecks.argerror(1, "Invalid collectible ID")
        end
        return result ~= 0
    end,
    ForceAddPillEffect = function(self, pillEffect)
        pillEffect = ffichecks.checkinteger(1, pillEffect)
        return repentogon.L_ItemPool_ForceAddPillEffect(self, pillEffect)
    end,
    GetBibleUpgrades = function(self, poolType)
        poolType = ffichecks.checkinteger(1, poolType)
        if not repentogon.L_ItemPool_GetBibleUpgrades(poolType, intOut) then
            ffichecks.argerror(1, "Invalid ItemPoolType")
        end
        return intOut[0]
    end,
    GetCard = function(self, seed, includePlayingCards, includeRunes, onlyRunes)
        seed = ffichecks.checkinteger(1, seed)
        return repentogon.L_ItemPool_GetCard(self, seed, ffichecks.optboolean(includePlayingCards, false), ffichecks.optboolean(includeRunes, false), ffichecks.optboolean(onlyRunes, false))
    end,
    GetCardEx = function(self, seed, specialChance, runeChance, suitChance, allowNonCards)
        seed = ffichecks.checkinteger(1, seed)
        specialChance = ffichecks.checkinteger(2, specialChance)
        runeChance = ffichecks.checkinteger(3, runeChance)
        suitChance = ffichecks.checkinteger(4, suitChance)
        allowNonCards = ffichecks.checkboolean(5, allowNonCards)
        return repentogon.L_ItemPool_GetCardEx(self, seed, specialChance, runeChance, suitChance, allowNonCards)
    end,
    GetCollectible = function(self, poolType, decrease, seed, defaultItem, flags)
        poolType = ffichecks.checkinteger(1, poolType)
        decrease = ffichecks.optboolean(decrease, false)
        if seed == nil then
            seed = Random()
        else
            seed = ffichecks.checkinteger(3, seed)
        end
        if defaultItem == nil then
            defaultItem = 0
        else
            defaultItem = ffichecks.checkinteger(4, defaultItem)
        end
        if flags == nil then
            flags = 0
        else
            flags = ffichecks.checkinteger(5, flags)
        end

        if not repentogon.L_ItemPool_GetCollectible(self, poolType, decrease, seed, defaultItem, flags, intOut) then
            ffichecks.argerror(1, "Invalid ItemPoolType")
        end
        return intOut[0]
    end,
    GetCollectibleFromList = function(self, list, seed, defaultItem, addToBlacklist, excludeActiveItems)
        ffichecks.checktable(1, list)
        if defaultItem == nil then
            defaultItem = 25 -- COLLECTIBLE_BREAKFAST
        else
            defaultItem = ffichecks.checkinteger(3, defaultItem)
        end

        -- if the table is empty, we should pass the default item
        local length = #list
        if length == 0 then
            return defaultItem
        end

        local array = IntArray(list, length)
        if seed == nil then
            seed = Random()
        else
            seed = ffichecks.checkinteger(2, seed)
        end
        return repentogon.L_ItemPool_GetCollectibleFromList(self, array, length, seed, defaultItem, ffichecks.optboolean(addToBlacklist, true), ffichecks.optboolean(excludeActiveItems, false))
    end,
    GetCollectiblesFromPool = function(self, poolType)
        poolType = ffichecks.checkinteger(1, poolType)
        local list = repentogon.L_ItemPool_GetPoolList(poolType)
        if list == nil then
            ffichecks.argerror(1, "Invalid ItemPoolType")
        end

        local first = ffi.getprivate(list, "Begin")
        local result = {}
        for i = 0, ffichecks.vectorsize(first, ffi.getprivate(list, "End"), ITEM_SIZE) - 1 do
            result[i + 1] = PoolItemToTable(first[i])
        end
        return result
    end,
    GetLastPool = function(self)
        return ffi.getprivate(self, "LastPool")
    end,
    GetNumAvailableTrinkets = function(self)
        return ffi.getprivate(self, "NumAvailableTrinkets")
    end,
    GetNumItemPools = function(self)
        return repentogon.L_ItemPool_GetNumItemPools()
    end,
    GetPill = function(self, seed)
        seed = ffichecks.checkinteger(1, seed)
        return repentogon.L_ItemPool_GetPill(self, seed)
    end,
    GetPillColor = function(self, pillEffect)
        pillEffect = ffichecks.checkinteger(1, pillEffect)
        return repentogon.L_ItemPool_GetPillColor(self, pillEffect)
    end,
    GetPillEffect = function(self, pillColor, player)
        pillColor = ffichecks.checkinteger(1, pillColor)
        ffichecks.checkcdata(2, player, "EntityPlayer", true)
        return repentogon.L_ItemPool_GetPillEffect(self, pillColor, player)
    end,
    GetPoolForRoom = function(self, roomType, seed)
        roomType = ffichecks.checkinteger(1, roomType)
        seed = ffichecks.checkinteger(2, seed)
        return repentogon.L_ItemPool_GetPoolForRoom(self, roomType, seed)
    end,
    GetRandomPool = function(self, rng, advancedSearch, filter, isWhitelist)
        ffichecks.checkcdata(1, rng, "RNG")
        if not ffichecks.optboolean(advancedSearch, false) then
            return repentogon.L_ItemPool_GetRandomPool(self, rng, false, nil, 0, false)
        end

        if not (filter == nil or type(filter) == "table") then
            ffichecks.argerror(3, "Invalid Filter")
        end
        isWhitelist = ffichecks.optboolean(isWhitelist, false)

        local count = filter and #filter or 0
        return repentogon.L_ItemPool_GetRandomPool(self, rng, true, IntArray(filter or {}, count), count, isWhitelist)
    end,
    GetRemovedCollectibles = function(self)
        return BitsToTable(self, false)
    end,
    GetRoomBlacklistedCollectibles = function(self)
        return BitsToTable(self, true)
    end,
    GetTrinket = function(self, dontAdvanceRNG)
        if dontAdvanceRNG ~= nil then
            dontAdvanceRNG = ffichecks.checkboolean(1, dontAdvanceRNG)
        end
        return repentogon.L_ItemPool_GetTrinket(self, dontAdvanceRNG or false)
    end,
    HasCollectible = function(self, collectible)
        collectible = ffichecks.checkinteger(1, collectible)
        return repentogon.L_ItemPool_HasCollectible(self, collectible)
    end,
    HasTrinket = function(self, trinket)
        trinket = ffichecks.checkinteger(1, trinket)
        return repentogon.L_ItemPool_HasTrinket(self, trinket)
    end,
    IdentifyPill = function(self, pillColor)
        pillColor = ffichecks.checkinteger(1, pillColor)
        repentogon.L_ItemPool_IdentifyPill(self, pillColor)
    end,
    IsPillIdentified = function(self, pillColor)
        pillColor = ffichecks.checkinteger(1, pillColor)
        return repentogon.L_ItemPool_IsPillIdentified(self, pillColor)
    end,
    PickCollectible = function(self, poolType, decrease, rng, flags)
        poolType = ffichecks.checkinteger(1, poolType)
        decrease = ffichecks.optboolean(decrease, false)
        if rng ~= nil then
            ffichecks.checkcdata(3, rng, "RNG")
        end
        if flags == nil then
            flags = 0
        else
            flags = ffichecks.checkinteger(4, flags)
        end
        CheckValidPool(1, poolType)

        local item = repentogon.L_ItemPool_PickCollectible(self, poolType, decrease, rng, flags)
        if item == nil then
            return nil
        end
        return PoolItemToTable(item)
    end,
    RemoveCollectible = function(self, collectible, param2, param3)
        collectible = ffichecks.checkinteger(1, collectible)
        return repentogon.L_ItemPool_RemoveCollectible(self, collectible, ffichecks.optboolean(param2, false), ffichecks.optboolean(param3, false))
    end,
    RemoveTemporaryCollectible = function(self, poolType, items)
        poolType = ffichecks.checkinteger(1, poolType)
        CheckValidPool(1, poolType)
        ffichecks.checktable(2, items)

        local warnings = {}
        for _, desc in ipairs(ParsePoolItems(items, warnings)) do
            local err = repentogon.L_ItemPool_RemoveTemporaryItem(poolType, desc.itemId, desc.weight, desc.decreaseBy, desc.removeOn)
            if err ~= nil then
                error(ffi.string(err), 0)
            end
        end
        PrintWarnings(warnings)
    end,
    RemoveTrinket = function(self, trinket)
        trinket = ffichecks.checkinteger(1, trinket)
        return repentogon.L_ItemPool_RemoveTrinket(self, trinket)
    end,
    ResetCollectible = function(self, collectible)
        collectible = ffichecks.checkinteger(1, collectible)
        if not repentogon.L_ItemPool_ResetCollectible(self, collectible) then
            ffichecks.argerror(1, "Invalid Collectible")
        end
    end,
    ResetRoomBlacklist = function(self)
        repentogon.L_ItemPool_ResetRoomBlacklist(self)
    end,
    ResetTrinkets = function(self)
        repentogon.L_ItemPool_ResetTrinkets(self)
    end,
    SetLastPool = function(self, poolType)
        poolType = ffichecks.checkinteger(1, poolType)
        CheckValidPool(1, poolType)
        ffi.setprivate(self, "LastPool", poolType)
    end,
    UnidentifyPill = function(self, pillColor)
        pillColor = ffichecks.checkinteger(1, pillColor)
        repentogon.L_ItemPool_UnidentifyPill(self, pillColor)
    end,
}

setmetatable(ItemPoolMT, { __index = function() end })
ItemPoolMT.__index = ItemPoolMT

ffi.metatype("struct ItemPool", ItemPoolMT)

ItemPool = setmetatable({}, {__class = ItemPoolMT})


