ffi.cdef [[
    struct LevelGeneratorEntry {
        private uint32_t ColIdx : 0x8;
        private uint32_t LineIdx : 0xc;
        private uint32_t Doors : 0x1c;
    } : 0x48;
    typedef struct LevelGeneratorEntry* LevelGeneratorEntryPtr;
]]

local ffi = ffi

local LevelGeneratorEntryMT
LevelGeneratorEntryMT = {
    __type = "LevelGeneratorEntry",
    SetAllowedDoors = function(self, doors)
        doors = ffichecks.checkinteger(1, doors)
        ffi.setprivate(self, "Doors", doors)
    end,
    SetColIdx = function(self, column)
        column = ffichecks.checkinteger(1, column)
        ffi.setprivate(self, "ColIdx", column)
    end,
    SetLineIdx = function(self, line)
        line = ffichecks.checkinteger(1, line)
        ffi.setprivate(self, "LineIdx", line)
    end,
}

setmetatable(LevelGeneratorEntryMT, { __index = function() end })
LevelGeneratorEntryMT.__index = LevelGeneratorEntryMT

local LevelGeneratorEntryT = ffi.metatype("struct LevelGeneratorEntry", LevelGeneratorEntryMT)

LevelGeneratorEntry = setmetatable({}, {
    __class = LevelGeneratorEntryMT,
})

rawset(Isaac, "LevelGeneratorEntry", function()
    return LevelGeneratorEntryT()
end)
