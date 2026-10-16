#include "IsaacRepentance.h"

MOD_EXPORT void L_SFXManager_AdjustPitch(int id, float pitch) {
	g_Manager->_sfxManager.AdjustPitch(id, pitch);
}

MOD_EXPORT void L_SFXManager_AdjustVolume(int id, float volume) {
	g_Manager->_sfxManager.AdjustVolume(id, volume);
}

MOD_EXPORT float L_SFXManager_GetAmbientSoundVolume(int id) {
	return g_Manager->_sfxManager.GetAmbientSoundVolume(id);
}
	
MOD_EXPORT bool L_SFXManager_IsPlaying(int id) {
	return g_Manager->_sfxManager.IsPlaying(id);
}

MOD_EXPORT void L_SFXManager_Play(int id, float volume, int frameDelay, bool loop, float pitch, float pan) {
	return g_Manager->_sfxManager.Play(id, volume, frameDelay, loop, pitch, pan);
}

MOD_EXPORT void L_SFXManager_Preload(int id) {
	return g_Manager->_sfxManager.Preload(id);
}

MOD_EXPORT void L_SFXManager_SetAmbientSound(int id, float volume, float pitch) {
	return g_Manager->_sfxManager.SetAmbientSound(id, volume, pitch);
}

MOD_EXPORT void L_SFXManager_Stop(int id) {
	return g_Manager->_sfxManager.Stop(id);
}

MOD_EXPORT void L_SFXManager_StopLoopingSounds() {
	return g_Manager->_sfxManager.StopLoopingSounds();
}