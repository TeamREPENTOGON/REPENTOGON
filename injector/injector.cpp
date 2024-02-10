#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstring>
#include <ctime>

#include <chrono>

#include <wx/wx.h>
#include "injector/window.h"

/* Perform the early setup for the injection: create the Isaac process, 
 * allocate memory for the remote thread function etc. 
 * 
 * No extra thread is created in the process. ImGui should be initialized
 * afterwards to setup the injector, and then the remote thread can be
 * created.
 * 
 * Return true of the initialization was sucessful, false otherwise.
 */
static int FirstStageInit(struct IsaacOptions const* options, HANDLE* process, void** page, size_t* functionOffset, PROCESS_INFORMATION* processInfo);

static void Log(const char* fmt, ...);

static void GenerateCLI(const struct IsaacOptions* options, char cli[256]);

void GenerateCLI(const struct IsaacOptions* options, char cli[256]) {
	memset(cli, 0, sizeof(cli));
	if (options->console) {
		strcat(cli, "--console ");
	}

	if (!options->updates) {
		strcat(cli, "--skipupdates ");
	}

	if (options->lua_debug) {
		strcat(cli, "--luadebug ");
	}

	if (options->level_stage) {
		strcat(cli, "--set-stage=");
		char buffer[13]; // 11 chars for a max int (including sign) + 1 char for space + 1 char for '\0'
		sprintf(buffer, "%d ", options->level_stage);
		strcat(cli, buffer);
	}

	if (options->stage_type) {
		strcat(cli, "--set-stage-type=");
		char buffer[13]; // 11 chars for a max int (including sign) + 1 char for space + 1 char for '\0'
		sprintf(buffer, "%d ", options->stage_type);
		strcat(cli, buffer);
	}

	if (options->lua_heap_size) {
		strcat(cli, "--luaheapsize=");
		strcat(cli, options->lua_heap_size);
	}
}

DWORD CreateIsaac(struct IsaacOptions const* options, PROCESS_INFORMATION* processInfo) {
	STARTUPINFOA startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));

	memset(processInfo, 0, sizeof(*processInfo));

	char cli[256];
	GenerateCLI(options, cli);

	DWORD result = CreateProcessA("isaac-ng.exe", cli, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, processInfo);
	if (result == 0) {
		Log("Failed to create process: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Started isaac-ng.exe in suspended state, processID = %d\n", processInfo->dwProcessId);
	}

	return result;
}

int UpdateMemory(HANDLE process, PROCESS_INFORMATION const* processInfo, void** page, size_t* functionOffset) {
	void* remotePage = VirtualAllocEx(process, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!remotePage) {
		Log("Failed to allocate memory in isaac-ng.exe to load the dsound DLL: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Allocated memory for remote thread at %p\n", remotePage);
	}

	SIZE_T bytesWritten = 0;
	char zeroBuffer[4096];
	memset(zeroBuffer, 0, sizeof(zeroBuffer));
	WriteProcessMemory(process, remotePage, zeroBuffer, 4096, &bytesWritten);

	HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
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

	*functionOffset = offset;

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

	*page = remotePage;
	return 0;
}

int FirstStageInit(struct IsaacOptions const* options, HANDLE* outProcess, void** page, size_t* functionOffset, PROCESS_INFORMATION* processInfo) {
	{
		FILE* f = fopen("injector.log", "w");
		if (f) {
			fclose(f);
		}
	}

	Log("Starting injector\n");
	DWORD processId = CreateIsaac(options, processInfo);
	
	HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE, processInfo->dwProcessId);
	if (!process) {
		Log("Failed to open process: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Acquired handle to isaac-ng.exe, process ID = %d\n", processInfo->dwProcessId);
	}

	if (UpdateMemory(process, processInfo, page, functionOffset)) {
		return -1;
	}

	*outProcess = process;
	
	return 0;
}

int CreateAndWait(HANDLE process, void* remotePage, size_t functionOffset) {
	HANDLE remoteThread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)((char*)remotePage + functionOffset), remotePage, 0, NULL);
	if (!remoteThread) {
		Log("Error while creating remote thread: %d\n", GetLastError());
		return -1;
	}
	else {
		Log("Created remote thread in isaac-ng.exe\n");
	}

	Log("Waiting for remote thread to complete\n");
	DWORD result = WaitForSingleObject(remoteThread, 60 * 1000);
	switch (result) {
	case WAIT_OBJECT_0:
		Log("RemoteThread completed\n");
		break;

	case WAIT_ABANDONED:
		Log("This shouldn't happened: RemoteThread returned WAIT_ABANDONNED\n");
		return -1;

	case WAIT_TIMEOUT:
		Log("RemoteThread timed out\n");
		return -1;

	case WAIT_FAILED:
		Log("WaitForSingleObject on RemoteThread failed: %d\n", GetLastError());
		return -1;
	}

	return 0;
}

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

int InjectIsaac(int updates, int console, int lua_debug, int level_stage, int stage_type, const char* lua_heap_size) {
	HANDLE process;
	void* remotePage;
	size_t functionOffset;
	PROCESS_INFORMATION processInfo;

	struct IsaacOptions options;
	options.updates = updates;
	options.console = console;
	options.lua_debug = lua_debug;
	options.level_stage = level_stage;
	options.stage_type = stage_type;
	options.lua_heap_size = lua_heap_size;

	if (FirstStageInit(&options, &process, &remotePage, &functionOffset, &processInfo)) {
		return -1;
	}

	if (CreateAndWait(process, remotePage, functionOffset)) {
		return -1;
	}

	DWORD result = ResumeThread(processInfo.hThread);
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

bool IsaacLauncher::App::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show();
	frame->PostInit();
	return true;
}

wxIMPLEMENT_APP(IsaacLauncher::App);