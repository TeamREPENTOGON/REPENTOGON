#include "ASMPatcher.hpp"
#include "ASMTweaks.h"
#include "Log.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "../../REPENTOGONOptions.h"

namespace ASMPatches {
	static void __stdcall __TearDetonatorPatch(EntityList_EL*);
	static bool __stdcall __IsLeadeboardEntryInvalid(int schwagBonus, int timePenalty, int encodedGameVersion, int encodedGameVersionFromScoreSheet);

	bool FixGodheadEntityPartition() {
		SigScan signature("6aff56f3??????????????50f3??????????????e8????????8b");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		ASMPatch patch;
		patch.AddBytes(ByteBuffer().AddString("\x6a\x08"));
		sASMPatcher.FlatPatch(addr, &patch);

		return true;
	}

	bool FixTearDetonatorEntityList() {
		using Reg = ASMPatch::SavedRegisters;

		/* There is no decent place to patch. Ideally, checking if the list is empty
		 * before performing the patch should be done, but the game begins the loop
		 * immediately after performing such a check.
		 * 
		 * Patch immediately after acquiring the list instead. Tear Detonator should
		 * not be fired too many times either way.
		 */
		SigScan signature("c745??1b0000008d8d????????33ff");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		ASMPatch patch;
		Reg registers(Reg::XMM_REGISTERS | Reg::GP_REGISTERS_STACKLESS, true);
		patch.PreserveRegisters(registers);
		patch.Push(ASMPatch::Registers::EAX); // eax contains the address of the EntityList to modify
		patch.AddInternalCall(__TearDetonatorPatch);
		patch.RestoreRegisters(registers);
		patch.AddRelativeJump((char*)addr + 0x7);

		sASMPatcher.PatchAt(addr, &patch);

		return true;
	}

	/* Change the content of the entity list so that it is no longer shared with other
	 * lists.
	 */
	void __stdcall __TearDetonatorPatch(EntityList_EL* list) {
		list->Untie();
	}

	bool FixHushFXVeins() {
		// update_vein_tree
		SigScan signature("76??8d70");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		ASMPatch patch;
		patch.AddBytes("\xEB"); // jmp
		sASMPatcher.FlatPatch(addr, &patch);

		// RenderVeins
		SigScan signature2("83bf????????0074??8d87");
		if (!signature2.Scan()) {
			return false;
		}

		void* addr2 = signature2.GetAddress();
		ASMPatch patch2;
		patch2.AddBytes("\xEB\x07\x90\x90\x90\x90\x90"); // jmp
		sASMPatcher.FlatPatch(addr2, &patch2);

		return true;
	};

	bool SkipArchiveChecksums() {
		SigScan loopsig("8b40??8985????????0f1f44??00");	//0x00e19252 in ghidra for 1.9.7.7, beginning of an other "do-while" block
		SigScan ifchecksig("74??ffb5????????ff77");
		ASMPatch ifpatch;
		ASMPatch patch;

		if (!loopsig.Scan()) {
			return false;
		};
		if (!ifchecksig.Scan()) {
			return false;
		};

		void* startptr = loopsig.GetAddress();
		void* endptr = (void*)((char*)startptr + 0x99);		//0x00e192eb in ghidra for 1.9.7.7, the label slightly above the if check patch pos
		void* ifcheck = ifchecksig.GetAddress();

		ifpatch.AddBytes("\xEB");	//swap to uncond jump
		patch.AddRelativeJump(endptr);

		for (int i = 1; i < __argc; i++) {
			char* arg = __argv[i];
			if (strcmp("-skipchecksum", arg) == 0) {
				sASMPatcher.FlatPatch(startptr,&patch);
				sASMPatcher.FlatPatch(ifcheck, &ifpatch);
				break;
			};
		};
		return true;
	};

	bool __stdcall __IsLeadeboardEntryInvalid(int schwagBonus, int timePenalty, int encodedGameVersion, int encodedGameVersionFromScoreSheet) {
		//checks from the code of defunct greedierbutt.com site with restored original version check
		return schwagBonus > 19150 || timePenalty > INT_MAX || encodedGameVersion < encodedGameVersionFromScoreSheet;
	}

	bool LeaderboarEntryCheckerUpdate() {
		ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
		ASMPatch patch;

		SigScan signature("0f9c85????????7c");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		printf("[REPENTOGON] Patching Leaderboard::render_leaderboard at %p\n", addr);

		//base leaderboard entry offset - 0x4ec
		const int leaderboardEntryOffset = -0x4ec;

		patch.PreserveRegisters(savedRegisters)
			.Push(ASMPatch::Registers::EBP, -0xb4) // Encode game version (from ScoreSheet)
			.Push(ASMPatch::Registers::EBP, leaderboardEntryOffset + 0x38) // Encode game version
			.Push(ASMPatch::Registers::EBP, leaderboardEntryOffset + 0x20)// Time Penalty
			.Push(ASMPatch::Registers::EBP, leaderboardEntryOffset + 0x4) // Schwag bonus
			.AddInternalCall(__IsLeadeboardEntryInvalid)
			.AddBytes("\x84\xC0") // test al, al
			.RestoreRegisters(savedRegisters)
			.AddBytes("\x0F\x95\x95\xBF\xFA\xFF\xFF") // setnz ebp-0x541
			.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x1A) // jump for true
			.AddRelativeJump((char*)addr + 0x9);
		sASMPatcher.PatchAt(addr, &patch);

		return true;
	}

	bool AllowConsoleInOnline() {
		ASMPatch patch;
		patch.AddBytes("\xEB");

		return sASMPatcher.FlatPatch("74??2bc133f6", "AllowConsoleInOnline", &patch);
	};


	// Custom resource path patch
	const char* resourcesName = "resources";
	const char* repentogonResourcesName = "resources-repentogon";

	static bool PatchResourcePath() {
		SigScan scanner("e8????????8d4424??50e8????????50");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		SigScan addFilepath("558bec6aff68????????64a1????????5083ec6ca1????????33c58945??5650");
		if (!addFilepath.Scan()) {
			return false;
		}

		printf("[REPENTOGON] Patching RebuildContentMountPoints for addFilepath patch at %p\n", addFilepath.GetAddress());

		void* addFilepathAddr = addFilepath.GetAddress();
		void* stringPtr = &resourcesName;
		void* stringRgonPtr = &repentogonResourcesName;

		ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ ASMPatch::SavedRegisters::ECX, true);
		ASMPatch patch;
		patch.PreserveRegisters(reg)
			.AddBytes("\x8B\x0D").AddBytes(ByteBuffer().AddAny((char*)&stringPtr, 4)) // mov ecx, dword ptr ds:[0xXXXXXXXX]
			.AddInternalCall(addFilepathAddr) // call addFilepath
			.AddBytes("\x8B\x0D").AddBytes(ByteBuffer().AddAny((char*)&stringRgonPtr, 4)) // mov ecx, dword ptr ds:[0xXXXXXXXX]
			.AddInternalCall(addFilepathAddr) // call addFilepath
			.RestoreRegisters(reg)
			.AddRelativeJump((char*)addr + 0x5);
		sASMPatcher.PatchAt(addr, &patch);

		return true;
	}
	static bool PatchPushResourcePath() {
		SigScan scanner1("b9????????e8????????8bf083c4088975");
		SigScan scanner2("68????????8d4d??c745??01000000e8????????8d4d");
		if (!scanner1.Scan() || !scanner2.Scan()) {
			return false;
		}

		void* addr1 = scanner1.GetAddress();
		void* addr2 = scanner2.GetAddress();

		//Part one - pushing the main thing//

		printf("[REPENTOGON] Patching addFilepath - 1st part at %p\n", addr1);

		ASMPatch patch1;
		ByteBuffer buffer1;

		buffer1.AddByte('\x90', 2);
		patch1.CopyRegister(ASMPatch::Registers::EDI, ASMPatch::Registers::ECX)
			.Push(ASMPatch::Registers::ECX)
			.AddBytes(buffer1);

		sASMPatcher.FlatPatch(addr1, &patch1);

		//Part two - pushing something at jmp part//

		printf("[REPENTOGON] Patching addFilepath - 2nd part at %p\n", addr2);

		ASMPatch patch2;
		ByteBuffer buffer2;

		buffer2.AddByte('\x90', 4);
		patch2.Push(ASMPatch::Registers::EDI)
			.AddBytes(buffer2);

		sASMPatcher.FlatPatch(addr2, &patch2);

		return true;
	}

	void NativeRepentogonResources() {
		PatchResourcePath();
		PatchPushResourcePath();
	}
}