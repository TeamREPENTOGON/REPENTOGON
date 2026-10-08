ffi.cdef [[
    struct Shape {
        private int Timeout : 0x4;
    }: 0x8;
    
    typedef struct Shape* ShapePtr;
    
    void L_Shape_Capsule(struct Shape*, struct Capsule*);
    void L_Shape_Circle(struct Shape*, struct Vector*, float);
]]

local ffi = ffi
local repentogon = ffidll

local ShapeMT
ShapeMT = {
    __type = "Shape",
    Capsule = function(self, capsule)
        ffichecks.checkcdata(1, capsule, "Capsule")
        repentogon.L_Shape_Capsule(self, capsule)
    end,
    Circle = function(self, position, size)
        ffichecks.checkcdata(1, position, "Vector")
        size = ffichecks.checknumber(2, size)
        repentogon.L_Shape_Circle(self, position, size)
    end,
    GetTimeout = function(self)
        return ffi.getprivate(self, "Timeout")
    end,
    SetTimeout = function(self, timeout)
        timeout = ffichecks.checkinteger(1, timeout)
        ffi.setprivate(self, "Timeout", timeout)
    end,
}

setmetatable(ShapeMT, { __index = function() end })
ShapeMT.__index = ShapeMT

local ShapeT = ffi.metatype("struct Shape", ShapeMT)

Shape = setmetatable({}, {
    __class = ShapeMT,
})
