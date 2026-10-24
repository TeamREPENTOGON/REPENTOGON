ffi.cdef [[
    struct Point {
        private struct Vector Position : 0x0;
        private float Width : 0x8;
        private float SpritesheetCoordinate : 0xc;
        private struct Color Color : 0x10;
        private bool WorldSpace : 0x3c;
    } : 0x40;
    typedef struct Point* PointPtr;

    struct Beam {
        private struct Sprite Sprite : 0x0;
        private unsigned int Layer : 0x118;
        private bool UseOverlay : 0x11c;
        private bool UnkBool : 0x11d;
    } : 0x140;
    typedef struct Beam* BeamPtr;

    void L_Beam_Init(struct Beam*, struct Sprite*, int, bool, bool);
    void L_Beam_Destroy(struct Beam*);
    void L_Beam_Add(struct Beam*, struct Point*);
    int L_Beam_Render(struct Beam*, bool);
    void L_Beam_SetSprite(struct Beam*, struct Sprite*);
    void L_Beam_SetLayer(struct Beam*, int);
    unsigned int L_Beam_GetPointCount(struct Beam*);
    void L_Beam_GetPoints(struct Beam*, struct Point*);
    void L_Beam_SetPoints(struct Beam*, struct Point*, unsigned int);
]]

local repentogon = ffidll
local ffi = ffi

local function OptColor(color)
    return ffichecks.optcdata(color, "Color") or Color()
end


local PointMT
PointMT = {
    __type = "Point",
    GetColor = function(self)
        local result = ffi.new("struct Color", ffi.getprivate(self, "Color")) return result
    end,
    GetIsWorldSpace = function(self)
        local result = ffi.getprivate(self, "WorldSpace") return result
    end,
    GetPosition = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "Position"))
    end,
    GetSpritesheetCoordinate = function(self)
        local result = ffi.getprivate(self, "SpritesheetCoordinate") return result
    end,
    GetWidth = function(self)
        local result = ffi.getprivate(self, "Width") return result
    end,
    SetColor = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        ffi.setprivate(self, "Color", color)
    end,
    SetIsWorldSpace = function(self, worldSpace)
        worldSpace = ffichecks.checkboolean(1, worldSpace)
        ffi.setprivate(self, "WorldSpace", worldSpace)
    end,
    SetPosition = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        ffi.setprivate(self, "Position", position)
    end,
    SetSpritesheetCoordinate = function(self, coordinate)
        ffi.setprivate(self, "SpritesheetCoordinate", tonumber(coordinate) or 0)
    end,
    SetWidth = function(self, width)
        ffi.setprivate(self, "Width", tonumber(width) or 0)
    end,
}

PointMT.GetHeight = PointMT.GetSpritesheetCoordinate
PointMT.SetHeight = PointMT.SetSpritesheetCoordinate

setmetatable(PointMT, { __index = function() end })
PointMT.__index = PointMT

local PointT = ffi.metatype("struct Point", PointMT)

local function NewPoint(position, coordinate, width, color, worldSpace)
    local point = PointT()
    ffi.setprivate(point, "Position", position)
    ffi.setprivate(point, "SpritesheetCoordinate", coordinate)
    ffi.setprivate(point, "Width", width)
    ffi.setprivate(point, "Color", OptColor(color))
    ffi.setprivate(point, "WorldSpace", worldSpace)
    return point
end

Point = setmetatable({}, {
    __call = function(_, position, coordinate, width, color, worldSpace)
        ffichecks.checkcdata(1, position, "Vector")
        coordinate = ffichecks.checknumber(2, coordinate)
        return NewPoint(position, coordinate, ffichecks.optnumber(width, 1), color, ffichecks.optboolean(worldSpace, false))
    end,
    __class = PointMT,
})

-------

local RENDER_ERRORS = {
    [0] = "Must have at least two points",
    [1] = "Overlay AnimState is NULL!",
    [2] = "AnimState is NULL!",
    [3] = "Invalid layer id",
}

local function GetLayerByName(sprite, idx, name, level)
    local layer = sprite:GetLayer(name)
    if not layer then
        ffichecks.argerror(idx, string.format("invalid layer name %s", name), level)
    end
    return layer:GetLayerID()
end

local function CheckLayerID(sprite, idx, layerID, level)
    if layerID < 0 or layerID >= sprite:GetLayerCount() then
        ffichecks.argerror(idx, string.format("invalid layer ID %d", layerID), level)
    end
    return layerID
end

local function ResolveLayer(sprite, idx, layer)
    if type(layer) == "string" then
        return GetLayerByName(sprite, idx, layer, 4)
    end
    layer = ffichecks.checkinteger(idx, layer, 3)
    return CheckLayerID(sprite, idx, layer, 4)
end

local BeamMT
BeamMT = {
    __type = "Beam",
    __gc = function(self)
        repentogon.L_Beam_Destroy(self)
    end,
    Add = function(self, ...)
        if select("#", ...) == 1 then
            local point = ...
            ffichecks.checkcdata(1, point, "Point")
            repentogon.L_Beam_Add(self, point)
            return
        end

        local position, coordinate, width, color, worldSpace = ...
        ffichecks.checkcdata(1, position, "Vector")
        local point = NewPoint(position, ffichecks.optnumber(coordinate, 0), ffichecks.optnumber(width, 1), color,
            ffichecks.optboolean(worldSpace, false))
        repentogon.L_Beam_Add(self, point)
    end,
    GetLayer = function(self)
        local result = ffi.getprivate(self, "Layer") return result
    end,
    GetPoints = function(self)
        local count = repentogon.L_Beam_GetPointCount(self)
        local points = {}
        if count > 0 then
            local buffer = ffi.new("struct Point[?]", count)
            repentogon.L_Beam_GetPoints(self, buffer)
            for i = 0, count - 1 do
                points[i + 1] = PointT(buffer[i])
            end
        end
        return points
    end,
    GetSprite = function(self)
        local result = ffi.getprivate(self, "Sprite") return result
    end,
    GetUnkBool = function(self)
        local result = ffi.getprivate(self, "UnkBool") return result
    end,
    GetUseOverlay = function(self)
        local result = ffi.getprivate(self, "UseOverlay") return result
    end,
    Render = function(self, clearPoints)
        local err = repentogon.L_Beam_Render(self, ffichecks.optboolean(clearPoints, true))
        if err >= 0 then
            error(RENDER_ERRORS[err], 2)
        end
    end,
    SetLayer = function(self, layer)
        repentogon.L_Beam_SetLayer(self, ResolveLayer(ffi.getprivate(self, "Sprite"), 1, layer))
    end,
    SetPoints = function(self, points)
        ffichecks.checktable(1, points)
        local count = #points
        if count < 2 then
            ffichecks.argerror(1, "Must have at least two points")
        end
        local buffer = ffi.new("struct Point[?]", count)
        for i = 1, count do
            local point = rawget(points, i)
            ffichecks.checkcdata(1, point, "Point")
            buffer[i - 1] = point
        end
        repentogon.L_Beam_SetPoints(self, buffer, count)
    end,
    SetSprite = function(self, ...)
        local sprite, layer, useOverlay = ...
        ffichecks.checkcdata(1, sprite, "Sprite")
        if select("#", ...) > 1 then
            local layerID = ffi.getprivate(self, "Layer")
            if type(layer) == "string" then
                layerID = GetLayerByName(sprite, 2, layer, 3)
            elseif math.type(layer) == "integer" then
                layerID = CheckLayerID(sprite, 2, layer, 3)
            end
            useOverlay = ffichecks.checkboolean(3, useOverlay)
            ffi.setprivate(self, "UseOverlay", useOverlay)
            ffi.setprivate(self, "Layer", layerID)
        end
        repentogon.L_Beam_SetSprite(self, sprite)
    end,
    SetUnkBool = function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "UnkBool", value)
    end,
    SetUseOverlay = function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "UseOverlay", value)
    end,
}

setmetatable(BeamMT, { __index = function() end })
BeamMT.__index = BeamMT

local BeamT = ffi.metatype("struct Beam", BeamMT)

Beam = setmetatable({}, {
    __call = function(_, ...)
        local count = select("#", ...)
        if count < 4 then
            error(string.format("Expected at least 4 arguments, got %d", count), 2)
        end
        local sprite, layer, useOverlay, unk = ...
        ffichecks.checkcdata(1, sprite, "Sprite")
        local layerID = ResolveLayer(sprite, 2, layer)
        useOverlay = ffichecks.checkboolean(3, useOverlay)
        unk = ffichecks.checkboolean(4, unk)

        local beam = BeamT()
        repentogon.L_Beam_Init(beam, sprite, layerID, useOverlay, unk)
        return beam
    end,
    __class = BeamMT,
})
