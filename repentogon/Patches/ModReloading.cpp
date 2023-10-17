#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ModReloading.h"
#include "../REPENTOGONOptions.h"

bool didModUpdates = false;

// We always want to restart after the game fully loads, this ensures that
HOOK_METHOD(ModManager, UpdateWorkshopMods, () -> void) {
	super();
	didModUpdates = true;
}

HOOK_METHOD(ModManager, Reset, () -> void) {
	bool res = repentogonOptions.GetBool("internal", "didmodreset");
	if (!res || didModUpdates) {
		repentogonOptions.Write("internal", "didmodreset", "1");
		GameRestart(); //if we ever walk back from this, for whateevr reason, we will need to recheck some stuff regarding a few xml reloads on xmldata
	}
	else {
		super();
	}
	repentogonOptions.Write("internal", "didmodreset", "0");
}
