ffi.cdef[[
struct DestinationQuad {
    private struct Vector TopLeft;
    private struct Vector TopRight;
    private struct Vector BottomLeft;
    private struct Vector BottomRight;
};

typedef struct DestinationQuad* DestinationQuadPtr;
]]

local ffi = ffi

local function get_render_matrix(index, val, level)
    if type(val) ~= "table" then
        error(string.format("bad argument #%d to '%s' (table expected, got %s)",
            index, debug_getinfo(level or 2).name, type(val)), (level or 2) + 1)
    end

    local xRow, yRow = val[1], val[2]
    if type(xRow) ~= "table" or type(yRow) ~= "table" then
        error(string.format("bad argument #%d to '%s' (render matrix row is not a table)",
            index, debug_getinfo(level or 2).name), (level or 2) + 1)
    end

    local a, b, tx = xRow[1], xRow[2], xRow[3]
    local c, d, ty = yRow[1], yRow[2], yRow[3]

    if type(a) ~= "number" or type(b) ~= "number" or type(tx) ~= "number"
    or type(c) ~= "number" or type(d) ~= "number" or type(ty) ~= "number" then
        error(string.format("bad argument #%d to '%s' (render matrix element is not a number!)",
            index, debug_getinfo(level or 2).name), (level or 2) + 1)
    end

    return a, b, tx, c, d, ty
end

local DestinationQuadMT
DestinationQuadMT = {
    __type = "DestinationQuad",
    Copy = function(self) 
        return DestinationQuadT(self)
    end,
    GetTopLeft = function(self) 
        return ffi.getprivate(self, "TopLeft");
    end,
    GetTopRight = function(self) 
        return ffi.getprivate(self, "TopRight");
    end,
    GetBottomLeft = function(self) 
        return ffi.getprivate(self, "BottomLeft");
    end,
    GetBottomRight = function(self) 
        return ffi.getprivate(self, "BottomRight");
    end,
    SetTopLeft = function(self, v) 
        ffichecks.checkcdata(1, v, "Vector")
        ffi.setprivate(self, "TopLeft", v)
    end,
    SetTopRight = function(self, v) 
        ffichecks.checkcdata(1, v, "Vector")
        ffi.setprivate(self, "TopRight", v)
    end,
    SetBottomLeft = function(self, v) 
        ffichecks.checkcdata(1, v, "Vector")
        ffi.setprivate(self, "BottomLeft", v)
    end,
    SetBottomRight = function(self, v) 
        ffichecks.checkcdata(1, v, "Vector")
        ffi.setprivate(self, "BottomRight", v)
    end,
    Translate = function(self, offset) 
        ffichecks.checkcdata(1, offset, "Vector")
        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            ffi.setprivate(self, field, ffi.getprivate(self, field) + offset)
        end
    end,
    Scale = function(self, scale, anchor)
        ffichecks.checkcdata(1, scale, "Vector")
        ffichecks.checkcdata(2, anchor, "Vector")
        self:Translate(-anchor)
        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            ffi.setprivate(self, field, ffi.getprivate(self, field) * scale)
        end
        self:Translate(anchor)
    end,
    Rotate = function(self, rotation, anchor)
        ffichecks.checknumber(1, rotation)
        ffichecks.checkcdata(2, anchor, "Vector")

        local radians = degrees * math.pi / 180
        local sin, cos = math.sin(radians), math.cos(radians)
        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            local p = ffi.getprivate(self, field)
            local v = p - anchor
            p.X = anchor.X + cos * v.X - sin * v.Y
            p.Y = anchor.Y + sin * v.X + cos * v.Y
            ffi.setprivate(self, field, p)
        end
    end,
    Shear = function(self, shear, anchor) 
        ffichecks.checkcdata(1, shear, "Vector")
        ffichecks.checkcdata(2, anchor, "Vector") 
        self:Translate(-anchor)
        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            local p = ffi.getprivate(self, field)
            local x = p.X
            local y = p.Y
            p.X = p.X + shear.X * y
            p.Y = p.Y + shear.Y * x
            ffi.setprivate(self, field, p)
        end
        self:Translate(anchor)
    end,
    ApplyMatrix = function(self, matrix, anchor)
        ffichecks.checktable(1, matrix)
        ffichecks.checkcdata(2, anchor, "Vector")

        local a, b, tx, c, d, ty = get_render_matrix(2, matrix, 2)
        self:Translate(-anchor)
        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            local p = ffi.getprivate(self, field)
            local x = p.X
            local y = p.Y
            p.X = a * x + b * y + tx
            p.Y = c * x + d * y + ty
            ffi.setprivate(self, field, p)
        end
        self:Translate(anchor)
    end,
    Flip = function(self, flipX, flipY) 
        flipX = ffichecks.optboolean(flipX, true)
        flipY = ffichecks.optboolean(flipY, true)

        if flipX then 
            local tl = VectorT(ffi.getprivate(self, "TopLeft"))
            ffi.setprivate(self, "TopLeft", ffi.getprivate(self, "TopRight"))
            ffi.setprivate(self, "TopRight", tl)

            local bl = VectorT(ffi.getprivate(self, "BottomLeft"))
            ffi.setprivate(self, "BottomLeft", ffi.getprivate(self, "BottomRight"))
            ffi.setprivate(self, "BottomRight", tl)
        end
        
        if flipY then 
            local tl = VectorT(ffi.getprivate(self, "TopLeft"))
            ffi.setprivate(self, "TopLeft", ffi.getprivate(self, "BottomLeft"))
            ffi.setprivate(self, "BottomLeft", tl)

            local tr = VectorT(ffi.getprivate(self, "TopRight"))
            ffi.setprivate(self, "TopRight", ffi.getprivate(self, "BottomRight"))
            ffi.setprivate(self, "BottomRight", tr)
        end
    end,
    __tostring = function(self)
        return string.format("[DestQuad: TopLeft %f %f | TopRight %f %f | BottomLeft %f %f | BottomRight %f %f]",
            ffi.getprivate(self, "TopLeft").x, ffi.getprivate(self, "TopLeft").y,
            ffi.getprivate(self, "TopRight").x, ffi.getprivate(self, "TopRight").y,
            ffi.getprivate(self, "BottomLeft").x, ffi.getprivate(self, "BottomLeft").y,
            ffi.getprivate(self, "BottomRight").x, ffi.getprivate(self, "BottomRight").y)
     end,
}

setmetatable(DestinationQuadMT, {
    __index = function() end,
})
DestinationQuadMT.__index = DestinationQuadMT

local DestinationQuadT = ffi.metatype("struct DestinationQuad", DestinationQuadMT)
DestinationQuad = setmetatable({ 
    NewFromBounds = function(TopLeft, BottomRight)
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checkcdata(2, BottomRight, "Vector")
        local dq = DestinationQuadT(
            TopLeft,
            Vector(BottomRight.X, TopLeft.Y),
            Vector(TopLeft.X, BottomRight.Y),
            BottomRight
        )
        return dq
    end, 
    NewFromRectangle = function(TopLeft, Width, Height)
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checknumber(2, Width)
        ffichecks.checknumber(3, Height)
        local dq = DestinationQuadT(
            TopLeft,
            Vector(TopLeft.X + Width, TopLeft.Y),
            Vector(TopLeft.X, TopLeft.Y + Height),
            Vector(TopLeft.X + Width, TopLeft.Y + Height)
        )
        return dq
    end, 
}, {
    __class = DestinationQuadMT,
    __call = function(_, TopLeft, BottomLeft, TopRight, BottomRight) 
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checkcdata(2, TopRight, "Vector")
        ffichecks.checkcdata(3, BottomLeft, "Vector")
        ffichecks.checkcdata(4, BottomRight, "Vector")
        local dq = DestinationQuadT(TopLeft, TopRight, BottomLeft, BottomRight) 
        return dq 
    end,
})