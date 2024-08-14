#include <sstream>

#include "ASMPatcher.hpp"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "SigScan.h"
#include "Delirium.h"
#include "Log.h"

static int32_t __stdcall VadeRetroCheck(Box<Entity_NPC*> bnpc, Box<EntityConfig_Entity*> bconfig) {
	Entity_NPC* npc = bnpc.Get();
	EntityConfig_Entity* config = bconfig.Get();

	if (config->tags & 0x8) { // "ghost" 
		if ((*npc->GetDeliriumBossType() == npc->_type && *npc->GetDeliriumBossVariant() == npc->_variant) || npc->_type == 412 /* Delirium */) {
			return 0;
		}

		return 1;
	}

	return 0;
}

namespace delirium {
	const char* DeliriumMetatable = "DeliriumMT";

	void PatchSkipFrames() {
		SigScan scanner("e85e5b5500");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		/* Override the call to the function that computes how many frames 
		 * must be skipped. Instead, set eax to 0 so no frames are skipped, 
		 * ever.
		 */
		ByteBuffer buffer;
		buffer.AddString("\x33\xC0"); // xor eax, eax
		ASMPatch patch(buffer);

		sASMPatcher.FlatPatch(addr, &patch, true);
	}

	void PatchCompanion() {
		SigScan scanner("e8f3140000");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		char buffer[5];
		memset(buffer, '\x90', 5);
		ASMPatch patch(ByteBuffer().AddAny(buffer, 5));
		sASMPatcher.FlatPatch(addr, &patch);
	}

	void PatchVadeRetro() {
		SigScan scanner("8b80d800000083e008");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		using Reg = ASMPatch::Registers;
		using Save = ASMPatch::SavedRegisters;

		Save registers(Save::EBX | Save::ECX | Save::EDX | Save::ESI | Save::EDI, true);
		ASMPatch patch;
		ByteBuffer orBytes;
		orBytes.AddAny("\x83\xC8\x00", 3);

		patch.PreserveRegisters(registers)
			.Push(Reg::EAX)
			.Push(Reg::ECX)
			.AddInternalCall(VadeRetroCheck)
			.AddBytes(orBytes)
			.RestoreRegisters(registers)
			.AddRelativeJump((char*)addr + 12);

		sASMPatcher.PatchAt(addr, &patch);
	}
}