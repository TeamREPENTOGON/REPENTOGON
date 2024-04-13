#include <Windows.h>

#include <cstdarg>
#include <cstdio>
#include <ctime>

static void Log(FILE* f, const char* kind, const char* fmt, ...);

extern "C" {
	__declspec(dllexport) int Launch() {
		FILE* f = fopen("launcher.log", "w");
		if (!f) {
			return -1;
		}

		Log(f, "INFO", "Loading ZHL\n");
		HMODULE zhl = LoadLibraryA("libzhl.dll");
		if (!zhl) {
			Log(f, "ERROR", "Unable to load ZHL\n");
			return -1;
		}

		Log(f, "INFO", "Loaded ZHL at %p\n", zhl);
		Log(f, "INFO", "Search InitZHL in ZHL\n");

		FARPROC initZhl = GetProcAddress(zhl, "InitZHL");
		if (!initZhl) {
			FreeLibrary(zhl);
			Log(f, "ERROR", "InitZHL not found\n");
			return -1;
		}

		Log(f, "INFO", "Found InitZHL at %p\n", initZhl);
		Log(f, "INFO", "Initializing ZHL\n");

		if (initZhl()) {
			Log(f, "ERROR", "Errors encountered while initializing ZHL, check zhl.log for infos\n");
			FreeLibrary(zhl);
			return -1;
		}

		Log(f, "INFO", "Successfully initialized ZHL\n");
		Log(f, "INFO", "Loading mods...\n");

		WIN32_FIND_DATAA data;
		memset(&data, 0, sizeof(data));
		HANDLE files = FindFirstFileA("zhl*.dll", &data);
		BOOL ok = (files != INVALID_HANDLE_VALUE);
		while (ok) {
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
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