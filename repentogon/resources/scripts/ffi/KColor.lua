ffi.cdef[[
struct KColor {
    float Red;
    float Green;
    float Blue;
    float Alpha;
};
typedef struct Color* KColorPtr;
]]

local KColorMT = {
    __type = "KColor",
}

local KColorT = ffi.metatype("struct KColor", KColorMT)

KColorMT.__index = function(self, key)
	return KColorMT[key]
end

local KColorGlobalMT = {
    __call = function(_, red, green, blue, alpha)
        return KColorT(red, green, blue, alpha)
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