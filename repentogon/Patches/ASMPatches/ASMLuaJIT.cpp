#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "ASMTweaks.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "../../REPENTOGONOptions.h"

void PerformLuaASMPatch(SigScan* scanner, ASMPatch* patch, unsigned int* counter) {
	while ((*scanner).Scan()) {
		(*counter)++;
		void* addr = (char*)(*scanner).GetAddress();
		sASMPatcher.FlatPatch(addr, patch);
	}
}
void ASMPatchLuaJIT()
{
	printf("Patching Lua indices (this will take a while)...\n");

	SigScan globalScanner("d8b9f0ff");

	ASMPatch globalPatch;
	globalPatch.AddBytes("\xF0\xD8\xFF\xFF");
	
	unsigned int globalAmount = 0;

	PerformLuaASMPatch(&globalScanner, &globalPatch, &globalAmount);

	printf("Patched %d global indices\n", globalAmount);

	SigScan twoScanner("d7b9f0ff");
	ASMPatch twoPatch;
	twoPatch.AddBytes("\xED\xD8\xFF\xFF");

	unsigned int twoAmount = 0;
	
	PerformLuaASMPatch(&twoScanner, &twoPatch, &twoAmount);

	printf("Patched %d ??? indices\n", twoAmount);
}