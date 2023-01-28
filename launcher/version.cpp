#include "version.h"
#include <Windows.h>
#include <stdio.h>

static int g_Version = -1;

int GetIsaacVersion()
{
	if(g_Version < 0)
	{
		HMODULE hModule = GetModuleHandle(NULL);

		unsigned char *pBase = (unsigned char*)hModule;
		IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER*)pBase;
		IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS*)(pBase + dos->e_lfanew);

		if(pe->Signature != IMAGE_NT_SIGNATURE)
			g_Version = ISAAC_UNKNOWN;
		else
		{
			unsigned int sz = pe->OptionalHeader.SizeOfImage;
			if(sz == 2760704)
				g_Version = ISAAC_REBIRTH;
			else if(sz == 3825664)
				g_Version = ISAAC_AFTERBIRTH;
			else
				g_Version = ISAAC_UNKNOWN;
		}
	}

	return g_Version;
}
