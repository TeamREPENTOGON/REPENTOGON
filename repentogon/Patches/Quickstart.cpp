#include "Quickstart.h"
#include "IsaacRepentance.h"
#include <chrono>
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"

bool quickStartChecked = false;
static inline bool quickstart_IsGameStateValid() {
	if (!g_Manager) {
		return false;
	};
	if (g_Manager->_gamestate._seeds._gameStartSeed != 0) {
		return true;
	};
	return false;
};
HOOK_METHOD(Menu_Save, select_save_slot, (uint32_t slot)->void) {
	super(slot);
	repentogonOptions.lastSaveFile = slot;
	repentogonOptions.Save();
};
HOOK_STATIC(Manager, Update, (bool unk)->void, _stdcall) {
	Manager* mngr = g_Manager;
	bool should_cancel_qstart=true;
	if (!quickStartChecked) {
		for (int i = 1; i < __argc; i++) {
			char* arg = __argv[i];
			if (strcmp("-quickstart", arg) == 0) {
				should_cancel_qstart = false;
			};
		};
		quickStartChecked = true;
		if (should_cancel_qstart || repentogonOptions.lastSaveFile==0) {
			return super(unk);
		};
		g_Manager->SetSaveSlot(repentogonOptions.lastSaveFile);
		g_Manager->_networkPlay = false;
		g_Manager->_debugStart = false;
		g_Manager->_rerunAvailable = quickstart_IsGameStateValid();
		if (!g_Manager->_rerunAvailable) {
			return super(unk);
		};
		g_Manager->_startingFromRerun = false;
		g_Manager->GetDailyChallenge()->_id = 0;
		g_Manager->_starting = true;
		g_Manager->_startCutscene = false;
	};
	super(unk);
};