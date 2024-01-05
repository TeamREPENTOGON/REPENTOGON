#include <Windows.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

void Log(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	FILE* f = fopen("injector.log", "a");
	if (!f) {
		f = stderr;
	}

	char buffer[4096];
	time_t now = time(NULL);
	struct tm* tm = localtime(&now);
	strftime(buffer, 4095, "%Y-%m-%d %H:%M:%S", tm);
	fprintf(f, "[%s] ", buffer);
	vfprintf(f, fmt, va);
	va_end(va);
}

int main() {
	{
		FILE* f = fopen("injector.log", "w");
		if (f) {
			fclose(f);
		}
	}

	Log("Starting injector\n");

	STARTUPINFOA startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));

	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(processInfo));

	DWORD result = CreateProcess("isaac-ng.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo);
	if (result == 0) {
		Log("Failed to create process: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Started isaac-ng.exe in suspended state, processID = %d\n", processInfo.dwProcessId);
	}

	HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE, processInfo.dwProcessId);
	if (!process) {
		Log("Failed to open process: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Acquired handle to isaac-ng.exe, process ID = %d\n", processInfo.dwProcessId);
	}

	void* remotePage = VirtualAllocEx(process, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!remotePage) {
		Log("Failed to allocate memory in isaac-ng.exe to load the dsound DLL: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Allocated memory for remote thread at %p\n", remotePage);
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
	else {
		Log("Acquired kernel32.dll at %p\n", kernel32);
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

	Log("Acquired GetProcAddress at %p, LoadLibraryA at %p\n", getProcAddress, loadLibraryA);

	const char* dllName = "launcher.dll";
	const char* functionName = "LaunchZHL";
	size_t offset = 0;
	// 0x0
	WriteProcessMemory(process, remotePage, &getProcAddress, sizeof(getProcAddress), &bytesWritten);
	offset += bytesWritten;
	// 0x4
	WriteProcessMemory(process, (char*)remotePage + offset, &loadLibraryA, sizeof(loadLibraryA), &bytesWritten);
	offset += bytesWritten;
	// 0x8
	WriteProcessMemory(process, (char*)remotePage + offset, dllName, strlen(dllName), &bytesWritten);
	offset += (bytesWritten + 1);
	// 0x16 (0x15 is a '\0')
	WriteProcessMemory(process, (char*)remotePage + offset, functionName, strlen(functionName), &bytesWritten);
	offset += (bytesWritten + 1);
	size_t functionOffset = offset;
	/* 0x21 (0x20 is a '\0')
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
		"\x8b\x73\x04" // mov esi, dword ptr ds:[ebx+4], put LoadLibraryA in esi
		"\x8d\x7b\x08" // lea edi, [ebx+8], put dllname in edi
		"\x57" // push edi, push dllname on stack
		"\xff\xd6" // call esi, call LoadLibraryA("launcher.dll")
		"\x8d\x7b\x15" // lea edi, [ebx + 0x15], put function name in edi
		"\x57" // push edi, put function name on stack
		"\x50" // push eax, put library on stack
		"\x8b\x33" // mov esi, dword ptr ds:[ebx], put GetProcAddress in esi
		"\xff\xd6" // call esi, call GetProcAddress(lib, "LaunchZHL")
		"\xff\xd0" // call eax, call LaunchZHL()
		"\x5f" // pop edi
		"\x5e" // pop esi
		"\x5b" // pop ebx
		"\x89\xec" // mov esp, ebp
		"\xb8\x01\x00\x00\x00" // mov eax, 1
		"\x5d" // pop ebp
		"\xc2\x04\x00" // ret 4
	};
	WriteProcessMemory(process, (char*)remotePage + offset, hook, 128, &bytesWritten);

	HANDLE remoteThread = CreateRemoteThread(process, NULL, 0, (char*)remotePage + functionOffset, remotePage, 0, NULL);
	if (!remoteThread) {
		Log("Error while creating remote thread: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Created remote thread in isaac-ng.exe\n");
	}

	Log("Waiting for remote thread to complete\n");
	result = WaitForSingleObject(remoteThread, 60 * 1000);
	switch (result) {
	case WAIT_OBJECT_0:
		Log("RemoteThread completed\n");
		break;

	case WAIT_ABANDONED:
		Log("This shouldn't happened: RemoteThread returned WAIT_ABANDONNED\n");
		break;

	case WAIT_TIMEOUT:
		Log("RemoteThread timed out\n");
		break;

	case WAIT_FAILED:
		Log("WaitForSingleObject on RemoteThread failed: %d\n", GetLastError());
		break;
	}

	result = ResumeThread(processInfo.hThread);
	if (result == -1) {
		Log("Failed to resume isaac-ng.exe main thread: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Resumed main thread of isaac-ng.exe, previous supend count was %d\n", result);
	}

	Log("Waiting for isaac-ng.exe main thread to return\n");
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	Log("isaac-ng.exe completed, shutting down injector\n");

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return 0;
}