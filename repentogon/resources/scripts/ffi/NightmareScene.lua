ffi.cdef[[
	struct NightmareScene {
		struct Sprite BackgroundSprite : 0x20;
		struct Sprite BubbleSprite : 0x134;
		struct Sprite ProgressBarSprite : 0x248;
		int ProgressBarMap[14] : 0x36c;
		struct Sprite PlayerExtraPortraitSprite : 0x4d4;
		bool _IsDogmaNightmare : 0x5e8;
	} : 0x5ec;

    struct NightmareScene* L_NightmareScene_Get();
]]

local repentogon = ffidll

local NightmareSceneMT
NightmareSceneMT = {
    __type = "NightmareScene",
}

setmetatable(NightmareSceneMT, { __index = function() end })
NightmareSceneMT.__index = NightmareSceneMT

local NightmareSceneT = ffi.metatype("struct NightmareScene", NightmareSceneMT)

local function GetNightmareScene()
    return repentogon.L_NightmareScene_Get()
end

NightmareScene = {
    GetBackgroundSprite = function()
		return GetNightmareScene().BackgroundSprite
	end,
	GetBubbleSprite = function()
		return GetNightmareScene().BubbleSprite
	end,
	GetPlayerExtraPortraitSprite = function()
		return GetNightmareScene().PlayerExtraPortraitSprite
	end,
	GetProgressBarMap = function()
		local map = GetNightmareScene().ProgressBarMap
		local result = {}
		for i = 0, 13 do
			result[i + 1] = map[i]
		end
		return result
	end,
    GetProgressBarSprite = function()
		return GetNightmareScene().ProgressBarSprite
	end,
	IsDogmaNightmare = function()
		return GetNightmareScene()._IsDogmaNightmare
	end,
}