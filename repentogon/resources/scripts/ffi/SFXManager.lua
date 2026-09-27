-- SFXManager is a singleton with no variables exposed to Lua, there's really no point in it having a ctype.
ffi.cdef[[
void L_SFXManager_AdjustPitch(int, float);
void L_SFXManager_AdjustVolume(int, float);
float L_SFXManager_GetAmbientSoundVolume(int);
bool L_SFXManager_IsPlaying(int);
void L_SFXManager_Play(int, float, int, bool, float, float);
void L_SFXManager_Preload(int);
void L_SFXManager_SetAmbientSound(int, float, float);
void L_SFXManager_Stop(int);
void L_SFXManager_StopLoopingSounds();
]]

local repentogon = ffidll

local SFXManagerMT
SFXManagerMT = {
	__type = "SFXManager",
    AdjustPitch = function(self, id, pitch)
        ffichecks.checkinteger(1, id)
        ffichecks.checknumber(2, pitch)
        repentogon.L_SFXManager_AdjustPitch(id, pitch)
    end,
    AdjustVolume = function(self, id, volume)
        ffichecks.checkinteger(1, id)
        ffichecks.checknumber(2, volume)
        repentogon.L_SFXManager_AdjustVolume(id, volume)
    end,    
    GetAmbientSoundVolume = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_SFXManager_GetAmbientSoundVolume(id)
    end,
    IsPlaying = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_SFXManager_IsPlaying(id)
    end,
    Play = function(self, id, volume, frameDelay, loop, pitch, pan)
        ffichecks.checkinteger(1, id)
        volume = ffichecks.optnumber(volume, 1)
        frameDelay = ffichecks.optnumber(frameDelay, 2)
        loop = ffichecks.optboolean(loop, false)
        pitch = ffichecks.optnumber(pitch, 1)
        pan = ffichecks.optnumber(pan, 0)
        repentogon.L_SFXManager_Play(id, volume, frameDelay, loop, pitch, pan)
    end,
    Preload = function(self, id)
        ffichecks.checkinteger(1, id)
        repentogon.L_SFXManager_Preload(id)
    end,
    SetAmbientSound = function(self, id, volume, pitch)
        ffichecks.checkinteger(1, id)
        ffichecks.checknumber(2, volume)
        ffichecks.checknumber(3, pitch)
        repentogon.L_SFXManager_SetAmbientSound(id, volume, pitch)
    end,
    Stop = function(self, id)
        ffichecks.checkinteger(1, id)
        repentogon.L_SFXManager_Stop(id)
    end,
    StopLoopingSounds = function(self)
        repentogon.L_SFXManager_StopLoopingSounds()
    end,
}

SFXManager = setmetatable({}, {
	__call = function()
        return SFXManagerMT
    end
})