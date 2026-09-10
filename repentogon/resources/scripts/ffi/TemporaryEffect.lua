ffi.cdef[[
struct TemporaryEffect {
    struct ItemConfigItem* Item;
    int Count;
    int Cooldown;
    padding char[0x4]; // Internally this is something Mars related but we haven't fully identified it.
};

typedef struct TemporaryEffect* TemporaryEffectPtr;
typedef struct const TemporaryEffect ConstTemporaryEffect;
]]

local TemporaryEffectMT
TemporaryEffectMT = {
    __type = "TemporaryEffect",
}

setmetatable(TemporaryEffectMT, { __index = function() end })
TemporaryEffectMT.__index = TemporaryEffectMT

local TemporaryEffectT = ffi.metatype("struct TemporaryEffect", TemporaryEffectMT)

TemporaryEffect = setmetatable({}, {
    __class = TemporaryEffectMT,
})
