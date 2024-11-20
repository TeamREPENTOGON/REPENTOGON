#include <Windows.h>
#include <ImageHlp.h>
#include "ConsoleWindow.h"
#include "dsound/utils.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include "version.h"
#include "dsound/Logger.h"
#include "updater/updater.h"
#include "updater/updater_resources.h"
#include <windows.h>
#include <psapi.h> // For MODULEINFO

#include <string>

typedef int (*ModInitFunc)(int, char **);

static std::vector<std::pair<std::string, HMODULE>> mods;

static void CheckForUpdates();

DWORD RedirectLua(HMODULE* outLua) {
	HMODULE isaac = GetModuleHandle(NULL);
	if (!isaac) {
		fprintf(stderr, "RedirectLua: unable to get main module handle %d\n", GetLastError());
		return -1;
	}

	PIMAGE_NT_HEADERS headers = ImageNtHeader(isaac);
	if (!headers) {
		fprintf(stderr, "RedirectLua: unable to get NT headers %d\n", GetLastError());
		return -1;
	}

	HMODULE lua = LoadLibrary("Lua5.4.dll");
	if (!lua) {
		fprintf(stderr, "RedirectLua: unable to load Lua 5.4 DLL %d\n", GetLastError());
		return -1;
	}

	*outLua = lua;

	PIMAGE_DATA_DIRECTORY importDataDirectory = headers->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT;
	PIMAGE_IMPORT_DESCRIPTOR descriptor = (PIMAGE_IMPORT_DESCRIPTOR)((char*)isaac + importDataDirectory->VirtualAddress);

	while (descriptor->Characteristics) {
		const char* name = (const char*)isaac + descriptor->Name;
		if (strcmp(name, "Lua5.3.3r.dll") == 0) {
			break;
		}

		++descriptor;
	}

	if (descriptor->Characteristics == 0) {
		fprintf(stderr, "RedirectLua: unable to find import table for Lua5.3.3r.dll");
		return -1;
	}

	PIMAGE_THUNK_DATA originalData = (PIMAGE_THUNK_DATA)((char*)isaac + descriptor->OriginalFirstThunk);
	PIMAGE_THUNK_DATA resolvedData = (PIMAGE_THUNK_DATA)((char*)isaac + descriptor->FirstThunk);

	struct FunctionMapping {
		PIMAGE_THUNK_DATA thunk;
		void* addr;
	};

	std::vector<FunctionMapping> functionMapping;

	while (originalData->u1.Ordinal != 0) {
		DWORD data = originalData->u1.Ordinal;
		if (data & 0x80000000) {
			fprintf(stderr, "RedirectLua: found import by ordinal, unable to proceed\n");
			return -1;
		}

		PIMAGE_IMPORT_BY_NAME import = (PIMAGE_IMPORT_BY_NAME)((char*)isaac + originalData->u1.AddressOfData);
		const char* symbolName = import->Name;

		void* newFunction = GetProcAddress(lua, symbolName);
		if (!newFunction) {
			fprintf(stderr, "RedirectLua: unable to find symbol %s in Lua 5.4\n", symbolName);
			return -1;
		}

		FunctionMapping mapping;
		mapping.thunk = resolvedData;
		mapping.addr = newFunction;
		functionMapping.push_back(mapping);

		++originalData;
		++resolvedData;
	}

	for (FunctionMapping& mapping : functionMapping) {
		MEMORY_BASIC_INFORMATION info;
		DWORD queryResult = VirtualQuery(mapping.thunk, &info, sizeof(info));
		if (queryResult == 0) {
			fprintf(stderr, "[fatal] RedirectLua: VirtualQuery failed %d\n", GetLastError());
			abort();
		}

		DWORD originalProtect;
		BOOL protectResult = VirtualProtect(info.BaseAddress, info.RegionSize, PAGE_READWRITE, &originalProtect);
		if (!protectResult) {
			fprintf(stderr, "[fatal] RedirectLua: VirtualProtect failed %d\n", GetLastError());
			abort();
		}

		*(void**)mapping.thunk = mapping.addr;

		protectResult = VirtualProtect(info.BaseAddress, info.RegionSize, originalProtect, &originalProtect);
		if (!protectResult) {
			fprintf(stderr, "[fatal] RedirectLua: VirtualProtect reverse failed %d\n", GetLastError());
			abort();
		}
	}

	return 0;
}

static HMODULE luaHandle = NULL;


std::string GetExeVersion() {
	HMODULE hModule = GetModuleHandle(NULL);
	MODULEINFO modInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO))) {
		return "Fucked";
	}
	const char* baseAddress = (const char*)modInfo.lpBaseOfDll;
	size_t moduleSize = modInfo.SizeOfImage;
	const char* searchStr = "isaacv";

	for (size_t i = 0; i < moduleSize - strlen(searchStr); ++i) {
		const char* potentialMatch = baseAddress + i;
		if (strncmp(potentialMatch, searchStr, strlen(searchStr)) == 0) {
			const char* afterIsaacv = potentialMatch + strlen(searchStr);
			const char* dashPos = strchr(afterIsaacv, '-');

			if (dashPos) {
				return std::string(afterIsaacv, dashPos);
			}
		}
	}

	return "Fucked";
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		InitCLI();
		sLogger->SetOutputFile("dsound.log", "w", true);
		sLogger->SetFlushOnLog(true);
		sLogger->Info("Loaded REPENTOGON dsound.dll\n");

		std::string version = GetExeVersion();
		sLogger->Info("Isaac Version: %s\n", version.c_str());
		if (HasCommandLineArgument("-repentogonoff") || HasCommandLineArgument("-repentogoff") || HasCommandLineArgument("-repentogone")) {
			sLogger->Info("Repentogon Disabled!\n");
			FILE* f = fopen("repentogon.log", "a");
			if (f) {
				fprintf(f, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				fprintf(f, "!!!!!!!!!!!!REPENTOGON DISABLED!!!!!!!!!!!!\n");
				fprintf(f, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				fclose(f);
			}
			return TRUE;
		}
		if (!HasCommandLineArgument("-skipupdates")) {
			sLogger->Info("dsound: Checking for updates\n");
			CheckForUpdates();
			sLogger->Info("dsound: Update checking done\n");
		}

		if (version != "1.9.7.4.J135") {
			sLogger->Info("This Version of Isaac is not compatible!!\n");
			return TRUE;
		}

		sLogger->Info("dsound: Overriding Lua 5.3.3 with Lua 5.4\n");
		DWORD redirectResult = RedirectLua(&luaHandle);
		if (redirectResult == -1) {
			if (luaHandle) {
				FreeLibrary(luaHandle);
				luaHandle = NULL;
			}

			sLogger->Error("dsound: Error while redirecting Lua calls, falling back to original Lua 5.3.3 DLL\n");
		}
		else {
			sLogger->Info("dsound: Successfully overriden Lua 5.3.3 with Lua 5.4\n");
		}

		// Prevents the module from being unloaded by the game
		HMODULE hModule_Dupe;
		GetModuleHandleEx(NULL, "dsound.dll", &hModule_Dupe);

		// Initialize the console
		if (HasCommandLineArgument("-console")) {
			sLogger->Info("dsound: Initializing console window\n");
			ConsoleWindow::Init();
			sLogger->Info("dsound: Initialized console window\n");
		}

		/*if(GetIsaacVersion() != ISAAC_REBIRTH)
		{
			printf("Afterbirth does not support mod loading yet, aborting\n");
			return TRUE;
		}*/

		//ZHL::Init();

		/* Initialize ZHL */
		HMODULE zhl = LoadLibrary("libzhl.dll");
		if (!zhl)
		{
			sLogger->Fatal("Unable to load libzhl.dll");
			return FALSE;
		}

		int(*zhlInit)() = (int(*)())GetProcAddress(zhl, "InitZHL");
		if (!zhlInit)
		{
			sLogger->Fatal("Unable to find \"InitZHL\" in libzhl.dll\n");
			FreeLibrary(zhl);
			return FALSE;
		}

		if (zhlInit())
		{
			sLogger->Fatal("Errors encountered while initializing ZHL\n");
			FreeLibrary(zhl);
			return FALSE;
		}

		// Load all the mods
		auto startTime = GetTickCount();

		sLogger->Info("dsound: Loading ZHL mods\n");
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile("*", &findData);

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					const char *fileName = findData.cFileName;

					int n = strlen(fileName);
					if(n >= 4 && !_stricmp(fileName + n - 4, ".dll") && !_strnicmp(fileName, "zhl", 3))
					{
						HMODULE library = LoadLibraryA(fileName);
						if (!library)
						{
							DWORD error = GetLastError();
							sLogger->Error("dsound: Unable to open library %s (error = %d)\n", fileName, error);
						}
						else
						{
							sLogger->Info("dsound: Loaded ZHL mod %s at %p\n", fileName, library);
							mods.push_back(std::make_pair(fileName, library));
						}
						//printf("loaded mod %s\n", fileName);
					}
				}
			} while(FindNextFile(hFind, &findData));
			FindClose(hFind);
		}

		sLogger->Info("dsound: Initializing ZHL mods\n");
		// Call all initialization functions once all the mods have been loaded
		for (auto it = mods.begin() ; it != mods.end() ; ++it)
		{
			ModInitFunc init = (ModInitFunc)GetProcAddress(it->second, "ModInit");
			if (init)
			{
				sLogger->Debug("dsound: Located ModInit function of mod %s at %p\n", it->first.c_str(), init);
				init(argc, argv);
			}
			else
				sLogger->Error("dsound: Failed to find ModInit in ZHL mod %s\n", it->first.c_str());
		}

		sLogger->Info("dsound: Loaded all ZHL mods in %d msecs\n", GetTickCount() - startTime);
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		sLogger->Info("dsound: Unloading dsound.dll\n");
		/* printf("unloading libs\n");
		for(auto it = mods.begin() ; it != mods.end() ; ++it)
		{
			FreeLibrary(*it);
		} */
		if (luaHandle) {
			FreeLibrary(luaHandle);
		}

		FILE* f = fopen("repentogon.log", "a");
		if (f) {
			fprintf(f, "Unloading dsound.dll\n");
			fclose(f);
		}
		else {
			MessageBox(0, "Unloading dsound.dll", "Info", MB_ICONINFORMATION);
		}
	}

	return TRUE;
}

void CheckForUpdates() {
	// Extract embedded updater and run it
	HMODULE dll = GetModuleHandle("dsound");
	HRSRC updater = FindResource(dll, MAKEINTRESOURCE(IDB_EMBEDEXE1), RT_RCDATA);
	if (!updater) {
		sLogger->Error("Unable to unpack updater: resource not found in executable !\n");
		return;
	}

	HGLOBAL global = LoadResource(dll, updater);
	if (!global) {
		sLogger->Error("Unable to unpack updater: last error = %d !\n", GetLastError());
		return;
	}

	DWORD size = SizeofResource(dll, updater);
	if (size == 0) {
		sLogger->Error("Invalid size of updater (0) !\n");
		return;
	}

	void* data = LockResource(global);
	if (!data) {
		sLogger->Error("Unable to lock updater !\n");
		return;
	}

	const char* filename = "./REPENTOGONUpdater_Base.exe";
	FILE* output = fopen(filename, "wb");
	if (!output) {
		sLogger->Error("Unable to open temporary file %s to extract the self updater\n", filename);
		return;
	}

	size_t count = fwrite(data, size, 1, output);
	if (count != size) {
		sLogger->Warn("Inconsistent amount of data written: expected %d, wrote %lu\n", size, count);
	}

	fclose(output);

	STARTUPINFOA startupInfos;
	PROCESS_INFORMATION processInfos;

	memset(&startupInfos, 0, sizeof(startupInfos));
	memset(&processInfos, 0, sizeof(processInfos));

	BOOL ok = CreateProcess(filename, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfos, &processInfos);
	if (!ok) {
		sLogger->Error("Unable to execute updater\n");
		return;
	}

	HANDLE h = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, processInfos.dwProcessId);
	if (!h) {
		sLogger->Error("Unable to get handle to updater process\n");
		return;
	}

	sLogger->Info("Waiting for updater to complete\n");
	if (WaitForSingleObject(h, INFINITE) == WAIT_FAILED) {
		sLogger->Error("Unable to wait until updater is done: %d\n", GetLastError());
		return;
	}
	DWORD exitCode = 0;
	ok = GetExitCodeProcess(h, &exitCode);
	if (!ok) {
		sLogger->Error("Error when getting exit code of updater: %d\n", GetLastError());
		return;
	}
	sLogger->Info("Updater completed with exit code %d\n", exitCode);

	switch (exitCode) {
	case UPDATER_EXIT_UPDATED:
		sLogger->Info("Updater ran successfully\nExiting the process as we need to restart\n");
		ExitProcess(0);
		break;

	case UPDATER_EXIT_NO_UPDATE_AVAILABLE:
		sLogger->Info("No update available, continuing with normal launch\n");
		break;

	case UPDATER_EXIT_ERROR:
		sLogger->Error("Errors were encountered during the update process\n");
		break;

	case UPDATER_EXIT_SKIPPED:
		sLogger->Info("Update skiped because CheckForUpdates is false\n");
		break;

	case UPDATER_EXIT_CLEANUP:
		sLogger->Info("Updater cleanup performed successfully\n");
		break;

	case UPDATER_EXIT_DEV_VERSION:
		sLogger->Info("Update skipped because we are running a dev version\n");
		break;

	case UPDATER_EXIT_DONT_UPDATE:
		sLogger->Info("Update skipped as the user refused the update\n");
		break;

	default:
		sLogger->Warn("Unexpected exit code %d\n", exitCode);
		break;
	}
}

/*
HOOK_METHOD_PRIORITY(InputManagerBase, Shutdown, -10000, () -> void)
{
	super();
	printf("shutdown\n");
	for(auto it = mods.begin() ; it != mods.end() ; ++it)
	{
		FreeLibrary(*it);
	}
	mods.clear();
	printf("end\n");
}
*/
