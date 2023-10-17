#include <limits.h>
#include <iostream>

inline void GameRestart() {
	g_Manager->GetOptions()->Save();
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	LPSTR commandLine = GetCommandLine();
	bool success = CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (success) {
		TerminateProcess(GetCurrentProcess(), 0);
	}
}