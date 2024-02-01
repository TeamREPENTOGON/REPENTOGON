ffi.cdef[[
typedef struct {
    void* _;
} ANM2;

ANM2* L_Sprite_GetPlayerAnim();
const char* L_Sprite_GetAnimation(ANM2*);
const char* L_Sprite_GetDefaultAnimation(ANM2*);
]]

local repentogon = ffidll
local lffi = ffi

local SpriteFuncs = {}

function SpriteFuncs:GetAnimation()
    return lffi.string(repentogon.L_Sprite_GetAnimation(self))
end

function SpriteFuncs:GetDefaultAnimation()
    return lffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
end

function SpriteFuncs:GetDefaultAnimationName()
    return lffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
end

local SpriteMT = lffi.metatype("ANM2", {
    __tostring = function()
        return "Sprite"
    end,
    __index = SpriteFuncs
})

Sprite = setmetatable({}, {
    __call = function() 
        return repentogon.L_Sprite_GetPlayerAnim() 
    end
})