ffi.cdef[[
ANM2* L_NightmareScene_GetBackgroundSprite();
ANM2* L_NightmareScene_GetBubbleSprite();
ANM2* L_NightmareScene_GetProgressBarSprite();
int* L_NightmareScene_GetProgressBarMap();
bool L_NightmareScene_IsDogmaNightmare();
]]

local repentogon = ffidll

NightmareScene = {}

NightmareScene.GetBackgroundSprite = function() return repentogon.L_NightmareScene_GetBackgroundSprite() end

NightmareScene.GetBubbleSprite = function() return repentogon.L_NightmareScene_GetBubbleSprite() end

NightmareScene.GetProgressBarSprite = function() return repentogon.L_NightmareScene_GetProgressBarSprite() end

NightmareScene.GetProgressBarMap = function()
	local progressBarMap = repentogon.L_NightmareScene_GetProgressBarMap()
	
	local progressBarTable = {}
	
	for i = 0 , 13 do
		progressBarTable[i + 1] = progressBarMap[i]
	end
	
	return progressBarTable
end

NightmareScene.IsDogmaNightmare = function() return repentogon.L_NightmareScene_IsDogmaNightmare() end