#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ModReloading.h"
#include "../REPENTOGONOptions.h"

// We always want to restart after the game fully loads, this ensures that
HOOK_METHOD(ModManager, UpdateWorkshopMods, () -> void) {
	super();
	repentogonOptions.Write("internal", "didmodreset", "0");
	// UpdateWorkshopMods gets called unconditionally by IsaacStartup, so we can guarantee that the bool is properly reset by doing it here
}

HOOK_METHOD(ModManager, Reset, () -> void) {
	bool res = repentogonOptions.GetBool("internal", "didmodreset");
	if (!res) {
		repentogonOptions.Write("internal", "didmodreset", "1");
		GameRestart(); //if we ever walk back from this, for whateevr reason, we will need to recheck some stuff regarding a few xml reloads on xmldata
	}
	else {
		super();
		// Reset gets conditionally called in UpdateWorkshopMods, so setting didmodreset here will have inconsistent results
	}
}
