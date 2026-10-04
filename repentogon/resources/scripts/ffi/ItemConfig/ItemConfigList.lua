ffi.cdef [[
    struct ItemConfigList {
        private struct ItemConfigItem** Begin : 0x0;
        private struct ItemConfigItem** End : 0x4;
        private struct ItemConfigItem** Capacity : 0x8;
    } : 0xc;
    typedef struct ItemConfigList* ItemConfigListPtr;
]]

local ffi = ffi

local function GetSize(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "Begin"), ffi.getprivate(self, "End"), 4)
end

local ItemConfigListMT
ItemConfigListMT = {
    __type = "ItemConfigList",

    __index = function(self, key)
        if key == "Size" then
            return GetSize(self)
        end
        return ItemConfigListMT[key]
    end,

    __len = GetSize,

    Get = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 0 or index >= GetSize(self) then
            ffichecks.argerror(1, "invalid vector subscript")
        end
        return ffi.getprivate(self, "Begin")[index]
    end,
}

ffi.metatype("struct ItemConfigList", ItemConfigListMT)

ItemConfigList = setmetatable({}, {__class = ItemConfigListMT})
