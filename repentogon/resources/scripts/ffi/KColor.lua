ffi.cdef[[
struct KColor {
    float Red;
    float Green;
    float Blue;
    float Alpha;
    padding int;
};
typedef struct KColor* KColorPtr;
struct KColor* L_KColor_Constructor(struct KColor*, float, float, float, float);
]]

local L_KColor_Constructor = ffidll.L_KColor_Constructor

local KColorMT = {
    __type = "KColor",
}

local KColorT = ffi.metatype("struct KColor", KColorMT)

KColorMT.__index = KColorMT

local KColorGlobalMT = {
    __call = function(_, red, green, blue, alpha)
        local color = KColorT();
        L_KColor_Constructor(color, red, green, blue, alpha)
        return color
    end,

    __class = KColorMT,
}

KColorGlobalMT.__index = KColorGlobalMT

KColor = setmetatable({
    Black = KColorT(0, 0, 0, 1),
    Red = KColorT(1, 0, 0, 1),
    Green = KColorT(0, 1, 0, 1),
    Blue = KColorT(0, 0, 1, 1),
    Yellow = KColorT(1, 1, 0, 1),
    Cyan = KColorT(0, 1, 1, 1),
    Magenta = KColorT(1, 0, 1, 1),
    White = KColorT(1, 1, 1, 1),
    Transparent = KColorT(0, 0, 0, 0),
}, KColorGlobalMT)