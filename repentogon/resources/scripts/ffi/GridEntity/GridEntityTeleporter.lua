ffi.cdef[[
    struct GridEntityTeleporter {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityTeleporter* GridEntityTeleporterPtr;
    
    void L_GridEntityTeleporter_Render(struct GridEntityTeleporter*, struct Vector);
    void L_GridEntityTeleporter_Update(struct GridEntityTeleporter*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityTeleporterMT
GridEntityTeleporterMT = {
    __type = "GridEntityTeleporter",
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityTeleporter_Render(self, offset)
    end,
    Update = function(self)
        repentogon.L_GridEntityTeleporter_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityTeleporterMT.__index = function(self, key)
    if GridEntityTeleporterMT[key] ~= nil then
        return GridEntityTeleporterMT[key]
    end
    return baseIndex(self, key)
end

GridEntityTeleporterMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityTeleporterT = ffi.metatype("struct GridEntityTeleporter", GridEntityTeleporterMT)
GridEntityTeleporter = setmetatable({}, {__class = GridEntityTeleporterMT})