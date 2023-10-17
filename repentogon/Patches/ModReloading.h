#include <limits.h>
#include <iostream>

inline void GameRestart() {
	g_Manager->GetOptions()->Save();
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	LPSTR commandLine = GetCommandLine();
	if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		TerminateProcess(GetCurrentProcess(), 0);
	}
}