
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>

#include <lua.hpp>
#include "LuaCore.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "XMLData.h"
using namespace std;

extern XMLData XMLStuff;

typedef enum CompletionType {
	MOMS_HEART = 0,
	ISAAC = 1,
	SATAN = 2,
	BOSS_RUSH = 3,
	BLUE_BABY = 4,
	LAMB = 5,
	MEGA_SATAN = 6,
	ULTRA_GREED = 7,
	//scrappedmark ?
	HUSH = 9,
	//scrappedmark ?
	ULTRA_GREEDIER = 11,
	DELIRIUM = 12,
	MOTHER = 13,
	BEAST = 14,
};
unordered_map <int, int> CompletionTypeRender;
bool initializedrendercmpl = false;
void InitMarkRenderTypes() {
	initializedrendercmpl = true;
	CompletionTypeRender[1] = MOMS_HEART;
	CompletionTypeRender[2] = ISAAC;
	CompletionTypeRender[3] = SATAN;
	CompletionTypeRender[4] = BOSS_RUSH;
	CompletionTypeRender[5] = BLUE_BABY;
	CompletionTypeRender[6] = LAMB;
	CompletionTypeRender[7] = MEGA_SATAN;
	CompletionTypeRender[8] = ULTRA_GREED; //this one is also greedier tho
	CompletionTypeRender[9] = HUSH;
	CompletionTypeRender[0] = DELIRIUM;
	CompletionTypeRender[10] = MOTHER;
	CompletionTypeRender[11] = BEAST;
}

unordered_map<string, int> reversemarksenum;
void initreversenum() {
	reversemarksenum["MOMS_HEART"] = 0;
	reversemarksenum["ISAAC"] = 1;
	reversemarksenum["SATAN"] = 2;
	reversemarksenum["BOSS_RUSH"] = 3;
	reversemarksenum["BLUE_BABY"] = 4;
	reversemarksenum["LAMB"] = 5;
	reversemarksenum["MEGA_SATAN"] = 6;
	reversemarksenum["ULTRA_GREED"] = 7;
	reversemarksenum["HUSH"] = 9;
	reversemarksenum["ULTRA_GREEDIER"] = 11;
	reversemarksenum["DELIRIUM"] = 12;
	reversemarksenum["MOTHER"] = 13;
	reversemarksenum["BEAST"] = 14;
	reversemarksenum["MomsHeart"] = 0;
	reversemarksenum["Isaac"] = 1;
	reversemarksenum["Satan"] = 2;
	reversemarksenum["BossRush"] = 3;
	reversemarksenum["BlueBaby"] = 4;
	reversemarksenum["???"] = 4;
	reversemarksenum["????"] = 4;
	reversemarksenum["Lamb"] = 5;
	reversemarksenum["MegaSatan"] = 6;
	reversemarksenum["UltraGreed"] = 7;
	reversemarksenum["Greed"] = 7;
	reversemarksenum["GREED"] = 7;
	reversemarksenum["Hush"] = 9;
	reversemarksenum["UltraGreedier"] = 11;
	reversemarksenum["GREEDIER"] = 11;
	reversemarksenum["Greedier"] = 11;
	reversemarksenum["Delirium"] = 12;
	reversemarksenum["Mother"] = 13;
	reversemarksenum["Beast"] = 14;
}




typedef enum MEventCounter {
	PROGRESSION_KILL_MOMS_HEART_WITH_ISAAC = 27, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_MAGDALENE = 28, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_CAIN = 29, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_JUDAS = 30, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_BLUE_BABY = 31, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_EVE = 32, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_SAMSON = 33, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_AZAZEL = 34, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_LAZARUS = 35, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_EDEN = 36, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_THE_LOST = 37, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_LILITH = 38, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_KEEPER = 39, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_APOLLYON = 40, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_ISAAC = 41, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_MAGDALENE = 42, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_CAIN = 43, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_JUDAS = 44, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_BLUE_BABY = 45, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_EVE = 46, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_SAMSON = 47, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_AZAZEL = 48, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_LAZARUS = 49, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_EDEN = 50, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_THE_LOST = 51, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_LILITH = 52, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_KEEPER = 53, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_APOLLYON = 54, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_ISAAC = 55, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_MAGDALENE = 56, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_CAIN = 57, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_JUDAS = 58, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_BLUE_BABY = 59, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_EVE = 60, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_SAMSON = 61, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_AZAZEL = 62, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_LAZARUS = 63, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_EDEN = 64, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_THE_LOST = 65, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_LILITH = 66, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_KEEPER = 67, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_APOLLYON = 68, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_ISAAC = 69, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_MAGDALENE = 70, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_CAIN = 71, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_JUDAS = 72, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_BLUE_BABY = 73, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_EVE = 74, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_SAMSON = 75, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_AZAZEL = 76, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_LAZARUS = 77, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_EDEN = 78, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_THE_LOST = 79, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_LILITH = 80, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_KEEPER = 81, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_APOLLYON = 82, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_ISAAC = 83, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_MAGDALENE = 84, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_CAIN = 85, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_JUDAS = 86, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_BLUE_BABY = 87, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_EVE = 88, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_SAMSON = 89, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_AZAZEL = 90, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_LAZARUS = 91, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_EDEN = 92, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_THE_LOST = 93, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_LILITH = 94, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_KEEPER = 95, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_APOLLYON = 96, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_ISAAC = 97, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_MAGDALENE = 98, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_CAIN = 99, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_JUDAS = 100, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_BLUE_BABY = 101, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_EVE = 102, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_SAMSON = 103, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_AZAZEL = 104, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_LAZARUS = 105, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_EDEN = 106, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_THE_LOST = 107, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_LILITH = 108, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_KEEPER = 109, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_APOLLYON = 110, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_ISAAC = 116, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_MAGDALENE = 117, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_CAIN = 118, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_JUDAS = 119, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_BLUE_BABY = 120, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_EVE = 121, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_SAMSON = 122, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_AZAZEL = 123, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_LAZARUS = 124, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_EDEN = 125, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_THE_LOST = 126, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_LILITH = 127, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_KEEPER = 128, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_APOLLYON = 129, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_ISAAC = 130, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_MAGDALENE = 131, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_CAIN = 132, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_JUDAS = 133, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_BLUE_BABY = 134, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_EVE = 135, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_SAMSON = 136, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_AZAZEL = 137, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_LAZARUS = 138, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_EDEN = 139, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_THE_LOST = 140, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_LILITH = 141, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_KEEPER = 142, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_GREED_MODE_CLEARED_WITH_APOLLYON = 143, //(value = 0, 1 for greed mode, 2 for greedier)
	PROGRESSION_KILL_HUSH_WITH_ISAAC = 144, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_MAGDALENE = 145, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_CAIN = 146, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_JUDAS = 147, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_BLUE_BABY = 148, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_EVE = 149, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_SAMSON = 150, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_AZAZEL = 151, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_LAZARUS = 152, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_EDEN = 153, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_THE_LOST = 154, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_LILITH = 155, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_KEEPER = 156, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_APOLLYON = 157, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_ISAAC = 173, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_MAGDALENE = 174, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_CAIN = 175, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_JUDAS = 176, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_BLUE_BABY = 177, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_EVE = 178, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_SAMSON = 179, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_AZAZEL = 180, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_LAZARUS = 181, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_EDEN = 182, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_THE_LOST = 183, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_LILITH = 184, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_KEEPER = 185, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_APOLLYON = 186, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_THE_FORGOTTEN = 203, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_THE_FORGOTTEN = 204, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_THE_FORGOTTEN = 205, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_THE_FORGOTTEN = 206, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_THE_FORGOTTEN = 207, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_LAMB_WITH_THE_FORGOTTEN = 208, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_THE_FORGOTTEN = 209, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_THE_FORGOTTEN = 210, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_THE_FORGOTTEN = 211, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_THE_FORGOTTEN = 213, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_BETHANY = 214, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_JACOB_AND_ESAU = 215, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_ISAAC = 216, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_MAGDALENE = 217, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_CAIN = 218, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_JUDAS = 219, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_BLUE_BABY = 220, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_EVE = 221, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_SAMSON = 222, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_AZAZEL = 223, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_LAZARUS = 224, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_EDEN = 225, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_THE_LOST = 226, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_LILITH = 227, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_KEEPER = 228, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_APOLLYON = 229, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_THE_FORGOTTEN = 230, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_BETHANY = 231, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOMS_HEART_WITH_T_JACOB_AND_ESAU = 232, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_BETHANY = 233, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_JACOB_AND_ESAU = 234, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_ISAAC = 235, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_MAGDALENE = 236, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_CAIN = 237, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_JUDAS = 238, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_BLUE_BABY = 239, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_EVE = 240, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_SAMSON = 241, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_AZAZEL = 242, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_LAZARUS = 243, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_EDEN = 244, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_THE_LOST = 245, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_LILITH = 246, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_KEEPER = 247, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_APOLLYON = 248, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_THE_FORGOTTEN = 249, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_BETHANY = 250, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_ISAAC_WITH_T_JACOB_AND_ESAU = 251, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_BETHANY = 252, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_JACOB_AND_ESAU = 253, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_ISAAC = 254, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_MAGDALENE = 255, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_CAIN = 256, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_JUDAS = 257, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_BLUE_BABY = 258, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_EVE = 259, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_SAMSON = 260, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_AZAZEL = 261, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_LAZARUS = 262, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_EDEN = 263, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_THE_LOST = 264, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_LILITH = 265, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_KEEPER = 266, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_APOLLYON = 267, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_THE_FORGOTTEN = 268, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_BETHANY = 269, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_SATAN_WITH_T_JACOB_AND_ESAU = 270, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_BETHANY = 271, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_JACOB_AND_ESAU = 272, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_ISAAC = 273, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_MAGDALENE = 274, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_CAIN = 275, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_JUDAS = 276, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_BLUE_BABY = 277, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_EVE = 278, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_SAMSON = 279, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_AZAZEL = 280, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_LAZARUS = 281, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_EDEN = 282, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_THE_LOST = 283, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_LILITH = 284, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_KEEPER = 285, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_APOLLYON = 286, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_THE_FORGOTTEN = 287, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_BETHANY = 288, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_BOSSRUSH_CLEARED_WITH_T_JACOB_AND_ESAU = 289, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_BETHANY = 290, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_JACOB_AND_ESAU = 291, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_ISAAC = 292, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_MAGDALENE = 293, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_CAIN = 294, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_JUDAS = 295, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_BLUE_BABY = 296, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_EVE = 297, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_SAMSON = 298, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_AZAZEL = 299, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_LAZARUS = 300, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_EDEN = 301, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_THE_LOST = 302, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_LILITH = 303, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_KEEPER = 304, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_APOLLYON = 305, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_THE_FORGOTTEN = 306, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_BETHANY = 307, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BLUE_BABY_WITH_T_JACOB_AND_ESAU = 308, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_BETHANY = 309, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_JACOB_AND_ESAU = 310, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_ISAAC = 311, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_MAGDALENE = 312, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_CAIN = 313, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_JUDAS = 314, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_BLUE_BABY = 315, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_EVE = 316, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_SAMSON = 317, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_AZAZEL = 318, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_LAZARUS = 319, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_EDEN = 320, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_THE_LOST = 321, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_LILITH = 322, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_KEEPER = 323, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_APOLLYON = 324, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_THE_FORGOTTEN = 325, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_BETHANY = 326, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_THE_LAMB_WITH_T_JACOB_AND_ESAU = 327, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_BETHANY = 328, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_JACOB_AND_ESAU = 329, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_ISAAC = 330, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_MAGDALENE = 331, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_CAIN = 332, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_JUDAS = 333, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_BLUE_BABY = 334, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_EVE = 335, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_SAMSON = 336, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_AZAZEL = 337, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_LAZARUS = 338, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_EDEN = 339, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_THE_LOST = 340, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_LILITH = 341, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_KEEPER = 342, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_APOLLYON = 343, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_THE_FORGOTTEN = 344, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_BETHANY = 345, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MEGA_SATAN_WITH_T_JACOB_AND_ESAU = 346, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_BETHANY = 347, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_JACOB_AND_ESAU = 348, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_ISAAC = 349, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_MAGDALENE = 350, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_CAIN = 351, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_JUDAS = 352, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_BLUE_BABY = 353, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_EVE = 354, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_SAMSON = 355, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_AZAZEL = 356, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_LAZARUS = 357, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_EDEN = 358, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_LOST = 359, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_LILITH = 360, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_KEEPER = 361, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_APOLLYON = 362, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_FORGOTTEN = 363, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_BETHANY = 364, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_GREED_MODE_CLEARED_WITH_T_JACOB_AND_ESAU = 365, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_BETHANY = 366, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_JACOB_AND_ESAU = 367, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_ISAAC = 368, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_MAGDALENE = 369, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_CAIN = 370, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_JUDAS = 371, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_BLUE_BABY = 372, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_EVE = 373, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_SAMSON = 374, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_AZAZEL = 375, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_LAZARUS = 376, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_EDEN = 377, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_THE_LOST = 378, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_LILITH = 379, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_KEEPER = 380, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_APOLLYON = 381, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_THE_FORGOTTEN = 382, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_BETHANY = 383, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_HUSH_WITH_T_JACOB_AND_ESAU = 384, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_BETHANY = 404, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_JACOB_AND_ESAU = 405, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_ISAAC = 406, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_MAGDALENE = 407, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_CAIN = 408, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_JUDAS = 409, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_BLUE_BABY = 410, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_EVE = 411, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_SAMSON = 412, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_AZAZEL = 413, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_LAZARUS = 414, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_EDEN = 415, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_THE_LOST = 416, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_LILITH = 417, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_KEEPER = 418, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_APOLLYON = 419, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_THE_FORGOTTEN = 420, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_BETHANY = 421, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_DELIRIUM_WITH_T_JACOB_AND_ESAU = 422, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_ISAAC = 423, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_MAGDALENE = 424, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_CAIN = 425, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_JUDAS = 426, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_BLUE_BABY = 427, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_EVE = 428, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_SAMSON = 429, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_AZAZEL = 430, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_LAZARUS = 431, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_EDEN = 432, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_THE_LOST = 433, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_LILITH = 434, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_KEEPER = 435, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_APOLLYON = 436, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_THE_FORGOTTEN = 437, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_BETHANY = 438, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_JACOB_AND_ESAU = 439, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_ISAAC = 440, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_MAGDALENE = 441, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_CAIN = 442, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_JUDAS = 443, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_BLUE_BABY = 444, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_EVE = 445, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_SAMSON = 446, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_AZAZEL = 447, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_LAZARUS = 448, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_EDEN = 449, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_THE_LOST = 450, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_LILITH = 451, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_KEEPER = 452, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_APOLLYON = 453, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_THE_FORGOTTEN = 454, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_BETHANY = 455, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_MOTHER_WITH_T_JACOB_AND_ESAU = 456, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_ISAAC = 457, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_MAGDALENE = 458, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_CAIN = 459, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_JUDAS = 460, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_BLUE_BABY = 461, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_EVE = 462, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_SAMSON = 463, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_AZAZEL = 464, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_LAZARUS = 465, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_EDEN = 466, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_THE_LOST = 467, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_LILITH = 468, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_KEEPER = 469, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_APOLLYON = 470, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_THE_FORGOTTEN = 471, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_BETHANY = 472, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_JACOB_AND_ESAU = 473, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_ISAAC = 474, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_MAGDALENE = 475, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_CAIN = 476, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_JUDAS = 477, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_BLUE_BABY = 478, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_EVE = 479, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_SAMSON = 480, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_AZAZEL = 481, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_LAZARUS = 482, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_EDEN = 483, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_THE_LOST = 484, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_LILITH = 485, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_KEEPER = 486, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_APOLLYON = 487, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_THE_FORGOTTEN = 488, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_BETHANY = 489, //(0 for not done. 1 for normal difficulty, 2 for hard)
	PROGRESSION_KILL_BEAST_WITH_T_JACOB_AND_ESAU = 490, //(0 for not done. 1 for normal difficulty, 2 for hard)
};

unordered_map<int, unordered_map<int, int>> MarksToEvents;
void initmarkstoevents() {
	unordered_map<int, int> Isaac;
	Isaac[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_ISAAC;
	Isaac[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_ISAAC;
	Isaac[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_ISAAC;
	Isaac[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_ISAAC;
	Isaac[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_ISAAC;
	Isaac[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_ISAAC;
	Isaac[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_ISAAC;
	Isaac[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_ISAAC;
	Isaac[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_ISAAC;
	Isaac[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_ISAAC;
	Isaac[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_ISAAC;
	Isaac[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_ISAAC;
	Isaac[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_ISAAC;
	MarksToEvents[0] = Isaac;
	unordered_map<int, int> Maggy;
	Maggy[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_MAGDALENE;
	Maggy[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_MAGDALENE;
	Maggy[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_MAGDALENE;
	Maggy[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_MAGDALENE;
	Maggy[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_MAGDALENE;
	Maggy[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_MAGDALENE;
	Maggy[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_MAGDALENE;
	Maggy[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_MAGDALENE;
	Maggy[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_MAGDALENE;
	Maggy[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_MAGDALENE;
	Maggy[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_MAGDALENE;
	Maggy[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_MAGDALENE;
	Maggy[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_MAGDALENE;
	MarksToEvents[1] = Maggy;
	unordered_map<int, int> Cain;
	Cain[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_CAIN;
	Cain[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_CAIN;
	Cain[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_CAIN;
	Cain[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_CAIN;
	Cain[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_CAIN;
	Cain[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_CAIN;
	Cain[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_CAIN;
	Cain[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_CAIN;
	Cain[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_CAIN;
	Cain[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_CAIN;
	Cain[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_CAIN;
	Cain[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_CAIN;
	Cain[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_CAIN;
	MarksToEvents[2] = Cain;
	unordered_map<int, int> Judas;
	Judas[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_JUDAS;
	Judas[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_JUDAS;
	Judas[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_JUDAS;
	Judas[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_JUDAS;
	Judas[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_JUDAS;
	Judas[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_JUDAS;
	Judas[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_JUDAS;
	Judas[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_JUDAS;
	Judas[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_JUDAS;
	Judas[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_JUDAS;
	Judas[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_JUDAS;
	Judas[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_JUDAS;
	Judas[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_JUDAS;
	MarksToEvents[3] = Judas;
	unordered_map<int, int> BlueBaby;
	BlueBaby[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_BLUE_BABY;
	BlueBaby[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_BLUE_BABY;
	BlueBaby[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_BLUE_BABY;
	BlueBaby[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_BLUE_BABY;
	BlueBaby[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_BLUE_BABY;
	BlueBaby[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_BLUE_BABY;
	BlueBaby[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_BLUE_BABY;
	BlueBaby[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_BLUE_BABY;
	BlueBaby[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_BLUE_BABY;
	BlueBaby[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_BLUE_BABY;
	BlueBaby[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_BLUE_BABY;
	BlueBaby[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_BLUE_BABY;
	BlueBaby[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_BLUE_BABY;
	MarksToEvents[4] = BlueBaby;
	unordered_map<int, int> Eve;
	Eve[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_EVE;
	Eve[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_EVE;
	Eve[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_EVE;
	Eve[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_EVE;
	Eve[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_EVE;
	Eve[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_EVE;
	Eve[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_EVE;
	Eve[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_EVE;
	Eve[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_EVE;
	Eve[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_EVE;
	Eve[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_EVE;
	Eve[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_EVE;
	Eve[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_EVE;
	MarksToEvents[5] = Eve;
	unordered_map<int, int> Samson;
	Samson[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_SAMSON;
	Samson[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_SAMSON;
	Samson[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_SAMSON;
	Samson[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_SAMSON;
	Samson[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_SAMSON;
	Samson[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_SAMSON;
	Samson[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_SAMSON;
	Samson[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_SAMSON;
	Samson[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_SAMSON;
	Samson[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_SAMSON;
	Samson[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_SAMSON;
	Samson[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_SAMSON;
	Samson[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_SAMSON;
	MarksToEvents[6] = Samson;
	unordered_map<int, int> Azazel;
	Azazel[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_AZAZEL;
	Azazel[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_AZAZEL;
	Azazel[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_AZAZEL;
	Azazel[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_AZAZEL;
	Azazel[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_AZAZEL;
	Azazel[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_AZAZEL;
	Azazel[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_AZAZEL;
	Azazel[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_AZAZEL;
	Azazel[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_AZAZEL;
	Azazel[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_AZAZEL;
	Azazel[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_AZAZEL;
	Azazel[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_AZAZEL;
	Azazel[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_AZAZEL;
	MarksToEvents[7] = Azazel;
	unordered_map<int, int> Laz;
	Laz[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_LAZARUS;
	Laz[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_LAZARUS;
	Laz[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_LAZARUS;
	Laz[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_LAZARUS;
	Laz[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_LAZARUS;
	Laz[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_LAZARUS;
	Laz[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_LAZARUS;
	Laz[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_LAZARUS;
	Laz[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_LAZARUS;
	Laz[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_LAZARUS;
	Laz[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_LAZARUS;
	Laz[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_LAZARUS;
	Laz[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_LAZARUS;
	MarksToEvents[8] = Laz;
	unordered_map<int, int> Eden;
	Eden[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_EDEN;
	Eden[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_EDEN;
	Eden[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_EDEN;
	Eden[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_EDEN;
	Eden[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_EDEN;
	Eden[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_EDEN;
	Eden[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_EDEN;
	Eden[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_EDEN;
	Eden[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_EDEN;
	Eden[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_EDEN;
	Eden[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_EDEN;
	Eden[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_EDEN;
	Eden[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_EDEN;
	MarksToEvents[9] = Eden;
	unordered_map<int, int> Lost;
	Lost[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_THE_LOST;
	Lost[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_THE_LOST;
	Lost[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_THE_LOST;
	Lost[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_THE_LOST;
	Lost[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_THE_LOST;
	Lost[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_THE_LOST;
	Lost[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_THE_LOST;
	Lost[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_THE_LOST;
	Lost[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_THE_LOST;
	Lost[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_THE_LOST;
	Lost[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_THE_LOST;
	Lost[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_THE_LOST;
	Lost[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_THE_LOST;
	MarksToEvents[10] = Lost;
	MarksToEvents[11] = Laz; //Lazarus II
	MarksToEvents[12] = Judas; //Black Judas
	unordered_map<int, int> Lilith;
	Lilith[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_LILITH;
	Lilith[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_LILITH;
	Lilith[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_LILITH;
	Lilith[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_LILITH;
	Lilith[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_LILITH;
	Lilith[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_LILITH;
	Lilith[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_LILITH;
	Lilith[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_LILITH;
	Lilith[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_LILITH;
	Lilith[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_LILITH;
	Lilith[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_LILITH;
	Lilith[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_LILITH;
	Lilith[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_LILITH;
	MarksToEvents[13] = Lilith;
	unordered_map<int, int> Keeper;
	Keeper[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_KEEPER;
	Keeper[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_KEEPER;
	Keeper[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_KEEPER;
	Keeper[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_KEEPER;
	Keeper[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_KEEPER;
	Keeper[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_KEEPER;
	Keeper[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_KEEPER;
	Keeper[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_KEEPER;
	Keeper[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_KEEPER;
	Keeper[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_KEEPER;
	Keeper[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_KEEPER;
	Keeper[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_KEEPER;
	Keeper[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_KEEPER;
	MarksToEvents[14] = Keeper;
	unordered_map<int, int> Apoly;
	Apoly[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_APOLLYON;
	Apoly[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_APOLLYON;
	Apoly[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_APOLLYON;
	Apoly[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_APOLLYON;
	Apoly[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_APOLLYON;
	Apoly[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_APOLLYON;
	Apoly[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_APOLLYON;
	Apoly[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_APOLLYON;
	Apoly[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_APOLLYON;
	Apoly[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_APOLLYON;
	Apoly[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_APOLLYON;
	Apoly[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_APOLLYON;
	Apoly[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_APOLLYON;
	MarksToEvents[15] = Apoly;
	unordered_map<int, int> Boner;
	Boner[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_THE_FORGOTTEN;
	Boner[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_THE_FORGOTTEN;
	Boner[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_THE_FORGOTTEN;
	Boner[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_THE_FORGOTTEN;
	Boner[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_LAMB_WITH_THE_FORGOTTEN;
	Boner[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_THE_FORGOTTEN;
	Boner[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_THE_FORGOTTEN;
	Boner[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_THE_FORGOTTEN;
	Boner[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_THE_FORGOTTEN;
	Boner[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_THE_FORGOTTEN;
	Boner[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_THE_FORGOTTEN;
	Boner[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_THE_FORGOTTEN;
	Boner[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_THE_FORGOTTEN;
	MarksToEvents[16] = Boner;
	MarksToEvents[17] = Boner; //Soul
	unordered_map<int, int> Bethany;
	Bethany[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_BETHANY;
	Bethany[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_BETHANY;
	Bethany[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_BETHANY;
	Bethany[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_BETHANY;
	Bethany[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_BETHANY;
	Bethany[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_BETHANY;
	Bethany[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_BETHANY;
	Bethany[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_BETHANY;
	Bethany[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_BETHANY;
	Bethany[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_BETHANY;
	Bethany[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_BETHANY;
	Bethany[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_BETHANY;
	Bethany[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_BETHANY;
	MarksToEvents[18] = Bethany;
	unordered_map<int, int> Jacob;
	Jacob[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_JACOB_AND_ESAU;
	Jacob[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_JACOB_AND_ESAU;
	MarksToEvents[19] = Jacob;
	MarksToEvents[20] = Jacob; //Esau


	//////TAINTDS

	unordered_map<int, int> BISAAC;
	BISAAC[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_ISAAC;
	BISAAC[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_ISAAC;
	BISAAC[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_ISAAC;
	BISAAC[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_ISAAC;
	BISAAC[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_ISAAC;
	BISAAC[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_ISAAC;
	BISAAC[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_ISAAC;
	BISAAC[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_ISAAC;
	BISAAC[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_ISAAC;
	BISAAC[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_ISAAC;
	BISAAC[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_ISAAC;
	BISAAC[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_ISAAC;
	BISAAC[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_ISAAC;
	MarksToEvents[21] = BISAAC;
	unordered_map<int, int> BMaggy;
	BMaggy[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_MAGDALENE;
	BMaggy[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_MAGDALENE;
	BMaggy[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_MAGDALENE;
	BMaggy[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_MAGDALENE;
	BMaggy[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_MAGDALENE;
	BMaggy[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_MAGDALENE;
	BMaggy[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_MAGDALENE;
	BMaggy[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_MAGDALENE;
	BMaggy[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_MAGDALENE;
	BMaggy[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_MAGDALENE;
	BMaggy[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_MAGDALENE;
	BMaggy[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_MAGDALENE;
	BMaggy[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_MAGDALENE;
	MarksToEvents[22] = BMaggy;
	unordered_map<int, int> BCain;
	BCain[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_CAIN;
	BCain[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_CAIN;
	BCain[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_CAIN;
	BCain[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_CAIN;
	BCain[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_CAIN;
	BCain[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_CAIN;
	BCain[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_CAIN;
	BCain[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_CAIN;
	BCain[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_CAIN;
	BCain[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_CAIN;
	BCain[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_CAIN;
	BCain[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_CAIN;
	BCain[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_CAIN;
	MarksToEvents[23] = BCain;
	unordered_map<int, int> BJudas;
	BJudas[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_JUDAS;
	BJudas[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_JUDAS;
	BJudas[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_JUDAS;
	BJudas[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_JUDAS;
	BJudas[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_JUDAS;
	BJudas[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_JUDAS;
	BJudas[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_JUDAS;
	BJudas[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_JUDAS;
	BJudas[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_JUDAS;
	BJudas[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_JUDAS;
	BJudas[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_JUDAS;
	BJudas[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_JUDAS;
	BJudas[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_JUDAS;
	MarksToEvents[24] = BJudas;
	unordered_map<int, int> BBlueBaby;
	BBlueBaby[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_BLUE_BABY;
	BBlueBaby[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_BLUE_BABY;
	MarksToEvents[25] = BBlueBaby;
	unordered_map<int, int> BEve;
	BEve[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_EVE;
	BEve[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_EVE;
	BEve[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_EVE;
	BEve[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_EVE;
	BEve[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_EVE;
	BEve[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_EVE;
	BEve[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_EVE;
	BEve[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_EVE;
	BEve[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_EVE;
	BEve[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_EVE;
	BEve[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_EVE;
	BEve[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_EVE;
	BEve[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_EVE;
	MarksToEvents[26] = BEve;
	unordered_map<int, int> BSamson;
	BSamson[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_SAMSON;
	BSamson[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_SAMSON;
	BSamson[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_SAMSON;
	BSamson[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_SAMSON;
	BSamson[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_SAMSON;
	BSamson[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_SAMSON;
	BSamson[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_SAMSON;
	BSamson[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_SAMSON;
	BSamson[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_SAMSON;
	BSamson[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_SAMSON;
	BSamson[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_SAMSON;
	BSamson[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_SAMSON;
	BSamson[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_SAMSON;
	MarksToEvents[27] = BSamson;
	unordered_map<int, int> BAzazel;
	BAzazel[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_AZAZEL;
	BAzazel[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_AZAZEL;
	BAzazel[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_AZAZEL;
	BAzazel[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_AZAZEL;
	BAzazel[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_AZAZEL;
	BAzazel[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_AZAZEL;
	BAzazel[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_AZAZEL;
	BAzazel[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_AZAZEL;
	BAzazel[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_AZAZEL;
	BAzazel[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_AZAZEL;
	BAzazel[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_AZAZEL;
	BAzazel[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_AZAZEL;
	BAzazel[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_AZAZEL;
	MarksToEvents[28] = BAzazel;
	unordered_map<int, int> BLaz;
	BLaz[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_LAZARUS;
	BLaz[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_LAZARUS;
	BLaz[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_LAZARUS;
	BLaz[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_LAZARUS;
	BLaz[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_LAZARUS;
	BLaz[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_LAZARUS;
	BLaz[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_LAZARUS;
	BLaz[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_LAZARUS;
	BLaz[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_LAZARUS;
	BLaz[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_LAZARUS;
	BLaz[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_LAZARUS;
	BLaz[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_LAZARUS;
	BLaz[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_LAZARUS;
	MarksToEvents[29] = BLaz;
	unordered_map<int, int> BEden;
	BEden[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_EDEN;
	BEden[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_EDEN;
	BEden[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_EDEN;
	BEden[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_EDEN;
	BEden[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_EDEN;
	BEden[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_EDEN;
	BEden[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_EDEN;
	BEden[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_EDEN;
	BEden[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_EDEN;
	BEden[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_EDEN;
	BEden[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_EDEN;
	BEden[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_EDEN;
	BEden[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_EDEN;
	MarksToEvents[30] = BEden;
	unordered_map<int, int> BLost;
	BLost[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_THE_LOST;
	BLost[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_THE_LOST;
	BLost[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_THE_LOST;
	BLost[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_THE_LOST;
	BLost[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_THE_LOST;
	BLost[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_THE_LOST;
	BLost[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_THE_LOST;
	BLost[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_THE_LOST;
	BLost[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_THE_LOST;
	BLost[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_THE_LOST;
	BLost[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_LOST;
	BLost[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_LOST;
	BLost[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_THE_LOST;
	MarksToEvents[31] = BLost;
	unordered_map<int, int> BLilith;
	BLilith[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_LILITH;
	BLilith[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_LILITH;
	BLilith[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_LILITH;
	BLilith[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_LILITH;
	BLilith[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_LILITH;
	BLilith[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_LILITH;
	BLilith[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_LILITH;
	BLilith[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_LILITH;
	BLilith[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_LILITH;
	BLilith[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_LILITH;
	BLilith[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_LILITH;
	BLilith[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_LILITH;
	BLilith[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_LILITH;
	MarksToEvents[32] = BLilith;
	unordered_map<int, int> BKeeper;
	BKeeper[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_KEEPER;
	BKeeper[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_KEEPER;
	BKeeper[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_KEEPER;
	BKeeper[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_KEEPER;
	BKeeper[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_KEEPER;
	BKeeper[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_KEEPER;
	BKeeper[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_KEEPER;
	BKeeper[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_KEEPER;
	BKeeper[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_KEEPER;
	BKeeper[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_KEEPER;
	BKeeper[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_KEEPER;
	BKeeper[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_KEEPER;
	BKeeper[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_KEEPER;
	MarksToEvents[33] = BKeeper;
	unordered_map<int, int> BApoly;
	BApoly[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_APOLLYON;
	BApoly[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_APOLLYON;
	BApoly[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_APOLLYON;
	BApoly[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_APOLLYON;
	BApoly[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_APOLLYON;
	BApoly[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_APOLLYON;
	BApoly[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_APOLLYON;
	BApoly[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_APOLLYON;
	BApoly[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_APOLLYON;
	BApoly[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_APOLLYON;
	BApoly[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_APOLLYON;
	BApoly[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_APOLLYON;
	BApoly[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_APOLLYON;
	MarksToEvents[34] = BApoly;
	unordered_map<int, int> BBoner;
	BBoner[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_THE_FORGOTTEN;
	BBoner[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_THE_FORGOTTEN;
	MarksToEvents[35] = BBoner;
	unordered_map<int, int> BBethany;
	BBethany[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_BETHANY;
	BBethany[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_BETHANY;
	BBethany[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_BETHANY;
	BBethany[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_BETHANY;
	BBethany[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_BETHANY;
	BBethany[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_BETHANY;
	BBethany[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_BETHANY;
	BBethany[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_BETHANY;
	BBethany[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_BETHANY;
	BBethany[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_BETHANY;
	BBethany[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_BETHANY;
	BBethany[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_BETHANY;
	BBethany[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_BETHANY;
	MarksToEvents[36] = BBethany;
	unordered_map<int, int> BT_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::MOMS_HEART] = MEventCounter::PROGRESSION_KILL_MOMS_HEART_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::ISAAC] = MEventCounter::PROGRESSION_KILL_ISAAC_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::SATAN] = MEventCounter::PROGRESSION_KILL_SATAN_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::BLUE_BABY] = MEventCounter::PROGRESSION_KILL_BLUE_BABY_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::LAMB] = MEventCounter::PROGRESSION_KILL_THE_LAMB_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::MEGA_SATAN] = MEventCounter::PROGRESSION_KILL_MEGA_SATAN_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::HUSH] = MEventCounter::PROGRESSION_KILL_HUSH_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::MOTHER] = MEventCounter::PROGRESSION_KILL_MOTHER_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::DELIRIUM] = MEventCounter::PROGRESSION_KILL_DELIRIUM_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::BEAST] = MEventCounter::PROGRESSION_KILL_BEAST_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::ULTRA_GREED] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::ULTRA_GREEDIER] = MEventCounter::PROGRESSION_GREED_MODE_CLEARED_WITH_T_JACOB_AND_ESAU;
	BT_JACOB_AND_ESAU[CompletionType::BOSS_RUSH] = MEventCounter::PROGRESSION_BOSSRUSH_CLEARED_WITH_T_JACOB_AND_ESAU;
	MarksToEvents[37] = BT_JACOB_AND_ESAU;
	MarksToEvents[38] = BLaz; 
	MarksToEvents[39] = BT_JACOB_AND_ESAU;
	MarksToEvents[40] = BBoner; 


}


unordered_map<string, std::array<int, 15> > CompletionMarks;
string jsonpath;

void SaveCompletionMarksToJson() {
	std::string directory = jsonpath.substr(0, jsonpath.find_last_of("\\/"));
	if (!CreateDirectory(directory.c_str(), NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			printf("[REPENTOGON] Error creating Repentogon Save directory \n");
			return ;
		}
	}

	rapidjson::Document doc;
	doc.SetObject();

	for (auto& kv : CompletionMarks) {
		rapidjson::Value array(rapidjson::kArrayType);
		for (int i = 0; i < 15; ++i) {
			array.PushBack(kv.second[i], doc.GetAllocator());
		}
		doc.AddMember(rapidjson::Value(kv.first.c_str(), kv.first.size(), doc.GetAllocator()),
			array, doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(jsonpath);
	ofs << buffer.GetString() << std::endl;
	printf("[REPENTOGON] Completion Marks saved to: %s \n", jsonpath.c_str());
}

void LoadCompletionMarksFromJson() {
	CompletionMarks.clear();
	ifstream ifs(jsonpath);
	if (!ifs.good()) {
		printf("[REPENTOGON] No marks for saveslot in: %s \n", jsonpath.c_str());
		return;
	}

	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	rapidjson::Document doc;
	doc.Parse(content.c_str());
	if (!doc.IsObject()) {
		return;
	}
	for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it) {
		string key = it->name.GetString();
		if (CompletionMarks.find(key) == CompletionMarks.end()) {
			CompletionMarks[key] = array<int, 15>{};
		}

		auto& value = it->value;
		if (!value.IsArray() || value.Size() != 15) {
			continue;
		}

		for (int i = 0; i < 15; ++i) {
			if (!value[i].IsInt()) {
				continue;
			}
			CompletionMarks[key][i] = value[i].GetInt();
		}
	}
	printf("[REPENTOGON] Completion Marks loaded from: %s \n", jsonpath.c_str());
}

int selectedchar = 0;
int ischartainted = false;
int hidemarks = false;

unordered_map<string, string> GetPlayerDataForMarks(int playerid) {
	unordered_map<string, string> playerdata = XMLStuff.PlayerData.players[playerid];
	if (playerdata.count("completionparent") > 0) {
		string aidx = playerdata["sourceid"] + "-" + playerdata["completionparent"];
		if (XMLStuff.PlayerData.playerbynamemod.count(aidx) == 0) {
			if (XMLStuff.PlayerData.playerbyname.count(playerdata["completionparent"]) > 0) {
				return XMLStuff.PlayerData.players[XMLStuff.PlayerData.playerbyname[playerdata["completionparent"]]];
			}
		}
		else {
			return XMLStuff.PlayerData.players[XMLStuff.PlayerData.playerbynamemod[aidx]];
		}
	}
	return playerdata;
}

string stolower(char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

string GetMarksIdx(int playerid) {
	unordered_map<string, string> playerdata = GetPlayerDataForMarks(playerid);
	string idx = playerdata["sourceid"] + "-" + playerdata["name"];
	ischartainted = false;
	if (playerdata.count("bskinparent") > 0) {
		idx = idx + "-Tainted-";
		ischartainted = true;
	}
	return idx;
}

array<int, 15> GetMarksForPlayer(int playerid,ANM2* anm = NULL) {
	array<int, 15> marks;
	if (XMLStuff.PlayerData.players.count(playerid) > 0) {
		unordered_map<string, string> playerdata = GetPlayerDataForMarks(playerid);
		string idx = GetMarksIdx(playerid);
		hidemarks = false;
		if (playerdata.count("nomarks") > 0) {
			if (strcmp(stolower((char *)playerdata["sourceid"].c_str()).c_str(), "false") == 0) {
				hidemarks = true;
			}
		}
		if (marks[CompletionType::ULTRA_GREED] == 2) {
			marks[CompletionType::ULTRA_GREEDIER] = 2;
		}
		if (marks[CompletionType::ULTRA_GREEDIER] > 0) {
			marks[CompletionType::ULTRA_GREED] = 2;
			marks[CompletionType::ULTRA_GREEDIER] = 2;
		}

		marks = CompletionMarks[idx];
		if (anm){
			if (ischartainted) {
				anm->SetLayerFrame(0, marks[CompletionTypeRender[0]] + 3);
			}
			else {
				anm->SetLayerFrame(0, marks[CompletionTypeRender[0]]);
			}
			for (int i = 1; i <= 11; i++) {
				if ((i == 8) && marks[ULTRA_GREEDIER]) {
					anm->SetLayerFrame(i, marks[CompletionTypeRender[i]]);
				}
				else {
					anm->SetLayerFrame(i, marks[CompletionTypeRender[i]]);
				}
			}

			anm->Update();
		}
		return marks;
	}
	return { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
}


HOOK_METHOD(Manager, LoadGameState, (int saveslot) -> void) {
	jsonpath = string((char*)&g_SaveDataPath) + "Repentogon/moddedcompletionmarks" + to_string(saveslot) +".json";

	LoadCompletionMarksFromJson();
	if (!initializedrendercmpl){
		InitMarkRenderTypes();
		initreversenum();
		initmarkstoevents();
	}
	super(saveslot);
}

HOOK_METHOD_PRIORITY(Manager, RecordPlayerCompletion,100, (int eEvent) -> void) {
	int numplayers = g_Game->GetNumPlayers();
	for(int i = 0;i<numplayers;i++){
		int playertype = g_Game->GetPlayer(i)->GetPlayerType();
		string idx = GetMarksIdx(playertype);
		if (CompletionMarks.count(idx) == 0) {
			CompletionMarks[idx] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		}
		int difficulty = g_Game->GetDifficulty();
		int marktype = 1;
		if ((difficulty == 1) || (difficulty == 3)) { //1:hard 3:greedier
			marktype = 2;
		}
		CompletionMarks[idx][eEvent] = marktype;
	}
	SaveCompletionMarksToJson();
	super(eEvent);
}

HOOK_METHOD(PauseScreen, Render, () -> void) {
	super();
	int playertype = g_Game->GetPlayer(0)->GetPlayerType();
	if ((playertype > 40) && (this->status !=2)) {
		NullFrame* nul = this->GetANM2()->GetNullFrame("CompletionWidget"); 
		Vector* widgtpos = nul->GetPos();
		Vector* widgtscale = nul->GetScale();
		CompletionWidget* cmpl = this->GetCompletionWidget();

		ANM2* anm = cmpl->GetANM2();

		array marks = GetMarksForPlayer(playertype,anm);
		if (!hidemarks){
			cmpl->CharacterId = playertype;
			cmpl->Render(new Vector((g_WIDTH * 0.6) + widgtpos->x, (g_HEIGHT * 0.5) +widgtpos->y), widgtscale);
		}
	}
}


HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	selectedchar = CharacterId;
	super(CharacterId, RenderPos, DefaultSprite);
}
HOOK_METHOD(Menu_Character, Render, () -> void) {
	super();
	CompletionWidget* cmpl = this->GetCompletionWidget();
	if(this->charaslot > 17){
		
		Vector* ref = (Vector *)(g_MenuManager + 60);
		Vector* cpos = new Vector(ref->x - 80, ref->y + 894);
		ANM2* anm = cmpl->GetANM2();
		
		array marks = GetMarksForPlayer(selectedchar,anm);
		if (!hidemarks) {
			cmpl->CharacterId = selectedchar;
			cmpl->Render(new Vector(ref->x + 80, ref->y + 860), new Vector(1, 1));
		}

	}
}





int Lua_IsaacSetCharacterMarks(lua_State* L)
{
	int playertype = 0;
	int length = 0;
	array<int, 15> marks = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	if (lua_istable(L, -1)) { 
		lua_pushnil(L); 
		while (lua_next(L, -2) != 0) { //need to use lua_next because normal method wont work with string indexes
			if (lua_isstring(L, -2)) { 
				const char* key = lua_tostring(L, -2); 
				int value = lua_tointeger(L, -1);
				if (reversemarksenum.count(key) > 0) {
					if ((value < 0) || (value > 2)) {
						return luaL_error(L, "Invalid Completion Marks value for %s is invalid(%d)",key, value);
					}
					marks[reversemarksenum[key]] = value;
				}else if(strcmp(key, "PlayerType") == 0) {
					playertype = value;
				}
				else {
					return luaL_error(L, "Invalid Completion Marks table index: %s",key);
				}
			}
			lua_pop(L, 1); 
			length++;
		}
	}
	else {
		return luaL_error(L, "Expected table as parameter #2, got %s", lua_typename(L, lua_type(L, 1)));
	}
	if (playertype < 0) {
		return luaL_error(L, "Invalid Player Type");
	}
	string idx = GetMarksIdx(playertype);
	if (marks[CompletionType::ULTRA_GREED] == 2) {
		marks[CompletionType::ULTRA_GREEDIER] = 2;
	}
	if (marks[CompletionType::ULTRA_GREEDIER] > 0) {
		marks[CompletionType::ULTRA_GREED] = 2;
		marks[CompletionType::ULTRA_GREEDIER] = 2;
	}
	if (playertype> 40){
		CompletionMarks[idx] = marks;
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MOMS_HEART], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MOMS_HEART]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::SATAN], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::SATAN]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MEGA_SATAN], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MEGA_SATAN]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::HUSH], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::HUSH]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::ISAAC], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::ISAAC]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BLUE_BABY], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BLUE_BABY]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MOTHER], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MOTHER]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::DELIRIUM], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::DELIRIUM]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BEAST], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BEAST]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::ULTRA_GREED],-PData->GetEventCounter(MarksToEvents[playertype][CompletionType::ULTRA_GREED]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BOSS_RUSH], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BOSS_RUSH]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::LAMB], -PData->GetEventCounter(MarksToEvents[playertype][CompletionType::LAMB]));

		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MOMS_HEART], marks[CompletionType::MOMS_HEART]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::SATAN], marks[CompletionType::SATAN]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MEGA_SATAN], marks[CompletionType::MEGA_SATAN]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::HUSH], marks[CompletionType::HUSH]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::ISAAC], marks[CompletionType::ISAAC]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BLUE_BABY], marks[CompletionType::BLUE_BABY]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::MOTHER], marks[CompletionType::MOTHER]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::DELIRIUM], marks[CompletionType::DELIRIUM]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BEAST], marks[CompletionType::BEAST]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::ULTRA_GREED], marks[CompletionType::ULTRA_GREED]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::BOSS_RUSH], marks[CompletionType::BOSS_RUSH]);
		PData->IncreaseEventCounter(MarksToEvents[playertype][CompletionType::LAMB], marks[CompletionType::LAMB]);
	}
	return 1;
}

int Lua_IsaacGetCharacterMark(lua_State* L)
{
	int completiontype = luaL_checkinteger(L, 2);
	int playertype = luaL_checkinteger(L, 1);
	if (playertype > 40) {
		array<int, 15> marks = GetMarksForPlayer(playertype);
		lua_pushnumber(L, marks[completiontype]);
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][completiontype]));
	}
	return 1;
}

int Lua_IsaacClearCompletionMarks(lua_State* L)
{
	int playertype = luaL_checkinteger(L, 1);
	if (playertype > 40) {
		string idx = GetMarksIdx(playertype);
		CompletionMarks[idx] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 15; i++) {
			if ((i != 8) && (i != 10) && (i != 11)) {
					PData->IncreaseEventCounter(MarksToEvents[playertype][i], -PData->GetEventCounter(MarksToEvents[playertype][i]));
			}
		}
	}
	return 1;
}


int Lua_IsaacFillCompletionMarks(lua_State* L)
{
	int playertype = luaL_checkinteger(L, 1);
	int cmpldif = 2;
	if (playertype > 40) {
		string idx = GetMarksIdx(playertype);
		CompletionMarks[idx] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }; 
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 15; i++) {
			if ((i != 8) && (i != 10) && (i != 11)) {
				if (PData->GetEventCounter(MarksToEvents[playertype][i]) < cmpldif) {
					PData->IncreaseEventCounter(MarksToEvents[playertype][i], 2);
				}
			}
		}
	}
	lua_pushnumber(L, cmpldif);
	return 1;
}


int Lua_IsaacGetFullCompletion(lua_State* L)
{
	int playertype = luaL_checkinteger(L, 1);
	int cmpldif = 2;
	if (playertype > 40) {
		array<int, 15> marks = GetMarksForPlayer(playertype);
		for (int i = 0; i < 15; i++) {
			if (marks[i] < cmpldif) {
				if ((i != 8) && (i != 10)) {
					cmpldif = marks[i];
				}
			}
		}
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 15; i++) {
			if ((i != 8) && (i != 10) && (i != 11)) {
				if (PData->GetEventCounter(MarksToEvents[playertype][i]) < cmpldif) {
					cmpldif = PData->GetEventCounter(MarksToEvents[playertype][i]);
				}
			}
		}
	}
	lua_pushnumber(L, cmpldif);
	return 1;
}

int Lua_IsaacSetCharacterMark(lua_State* L)
{
	int completiontype = luaL_checkinteger(L, 2);
	int playertype = luaL_checkinteger(L, 1);
	int value = luaL_checkinteger(L, 3);
	if ((value < 0) || (value > 2)) {
		return luaL_error(L, "Invalid Completion Marks value!(%d)", value);
	}
	if (playertype > 40){
		array<int, 15> marks = GetMarksForPlayer(playertype);
		marks[completiontype] = value;
		string idx = GetMarksIdx(playertype);
		CompletionMarks[idx] = marks;
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		PData->IncreaseEventCounter(MarksToEvents[playertype][completiontype], -PData->GetEventCounter(MarksToEvents[playertype][completiontype]));
		PData->IncreaseEventCounter(MarksToEvents[playertype][completiontype],value);
	}
	return 1;
}


int Lua_IsaacGetCharacterMarks(lua_State* L)
{
	int playertype = luaL_checkinteger(L, 1);
	if (playertype > 40) {
		array<int, 15> marks = GetMarksForPlayer(playertype);
		lua_newtable(L, 0);
		lua_pushstring(L, "PlayerType");
		lua_pushnumber(L, playertype);
		lua_settable(L, -3);
		lua_pushstring(L, "MomsHeart");
		lua_pushnumber(L, marks[0]);
		lua_settable(L, -3);
		lua_pushstring(L, "Isaac");
		lua_pushnumber(L, marks[1]);
		lua_settable(L, -3);
		lua_pushstring(L, "Satan");
		lua_pushnumber(L, marks[2]);
		lua_settable(L, -3);
		lua_pushstring(L, "BossRush");
		lua_pushnumber(L, marks[3]);
		lua_settable(L, -3);
		lua_pushstring(L, "BlueBaby");
		lua_pushnumber(L, marks[4]);
		lua_settable(L, -3);
		lua_pushstring(L, "Lamb");
		lua_pushnumber(L, marks[5]);
		lua_settable(L, -3);
		lua_pushstring(L, "MegaSatan");
		lua_pushnumber(L, marks[6]);
		lua_settable(L, -3);
		lua_pushstring(L, "UltraGreed");
		lua_pushnumber(L, marks[7]);
		lua_settable(L, -3);
		lua_pushstring(L, "Hush");
		lua_pushnumber(L, marks[9]);
		lua_settable(L, -3);
		lua_pushstring(L, "UltraGreedier");
		lua_pushnumber(L, marks[11]);
		lua_settable(L, -3);
		lua_pushstring(L, "Delirium");
		lua_pushnumber(L, marks[12]);
		lua_settable(L, -3);
		lua_pushstring(L, "Mother");
		lua_pushnumber(L, marks[13]);
		lua_settable(L, -3);
		lua_pushstring(L, "Beast");
		lua_pushnumber(L, marks[14]);
		lua_settable(L, -3);
		
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		array<int, 15> marks = GetMarksForPlayer(playertype);
		lua_newtable(L, 0);
		lua_pushstring(L, "PlayerType");
		lua_pushnumber(L, playertype);
		lua_settable(L, -3);
		lua_pushstring(L, "MomsHeart");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MOMS_HEART]));
		lua_settable(L, -3);
		lua_pushstring(L, "Isaac");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::ISAAC]));
		lua_settable(L, -3);
		lua_pushstring(L, "Satan");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::SATAN]));
		lua_settable(L, -3);
		lua_pushstring(L, "BossRush");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BOSS_RUSH]));
		lua_settable(L, -3);
		lua_pushstring(L, "BlueBaby");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BLUE_BABY]));
		lua_settable(L, -3);
		lua_pushstring(L, "Lamb");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::LAMB]));
		lua_settable(L, -3);
		lua_pushstring(L, "MegaSatan");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MEGA_SATAN]));
		lua_settable(L, -3);
		lua_pushstring(L, "UltraGreed");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::ULTRA_GREED]));
		lua_settable(L, -3);
		lua_pushstring(L, "Hush");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::HUSH]));
		lua_settable(L, -3);
		lua_pushstring(L, "UltraGreedier");
		lua_pushnumber(L, 0);
		lua_settable(L, -3);
		lua_pushstring(L, "Delirium");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::DELIRIUM]));
		lua_settable(L, -3);
		lua_pushstring(L, "Mother");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::MOTHER]));
		lua_settable(L, -3);
		lua_pushstring(L, "Beast");
		lua_pushnumber(L, PData->GetEventCounter(MarksToEvents[playertype][CompletionType::BEAST]));
		lua_settable(L, -3);
	
	}
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "GetCompletionMarks");
	lua_pushcfunction(state, Lua_IsaacGetCharacterMarks);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "GetCompletionMark");
	lua_pushcfunction(state, Lua_IsaacGetCharacterMark);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "SetCompletionMarks");
	lua_pushcfunction(state, Lua_IsaacSetCharacterMarks);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "SetCompletionMark");
	lua_pushcfunction(state, Lua_IsaacSetCharacterMark);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "AllMarksFilled");
	lua_pushcfunction(state, Lua_IsaacGetFullCompletion);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "FillCompletionMarks");
	lua_pushcfunction(state, Lua_IsaacFillCompletionMarks);
	lua_rawset(state, -3);
	lua_pop(state, 1);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "ClearCompletionMarks");
	lua_pushcfunction(state, Lua_IsaacClearCompletionMarks);
	lua_rawset(state, -3);
	lua_pop(state, 1);
	

}