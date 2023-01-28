#include <Windows.h>
#include "ConsoleWindow.h"
#include "utils.h"
#include <stdio.h>
#include <vector>
#include <time.h>
#include "version.h"

typedef int (*ModInitFunc)(int, char **);

static int argc = 0;
static char **argv = NULL;

static bool HasCommandLineArgument(const char *arg)
{
	if(!argv)
		argv = CommandLineToArgvA(GetCommandLineA(), &argc);

	for(int i=1 ; i < argc ; ++i)
	{
		if(_stricmp(argv[i], arg) == 0) return true;
	}

	return false;
}

static std::vector<HMODULE> mods;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		// Prevents the module from being unloaded by the game
		HMODULE hModule_Dupe;
		GetModuleHandleEx(NULL, "dsound.dll", &hModule_Dupe);

		// Initialize the console
		if(HasCommandLineArgument("-console"))
			ConsoleWindow::Init();
		
		/*if(GetIsaacVersion() != ISAAC_REBIRTH)
		{
			printf("Afterbirth does not support mod loading yet, aborting\n");
			return TRUE;
		}*/

		//ZHL::Init();

		// Load all the mods
		auto startTime = GetTickCount();

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
						mods.push_back(LoadLibraryA(findData.cFileName));
						//printf("loaded mod %s\n", fileName);
					}
				}
			} while(FindNextFile(hFind, &findData));
			FindClose(hFind);
		}

		// Call all initialization functions once all the mods have been loaded
		for(auto it = mods.begin() ; it != mods.end() ; ++it)
		{
			ModInitFunc init = (ModInitFunc)GetProcAddress(*it, "ModInit");
			if(init)
				init(argc, argv);
			else
				printf("failed to find ModInit (%08x)\n", *it);
		}

		printf("Loaded all mods in %d msecs\n", GetTickCount() - startTime);
	}
	/*else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		printf("unloading libs\n");
		for(auto it = mods.begin() ; it != mods.end() ; ++it)
		{
			FreeLibrary(*it);
		}
	}*/

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
