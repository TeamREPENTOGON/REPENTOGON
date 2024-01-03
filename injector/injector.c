#include <Windows.h>

#include <stdio.h>
#include <string.h>

void Log(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	FILE* f = fopen("injector.log", "a");
	if (!f) {
		f = stderr;
	}

	vfprintf(f, fmt, va);
	va_end(va);
}

int main() {
	STARTUPINFOA startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(processInfo));

	DWORD result = CreateProcess("isaac-ng.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo);
	if (result == 0) {
		Log("Failed to create process: %d\n", GetLastError());
		return -1;
	}

	HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE, processInfo.dwProcessId);
	if (!process) {
		Log("Failed to open process: %d\n", GetLastError());
		return -1;
	}

	void* remotePage = VirtualAllocEx(process, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!remotePage) {
		Log("Failed to allocate memory in isaac-ng.exe to load the dsound DLL: %d\n", GetLastError());
		return -1;
	}

	size_t bytesWritten = 0;
	char zeroBuffer[4096];
	memset(zeroBuffer, 0, sizeof(zeroBuffer));
	WriteProcessMemory(process, remotePage, zeroBuffer, 4096, &bytesWritten);

	HMODULE kernel32 = GetModuleHandle("kernel32.dll");
	if (!kernel32) {
		Log("Unable to find kernel32.dll, WTF\n");
		return -1;
	}

	FARPROC getProcAddress = GetProcAddress(kernel32, "GetProcAddress");
	FARPROC loadLibraryA = GetProcAddress(kernel32, "LoadLibraryA");

	if (!getProcAddress) {
		Log("Unable to find GetProcAddress\n");
		return -1;
	}

	if (!loadLibraryA) {
		Log("Unable to find LoadLibraryA\n");
		return -1;
	}

	const char* dllName = "launcher.dll";
	// 0x0
	WriteProcessMemory(process, remotePage, &getProcAddress, sizeof(getProcAddress), &bytesWritten);
	// 0x4
	WriteProcessMemory(process, (char*)remotePage + 4, &loadLibraryA, sizeof(loadLibraryA), &bytesWritten);
	// 0x8
	WriteProcessMemory(process, (char*)remotePage + 8, dllName, strlen(dllName), &bytesWritten);
	/* 0x16 (0x15 is a '\0')
	 * Call LoadLibraryA in the remote thread.
	 * The thread will push the name of the DLL from its stack.
	 * It will then call LoadLibraryA.
	 * 
	 * This function is a THREAD_START_ROUTINE, with the following signature :
	 * DWORD WINAPI(LPVOID);
	 * 
	 * WINAPI is __stdcall: arguments are pushed in reverse order on the stack and callee cleans the stack.
	 */
	char hook[128] = {
		"\x55" // push ebp
		"\x89\xe5" // mov ebp, esp
		"\x53" // push ebx
		"\x56" // push esi
		"\x57" // push edi
		"\x3e\x8b\x5d\x08" // mov ebx, dword ptr ds:[ebp+8], put parameter in ebx
		"\x8b\x73\x04" // mov esi, dword ptr ds:[ebx+4], put load library a in esi
		"\x8d\x7b\x08" // lea edi, [ebx+8], put dllname in edi
		"\x57" // push edi, push dllname on stack
		"\xff\xd6" // call LoadLibraryA
		"\x5f" // pop edi
		"\x5e" // pop esi
		"\x5b" // pop ebx
		"\x89\xec" // mov esp, ebp
		"\xb8\x01\x00\x00\x00" // mov eax, 1
		"\x5d" // pop ebp
		"\xc3" // ret
	};
	WriteProcessMemory(process, (char*)remotePage + 0x15, hook, 128, &bytesWritten);

	HANDLE remoteThread = CreateRemoteThread(process, NULL, 0, (char*)remotePage + 0x15, remotePage, 0, NULL);
	if (!remoteThread) {
		Log("Error while creating remote thread: %d\n", GetLastError());
		return -1;
	}

	result = ResumeThread(processInfo.hThread);
	if (result == -1) {
		Log("Failed to resume isaac-ng.exe main thread: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("ResumeThread: %d\n", result);
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return 0;
}