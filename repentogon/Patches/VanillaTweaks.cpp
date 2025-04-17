#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"
#include "Log.h"
#include "imgui.h"


// Allow Void to have its own rooms. 
// By default, the void path is "rooms/01.Basement.xml" which is not ideal!
// Redirect to "rooms/26.The Void_ex.xml" since the game already has a "rooms/26.The Void.xml" that hasn't been tested.
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int Stage, unsigned int Mode) -> void) {
	if (Stage == 26 && g_Game->GetRoomConfig()->_stages[26]._rooms[Mode]._filepath == "rooms/01.Basement.xml")
		g_Game->GetRoomConfig()->_stages[26]._rooms[Mode]._filepath = "rooms/26.The Void_ex.xml";
	super(Stage, Mode);
}

// Force achievements to be unlockable (expect outside of game mode) [moved to ASMTweaks.cpp]
/*HOOK_METHOD(Manager, AchievementUnlocksDisallowed, (bool unk) -> bool) {
	ModManager* modman = g_Manager->GetModManager();
	auto loadedMod = std::find_if(modman->_mods.begin(), modman->_mods.end(), [](ModEntry* mod) { return mod->_loaded; });

	if (loadedMod != modman->_mods.end() || g_Manager->GetOptions()->_enableDebugConsole) {
		if (((g_Manager->GetState() != 2 || g_Game == nullptr) || (g_Game->GetDailyChallenge()._id == 0 && !g_Game->IsDebug() ))) {
			return true;
		}
	}

	return false;
}
*/

// I'm putting this here bc I don't want to burden REPENTOGONOptions.h with the whole hooking system
HOOK_METHOD(OptionsConfig, Save, () -> void) {
	repentogonOptions.Save();
	super();
}

/*HOOK_METHOD(Game, Update, () -> void) {
	super();

	if (GetDailyChallenge()._id != 0 && !GetDailyChallenge()._isPractice) {
		if (g_Manager->GetOptions()->_enableDebugConsole || g_Manager->GetOptions()->ModsEnabled()) {
			if (_leaderboard._displayState < 1 && _frameCount > 0) {
				End(1);
			}
			
		}
	}
}
*/

HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player) -> void) {
	Game* game = g_Game;
	if (game->GetDailyChallenge()._id != 0 && !game->GetDailyChallenge()._isPractice) {
		return;
	}
	super(in, out, player);
}

// Instruct the stat HUD to recalculate planetarium chance after every new level. Avoids running planetarium chance calculation and associated callbacks every frame
HOOK_METHOD(Level, Init, (bool unk) -> void) {
	super(unk);
	int playerId = g_Game->GetHUD()->_statHUD.GetPlayerId(g_Game->_playerManager._playerList[0]);
	g_Game->GetHUD()->_statHUD.RecomputeStats(playerId, 0x100, false); // TODO: enum
};
HOOK_METHOD(Game, RestoreState, (GameState* gstate, bool loaded) -> void) { //so it also runs on save/continue
	super(gstate, loaded);
	int playerId = g_Game->GetHUD()->_statHUD.GetPlayerId(g_Game->_playerManager._playerList[0]);
	g_Game->GetHUD()->_statHUD.RecomputeStats(playerId, 0x100, false); // TODO: enum
};
HOOK_STATIC(KAGE_Filesys_FileManager, LoadArchiveFile, (char* path, int unk1, unsigned short unk2)->void, __stdcall) {
	ZHL::Logger logger;
	static bool unpacked_flag_check_done=false;
	static bool unpacked_flag_set = false;

	if (!unpacked_flag_check_done) {
		for (int i = 1; i < __argc; i++) {
			char* arg = __argv[i];
			if (strcmp("-unpacked", arg) == 0) {
				logViewer.AddLog("[REPENTOGON]", "Ignoring archives because of the -unpacked flag! Be careful!\n");
				printf("[REPENTOGON] Ignoring archives because of the -unpacked flag! Be careful!\n");
				unpacked_flag_set = true;
			};
		};
		unpacked_flag_check_done = true;
	};

	if (unpacked_flag_set) {
		return;
	}
	else {
		return super(path, unk1, unk2);
	};
};

//Hornfel rare crash with minecart

HOOK_METHOD(Entity_NPC, Hornfel_UpdateAI, () -> void) {
	switch (_state) {
	case 6:
	case 7:
		if (!GetMinecart()) {
			printf("Warning! Hornfel is about to blow this game! (I hate this red thing, I spit on you)\n");
			//restoring summon state while Hornfel is out of the room
			_state = 13, _entityGridCollisionClass = 5, _entityCollisionClass = 4, _visible = true;
			break;
		}
	}
	super();
}

HOOK_METHOD(Entity_NPC, Hornfel_UpdateFrame, () -> void) {
	if (_state == 4 || _state == 13) {
		//printf("Trying to switch states, mm?\n");
		return;
	}
	super();
}

//resetting wispCollectibleType for Mystery Gift/Eden's Soul wisps bug
HOOK_METHOD(Entity_Familiar, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {
	_wispCollectibleType = 0;

	super(type, variant, subtype, initSeed);
}

// eco mode stuff 
decltype(&SetProcessInformation) p_SetProcessInformation=0x0;
void EcoMode_toggle_qos(bool eco_state) {
	if (p_SetProcessInformation == 0x0) {
		p_SetProcessInformation = (decltype(&SetProcessInformation))GetProcAddress(GetModuleHandle("kernel32"), "SetProcessInformation");
		if (!p_SetProcessInformation) {
			repentogonOptions.ecoMode = false;
			repentogonOptions.Save();
			return;
		}
	};
	HANDLE cur_process = GetCurrentProcess();
	PROCESS_POWER_THROTTLING_STATE PowerThrottling = { 0 };
	PowerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
	PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
	PowerThrottling.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED*(eco_state);	// 0 is normal, the macro is eco
	if (eco_state) {
		SetPriorityClass(cur_process, IDLE_PRIORITY_CLASS);
	}
	else {
		SetPriorityClass(cur_process, NORMAL_PRIORITY_CLASS);
	};
//	SetProcessInformation(cur_process, ProcessPowerThrottling, &PowerThrottling, sizeof(PowerThrottling));
	p_SetProcessInformation(cur_process, ProcessPowerThrottling, &PowerThrottling, sizeof(PowerThrottling));
};

bool EcoMode_old_state = 0;
HOOK_METHOD(Manager, Render, (void)->void) {
	if (repentogonOptions.ecoMode) {
		HWND hwnd = (HWND)__ptr_g_KAGE_Graphics_Manager->_unk_HWND->HWND;
		bool EcoMode_new_state = IsIconic(hwnd);
//		EcoMode_new_state = GetForegroundWindow() != hwnd;
		if ((EcoMode_new_state ^ EcoMode_old_state) == 1) {
			EcoMode_toggle_qos(EcoMode_new_state);
		};
		EcoMode_old_state = EcoMode_new_state;
		bool game_inactive = (g_Manager->_state!=2 || g_Game->IsPauseMenuOpen());
		if (EcoMode_new_state == 1) {
			Sleep(5+(333-5)*game_inactive);
			return;	//skip over super
		};
	};
	super();
};

// eco mode stuff end

//clearing kerning pairs from parsed font
/*HOOK_METHOD(Font, Load, (const char* path, bool unusedIsLoading) -> void) {
	super(path, unusedIsLoading);

	auto& kernPair = _kerningPairs;

	if (!kernPair.empty()) {
		kernPair.clear();
	}

}
*/