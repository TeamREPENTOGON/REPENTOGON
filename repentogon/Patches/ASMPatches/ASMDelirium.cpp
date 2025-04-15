#include <sstream>

#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMDelirium.h"
#include "../../REPENTOGONDelirium.h"


namespace delirium {
	std::map<Entity_NPC*, std::tuple<int, int, bool>> ForcedTransformations;
}

static bool __stdcall PreTransformationCallback(Box<Entity_NPC*> deliriumBox) {
	lua_State* L = g_LuaEngine->_state;
	Entity_NPC* delirium = deliriumBox.Get();

	lua::LuaStackProtector protector(L);
	lua::PushCallbackRegistryKey();
	lua::LuaCaller caller(L);
	lua::LuaResults results = // caller.pushCallbackID("PRE_TRANSFORMATION", "DeliriumCallbacks")
		caller.push("DeliriumPreTransformation")
		.pushnil()
		.pushLuabridge(delirium, delirium::DeliriumMetatable)
		.call(1);

	if (!results) {
		if (lua_isboolean(L, -1)) {
			bool res = lua_toboolean(L, -1);
			if (!res) {
				// Force an attack state to prevent Delirium from becoming stuck
				// in a loop of attempting to transform. 
				*delirium->GetDeliriumState() = 8; 
			}

			return res;
		}
	}

	return true;
}

struct DeliriumTransformationData {
	int type;
	int variant;
};

static void __stdcall TransformationCallback(Box<DeliriumTransformationData*> bdata, Box<Entity_NPC*> bdelirium, Box<int> btype, Box<int> bvariant) {
	Entity_NPC* delirium = bdelirium.Get();
	int type = btype.Get(), variant = bvariant.Get();
	
	auto iter = delirium::ForcedTransformations.find(delirium);
	bool forced = iter != delirium::ForcedTransformations.end();
	DeliriumTransformationData* data = bdata.Get();

	if (forced) {
		type = std::get<0>(iter->second);
		variant = std::get<1>(iter->second);
		bool process = std::get<bool>(iter->second);
		delirium::ForcedTransformations.erase(iter);

		if (!process) {
			data->type = type;
			data->variant = variant;
			return;
		}
	}

	
	data->type = type;
	data->variant = variant;

	lua_State* L = g_LuaEngine->_state;

	lua::LuaStackProtector protector(L);
	lua::PushCallbackRegistryKey(L);
	lua::LuaCaller caller(L);
	lua::LuaResults results = caller.pushCallbackID("TRANSFORMATION", "DeliriumCallbacks")
		.pushnil()
		.pushLuabridge(delirium, delirium::DeliriumMetatable)
		.push(type)
		.push(variant)
		.push(forced)
		.call(1);

	if (!results) {
		if (lua_isboolean(L, -1)) {
			// True lets the transformation happen
			if (lua_toboolean(L, -1)) {
				return;
			}

			// False prevents it
			data->type = *delirium->GetDeliriumBossType();
			data->variant = *delirium->GetDeliriumBossVariant();
		}
		else if (lua_istable(L, -1)) {
			lua_len(L, -1);
			int length = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
			
			// Empty table
			if (length == 0) {
				return;
			}
			else {
				int luaType = lua_rawgeti(L, -1, 1);
				// Unreadable content
				if (luaType != LUA_TNUMBER) {
					return;
				}

				data->type = (int)lua_tointeger(L, -1);
				// Variant defaults to 0
				data->variant = 0;
				lua_pop(L, 1);

				// >= because people may return more data than necessary
				if (length >= 2) {
					luaType = lua_rawgeti(L, -1, 2);
					// Unreadable content
					if (luaType != LUA_TNUMBER) {
						return;
					}
					else {
						data->variant = (int)lua_tointeger(L, -1);
						lua_pop(L, 1);
					}
				}
			}
		}
		else {
			// Invalid return type or nil
			return;
		}
	}
}

static void __stdcall PostTransformationCallback(Box<Entity_NPC*> bdelirium) {
	Entity_NPC* delirium = *bdelirium;

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);
	lua::PushCallbackRegistryKey(L);
	lua::LuaCaller caller(L);
	caller.pushCallbackID("POST_TRANSFORMATION", "DeliriumCallbacks")
		.pushnil()
		.pushLuabridge(delirium, delirium::DeliriumMetatable)
		.call(0);
}

namespace delirium {
	const char* DeliriumMetatable = "DeliriumMT";

	void AddPreTransformationCallback() {
		SigScan scanner("0f8551080000ffb3380b0000");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		/* Full patch:
		 * 
		 * jz addr + 0x857
		 * <save registers>
		 * push ebx
		 * call PreTransformationCallback
		 * test al, al
		 * <restore registers>
		 * jnz addr + 0x6
		 * jmp addr + 0x857
		 */
		ASMPatch patch;
		ASMPatch::SavedRegisters saved(ASMPatch::SavedRegisters::EBX |
			ASMPatch::SavedRegisters::ECX |
			ASMPatch::SavedRegisters::EDX |
			ASMPatch::SavedRegisters::EDI |
			ASMPatch::SavedRegisters::ESI | 
			ASMPatch::SavedRegisters::EAX, true);

		// test al, al buffer
		ByteBuffer check;
		check.AddString("\x84\xc0");

		patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x857);
		patch.PreserveRegisters(saved);
		patch.Push(ASMPatch::Registers::EBX);
		// The call returns false if Delirium shouldn't transform, true if 
		// he should.
		patch.AddInternalCall(PreTransformationCallback);
		patch.AddBytes(check);
		patch.RestoreRegisters(saved); // This resets al to its correct value
		// Jump to the first valid instruction (it comes after the jne that was broken by the patch)
		patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x6);
		// Set al to 8
		patch.MoveImmediate(ASMPatch::Registers::EAX, 0x8);
		// Perform the jump that was broken
		patch.AddRelativeJump((char*)addr + 0x857);
		sASMPatcher.PatchAt(addr, &patch);
	}

	void AddTransformationCallback() {
		SigScan scanner("898b????????89bb");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		using Reg = ASMPatch::SavedRegisters;

		ASMPatch patch;
		// Don't preserve ecx and edi, they need to be updated to match
		// the decision taken by the user.
		ASMPatch::SavedRegisters registers(Reg::EAX | Reg::EBX | Reg::EDX | Reg::ESI, true);
		ASMPatch::StackSpace space = ASMPatch::StackSpace::Make<DeliriumTransformationData>(true);
		ByteBuffer leaBytes, movTypeBytes, movVariantBytes; 
		leaBytes.AddByte('\x8D') // opcode
			.AddByte(0b01000100) // ModRM, mod = 01 (8 bits disp), reg = 000 (EAX), rm = 100 (ESP)
			.AddByte(0b00100100) // SIB, scale = 00 (ignored), index = 100 (ESP), base = 100 (ESP)
			.AddByteBuffer(ASMPatch::ToHexString(12));

		movVariantBytes.AddByte('\x8B') // opcode
			.AddByte(0b01111100) // ModRM, mod = 01 (8 bits disp), reg = 111 (edi), rm = 100 (ESP)
			.AddByte(0b00100100) // SIB, scale = 00 (ignored), index = 100 (ESP), base = 100 (ESP)
			.AddByteBuffer(ASMPatch::ToHexString(0x4));

		movTypeBytes.AddByte('\x8B') // opcode
			.AddByte(0b00001100) // Mod RM, mod = 00 (no disp), reg = 001 (ecx), rm = 100 (ESP)
			.AddByte(0b00100100); // SIB, scale = 00 (ignored), index = 100 (ESP), base = 100 (ESP)

		patch.PreserveRegisters(registers)
			.AllocateReturn(space)
			.Push(ASMPatch::Registers::EDI) // Variant
			.Push(ASMPatch::Registers::ECX) // Type
			.Push(ASMPatch::Registers::EBX) // Delirium
			// .LoadEffectiveAddress(ASMPatch::Registers::ESP, 12, ASMPatch::Registers::EAX)
			.AddBytes(leaBytes)
			.Push(ASMPatch::Registers::EAX) // Return value
			.AddInternalCall(TransformationCallback)
			// .MoveFromMemory(ASMPatch::Registers::ESP, 0, ASMPatch::Registers::ECX)
			.AddBytes(movTypeBytes)
			// .MoveFromMemory(ASMPatch::Registers::ESP, 4, ASMPatch::Registers::EDI)
			.AddBytes(movVariantBytes)
			.FreeReturn(space)
			.RestoreRegisters(registers)
			.MoveToMemory(ASMPatch::Registers::ECX, 0xBC0, ASMPatch::Registers::EBX)
			.MoveToMemory(ASMPatch::Registers::EDI, 0xBC4, ASMPatch::Registers::EBX)
			.AddRelativeJump((char*)addr + 12);

		sASMPatcher.PatchAt(addr, &patch);
	}

	void AddPostTransformationCallback() {
		SigScan scanner("e9????????3c080f85????????8b43");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		ZyanU64 target;
		ZydisDisassembledInstruction instruction;
		ZydisDisassembleIntel(ZydisMachineMode::ZYDIS_MACHINE_MODE_LEGACY_32, (ZyanU64)addr, addr, 15, &instruction);
		ZydisCalcAbsoluteAddress(&instruction.info, instruction.operands, (ZyanU64)addr, &target);

		ZyanU64 distance = target - (ZyanU64)addr;

		using Reg = ASMPatch::Registers;
		using Save = ASMPatch::SavedRegisters;

		Save registers(Save::EAX | Save::EBX | Save::ECX | Save::EDX | Save::EDI | Save::ESI, true);

		ASMPatch patch;
		patch.PreserveRegisters(registers)
			.Push(Reg::EDI)
			.AddInternalCall(PostTransformationCallback)
			.RestoreRegisters(registers)
			.AddRelativeJump((char*)addr + distance);

		sASMPatcher.PatchAt(addr, &patch);
	}
}