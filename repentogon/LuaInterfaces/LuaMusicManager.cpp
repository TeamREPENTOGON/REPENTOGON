#include "IsaacRepentance.h"
#include "HookSystem.h"


MOD_EXPORT void L_MusicManager_Crossfade(unsigned int id, float fadeRate) {
	g_Manager->_musicmanager.Crossfade(id, fadeRate);
}

MOD_EXPORT void L_MusicManager_Disable() {
	g_Manager->_musicmanager._enabled = false;
}

MOD_EXPORT void L_MusicManager_DisableLayer(unsigned int id) {
	g_Manager->_musicmanager.DisableLayer(id);
}

MOD_EXPORT void L_MusicManager_Enable() {
	g_Manager->_musicmanager._enabled = true;
}

MOD_EXPORT void L_MusicManager_EnableLayer(unsigned int id, bool instant) {
	g_Manager->_musicmanager.EnableLayer(id, instant);
}

MOD_EXPORT void L_MusicManager_Fadein(unsigned int id, float volume, float fadeRate) {
	g_Manager->_musicmanager.Fadein(id, volume, fadeRate);
}

MOD_EXPORT void L_MusicManager_Fadeout(float fadeRate) {
	g_Manager->_musicmanager.Fadeout(fadeRate);
}

MOD_EXPORT int L_MusicManager_GetCurrentJingleID() {
	std::uint16_t ret = g_Manager->_musicmanager._jingleId;
	return g_Manager->_musicmanager._jingleCountdownMaybe < 1 ? 0 : ret;
}

MOD_EXPORT int L_MusicManager_GetCurrentMusicID() {
	return g_Manager->_musicmanager._currentId;
}

MOD_EXPORT int L_MusicManager_GetQueuedMusicID() {
	return g_Manager->_musicmanager._queuedId;
}

MOD_EXPORT float L_MusicManager_GetCurrentPitch() {
	return g_Manager->_musicmanager._pitch;
}

MOD_EXPORT bool L_MusicManager_IsEnabled() {
	return g_Manager->_musicmanager._enabled;
}

MOD_EXPORT bool L_MusicManager_IsLayerEnabled(unsigned int id) {
	return g_Manager->_musicmanager.IsLayerEnabled(id);
}

MOD_EXPORT void L_MusicManager_Pause() {
	g_Manager->_musicmanager.Pause();
}

MOD_EXPORT void L_MusicManager_PitchSlide(float targetPitch) {
	g_Manager->_musicmanager._targetPitch = targetPitch;
}

MOD_EXPORT void L_MusicManager_Play(unsigned int id, float volume) {
	g_Manager->_musicmanager.Play(id, volume);
}

MOD_EXPORT void L_MusicManager_PlayJingle(unsigned int id, int duration) {
	g_Manager->_musicmanager.PlayJingle(id, 140, false);

	//duration was inlined and (at least most) calls to the func had it stripped from the args, just set it ourselves
	g_Manager->_musicmanager._jingleCountdownMaybe = duration;
}

// stuff the jingle id into unused bytes
// Altered priority so that this correctly gets the modified jingle from MC_PRE_MUSIC_PLAY_JINGLE
HOOK_METHOD_PRIORITY(Music, PlayJingle, 1, (int musicId, int unusedInt, bool unusedBool) -> void) {
	super(musicId, unusedInt, unusedBool);
	// if someone's got 65,417 custom songs loaded there are bigger issues
	this->_jingleId = (std::uint16_t)musicId;
}

MOD_EXPORT void L_MusicManager_Queue(unsigned int id) {
	g_Manager->_musicmanager._queuedId = id;
}

MOD_EXPORT void L_MusicManager_ResetPitch() {
	g_Manager->_musicmanager._pitch = 1.0f;
	g_Manager->_musicmanager._targetPitch = 1.0f;
}

MOD_EXPORT void L_MusicManager_Resume() {
	g_Manager->_musicmanager.Resume();
}

MOD_EXPORT void L_MusicManager_SetCurrentPitch(float pitch) {
	g_Manager->_musicmanager._pitch = pitch;
}

MOD_EXPORT void L_MusicManager_StopJingle() {
	// magic offset is music->_jingleStream.playing
	if (g_Manager->_musicmanager._jingleCountdownMaybe > 0 || *(bool*)((char*)&g_Manager->_musicmanager + 0x354) == true) {
		g_Manager->_musicmanager.StopJingle();
	}
}

MOD_EXPORT void L_MusicManager_UpdateVolume() {
	g_Manager->_musicmanager.UpdateVolume();
}

MOD_EXPORT bool L_MusicManager_ValidateMusicID(int id, int* max) {
	*max = g_Manager->_musicmanager._entries.size();
	return id >= 0 && id < *max;
}

MOD_EXPORT void L_MusicManager_VolumeSlide(float targetVolume, float fadeRate) {
	g_Manager->_musicmanager.VolumeSlide(targetVolume, fadeRate);
}