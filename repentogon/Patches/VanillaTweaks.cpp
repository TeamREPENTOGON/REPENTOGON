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

// Force achievements to be unlockable
HOOK_METHOD(Manager, AchievementUnlocksDisallowed, (bool unk) -> bool) {
	return false;
}

// I'm putting this here bc I don't want to burden REPENTOGONOptions.h with the whole hooking system
HOOK_METHOD(OptionsConfig, Save, () -> void) {
	repentogonOptions.Save();
	super();
}