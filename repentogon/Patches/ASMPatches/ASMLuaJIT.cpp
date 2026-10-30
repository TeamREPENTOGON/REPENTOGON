#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "ASMTweaks.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "../../REPENTOGONOptions.h"

// Room::Init manually invokes a full gc cycle, adding significant delay to room transitions
// for a pitiful gain of a few mb over standard gc (which immediately return afterwards!)
// Just skipping this makes room transitions fast again for, effectively, free.
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

void __stdcall UpdateLoadingMessageTrampoline(ModEntry* mod) {
	std::string loadMessage = std::string("Loading ") + mod->_name + "...";

	REPENTOGON::UpdateProgressDisplay(loadMessage.c_str());
}

void ASMPatchLuaLoadMessage() {

	SigScan scanner("050001000033ff");
	scanner.Scan();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	void* addr = scanner.GetAddress();

	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(UpdateLoadingMessageTrampoline)
		.RestoreRegisters(reg)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);

	printf("[REPENTOGON] Patching Lua mod loading messages at %p\n", addr);
}