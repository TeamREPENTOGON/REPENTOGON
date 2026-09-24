ffi.cdef[[
struct FXLayers {
	private struct FXParams FXParams : 0x4f8;
} : 0x598;

void L_FXLayers_AddPoopFx(struct FXLayers*, struct Color*);
]]

local repentogon = ffidll
local ffi = ffi

local FXLayersMT
FXLayersMT = {
    __type = "FXLayers",
    AddPoopFx = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        repentogon.L_FXLayers_AddPoopFx(self, color)
    end,
}

setmetatable(FXLayersMT, { __index = function() end })
FXLayersMT.__index = function(self, key)
    return FXLayersMT[key]
end

local FXLayersT = ffi.metatype("struct FXLayers", FXLayersMT)

FXLayers = setmetatable({}, {
    __class = FXLayersMT,
})