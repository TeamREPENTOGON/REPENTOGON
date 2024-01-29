ffi.cdef[[
void L_SFXManager_AdjustPitch(unsigned int, float);
void L_SFXManager_AdjustVolume(unsigned int, float);
float L_SFXManager_GetAmbientSoundVolume(unsigned int);
bool L_SFXManager_IsPlaying(unsigned int);
void L_SFXManager_Play(unsigned int, float, int, bool, float, float);
void L_SFXManager_Preload(unsigned int);
void L_SFXManager_SetAmbientSound(unsigned int, float, float);
void L_SFXManager_Stop(unsigned int);
void L_SFXManager_StopLoopingSounds();
]]

-- This doesn't need to be a real C++ type, so it won't be.

local repentogon = ffidll
local lffi = ffi

local SFXFuncs = {}

function SFXFuncs:AdjustPitch(id, pitch)
	ffichecks.checknumber(1, id)
	ffichecks.checknumber(2, pitch)

	repentogon.L_SFXManager_AdjustPitch(id, pitch)
end

function SFXFuncs:AdjustVolume(id, volume)
	ffichecks.checknumber(1, id)
	ffichecks.checknumber(2, volume)

	repentogon.L_SFXManager_AdjustVolume(id, volume)
end

function SFXFuncs:GetAmbientSoundVolume(id)
	ffichecks.checknumber(1, id)

	return repentogon.L_SFXManager_GetAmbientSoundVolume(id)
end

function SFXFuncs:IsPlaying(id)
	ffichecks.checknumber(1, id)

	return repentogon.L_SFXManager_IsPlaying(id)
end

function SFXFuncs:Play(id, volume, frameDelay, loop, pitch, pan)
	ffichecks.checknumber(1, id)
	if volume ~= nil then ffichecks.checknumber(2, volume) end
	if frameDelay ~= nil then ffichecks.checknumber(3, frameDelay) end
	if loop ~= nil then ffichecks.checkboolean(4, loop) end
	if pitch ~= nil then ffichecks.checknumber(5, pitch) end
	if pan ~= nil then ffichecks.checknumber(6, pan) end

	repentogon.L_SFXManager_Play(id, volume or 1, frameDelay or 2, loop or false, pitch or 1, pan or 0)
end

function SFXFuncs:Preload(id)
	ffichecks.checknumber(1, id)

	repentogon.L_SFXManager_Preload(id)
end

function SFXFuncs:SetAmbientSound(id, volume, pitch)
	ffichecks.checknumber(1, id)
	ffichecks.checknumber(2, volume)
	ffichecks.checknumber(3, pitch)

	repentogon.L_SFXManager_SetAmbientSound(id, volume, pitch)
end

function SFXFuncs:Stop(id)
	ffichecks.checknumber(1, id)

	repentogon.L_SFXManager_Stop(id)
end

function SFXFuncs:StopLoopingSounds()
	repentogon.L_SFXManager_StopLoopingSounds()
end

local SFXMT
SFXMT = {
	__index = SFXFuncs
}

SFXManager = setmetatable({}, {
	__call = function()
        return setmetatable({}, SFXMT)
    end
})