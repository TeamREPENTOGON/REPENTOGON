#ifndef STAGESSTUFF_H
#define STAGESSTUFF_H

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>
#include <Vector>

#include <lua.hpp>
#include "LuaCore.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "XMLData.h"
#include "CompletionTracker.h"

using namespace std;
extern void SetCurrentFloorMusic(int etype);
extern tuple<int, int> GetSetStage(int stageid, bool secondfloor);

inline unordered_map<int, tuple<int, int> > stageidtotuple;

// NOTE: StageType 3 is unused because it's deprecated (StageType.STAGETYPE_GREEDMODE)
inline void initstagetotuple() {
	//Vanilla
	stageidtotuple[1] = { 1,0 }; // Basement
	stageidtotuple[2] = { 1,1 }; // Cellar
	stageidtotuple[3] = { 1,2 }; // Burning Basement
	stageidtotuple[4] = { 3,0 }; // Caves
	stageidtotuple[5] = { 3,1 }; // Catacombs
	stageidtotuple[6] = { 3,2 }; // Flooded Caves
	stageidtotuple[7] = { 5,0 }; // Depths
	stageidtotuple[8] = { 5,1 }; // Necropolis
	stageidtotuple[9] = { 5,2 }; // Dank Depths
	stageidtotuple[10] = { 7,0 }; // Womb
	stageidtotuple[11] = { 7,1 }; // Utero
	stageidtotuple[12] = { 7,2 }; // Scarred Womb
	stageidtotuple[13] = { 9,0 }; // Blue Womb
	stageidtotuple[14] = { 10,0 }; // Sheol
	stageidtotuple[15] = { 10,1 }; // Cathedral
	stageidtotuple[16] = { 11,0 }; // Dark Room
	stageidtotuple[17] = { 11,1 }; // Chest
	stageidtotuple[26] = { 12,0 }; // Void
	//Vanilla
	//Greed
	//stageidtotuple[24] = { 6,0 };
	//stageidtotuple[25] = { 7,0 };
	//Greed
	//Repentance
	stageidtotuple[27] = { 1,4 }; // Downpour
	stageidtotuple[28] = { 1,5 }; // Dross
	stageidtotuple[29] = { 3,4 }; // Mines
	stageidtotuple[30] = { 3,5 }; // Ashpit
	stageidtotuple[31] = { 5,4 }; // Mausoleum
	stageidtotuple[32] = { 5,5 }; // Gehenna
	stageidtotuple[33] = { 7,4 }; // Corpse
	stageidtotuple[34] = { 7,5 }; // Mortis
	stageidtotuple[35] = { 13,0 }; // Home
	//Repentance
}

#endif