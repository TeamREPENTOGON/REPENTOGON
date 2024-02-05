ffi.cdef[[
typedef struct {
	int ID;
	int AchievementID;
	char _[0x18];  // Name
	char _[0x18];  // Description
	bool GreedModeAllowed;
	unsigned int AnnouncerVoice;
	unsigned int AnnouncerSuper;
	unsigned int AnnouncerDelay;
	unsigned int MimicCharge;
	uint8_t EffectClass;
	uint8_t EffectSubClass;
} ItemConfig_Pill;

bool L_ItemConfigPill_IsAvailable(ItemConfig_Pill*);

const char* L_ItemConfigPill_GetName(ItemConfig_Pill*);
const char* L_ItemConfigPill_SetName(ItemConfig_Pill*, const char*);
const char* L_ItemConfigPill_GetDescription(ItemConfig_Pill*);
const char* L_ItemConfigPill_SetDescription(ItemConfig_Pill*, const char*);
]]

local repentogon = ffidll
local lffi = ffi

local ItemConfigPillFuncs = {}

function ItemConfigPillFuncs:IsAvailable()
	return repentogon.L_ItemConfigPill_IsAvailable(self)
end

local function GetStrFunc(func)
	return function(self)
		return lffi.string(lffi.gc(func(self), repentogon.L_Free))
	end
end

local getkeys = {
	Name = GetStrFunc(repentogon.L_ItemConfigPill_GetName),
	Description = GetStrFunc(repentogon.L_ItemConfigPill_GetDescription),
}

local function SetStrFunc(func)
	return function(self, str)
		ffichecks.checkstring(2, str)
		func(self, str)
	end
end

local setkeys = {
	Name = SetStrFunc(repentogon.L_ItemConfigPill_SetName),
	Description = SetStrFunc(repentogon.L_ItemConfigPill_SetDescription),
}

local ItemConfigPillEffectMT = lffi.metatype("ItemConfig_Pill", {
	__tostring = function(self)
		return "ItemConfigPillEffect(#" .. self.ID .. ": " .. self.Name .. ")"
	end,
	__index = function(self, key)
		if getkeys[key] ~= nil then
			return getkeys[key](self)
		end
		return ItemConfigPillFuncs[key]
	end,
	__newindex = function(self, key, value)
		if setkeys[key] ~= nil then
			return setkeys[key](self, value)
		else
			error(string.format("no writable variable '%s'", key))
		end
	end,
})
