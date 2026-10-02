ffi.cdef[[
struct VectorList {
    private struct Vector* _first;
    private struct Vector* _last;
    private struct Vector* _end;
};
typedef struct VectorList* VectorListPtr;
]]

local ffi = ffi

local VectorListMT; VectorListMT = { __type = "VectorList" }

local function el_size(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "_first"), ffi.getprivate(self, "_last"), ffi.sizeof("struct Vector"))
end

VectorListMT.__len = function(self) return el_size(self) end

function VectorListMT:Get(idx)
    ffichecks.checkinteger(1, idx)
    if idx < 0 or idx >= el_size(self) then return nil end
    local v = ffi.getprivate(self, "_first")[idx]
    return Vector(v.X, v.Y)
end

setmetatable(VectorListMT, { __index = function() end })
VectorListMT.__index = VectorListMT

ffi.metatype("struct VectorList", VectorListMT)
VectorList = setmetatable({}, { __class = VectorListMT })
