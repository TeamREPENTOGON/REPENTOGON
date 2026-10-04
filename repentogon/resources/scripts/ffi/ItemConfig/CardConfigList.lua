ffi.cdef [[
    struct CardConfigList {
        private struct ItemConfigCard** Begin : 0x0;
        private struct ItemConfigCard** End : 0x4;
        private struct ItemConfigCard** Capacity : 0x8;
    } : 0xc;
    typedef struct CardConfigList* CardConfigListPtr;
]]

local ffi = ffi

local function GetSize(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "Begin"), ffi.getprivate(self, "End"), 4)
end

local CardConfigListMT
CardConfigListMT = {
    __type = "CardConfigList",

    __index = function(self, key)
        if key == "Size" then
            return GetSize(self)
        end
        return CardConfigListMT[key]
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

ffi.metatype("struct CardConfigList", CardConfigListMT)

CardConfigList = setmetatable({}, {__class = CardConfigListMT})
