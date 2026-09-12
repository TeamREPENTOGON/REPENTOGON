ffi.cdef[[
struct SourceQuad {
    private struct Vector TopLeft;
    private struct Vector TopRight;
    private struct Vector BottomLeft;
    private struct Vector BottomRight;
    private int CoordinateSpace;
};

typedef struct SourceQuad* SourceQuadPtr;
]]

local ffi = ffi

local CoordinateSpace = {
    PIXEL = 0,
    NORMALIZED_UV = 1,
}

local SourceQuadMT
SourceQuadMT = {
    __type = "SourceQuad",
    Copy = function(self) 
        return SourceQuadT(self)
    end,
    IsUVSpace = function(self) 
        return ffi.getprivate(self, "CoordinateSpace") == CoordinateSpace.NORMALIZED_UV 
    end,
    ConvertToPixelSpace = function(self, image) 
        if ffi.getprivate(self, "CoordinateSpace") == CoordinateSpace.PIXEL then return end
        ffichecks.checkcdata(1, image, "Image")

        local conv = Vector(image:GetWidth(), image:GetHeight())

        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            ffi.setprivate(self, field, ffi.getprivate(self, field) * conv)
        end 
        ffi.setprivate(self, "CoordinateSpace", CoordinateSpace.PIXEL)
    end,
    ConvertToUVSpace = function(self, image) 
        if ffi.getprivate(self, "CoordinateSpace") == CoordinateSpace.NORMALIZED_UV then return end
        ffichecks.checkcdata(1, image, "Image")

        local conv = Vector(1 / image:GetWidth(), 1 / image:GetHeight())

        for _, field in ipairs("TopLeft", "TopRight", "BottomLeft", "BottomRight") do
            ffi.setprivate(self, field, ffi.getprivate(self, field) * conv)
        end
        ffi.setprivate(self, "CoordinateSpace", CoordinateSpace.NORMALIZED_UV)
    end,
    __tostring = function(self)
        return string.format("[SourceQuad: TopLeft %f %f | TopRight %f %f | BottomLeft %f %f | BottomRight %f %f | UV %s]",
            ffi.getprivate(self, "TopLeft").x, ffi.getprivate(self, "TopLeft").y,
            ffi.getprivate(self, "TopRight").x, ffi.getprivate(self, "TopRight").y,
            ffi.getprivate(self, "BottomLeft").x, ffi.getprivate(self, "BottomLeft").y,
            ffi.getprivate(self, "BottomRight").x, ffi.getprivate(self, "BottomRight").y,
            ffi.getprivate(self, "CoordinateSpace") == CoordinateSpace.NORMALIZED_UV and true or false)
     end,
}

setmetatable(SourceQuadMT, {
    __index = getmetatable(DestinationQuad).__class,
    __call  = function(t, _, k) return t[k] end,
})

SourceQuadMT.__index = SourceQuadMT
SourceQuadT = ffi.metatype("struct SourceQuad", SourceQuadMT)
SourceQuad = setmetatable({ 
    NewFromBounds = function(TopLeft, BottomRight, UV)
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checkcdata(2, BottomRight, "Vector")
        ffichecks.checkboolean(3, UV)
        local sq = SourceQuadT(
            TopLeft,
            Vector(BottomRight.X, TopLeft.Y),
            Vector(TopLeft.X, BottomRight.Y),
            BottomRight,
            UV and CoordinateSpace.NORMALIZED_UV or CoordinateSpace.PIXEL
        )
        return sq
    end, 
    NewFromRectangle = function(TopLeft, Width, Height)
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checknumber(2, Width)
        ffichecks.checknumber(3, Height)
        local sq = SourceQuadT(
            TopLeft,
            Vector(TopLeft.X + Width, TopLeft.Y),
            Vector(TopLeft.X, TopLeft.Y + Height),
            Vector(TopLeft.X + Width, TopLeft.Y + Height),
            UV and CoordinateSpace.NORMALIZED_UV or CoordinateSpace.PIXEL
        )
        return sq
    end, 
}, {
    __class = SourceQuadMT,
        __call = function(_, TopLeft, BottomLeft, TopRight, BottomRight, UV) 
        ffichecks.checkcdata(1, TopLeft, "Vector")
        ffichecks.checkcdata(2, TopRight, "Vector")
        ffichecks.checkcdata(3, BottomLeft, "Vector")
        ffichecks.checkcdata(4, BottomRight, "Vector")
        ffichecks.optboolean(UV, false)
        local sq = SourceQuadT(TopLeft, TopRight, BottomLeft, BottomRight, UV and CoordinateSpace.NORMALIZED_UV or CoordinateSpace.PIXEL) 
        return sq 
    end,
})