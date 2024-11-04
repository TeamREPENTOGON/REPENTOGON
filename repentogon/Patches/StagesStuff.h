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
inline void initstagetotuple() {
	//Vanilla
	stageidtotuple[1] = { 1,0 };
	stageidtotuple[2] = { 1,1 };
	stageidtotuple[3] = { 1,2 };
	stageidtotuple[4] = { 3,0 };
	stageidtotuple[5] = { 3,1 };
	stageidtotuple[6] = { 1,2 };
	stageidtotuple[7] = { 5,0 };
	stageidtotuple[8] = { 5,1 };
	stageidtotuple[9] = { 5,2 };
	stageidtotuple[10] = { 7,0 };
	stageidtotuple[11] = { 7,1 };
	stageidtotuple[12] = { 7,2 };
	stageidtotuple[13] = { 9,0 };
	stageidtotuple[14] = { 10,0 };
	stageidtotuple[15] = { 10,1 };
	stageidtotuple[16] = { 11,0 };
	stageidtotuple[17] = { 11,1 };
	stageidtotuple[26] = { 12,0 };
	//Vanilla
	//Greed
	//stageidtotuple[24] = { 6,0 };
	//stageidtotuple[25] = { 7,0 };
	//Greed
	//Repentance
	stageidtotuple[27] = { 1,4 };
	stageidtotuple[28] = { 1,5 };
	stageidtotuple[29] = { 3,4 };
	stageidtotuple[30] = { 3,5 };
	stageidtotuple[31] = { 5,4 };
	stageidtotuple[32] = { 5,5 };
	stageidtotuple[33] = { 7,4 };
	stageidtotuple[35] = { 13,0 };
	//Repentance
}

#endif