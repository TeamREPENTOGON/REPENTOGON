#include "IsaacRepentance.h"
#include "LuaCore.h"

// I suppose calling these files "Lua interfaces" isn't necessarily accurate anymore, is it?

extern "C" {

    void L_SFXManager_AdjustPitch(unsigned int id, float pitch) {
        g_Manager->_sfxManager.AdjustPitch(id, pitch);
    }

    void L_SFXManager_AdjustVolume(unsigned int id, float volume) {
        g_Manager->_sfxManager.AdjustVolume(id, volume);
    }

    float L_SFXManager_GetAmbientSoundVolume(unsigned int id) {
        return g_Manager->_sfxManager.GetAmbientSoundVolume(id);
    }

    bool L_SFXManager_IsPlaying(unsigned int id) {
        return g_Manager->_sfxManager.IsPlaying(id);
    }
    
    void L_SFXManager_Play(unsigned int id, float volume, int frameDelay, bool loop, float pitch, float pan) {
        g_Manager->_sfxManager.Play(id, volume, frameDelay, loop, pitch, pan);
    }
    
    void L_SFXManager_Preload(unsigned int id) {
        g_Manager->_sfxManager.Preload(id);
    }

    void L_SFXManager_SetAmbientSound(unsigned int id, float volume, float pitch) {
        g_Manager->_sfxManager.SetAmbientSound(id, volume, pitch);
    }

    void L_SFXManager_Stop(unsigned int id) {
        g_Manager->_sfxManager.Stop(id);
    }
    
    void L_SFXManager_StopLoopingSounds() {
        g_Manager->_sfxManager.StopLoopingSounds();
    }
}