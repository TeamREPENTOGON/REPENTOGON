#include "ASMPatcher.hpp"
#include "ASMTweaks.h"
#include "Log.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "../../REPENTOGONOptions.h"

namespace ASMPatches {
	static void __stdcall __TearDetonatorPatch(EntityList_EL*);

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
		SigScan signature("c745fc1a0000008d8d????????33??89bd");
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

	namespace BerserkSpiritShacklesCrash
	{
		/* This pointer stores the address of the first element of the vector. 
		 * If the address of the first element after removing an effect is no 
		 * longer the same, adjust the iterator to make it point to the new 
		 * element.
		 */
		static void* begin = NULL;

		/* This pointer stores the address of the current effect before it 
		 * is removed from the vector. This works as an offset inside the
		 * vector: if the removed effect was in the middle then we need to 
		 * adjust the iterator based on this offset inside the new vector.
		 */
		static void* current = NULL;

		/* This function initializes both variables that are used to adjust 
		 * the iterator if needed.
		 */
		static void __stdcall SaveTemporaryEffectsVectorState(::TemporaryEffects* effects, void* iterator) {
			if (begin || current) {
				ZHL::Log("[CRITICAL] Berserk + Spirit Shackles crash ASM patch: the values of begin and current "
						 "have not been properly reset. This indicates an error in the state of the patch "
						 "and could lead to crashes.\n");
			}

			begin = effects->_effects.data();
			current = iterator;
		}

		/* Return the new value of the iterator. If the iterator does not 
		 * need to be changed, return its value. 
		 */
		static void* __stdcall AdjustTemporaryEffectsIterator(::TemporaryEffects* effects, void* iterator) {
			void* newBegin = effects->_effects.data();
			void* result = iterator;
			if (newBegin != begin) {
				ptrdiff_t diff = (uintptr_t)current - (uintptr_t)begin;
				if (diff < 0) {
					ZHL::Log("[EMERG] Berserk + Spirit Shackles crash ASM patch: the value of current is lower "
							 "than the value of begin. This may indicate a memory corruption inside Repentogon.\n");
					return iterator;
				}

				result = (char*)newBegin + diff;
			}

			begin = current = NULL;
			return result;
		}

		/* Find the address at which to apply the patch to save the state of 
		 * the vector. Return NULL if not found.
		 */
		static void* FindPre() {
			SigScan scanner("FF742414FF742414E8????????C6471001");
			if (!scanner.Scan()) {
				ZHL::Log("[ERROR] Berserk + Spirit Shackles crash ASM patch: unable to find spot for first "
						 "patch.\n");
			}

			return scanner.GetAddress();
		}

		/* Find the address at which to apply the patch to adjust the iterator
		 * if needed. Return NULL if not found. 
		 */
		static void* FindPost() {
			SigScan scanner("8B4708EB30518B0D????????FF742414");
			if (!scanner.Scan()) {
				ZHL::Log("[ERROR] Berserk + Spirit Shackles crash ASM patch: unable to find spot for second "
						 "patch.\n");
			}

			return scanner.GetAddress();
		}

		static void PatchPre(void* addr) {
			ASMPatch patch;
			ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
			/* Important stuff in registers here: 
			 *	- esi is a pointer to the current TemporaryEffect
			 *  - edi is a pointer to the TemporaryEffect**s** object
			 */
			patch.PreserveRegisters(registers);
			patch.Push(ASMPatch::Registers::ESI)
				.Push(ASMPatch::Registers::EDI)
				.AddInternalCall(&SaveTemporaryEffectsVectorState); // SaveTemporaryEffectsVectorState(this, iterator);
			patch.RestoreRegisters(registers);
			patch.AddBytes("\xFF\x74\x24\x14\xFF\x74\x24\x14"); // Overriden instructions
			patch.AddRelativeJump((char*)addr + 8); // Jump to next valid instruction
			sASMPatcher.PatchAt(addr, &patch);
		}

		static void PatchPost(void* addr) {
			ASMPatch patch;
			/* Do not preserve ESI as we override it regardless */
			ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS & ~ASMPatch::SavedRegisters::ESI, true);
			patch.PreserveRegisters(registers);
			patch.Push(ASMPatch::Registers::ESI)
				.Push(ASMPatch::Registers::EDI)
				.AddInternalCall(AdjustTemporaryEffectsIterator);
			/* eax contains the new value for esi */
			patch.CopyRegister(ASMPatch::Registers::ESI, ASMPatch::Registers::EAX);
			patch.RestoreRegisters(registers);
			patch.AddBytes("\x8B\x47\x08");
			patch.AddRelativeJump((char*)addr + 53);
			sASMPatcher.PatchAt(addr, &patch);
		}

		bool Patch() {
			void* pre = FindPre();
			void* post = FindPost();

			if (!pre || !post) {
				return false;
			}

			PatchPre(pre);
			PatchPost(post);

			return true;
		}
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
		SigScan loopsig("8bf88d8d????????3d00020000");
		SigScan ifchecksig("74??ffb5????????8bb5");
		ASMPatch ifpatch;
		ASMPatch patch;

		if (!loopsig.Scan()) {
			return false;
		};
		if (!ifchecksig.Scan()) {
			return false;
		};

		void* startptr = loopsig.GetAddress();
		void* endptr = (void*)((char*)startptr + 0x8f);
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
}