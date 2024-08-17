#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "EntityPlus.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"

std::vector<Entity_Familiar*> InvalidateCachedFamiliarMultipliers(Entity_Player* player) {
	Room* room = g_Game->GetCurrentRoom();
	EntityList* entityList = room->GetEntityList();
	EntityList_EL queryResult(*entityList->GetUpdateEL());
	entityList->QueryType(&queryResult, 3, -1, -1, false, false);

	unsigned int size = queryResult._size;

	std::vector<Entity_Familiar*> out;

	if (size) {
		Entity** data = queryResult._data;
		while (size) {
			Entity_Familiar* familiar = (*data)->ToFamiliar();
			if (familiar && familiar->_player == player) {
				EntityFamiliarPlus* familiarPlus = GetEntityFamiliarPlus(familiar);
				if (familiarPlus) {
					familiarPlus->cachedMultiplier = std::nullopt;
				}
			}
			++data;
			--size;
		}
		queryResult.Destroy();
	}

	return out;
}

float RunEvaluateFamiliarMultiplierCallback(Entity_Familiar* familiar, const float baseMult) {
	// MC_EVALUATE_FAMILIAR_MULTIPLIER
	const int callbackid = 1225;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(familiar->_variant)
			.push(familiar, lua::Metatables::ENTITY_FAMILIAR)
			.push(baseMult)
			.push(familiar->_player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result && lua_isnumber(L, -1)) {
			const float newMult = (float)lua_tonumber(L, -1);
			if (newMult > 0) {
				return newMult;
			}
		}
	}
	return baseMult;
}

void RunEvaluateCustomCacheCallback(Entity_Player* player, const std::string& customcache) {
	if (customcache == "familiarmultiplier") {
		// Familiar multiplier has special treatment, and does not go through the usual callback.
		// Still otherwise implemented as a customcache because it is "triggered" in the same ways.
		// When this cache is triggered on the player, wipe the cached multipliers from all their
		// familiars, indirectly triggering MC_EVALUATE_FAMILIAR_MULTIPLIER instead.
		InvalidateCachedFamiliarMultipliers(player);
		return;
	}

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (!playerPlus) {
		// Aaah!
		return;
	}

	// MC_EVALUATE_CUSTOM_CACHE
	const int callbackid = 1224;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		float initialvalue = 0.0;

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(customcache.c_str())
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(customcache.c_str())
			.push(initialvalue)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				playerPlus->customCacheResults[customcache] = lua_isinteger(L, -1);
				return;
			} else if (lua_isnumber(L, -1)) {
				playerPlus->customCacheResults[customcache] = lua_tonumber(L, -1);
				return;
			}
		}
	}

	playerPlus->customCacheResults.erase(customcache);
}

void TriggerCustomCache(Entity_Player* player, const std::set<std::string>& customcaches, const bool immediate) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (immediate) {
		for (const std::string& customcache : customcaches) {
			RunEvaluateCustomCacheCallback(player, customcache);
		}
	} else if (playerPlus) {
		for (const std::string& customcache : customcaches) {
			playerPlus->customCacheTags.insert(customcache);
		}
		player->_temporaryeffects._shouldEvaluateCache = true;
	}
}

void TriggerNullCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.NullItemData->GetCustomCache(id), immediate);
}

void TriggerCollectibleCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.ItemData->GetCustomCache(id), immediate);
}

void TriggerTrinketCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.TrinketData->GetCustomCache(id), immediate);
}

void TriggerItemCustomCache(Entity_Player* player, ItemConfig_Item* item, const bool immediate) {
	if (item->type == 0) {  // Null
		TriggerNullCustomCache(player, item->id, immediate);
	}
	else if (item->type == 2) {  // Trinket
		TriggerTrinketCustomCache(player, item->id, immediate);
	}
	else {  // Collectible (Passive/Active/Familiar)
		TriggerCollectibleCustomCache(player, item->id, immediate);
	}
}

// Re-evaluate custom caches that appeared in XML data if EvaluateItems is run for CacheFlag.CACHE_ALL.
HOOK_METHOD_PRIORITY(Entity_Player, EvaluateItems, -1, () -> void) {
	const int cacheFlagAll = (1 << 16) - 1;  // CacheFlag.CACHE_ALL
	const bool evaluateAll = (this->_cacheFlags & cacheFlagAll) == cacheFlagAll;

	super();

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this);
	if (playerPlus) {
		if (evaluateAll) {
			for (const std::string& customcache : XMLStuff.AllCustomCaches) {
				playerPlus->customCacheTags.insert(customcache);
			}
		}
		if (!playerPlus->customCacheTags.empty()) {
			for (const std::string& customcache : playerPlus->customCacheTags) {
				RunEvaluateCustomCacheCallback(this, customcache);
			}
		}
		playerPlus->customCacheTags.clear();
	}
}

// Collectibles (both real and via wisps) trigger cache evaluations immediately when added or removed.

HOOK_METHOD_PRIORITY(Entity_Player, AddCollectible, -1, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	super(type, charge, firsttime, slot, vardata);
	TriggerCollectibleCustomCache(this, type, true);
}

HOOK_METHOD_PRIORITY(Entity_Familiar, WispInit, -1, () -> void) {
	super();

	if (this->_variant == 237 && this->_player) {
		TriggerCollectibleCustomCache(this->_player, this->_subtype, true);
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerCollectibleRemoved, -1, (unsigned int collectibleType) -> void) {
	super(collectibleType);
	TriggerCollectibleCustomCache(this, collectibleType, true);
}

// Trinkets & TemporaryEffects being added/removed don't trigger EvaluateItems immediately, instead waiting until the next EntityPlayer::Update.

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketAdded, -1, (unsigned int trinketType, bool firstTime) -> void) {
	super(trinketType, firstTime);
	TriggerTrinketCustomCache(this, trinketType, false);
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketRemoved, -1, (unsigned int trinketType) -> void) {
	super(trinketType);
	TriggerTrinketCustomCache(this, trinketType, false);
}

HOOK_METHOD_PRIORITY(TemporaryEffects, AddEffect, -1, (TemporaryEffect* effect, bool addCostume, int count) -> void) {
	super(effect, addCostume, count);
	if (!this->_disabled && count != 0 && this->_player && effect->_item) {
		TriggerItemCustomCache(this->_player, effect->_item, false);
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerEffectRemoved, -1, (ItemConfig_Item* item, int unused) -> void) {
	super(item, unused);
	if (item) {
		TriggerItemCustomCache(this, item, false);
	}
}

// Familiar Multiplier!
// Patched into the end of Entity_Familiar::GetMultiplier(). Multiplier is in XMM0.
void __stdcall FamiliarGetMultiplierTrampoline(Entity_Familiar* familiar) {
	EntityFamiliarPlus* familiarPlus = GetEntityFamiliarPlus(familiar);

	if (!familiarPlus) {
		// Aaah!
		return;
	}

	float mult = 1.0;
	__asm {
		movd mult, xmm0
	}

	if (!familiarPlus->cachedMultiplier.has_value() || familiarPlus->cachedMultiplierPlayer != (uintptr_t)familiar->_player) {
		familiarPlus->cachedMultiplier = RunEvaluateFamiliarMultiplierCallback(familiar, mult);
		familiarPlus->cachedMultiplierPlayer = (uintptr_t)familiar->_player;
	}

	mult = *familiarPlus->cachedMultiplier;

	__asm {
		movd xmm0, mult
	}
}
void PatchFamiliarGetMultiplierCallback() {
	SigScan scanner("5e5b8be55dc3????????????????????????????????????????????????????????558bec6aff68????????64a1????????5081ec28030000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // Entity_Familiar*
		.AddInternalCall(FamiliarGetMultiplierTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForCustomCache() {
	PatchFamiliarGetMultiplierCallback();
}
