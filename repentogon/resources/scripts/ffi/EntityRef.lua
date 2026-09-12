ffi.cdef [[
    struct EntityRef {
        int Type;
        int Variant;
        int SpawnerType;
        int SpawnerVariant;
        struct Vector Position;
        private struct Vector Velocity; // No idea why John Nicalis didn't expose this
        private int Flags;
        private void* EntityTODO;
    };

    typedef struct EntityRef* EntityRefPtr;
]]

--[[
    We haven't FFI'd Entity yet so we're getting a little creative.
    EntityRef's constructor and Entity fields are going through the traditional API for now.
    This is thanks to our LuaJIT fork supporting pushing CData through the standard API.
    We can follow this pattern for anything that depends on a class that isn't FFI'd.
]]
local cfuncs = {
    Ctor = __Lua_EntityRef_Ctor,
    GetEntity = __Lua_EntityRef_GetEntity,
    SetEntity = __Lua_EntityRef_SetEntity,
}

local FLAG_CHARMED = 1
local FLAG_FRIENDLY = 2

local ffi = ffi
local EntityRefMT
EntityRefMT = {
    __type = "EntityRef",
}

EntityRefMT.__index = function(self, key)
    if key == "Entity" then
        return cfuncs.GetEntity(self)
    end
    if key == "IsCharmed" then
        return (ffi.getprivate(self, "Flags") & FLAG_CHARMED) ~= 0
    end
    if key == "IsFriendly" then
        return (ffi.getprivate(self, "Flags") & FLAG_FRIENDLY) ~= 0
    end
    return EntityRefMT[key]
end

EntityRefMT.__newindex = function(self, key, value)
    if key == "Entity" then
        return cfuncs.SetEntity(self, value)
    end
    if key == "IsCharmed" then
        ffichecks.checkboolean(1, value)
        local flags = ffi.getprivate(self, "Flags")
        if value then
            flags = flags | FLAG_CHARMED
        else
            flags = flags & ~FLAG_CHARMED
        end
        ffi.setprivate(self, "Flags", flags)
        return
    end
    if key == "IsFriendly" then
        ffichecks.checkboolean(1, value)
        local flags = ffi.getprivate(self, "Flags")
        if value then
            flags = flags | FLAG_FRIENDLY
        else
            flags = flags & ~FLAG_FRIENDLY
        end
        ffi.setprivate(self, "Flags", flags)
        return
    end
end

local EntityRefT = ffi.metatype("struct EntityRef", EntityRefMT)

EntityRef = setmetatable({}, {
    __class = EntityRefMT,
    __call = function(_, ent)
        return cfuncs.Ctor(ent)
    end,
})

__Lua_EntityRef_Ctor = nil
__Lua_EntityRef_GetEntity = nil
__Lua_EntityRef_SetEntity = nil