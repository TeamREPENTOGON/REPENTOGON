ffi.cdef[[
typedef struct {
    float Red;
    float Green;
    float Blue;
    float Alpha;
} Color;
]]

local repentogon = ffidll
local lffi = ffi

local function ParseKColorString(str, mt)
    return string.format(str, mt.Red, mt.Green, mt.Blue, mt.Alpha)
end

local KColorMT = lffi.metatype("Color", {
    __tostring = function(self) 
        return ParseKColorString("[KColor %g %g %g %g]", self)
    end
})

KColor = setmetatable({
    Black = KColorMT(0, 0, 0, 1),
    Red = KColorMT(1, 0, 0, 1),
    Green = KColorMT(0, 1, 0, 1),
    Blue = KColorMT(0, 0, 1, 1),
    Yellow = KColorMT(1, 1, 0, 1),
    Cyan = KColorMT(0, 1, 1, 1),
    Magenta = KColorMT(1, 0, 1, 1),
    White = KColorMT(1, 1, 1, 1),
    Transparent = KColorMT(0, 0, 0, 0)
},
{
    __call = function(_, Red, Green, Blue, Alpha)
        return KColorMT(Red or 1, Green or 1, Blue or 1, Alpha or 1)
    end
})