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

extern const int opensearchkey;
extern bool issearching;
extern unordered_map<int, int> lastKeyPressTimeMap;
extern int minmods;


#endif