local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityPickup { " .. Entity.Fields .. [[
    int Charge : 0x524;
    int OptionsPickupIndex : 0x528;
    private bool TouchedValue : 0x52c;
    private int PriceValue : 0x534;
    private bool AutoUpdatePriceValue : 0x538;
    private int ShopItemIdValue : 0x53c;
    int Timeout : 0x540;
    int Wait : 0x544;
    private int DropDelayValue : 0x554;
    int State : 0x584;
    private struct EntityEffect* PickupGhostValue : 0x590;
    private struct EntityPickup* MegaChestLeftValue : 0x594;
    private struct EntityPickup* MegaChestRightValue : 0x598;
    private int VarDataValue : 0x59c;
    private uint32_t CycleCollectibleListValue[8] : 0x5a0;
    private uint32_t CycleCollectibleCountValue : 0x5c0;
} : 0x6f0;
typedef struct EntityPickup* EntityPickupPtr;
]])

ffi.cdef [[
    void L_EntityPickup_Morph(struct EntityPickup*, int, int, int, bool, bool, bool);
    void L_EntityPickup_SetPrice(struct EntityPickup*, int);
    int L_EntityPickup_GetCoinValue(struct EntityPickup*);
    bool L_EntityPickup_TryOpenChest(struct EntityPickup*, void*);
    void L_EntityPickup_PlayDropSound(struct EntityPickup*);
    void L_EntityPickup_PlayPickupSound(struct EntityPickup*);
    void L_EntityPickup_AppearFast(struct EntityPickup*);
    bool L_EntityPickup_CanReroll(struct EntityPickup*);
    bool L_EntityPickup_CanJeraDuplicate(struct EntityPickup*);
    void L_EntityPickup_SetAlternatePedestal(struct EntityPickup*, int);
    int L_EntityPickup_GetAlternatePedestal(struct EntityPickup*);
    bool L_EntityPickup_TryRemoveCollectible(struct EntityPickup*);
    void L_EntityPickup_SetForceBlind(struct EntityPickup*, bool);
    bool L_EntityPickup_IsBlind(struct EntityPickup*, bool);
    int L_EntityPickup_SetNewOptionsPickupIndex(struct EntityPickup*);
    bool L_EntityPickup_TryInitOptionCycle(struct EntityPickup*, int);
    void L_EntityPickup_MakeShopItem(struct EntityPickup*, int);
    bool L_EntityPickup_TryFlip(struct EntityPickup*);
    struct Sprite* L_EntityPickup_GetPriceSprite(struct EntityPickup*);
    void L_EntityPickup_UpdatePickupGhosts(struct EntityPickup*);
    void L_EntityPickup_TriggerTheresOptionsPickup(struct EntityPickup*);
    int L_EntityPickup_AddCycleCollectible(struct EntityPickup*, int);
    bool L_EntityPickup_HasFlipData(struct EntityPickup*);
    int L_EntityPickup_GetFlipCollectible(struct EntityPickup*);
    void L_EntityPickup_InitFlipState(struct EntityPickup*, int, bool);
    void L_EntityPickup_ReloadGraphics(struct EntityPickup*, bool);
    void L_EntityPickup_GetLootList(struct EntityPickup*, bool, struct LootList*);
    int L_EntityPickup_GetCanRerollOverride(struct EntityPickup*);
    void L_EntityPickup_SetCanRerollOverride(struct EntityPickup*, bool);
    void L_EntityPickup_ClearCanRerollOverride(struct EntityPickup*);
    bool L_EntityPickup_ShouldIgnoreModifiers();
    void L_EntityPickup_GetRandomPickupVelocity(struct Vector*, struct RNG*, int, struct Vector*);
    int L_EntityPickup_SetupCollectibleGraphics(struct Sprite*, int, int, bool, unsigned int, bool);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local BooleanSetter = helpers.BooleanSetter
local EntityToPointer = ffichecks.entitytopointer

local TYPE_PICKUP = 5

local MAX_CYCLE = 8

local function IntegerSetter(field)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function GetRandomPickupVelocity(position, rng, velocityType)
    ffichecks.checkcdata(1, position, "Vector")
    ffichecks.checkcdata(2, rng, "RNG", true)
    velocityType = ffichecks.optnumber(velocityType, 0)
    local result = Vector(0, 0)
    repentogon.L_EntityPickup_GetRandomPickupVelocity(position, rng, velocityType, result)
    return result
end

local getters = {
    Price = Getter("PriceValue"),
    ShopItemId = Getter("ShopItemIdValue"),
    Touched = Getter("TouchedValue"),
    AutoUpdatePrice = Getter("AutoUpdatePriceValue"),
}

local setters = {
    Price = function(self, value)
        value = ffichecks.checkinteger(1, value)
        repentogon.L_EntityPickup_SetPrice(self, value)
    end,
    ShopItemId = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "ShopItemIdValue", math.min(math.max(value, -2), 7))
    end,
    Touched = function(self, value)
        ffi.setprivate(self, "TouchedValue", not not value)
    end,
    AutoUpdatePrice = function(self, value)
        ffi.setprivate(self, "AutoUpdatePriceValue", not not value)
    end,
}

local methods = {
    AddCollectibleCycle = function(self, id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_EntityPickup_AddCycleCollectible(self, id)
        if result < 0 then
            ffichecks.argerror(1, "Invalid collectible ID " .. id, 3)
        end
        return result == 1
    end,
    AppearFast = function(self)
        repentogon.L_EntityPickup_AppearFast(self)
    end,
    CanJeraDuplicate = function(self)
        local result = repentogon.L_EntityPickup_CanJeraDuplicate(self) return result
    end,
    CanReroll = function(self)
        local result = repentogon.L_EntityPickup_CanReroll(self) return result
    end,
    ClearCanRerollOverride = function(self)
        repentogon.L_EntityPickup_ClearCanRerollOverride(self)
    end,
    GetAlternatePedestal = function(self)
        local result = repentogon.L_EntityPickup_GetAlternatePedestal(self) return result
    end,
    GetCanRerollOverride = function(self)
        local override = repentogon.L_EntityPickup_GetCanRerollOverride(self)
        if override < 0 then
            return nil
        end
        return override == 1
    end,
    GetCoinValue = function(self)
        local result = repentogon.L_EntityPickup_GetCoinValue(self) return result
    end,
    GetCollectibleCycle = function(self)
        local list = ffi.getprivate(self, "CycleCollectibleListValue")
        local result = {}
        for i = 0, ffi.getprivate(self, "CycleCollectibleCountValue") - 1 do
            result[i + 1] = list[i]
        end
        return result
    end,
    GetDropDelay = Getter("DropDelayValue"),
    GetFlipCollectible = function(self)
        if repentogon.L_EntityPickup_HasFlipData(self) then
            local result = repentogon.L_EntityPickup_GetFlipCollectible(self) return result
        end
        return nil
    end,
    GetLootList = function(self, shouldAdvance)
        local list = LootList()
        repentogon.L_EntityPickup_GetLootList(self, ffichecks.optboolean(shouldAdvance, false), list)
        return list
    end,
    GetMegaChestLeftCollectible = Getter("MegaChestLeftValue"),
    GetMegaChestOtherCollectible = function(self)
        local left = ffi.getprivate(self, "MegaChestLeftValue")
        if left ~= nil then
            return left, false
        end
        local right = ffi.getprivate(self, "MegaChestRightValue")
        if right ~= nil then
            return right, true
        end
        return nil, nil
    end,
    GetMegaChestRightCollectible = Getter("MegaChestRightValue"),
    GetPickupGhost = Getter("PickupGhostValue"),
    GetPriceSprite = function(self)
        local result = repentogon.L_EntityPickup_GetPriceSprite(self) return result
    end,
    GetRandomPickupVelocity = function(self, ...)
        return GetRandomPickupVelocity(...)
    end,
    GetVarData = Getter("VarDataValue"),
    HasFlipData = function(self)
        local result = repentogon.L_EntityPickup_HasFlipData(self) return result
    end,
    InitFlipState = function(self, collectibleId, setupCollectibleGraphics)
        if collectibleId == nil then
            collectibleId = 0
        else
            collectibleId = ffichecks.checkinteger(1, collectibleId)
        end
        repentogon.L_EntityPickup_InitFlipState(self, collectibleId, ffichecks.optboolean(setupCollectibleGraphics, true))
    end,
    IsBlind = function(self, checkForcedBlindOnly)
        local result = repentogon.L_EntityPickup_IsBlind(self, ffichecks.optboolean(checkForcedBlindOnly, true)) return result
    end,
    IsShopItem = function(self)
        return ffi.getprivate(self, "PriceValue") ~= 0
    end,
    MakeShopItem = function(self, shopItemId)
        shopItemId = ffichecks.checkinteger(1, shopItemId)
        repentogon.L_EntityPickup_MakeShopItem(self, shopItemId)
    end,
    Morph = function(self, entityType, variant, subType, keepPrice, keepSeed, ignoreModifiers)
        entityType = ffichecks.checkinteger(1, entityType)
        variant = ffichecks.checkinteger(2, variant)
        subType = ffichecks.checkinteger(3, subType)
        repentogon.L_EntityPickup_Morph(self, entityType, variant, subType, not not keepPrice, not not keepSeed, not not ignoreModifiers)
    end,
    PlayDropSound = function(self)
        repentogon.L_EntityPickup_PlayDropSound(self)
    end,
    PlayPickupSound = function(self)
        repentogon.L_EntityPickup_PlayPickupSound(self)
    end,
    ReloadGraphics = function(self, ignoreBlind)
        ignoreBlind = ffichecks.checkboolean(1, ignoreBlind)
        repentogon.L_EntityPickup_ReloadGraphics(self, ignoreBlind)
    end,
    RemoveCollectibleCycle = function(self)
        local list = ffi.getprivate(self, "CycleCollectibleListValue")
        for i = 0, 6 do
            list[i] = 0
        end
        ffi.setprivate(self, "CycleCollectibleCountValue", 0)
    end,
    SetAlternatePedestal = function(self, pedestalType)
        pedestalType = ffichecks.checkinteger(1, pedestalType)
        repentogon.L_EntityPickup_SetAlternatePedestal(self, pedestalType)
    end,
    SetCanRerollOverride = function(self, canReroll)
        canReroll = ffichecks.checkboolean(1, canReroll)
        repentogon.L_EntityPickup_SetCanRerollOverride(self, canReroll)
    end,
    SetDropDelay = IntegerSetter("DropDelayValue"),
    SetForceBlind = function(self, blind)
        blind = ffichecks.checkboolean(1, blind)
        repentogon.L_EntityPickup_SetForceBlind(self, blind)
    end,
    SetNewOptionsPickupIndex = function(self)
        local result = repentogon.L_EntityPickup_SetNewOptionsPickupIndex(self) return result
    end,
    SetVarData = IntegerSetter("VarDataValue"),
    TriggerTheresOptionsPickup = function(self)
        repentogon.L_EntityPickup_TriggerTheresOptionsPickup(self)
    end,
    TryFlip = function(self)
        local result = repentogon.L_EntityPickup_TryFlip(self) return result
    end,
    TryInitOptionCycle = function(self, numCycle)
        numCycle = ffichecks.checkinteger(1, numCycle)
        local result = repentogon.L_EntityPickup_TryInitOptionCycle(self, numCycle) return result
    end,
    TryOpenChest = function(self, player)
        local result = repentogon.L_EntityPickup_TryOpenChest(self, EntityToPointer(player)) return result
    end,
    TryRemoveCollectible = function(self)
        local result = repentogon.L_EntityPickup_TryRemoveCollectible(self) return result
    end,
    UpdatePickupGhosts = function(self)
        repentogon.L_EntityPickup_UpdatePickupGhosts(self)
    end,
}

local PickupMT = Entity.Inherit("EntityPickup", methods, getters, setters)
ffi.metatype("struct EntityPickup", PickupMT)
Entity.SetClassType(TYPE_PICKUP, ffi.typeof("struct EntityPickup*"))

EntityPickup = setmetatable({
    GetRandomPickupVelocity = GetRandomPickupVelocity,
    SetupCollectibleGraphics = function(sprite, layerId, collectibleType, blind, seed, loadGraphics)
        ffichecks.checkcdata(1, sprite, "Sprite")
        layerId = ffichecks.checkinteger(2, layerId)
        collectibleType = ffichecks.checkinteger(3, collectibleType)
        blind = ffichecks.checkboolean(4, blind)
        seed = ffichecks.optnumber(seed, Random())
        loadGraphics = ffichecks.checkboolean(6, loadGraphics)
        local result = repentogon.L_EntityPickup_SetupCollectibleGraphics(sprite, layerId, collectibleType, blind, seed, loadGraphics)
        if result == 1 then
            ffichecks.argerror(2, "No Layer with Id " .. layerId, 3)
        elseif result == 2 then
            ffichecks.argerror(3, "Invalid collectible with ID " .. collectibleType, 3)
        end
    end,
    ShouldIgnoreModifiers = function()
        local result = repentogon.L_EntityPickup_ShouldIgnoreModifiers() return result
    end,
}, { __class = PickupMT })
