ffi.cdef [[
    struct EntitySmartPtr {
        private struct Entity* RefValue : 0x0;
    } : 0x4;

    void L_EntityPtr_SetReference(struct EntitySmartPtr*, void*);
]]

local ffi = ffi
local repentogon = ffidll

local EntityToPointer = ffichecks.entitytopointer

local EntityPtrMT
EntityPtrMT = {
    __type = "EntityPtr",
    __gc = function(self)
        repentogon.L_EntityPtr_SetReference(self, nil)
    end,
    SetReference = function(self, entity)
        repentogon.L_EntityPtr_SetReference(self, EntityToPointer(entity))
    end,
}

EntityPtrMT.__index = function(self, key)
    if key == "Ref" then
        local result = ffi.getprivate(self, "RefValue") return result
    end
    return EntityPtrMT[key]
end

local EntityPtrT = ffi.metatype("struct EntitySmartPtr", EntityPtrMT)

EntityPtr = setmetatable({}, {
    __class = EntityPtrMT,
    __call = function(_, entity)
        local ptr = EntityPtrT()
        repentogon.L_EntityPtr_SetReference(ptr, EntityToPointer(entity))
        return ptr
    end,
})
