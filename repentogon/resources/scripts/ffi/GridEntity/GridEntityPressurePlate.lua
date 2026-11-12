ffi.cdef[[
    struct GridEntityPressurePlate {
        padding void* vtable;
        private struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        struct Sprite Sprite;
        private struct Sprite TimerPlate;
        const char* NextGreedAnimation;
        private struct RNG GreedModeRNG;
    };
    typedef struct GridEntityPressurePlate* GridEntityPressurePlatePtr;
    
    void L_GridEntityPressurePlate_Render(struct GridEntityPressurePlate*, struct Vector);
    void L_GridEntityPressurePlate_Reward(struct GridEntityPressurePlate*);
    void L_GridEntityPressurePlate_SetTimerPlate(struct GridEntityPressurePlate*, struct Sprite*);
    void L_GridEntityPressurePlate_Update(struct GridEntityPressurePlate*);
]]
local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_GridEntityPressurePlate_Render)
ffi.reentrant(repentogon.L_GridEntityPressurePlate_Reward)
ffi.reentrant(repentogon.L_GridEntityPressurePlate_Update)

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
    if key == "TimerPlate" then
        return ffi.getprivate(self, "TimerPlate"):Copy()
    end
    if key == "GreedModeRNG" then
        local copy = ffi.new("struct RNG")
        ffi.copy(copy, ffi.getprivate(self, "GreedModeRNG"), ffi.sizeof("struct RNG"))
        return copy
    end
    return baseIndex(self, key)
end

GridEntityPressurePlateMT.__newindex = function(self, key, value)
    if key == "TimerPlate" then
        ffichecks.checkcdata(1, value, "Sprite")
        repentogon.L_GridEntityPressurePlate_SetTimerPlate(self, value)
        return
    end
    if key == "GreedModeRNG" then
        ffichecks.checkcdata(1, value, "RNG")
        ffi.setprivate(self, "GreedModeRNG", value)
        return
    end
    baseNewindex(self, key, value)
end

local GridEntityPressurePlateT = ffi.metatype("struct GridEntityPressurePlate", GridEntityPressurePlateMT)
GridEntityPressurePlate = setmetatable({}, {__class = GridEntityPressurePlateMT})
