ffi.cdef[[
unsigned int L_MusicManager_GetMaxID();
void L_MusicManager_Crossfade(unsigned int, float);
void L_MusicManager_Disable();
void L_MusicManager_DisableLayer(unsigned int);
void L_MusicManager_Enable();
void L_MusicManager_EnableLayer(unsigned int, bool);
void L_MusicManager_Fadein(unsigned int, float, float);
void L_MusicManager_Fadeout(float);
unsigned int L_MusicManager_GetCurrentMusicID();
unsigned int L_MusicManager_GetQueuedMusicID();
bool L_MusicManager_IsEnabled();
bool L_MusicManager_IsLayerEnabled(unsigned int);
void L_MusicManager_Pause();
void L_MusicManager_PitchSlide(float);
void L_MusicManager_Play(unsigned int, float);
void L_MusicManager_PlayJingle(unsigned int);
void L_MusicManager_Queue(unsigned int);
void L_MusicManager_ResetPitch();
void L_MusicManager_Resume();
void L_MusicManager_StopJingle();
void L_MusicManager_UpdateVolume();
void L_MusicManager_VolumeSlide(float, float);
]]

-- This doesn't need to be a real C++ type, so it won't be.

local repentogon = ffidll
local lffi = ffi

local MusicFuncs = {}

local function ValidateMusicID(id)
	local maxID = repentogon.L_MusicManager_GetMaxID()
	if id < 0 or id >= maxID then
		error(string.format("Invalid music ID %d. Min = 0, Max = %d", id, maxID - 1))
	end
end

function MusicFuncs:Crossfade(id, faderate)
	ffichecks.checknumber(1, id)
	ffichecks.checknumber(2, faderate)
	ValidateMusicID(id)

	repentogon.L_MusicManager_Crossfade(id, faderate)
end

function MusicFuncs:Disable()
	repentogon.L_MusicManager_Disable()
end

function MusicFuncs:DisableLayer(id)
	local resID = 0
	if ffichecks.isnumber(1, id) then
		resID = id
	end

	repentogon.L_MusicManager_DisableLayer(resID)
end

function MusicFuncs:Enable()
	repentogon.L_MusicManager_Enable()
end

function MusicFuncs:EnableLayer(id, instant)
	local resID = 0
	local resInstant = false
	if ffichecks.isnumber(1, id) then
		resID = id
	end
	if ffichecks.isboolean(2, instant) then
		resInstant = instant
	end
	
	repentogon.L_MusicManager_EnableLayer(resID, resInstant)
end

function MusicFuncs:Fadein(id, volume, faderate)
	local resVolume = Options.MusicVolume
	local resFaderate = 0.08
	
	ffichecks.checknumber(1, id)
	if ffichecks.isnumber(2, volume) then
		resVolume = volume
	end
	if ffichecks.isnumber(3, faderate) then
		resFaderate = faderate
	end
	ValidateMusicID(id)

	repentogon.L_MusicManager_Fadein(id, resVolume, resFaderate)
end

function MusicFuncs:Fadeout(faderate)
	local resFaderate = 0.08
	if ffichecks.isnumber(1, faderate) then
		resFaderate = faderate
	end

	repentogon.L_MusicManager_Fadeout(resFaderate)
end

function MusicFuncs:GetCurrentMusicID()
	return repentogon.L_MusicManager_GetCurrentMusicID()
end

function MusicFuncs:GetQueuedMusicID()
	return repentogon.L_MusicManager_GetQueuedMusicID()
end

function MusicFuncs:IsEnabled()
	return repentogon.L_MusicManager_IsEnabled()
end

function MusicFuncs:IsLayerEnabled(id)
	local resID = 0
	if ffichecks.isnumber(1, id) then
		resID = id
	end
	
	return repentogon.L_MusicManager_IsLayerEnabled(resID)
end

function MusicFuncs:Pause()
	repentogon.L_MusicManager_Pause()
end

function MusicFuncs:PitchSlide(target)
	ffichecks.checknumber(1, target)

	repentogon.L_MusicManager_PitchSlide(target)
end

function MusicFuncs:Play(id, volume)
	ffichecks.checknumber(1, id)

	local finalVolume = Options.MusicVolume
	if ffichecks.isnumber(2, volume) then
		finalVolume = volume
	end

	ValidateMusicID(id)

	repentogon.L_MusicManager_Play(id, finalVolume)
end

function MusicFuncs:PlayJingle(id, volume)
	ffichecks.checknumber(1, id)
	ValidateMusicID(id)

	repentogon.L_MusicManager_PlayJingle(id)
end

function MusicFuncs:Queue(id)
	ffichecks.checknumber(1, id)
	ValidateMusicID(id)

	repentogon.L_MusicManager_Queue(id)
end

function MusicFuncs:ResetPitch()
	repentogon.L_MusicManager_ResetPitch()
end

function MusicFuncs:Resume()
	repentogon.L_MusicManager_Resume()
end

function MusicFuncs:StopJingle()
	repentogon.L_MusicManager_StopJingle()
end

function MusicFuncs:UpdateVolume()
	repentogon.L_MusicManager_UpdateVolume()
end

function MusicFuncs:VolumeSlide(volume, faderate)
	ffichecks.checknumber(1, volume)
	local resFaderate = 0.08
	if ffichecks.isnumber(1, faderate) then
		resFaderate = faderate
	end

	repentogon.L_MusicManager_VolumeSlide(volume, resFaderate)
end

local MusicMT
MusicMT = {
	__index = MusicFuncs
}

MusicManager = setmetatable({}, {
	__call = function()
        return setmetatable({}, MusicMT)
    end
})
