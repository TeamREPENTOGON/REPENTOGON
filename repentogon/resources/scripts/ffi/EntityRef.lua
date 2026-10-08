ffi.cdef [[
    struct EntityRef {
        int Type;
        int Variant;
        int SpawnerType;
        int SpawnerVariant;
        struct Vector Position;
        private struct Vector Velocity; // No idea why John Nicalis didn't expose this
        private int Flags;
        private struct Entity* EntityValue;
    };

    typedef struct EntityRef* EntityRefPtr;

    void L_EntityRef_Init(struct EntityRef*, void*);
]]

local repentogon = ffidll

local FLAG_CHARMED = 1
local FLAG_FRIENDLY = 2

local ffi = ffi
local EntityRefMT
EntityRefMT = {
    __type = "EntityRef",
}

EntityRefMT.__index = function(self, key)
    if key == "Entity" then
        return ffi.getprivate(self, "EntityValue")
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
        ffi.setprivate(self, "EntityValue", ffichecks.entitytopointer(value))
        return
    end
    if key == "IsCharmed" then
        value = ffichecks.checkboolean(1, value)
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
        value = ffichecks.checkboolean(1, value)
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
    __call = function(_, entity)
        local ref = EntityRefT()
        repentogon.L_EntityRef_Init(ref, ffichecks.entitytopointer(entity))
        return ref
    end,
})
