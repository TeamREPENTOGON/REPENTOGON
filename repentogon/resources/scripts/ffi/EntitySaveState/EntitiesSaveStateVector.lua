ffi.cdef [[
    struct EntitiesSaveStateVector {
        private struct EntitySaveStateData* First;
        private struct EntitySaveStateData* Last;
        private struct EntitySaveStateData* End;
    };
    typedef struct EntitiesSaveStateVector* EntitiesSaveStateVectorPtr;

    void L_EntitiesSaveStateVector_Clear(struct EntitiesSaveStateVector*);
]]

local repentogon = ffidll
local ffi = ffi

local EntitySaveStateT = ffi.typeof("struct EntitySaveState")
local DATA_SIZE = ffi.sizeof("struct EntitySaveStateData")

local function GetSize(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "First"), ffi.getprivate(self, "Last"), DATA_SIZE)
end

local function MakeHandle(self, index)
    local handle = EntitySaveStateT()
    ffi.setprivate(handle, "Vector", self)
    ffi.setprivate(handle, "Index", index)
    return handle
end

local EntitiesSaveStateVectorMT
EntitiesSaveStateVectorMT = {
    __type = "EntitiesSaveStateVector",
    __len = function(self)
        return GetSize(self)
    end,
    Clear = function(self)
        repentogon.L_EntitiesSaveStateVector_Clear(self)
    end,
    Get = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 0 or index >= GetSize(self) then
            ffichecks.argerror(1, string.format("invalid index for Get(): %d", index))
        end
        return MakeHandle(self, index)
    end,
    GetByType = function(self, type, variant, subType)
        ffichecks.checkinteger(1, type)
        variant = ffichecks.optnumber(variant, 0)
        subType = ffichecks.optnumber(subType, 0)

        local result = {}
        local first = ffi.getprivate(self, "First")
        for i = 0, GetSize(self) - 1 do
            local data = first[i]
            if data.Type == type and (variant == -1 or data.Variant == variant) and (subType == -1 or data.SubType == subType) then
                result[#result + 1] = MakeHandle(self, i)
            end
        end
        return result
    end,
}

setmetatable(EntitiesSaveStateVectorMT, { __index = function() end })
EntitiesSaveStateVectorMT.__index = EntitiesSaveStateVectorMT

ffi.metatype("struct EntitiesSaveStateVector", EntitiesSaveStateVectorMT)

EntitiesSaveStateVector = setmetatable({}, {
    __class = EntitiesSaveStateVectorMT,
})
