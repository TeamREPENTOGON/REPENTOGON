#pragma once

void ASMPatchBlueWombCurse();
void ASMPatchVoidGeneration();
void PatchSpecialQuest();
void ASMPatchDealRoomVariants();
void PatchOverrideDataHandling();
void PatchLevelGeneratorTryResizeEndroom();

const int stageidToAchievement[36] = {
	-1, // Special Rooms
	-1, // Basement
	86, // Cellar
	342, // Burning Basement
	-1, // Caves
	87, // Catacombs
	343, // Flooded Caves
	-1, // Depths
	88, // Necropolis
	344, // Dank Depths
	4, // Womb
	4, // Utero
	345, // Scarred Womb
	234, // Blue Womb
	32, // Sheol (this is for ??? character unlock but it works)
	32, // Cathedral
	78, // Dark Room (this is for the Negative)
	57, // Chest (this is for the Polaroid)
	-1, // Unused AB Greed stageids
	-1,
	-1,
	-1,
	-1,
	-1,
	-1, // The Shop (floor)
	-1, // Ultra Greed (floor)
	//320, // The Void
	-1, // The Void - changed to always be unlocked so Void rooms always show up
	407, // Downpour (this is A Secret Exit)
	412, // Dross
	407, // Mines
	413, // Ashpit
	407, // Mausoleum
	414, // Gehenna
	411, // Corpse (this is Rotten Hearts)
	-1, // Mortis
	415, // Home (this is Red Key)
};