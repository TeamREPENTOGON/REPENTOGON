ffi.cdef[[
    struct GridEntityPressurePlate {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        struct Sprite TimerPlate;
        const char* NextGreedAnimation;
        struct RNG GreedModeRNG;
    };
    typedef struct GridEntityPressurePlate* GridEntityPressurePlatePtr;
    
    void L_GridEntityPressurePlate_Render(struct GridEntityPressurePlate*, struct Vector);
    void L_GridEntityPressurePlate_Reward(struct GridEntityPressurePlate*);
    void L_GridEntityPressurePlate_Update(struct GridEntityPressurePlate*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityPressurePlateMT
GridEntityPressurePlateMT = {
    __type = "GridEntityPressurePlate",
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityPressurePlate_Render(self, offset)
    end,
    Reward = function(self)
        repentogon.L_GridEntityPressurePlate_Reward(self)
    end,
    Update = function(self)
        repentogon.L_GridEntityPressurePlate_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityPressurePlateMT.__index = function(self, key)
    if GridEntityPressurePlateMT[key] ~= nil then
        return GridEntityPressurePlateMT[key]
    end
    return baseIndex(self, key)
end

GridEntityPressurePlateMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityPressurePlateT = ffi.metatype("struct GridEntityPressurePlate", GridEntityPressurePlateMT)
GridEntityPressurePlate = setmetatable({}, {__class = GridEntityPressurePlateMT})
