#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"

// These hooks allow for reverting Repentance+ changes to 1.7.9b behavior.

// 1.9.7.11 makes The Siren unable to charm Isaac's Heart, and Lilith's Incubus.
HOOK_METHOD_PRIORITY(Entity_Familiar, CanCharm, 1000, () -> bool) {
	if (!repentogonOptions.vfSirenCharmHeartIncubus && (_variant == 62 || _variant == 80)) {
		return true;
	}
	
	return super();
}
