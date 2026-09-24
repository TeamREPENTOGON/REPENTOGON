ffi.cdef [[
    struct LRoomTileDesc {
        private int HighTopLeft[2];
        private int HighBottomRight[2];
        private int LowTopLeft[2];
        private int LowBottomRight[2];
    };

    void L_LRoomTileDesc_GetRandomTile(struct LRoomTileDesc*, int, int[2]);
]]

local repentogon = ffidll
local ffi = ffi

local LRoomTileDescMT
LRoomTileDescMT = {
    __type = "LRoomTileDesc",
    GetHighTopLeft = function(self)
        return {ffi.getprivate(self, "HighTopLeft")[0], ffi.getprivate(self, "HighTopLeft")[1]}
    end,
    GetHighBottomRight = function(self)
        return {ffi.getprivate(self, "HighBottomRight")[0], ffi.getprivate(self, "HighBottomRight")[1]}
    end,
    GetLowTopLeft = function(self)
        return {ffi.getprivate(self, "LowTopLeft")[0], ffi.getprivate(self, "LowTopLeft")[1]}
    end,
    GetLowBottomRight = function(self)
        return {ffi.getprivate(self, "LowBottomRight")[0], ffi.getprivate(self, "LowBottomRight")[1]}
    end,
    GetRandomTile = function(self, seed)
        ffichecks.checkinteger(1, seed)
        if seed == 0 then
            ffichecks.argerror(2, "Seed must be non-zero!")
        end

        local ret = ffi.new("int[2]")
        repentogon.L_LRoomTileDesc_GetRandomTile(self, seed, ret)
        return {ret[0], ret[1]}
    end,
}

setmetatable(LRoomTileDescMT, { __index = function() end })
LRoomTileDescMT.__index = LRoomTileDescMT

local LRoomTileDescT = ffi.metatype("struct LRoomTileDesc", LRoomTileDescMT)

LRoomTileDesc = setmetatable({
}, {
    __class = LRoomTileDescMT,
})
