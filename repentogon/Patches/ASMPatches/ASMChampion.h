#include <map>
#include <vector>
#include "IsaacRepentance.h"
namespace ASMChampion {
	/*
		Some champions may only appear in certain points of the game. Furthermore,
		the odds of champions change depending on the player's LevelStage.
	*/
	enum class ChampionTier {
		/** LevelStage 1 - 2 **/
		EARLY_GAME = 0,

		/** LevelStage 3 - 4 **/
		MID_GAME = 1,

		/** LevelStage 5 - 6 **/
		LATE_GAME = 2,
	};

	// <ChampionColor, weight>
    std::map<ChampionTier, std::vector<std::pair<eChampionColor, float>>> champion_chances = {
        {
            ChampionTier::EARLY_GAME, {
            {eChampionColor::CHAMPION_ORANGE, 0.05f},
            {eChampionColor::CHAMPION_BLUE, 0.05f},
            {eChampionColor::CHAMPION_DARK_RED, 0.05f},
            {eChampionColor::CHAMPION_CAMO, 0.0375f},
            {eChampionColor::CHAMPION_PULSE_GREEN, 0.0375f},
            {eChampionColor::CHAMPION_PULSE_GREY, 0.0375f},
            {eChampionColor::CHAMPION_FLY_PROTECTED, 0.0375f},
            {eChampionColor::CHAMPION_RED, 0.1167f},
            {eChampionColor::CHAMPION_GREEN, 0.1167f},
            {eChampionColor::CHAMPION_BLACK, 0.1167f},
            {eChampionColor::CHAMPION_GREY, 0.1167f},
            {eChampionColor::CHAMPION_PINK, 0.1167f},
            {eChampionColor::CHAMPION_LIGHT_BLUE, 0.1167f},
        }
    },
    {
        ChampionTier::MID_GAME, {
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
        }
    },    
    {
        ChampionTier::LATE_GAME, {
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
    }
    };

	void ASMPatchChampionInitialization();

};