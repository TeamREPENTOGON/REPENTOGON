ffi.cdef[[
	int L_Options_GetAnnouncerVoiceMode();
	void L_Options_SetAnnouncerVoiceMode(int);
    bool L_Options_GetBulletVisibility();
	void L_Options_SetBulletVisibility(bool);
    int L_Options_GetCameraStyle();
	void L_Options_SetCameraStyle(int);
    bool L_Options_GetChargeBars();
	void L_Options_SetChargeBars(bool);
    int L_Options_GetConsoleFont();
	void L_Options_SetConsoleFont(int);
    bool L_Options_GetDebugConsoleEnabled();
	void L_Options_SetDebugConsoleEnabled(bool);
    bool L_Options_GetDisplayPopups();
	void L_Options_SetDisplayPopups(bool);
    int L_Options_GetExtraHUDStyle();
	void L_Options_SetExtraHUDStyle(int);
    bool L_Options_GetFadedConsoleDisplay();
	void L_Options_SetFadedConsoleDisplay(bool);
    bool L_Options_GetFilter();
	void L_Options_SetFilter(bool);
    bool L_Options_GetFoundHUD();
	void L_Options_SetFoundHUD(bool);
    bool L_Options_GetFullscreen();
	void L_Options_SetFullscreen(bool);
    float L_Options_GetGamma();
	void L_Options_SetGamma(float);
    float L_Options_GetHUDOffset();
	void L_Options_SetHUDOffset(float);
    const char* L_Options_GetLanguage();
    float L_Options_GetMapOpacity();
    void L_Options_SetMapOpacity(float);
    int L_Options_GetMaxRenderScale();
	void L_Options_SetMaxRenderScale(int);
    int L_Options_GetMaxScale();
	void L_Options_SetMaxScale(int);
    bool L_Options_GetMouseControl();
	void L_Options_SetMouseControl(bool);
    float L_Options_GetMusicVolume();
	void L_Options_SetMusicVolume(float);
    bool L_Options_GetPauseOnFocusLost();
	void L_Options_SetPauseOnFocusLost(bool);
    bool L_Options_GetRumbleEnabled();
	void L_Options_SetRumbleEnabled(bool);
    bool L_Options_GetSaveCommandHistory();
	void L_Options_SetSaveCommandHistory(bool);
    float L_Options_GetSFXVolume();
	void L_Options_SetSFXVolume(float);
    bool L_Options_GetUseBorderlessFullscreen();
    void L_Options_SetUseBorderlessFullscreen(bool);
    bool L_Options_GetVSync();
    void L_Options_SetVSync(bool);

    bool L_Options_GetBetterVoidGeneration();
    void L_Options_SetBetterVoidGeneration(bool);
    bool L_Options_GetHushPanicStateFix();
    void L_Options_SetHushPanicStateFix(bool);
    bool L_Options_GetKeyMasterDealChance();
    void L_Options_SetKeyMasterDealChance(bool);
    bool L_Options_GetPreventModUpdates();
    void L_Options_SetPreventModUpdates(bool);
    bool L_Options_GetQuickRoomClear();
    void L_Options_SetQuickRoomClear(bool);
]]

local repentogon = ffidll
local lffi = ffi

local function checkclamp(min, max, val, name) 
    if val < min or val > max then error(string.format("%s must be between %g and %g, inclusive", name, min, max)) end
end

local function fixvalue(min, max, val)
    if val < min then val = min
    elseif val > max then val = max end

    val = val * 10;
    val = val + (2^52 + 2^51) - (2^52 + 2^51)
    return val / 10;
end

local getkeys = {

    -- VANILLA

    AnnouncerVoiceMode = repentogon.L_Options_GetAnnouncerVoiceMode,
    BulletVisibility = repentogon.L_Options_GetBulletVisibility,
    CameraStyle = repentogon.L_Options_GetCameraStyle,
    ChargeBars = repentogon.L_Options_GetChargeBars,
    ConsoleFont = repentogon.L_Options_GetConsoleFont,
    DebugConsoleEnabled = repentogon.L_Options_GetDebugConsoleEnabled,
    DisplayPopups = repentogon.L_Options_GetDisplayPopups,
    ExtraHUDStyle = repentogon.L_Options_GetExtraHUDStyle,
    FadedConsoleDisplay = repentogon.L_Options_GetFadedConsoleDisplay,
    Filter = repentogon.L_Options_GetFilter,
    FoundHUD = repentogon.L_Options_GetFoundHUD,
    Fullscreen = repentogon.L_Options_GetFullscreen,
    Gamma = repentogon.L_Options_GetGamma,
    HUDOffset = repentogon.L_Options_GetHUDOffset,
    Language = function()
        return lffi.string(repentogon.L_Options_GetLanguage())
    end,
    MapOpacity = repentogon.L_Options_GetMapOpacity,
    MaxRenderScale = repentogon.L_Options_GetMaxRenderScale,
    MaxScale = repentogon.L_Options_GetMaxScale,
    MouseControl = repentogon.L_Options_GetMouseControl,
    MusicVolume = repentogon.L_Options_GetMusicVolume,
    PauseOnFocusLost = repentogon.L_Options_GetPauseOnFocusLost,
    RumbleEnabled = repentogon.L_Options_GetRumbleEnabled,
    SaveCommandHistory = repentogon.L_Options_GetSaveCommandHistory,
    SFXVolume = repentogon.L_Options_GetSFXVolume,
    UseBorderlessFullscreen = repentogon.L_Options_GetUseBorderlessFullscreen,
    VSync = repentogon.L_Options_GetVSync,

    -- REPENTOGON

    BetterVoidGeneration = repentogon.L_Options_GetBetterVoidGeneration,
    HushPanicStateFix = repentogon.L_Options_GetHushPanicStateFix,
    KeyMasterDealChance = repentogon.L_Options_GetKeyMasterDealChance,
    PreventModUpdatesx = repentogon.L_Options_GetPreventModUpdates,
    QuickRoomClear = repentogon.L_Options_GetQuickRoomClear
}

-- FFI will automatically reject incompatible types.
-- On the other hand, we want to do some Lua-side checks for some functions so we can cleanly error.
local setkeys = {

    -- VANILLA

    AnnouncerVoiceMode = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 2, val, "Options.AnnouncerVoiceMode")
        repentogon.L_Options_SetAnnouncerVoiceMode(val)
    end,
    BulletVisibility = repentogon.L_Options_SetBulletVisibility,
    CameraStyle = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(1, 2, val, "Options.CameraStyle")
        repentogon.L_Options_SetCameraStyle(val)
    end,
    ChargeBars = repentogon.L_Options_SetChargeBars,
    ConsoleFont = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 2, val, "Options.ConsoleFont")
        repentogon.L_Options_SetConsoleFont(val)
    end,
    DebugConsoleEnabled = repentogon.L_Options_SetDebugConsoleEnabled,
    DisplayPopups = repentogon.L_Options_SetDisplayPopups,
    ExtraHUDStyle = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 2, val, "Options.ExtraHUDStyle")
        repentogon.L_Options_SetExtraHUDStyle(val)
    end,
    FadedConsoleDisplay = repentogon.L_Options_SetFadedConsoleDisplay,
    Filter = repentogon.L_Options_SetFilter,
    FoundHUD = repentogon.L_Options_SetFoundHUD,
    Fullscreen = repentogon.L_Options_SetFullscreen,
    Gamma = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0.5, 1.5, val, "Options.Gamma")
        repentogon.L_Options_SetGamma(val)
    end,
    HUDOffset = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 1, val, "Options.HUDOffset")
        repentogon.L_Options_SetHUDOffset(val)
    end,
    MapOpacity = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 1, val, "Options.MapOpacity")
        repentogon.L_Options_SetMapOpacity(val)
    end,
    MaxRenderScale = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(1, 99, val, "Options.MaxRenderScale")
        repentogon.L_Options_SetMaxRenderScale(val)
    end,
    MaxScale = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(1, 99, val, "Options.MaxScale")
        repentogon.L_Options_SetMaxScale(val)
    end,
    MouseControl = repentogon.L_Options_SetMouseControl,
    MusicVolume = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 1, val, "Options.MusicVolume")
        repentogon.L_Options_SetMusicVolume(fixvalue(0, 1, val))
    end,
    PauseOnFocusLost = repentogon.L_Options_SetPauseOnFocusLost,
    RumbleEnabled = repentogon.L_Options_SetRumbleEnabled,
    SaveCommandHistory = repentogon.L_Options_SetSaveCommandHistory,
    SFXVolume = function(val)
        ffichecks.checknumber(1, val)
        checkclamp(0, 1, val, "Options.SFXVolume")
        val = fixvalue(0, 1, val)
        repentogon.L_Options_SetSFXVolume(fixvalue(0, 1, val))
    end,
    UseBorderlessFullscreen = repentogon.L_Options_SetUseBorderlessFullscreen,
    VSync = repentogon.L_Options_SetVSync,

    -- REPENTOGON

    BetterVoidGeneration = repentogon.L_Options_SetBetterVoidGeneration,
    HushPanicStateFix = repentogon.L_Options_SetHushPanicStateFix,
    KeyMasterDealChance = repentogon.L_Options_SetKeyMasterDealChance,
    PreventModUpdates = repentogon.L_Options_SetPreventModUpdates,
    QuickRoomClear = repentogon.L_Options_SetQuickRoomClear
}

local OptionsMT = {

	__index = function(tbl, key)
        if getkeys[key] ~= nil then
            return getkeys[key]()
        end
    end,
    
    __newindex = function(tbl, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
}

Options = setmetatable({}, OptionsMT)