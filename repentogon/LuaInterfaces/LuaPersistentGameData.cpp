#include "IsaacRepentance.h"
#include "../Patches/XMLData.h"
#include "../Patches/ChallengesStuff.h"
#include "../Patches/AchievementsStuff.h"

static const unsigned int CHALLENGE_MAX = 45;

MOD_EXPORT PersistentGameData* L_PersistentGameData_Get() {
	return &g_Manager->_persistentGameData;
}

MOD_EXPORT bool L_PersistentGameData_AddBestiaryKill(PersistentGameData* pgd, int type, int variant) {
	return pgd->AddBestiaryKill(type, variant);
}

MOD_EXPORT void L_PersistentGameData_AddBossKilled(PersistentGameData* pgd, int boss) {
	pgd->AddBoss(boss);
}

MOD_EXPORT int L_PersistentGameData_GetBestiaryDeathCount(PersistentGameData* pgd, int type, int variant) {
	return pgd->GetBestiaryDeathCount(type, variant);
}

MOD_EXPORT int L_PersistentGameData_GetBestiaryEncounterCount(PersistentGameData* pgd, int type, int variant) {
	return pgd->GetBestiaryEncounterCount(type, variant);
}

MOD_EXPORT int L_PersistentGameData_GetBestiaryKillCount(PersistentGameData* pgd, int type, int variant) {
	return pgd->GetBestiaryKillCount(type, variant);
}

MOD_EXPORT int L_PersistentGameData_GetEventCounter(PersistentGameData* pgd, int eventCounter) {
	return pgd->GetEventCounter(eventCounter);
}

MOD_EXPORT void L_PersistentGameData_IncreaseEventCounter(PersistentGameData* pgd, int eventCounter, int count) {
	return pgd->IncreaseEventCounter(eventCounter, count);
}

MOD_EXPORT bool L_PersistentGameData_IsChallengeCompleted(PersistentGameData* pgd, int challengeID) {
	if (challengeID <= CHALLENGE_MAX) {
		return pgd->challenges[challengeID];
	}
	else {
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeID);
		return Challenges[node["name"] + node["sourceid"]] > 0;
	}
}

MOD_EXPORT bool L_PersistentGameData_TryUnlock(PersistentGameData* pgd, int unlock, bool blockPaperPopup) {
	if (blockPaperPopup) {
		nextSkipAchiev = unlock;
	}

	bool success = pgd->TryUnlock(unlock);
	if (!success) {
		// It failed, so reset state manually
		nextSkipAchiev = -1;
	}
	return success;
}

MOD_EXPORT bool L_PersistentGameData_Unlock(PersistentGameData* pgd, int unlock, bool blockPaperPopup) {
	if (blockPaperPopup) {
		nextSkipAchiev = unlock;
	}
	forceunlock = true;
	bool success = pgd->TryUnlock(unlock);
	forceunlock = false;
	if (!success) {
		// It failed, so reset state manually
		nextSkipAchiev = -1;
	}
	return success;
}

MOD_EXPORT bool L_PersistentGameData_Unlocked(PersistentGameData* pgd, int unlock) {
	return pgd->Unlocked(unlock);
}