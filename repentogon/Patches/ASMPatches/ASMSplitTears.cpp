#include "ASMSplitTears.h"

#include <variant>
#include <string>

#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "SigScan.h"
#include "HookSystem.h"
#include "LuaCore.h"

namespace SplitTears {

// MC_POST_FIRE_SPLIT_TEAR
void RunPostFireSplitTearCallback(Entity_Tear* tear, Entity* sourceEntity, const CustomSplitTearType splitType) {
	const uint32_t sourceType = *sourceEntity->GetType();
	lua::Metatables sourceTypeMt;

	if (sourceType == 2) {
		sourceTypeMt = lua::Metatables::ENTITY_TEAR;
	} else if (sourceType == 7) {
		sourceTypeMt = lua::Metatables::ENTITY_LASER;
	} else if (sourceType == 8) {
		sourceTypeMt = lua::Metatables::ENTITY_KNIFE;
	} else {
		return;
	}

	SplitTearType splitTypeEnum = SPLIT_GENERIC;
	std::string customSplitTypeStr = "";

	if (std::holds_alternative<SplitTearType>(splitType)) {
		splitTypeEnum = std::get<SplitTearType>(splitType);
	} else if (std::holds_alternative<std::string>(splitType)) {
		customSplitTypeStr = std::get<std::string>(splitType);
	}

	int callbackid = 1025;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller luaCaller = lua::LuaCaller(L);
		luaCaller.push(callbackid);
		if (!customSplitTypeStr.empty()) {
			luaCaller.push(customSplitTypeStr.c_str());
		} else {
			luaCaller.push((int)splitTypeEnum);
		}
		luaCaller.push(tear, lua::Metatables::ENTITY_TEAR)
			.push(sourceEntity, sourceTypeMt);
		if (!customSplitTypeStr.empty()) {
			luaCaller.push(customSplitTypeStr.c_str());
		} else {
			luaCaller.push((int)splitTypeEnum);
		}
		luaCaller.call(1);
	}
}
void __stdcall PostFireSplitTearCallbackTrampoline(Entity_Tear* tear, Entity* source, const bool updateTear, const SplitTearType splitType) {
	if (updateTear) {
		tear->Update();
	}
	RunPostFireSplitTearCallback(tear, source, splitType);
}

Entity_Tear* FireSplitTear(Entity* sourceEntity, const Vector pos, const Vector vel, const float damageMult, const float sizeMult, const int variant, const CustomSplitTearType splitType) {
	float damage = 3.5f;
	float scale = 1.0f;
	BitSet128 flags;
	ColorMod color;

	if (sourceEntity->_type == ENTITY_TEAR) {
		Entity_Tear* sourceTear = (Entity_Tear*)sourceEntity;
		damage = sourceTear->_collisionDamage;
		scale = sourceTear->_fScale;
		flags = sourceTear->_tearFlags;
		color = sourceTear->_sprite._color;
	} else if (sourceEntity->_type == ENTITY_LASER) {
		Entity_Laser* sourceLaser = (Entity_Laser*)sourceEntity;
		damage = sourceLaser->_collisionDamage;
		scale = sourceLaser->_scale;
		flags = sourceLaser->_tearFlags;
		sourceLaser->CalculateTearColor(&color);
	} else if (sourceEntity->_type == ENTITY_KNIFE) {
		Entity_Knife* sourceKnife = (Entity_Knife*)sourceEntity;
		damage = sourceKnife->_collisionDamage;
		scale = sourceKnife->_sprite._scale.y;  // Entity_Knife::trigger_collision
		flags = sourceKnife->_tearFlags;
		color = sourceKnife->_sprite._color;
	}

	damage = std::max(damage * damageMult, 0.0f);
	scale *= sizeMult;
	flags.hh &= ~(1 << 31);  // Always remove ludo flag
	if (variant != 50) {
		flags.hh &= ~(1 << 18); // Remove the fetus flag if the tear is not a fetus
	}

	Entity_Tear* newTear = (Entity_Tear*)g_Game->Spawn(ENTITY_TEAR, variant, pos, vel, sourceEntity->GetSpawnerEntity(), 0, std::max(Isaac::genrand_int32(), 1u), 0);

	if (!newTear) {
		return nullptr;
	}

	Entity_Player* player = nullptr;

	// Tears (Entity_Tear::Update) and Lasers (Entity_Laser::update_hitparams/do_damage) usually check their SpawnerEntity for a player when evaluating relevant effects.
	// Tears occasionally check the Parent instead for some reason (Entity_Tear::explode_balloon).
	// Knives use a dedicated function (Entity_Knife::trigger_collision).
	if (sourceEntity->_type == ENTITY_KNIFE) {
		player = ((Entity_Knife*)sourceEntity)->GetPlayer();
	} else if (Entity* spawner = sourceEntity->GetSpawnerEntity(); spawner && spawner->_type == ENTITY_PLAYER) {
		player = spawner->ToPlayer();
	} else if (Entity* parent = sourceEntity->GetParent(); parent && parent->_type == ENTITY_PLAYER) {
		player = parent->ToPlayer();
	}

	// Default height/fallingspeed/fallingaccel mostly based on the behaviour of Cricket's Body.
	// Users will probably modify these values afterwards, but this provides a reasonable default.
	RNG fallingSpeedRng;
	fallingSpeedRng.SetSeed(newTear->_initSeed, 35);
	const float fallingSpeedOffset = 0.2f * fallingSpeedRng.RandomFloat();

	if (player) {
		newTear->SetHeight(player->_tearheight * 0.5f);
		newTear->_fallingAccel = player->_tearfallaccel;

		float fallingSpeedStat = player->_tearfallspeed;
		if (fallingSpeedStat > 0) {
			fallingSpeedStat *= 0.3f;
		}
		newTear->_fallingSpeed = fallingSpeedOffset - fallingSpeedStat;
	} else {
		newTear->SetHeight(-11.875f);
		newTear->_fallingSpeed = fallingSpeedOffset + 0.185f;
	}

	newTear->_baseDamage = damage;
	newTear->_collisionDamage = damage;
	newTear->SetScale(scale);
	newTear->SetTearFlags(flags);
	newTear->SetColor(&color, -1, 255, true, false);

	if (player) {
		newTear->SetParent(player);
	}

	newTear->Update();

	RunPostFireSplitTearCallback(newTear, sourceEntity, splitType);

	return newTear;
}

LUALIB_API int Lua_FireSplitTear(lua_State* L) {
	Entity* sourceEntity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const Vector pos = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const Vector vel = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	const float damageMult = (float)luaL_optnumber(L, 4, 0.5);
	const float sizeMult = (float)luaL_optnumber(L, 5, 0.6);
	const int variant = (int)luaL_optinteger(L, 6, 0);
	CustomSplitTearType splitType = SPLIT_GENERIC;
	if (!lua_isnoneornil(L, 7)) {
		if (lua_type(L, 7) != LUA_TNUMBER) {
			splitType = std::string(luaL_checkstring(L, 7));
		} else {
			splitType = (SplitTearType)luaL_checkinteger(L, 7);
		}
	}
	Entity_Tear* newTear = FireSplitTear(sourceEntity, pos, vel, damageMult, sizeMult, variant, splitType);
	if (newTear) {
		lua::luabridge::UserdataPtr::push(L, newTear, lua::GetMetatableKey(lua::Metatables::ENTITY_TEAR));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

void PatchCricketsBodySplitTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_CricketsBody);

	printf("[REPENTOGON] Patching Entity_Tear::Update for split tears (cricket's body) at %p\n", addr);
	
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(SPLIT_QUAD)
		.Push(0x1)
		.Push(ASMPatch::Registers::EDI)  // Original Tear
		.Push(ASMPatch::Registers::ESI)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

// Lachwhatever
void PatchAbsorbSplitTears() {
	const int newTearOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_AbsorbNewTearOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_Absorb);

	printf("[REPENTOGON] Patching Entity_Tear::Update for split tears (absorb tears) at %p\n", addr);
	
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(SPLIT_ABSORB)
		.Push(0x0)
		.Push(ASMPatch::Registers::EDI)  // Original Tear
		.Push(ASMPatch::Registers::EBP, newTearOffset)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

// Mucowhatever
void PatchSporeSplitTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_Spore);

	printf("[REPENTOGON] Patching Entity_Tear::Update for split tears (spore tears) at %p\n", addr);
	
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(SPLIT_SPORE)
		.Push(0x1)
		.Push(ASMPatch::Registers::ESI)  // Original Tear
		.Push(ASMPatch::Registers::EDI)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchLudovicoStickyTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LudoSticky);

	printf("[REPENTOGON] Patching Entity_Tear::handle_collision for split tears (ludo sticky) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(SPLIT_STICKY)
		.Push(0x0)
		.Push(ASMPatch::Registers::ESI)  // Original Tear
		.Push(ASMPatch::Registers::ECX)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchLaserStickyTears() {
	const int laserOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LaserStickyLaserOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LaserSticky);

	printf("[REPENTOGON] Patching Entity_Laser::damage_entities for split tears (sticky) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the bytes we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(SPLIT_STICKY)
		.Push(0x0)
		.Push(ASMPatch::Registers::EBP, laserOffset)  // Laser
		.Push(ASMPatch::Registers::EDX)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchKnifeStickyTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_KnifeSticky);

	printf("[REPENTOGON] Patching Entity_Knife::trigger_collision for split tears (sticky) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(SPLIT_STICKY)
		.Push(0x0)
		.Push(ASMPatch::Registers::EDI)  // Knife
		.Push(ASMPatch::Registers::ESI)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchLaserPopTears() {
	void* jumpAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LaserPopJump);
	printf("[REPENTOGON] Flat patching Entity_Laser::Update for split tears (POP! jump) at %p\n", jumpAddr);
	sASMPatcher.FlatPatch(jumpAddr, "\x74\x21", 2);

	const int tearOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LaserPopTearOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_LaserPop);

	printf("[REPENTOGON] Patching Entity_Laser::Update for split tears (POP!) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(SPLIT_POP)
		.Push(0x0)
		.Push(ASMPatch::Registers::EDI)  // Laser
		.Push(ASMPatch::Registers::EBP, tearOffset)  // Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchKnifeCricketsBodyParasiteTears(const char* desc, const char* asmDef, const SplitTearType splitType) {
	void* addr = sASMDefinitionHolder->GetDefinition(asmDef);
	printf("[REPENTOGON] Patching Entity_Knife::trigger_collision for split tears (%s) at %p\n", desc, addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x3))  // mov ECX, dword ptr [EBP-0x??]
		.PreserveRegisters(savedRegisters)
		.Push(splitType)
		.Push(0x1)
		.Push(ASMPatch::Registers::EDI)  // Original Tear/Laser/Knife
		.Push(ASMPatch::Registers::ECX)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchSplitTears(const char* funcName, const char* desc, const char* asmDef, const SplitTearType splitType) {
	void* addr = sASMDefinitionHolder->GetDefinition(asmDef);

	printf("[REPENTOGON] Patching %s for split tears (%s) at %p\n", funcName, desc, addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(splitType)
		.Push(0x1)
		.Push(ASMPatch::Registers::EDI)  // Original Tear/Laser/Knife
		.Push(ASMPatch::Registers::ESI)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchAngelicPrismTears() {
	const int originalTearOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_AngelicPrismOriginalTearOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SplitTears_AngelicPrism);

	printf("[REPENTOGON] Patching for Angelic Prism split tears at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(SPLIT_ANGELIC_PRISM)
		.Push(0x1)
		.Push(ASMPatch::Registers::EBP, originalTearOffset)  // Original Tear
		.Push(ASMPatch::Registers::ESI)  // New Tear
		.AddInternalCall(PostFireSplitTearCallbackTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForSplitTearCallback() {
	PatchCricketsBodySplitTears();
	PatchAbsorbSplitTears();
	PatchSporeSplitTears();
	PatchSplitTears("Entity_Tear::explode_balloon", "haemolacria", &AsmDefinitions::SplitTears_Haemo, SPLIT_BURST);
	PatchSplitTears("Entity_Tear::trigger_collision", "parasite", &AsmDefinitions::SplitTears_Parasite, SPLIT_PARASITE);
	PatchSplitTears("Entity_Tear::trigger_collision", "compound fracture", &AsmDefinitions::SplitTears_Bone, SPLIT_BONE);
	PatchLudovicoStickyTears();

	PatchSplitTears("Entity_Laser::trigger_collision", "cricket's body", &AsmDefinitions::SplitTears_LaserCricketsBody, SPLIT_QUAD);
	PatchLaserStickyTears();
	PatchLaserPopTears();

	PatchSplitTears("Entity_Knife::trigger_collision", "compound fracture", &AsmDefinitions::SplitTears_KnifeBone, SPLIT_BONE);
	PatchSplitTears("Entity_Knife::trigger_collision", "haemolacria", &AsmDefinitions::SplitTears_KnifeHaemo, SPLIT_BURST);
	PatchKnifeCricketsBodyParasiteTears("cricket's body", &AsmDefinitions::SplitTears_KnifeCricketsBody, SPLIT_QUAD);
	PatchKnifeCricketsBodyParasiteTears("parasite", &AsmDefinitions::SplitTears_KnifeParasite, SPLIT_PARASITE);
	PatchKnifeStickyTears();

	// This is kinda similar functionally but eh, doesn't feel right including this.
	//PatchSplitTears("Entity_Knife::Update", "sword beam", &AsmDefinitions::SplitTears_SwordBeam, SPLIT_SWORD_BEAM);

	PatchAngelicPrismTears();
}

HOOK_METHOD(Entity_Tear, MakeMultidimensionalCopy, (Entity* spawner) -> Entity_Tear*) {
	Entity_Tear* newTear = super(spawner);
	RunPostFireSplitTearCallback(newTear, this, SPLIT_MULTIDIMENSIONAL);
	return newTear;
}

// Wip scraps of split lasers
/*
void __stdcall LightShotTrampoline(Entity_Laser* laser, Entity_Tear* tear, int timeout) {
	laser->_timeout = timeout;
	RunPostFireSplitLaserCallback(laser, tear);
}
void PatchLightShotLaser() {
	SigScan scanner("e8????????eb??8b8f????????85c974??c687????????01");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Tear::Update for split laser (trisagion) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Timeout
		.Push(ASMPatch::Registers::EDI)  // Source Tear
		.Push(ASMPatch::Registers::ECX)  // Laser
		.AddInternalCall(LightShotTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchCSectionTechXLaser() {
	SigScan setRefScanner("558bec51568bf1578b7d");
	setRefScanner.Scan();
	void* setRefAddr = setRefScanner.GetAddress();
	printf("[REPENTOGON] Found address of EHandle_EntityPlayer_SetRef at %p\n", setRefAddr);

	SigScan scanner("8d8f????????e8????????eb??8d85");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Tear::Update for split laser (c-section tech x) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.CopyRegister(ASMPatch::Registers::EAX, ASMPatch::Registers::ECX)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // mov ECX, [EDI + 0x???]
		.AddInternalCall(setRefAddr)  // Call EHandle_EntityPlayer_SetRef
		.PreserveRegisters(savedRegisters)
		.Push(0x0)
		.Push(ASMPatch::Registers::EDI)  // Source Tear (fetus)
		.Push(ASMPatch::Registers::EAX)  // New Laser (tech X)
		.AddInternalCall(RunPostFireSplitLaserCallback)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}
*/

}  // namespace SplitTears
