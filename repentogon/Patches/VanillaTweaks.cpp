#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "REPENTOGONOptions.h"

HOOK_METHOD(Entity_Slot, TakeDamage, (float Damage, unsigned long long DamageFlags, EntityRef* Source, int DamageCountdown) -> bool) {
	bool result = super(Damage, DamageFlags, Source, DamageCountdown);


	if (result && repentogonOptions.keyMasterDealChance) {
		if (g_Game->GetDailyChallenge()._id == 0 && _variant == 7) //KEY_MASTER
			g_Game->_stateFlags |= 1;
	}
	return result;
}