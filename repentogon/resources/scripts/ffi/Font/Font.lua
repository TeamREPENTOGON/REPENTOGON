ffi.cdef [[
    struct Font {
        private bool Loaded : 0x0;
        private uint16_t LineHeight : 0x14;
        private uint16_t BaselineHeight : 0x16;
    } : 0x4c;
    typedef struct Font* FontPtr;

    void L_Font_Init(struct Font*);
    void L_Font_Destroy(struct Font*);
    void L_Font_Load(struct Font*, const char*);
    void L_Font_Unload(struct Font*);
    int L_Font_GetCharacterWidth(struct Font*, int);
    int L_Font_GetStringWidth(struct Font*, const char*);
    void L_Font_SetMissingCharacter(struct Font*, int);
    void L_Font_DrawString(struct Font*, const char*, float, float, float, float, struct KColor*, struct FontRenderSettings*);
]]

local repentogon = ffidll
local ffi = ffi

local ALIGN_TOP_LEFT = 0
local ALIGN_TOP_CENTER = 1
local ALIGN_TOP_RIGHT = 2

local function DrawStringScaled(self, str, x, y, scaleX, scaleY, color, boxWidth, center)
    str = ffichecks.checkstring(1, str, 3)
    x = ffichecks.checknumber(2, x, 3)
    y = ffichecks.checknumber(3, y, 3)
    scaleX = ffichecks.checknumber(4, scaleX, 3)
    scaleY = ffichecks.checknumber(5, scaleY, 3)
    ffichecks.checkcdata(6, color, "KColor", false, 3)
    boxWidth = ffichecks.optnumber(boxWidth, 0)

    local align = ALIGN_TOP_LEFT
    if boxWidth ~= 0 then
        if center == true then
            align = ALIGN_TOP_CENTER
            x = x + boxWidth * 0.5
        else
            align = ALIGN_TOP_RIGHT
            x = x + boxWidth
        end
    end

    local settings = FontRenderSettings()
    settings:SetAlignment(align)
    repentogon.L_Font_DrawString(self, str, x, y, scaleX, scaleY, color, settings)
end

local function GetStringWidth(self, str)
    str = ffichecks.checkstring(1, str)
    return repentogon.L_Font_GetStringWidth(self, str)
end

local FontMT
FontMT = {
    __type = "Font",
    __gc = function(self)
        repentogon.L_Font_Destroy(self)
    end,
    DrawString = function(self, str, x, y, param4, param5, param6, param7) -- teehee
        if ffichecks.iscdata(param4, "KColor") then
            DrawStringScaled(self, str, x, y, 1, 1, param4, param5, param6)
            return
        end
        str = ffichecks.checkstring(1, str)
        x = ffichecks.checknumber(2, x)
        y = ffichecks.checknumber(3, y)
        param4 = ffichecks.checknumber(4, param4)
        param5 = ffichecks.checknumber(5, param5)
        ffichecks.checkcdata(6, param6, "KColor")
        ffichecks.checkcdata(7, param7, "FontRenderSettings")
        repentogon.L_Font_DrawString(self, str, x, y, param4, param5, param6, param7)
    end,
    DrawStringScaled = function(self, str, x, y, scaleX, scaleY, color, boxWidth, center)
        DrawStringScaled(self, str, x, y, scaleX, scaleY, color, boxWidth, center)
    end,
    DrawStringScaledUTF8 = function(self, str, x, y, scaleX, scaleY, color, boxWidth, center)
        DrawStringScaled(self, str, x, y, scaleX, scaleY, color, boxWidth, center)
    end,
    DrawStringUTF8 = function(self, str, x, y, color, boxWidth, center)
        DrawStringScaled(self, str, x, y, 1, 1, color, boxWidth, center)
    end,
    GetBaselineHeight = function(self)
        return ffi.getprivate(self, "BaselineHeight")
    end,
    GetCharacterWidth = function(self, character)
        character = ffichecks.checkstring(1, character)
        -- Vanilla passes the first byte as a (signed) char
        local byte = string.byte(character, 1) or 0
        if byte > 127 then
            byte = byte - 256
        end
        return repentogon.L_Font_GetCharacterWidth(self, byte)
    end,
    GetLineHeight = function(self)
        return ffi.getprivate(self, "LineHeight")
    end,
    GetStringWidth = GetStringWidth,
    GetStringWidthUTF8 = GetStringWidth,
    IsLoaded = function(self)
        return ffi.getprivate(self, "Loaded")
    end,
    Load = function(self, path)
        path = ffichecks.checkstring(1, path)
        repentogon.L_Font_Load(self, path)
    end,
    SetMissingCharacter = function(self, character)
        character = ffichecks.checkinteger(1, character)
        repentogon.L_Font_SetMissingCharacter(self, character)
    end,
    Unload = function(self)
        repentogon.L_Font_Unload(self)
    end,
}

setmetatable(FontMT, { __index = function() end })
FontMT.__index = FontMT

local FontT = ffi.metatype("struct Font", FontMT)

Font = setmetatable({}, {
    __call = function()
        local font = FontT()
        repentogon.L_Font_Init(font)
        return font
    end,
    __class = FontMT,
})
