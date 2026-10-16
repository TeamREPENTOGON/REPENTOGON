#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

MOD_EXPORT Game* L_Game_Get() {
	return g_Game;
}

MOD_EXPORT void L_Game_Update(Game* game) {
	game->Update();
}

MOD_EXPORT void L_Game_Render(Game* game) {
	game->Render();
}

MOD_EXPORT bool L_Game_IsPaused(Game* game) {
	return game->IsPaused();
}

MOD_EXPORT void L_Game_Fadein(Game* game, float speed, bool showIcon, KColor* color) {
	game->FadeIn(speed, showIcon, color);
}

MOD_EXPORT void L_Game_Fadeout(Game* game, float speed, int fadeoutTarget, KColor* color) {
	game->FadeOut(speed, fadeoutTarget, color);
}

MOD_EXPORT void L_Game_End(Game* game, int endingID) {
	game->End(endingID);
}

MOD_EXPORT int L_Game_GetNumPlayers(Game* game) {
	return game->GetNumPlayers();
}

MOD_EXPORT Entity_Player* L_Game_GetPlayer(Game* game, int idx) {
	if (game->_playerManager._playerList.size() == 0) {
		return nullptr;
	}
	if (idx < 0) {
		idx = 0;
	}
	return game->GetPlayer(idx);
}

MOD_EXPORT Entity_Player* L_Game_GetNearestPlayer(Game* game, Vector* pos) {
	return game->GetNearestPlayer(pos);
}

MOD_EXPORT Entity_Player* L_Game_GetRandomPlayer(Game* game, Vector* pos, float radius) {
	return game->GetRandomPlayer(pos, radius);
}

MOD_EXPORT Font* L_Game_GetFont() {
	return &g_Manager->_font2_TeamMeatEx12;
}

MOD_EXPORT void L_Game_ShowFortune(Game* game) {
	game->ShowFortune();
}

MOD_EXPORT void L_Game_ShowRule(Game* game) {
	game->ShowRule();
}

MOD_EXPORT void L_Game_ChangeRoom(Game* game, int roomIndex, int dimension) {
	game->ChangeRoom(roomIndex, dimension);
}

MOD_EXPORT void L_Game_StartRoomTransition(Game* game, int roomIndex, int direction, int animation, Entity_Player* player, int dimension) {
	game->StartRoomTransition(roomIndex, direction, animation, player, dimension);
}

MOD_EXPORT void L_Game_StartStageTransition(Game* game, bool sameStage, int transition, Entity_Player* player) {
	game->StartStageTransition(sameStage, transition, player);
}

MOD_EXPORT void L_Game_MoveToRandomRoom(Game* game, bool iAmErrorRoom, int seed, Entity_Player* player) {
	game->MoveToRandomRoom(iAmErrorRoom, seed, player);
}

MOD_EXPORT bool L_Game_GetStateFlag(Game* game, int flag) {
	flag = std::clamp(flag, 0, 0x34);
	return (game->_gameStateFlags >> flag) & 1;
}

MOD_EXPORT void L_Game_SetStateFlag(Game* game, int flag, bool value) {
	flag = std::clamp(flag, 0, 0x34);
	const uint64_t mask = 1ull << flag;
	if (value) {
		game->_gameStateFlags |= mask;
	}
	else {
		game->_gameStateFlags &= ~mask;
	}
}

MOD_EXPORT void L_Game_NextVictoryLap(Game* game) {
	game->NextVictoryLap();
}

MOD_EXPORT void L_Game_RerollLevelCollectibles(Game* game) {
	game->RerollLevelCollectibles();
}

MOD_EXPORT void L_Game_RerollLevelPickups(Game* game, int seed) {
	game->RerollLevelPickups(seed);
}

MOD_EXPORT void L_Game_FinishChallenge(Game* game) {
	game->FinishChallenge();
}

MOD_EXPORT void L_Game_AddPixelation(Game* game, int duration) {
	game->AddPixelation(duration);
}

MOD_EXPORT void L_Game_ShowHallucination(Game* game, int frameCount, int backdrop) {
	game->ShowHallucination(frameCount, backdrop);
}

MOD_EXPORT bool L_Game_RerollEnemy(Game* game, Entity* entity, bool unk) {
	return game->RerollEnemy(entity, unk);
}

MOD_EXPORT void L_Game_AddEncounteredBoss(Game* game, int type, int variant) {
	game->AddEncounteredBoss(type, variant);
}

MOD_EXPORT bool L_Game_HasEncounteredBoss(Game* game, int type, int variant) {
	return game->HasEncounteredBoss(type, variant);
}

MOD_EXPORT void L_Game_AddDevilRoomDeal(Game* game) {
	game->AddDevilRoomDeal();
}

MOD_EXPORT void L_Game_SetChallenge(Game* game, int challenge) {
	game->SetChallenge(challenge);
}

MOD_EXPORT void L_Game_ShakeScreen(Game* game, int timeout) {
	game->ShakeScreen(timeout);
}

MOD_EXPORT Entity* L_Game_Spawn(Game* game, int type, int variant, Vector* pos, Vector* vel, Entity* spawner, int subtype, int seed) {
	return game->Spawn(type, variant, *pos, *vel, spawner, subtype, seed, 0);
}

MOD_EXPORT void L_Game_BombDamage(Game* game, Vector* pos, float damage, float radius, bool lineCheck, Entity* source, BitSet128* tearFlags, unsigned long long damageFlags, bool damageSource) {
	game->BombDamage(pos, damage, radius, lineCheck, source, tearFlags ? *tearFlags : BitSet128(), damageFlags, damageSource);
}

MOD_EXPORT void L_Game_BombExplosionEffects(Game* game, Vector* pos, float damage, BitSet128* tearFlags, ColorMod* color, Entity* source, float radiusMult, bool lineCheck, unsigned long long damageFlags, bool damageSource) {
	ColorMod defaultColor;
	game->BombExplosionEffects(pos, damage, tearFlags ? *tearFlags : BitSet128(), color ? color : &defaultColor, source, radiusMult, lineCheck, damageFlags, damageSource);
}

MOD_EXPORT void L_Game_BombTearflagEffects(Game* game, Vector* pos, float radius, BitSet128* tearFlags, Entity* source, float radiusMult) {
	game->BombTearflagEffects(pos, radius, *tearFlags, source, radiusMult);
}

MOD_EXPORT void L_Game_ButterBeanFart(Game* game, Vector* pos, float radius, Entity* source, bool showEffect, bool doSuperKnockback) {
	game->ButterBeanFart(pos, radius, source, showEffect, doSuperKnockback);
}

MOD_EXPORT void L_Game_CharmFart(Game* game, Vector* pos, float radius, Entity* source) {
	game->CharmFart(pos, radius, source);
}

MOD_EXPORT void L_Game_Fart(Game* game, Vector* pos, float radius, Entity* source, float fartScale, int fartSubType, ColorMod* color) {
	ColorMod defaultColor;
	game->Fart(pos, radius, source, fartScale, fartSubType, color ? *color : defaultColor);
}

MOD_EXPORT void L_Game_MakeShockwave(Game* game, Vector* pos, float amplitude, float speed, int duration) {
	game->MakeShockwave(*pos, amplitude, speed, duration);
}

MOD_EXPORT void L_Game_SpawnParticles(Game* game, Vector* pos, int variant, int num, float speed, ColorMod* color, float height, int subtype) {
	ColorMod defaultColor;
	game->SpawnParticles(pos, variant, num, speed, color ? *color : defaultColor, height, subtype);
}

MOD_EXPORT void L_Game_UpdateStrangeAttractor(Game* game, Vector* pos, float force, float radius) {
	game->UpdateStrangeAttractor(pos, nullptr, 0, force, radius);
}

MOD_EXPORT Entity* L_Game_SpawnBombCrater(Game* game, Vector* pos, float radius) {
	return game->SpawnBombCrater(pos, radius);
}

MOD_EXPORT void L_Game_DevolveEnemy(Game* game, Entity* entity) {
	game->DevolveEnemy(entity, nullptr);
}

MOD_EXPORT Entity_Effect* L_Game_ChainLightning(Game* game, Vector* pos, float baseDamage, BitSet128* flags, Entity* spawner) {
	return game->ChainLightning(pos, baseDamage, flags ? *flags : BitSet128(), spawner);
}

MOD_EXPORT void L_Game_AddErasedEnemy(Game* game, Entity* entity) {
	game->AddErasedEnemy(entity);
}

MOD_EXPORT void L_Game_AddErasedEnemyByIds(Game* game, int type, int variant) {
	// The game ignores subtypes for erased enemies do not believe the lies
	game->AddErasedEnemyByIds(type, variant, 0);
}

MOD_EXPORT void L_Game_RemoveErasedEnemy(Game* game, int type, int variant) {
	game->RemoveErasedEnemy(type, variant, 0);
}

MOD_EXPORT bool L_Game_IsErased(Game* game, int type, int variant, int subtype) {
	return game->IsErased(type, variant, subtype);
}

MOD_EXPORT void L_Game_ClearErasedEnemies(Game* game) {
	game->_erasedEntities.clear();
}

MOD_EXPORT bool L_Game_AchievementUnlocksDisallowed(Game* game) {
	return game->AchievementUnlocksDisallowed();
}

MOD_EXPORT bool L_Game_IsPauseMenuOpen(Game* game) {
	return game->IsPauseMenuOpen();
}

MOD_EXPORT int L_Game_GetPauseMenuState(Game* game) {
	return game->GetPauseMenu()->state;
}

MOD_EXPORT bool L_Game_IsHardMode(Game* game) {
	return game->IsHardMode();
}

MOD_EXPORT bool L_Game_IsErasedEntity(Game* game, Entity* entity) {
	return game->IsErased(entity->_type, entity->_variant, entity->_subtype);
}

MOD_EXPORT ChallengeParam* L_Game_GetChallengeParams(Game* game) {
	return game->GetChallengeParams();
}

MOD_EXPORT bool L_Game_IsGreedBoss(Game* game) {
	return game->IsGreedBoss();
}

MOD_EXPORT bool L_Game_IsGreedFinalBoss(Game* game) {
	return game->IsGreedFinalBoss();
}

MOD_EXPORT bool L_Game_IsGreedMode(Game* game) {
	return game->IsGreedMode();
}

MOD_EXPORT void L_Game_SetColorModifier(Game* game, ColorModState* color, bool lerp, float rate) {
	game->SetColorModifier(color, lerp, rate);
}

MOD_EXPORT void L_Game_SetBloom(Game* game, int time, float strength) {
	game->SetBloom(time, strength);
}

MOD_EXPORT void L_Game_ShowGenericLeaderboard(Game* game) {
	game->_leaderboard.Show(1, &game->_scoreSheet, false, false);
}

MOD_EXPORT void L_Game_CopyGenericPrompt(Game* game, GenericPrompt* out) {
	new (out) GenericPrompt(*game->GetGenericPrompt());
}

MOD_EXPORT void L_Game_AddShopVisits(Game* game, int visitCount) {
	game->_shopVisits += visitCount;

	if (game->_shopVisits >= 6 && !game->IsGreedMode()) {
		g_Manager->GetPersistentGameData()->TryUnlock(379); // Unlock schoolbag
	}
}

MOD_EXPORT void L_Game_RecordPlayerCompletion(int event) {
	g_Manager->RecordPlayerCompletion(event);
}
