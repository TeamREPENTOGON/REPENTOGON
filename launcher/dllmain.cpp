#include <Windows.h>
#include <ImageHlp.h>
#include "ConsoleWindow.h"
#include "Updater.h"
#include "utils.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include "version.h"
#include "Logger.h"

#include <string>

typedef int (*ModInitFunc)(int, char **);

static std::vector<std::pair<std::string, HMODULE>> mods;

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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		sLogger->SetOutputFile("dsound.log", "w", true);
		sLogger->SetFlushOnLog(true);
		sLogger->Info("Loaded REPENTOGON dsound.dll\n");
		if (HasCommandLineArgument("-repentogonoff")) {
			FILE* f = fopen("repentogon.log", "a");
			if (f) {
				fprintf(f, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				fprintf(f, "!!!!!!!!!!!!REPENTOGON DISABLED!!!!!!!!!!!!\n");
				fprintf(f, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				fclose(f);
			}
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
		
		if (!HasCommandLineArgument("-skipupdates")) {
			sLogger->Info("dsound: Checking for updates\n");
			CheckForUpdates();
			sLogger->Info("dsound: Update checking done\n");
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
