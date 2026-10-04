ffi.cdef [[
    struct EntityConfigBaby {
        private int ID : 0x0;
        private struct StdString Name : 0x4;
        private struct StdString Gfx : 0x1c;
        private int AchievementID : 0x34;
    } : 0x38;
    typedef struct EntityConfigBaby* EntityConfigBabyPtr;
]]

local ffi = ffi

local EntityConfigBabyMT
EntityConfigBabyMT = {
    __type = "EntityConfigBaby",
    GetAchievementID = function(self)
        return ffi.getprivate(self, "AchievementID")
    end,
    GetID = function(self)
        return ffi.getprivate(self, "ID")
    end,
    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Name"))
    end,
    GetSpritesheetPath = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Gfx"))
    end,
}

setmetatable(EntityConfigBabyMT, { __index = function() end })
EntityConfigBabyMT.__index = EntityConfigBabyMT

ffi.metatype("struct EntityConfigBaby", EntityConfigBabyMT)

EntityConfigBaby = setmetatable({}, {
    __class = EntityConfigBabyMT,
})
