#include <lua.hpp>

#include <vector>
#include <stdexcept>

#include "ASMPatcher.hpp"
#include "SigScan.h"
#include "LuaEntityNPC.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

struct FireProjectilesStorage {
	std::vector<Entity_Projectile*> projectiles;
	bool inUse = false;
};

thread_local FireProjectilesStorage projectilesStorage;

int Lua_NPCUpdateDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	bool unk = lua_toboolean(L, 2);

	npc->UpdateDirtColor(unk);
	return 0;
}

int Lua_NPCGetDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *npc->GetDirtColor();

	return 1;
}

int Lua_NPCGetControllerId(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	lua_pushnumber(L,*npc->GetControllerId());

	return 1;
}

int Lua_NPCSetControllerId(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	int unk = (int)lua_tonumber(L, 2);

	npc->SetControllerId(unk);
	return 0;
}

LUA_FUNCTION(Lua_NPCTryForceTarget) {
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	Entity* target = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	int duration = (int)luaL_checkinteger(L, 3);

	lua_pushboolean(L, npc->TryForceTarget(target, duration));
	return 1;
}

static std::vector<Entity_Projectile*>& InitProjectileStorage() {
	std::vector<Entity_Projectile*>& projectiles = projectilesStorage.projectiles;
	projectiles.clear();
	projectilesStorage.inUse = true;
	return projectiles;
}

static void ProjectileStorageToLua(lua_State* L, std::vector<Entity_Projectile*>& projectiles) {
	lua_newtable(L);
	for (int i = 0; i < projectiles.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, projectiles[i], lua::GetMetatableKey(lua::Metatables::ENTITY_PROJECTILE));
		lua_rawset(L, -3);
	}

	projectilesStorage.projectiles.clear();
	projectilesStorage.inUse = false;
}

LUA_FUNCTION(Lua_EntityNPC_FireProjectilesEx) {
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* velocity = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	uint32_t mode = luaL_checkinteger(L, 4);

	if (mode > 9) {
		return luaL_error(L, "Invalid projectile mode %u\n", mode);
	}

	ProjectileParams* params = lua::GetUserdata<ProjectileParams*>(L, 5, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");

	std::vector<Entity_Projectile*>& projectiles = InitProjectileStorage();
	npc->FireProjectiles(position, velocity, mode, params);
	ProjectileStorageToLua(L, projectiles);

	return 1;
}

LUA_FUNCTION(Lua_EntityNPC_FireBossProjectilesEx) {
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	int numProjectiles = luaL_checkinteger(L, 2);

	if (numProjectiles <= 0) {
		return luaL_error(L, "Invalid amount of projectiles %d\n", numProjectiles);
	}

	Vector* targetPos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	float trajectoryModifier = luaL_checknumber(L, 4);
	ProjectileParams* params = lua::GetUserdata<ProjectileParams*>(L, 5, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");

	std::vector<Entity_Projectile*>& projectiles = InitProjectileStorage();
	npc->FireBossProjectiles(numProjectiles, *targetPos, trajectoryModifier, *params);
	ProjectileStorageToLua(L, projectiles);

	return 1;
}

LUA_FUNCTION(Lua_NPCGetHitList) {
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	std::vector<unsigned int> hitList = npc->GetHitList();

	lua_newtable(L);
	int idx = 1;
	for (int index : hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

int Lua_NPCGetShieldStrength(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	lua_pushnumber(L, *npc->GetShieldStrength());

	return 1;
}

int Lua_NPCSetShieldStrength(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	*npc->GetShieldStrength() = luaL_checknumber(L, 2);

	return 1;
}

int Lua_NPCPlaySound(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	int id = (int)luaL_checkinteger(L, 2);
	float volume = (float)luaL_optnumber(L, 3, 1.0);
	int framedelay = (int)luaL_optinteger(L, 4, 2);
	bool loop = false;
	if lua_isboolean(L, 5)
		loop = lua_toboolean(L, 5);
	float pitch = (float)luaL_optnumber(L, 6, 1.0);

	npc->PlaySound(id, volume, framedelay, loop, pitch);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_NPC;
	lua::RegisterFunction(state, mt, "PlaySound", Lua_NPCPlaySound);
	lua::RegisterFunction(state, mt, "UpdateDirtColor", Lua_NPCUpdateDirtColor);
	lua::RegisterFunction(state, mt, "GetDirtColor", Lua_NPCGetDirtColor);
	lua::RegisterFunction(state, mt, "GetControllerId", Lua_NPCGetControllerId);
	lua::RegisterFunction(state, mt, "SetControllerId", Lua_NPCSetControllerId);
	lua::RegisterFunction(state, mt, "TryForceTarget", Lua_NPCTryForceTarget);
	lua::RegisterFunction(state, mt, "FireProjectilesEx", Lua_EntityNPC_FireProjectilesEx);
	lua::RegisterFunction(state, mt, "FireBossProjectilesEx", Lua_EntityNPC_FireBossProjectilesEx);
	lua::RegisterFunction(state, mt, "GetHitList", Lua_NPCGetHitList);
	lua::RegisterFunction(state, mt, "GetShieldStrength", Lua_NPCGetShieldStrength);
	lua::RegisterFunction(state, mt, "SetShieldStrength", Lua_NPCSetShieldStrength);
}

void __stdcall FireProjectilesEx_Internal(std::vector<Entity_Projectile*> const& projectiles) {
	if (!projectilesStorage.inUse) {
		return;
	}

	for (Entity_Projectile* projectile : projectiles) {
		projectilesStorage.projectiles.push_back(projectile);
	}
}

void __stdcall FireBossProjectilesEx_Internal(Entity_Projectile* projectile) {
	if (!projectilesStorage.inUse) {
		return;
	}

	projectilesStorage.projectiles.push_back(projectile);
}

void PatchFireProjectiles() {
	const char* signature = "33c92b55b4c1fa02894ddc8955e4";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	using Reg = ASMPatch::SavedRegisters::Registers;
	using GPReg = ASMPatch::Registers;

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(Reg::EAX | Reg::EBX | Reg::ECX | Reg::EDX | Reg::EDI | Reg::ESI | 
		Reg::XMM0 | Reg::XMM1 | Reg::XMM2 | Reg::XMM3 | Reg::XMM4 | Reg::XMM5, true);
	patch.PreserveRegisters(registers);
	// patch.MoveFromMemory(GPReg::EBP, -0x4C, GPReg::ESI, true);
	patch.LoadEffectiveAddress(GPReg::EBP, -0x4C, GPReg::ESI);
	patch.Push(GPReg::ESI);
	patch.AddInternalCall(FireProjectilesEx_Internal);
	patch.RestoreRegisters(registers);
	patch.AddBytes("\x33\xc9\x2b\x55\xb4");
	patch.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

void PatchFireBossProjectiles() {
	const char* signature = "f30f104424388bf883c414";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	using Reg = ASMPatch::SavedRegisters::Registers;
	using GPReg = ASMPatch::Registers;

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(Reg::GP_REGISTERS_STACKLESS | Reg::XMM0 | Reg::XMM1 | Reg::XMM2 | Reg::XMM3, true);
	patch.PreserveRegisters(registers);
	patch.Push(GPReg::EAX);
	patch.AddInternalCall(FireBossProjectilesEx_Internal);
	patch.RestoreRegisters(registers);
	patch.AddBytes("\xF3\x0f\x10\x44\x24\x38");
	patch.AddRelativeJump((char*)addr + 0x6);

	sASMPatcher.PatchAt(addr, &patch);
}
