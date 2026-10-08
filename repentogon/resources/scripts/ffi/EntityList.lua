ffi.cdef [[
    struct EntityList_EL {
        private bool Sublist : 0x0;
        private struct Entity** Data : 0x4;
        private unsigned int Capacity : 0x8;
        private unsigned int SizeValue : 0xc;
    } : 0x10;
]]

local ffi = ffi

local EntityListMT
EntityListMT = {
    __type = "EntityList",
    __len = function(self)
        return ffi.getprivate(self, "SizeValue")
    end,
    Get = function(self, index)
        index = ffichecks.checkinteger(1, index)
        local size = ffi.getprivate(self, "SizeValue")
        if size == 0 then
            return nil
        end
        if index < 0 or index >= size then
            index = size - 1
        end
        return ffi.getprivate(self, "Data")[index]
    end,
}

EntityListMT.__index = function(self, key)
    if key == "Size" then
        return ffi.getprivate(self, "SizeValue")
    end
    return EntityListMT[key]
end

ffi.metatype("struct EntityList_EL", EntityListMT)

EntityList = setmetatable({}, { __class = EntityListMT })
