#include "LuaCore.h"
#include "IsaacRepentance.h"

#include "../REPENTOGONOptions.h"

extern "C" {

	// VANILLA

	int L_Options_GetAnnouncerVoiceMode() {
		return g_Manager->GetOptions()->_announcerVoiceMode;
	}

	void L_Options_SetAnnouncerVoiceMode(int mode) {
		g_Manager->GetOptions()->_announcerVoiceMode = mode;
	}
	
	bool L_Options_GetBulletVisibility() {
		return g_Manager->GetOptions()->_enableBulletVisibility;
	} 

	void L_Options_SetBulletVisibility(bool visibility) {
		g_Manager->GetOptions()->_enableBulletVisibility = visibility;
	}

	int L_Options_GetCameraStyle() {
		return g_Manager->GetOptions()->_cameraStyle;
	}

	void L_Options_SetCameraStyle(int style) {
		g_Manager->GetOptions()->_cameraStyle = style;
	}

	bool L_Options_GetChargeBars() {
		return g_Manager->GetOptions()->_enableChargeBars;
	}

	void L_Options_SetChargeBars(bool bars) {
		g_Manager->GetOptions()->_enableChargeBars = bars;
	}

	int L_Options_GetConsoleFont() {
		return g_Manager->GetOptions()->_consoleFont;
	}

	void L_Options_SetConsoleFont(int font) {
		g_Manager->GetOptions()->_consoleFont = font;
	}

	bool L_Options_GetDebugConsoleEnabled() {
		return g_Manager->GetOptions()->_enableDebugConsole;
	}

	void L_Options_SetDebugConsoleEnabled(bool console) {
		g_Manager->GetOptions()->_enableDebugConsole = console;
	}

	bool L_Options_GetDisplayPopups() {
		return g_Manager->GetOptions()->_popupsEnabled;
	}

	void L_Options_SetDisplayPopups(bool popups) {
		g_Manager->GetOptions()->_popupsEnabled = popups;
	}

	int L_Options_GetExtraHUDStyle() {
		return g_Manager->GetOptions()->_historyHudMode;
	}

	void L_Options_SetExtraHUDStyle(int style) {
		g_Manager->GetOptions()->_historyHudMode = style;
	}

	bool L_Options_GetFadedConsoleDisplay() {
		return g_Manager->GetOptions()->_enableFadedConsoleDisplay;
	}

	void L_Options_SetFadedConsoleDisplay(bool display) {
		g_Manager->GetOptions()->_enableFadedConsoleDisplay = display;
	}

	bool L_Options_GetFilter() {
		return g_Manager->GetOptions()->_enableFilter;
	}

	void L_Options_SetFilter(bool filter) {
		g_Manager->GetOptions()->_enableFilter = filter;
	}

	bool L_Options_GetFoundHUD() {
		return g_Manager->GetOptions()->_enableFoundHUD;
	}

	void L_Options_SetFoundHUD(bool hud) {
		g_Manager->GetOptions()->_enableFoundHUD = hud;
	}

	bool L_Options_GetFullscreen() {
		return g_Manager->GetOptions()->_isFullscreen;
	}
	
	void L_Options_SetFullscreen(bool fullscreen) {
		g_Manager->GetOptions()->SetFullScreen(fullscreen);
	}
	
	float L_Options_GetGamma() {
		return g_Manager->GetOptions()->_gamma;
	}

	void L_Options_SetGamma(float gamma) {
		g_Manager->GetOptions()->_gamma = gamma;
	}

	float L_Options_GetHUDOffset() {
		return g_Manager->GetOptions()->_hudOffset;
	}
	
	void L_Options_SetHUDOffset(float offset) {
		g_Manager->GetOptions()->_hudOffset = offset;
	}

	const char* L_Options_GetLanguage() {
		return Manager::GetLanguage();
	}

	float L_Options_GetMapOpacity() {
		return g_Manager->GetOptions()->_mapOpacity;
	}
	
	void L_Options_SetMapOpacity(float opacity) {
		g_Manager->GetOptions()->_mapOpacity = opacity;
	}

	int L_Options_GetMaxRenderScale() {
		return g_Manager->GetOptions()->_maxRenderScale;
	}

	void L_Options_SetMaxRenderScale(int scale) {
		g_Manager->GetOptions()->_maxRenderScale = scale;
	}

	int L_Options_GetMaxScale() {
		return g_Manager->GetOptions()->_maxScale;
	}

	void L_Options_SetMaxScale(int scale) {
		g_Manager->GetOptions()->_maxScale = scale;
	}

	bool L_Options_GetMouseControl() {
		return g_Manager->GetOptions()->_enableMouseControl;
	}
	
	void L_Options_SetMouseControl(bool mouse) {
		LuaEngine_Options::SetMouseControl(mouse);
	}

	float L_Options_GetMusicVolume() {
		return g_Manager->GetOptions()->_musicVolume;
	}

	void L_Options_SetMusicVolume(float volume) {
		g_Manager->GetOptions()->SetMusicVolume(volume);
	}

	bool L_Options_GetPauseOnFocusLost() {
		return g_Manager->GetOptions()->_enablePauseOnFocusLost;
	}

	void L_Options_SetPauseOnFocusLost(bool pause) {
		g_Manager->GetOptions()->_enablePauseOnFocusLost = pause;
	}
	
	bool L_Options_GetRumbleEnabled() {
		return g_Manager->GetOptions()->_enableRumble;
	}

	void L_Options_SetRumbleEnabled(bool rumble) {
		g_Manager->GetOptions()->_enableRumble = rumble;
	}

	bool L_Options_GetSaveCommandHistory() {
		return g_Manager->GetOptions()->_enableSaveCommandHistory;
	}

	void L_Options_SetSaveCommandHistory(bool saveHistory) {
		g_Manager->GetOptions()->_enableSaveCommandHistory = saveHistory;
	}

	float L_Options_GetSFXVolume() {
		return g_Manager->GetOptions()->_sfxVolume;
	}

	void L_Options_SetSFXVolume(float volume) {
		g_Manager->GetOptions()->_sfxVolume = volume;
		g_Manager->_sfxManager.ClearVolumeModifier();
	}

	bool L_Options_GetUseBorderlessFullscreen() {
		return g_Manager->GetOptions()->_enableBorderlessFullscreen;
	}

	void L_Options_SetUseBorderlessFullscreen(bool borderless) {
		g_Manager->GetOptions()->_enableBorderlessFullscreen = borderless;
	}

	bool L_Options_GetVSync() {
		return g_Manager->GetOptions()->_enableVSync;
	}

	void L_Options_SetVSync(bool vsync) {
		g_Manager->GetOptions()->SetVSync(vsync);
	}

	// REPENTOGON

	bool L_Options_GetBetterVoidGeneration() {
		return repentogonOptions.betterVoidGeneration;
	}

	void L_Options_SetBetterVoidGeneration(bool generation) {
		repentogonOptions.betterVoidGeneration = generation;
	}

	bool L_Options_GetHushPanicStateFix() {
		return repentogonOptions.hushPanicStateFix;
	}

	void L_Options_SetHushPanicStateFix(bool disableVinhMode) {
		repentogonOptions.hushPanicStateFix = disableVinhMode;
	}
	
	bool L_Options_GetKeyMasterDealChance() {
		return repentogonOptions.keyMasterDealChance;
	}

	void L_Options_SetKeyMasterDealChance(bool dealChance) {
		repentogonOptions.keyMasterDealChance = dealChance;
	}

	bool L_Options_GetPreventModUpdates() {
		return repentogonOptions.preventModUpdates;
	}

	void L_Options_SetPreventModUpdates(bool preventUpdates) {
		repentogonOptions.preventModUpdates = preventUpdates;
	}

	bool L_Options_GetQuickRoomClear() {
		return repentogonOptions.quickRoomClear;
	}

	void L_Options_SetQuickRoomClear(bool quickClear) {
		repentogonOptions.quickRoomClear = quickClear;
	}
}