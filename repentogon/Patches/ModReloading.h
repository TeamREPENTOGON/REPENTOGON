#include <limits.h>
#include <iostream>

#include "Log.h"
#include "../REPENTOGONOptions.h"
#include "../LauncherInterface.h"

inline void GameRestart() {
	repentogonOptions.Write("internal", "DidModReset", "1");
	g_Manager->GetOptions()->Save();
	ExitProcess(LauncherInterface::LAUNCHER_EXIT_MODS_CHANGED);
}