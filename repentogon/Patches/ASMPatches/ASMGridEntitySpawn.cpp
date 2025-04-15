#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "HookSystem.h"

#include "ASMGridEntitySpawn.h"
#include "ASMGridEntitySpawn_Definitions.h"

/* /////////////////////
// shared SpawnGridEntity trampoline
*/ /////////////////////

bool __stdcall SpawnGridEntityTrampoline(int idx, unsigned int type, unsigned int variant, unsigned int seed, int vardata) {
	return g_Game->_room->SpawnGridEntity(idx, type, variant, seed, vardata);
}

/* /////////////////////
// Generic inline patch
*/ /////////////////////

void ASMPatchInlinedSpawnGridEntity(void* addr, GridEntityType type, std::optional<ASMPatch::Registers> variantReg, int variantOffset, ASMPatch::Registers idxReg, int idxOffset, ASMPatch::Registers seedReg, int seedOffset, int jumpOffset, std::optional<int> jumpCondOffset) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	
	patch.PreserveRegisters(savedRegisters)
		.Push(0); // vardata
	if (seedOffset != 0) {
		patch.Push(seedReg, seedOffset);
	}
	else
	{
		patch.Push(seedReg);
	}
	if (variantReg.has_value() == false)
	{
		patch.Push((int32_t)variantOffset); // variant constant
	}
	else
	{
		patch.Push(variantReg.value(), variantOffset); // variant from register
	}
	patch.Push((int32_t)type); // type
	if (idxOffset != 0) {
		patch.Push(idxReg, idxOffset);
	}
	else
	{
		patch.Push(idxReg);
	}
	patch.AddInternalCall(SpawnGridEntityTrampoline);
	if (jumpCondOffset.has_value())
	{
		patch.AddBytes("\x85\xc0");  // cmp eax, eax
	}
	patch.RestoreRegisters(savedRegisters);
	if (jumpCondOffset.has_value())
	{
		patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + jumpCondOffset.value());
	}
		patch.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchInlinedSpawnGridEntity()
{
	ZHL::Logger logger;
	for (const GridEntitySpawnPatchInfo& i : patches) {
		SigScan scanner(i.signature);
		if (!scanner.Scan()) {
			logger.Log("SpawnGridEntity patch %s failed, signature not found!\n", i.comment);
			continue;
		}
		void* addr = (char*)scanner.GetAddress() + i.sigOffset;

		logger.Log("Patching inlined SpawnGridEntity %s at %p\n", i.comment, addr);
		ASMPatchInlinedSpawnGridEntity(addr, i.type, i.variantReg, i.variantOffset, i.idxReg, i.idxOffset, i.seedReg, i.seedOffset, i.jumpOffset, i.jumpCondOffset);
	};
}

/* /////////////////////
// Room::spawn_entity
*/ /////////////////////

GridEntity * __stdcall RoomSpawnTrampoline(GridEntityType type, unsigned int variant, int vardata, unsigned int seed, unsigned int idx, unsigned short entrySubtype, unsigned int teleState) {
	const int callbackid = 1192;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push((unsigned int)type)
			.push((unsigned int)type)
			.push((unsigned int)variant)
			.push((int)vardata)
			.push((unsigned int)idx)
			.push((unsigned int)seed)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				type = (GridEntityType)lua::callbacks::ToInteger(L, 1);
				variant = (unsigned int)lua::callbacks::ToInteger(L, 2);
				vardata = lua::callbacks::ToInteger(L, 3);
				seed = (unsigned int)lua::callbacks::ToInteger(L, 4);
			}
			else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
			{
				return nullptr;
			}
		}
	}

	GridEntity* ent = Isaac::CreateGridEntity(type, idx);
	if (ent != nullptr) {
		g_Game->_room->_gridEntities[idx] = ent;
		GridEntityDesc* desc = ent->GetDesc();
		desc->_variant = variant;
		desc->_varData = vardata;

		if (type == GRID_TELEPORTER) {
			desc->_state = entrySubtype != 0 ? teleState : 0;
		}

		ent->Init(seed);
	}

	return ent;
}

void ASMPatchRoomSpawnEntity() {
	SigScan scanner_spawn_entity("e8????????8bc885c90f84????????837d??17");

	ZHL::Logger logger;
	if (!scanner_spawn_entity.Scan()) {
		logger.Log("Room::spawn_entity patch failed, signature not found!\n");
		return;
	}

	void* addr = scanner_spawn_entity.GetAddress();

	logger.Log("Patching Room::spawn_entity at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS - ASMPatch::SavedRegisters::Registers::ECX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, -0x44) // teleporter state
		.Push(ASMPatch::Registers::EBP, -0xa) // entry subtype
		.Push(ASMPatch::Registers::EDX) // idx
		.Push(ASMPatch::Registers::EBP, -0x40) // seed
		.Push(ASMPatch::Registers::EDI) // vardata
		.Push(ASMPatch::Registers::ESI) // variant
		.Push(ASMPatch::Registers::ECX) // type
		.AddInternalCall(RoomSpawnTrampoline)
		.AddBytes("\x85\xC0") // test eax,eax (does GridEntity exist)
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr - 0xf70) // jump to misc + return false
		.AddRelativeJump((char*)addr + 0x3a); // jump to misc + return true
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchGridSpawnCallback()
{
	PatchInlinedSpawnGridEntity();
	ASMPatchRoomSpawnEntity();
}