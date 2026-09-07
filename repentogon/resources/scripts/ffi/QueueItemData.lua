ffi.cdef[[
struct QueueItemData {
    struct ItemConfigItem* Item;
    int Charge;
    private unsigned int _flags;
    padding char[0x4];
};

typedef struct QueueItemData* QueueItemDataPtr;
]]

local TOUCHED_FLAG = 0x1

local ffi = ffi

local QueueItemDataMT
QueueItemDataMT = {
    __type = "QueueItemData",
}

QueueItemDataMT.__index = function(self, key)
    if key == "Touched" then
        return ffi.getprivate(self, "_flags") & TOUCHED_FLAG ~= 0
    end

    return QueueItemDataMT[key]
end

QueueItemDataMT.__newindex = function(self, key, value)
    if key == "Touched" then
        local currentFlag = ffi.getprivate(self, "_flags")

        if value == true then
            ffi.setprivate(self, "_flags", currentFlag | TOUCHED_FLAG)
        else
            ffi.setprivate(self, "_flags", currentFlag & ~TOUCHED_FLAG)
        end

        return
    end

    return QueueItemDataMT[key]
end

local QueueItemDataT = ffi.metatype("struct QueueItemData", QueueItemDataMT)
