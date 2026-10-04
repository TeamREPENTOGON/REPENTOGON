ffi.cdef [[
    struct PillConfigList {
        private struct ItemConfigPillEffect** Begin : 0x0;
        private struct ItemConfigPillEffect** End : 0x4;
        private struct ItemConfigPillEffect** Capacity : 0x8;
    } : 0xc;
    typedef struct PillConfigList* PillConfigListPtr;
]]

local ffi = ffi

local function GetSize(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "Begin"), ffi.getprivate(self, "End"), 4)
end

local PillConfigListMT
PillConfigListMT = {
    __type = "PillConfigList",

    __index = function(self, key)
        if key == "Size" then
            return GetSize(self)
        end
        return PillConfigListMT[key]
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

ffi.metatype("struct PillConfigList", PillConfigListMT)

PillConfigList = setmetatable({}, {__class = PillConfigListMT})
