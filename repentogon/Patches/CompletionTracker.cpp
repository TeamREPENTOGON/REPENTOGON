
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>

#include <lua.hpp>
#include "LuaCore.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "XMLData.h"
#include "AchievementsStuff.h"
#include "JsonSavedata.h"

#include "../ImGuiFeatures/LogViewer.h"
#include "../MiscFunctions.h"


using namespace std;

extern XMLData XMLStuff;
extern std::bitset<500> CallbackState;

bool debugmode = false;

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





unordered_map<int, unordered_map<int, int>> MarksToEvents;
unordered_map<int, int> EventsToPlayerType;
unordered_map<int, int> EventsToCompletionType;

void initEventsToPlayerType() {
	for (int i = 0; i <= 40; i++) {
		unordered_map<int, int> chara = MarksToEvents[i];
		for (auto& kv : chara) {
			EventsToPlayerType[kv.second] = i;
			EventsToCompletionType[kv.second] = kv.first;
		}
	}

}

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

	initEventsToPlayerType();
}




unordered_map<string, std::array<int, 15> > CompletionMarks;
string jsonpath;

void SaveCompletionMarksToJson() {
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { return ; }
	rapidjson::Document doc;
	doc.SetObject();

	//SavingMarks
	ArrayOfArrayToJson(&doc, "CompletionMarks", CompletionMarks);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(jsonpath);
	ofs << buffer.GetString() << std::endl;
	logViewer.AddLog("[REPENTOGON]", "Completion Marks saved to : % s \n", jsonpath.c_str());
}

void LoadCompletionMarksFromJson() {
	CompletionMarks.clear();
	rapidjson::Document doc = GetJsonDoc(&jsonpath);
	if (!doc.IsObject()) {
		logViewer.AddLog("[REPENTOGON]", "No Completion Marks for saveslot in: %s \n", jsonpath.c_str());
		return;
	}
	JsonToArrayOfArray(doc["CompletionMarks"], CompletionMarks);

	logViewer.AddLog("[REPENTOGON]", "Completion Marks loaded from: %s \n", jsonpath.c_str());
}


bool PreMarksCallbackTrigger(int markid, int playertpe) {
	const int callbackid = 1047;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(playertpe)
			.push(markid)
			.push(playertpe)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return false;
				}
			}
		}
	}
	return true;
}
void PostMarksCallbackTrigger(int markid, int playertpe) {
	const int callbackid = 1048;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(playertpe)
			.push(markid)
			.push(playertpe)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

}




int selectedchar = 0;
int ischartainted = false;
int hidemarks = false;

XMLAttributes GetPlayerDataForMarks(int playerid) {
	XMLAttributes playerdata = XMLStuff.PlayerData->GetNodeById(playerid);
	if (playerdata.count("completionparent") > 0) {
		string aidx = playerdata["sourceid"] + "-" + playerdata["completionparent"];
		if (XMLStuff.PlayerData->bynamemod.count(aidx) == 0) {
			if (XMLStuff.PlayerData->byname.count(playerdata["completionparent"]) > 0) {
				return XMLStuff.PlayerData->GetNodeById(XMLStuff.PlayerData->byname[playerdata["completionparent"]]);
			}
		}
		else {
			return XMLStuff.PlayerData->GetNodeById(XMLStuff.PlayerData->bynamemod[aidx]);
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
	XMLAttributes playerdata = GetPlayerDataForMarks(playerid);
	string idx = playerdata["sourceid"] + "-" + playerdata["name"];
	ischartainted = false;
	if (playerdata.count("bskinparent") > 0) {
		idx = idx + "-Tainted-";
		ischartainted = true;
	}
	return idx;
}

array<int, 15> GetMarksForPlayer(int playerid, ANM2* anm = NULL,bool forrender = false) {
	array<int, 15> marks;
	if ((XMLStuff.PlayerData->nodes.count(playerid) > 0) || debugmode) {
		XMLAttributes playerdata = GetPlayerDataForMarks(playerid);
		string idx = GetMarksIdx(playerid);
		if (forrender) {
			hidemarks = false;
			if (playerdata.count("nomarks") > 0) {
				if (playerdata["nomarks"] != "false") {
					hidemarks = true;
				}
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
		if (anm) {
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

/*
HOOK_METHOD(Game,StartDebug, (int levelStage, int stageType, int difficulty, std_string* unk)->void) {
	//callsaveslot when instantstart is ran
	printf("[REPENTOGON] It's instant launch mode! %d %d %d %s \n", levelStage, stageType, difficulty, unk->c_str());
	g_Manager->SetSaveSlot(0); //normally the game does this as soon as it enters the main menu but since it never does here, it never does
	super(levelStage, stageType, difficulty, unk);
}
*/
HOOK_METHOD(Manager, SetSaveSlot, (unsigned int slot) -> void) {
	super(slot);
	int saveslot = 1;
	if (slot > 0) { saveslot = slot; }
	jsonpath = std::string(REPENTOGON::GetRepentogonDataPath());
	jsonpath.append("completionmarks").append(to_string(saveslot)).append(".json");

	LoadCompletionMarksFromJson();
	if (!initializedrendercmpl) {
		InitMarkRenderTypes();
		initreversenum();
		initmarkstoevents();
	}
}


bool evntreturnedfalse = false;
int lastcmplevent = -1;
HOOK_METHOD_PRIORITY(PersistentGameData, IncreaseEventCounter, 0, (int eEvent, int val) -> void) {
	if ((val > 0) && (EventsToPlayerType.count(eEvent) > 0)) {
		int playertype = EventsToPlayerType[eEvent];
		int cplmtype = EventsToCompletionType[eEvent];
		bool hardmode = (g_Game->IsHardMode());
		if (hardmode && (lastcmplevent == eEvent) && (val == 1)) {
			if (evntreturnedfalse) {
				evntreturnedfalse = false;
				return;
			}
			else {
				super(eEvent, val);
				return;
			}
		}
		if (!PreMarksCallbackTrigger(cplmtype, playertype)) {
			if (hardmode && (val == 1)) {
				lastcmplevent = eEvent;
				evntreturnedfalse = true;
			}
			return;
		}
		super(eEvent, val);
		if (val > 0) { RunTrackersForMark(cplmtype, playertype); PostMarksCallbackTrigger(cplmtype, playertype); }
		if (hardmode && (val == 1)) {
			lastcmplevent = eEvent;
		}
		return;
	}
	super(eEvent, val);

}


HOOK_STATIC_PRIORITY(Manager, RecordPlayerCompletion, 100, (int eEvent) -> void, __stdcall) {
	int numplayers = g_Game->GetNumPlayers();
	for (int i = 0; i < numplayers; i++) {
		int playertype = g_Game->GetPlayer(i)->GetPlayerType();
		if (playertype > 40) {
			string idx = GetMarksIdx(playertype);
			if (CompletionMarks.count(idx) == 0) {
				CompletionMarks[idx] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
			}
			int marktype = 1;
			if (g_Game->IsHardMode()) {
				marktype = 2;
			}
			if (CompletionMarks[idx][eEvent] < marktype) {
				if (PreMarksCallbackTrigger(eEvent, playertype)) {
					CompletionMarks[idx][eEvent] = marktype;
					RunTrackersForMark(eEvent, playertype);
					PostMarksCallbackTrigger(eEvent, playertype);
				}
			}
		}
	}
	SaveCompletionMarksToJson();
	super(eEvent);
}

bool postponepromptrender = false;
HOOK_METHOD_PRIORITY(GenericPrompt, Render,100, () -> void) {
	if (!postponepromptrender) {
		super();
	}
}

HOOK_METHOD_PRIORITY(GenericPopup, Render, 100, () -> void) {
	if (!postponepromptrender) {
		super();
	}
}

HOOK_METHOD(PauseScreen, Render, () -> void) {
	int playertype = g_Game->GetPlayer(0)->GetPlayerType();
	if ((playertype > 40) && (this->status != 2)) {
		postponepromptrender = true;
		super();
		postponepromptrender = false;

		NullFrame* nul = this->mainsprite.GetNullFrame("CompletionWidget");
		Vector* widgtpos = nul->GetPos();
		Vector* widgtscale = nul->GetScale();
		CompletionWidget* cmpl = this->GetCompletionWidget();

		ANM2* anm = cmpl->GetANM2();

		array marks = GetMarksForPlayer(playertype, anm,true);
		if (!hidemarks) {
			cmpl->CharacterId = playertype;
			cmpl->Render(new Vector((g_WIDTH * 0.6f) + widgtpos->x, (g_HEIGHT * 0.5f) + widgtpos->y), widgtscale);
		}
		if (this->notinfocus){
			this->_controllerconnectionpopup.Render();
		}
		g_Game->GetGenericPrompt()->Render();
	}
	else {
		super();
	}
}


HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	selectedchar = CharacterId;
	super(CharacterId, RenderPos, DefaultSprite);
}
HOOK_METHOD(Menu_Character, Render, () -> void) {
	super();
	CompletionWidget* cmpl = this->GetCompletionWidget();
	if (this->SelectedCharacterID > 17) {

		Vector* ref = &g_MenuManager->_ViewPosition;
		Vector* cpos = new Vector(ref->x - 80, ref->y + 894);
		ANM2* anm = cmpl->GetANM2();

		array marks = GetMarksForPlayer(selectedchar, anm,true);
		if (!hidemarks) {
			cmpl->CharacterId = selectedchar;
			cmpl->Render(new Vector(ref->x + 80, ref->y + 860), new Vector(1, 1));
		}

	}
}




array<int, 12> actualmarks = { CompletionType::MOMS_HEART,CompletionType::SATAN,CompletionType::MEGA_SATAN,CompletionType::HUSH,CompletionType::ISAAC,CompletionType::BLUE_BABY,CompletionType::MOTHER,CompletionType::DELIRIUM,CompletionType::BEAST,CompletionType::ULTRA_GREED,CompletionType::BOSS_RUSH,CompletionType::LAMB };
LUA_FUNCTION(Lua_IsaacSetCharacterMarks)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { return 0; }
	int playertype = 0;
	int length = 0;
	array<int, 15> marks = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) { //need to use lua_next because normal method wont work with string indexes
			if (lua_isstring(L, -2)) {
				const char* key = lua_tostring(L, -2);
				int value = (int)lua_tointeger(L, -1);
				if (reversemarksenum.count(key) > 0) {
					if ((value < 0) || (value > 2)) {
						return luaL_error(L, "Invalid Completion Marks value for %s is invalid(%d)", key, value);
					}
					marks[reversemarksenum[key]] = value;
				}
				else if (strcmp(key, "PlayerType") == 0) {
					playertype = value;
				}
				else {
					return luaL_error(L, "Invalid Completion Marks table index: %s", key);
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
	if (playertype > 40) {
		for (int i = 0; i < 15; i++) {
			if (marks[i] > 0) {
				if (PreMarksCallbackTrigger(i, playertype)) {
					RunTrackersForMark(i, playertype);
					PostMarksCallbackTrigger(i, playertype);
				}
				else {
					marks[i] = CompletionMarks[idx][i];
				}
			}
		}
		CompletionMarks[idx] = marks;
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 12; i++) {
			if ((marks[actualmarks[i]] == 0) || PreMarksCallbackTrigger(actualmarks[i], playertype)) {
				PData->IncreaseEventCounter(MarksToEvents[playertype][actualmarks[i]], -PData->GetEventCounter(MarksToEvents[playertype][actualmarks[i]]));
				PData->IncreaseEventCounter(MarksToEvents[playertype][actualmarks[i]], marks[actualmarks[i]]);
				if (marks[actualmarks[i]] > 0) {
					PostMarksCallbackTrigger(actualmarks[i], playertype);
				}
			}
		}
	}
	return 1;
}

LUA_FUNCTION(Lua_IsaacGetCharacterMark)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { lua_pushnumber(L, 2); return 0; }
	int completiontype = (int)luaL_checkinteger(L, 2);
	int playertype = (int)luaL_checkinteger(L, 1);
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

LUA_FUNCTION(Lua_IsaacClearCompletionMarks)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { return 0; }
	int playertype = (int)luaL_checkinteger(L, 1);
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


LUA_FUNCTION(Lua_IsaacFillCompletionMarks)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { lua_pushnumber(L, 2); return 0; }
	int playertype = (int)luaL_checkinteger(L, 1);
	int cmpldif = 2;
	if (playertype > 40) {
		string idx = GetMarksIdx(playertype);
		for (int i = 0; i < 15; i++) {
			if (PreMarksCallbackTrigger(i, playertype)) {
				CompletionMarks[idx][i] = 2;
				RunTrackersForMark(i, playertype);
				PostMarksCallbackTrigger(i, playertype);
			}
		}
		SaveCompletionMarksToJson();
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 15; i++) {
			if ((i != 8) && (i != 10) && (i != 11)) {
				if (PData->GetEventCounter(MarksToEvents[playertype][i]) < cmpldif) {
					if (PreMarksCallbackTrigger(i, playertype)) {
						PData->IncreaseEventCounter(MarksToEvents[playertype][i], 2);
						PostMarksCallbackTrigger(i, playertype);
					}
				}
			}
		}
	}
	lua_pushnumber(L, cmpldif);
	return 1;
}

array<int, 6> tduet = { CompletionType::HUSH,CompletionType::BOSS_RUSH,CompletionType::BOSS_RUSH,CompletionType::BOSS_RUSH,CompletionType::BOSS_RUSH,CompletionType::BOSS_RUSH };
array<int, 6> tquartet = { CompletionType::ISAAC,CompletionType::SATAN,CompletionType::LAMB,CompletionType::BLUE_BABY,CompletionType::BLUE_BABY,CompletionType::BLUE_BABY };
array<int, 6> tboth = { CompletionType::ISAAC,CompletionType::SATAN,CompletionType::LAMB,CompletionType::BLUE_BABY,CompletionType::HUSH,CompletionType::BOSS_RUSH };
LUA_FUNCTION(Lua_IsaacGetTaintedFullCompletion)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { lua_pushnumber(L, 2); return 0; }
	int playertype = (int)luaL_checkinteger(L, 1);
	int group = (int)luaL_checkinteger(L, 2);
	array g = tboth;
	switch (group) {
	case 0: g = tboth; break;
	case 1: g = tduet; break;
	case 2: g = tquartet; break;
	}
	if (group == 1) { g = tduet; }
	int cmpldif = 2;
	if (playertype > 40) {
		array<int, 15> marks = GetMarksForPlayer(playertype);
		for (int i = 0; i < 6; i++) {
			if (marks[g[i]] < cmpldif) {
				cmpldif = marks[g[i]];
			}
		}
	}
	else {
		PersistentGameData* PData = g_Manager->GetPersistentGameData();
		for (int i = 0; i < 6; i++) {
			if (PData->GetEventCounter(MarksToEvents[playertype][g[i]]) < cmpldif) {
				cmpldif = PData->GetEventCounter(MarksToEvents[playertype][g[i]]);
			}
		}
	}
	lua_pushnumber(L, cmpldif);
	return 1;
}


LUA_FUNCTION(Lua_IsaacGetFullCompletion)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) { lua_pushnumber(L, 2); return 0; }
	int playertype = (int)luaL_checkinteger(L, 1);
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

LUA_FUNCTION(Lua_IsaacSetCharacterMark)
{
	if (!debugmode && !initializedrendercmpl) { debugmode = true; }
	if (debugmode) {  return 0; }
	int completiontype = (int)luaL_checkinteger(L, 2);
	int playertype = (int)luaL_checkinteger(L, 1);
	int value = (int)luaL_checkinteger(L, 3);
	if ((value < 0) || (value > 2)) {
		return luaL_error(L, "Invalid Completion Marks value!(%d)", value);
	}
	if (playertype > 40) {
		if ((value == 0) || PreMarksCallbackTrigger(completiontype, playertype)) {
			array<int, 15> marks = GetMarksForPlayer(playertype);
			marks[completiontype] = value;
			string idx = GetMarksIdx(playertype);
			CompletionMarks[idx] = marks;
			SaveCompletionMarksToJson();
			if (value > 0) {
				RunTrackersForMark(completiontype, playertype);
				PostMarksCallbackTrigger(completiontype, playertype);
			}
		}
	}
	else {
		if ((value == 0) || (PreMarksCallbackTrigger(completiontype, playertype))) {
			PersistentGameData* PData = g_Manager->GetPersistentGameData();
			PData->IncreaseEventCounter(MarksToEvents[playertype][completiontype], -PData->GetEventCounter(MarksToEvents[playertype][completiontype]));
			PData->IncreaseEventCounter(MarksToEvents[playertype][completiontype], value);
			if (value > 0) {
				RunTrackersForMark(completiontype, playertype);
				PostMarksCallbackTrigger(completiontype, playertype);
			}
		}
	}
	return 1;
}


LUA_FUNCTION(Lua_IsaacGetCharacterMarks)
{
	int playertype = (int)luaL_checkinteger(L, 1);
	if (playertype > 40) {
		array<int, 15> marks = GetMarksForPlayer(playertype);
		lua_newtable(L);
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
		lua_newtable(L);
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

	lua::LuaStackProtector protector(_state);

	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCompletionMarks", Lua_IsaacGetCharacterMarks);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCompletionMark", Lua_IsaacGetCharacterMark);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetCompletionMarks", Lua_IsaacSetCharacterMarks);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetCompletionMark", Lua_IsaacSetCharacterMark);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "AllMarksFilled", Lua_IsaacGetFullCompletion);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "AllTaintedCompletion", Lua_IsaacGetTaintedFullCompletion);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "FillCompletionMarks", Lua_IsaacFillCompletionMarks);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "ClearCompletionMarks", Lua_IsaacClearCompletionMarks);
}