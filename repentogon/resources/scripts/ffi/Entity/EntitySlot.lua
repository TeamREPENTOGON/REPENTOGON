local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntitySlot { " .. Entity.Fields .. [[
    private int StateValue : 0x410;
    private int PrizeTypeValue : 0x414;
    private int16_t ShellGameAnimationIndexValue : 0x41a;
    private int16_t TimeoutValue : 0x41c;
    private int DonationValueValue : 0x420;
    private uint32_t TriggerTimerNumValue : 0x424;
    private uint16_t TouchValue : 0x42a;
    private int PrizeCollectibleValue : 0x540;
} : 0x548;
typedef struct EntitySlot* EntitySlotPtr;
]])

ffi.cdef [[
    void L_EntitySlot_CreateDropsFromExplosion(struct EntitySlot*);
    void L_EntitySlot_SetPrizeCollectible(struct EntitySlot*, int);
    struct Sprite* L_EntitySlot_GetPrizeSprite(struct EntitySlot*);
    int L_EntitySlot_RandomCoinJamIndex();
]]

local ffi = ffi
local repentogon = ffidll

local Getter = Entity.Helpers.Getter

local TYPE_SLOT = 6

local COIN_JAM_ANIMS = { [0] = "CoinJam", [1] = "CoinJam2", [2] = "CoinJam3", [3] = "CoinJam4" }

local function IntegerSetter(field)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, value)
    end
end

local methods = {
    CreateDropsFromExplosion = function(self)
        repentogon.L_EntitySlot_CreateDropsFromExplosion(self)
    end,
    GetDonationValue = Getter("DonationValueValue"),
    GetPrizeCollectible = Getter("PrizeCollectibleValue"),
    GetPrizeSprite = function(self)
        local result = repentogon.L_EntitySlot_GetPrizeSprite(self) return result
    end,
    GetPrizeType = Getter("PrizeTypeValue"),
    GetShellGameAnimationIndex = Getter("ShellGameAnimationIndexValue"),
    GetState = Getter("StateValue"),
    GetTimeout = Getter("TimeoutValue"),
    GetTouch = Getter("TouchValue"),
    GetTriggerTimerNum = Getter("TriggerTimerNumValue"),
    RandomCoinJamAnim = function(self)
        return COIN_JAM_ANIMS[repentogon.L_EntitySlot_RandomCoinJamIndex()]
    end,
    SetDonationValue = IntegerSetter("DonationValueValue"),
    SetPrizeCollectible = function(self, collectible)
        collectible = ffichecks.checkinteger(1, collectible)
        repentogon.L_EntitySlot_SetPrizeCollectible(self, collectible)
    end,
    SetPrizeType = IntegerSetter("PrizeTypeValue"),
    SetShellGameAnimationIndex = IntegerSetter("ShellGameAnimationIndexValue"),
    SetState = IntegerSetter("StateValue"),
    SetTimeout = IntegerSetter("TimeoutValue"),
    SetTouch = IntegerSetter("TouchValue"),
    SetTriggerTimerNum = IntegerSetter("TriggerTimerNumValue"),
}

local SlotMT = Entity.Inherit("EntitySlot", methods)
ffi.metatype("struct EntitySlot", SlotMT)
Entity.SetClassType(TYPE_SLOT, ffi.typeof("struct EntitySlot*"))
