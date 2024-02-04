ffi.cdef[[
typedef struct {
	int ID;
	char _[0x18];  // Anm2Path
	unsigned int Priority;
	bool HasOverlay;
	bool IsFlying;
	bool HasSkinAlt;
	int SkinColor;
	unsigned int OverwriteColor;
} ItemConfig_Costume;

const char* L_ItemConfigCostume_GetAnm2Path(ItemConfig_Costume*);
const char* L_ItemConfigCostume_SetAnm2Path(ItemConfig_Costume*, const char*);
]]

local repentogon = ffidll
local lffi = ffi

local ItemConfigCostumeFuncs = {}

local getkeys = {
	Anm2Path = function(self) return lffi.string(lffi.gc(repentogon.L_ItemConfigCostume_GetAnm2Path(self), repentogon.L_Free)) end,
}

local setkeys = {
	Anm2Path = function(self, str)
		ffichecks.checkstring(2, str)
		repentogon.L_ItemConfigCostume_SetAnm2Path(self, str)
	end,
}

local ItemConfigCostumeMT = lffi.metatype("ItemConfig_Costume", {
	__tostring = function(self)
		return "ItemConfigCostume(#" .. self.ID .. ": " .. self.Anm2Path .. ")"
	end,
	__index = function(self, key)
		if getkeys[key] ~= nil then
			return getkeys[key](self)
		end
		return ItemConfigCostumeFuncs[key]
	end,
	__newindex = function(self, key, value)
		if setkeys[key] ~= nil then
			return setkeys[key](self, value)
		else
			error(string.format("no writable variable '%s'", key))
		end
	end,
})
