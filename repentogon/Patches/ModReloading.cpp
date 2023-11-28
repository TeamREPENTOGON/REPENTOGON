#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ModReloading.h"

// We always want to restart after the game fully loads, this ensures that
HOOK_METHOD(ModManager, UpdateWorkshopMods, () -> void) {
	bool res = repentogonOptions.GetBool("internal", "DidModReset");
	if (!res && !repentogonOptions.preventModUpdates) {
		super();
	}
	repentogonOptions.Write("internal", "DidModReset", "0");
	// UpdateWorkshopMods gets called unconditionally by IsaacStartup, so we can guarantee that the bool is properly reset by doing it here
}

HOOK_METHOD(ModManager, Reset, () -> void) {
		GameRestart(); //if we ever walk back from this, for whateevr reason, we will need to recheck some stuff regarding a few xml reloads on xmldata
}
