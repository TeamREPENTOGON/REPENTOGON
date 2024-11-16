#include <limits.h>
#include <iostream>

#include "Log.h"
#include "../REPENTOGONOptions.h"

inline void GameRestart() {
	repentogonOptions.Write("internal", "DidModReset", "1");
	g_Manager->GetOptions()->Save();
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	LPSTR commandLine = GetCommandLine();
	bool success = CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!success) {
		ZHL::Log("[ERROR] CreateProcess failed (GetLastError() = %d, cli = %s)\n", GetLastError(), commandLine);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (success) {
		TerminateProcess(GetCurrentProcess(), 0);
	}
}