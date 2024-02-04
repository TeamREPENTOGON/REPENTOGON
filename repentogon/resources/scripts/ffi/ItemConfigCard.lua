ffi.cdef[[
typedef struct {
	int ID;
	int AchievementID;
	char _[0x18];  // Name
	char _[0x18];  // Description
	char _[0x18];  // HudAnim
	bool GreedModeAllowed;
	unsigned int PickupSubtype;
	unsigned int CardType;
	unsigned int AnnouncerVoice;
	unsigned int AnnouncerDelay;
	unsigned int MimicCharge;
} ItemConfig_Card;

bool L_ItemConfigCard_IsAvailable(ItemConfig_Card*);

const char* L_ItemConfigCard_GetName(ItemConfig_Card*);
const char* L_ItemConfigCard_SetName(ItemConfig_Card*, const char*);
const char* L_ItemConfigCard_GetDescription(ItemConfig_Card*);
const char* L_ItemConfigCard_SetDescription(ItemConfig_Card*, const char*);
const char* L_ItemConfigCard_GetHudAnim(ItemConfig_Card*);
const char* L_ItemConfigCard_SetHudAnim(ItemConfig_Card*, const char*);
]]

local repentogon = ffidll
local lffi = ffi

local ItemConfigCardFuncs = {}

function ItemConfigCardFuncs:IsCard()
	return self.CardType == ItemConfig.CARDTYPE_TAROT or self.CardType == ItemConfig.CARDTYPE_SUIT
			or self.CardType == ItemConfig.CARDTYPE_SPECIAL or self.CardType == ItemConfig.CARDTYPE_TAROT_REVERSE
end

function ItemConfigCardFuncs:IsRune()
	return self.CardType == ItemConfig.CARDTYPE_RUNE
end

function ItemConfigCardFuncs:IsAvailable()
	return repentogon.L_ItemConfigCard_IsAvailable(self)
end

local function GetStrFunc(func)
	return function(self)
		return lffi.string(lffi.gc(func(self), repentogon.L_Free))
	end
end

local getkeys = {
	Name = GetStrFunc(repentogon.L_ItemConfigCard_GetName),
	Description = GetStrFunc(repentogon.L_ItemConfigCard_GetDescription),
	HudAnim = GetStrFunc(repentogon.L_ItemConfigCard_GetHudAnim),
}

local function SetStrFunc(func)
	return function(self, str)
		ffichecks.checkstring(2, str)
		func(self, str)
	end
end

local setkeys = {
	Name = SetStrFunc(repentogon.L_ItemConfigCard_SetName),
	Description = SetStrFunc(repentogon.L_ItemConfigCard_SetDescription),
	HudAnim = SetStrFunc(repentogon.L_ItemConfigCard_SetHudAnim),
}

local ItemConfigCardMT = lffi.metatype("ItemConfig_Card", {
	__tostring = function(self)
		return "ItemConfigCard(#" .. self.ID .. ": " .. self.Name .. ")"
	end,
	__index = function(self, key)
		if getkeys[key] ~= nil then
			return getkeys[key](self)
		end
		return ItemConfigCardFuncs[key]
	end,
	__newindex = function(self, key, value)
		if setkeys[key] ~= nil then
			return setkeys[key](self, value)
		else
			error(string.format("no writable variable '%s'", key))
		end
	end,
})
