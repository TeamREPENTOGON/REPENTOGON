ffi.cdef[[
struct LayerData {
    unsigned int ID;
    unsigned int spritesheetID;
    padding char[0x18]; // std::string name;
    padding char[0x18]; // std::string defaultSpriteSheetPath;
};

struct LayerState {
    private struct LayerData* LayerData;
    private struct Sprite* Animation;
    padding char[0x18]; // std::string SpritesheetFilePath;
    private unsigned int MinFilterMode;
    private unsigned int MagFilterMode;
    private unsigned int WrapSMode;
    private unsigned int WrapTMode;
    private bool SpriteSheetLoaded;
    private bool unk;
    private bool FlipX;
    private bool FlipY;
    private struct Vector Size;
    private float Rotation;
    private struct Vector Pos;
    private struct Color Color;
    private bool Visible;
    padding char[0x3];
    private struct BlendMode BlendMode;
    private unsigned int RenderFlags;
    private struct Vector CropOffset;
    private struct Image Spritesheet;
};

typedef struct LayerState* LayerStatePtr;

void L_LayerState_ClearCustomShader(struct LayerState*, bool);
const char* L_LayerState_GetDefaultSpritesheetPath(struct LayerState*);
const char* L_LayerState_GetName(struct LayerState*);
const char* L_LayerState_GetSpritesheetPath(struct LayerState*);
bool L_LayerState_HasCustomShader(struct LayerState*, bool);
bool L_LayerState_HasCustomShaderWithPath(struct LayerState*, const char*, bool);
bool L_LayerState_SetCustomShader(struct LayerState*, const char*, bool);
]]


local ffi = ffi
local repentogon = ffidll

local LayerStateMT
LayerStateMT = {
    __type = "LayerState",
    ClearCustomChampionShader = function(self)
        repentogon.L_LayerState_ClearCustomShader(self, true)
    end,
    ClearCustomShader = function(self)
        repentogon.L_LayerState_ClearCustomShader(self, false)
    end, 
    GetBlendMode = function(self)
        return ffi.getprivate(self, "BlendMode")
    end,
    GetColor = function(self)
        return ffi.getprivate(self, "Color")
    end,
    GetCropOffset = function(self)
        return ffi.getprivate(self, "CropOffset")
    end,
    GetDefaultSpritesheetPath = function(self)
        return ffi.string(repentogon.L_LayerState_GetDefaultSpritesheetPath(self))
    end,
    GetFlipX = function(self)
        return ffi.getprivate(self, "FlipX")
    end,
    GetFlipY = function(self)
        return ffi.getprivate(self, "FlipY")
    end,
    GetLayerID = function(self)
        return ffi.getprivate(self, "LayerData").ID
    end,
    GetName = function(self)
        return ffi.string(repentogon.L_LayerState_GetName(self))
    end,
    GetPos = function(self)
        return ffi.getprivate(self, "Pos")
    end,
    GetRenderFlags = function(self)
        return ffi.getprivate(self, "RenderFlags")
    end,
    GetRotation = function(self)
        return ffi.getprivate(self, "Rotation")
    end,
    GetSize = function(self)
        return ffi.getprivate(self, "Size")
    end,
    GetSpritesheet = function(self)
        return ffi.getprivate(self, "Spritesheet")
    end,
    GetSpritesheetPath = function(self)
        return ffi.string(repentogon.L_LayerState_GetSpritesheetPath(self))
    end,
    GetWrapSMode = function(self)
        return ffi.getprivate(self, "WrapSMode")
    end,
    GetWrapTMode = function(self)
        return ffi.getprivate(self, "WrapTMode")
    end,
    HasCustomChampionShader = function(self, path)
        path = ffichecks.optstring(path, "")
        if path == "" then
            return repentogon.L_LayerState_HasCustomShader(self, true)
        end
        return repentogon.L_LayerState_HasCustomShaderWithPath(self, path, true)
    end,
    HasCustomShader = function(self, path)
        path = ffichecks.optstring(path, "")
        if path == "" then
            return repentogon.L_LayerState_HasCustomShader(self, false)
        end
        return repentogon.L_LayerState_HasCustomShaderWithPath(self, path, false)
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Visible")
    end,
    SetColor = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        ffi.setprivate(self, "Color", color)
    end,
    SetCropOffset = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        ffi.setprivate(self, "CropOffset", offset)
    end,
    SetCustomChampionShader = function(self, path)
        ffichecks.checkstring(1, path)
        if not repentogon.L_LayerState_SetCustomShader(self, path, true) then 
            error("Failed to load shader: " .. path)
        end
    end,
    SetCustomShader = function(self, path)
        ffichecks.checkstring(1, path)
        if not repentogon.L_LayerState_SetCustomShader(self, path, false) then 
            error("Failed to load shader: " .. path)
        end
    end,
    SetFlipX = function(self, value)
        ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "FlipX", value)
    end,
    SetFlipY = function(self, value)
        ffichecks.checkboolean(1, value)
        ffi.setprivate(self, "FlipY", value)
    end,
    SetPos = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        ffi.setprivate(self, "Pos", pos)
    end,
    SetRenderFlags = function(self, flags)
        ffichecks.checkinteger(1, flags)
        ffi.setprivate(self, "RenderFlags", flags)
    end,
    SetRotation = function(self, rotation)
        ffichecks.checknumber(1, rotation)
        ffi.setprivate(self, "Rotation", rotation)
    end,
    SetSize = function(self, size)
        ffichecks.checkcdata(1, size, "Vector")
        ffi.setprivate(self, "Size", size)
    end,
    SetSpritesheet = function(self, spritesheet)
        ffichecks.checkcdata(1, spritesheet, "Image")
        ffi.setprivate(self, "Spritesheet", spritesheet)
    end,
    SetVisible = function(self, visible)
        ffichecks.checkboolean(1, visible)
        ffi.setprivate(self, "Visible", visible)
    end,
    SetWrapSMode = function(self, mode)
        ffichecks.checkinteger(1, mode)
        ffi.setprivate(self, "WrapSMode", mode)
    end,
    SetWrapTMode = function(self, mode)
        ffichecks.checkinteger(1, mode)
        ffi.setprivate(self, "WrapTMode", mode)
    end,
}

setmetatable(LayerStateMT, {
    __index = function() end,
})
LayerStateMT.__index = LayerStateMT

local LayerStateT = ffi.metatype("struct LayerState", LayerStateMT)
LayerState = setmetatable({}, {
    __class = LayerStateMT,
})