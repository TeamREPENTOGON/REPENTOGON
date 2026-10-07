ffi.cdef [[
    struct OptionsConfig {
        float MusicVolume : 0x18;
        float SFXVolume : 0x1c;
        float MapOpacity : 0x20;
        bool Fullscreen : 0x24;
        bool Filter : 0x25;
        float Gamma : 0x28;
        float Exposure : 0x2c;
        float Brightness : 0x30;
        float Contrast : 0x34;
        int DisplayPopups : 0x3c;
        bool FoundHUD : 0x40;
        int CameraStyle : 0x44;
        int ExtraHUDStyle : 0x48;
        float HUDOffset : 0x4c;
        bool RumbleEnabled : 0x52;
        bool ChargeBars : 0x53;
        bool BulletVisibility : 0x54;
        bool AimLockEnabled : 0x55;
        int TouchMode : 0x58;
        bool DebugConsoleEnabled : 0x5c;
        bool VSync : 0x5d;
        bool PauseOnFocusLost : 0x5f;
        bool MouseControl : 0x60;
        uint32_t MaxScale : 0x64;
        uint32_t MaxRenderScale : 0x68;
        int WindowWidth : 0x6c;
        int WindowHeight : 0x70;
        int WindowPosX : 0x74;
        int WindowPosY : 0x78;
        uint32_t ConsoleFont : 0x7c;
        bool FadedConsoleDisplay : 0x80;
        bool SaveCommandHistory : 0x81;
        bool UseBorderlessFullscreen : 0x82;
        bool BossHPOnBottom : 0x8e;
        int AnnouncerVoiceMode : 0x90;
        int JacobEsauControls : 0x94;
        bool AscentVoiceOver : 0x98;
        int OnlineHUD : 0x9c;
        bool StreamerMode : 0xa0;
        int OnlinePlayerVolume : 0xa4;
        int OnlinePlayerOpacity : 0xa8;
        bool OnlineChatEnabled : 0xac;
        bool OnlineChatFilterEnabled : 0xad;
        int OnlineColorSet : 0xb0;
        int OnlineInputDelay : 0xb4;
    } : 0xbc;

    struct OptionsConfig* L_Options_Get();
    const char* L_Options_GetLanguage();
    void L_Options_SetFullscreen(bool);
    void L_Options_SetVSync(bool);
    void L_Options_SetMusicVolume(float);
    void L_Options_ClearSFXVolumeModifier();
    bool L_Options_GetRepentogonOption(int);
    void L_Options_SetRepentogonOption(int, bool);
]]

local ffi = ffi
local repentogon = ffidll

local options
local function Config()
    if options == nil then
        options = repentogon.L_Options_Get()
    end
    return options
end

local getters = {}
local setters = {}

local function CheckedBool(name, writable)
    getters[name] = function()
        return Config()[name]
    end
    if writable then
        setters[name] = function(value)
            Config()[name] = ffichecks.checkboolean(1, value)
        end
    end
end

local function Number(name, writable)
    getters[name] = function()
        return Config()[name]
    end
    if writable then
        setters[name] = function(value)
            ffichecks.checknumber(1, value)
            Config()[name] = value
        end
    end
end

local function Integer(name, writable, min, max)
    getters[name] = function()
        return Config()[name]
    end
    if writable then
        setters[name] = function(value)
            ffichecks.checkinteger(1, value)
            if min and value < min then
                value = min
            end
            if max and value > max then
                value = max
            end
            Config()[name] = value
        end
    end
end

local function Clamped(name, min, max, apply)
    getters[name] = function()
        return Config()[name]
    end
    setters[name] = function(value)
        ffichecks.checknumber(1, value)
        if value < min then
            value = min
        end
        if value > max then
            value = max
        end
        if apply then
            apply(value)
        else
            Config()[name] = value
        end
    end
end

local function Unsigned(name, min, max)
    Integer(name, true, min, max)
end

getters.Language = function()
    return ffi.string(repentogon.L_Options_GetLanguage())
end
setters.Language = function()
    error("'Language' is read-only", 0)
end

Integer("JacobEsauControls", true, 0, 1)
CheckedBool("DebugConsoleEnabled", true)
CheckedBool("UseBorderlessFullscreen", true)
CheckedBool("SaveCommandHistory", true)
CheckedBool("FadedConsoleDisplay", true)
Unsigned("ConsoleFont", 0, 2)
Unsigned("MaxRenderScale", 1, 99)
Unsigned("MaxScale", 1, 99)
Integer("AnnouncerVoiceMode", true, 0, 2)
CheckedBool("MouseControl", true)
CheckedBool("PauseOnFocusLost", true)
CheckedBool("BulletVisibility", true)
CheckedBool("ChargeBars", true)
CheckedBool("RumbleEnabled", true)
CheckedBool("FoundHUD", true)
Integer("ExtraHUDStyle", true, 0, 2)
Integer("CameraStyle", true, 1, 2)
Integer("DisplayPopups", true, 0, 2)
Clamped("Gamma", 0.5, 1.5)
CheckedBool("Filter", true)

getters.VSync = function()
    return Config().VSync
end
setters.VSync = function(value)
    repentogon.L_Options_SetVSync(not not value)
end

getters.Fullscreen = function()
    return Config().Fullscreen
end
setters.Fullscreen = function(value)
    repentogon.L_Options_SetFullscreen(not not value)
end

Clamped("HUDOffset", 0, 1)
Clamped("MapOpacity", 0, 1)
Clamped("MusicVolume", 0, 1, function(value)
    repentogon.L_Options_SetMusicVolume(value)
end)

getters.SFXVolume = function()
    return Config().SFXVolume
end
setters.SFXVolume = function(value)
    ffichecks.checknumber(1, value)
    if value < 0 then
        value = 0
    end
    if value > 1 then
        value = 1
    end

    Config().SFXVolume = math.floor(value * 10 + 0.5) / 10
    repentogon.L_Options_ClearSFXVolumeModifier()
end

Integer("OnlineHUD")
CheckedBool("StreamerMode")
CheckedBool("OnlineChatEnabled")
Integer("OnlinePlayerVolume")
Integer("OnlinePlayerOpacity")
Integer("OnlineColorSet")
Integer("OnlineInputDelay")
Number("Exposure", true)
Number("Brightness", true)
Number("Contrast", true)
CheckedBool("BossHPOnBottom", true)
Integer("TouchMode", true)
CheckedBool("AimLockEnabled", true)
CheckedBool("AscentVoiceOver", true)
CheckedBool("OnlineChatFilterEnabled")
Integer("WindowWidth", true)
Integer("WindowHeight", true)
Integer("WindowPosX", true, 8, 65535)
Integer("WindowPosY", true, 8, 65535)

local function RepentogonOption(name, index, writable)
    getters[name] = function()
        return repentogon.L_Options_GetRepentogonOption(index)
    end
    if writable then
        setters[name] = function(value)
            repentogon.L_Options_SetRepentogonOption(index, ffichecks.checkboolean(1, value))
        end
    end
end

RepentogonOption("BetterVoidGeneration", 0, true)
RepentogonOption("HushPanicStateFix", 1, true)
RepentogonOption("StatHUDPlanetarium", 2, true)
RepentogonOption("QuickRoomClear", 3, true)
RepentogonOption("PreventModUpdates", 4, false)

local OptionsT = {
    __propget = getters,
    __propset = setters,
}
OptionsT.__index = function(_, key)
    local getter = OptionsT.__propget[key]
    if getter then
        return getter()
    end
end
OptionsT.__newindex = function(_, key, value)
    local setter = OptionsT.__propset[key]
    if setter then
        setter(value)
        return
    end
    error(string.format("no writable variable '%s'", tostring(key)), 0)
end

Options = setmetatable(OptionsT, OptionsT)
