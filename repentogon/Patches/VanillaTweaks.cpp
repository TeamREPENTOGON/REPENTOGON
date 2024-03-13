#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"

// Key Master affects Devil Deal chance
HOOK_METHOD(Entity_Slot, TakeDamage, (float Damage, unsigned long long DamageFlags, EntityRef* Source, int DamageCountdown) -> bool) {
	bool result = super(Damage, DamageFlags, Source, DamageCountdown);


	if (result && repentogonOptions.keyMasterDealChance) {
		if (g_Game->GetDailyChallenge()._id == 0 && _variant == 7) //KEY_MASTER
			g_Game->_stateFlags |= 1;
	}
	return result;
}

// Allow Void to have its own rooms. 
// By default, the void path is "rooms/01.Basement.xml" which is not ideal!
// Redirect to "rooms/26.The Void_ex.xml" since the game already has a "rooms/26.The Void.xml" that hasn't been tested.
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int Stage, unsigned int Mode) -> void) {
	if (Stage == 26 && g_Game->GetRoomConfigHolder()->configs[26].xmlFileName == "rooms/01.Basement.xml")
		g_Game->GetRoomConfigHolder()->configs[26].xmlFileName = "rooms/26.The Void_ex.xml";
	super(Stage, Mode);
}

// Force achievements to be unlockable (expect outside of game mode)
HOOK_METHOD(Manager, AchievementUnlocksDisallowed, (bool unk) -> bool) {
	ModManager* modman = g_Manager->GetModManager();
	auto loadedMod = std::find_if(modman->_mods.begin(), modman->_mods.end(), [](ModEntry* mod) { return mod->_loaded; });

	if (loadedMod != modman->_mods.end() || g_Manager->GetOptions()->_enableDebugConsole) {
		if ((unk) || ((g_Manager->GetState() != 2 || g_Game == nullptr) || (g_Game->GetDailyChallenge()._id == 0 && !g_Game->IsDebug() ))) {
			return true;
		}
	}

	return false;
}

// I'm putting this here bc I don't want to burden REPENTOGONOptions.h with the whole hooking system
HOOK_METHOD(OptionsConfig, Save, () -> void) {
	repentogonOptions.Save();
	super();
}

HOOK_METHOD(Game, Update, () -> void) {
	super();

	if (GetDailyChallenge()._id != 0 && !GetDailyChallenge()._isPractice) {
		if (g_Manager->GetOptions()->_enableDebugConsole || g_Manager->GetOptions()->ModsEnabled()) {
			if (_leaderboard._displayState < 1 && _frameCount > 0) {
				End(1); //You shall not pass! (@ wise greybeard dude)
			}
			
		}
	}
}

HOOK_METHOD(Console, SubmitInput, (bool unk) -> void) {
	super(unk);
	Game* game = g_Game;
	if (game->GetDailyChallenge()._id != 0 && !game->GetDailyChallenge()._isPractice) {
		if (game->_leaderboard._displayState < 1 && game->_frameCount > 0)
			game->End(1);
	}
}

// Instruct the stat HUD to recalculate planetarium chance after every new level. Avoids running planetarium chance calculation and associated callbacks every frame
HOOK_METHOD(Level, Init, () -> void) {
	super();

	int playerId = g_Game->GetHUD()->_statHUD.GetPlayerId(g_Game->_playerManager._playerList[0]);
	g_Game->GetHUD()->_statHUD.RecomputeStats(playerId, 0x100); // TODO: enum
};