#include <Windows.h>
#include <ImageHlp.h>

#include <cstdarg>
#include <cstdio>
#include <ctime>

#include <vector>

#include "ConsoleWindow.h"
#include "HookSystem.h"

static void Log(FILE* f, const char* kind, const char* fmt, ...);

DWORD RedirectLua(FILE* f, HMODULE* outLua) {
	HMODULE isaac = GetModuleHandle(NULL);
	if (!isaac) {
		Log(f, "ERROR", "RedirectLua: unable to get main module handle %d\n", GetLastError());
		return -1;
	}

	PIMAGE_NT_HEADERS headers = ImageNtHeader(isaac);
	if (!headers) {
		Log(f, "ERROR", "RedirectLua: unable to get NT headers %d\n", GetLastError());
		return -1;
	}

	HMODULE lua = LoadLibrary("Lua5.4.dll");
	if (!lua) {
		Log(f, "ERROR", "RedirectLua: unable to load Lua 5.4 DLL %d\n", GetLastError());
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
		Log(f, "ERROR", "RedirectLua: unable to find import table for Lua5.3.3r.dll");
		return -1;
	}

	PIMAGE_THUNK_DATA originalData = (PIMAGE_THUNK_DATA)((char*)isaac + descriptor->OriginalFirstThunk);
	PIMAGE_THUNK_DATA resolvedData = (PIMAGE_THUNK_DATA)((char*)isaac + descriptor->FirstThunk);

	struct FunctionMapping {
		PIMAGE_THUNK_DATA thunk;
		void* addr;
	};

	std::vector<FunctionMapping> functionMapping;

	/* Search all Lua functions in the 5.4 libs and only proceed if all are
	 * found. This way we avoid a partial redirect.
	 */
	while (originalData->u1.Ordinal != 0) {
		DWORD data = originalData->u1.Ordinal;
		if (data & 0x80000000) {
			Log(f, "ERROR", "RedirectLua: found import by ordinal, unable to proceed\n");
			return -1;
		}

		PIMAGE_IMPORT_BY_NAME import = (PIMAGE_IMPORT_BY_NAME)((char*)isaac + originalData->u1.AddressOfData);
		const char* symbolName = import->Name;

		void* newFunction = GetProcAddress(lua, symbolName);
		if (!newFunction) {
			Log(f, "ERROR", "RedirectLua: unable to find symbol %s in Lua 5.4\n", symbolName);
			return -1;
		}

		FunctionMapping mapping;
		mapping.thunk = resolvedData;
		mapping.addr = newFunction;
		functionMapping.push_back(mapping);

		++originalData;
		++resolvedData;
	}

	/* This is a point of no return. Either we manage to redirect everything, 
	 * or the process dies.
	 */
	for (FunctionMapping& mapping : functionMapping) {
		MEMORY_BASIC_INFORMATION info;
		DWORD queryResult = VirtualQuery(mapping.thunk, &info, sizeof(info));
		if (queryResult == 0) {
			Log(f, "FATAL", "RedirectLua: VirtualQuery failed %d\n", GetLastError());
			abort();
		}

		DWORD originalProtect;
		BOOL protectResult = VirtualProtect(info.BaseAddress, info.RegionSize, PAGE_READWRITE, &originalProtect);
		if (!protectResult) {
			Log(f, "FATAL", "RedirectLua: VirtualProtect failed %d\n", GetLastError());
			abort();
		}

		*(void**)mapping.thunk = mapping.addr;

		protectResult = VirtualProtect(info.BaseAddress, info.RegionSize, originalProtect, &originalProtect);
		if (!protectResult) {
			Log(f, "FATAL", "RedirectLua: VirtualProtect reverse failed %d\n", GetLastError());
			abort();
		}
	}

	return 0;
}

static HMODULE luaHandle = NULL;

void LoadMods() {
	FILE* f = fopen("zhlLoader.log", "a");
	Log(f, "INFO", "Loading mods...\n");

	WIN32_FIND_DATAA data;
	memset(&data, 0, sizeof(data));
	HANDLE files = FindFirstFileA("zhl*.dll", &data);
	BOOL ok = (files != INVALID_HANDLE_VALUE);
	while (ok) {
		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || !strcmp(data.cFileName, "zhlLoader.dll")) {
			HMODULE mod = LoadLibraryA(data.cFileName);
			if (!mod) {
				Log(f, "WARN", "Unable to load mod %s\n", data.cFileName);
			}
			else {
				FARPROC init = GetProcAddress(mod, "ModInit");
				if (!init) {
					Log(f, "WARN", "No ModInit found in mod %s\n", data.cFileName);
					FreeLibrary(mod);
				}
				else {
					if (init()) {
						Log(f, "WARN", "Errors encountered while initializing mod %s\n", data.cFileName);
						FreeLibrary(mod);
					}
					else {
						Log(f, "INFO", "Successfully loaded and initialized mod %s at %p\n", data.cFileName, mod);
						/* env->mods.push_back(mod); */
					}
				}
			}
		}
		ok = FindNextFileA(files, &data);
	}

	fclose(f);

	ZHL::Init();
}

extern "C" {
	__declspec(dllexport) int Launch(bool initConsole) {
		FILE* f = fopen("zhlLoader.log", "w");
		if (!f) {
			return -1;
		}

		Log(f, "INFO", "Redirect calls to Lua 5.3.3 to Lua 5.4\n");
		/* DWORD redirectResult = RedirectLua(f, &luaHandle);
		if (redirectResult != 0) {
			Log(f, "ERROR", "Unable to redirect Lua (no redirections were made)\n");

			if (luaHandle != NULL) {
				FreeLibrary(luaHandle);
				luaHandle = NULL;
			}
		} */

		if (initConsole) {
			ConsoleWindow::Init();
		}

		Log(f, "INFO", "Loading ZHL\n");
		HMODULE zhl = LoadLibraryA("libzhl.dll");
		if (!zhl) {
			Log(f, "ERROR", "Unable to load ZHL\n");
			return -1;
		}

		Log(f, "INFO", "Loaded ZHL at %p\n", zhl);
		Log(f, "INFO", "Search InitZHL in ZHL\n");

		int (*initZhl)(void (*)()) = (int(*)(void(*)()))GetProcAddress(zhl, "InitZHL");
		if (!initZhl) {
			FreeLibrary(zhl);
			Log(f, "ERROR", "InitZHL not found\n");
			return -1;
		}

		Log(f, "INFO", "Found InitZHL at %p\n", initZhl);
		Log(f, "INFO", "Initializing ZHL\n");

		if (initZhl(&LoadMods)) {
			Log(f, "ERROR", "Errors encountered while initializing ZHL, check zhl.log for infos\n");
			FreeLibrary(zhl);
			return -1;
		}

		Log(f, "INFO", "Successfully initialized ZHL\n");
		fclose(f);

		return 0;
	}
}

void Log(FILE* f, const char* type, const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	time_t now = time(nullptr);
	tm* nowTm = localtime(&now);
	char timeBuffer[4096];
	strftime(timeBuffer, 4095, "[%Y-%m-%d %H:%M:%S] ", nowTm);

	fprintf(f, "[%s] ", type);
	vfprintf(f, fmt, va);
	fflush(f);
}