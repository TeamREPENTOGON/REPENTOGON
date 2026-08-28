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
	printf("[REPENTOGON] Patching Lua indices (this will take a while)...\n");

	SigScan globalScanner("d8b9f0ff");

	ASMPatch globalPatch;
	globalPatch.AddBytes("\xF0\xD8\xFF\xFF");
	
	unsigned int amount = 0;

	PerformLuaASMPatch(&globalScanner, &globalPatch, &amount);

	printf("[REPENTOGON] Patched %d global indices\n", amount);

	SigScan uplevelScanner("d7b9f0ff");

	ASMPatch uplevelPatch;
	uplevelPatch.AddBytes("\xED\xD8\xFF\xFF");

	amount = 0;
	
	PerformLuaASMPatch(&uplevelScanner, &uplevelPatch, &amount);

	printf("[REPENTOGON] Patched %d uplevel indices\n", amount);
}

void ASMPatchLuaGC()
{
	SigScan scanner("8b3d????????8b35????????6a00");
	scanner.Scan();

	ASMPatch patch;
	void* addr = scanner.GetAddress();

	patch.AddRelativeJump((char*)addr + 0x3E);
	sASMPatcher.FlatPatch(addr, &patch);

	printf("[REPENTOGON] Patching out Room::Init GC at %p\n", addr);
}