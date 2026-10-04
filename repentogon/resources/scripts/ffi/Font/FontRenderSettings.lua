ffi.cdef [[
    struct FontRenderSettings {
        private int Align : 0x0;
        private uint16_t MaxCharacters : 0x4;
        private uint16_t MaxLineWidth : 0x6;
        private int WrapMode : 0x8;
        private float LineHeightModifier : 0xc;
        private int MissingCharacterOverride : 0x10;
    } : 0x1c;
    typedef struct FontRenderSettings* FontRenderSettingsPtr;
]]

local ffi = ffi

local WRAP_NONE = 0
local WRAP_AUTO = 1
local WRAP_TRUNCATE = 2

local FontRenderSettingsMT
FontRenderSettingsMT = {
    __type = "FontRenderSettings",
    EnableAutoWrap = function(self, width)
        ffichecks.checkinteger(1, width)
        ffi.setprivate(self, "WrapMode", WRAP_AUTO)
        ffi.setprivate(self, "MaxLineWidth", width)
        ffi.setprivate(self, "LineHeightModifier", 1)
    end,
    EnableTruncation = function(self, width)
        ffichecks.checkinteger(1, width)
        ffi.setprivate(self, "WrapMode", WRAP_TRUNCATE)
        ffi.setprivate(self, "MaxLineWidth", width)
    end,
    GetAlignment = function(self)
        return ffi.getprivate(self, "Align")
    end,
    GetLineHeightModifier = function(self)
        return ffi.getprivate(self, "LineHeightModifier")
    end,
    GetMaxCharacters = function(self)
        return ffi.getprivate(self, "MaxCharacters")
    end,
    GetMissingCharacterOverride = function(self)
        return ffi.getprivate(self, "MissingCharacterOverride")
    end,
    IsAutoWrapEnabled = function(self)
        return ffi.getprivate(self, "WrapMode") == WRAP_AUTO
    end,
    IsTruncationEnabled = function(self)
        return ffi.getprivate(self, "WrapMode") == WRAP_TRUNCATE
    end,
    SetAlignment = function(self, alignment)
        ffichecks.checkinteger(1, alignment)
        ffi.setprivate(self, "Align", alignment)
    end,
    SetLineHeightModifier = function(self, modifier)
        ffichecks.checknumber(1, modifier)
        ffi.setprivate(self, "LineHeightModifier", modifier)
    end,
    SetMaxCharacters = function(self, maxCharacters)
        ffichecks.checkinteger(1, maxCharacters)
        ffi.setprivate(self, "MaxCharacters", maxCharacters)
    end,
    SetMissingCharacterOverride = function(self, character)
        ffichecks.checkinteger(1, character)
        ffi.setprivate(self, "MissingCharacterOverride", character)
    end,
}

setmetatable(FontRenderSettingsMT, { __index = function() end })
FontRenderSettingsMT.__index = FontRenderSettingsMT

local FontRenderSettingsT = ffi.metatype("struct FontRenderSettings", FontRenderSettingsMT)

FontRenderSettings = setmetatable({}, {
    __call = function()
        local settings = FontRenderSettingsT()
        ffi.setprivate(settings, "MaxCharacters", 0xffff)
        ffi.setprivate(settings, "WrapMode", WRAP_NONE)
        ffi.setprivate(settings, "LineHeightModifier", 1)
        ffi.setprivate(settings, "MissingCharacterOverride", -1)
        return settings
    end,
    __class = FontRenderSettingsMT,
})
