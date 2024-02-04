ffi.cdef[[
ItemConfig_Item* L_ItemConfig_GetCollectible(const int);
]]

local repentogon = ffidll
local lffi = ffi

-- TODO: Add those weird ItemConfig enums, maybe after bitwise stuff is supported.

ItemConfig = {}

Isaac.GetItemConfig = function()
	return ItemConfig
end

ItemConfig.GetCollectible = function(selfOrID, id)
	if selfOrID ~= ItemConfig then
		ffichecks.checknumber(1, selfOrID)
		id = selfOrID
	else
		ffichecks.checknumber(2, id)
	end
	local item = repentogon.L_ItemConfig_GetCollectible(id)
	if item == nil then
		return nil
	end
	return item
end
