#include <algorithm>

#include "LuaCore.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"

#include "../REPENTOGONOptions.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

MOD_EXPORT OptionsConfig* L_Options_Get() {
	return g_Manager->GetOptions();
}

MOD_EXPORT const char* L_Options_GetLanguage() {
	return Manager::GetLanguage();
}

MOD_EXPORT void L_Options_SetFullscreen(bool value) {
	g_Manager->GetOptions()->SetFullScreen(value);
}

MOD_EXPORT void L_Options_SetVSync(bool value) {
	g_Manager->GetOptions()->SetVSync(value);
}

MOD_EXPORT void L_Options_SetMusicVolume(float value) {
	g_Manager->GetOptions()->SetMusicVolume(value);
}

MOD_EXPORT void L_Options_ClearSFXVolumeModifier() {
	g_Manager->_sfxManager.ClearVolumeModifier();
}

static bool* RepentogonOption(int index) {
	switch (index) {
	case 0: return &repentogonOptions.betterVoidGeneration;
	case 1: return &repentogonOptions.hushLaserSpeedFix;
	case 2: return &repentogonOptions.statHUDPlanetarium;
	case 3: return &repentogonOptions.quickRoomClear;
	case 4: return &repentogonOptions.preventModUpdates;
	}
	return nullptr;
}

MOD_EXPORT bool L_Options_GetRepentogonOption(int index) {
	bool* option = RepentogonOption(index);
	return option && *option;
}

MOD_EXPORT void L_Options_SetRepentogonOption(int index, bool value) {
	if (bool* option = RepentogonOption(index)) {
		*option = value;
	}
}
