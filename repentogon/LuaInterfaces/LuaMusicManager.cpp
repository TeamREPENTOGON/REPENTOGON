#include "IsaacRepentance.h"
#include "LuaCore.h"

// not as if this is going to change midgame
int maxId = -1;

extern "C" {
	unsigned int L_MusicManager_GetMaxID() {
		if (maxId != -1)
			return maxId;

		Music* music = &g_Manager->_musicManager;
		void* ecx = *(void**)((char*)music + 0x320);
		void* edi = *(void**)((char*)music + 0x31C);

		ptrdiff_t length = (ptrdiff_t)ecx - (ptrdiff_t)edi;
		maxId = length / 96; // Size of the content of the array

		return maxId;
	}

	void L_MusicManager_Crossfade(unsigned int id, float faderate) {
		g_Manager->_musicManager.Crossfade(id, faderate);
	}

	void L_MusicManager_Disable() {
		g_Manager->_musicManager._enabled = false;
	}

	void L_MusicManager_DisableLayer(unsigned int id) {
		g_Manager->_musicManager.DisableLayer(id);
	}

	void L_MusicManager_Enable() {
		g_Manager->_musicManager._enabled = true;
	}

	void L_MusicManager_EnableLayer(unsigned int id, bool instant) {
		g_Manager->_musicManager.EnableLayer(id, instant);
	}

	void L_MusicManager_Fadein(unsigned int id, float volume, float faderate) {
		g_Manager->_musicManager.Fadein(id, volume, faderate);
	}

	void L_MusicManager_Fadeout(float faderate) {
		g_Manager->_musicManager.Fadeout(faderate);
	}

	unsigned int L_MusicManager_GetCurrentMusicID() {
		return g_Manager->_musicManager._currentId;
	}

	unsigned int L_MusicManager_GetQueuedMusicID() {
		return g_Manager->_musicManager._queuedId == 0 ? g_Manager->_musicManager._currentId : g_Manager->_musicManager._queuedId;
	}

	bool L_MusicManager_IsEnabled() {
		return g_Manager->_musicManager._enabled;
	}

	bool L_MusicManager_IsLayerEnabled(unsigned int id) {
		return g_Manager->_musicManager.IsLayerEnabled(id);
	}

	void L_MusicManager_Pause() {
		g_Manager->_musicManager.Pause();
	}

	void L_MusicManager_PitchSlide(float TargetPitch) {
		g_Manager->_musicManager._targetPitch = TargetPitch;
	}

	void L_MusicManager_Play(unsigned int id, float volume) {
		g_Manager->_musicManager.Play(id, volume);
	}

	void L_MusicManager_PlayJingle(unsigned int id) {
		g_Manager->_musicManager.PlayJingle(id, 140, false);
	}

	void L_MusicManager_Queue(unsigned int id) {
		g_Manager->_musicManager._queuedId = id;
	}

	void L_MusicManager_ResetPitch() {
		g_Manager->_musicManager.ResetPitch();
	}

	void L_MusicManager_Resume() {
		g_Manager->_musicManager.Resume();
	}

	void L_MusicManager_StopJingle() {
		g_Manager->_musicManager.StopJingle();
	}

	void L_MusicManager_UpdateVolume() {
		g_Manager->_musicManager.UpdateVolume();
	}

	void L_MusicManager_VolumeSlide(float volume, float faderate) {
		g_Manager->_musicManager.VolumeSlide(volume, faderate);
	}
}
