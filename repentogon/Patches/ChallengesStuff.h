#ifndef ACHIEVSTUFF_H
#define ACHIEVSTUFF_H

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

extern unordered_map<string, int > Challenges; //id - times beaten
extern string chaljsonpath;

inline void InitChals() {
	Challenges.clear();
}

extern void UndoChallenge(int challengeid);



#endif