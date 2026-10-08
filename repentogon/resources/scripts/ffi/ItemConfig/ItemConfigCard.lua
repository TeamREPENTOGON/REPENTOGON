ffi.cdef [[
    struct ItemConfigCard {
        int ID : 0x0;
        int AchievementID : 0x4;
        private struct StdString NameString : 0x8;
        private struct StdString DescriptionString : 0x20;
        private struct StdString HudAnimString : 0x38;
        bool GreedModeAllowed : 0x50;
        unsigned int PickupSubtype : 0x54;
        int CardType : 0x58;
        int AnnouncerVoice : 0x5c;
        int AnnouncerDelay : 0x60;
        int MimicCharge : 0x64;
        private struct Sprite* ModdedCardFrontSprite : 0x6c;
    } : 0x70;
    typedef struct ItemConfigCard* ItemConfigCardPtr;

    bool L_ItemConfigCard_IsAvailable(struct ItemConfigCard*);
    bool L_ItemConfigCard_GetHidden(struct ItemConfigCard*);
    float L_ItemConfigCard_GetInitialWeight(struct ItemConfigCard*);
    float L_ItemConfigCard_GetWeight(struct ItemConfigCard*);
    void L_ItemConfigCard_SetWeight(struct ItemConfigCard*, float);
    void L_ItemConfigCard_SetAvailabilityCondition(struct ItemConfigCard*, bool (*)());
    void L_ItemConfigCard_ReportAvailabilityError(struct ItemConfigCard*, const char*);
]]

local ffi = ffi
local repentogon = ffidll

local conditions = {}

local function ReleaseCondition(card)
    local old = conditions[card.ID]
    if old then
        old.callback:free()
        conditions[card.ID] = nil
    end
end

local stringKeys = {
    Name = "NameString",
    Description = "DescriptionString",
    HudAnim = "HudAnimString",
}

local getters = {
    ModdedCardFront = function(self)
        local result = ffi.getprivate(self, "ModdedCardFrontSprite") return result
    end,
    Hidden = repentogon.L_ItemConfigCard_GetHidden,
    InitialWeight = repentogon.L_ItemConfigCard_GetInitialWeight,
    Weight = repentogon.L_ItemConfigCard_GetWeight,
}

local ItemConfigCardMT
ItemConfigCardMT = {
    __type = "ItemConfigCard",

    __index = function(self, key)
        local field = stringKeys[key]
        if field then
            return ffichecks.stdstring(ffi.getprivate(self, field))
        end
        local getter = getters[key]
        if getter then
            return getter(self)
        end
        return ItemConfigCardMT[key]
    end,

    __newindex = function(self, key, value)
        local field = stringKeys[key]
        if field then
            value = ffichecks.checkstring(3, value)
            repentogon.L_StdString_Assign(ffi.getprivate(self, field), value)
            return
        end
        if key == "Weight" then
            value = ffichecks.checknumber(3, value)
            repentogon.L_ItemConfigCard_SetWeight(self, value)
            return
        end
        error(string.format("cannot set '%s'", tostring(key)))
    end,

    ClearAvailabilityCondition = function(self)
        repentogon.L_ItemConfigCard_SetAvailabilityCondition(self, nil)
        ReleaseCondition(self)
    end,
    GetAvailabilityCondition = function(self)
        local entry = conditions[self.ID]
        return entry and entry.condition
    end,
    IsAvailable = function(self)
        local result = repentogon.L_ItemConfigCard_IsAvailable(self) return result
    end,
    IsCard = function(self)
        local cardType = self.CardType
        return cardType == 0 or cardType == 1 or cardType == 3 or cardType == 5
    end,
    IsRune = function(self)
        return self.CardType == 2
    end,
    SetAvailabilityCondition = function(self, condition)
        ffichecks.checkfunction(1, condition)
        local callback = ffi.cast("bool (*)()", function()
            local ok, result = pcall(condition)
            if not ok then
                repentogon.L_ItemConfigCard_ReportAvailabilityError(self, tostring(result))
                return true
            end
            return result == true
        end)
        repentogon.L_ItemConfigCard_SetAvailabilityCondition(self, callback)
        ReleaseCondition(self)
        conditions[self.ID] = {condition = condition, callback = callback}
    end,
}

ffi.metatype("struct ItemConfigCard", ItemConfigCardMT)

ItemConfigCard = setmetatable({}, {__class = ItemConfigCardMT})
