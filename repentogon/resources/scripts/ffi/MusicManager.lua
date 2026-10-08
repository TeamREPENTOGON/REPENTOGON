-- Like SFXManager I'm just going to make it a normal Lua table, not a ctype.
ffi.cdef[[
    void L_MusicManager_Crossfade(unsigned int, float);
    void L_MusicManager_Disable();
    void L_MusicManager_DisableLayer(unsigned int);
    void L_MusicManager_Enable();
	void L_MusicManager_EnableLayer(unsigned int, bool);
    void L_MusicManager_Fadein(unsigned int, float, float);
    void L_MusicManager_Fadeout(float);
    int L_MusicManager_GetCurrentMusicID();
    int L_MusicManager_GetQueuedMusicID();
    bool L_MusicManager_IsEnabled();
    bool L_MusicManager_IsLayerEnabled(unsigned int);
    void L_MusicManager_Pause();
    void L_MusicManager_PitchSlide(float);
    void L_MusicManager_Play(unsigned int, float);
    int L_MusicManager_GetCurrentJingleID();
    float L_MusicManager_GetCurrentPitch();
    void L_MusicManager_PlayJingle(unsigned int, int);
    void L_MusicManager_Queue(unsigned int);
    void L_MusicManager_ResetPitch();
    void L_MusicManager_Resume();
    void L_MusicManager_SetCurrentPitch(float);
    void L_MusicManager_StopJingle();
    void L_MusicManager_UpdateVolume();
    bool L_MusicManager_ValidateMusicID(int, int*);
    void L_MusicManager_VolumeSlide(float, float);
]]

local ffi = ffi
local repentogon = ffidll

local function ValidateMusicId(idx, id)
    local max = ffi.new("int[1]")
    if not repentogon.L_MusicManager_ValidateMusicID(id, max) then
        ffichecks.argerror(idx, string.format("Invalid music ID %d. Min = 0, Max = %d", id, max[0]), 3)
    end
end

local MusicManagerMT
MusicManagerMT = {
	__type = "MusicManager",
    Crossfade = function(self, id, fadeRate)
        id = ffichecks.checkinteger(1, id)
        ValidateMusicId(1, id)
        fadeRate = ffichecks.optnumber(fadeRate, 0.08);
        repentogon.L_MusicManager_Crossfade(id, fadeRate);
    end,
    Disable = function(self)
        repentogon.L_MusicManager_Disable();
    end,
    DisableLayer = function(self, id)
        id = ffichecks.checkinteger(1, id)
        repentogon.L_MusicManager_DisableLayer(id)
    end,
    Enable = function(self)
        repentogon.L_MusicManager_Enable()
    end,
    EnableLayer = function(self, layerId, instant)
        layerId = ffichecks.optnumber(layerId, 0)
        instant = ffichecks.optboolean(instant, false)
        repentogon.L_MusicManager_EnableLayer(layerId, instant)
    end,
    Fadein = function(self, id, volume, fadeRate)
        id = ffichecks.checkinteger(1, id)
        ValidateMusicId(1, id)
        volume = ffichecks.optnumber(volume, 1)
        fadeRate = ffichecks.optnumber(fadeRate, 0.08)
        repentogon.L_MusicManager_Fadein(id, volume, fadeRate)
    end,
    Fadeout = function(self, fadeRate)
        fadeRate = ffichecks.optnumber(fadeRate, 0.08)
        repentogon.L_MusicManager_Fadeout(fadeRate)
    end,
    GetCurrentJingleID = function(self)
        return repentogon.L_MusicManager_GetCurrentJingleID()
    end,
    GetCurrentMusicID = function(self)
        return repentogon.L_MusicManager_GetCurrentMusicID()
    end,
    GetCurrentPitch = function(self)
        return repentogon.L_MusicManager_GetCurrentPitch()
    end,
    GetQueuedMusicID = function(self)
        return repentogon.L_MusicManager_GetQueuedMusicID()
    end,
    IsEnabled = function(self)
        return repentogon.L_MusicManager_IsEnabled()
    end,
    IsLayerEnabled = function(self, layerId)
        layerId = ffichecks.optnumber(layerId, 0)
        return repentogon.L_MusicManager_IsLayerEnabled(layerId)
    end,
    Pause = function(self)
        repentogon.L_MusicManager_Pause()
    end,
    PitchSlide = function(self, targetPitch)
        targetPitch = ffichecks.checknumber(1, targetPitch)
        repentogon.L_MusicManager_PitchSlide(targetPitch)
    end,
    Play = function(self, id, volume)
        id = ffichecks.checkinteger(1, id)
        ValidateMusicId(1, id)
        volume = ffichecks.optnumber(volume, -1)
        repentogon.L_MusicManager_Play(id, volume)
    end,
    PlayJingle = function(self, id, duration)
        id = ffichecks.checkinteger(1, id)
        ValidateMusicId(1, id)
        duration = ffichecks.optnumber(duration, 140)
        repentogon.L_MusicManager_PlayJingle(id, duration)
    end,
    Queue = function(self, id)
        id = ffichecks.checkinteger(1, id)
        ValidateMusicId(1, id)
        repentogon.L_MusicManager_Queue(id)
    end,
    ResetPitch = function(self)
        repentogon.L_MusicManager_ResetPitch()
    end,
    Resume = function(self)
        repentogon.L_MusicManager_Resume()
    end,
    SetCurrentPitch = function(self, pitch)
        pitch = ffichecks.checknumber(1, pitch)
        repentogon.L_MusicManager_SetCurrentPitch(pitch)
    end,
    StopJingle = function(self)
        repentogon.L_MusicManager_StopJingle()
    end,
    UpdateVolume = function(self)
        repentogon.L_MusicManager_UpdateVolume()
    end,
    VolumeSlide = function(self, targetVolume, fadeRate)
        targetVolume = ffichecks.checknumber(1, targetVolume)
        fadeRate = ffichecks.optnumber(fadeRate, 0.08)
        repentogon.L_MusicManager_VolumeSlide(targetVolume, fadeRate)
    end,
}

MusicManager = setmetatable({}, {
	__call = function()
        return MusicManagerMT
    end
})