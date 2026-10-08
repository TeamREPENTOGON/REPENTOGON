ffi.cdef [[
    struct GridEntitiesSaveStateVector {
        private struct GridEntityDesc* First;
        private struct GridEntityDesc* Last;
        private struct GridEntityDesc* End;
    };
    typedef struct GridEntitiesSaveStateVector* GridEntitiesSaveStateVectorPtr;
]]

local ffi = ffi

local function GetSize(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "First"), ffi.getprivate(self, "Last"), ffi.sizeof("struct GridEntityDesc"))
end

local GridEntitiesSaveStateVectorMT
GridEntitiesSaveStateVectorMT = {
    __type = "GridEntitiesSaveStateVector",
    __len = function(self)
        return GetSize(self)
    end,
    Clear = function(self)
        local first = ffi.getprivate(self, "First")
        for i = 0, GetSize(self) - 1 do
            first[i].Type = 0
        end
    end,
    Get = function(self, index)
        index = ffichecks.checkinteger(1, index)
        if index < 0 or index >= GetSize(self) then
            ffichecks.argerror(1, string.format("invalid index for Get(): %d", index))
        end
        return ffi.getprivate(self, "First") + index
    end,
    GetByType = function(self, type)
        type = ffichecks.checkinteger(1, type)

        local result = {}
        local first = ffi.getprivate(self, "First")
        for i = 0, GetSize(self) - 1 do
            if first[i].Type == type then
                result[#result + 1] = first + i
            end
        end
        return result
    end,
}

setmetatable(GridEntitiesSaveStateVectorMT, { __index = function() end })
GridEntitiesSaveStateVectorMT.__index = GridEntitiesSaveStateVectorMT

ffi.metatype("struct GridEntitiesSaveStateVector", GridEntitiesSaveStateVectorMT)

GridEntitiesSaveStateVector = setmetatable({}, {
    __class = GridEntitiesSaveStateVectorMT,
})
