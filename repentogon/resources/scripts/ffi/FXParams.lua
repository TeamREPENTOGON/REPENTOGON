ffi.cdef[[
struct FXParams {
    private struct KColor RoomColor : 0x0;
    private float Brightness : 0x14;
	private float Contrast : 0x18;
	bool UseWaterV2 : 0x1c;
	struct KColor WaterColor : 0x20; 
    struct KColor WaterColorMultiplier : 0x34;
	float ShadowAlpha : 0x48;
    struct KColor ShadowColor : 0x4c; 
    struct KColor LightColor : 0x60;
	struct Color WaterEffectColor : 0x74;
} : 0xa0;

]]

local repentogon = ffidll
local ffi = ffi

local FXParamsMT
FXParamsMT = {
    __type = "FXParams",
}

setmetatable(FXParamsMT, { __index = function() end })
FXParamsMT.__index = function(self, key)
    if key == "ColorModifier" then
        -- It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
        -- This logic provides backwards compatability.

        local roomColor = ffi.getprivate(self, "RoomColor")
        return ColorModifier(roomColor.Red, roomColor.Green, roomColor.Blue, roomColor.Alpha, ffi.getprivate(self, "Brightness"), ffi.getprivate(self, "Contrast"))
    end
    return FXParamsMT[key]
end

FXParamsMT.__newindex = function(self, key, value)
    if key == "ColorModifier" then
        -- It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
        -- This logic provides backwards compatability.

        ffichecks.checkcdata(value, "ColorModifier")
        ffi.setprivate(self, "RoomColor", KColor(value.R, value.G, value.B, value.A))
        ffi.setprivate(self, "Brightness", value.Brightness)
        ffi.setprivate(self, "Contrast", value.Contrast)
    end
end

local FXParamsT = ffi.metatype("struct FXParams", FXParamsMT)

FXParams = setmetatable({}, {
    __class = FXParamsMT,
})