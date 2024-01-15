#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

//////////////////////////////////////////////
// !!!!! GRID CALLBACK STUFF HERE !!!!!
//////////////////////////////////////////////

/* /////////////////
// Room::spawn_entity
*/ /////////////////

GridEntity* __stdcall RoomSpawnTrampoline(GridEntityType type, unsigned int variant, int vardata, unsigned int seed, unsigned int idx, unsigned short entrySubtype, unsigned int teleState) {
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
	scanner_spawn_entity.Scan();
	void* addr = scanner_spawn_entity.GetAddress();

	printf("[REPENTOGON] Patching Room::spawn_entity at %p\n", addr);

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
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr - 0xf2a) // jump to misc + return false
		.AddRelativeJump((char*)addr + 0x3a); // jump to misc + return true
	sASMPatcher.PatchAt(addr, &patch);
}

/* /////////////////////
// Room::SpawnGridEntity
*/ /////////////////////

void __stdcall PostGridSpawnTrampoline(GridEntity* grid) {
	const int callbackid = 1101;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(grid->GetType())
			.push(grid, lua::Metatables::GRID_ENTITY)
			.call(1);
	}
}

void ASMPatchSpawnGridEntityPost(void* addr) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // GridEntity
		.AddInternalCall(PostGridSpawnTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 5))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 5);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchPostSpawnGridEntity()
{
	SigScan scanner1("8947??b0015f5e5b5dc21400");
	SigScan scanner2("8947??b0015f5e5b595dc20800");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };

	printf("[REPENTOGON] Patching SpawnGridEntity POST_GRID_ENTITY_SPAWN at %p, %p\n", addrs[0], addrs[1]);
	ASMPatchSpawnGridEntityPost(addrs[0]);
	ASMPatchSpawnGridEntityPost(addrs[1]);
}

/* /////////////////////
// shared SpawnGridEntity trampoline
*/ /////////////////////

bool __stdcall SpawnGridEntityTrampoline(int idx, unsigned int type, unsigned int variant, unsigned int seed, int vardata) {
	return g_Game->_room->SpawnGridEntity(idx, type, variant, seed, vardata);
}

/* /////////////////////
// Generic inline patch
*/ /////////////////////

void ASMPatchInlinedSpawnGridEntity_Generic(void* addr, ASMPatch::Registers idxReg, int idxOffset, ASMPatch::Registers seedReg, int seedOffset, unsigned int jumpOffset, GridEntityType type, unsigned int variant) {
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
	patch.Push((int32_t)variant) // variant
		.Push((int32_t)type); // type
	if (idxOffset != 0) {
		patch.Push(idxReg, idxOffset);
	}
	else
	{
		patch.Push(idxReg);
	}
	patch.AddInternalCall(SpawnGridEntityTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

// Condensed that works for majority of cases
void ASMPatchInlinedSpawnGridEntity_Generic(void* addr, ASMPatch::Registers idxReg, int jumpOffset, GridEntityType type, unsigned int variant) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(0) // vardata
		.Push(ASMPatch::Registers::EAX) // seed
		.Push((int32_t)variant) // variant
		.Push((int32_t)type) // type
		.Push(idxReg) // idx
		.AddInternalCall(SpawnGridEntityTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchInlinedSpawnGridEntity_Generic(void* addr, ASMPatch::Registers idxReg, int jumpOffset, GridEntityType type, ASMPatch::Registers varReg, int varJumpOffset) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(0) // vardata
		.Push(ASMPatch::Registers::EAX) // seed
		.Push(varReg, varJumpOffset) // variant
		.Push((int32_t)type) // type
		.Push(idxReg) // idx
		.AddInternalCall(SpawnGridEntityTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchInlinedSpawnGridEntity()
{
	SigScan scanner_megafatty("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????c706");
	SigScan scanner_larryjr("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??08000000");
	SigScan scanner_chub("8b0d????????8945??8b81");
	SigScan scanner_dingle1("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????8b8d");
	SigScan scanner_dingle2("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??1c000000");
	SigScan scanner_gideon("8b0d????????8bbf");
	SigScan scanner_raglich_arm("8945??a1????????8b80");
	//
	SigScan scanner_singe_explosive_fart("8bd885f678");
	SigScan scanner_player_collide_with_grid("8945??81ffc00100007d??8b75");
	//
	//
	//
	SigScan scanner_get_giant_part("8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300000f8f");
	SigScan scanner_make_wall1("8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300007f??6854010000e8????????8bf083c4048975??8bcec745??00000000");
	SigScan scanner_make_wall2("8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300007f??6854010000e8????????8bf083c4048975??8bcec745??27000000");
	SigScan scanner_card_against_humanity("8945??a1????????8bb0????????8975");
	SigScan scanner_pickup_grid_entity1("8945??81ffbf010000");
	//
	SigScan scanner_try_spawn_sanguine_spikes("85f678??81fec00100007c??68????????6a03e8????????83c40881bc??????????84030000");
	SigScan scanner_pressure_plate_reward1("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c4048975??8bcec745??1c000000");
	SigScan scanner_pressure_plate_reward2("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c4048975??8bcec745??06000000");
	scanner_megafatty.Scan();
	scanner_larryjr.Scan();
	scanner_chub.Scan();
	scanner_dingle1.Scan();
	scanner_dingle2.Scan();
	scanner_gideon.Scan();
	scanner_raglich_arm.Scan();
	//
	scanner_singe_explosive_fart.Scan();
	scanner_player_collide_with_grid.Scan();
	//
	//
	//
	scanner_get_giant_part.Scan();
	scanner_make_wall1.Scan();
	scanner_make_wall2.Scan();
	scanner_card_against_humanity.Scan();
	scanner_pickup_grid_entity1.Scan();
	//
	scanner_try_spawn_sanguine_spikes.Scan();
	scanner_pressure_plate_reward1.Scan();
	scanner_pressure_plate_reward2.Scan();
	void* addrs[22] = {
		scanner_megafatty.GetAddress(),
		scanner_larryjr.GetAddress(),
		scanner_chub.GetAddress(),
		scanner_dingle1.GetAddress(),
		scanner_dingle2.GetAddress(),
		scanner_gideon.GetAddress(),
		scanner_raglich_arm.GetAddress(),
		0x0,
		scanner_singe_explosive_fart.GetAddress(),
		scanner_player_collide_with_grid.GetAddress(),
		0x0,
		0x0,
		0x0,
		scanner_get_giant_part.GetAddress(),
		scanner_make_wall1.GetAddress(),
		scanner_make_wall2.GetAddress(),
		scanner_card_against_humanity.GetAddress(),
		scanner_pickup_grid_entity1.GetAddress(),
		0x0,
		scanner_try_spawn_sanguine_spikes.GetAddress(),
		scanner_pressure_plate_reward1.GetAddress(),
		scanner_pressure_plate_reward2.GetAddress()
	};
	printf("[REPENTOGON] Patching inlined SpawnGridEntity starting from %p, read log for rest\n", addrs[0]);
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[0], ASMPatch::Registers::ESI, 0xf0, GRID_POOP, 1); // ai_mega_fatty
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[1], ASMPatch::Registers::ESI, 0xf6, GRID_POOP, 0); // ai_larryjr
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[2], ASMPatch::Registers::ESI, 0x109, GRID_POOP, 1); // ai_chub
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[3], ASMPatch::Registers::ESI, 0x139, GRID_POOP, 0); // ai_dingle (1)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[4], ASMPatch::Registers::ESI, 0xf0, GRID_POOP, 1); // ai_dingle (2)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[5], ASMPatch::Registers::EDI, 0xaf4, ASMPatch::Registers::EAX, 0x0, 0xc0, GRID_STAIRS, 1); // CreateGideonDungeon
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[6], ASMPatch::Registers::EDI, 0xc5, GRID_PIT, 0); // ai_raglich_arm
	// There's a Rotgut function here that needs a patch [7]
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[8], ASMPatch::Registers::ESI, 0xbd, GRID_PIT, 0); // DoExplosiveFart

	// This will need special handling, when trying to break a steel block it fails a GridPath check (I think) and returns without doing anything
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[9], ASMPatch::Registers::EDI, 0, 0x0, 0x145, GRID_ROCK, 0); // PlayerCollideWithGrid

	// So will BombDamage (Custom handling of valid grid idx range) [10]
	// And BombFlagTearEffects (grid idx range) [11]
	// BombFlagTearEffects again (??? can't tell) [12]
	// And get_giant_part (Room path check) [13]

	ASMPatchInlinedSpawnGridEntity_Generic(addrs[14], ASMPatch::Registers::EDI, 0xa9, GRID_DECORATION, 10); // make_wall (crawlspace ladder)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[15], ASMPatch::Registers::EDI, 0xa9, GRID_GRAVITY, 0); // make_wall (crawlspace gravity)
	// Possibly will need another, unique patch for the default case of spawning a wall, idk if it's worth it
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[16], ASMPatch::Registers::EDI, 0xe1, GRID_POOP, ASMPatch::Registers::EBP, -0x14); // card against humanity

	// This needs special handling bc the code loads g_Game back into EAX near the end of the inline
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[17], ASMPatch::Registers::EDI, 0xd5, GRID_DECORATION, 1000); // PickupGridEntity
	// So will the other use, inlining in this function is wacky [18]
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[19], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EBX, 0x0, 0xbd, GRID_SPIKES, 100); // TrySpawnSanguineBondSpike

	// These aren't done yet
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[20], ASMPatch::Registers::EAX, 0, ASMPatch::Registers::EBP, -0x1c, 0x1164, GRID_POOP, 0); // pressure plate reward (1)
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[21], ASMPatch::Registers::EAX, 0, ASMPatch::Registers::EBP, -0x1c, 0x1164, GRID_POOP, 3); // pressure plate reward (2)
}

void PatchGridCallbackShit()
{
	ASMPatchRoomSpawnEntity();
	PatchPostSpawnGridEntity();
	PatchInlinedSpawnGridEntity();
}


//////////////////////////////////////////////
// !!!!! GRID CALLBACK STUFF DONE !!!!!
//////////////////////////////////////////////