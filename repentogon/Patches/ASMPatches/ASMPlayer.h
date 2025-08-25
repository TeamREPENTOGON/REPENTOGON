#pragma once

struct CheckFamiliarStorage {
	std::vector<Entity_Familiar*> familiars;
	bool inUse = false;
};

extern thread_local CheckFamiliarStorage familiarsStorage;

static std::vector<Entity_Familiar*>& InitFamiliarStorage() {
	std::vector<Entity_Familiar*>& familiars = familiarsStorage.familiars;
	familiars.clear();
	familiarsStorage.inUse = true;
	return familiars;
}

void ASMPatchCheckFamiliar();
void ASMPatchPlayerStats();
void ASMPatchPlayerNoShake();
void ASMPatchPlayerItemNoMetronome();
void ASMPatchMarsDoubleTapWindow();
void ASMPatchAddActiveCharge();
void ASMPatchPlayerHurtSound();
void ASMPatchPlayerDeathSound();
void ASMPatchPlayerDeathSoundSoulOfLazarus();
void ASMPatchPlayerDeathSoundLost();
void ASMPatchPlayerDeathSoundAstralProjection();