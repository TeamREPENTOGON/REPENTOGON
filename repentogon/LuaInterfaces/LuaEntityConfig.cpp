#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "../Patches/EntityConfigEx.h"

MOD_EXPORT EntityConfig_Entity* L_EntityConfig_GetEntity(int type, int variant, int subtype) {
	return g_Manager->GetEntityConfig()->GetEntity(type, variant, subtype);
}

MOD_EXPORT EntityConfig_Player* L_EntityConfig_GetPlayer(int playerType) {
	return g_Manager->GetEntityConfig()->GetPlayer(playerType);
}

MOD_EXPORT int L_EntityConfig_GetPlayerCount() {
	return (int)g_Manager->GetEntityConfig()->GetPlayers()->size();
}

MOD_EXPORT EntityConfig_Baby* L_EntityConfig_GetBaby(int id) {
	return g_Manager->GetEntityConfig()->GetBaby(id);
}

MOD_EXPORT int L_EntityConfig_GetBabyCount() {
	return (int)g_Manager->GetEntityConfig()->GetBabies()->size();
}

/*
* EntityConfigEntity Functions
*/

MOD_EXPORT const char* L_EntityConfigEntity_GetModName(EntityConfig_Entity* entity) {
	if (entity->modEntry == nullptr) {
		return nullptr;
	}
	return entity->modEntry->_name.c_str();
}

MOD_EXPORT bool L_EntityConfigEntity_HasCustomTag(EntityConfig_Entity* entity, const char* tag) {
	if (EntityConfigEx::EntityEx* ex = EntityConfigEx::GetEntityEx(entity)) {
		return ex->HasCustomTag(tag);
	}
	return false;
}

// Returns the tag count; fills out with up to max of them.
MOD_EXPORT int L_EntityConfigEntity_GetCustomTags(EntityConfig_Entity* entity, const char** out, int max) {
	EntityConfigEx::EntityEx* ex = EntityConfigEx::GetEntityEx(entity);
	if (!ex) {
		return 0;
	}

	const std::set<std::string>& tags = ex->GetCustomTags();
	int i = 0;
	for (const std::string& tag : tags) {
		if (i >= max) {
			break;
		}
		out[i++] = tag.c_str();
	}
	return (int)tags.size();
}

MOD_EXPORT EntityConfig_Entity* L_EntityConfigEntity_GetDevolvedEntity(EntityConfig_Entity* entity) {
	if (entity->devolve.empty()) {
		return nullptr;
	}
	// The game only uses the first one.
	const Devolve& devolve = entity->devolve.front();
	return g_Manager->GetEntityConfig()->GetEntity(devolve.type, devolve.variant, devolve.subtype);
}

/*
* EntityConfigPlayer Functions
*/

std::unordered_map<int, int> TaintedMap = {
	{0, 21},  // Isaac
	{1, 22},  // Maggy
	{2, 23},  // Cain
	{3, 24},  // Judas
	{4, 25},  // BlueBaby
	{5, 26},  // Eve
	{6, 27},  // Samson
	{7, 28},  // Azazel
	{8, 29},  // Lazarus
	{9, 30},  // Eden
	{10, 31}, // Lost
	{11, 38}, // Lazarus 2
	{12, 24}, // Dark Judas
	{13, 32}, // Lilith
	{14, 33}, // Keeper
	{15, 34}, // Apollyon
	{16, 35}, // Forgotten
	{17, 40}, // Soul
	{18, 36}, // Bethany
	{19, 37}, // Jacob
	{20, 37}, // Esau
	{21, 0},  // IsaacB
	{22, 1},  // MaggyB
	{23, 2},  // CainB
	{24, 3},  // JudasB
	{25, 4},  // BlueBabyB
	{26, 5},  // EveB
	{27, 6},  // SamsonB
	{28, 7},  // AzazelB
	{29, 8},  // LazarusB
	{30, 9},  // EdenB
	{31, 10}, // LostB
	{32, 13}, // LilithB
	{33, 14}, // KeeperB
	{34, 15}, // ApollyonB
	{35, 16}, // ForgottenB
	{36, 18}, // BethanyB
	{37, 19}, // JacobB
	{38, 11}, // Lazarus2B
	{39, 19}, // Jacob2B
	{40, 17}, // SoulB
};

MOD_EXPORT EntityConfig_Player* L_EntityConfigPlayer_GetTaintedCounterpart(EntityConfig_Player* player) {
	int counterpartID = -1;

	if (TaintedMap.find(player->_id) != TaintedMap.end()) {
		counterpartID = TaintedMap[player->_id];
	}
	else if (!player->_bSkinParentName.empty()) {
		// Modded Tainted
		for (unsigned int i = 41; i < g_Manager->GetEntityConfig()->GetPlayers()->size(); i++) {
			EntityConfig_Player* otherPlayer = g_Manager->GetEntityConfig()->GetPlayer(i);
			if (otherPlayer->_id != player->_id && otherPlayer->_name == player->_bSkinParentName && otherPlayer->_bSkinParentName.empty()) {
				counterpartID = otherPlayer->_id;
				break;
			}
		}
		// Cache it
		TaintedMap[player->_id] = counterpartID;
	}
	else if (player->_moddedTaintedPlayerID > 40 && player->_moddedTaintedPlayerID != player->_id) {
		// Modded Non-Tainted
		counterpartID = player->_moddedTaintedPlayerID;
	}

	return g_Manager->GetEntityConfig()->GetPlayer(counterpartID);
}
