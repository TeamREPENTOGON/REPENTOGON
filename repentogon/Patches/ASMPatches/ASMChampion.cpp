#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMChampion.h"
#include "../XMLData.h"

#include "ASMDefinition.h"
#include "IsaacRepentance.h"

extern WeightedOutcomePicker ASMChampion::weightedOutcomePicker[] = {
    WeightedOutcomePicker(),
    WeightedOutcomePicker(),
    WeightedOutcomePicker(),
};

extern std::vector<std::pair<eChampionColor, float>> ASMChampion::championChances[] = {
     // For LevelStage 1 - 2
     {
         {eChampionColor::CHAMPION_ORANGE, 0.05f},
         { eChampionColor::CHAMPION_BLUE, 0.05f },
         { eChampionColor::CHAMPION_DARK_RED, 0.05f },
         { eChampionColor::CHAMPION_CAMO, 0.0375f },
         { eChampionColor::CHAMPION_PULSE_GREEN, 0.0375f },
         { eChampionColor::CHAMPION_PULSE_GREY, 0.0375f },
         { eChampionColor::CHAMPION_FLY_PROTECTED, 0.0375f },
         { eChampionColor::CHAMPION_RED, 0.1167f },
         { eChampionColor::CHAMPION_GREEN, 0.1167f },
         { eChampionColor::CHAMPION_BLACK, 0.1167f },
         { eChampionColor::CHAMPION_GREY, 0.1167f },
         { eChampionColor::CHAMPION_PINK, 0.1167f },
         { eChampionColor::CHAMPION_LIGHT_BLUE, 0.1167f },
     },

         // For LevelStage 3 - 4
        {
            {eChampionColor::CHAMPION_ORANGE, 0.05f},
            {eChampionColor::CHAMPION_BLUE, 0.05f},
            {eChampionColor::CHAMPION_DARK_RED, 0.05f},
            {eChampionColor::CHAMPION_CAMO, 0.025f},
            {eChampionColor::CHAMPION_PULSE_GREEN, 0.025f},
            {eChampionColor::CHAMPION_PULSE_GREY, 0.025f},
            {eChampionColor::CHAMPION_FLY_PROTECTED, 0.025f},
            {eChampionColor::CHAMPION_TINY, 0.025f},
            {eChampionColor::CHAMPION_GIANT, 0.025f},
            {eChampionColor::CHAMPION_RED, 0.1f},
            {eChampionColor::CHAMPION_YELLOW, 0.1f},
            {eChampionColor::CHAMPION_GREEN, 0.1f},
            {eChampionColor::CHAMPION_BLACK, 0.1f},
            {eChampionColor::CHAMPION_GREY, 0.1f},
            {eChampionColor::CHAMPION_PINK, 0.1f},
            {eChampionColor::CHAMPION_LIGHT_BLUE, 0.1f},
        },

        // For LevelStage 5+
        {
            {eChampionColor::CHAMPION_WHITE, 0.005f},
            {eChampionColor::CHAMPION_RAINBOW, 0.005f},
            {eChampionColor::CHAMPION_TRANSPARENT, 0.01f},
            {eChampionColor::CHAMPION_FLICKER, 0.01f},
            {eChampionColor::CHAMPION_PURPLE, 0.01f},
            {eChampionColor::CHAMPION_BROWN, 0.01f},
            {eChampionColor::CHAMPION_ORANGE, 0.0333f},
            {eChampionColor::CHAMPION_BLUE, 0.0333f},
            {eChampionColor::CHAMPION_DARK_RED, 0.0333f},
            {eChampionColor::CHAMPION_CAMO, 0.025f},
            {eChampionColor::CHAMPION_PULSE_GREEN, 0.025f},
            {eChampionColor::CHAMPION_PULSE_GREY, 0.025f},
            {eChampionColor::CHAMPION_FLY_PROTECTED, 0.025f},
            {eChampionColor::CHAMPION_TINY, 0.025f},
            {eChampionColor::CHAMPION_GIANT, 0.025f},
            {eChampionColor::CHAMPION_PULSE_RED, 0.0125f},
            {eChampionColor::CHAMPION_SIZE_PULSE, 0.0125f},
            {eChampionColor::CHAMPION_KING, 0.0125f},
            {eChampionColor::CHAMPION_DEATH, 0.0125f},
            {eChampionColor::CHAMPION_RED, 0.0929f},
            {eChampionColor::CHAMPION_YELLOW, 0.0929f},
            {eChampionColor::CHAMPION_GREEN, 0.0929f},
            {eChampionColor::CHAMPION_BLACK, 0.0929f},
            {eChampionColor::CHAMPION_GREY, 0.0929f},
            {eChampionColor::CHAMPION_PINK, 0.0929f},
            {eChampionColor::CHAMPION_LIGHT_BLUE, 0.0929f},
        }
    };

int __stdcall ASMChampion::SelectChampionColor(unsigned int seed) {
	Level* level = g_Game->GetLevel();
	int levelStage = level->_stage;
	int chanceIndex;

	if (levelStage <= 2) {
		chanceIndex = 0;
	}
	else if (levelStage <= 4) {
		chanceIndex = 1;
	}
	else {
		chanceIndex = 2;
	}

    WeightedOutcomePicker& outcomePicker = ASMChampion::weightedOutcomePicker[chanceIndex];
    RNG rng = RNG();
    rng.SetSeed(seed, 35);
    return outcomePicker.PickOutcome(rng);
}


void ASMChampion::ASMPatchChampionInitialization() {
	void* championColorSelectAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Champion_ColorSelect);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);

    for (int i = 0; i < sizeof(ASMChampion::championChances) / sizeof(ASMChampion::championChances[0]); i++) {
        WeightedOutcomePicker &outcomePicker = ASMChampion::weightedOutcomePicker[i];

        for (const auto& [color, weight] : ASMChampion::championChances[i]) {
            outcomePicker.AddOutcomeWeight({ (uint32_t)color, (uint32_t)(weight * 100) }, false);
        }
    }


	ASMPatch championColorSelectPatch;
    championColorSelectPatch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ESI) // Seed
        .AddInternalCall(ASMChampion::SelectChampionColor)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((char*)championColorSelectAddr + 0x169);

	sASMPatcher.PatchAt(championColorSelectAddr, &championColorSelectPatch);
}