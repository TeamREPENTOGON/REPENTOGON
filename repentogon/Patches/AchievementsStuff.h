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

extern unordered_map<string, int> simplifiedeventsenum;
inline void initsimplifiedeventsenum() {
	simplifiedeventsenum["MOM_KILLS"] = 1; //Stat from the stat screen.Counts kills of Moms heart(Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["ROCKS_DESTROYED"] = 2; //Includes all rock types including tinted rocks
	simplifiedeventsenum["TINTED_ROCKS_DESTROYED"] = 3;
	simplifiedeventsenum["SUPER_SPECIAL_ROCKS_DESTROYED"] = 4; //Super Special Rocks are Tinted Rock variants with smaller marksand lack of discoloration
	simplifiedeventsenum["POOP_DESTROYED"] = 5; //Includes all poop types.Red poop counts everytime it gets destroyed
	simplifiedeventsenum["PILLS_EATEN"] = 6; //Includes all pills including horse pills
	simplifiedeventsenum["XIII_DEATH_CARD_USED"] = 7;
	simplifiedeventsenum["UNKNOWN_EVENT_8"] = 8;
	simplifiedeventsenum["ARCADES_ENTERED"] = 9; //Also includes teleports with goto.
	simplifiedeventsenum["DEATHS"] = 10; //does not increase when revived
	simplifiedeventsenum["ISAAC_KILLS"] = 11; //How often was the final boss of cathedral killed. (Isaac)(Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["SHOPKEEPER_KILLED"] = 12;
	simplifiedeventsenum["SATAN_KILLS"] = 13; //How often was the final boss of sheol killed. (Satan)(Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["SHELLGAMES_PLAYED"] = 14; //How often the shellgame was played
	simplifiedeventsenum["ANGEL_DEALS_TAKEN"] = 15;
	simplifiedeventsenum["DEVIL_DEALS_TAKEN"] = 16;
	simplifiedeventsenum["BLOOD_DONATION_MACHINE_USED"] = 17;
	simplifiedeventsenum["SLOT_MACHINES_BROKEN"] = 18;
	simplifiedeventsenum["DONATE_MACHINE_COUNTER"] = 20;
	simplifiedeventsenum["DONATION_MACHINE_COUNTER"] = 20;
	simplifiedeventsenum["EDEN_TOKENS"] = 21;
	simplifiedeventsenum["STREAK_COUNTER"] = 22; //counts your current streak.
	simplifiedeventsenum["BEST_STREAK"] = 23;
	simplifiedeventsenum["BLUE_BABY_KILLS"] = 24; //How often was the final boss of chest killed. (? ? ? -Blue Baby) (Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["LAMB_KILLS"] = 25; //How often was the final boss of dark room killed. (The lamb) (Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["MEGA_SATAN_KILLS"] = 26;
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_ISAAC"] = 27; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_MAGDALENE"] = 28; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_CAIN"] = 29; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_JUDAS"] = 30; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_BLUE_BABY"] = 31; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_EVE"] = 32; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_SAMSON"] = 33; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_AZAZEL"] = 34; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_LAZARUS"] = 35; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_EDEN"] = 36; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_THE_LOST"] = 37; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_LILITH"] = 38; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_KEEPER"] = 39; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_APOLLYON"] = 40; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_ISAAC"] = 41; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_MAGDALENE"] = 42; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_CAIN"] = 43; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_JUDAS"] = 44; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_BLUE_BABY"] = 45; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_EVE"] = 46; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_SAMSON"] = 47; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_AZAZEL"] = 48; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_LAZARUS"] = 49; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_EDEN"] = 50; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_THE_LOST"] = 51; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_LILITH"] = 52; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_KEEPER"] = 53; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_APOLLYON"] = 54; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_ISAAC"] = 55; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_MAGDALENE"] = 56; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_CAIN"] = 57; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_JUDAS"] = 58; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_BLUE_BABY"] = 59; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_EVE"] = 60; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_SAMSON"] = 61; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_AZAZEL"] = 62; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_LAZARUS"] = 63; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_EDEN"] = 64; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_THE_LOST"] = 65; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_LILITH"] = 66; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_KEEPER"] = 67; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_APOLLYON"] = 68; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_ISAAC"] = 69; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_MAGDALENE"] = 70; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_CAIN"] = 71; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_JUDAS"] = 72; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_BLUE_BABY"] = 73; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_EVE"] = 74; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_SAMSON"] = 75; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_AZAZEL"] = 76; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_LAZARUS"] = 77; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_EDEN"] = 78; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_THE_LOST"] = 79; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_LILITH"] = 80; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_KEEPER"] = 81; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_APOLLYON"] = 82; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_ISAAC"] = 83; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_MAGDALENE"] = 84; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_CAIN"] = 85; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_JUDAS"] = 86; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_BLUE_BABY"] = 87; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_EVE"] = 88; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_SAMSON"] = 89; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_AZAZEL"] = 90; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_LAZARUS"] = 91; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_EDEN"] = 92; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_THE_LOST"] = 93; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_LILITH"] = 94; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_KEEPER"] = 95; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_APOLLYON"] = 96; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_ISAAC"] = 97; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_MAGDALENE"] = 98; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_CAIN"] = 99; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_JUDAS"] = 100; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_BLUE_BABY"] = 101; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_EVE"] = 102; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_SAMSON"] = 103; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_AZAZEL"] = 104; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_LAZARUS"] = 105; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_EDEN"] = 106; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_THE_LOST"] = 107; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_LILITH"] = 108; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_KEEPER"] = 109; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_APOLLYON"] = 110; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSHS_CLEARED"] = 111;
	simplifiedeventsenum["UNKNOWN_EVENT_112"] = 112; //seems to only be at 1 for a dead god file
	simplifiedeventsenum["NEGATIVE_STREAK_COUNTER"] = 113; //counts your current negative streak.aka consecutive runs ending with death.
	simplifiedeventsenum["EDENS_BLESSINGS_NEXT_RUN"] = 114; //number of edens blessing items added to the player in next run
	simplifiedeventsenum["GREED_DONATION_MACHINE_COUNTER"] = 115;
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_ISAAC"] = 116; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_MAGDALENE"] = 117; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_CAIN"] = 118; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_JUDAS"] = 119; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_BLUE_BABY"] = 120; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_EVE"] = 121; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_SAMSON"] = 122; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_AZAZEL"] = 123; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_LAZARUS"] = 124; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_EDEN"] = 125; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_THE_LOST"] = 126; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_LILITH"] = 127; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_KEEPER"] = 128; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_APOLLYON"] = 129; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_ISAAC"] = 130; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_MAGDALENE"] = 131; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_CAIN"] = 132; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_JUDAS"] = 133; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_BLUE_BABY"] = 134; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_EVE"] = 135; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_SAMSON"] = 136; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_AZAZEL"] = 137; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_LAZARUS"] = 138; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_EDEN"] = 139; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_THE_LOST"] = 140; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_LILITH"] = 141; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_KEEPER"] = 142; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_APOLLYON"] = 143; //(value"] = 0; 1 for greed mode; 2 for greedier)
	simplifiedeventsenum["KILL_HUSH_WITH_ISAAC"] = 144; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_MAGDALENE"] = 145; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_CAIN"] = 146; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_JUDAS"] = 147; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_BLUE_BABY"] = 148; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_EVE"] = 149; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_SAMSON"] = 150; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_AZAZEL"] = 151; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_LAZARUS"] = 152; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_EDEN"] = 153; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_THE_LOST"] = 154; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_LILITH"] = 155; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_KEEPER"] = 156; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_APOLLYON"] = 157; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["HUSH_KILLS"] = 158; //How often Hush was killed. (Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_ISAAC"] = 159;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_MAGDALENE"] = 160;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_CAIN"] = 161;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_JUDAS"] = 162;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_BLUE"] = 163;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_EVE"] = 164;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_SAMSON"] = 165;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_AZAZEL"] = 166;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_LAZARUS"] = 167;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_EDEN"] = 168;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_THE"] = 169;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_LILITH"] = 170;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_KEEPER"] = 171;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_APOLLYON"] = 172;
	simplifiedeventsenum["KILL_DELIRIUM_WITH_ISAAC"] = 173; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_MAGDALENE"] = 174; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_CAIN"] = 175; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_JUDAS"] = 176; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_BLUE_BABY"] = 177; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_EVE"] = 178; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_SAMSON"] = 179; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_AZAZEL"] = 180; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_LAZARUS"] = 181; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_EDEN"] = 182; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_THE_LOST"] = 183; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_LILITH"] = 184; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_KEEPER"] = 185; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_APOLLYON"] = 186; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["DELIRIUM_KILLS"] = 187; //How often was the final boss of the void was killed. (Delirium)(Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["CHARACTER_LAST_RUN_WIN"] = 188; //Potential Bitmask containing the ids of the characters that won the last run; including coop.gets cleared on run restart.TODO: Usage and pattern still unknown!!!
	simplifiedeventsenum["UNKNOWN_EVENT_189"] = 189; //really big counter
	simplifiedeventsenum["DAILYS_PLAYED"] = 190; //counts dailies played
	simplifiedeventsenum["DAILY_RUN_IS_ACTIVE"] = 191; //Set to 1 when daily is currently played.Set to 0 when trophy was collected
	simplifiedeventsenum["DAILYS_STREAK"] = 192; //Numbers of dailies won in a row
	simplifiedeventsenum["DAILYS_WON"] = 193; //Number of dailies won
	simplifiedeventsenum["RAINBOW_POOP_DESTROYED"] = 194;
	simplifiedeventsenum["BATTERIES_COLLECTED"] = 195; //Sum of all batteries collected
	simplifiedeventsenum["CARDS_USED"] = 196; //Sum of cards used
	simplifiedeventsenum["SHOP_ITEMS_BOUGHT"] = 197; //Sum of shop items bought. this includes devil deals; dark room red chest deals; and black market deals
	simplifiedeventsenum["CHESTS_OPENED_WITH_KEY"] = 198; //Sum of all goldenand Eternal Chests opened.Mega chests dont count
	simplifiedeventsenum["SECRET_ROOMS_WALLS_OPENED"] = 199; //Includes super secret roomsand counts walls that lead to secret rooms already entered or discovered.Also counts the door towards the mines.Does not count X - Ray Vision effect
	simplifiedeventsenum["BLOOD_CLOT_ITEM_AQUIRED"] = 200;
	simplifiedeventsenum["RUBBER_CEMENT_ITEM_AQUIRED"] = 201;
	simplifiedeventsenum["BEDS_USED"] = 202; //Sleeping in a bed
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_THE_FORGOTTEN"] = 203; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_THE_FORGOTTEN"] = 204; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_THE_FORGOTTEN"] = 205; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_THE_FORGOTTEN"] = 206; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_THE_FORGOTTEN"] = 207; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_THE_FORGOTTEN"] = 208; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_THE_FORGOTTEN"] = 209; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_THE_FORGOTTEN"] = 210; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_THE_FORGOTTEN"] = 211; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_FORGOTTEN"] = 212;
	simplifiedeventsenum["KILL_DELIRIUM_WITH_THE_FORGOTTEN"] = 213; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_BETHANY"] = 214; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_JACOB_AND_ESAU"] = 215; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_ISAAC"] = 216; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_MAGDALENE"] = 217; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_CAIN"] = 218; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_JUDAS"] = 219; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_BLUE_BABY"] = 220; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_EVE"] = 221; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_SAMSON"] = 222; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_AZAZEL"] = 223; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_LAZARUS"] = 224; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_EDEN"] = 225; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_THE_LOST"] = 226; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_LILITH"] = 227; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_KEEPER"] = 228; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_APOLLYON"] = 229; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_THE_FORGOTTEN"] = 230; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_BETHANY"] = 231; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOMS_HEART_WITH_T_JACOB_AND_ESAU"] = 232; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_BETHANY"] = 233; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_JACOB_AND_ESAU"] = 234; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_ISAAC"] = 235; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_MAGDALENE"] = 236; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_CAIN"] = 237; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_JUDAS"] = 238; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_BLUE_BABY"] = 239; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_EVE"] = 240; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_SAMSON"] = 241; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_AZAZEL"] = 242; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_LAZARUS"] = 243; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_EDEN"] = 244; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_THE_LOST"] = 245; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_LILITH"] = 246; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_KEEPER"] = 247; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_APOLLYON"] = 248; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_THE_FORGOTTEN"] = 249; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_BETHANY"] = 250; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_ISAAC_WITH_T_JACOB_AND_ESAU"] = 251; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_BETHANY"] = 252; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_JACOB_AND_ESAU"] = 253; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_ISAAC"] = 254; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_MAGDALENE"] = 255; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_CAIN"] = 256; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_JUDAS"] = 257; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_BLUE_BABY"] = 258; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_EVE"] = 259; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_SAMSON"] = 260; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_AZAZEL"] = 261; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_LAZARUS"] = 262; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_EDEN"] = 263; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_THE_LOST"] = 264; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_LILITH"] = 265; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_KEEPER"] = 266; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_APOLLYON"] = 267; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_THE_FORGOTTEN"] = 268; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_BETHANY"] = 269; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_SATAN_WITH_T_JACOB_AND_ESAU"] = 270; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_BETHANY"] = 271; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_JACOB_AND_ESAU"] = 272; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_ISAAC"] = 273; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_MAGDALENE"] = 274; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_CAIN"] = 275; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_JUDAS"] = 276; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_BLUE_BABY"] = 277; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_EVE"] = 278; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_SAMSON"] = 279; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_AZAZEL"] = 280; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_LAZARUS"] = 281; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_EDEN"] = 282; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_THE_LOST"] = 283; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_LILITH"] = 284; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_KEEPER"] = 285; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_APOLLYON"] = 286; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_THE_FORGOTTEN"] = 287; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_BETHANY"] = 288; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["BOSSRUSH_CLEARED_WITH_T_JACOB_AND_ESAU"] = 289; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_BETHANY"] = 290; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_JACOB_AND_ESAU"] = 291; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_ISAAC"] = 292; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_MAGDALENE"] = 293; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_CAIN"] = 294; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_JUDAS"] = 295; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_BLUE_BABY"] = 296; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_EVE"] = 297; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_SAMSON"] = 298; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_AZAZEL"] = 299; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_LAZARUS"] = 300; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_EDEN"] = 301; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_THE_LOST"] = 302; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_LILITH"] = 303; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_KEEPER"] = 304; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_APOLLYON"] = 305; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_THE_FORGOTTEN"] = 306; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_BETHANY"] = 307; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BLUE_BABY_WITH_T_JACOB_AND_ESAU"] = 308; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_BETHANY"] = 309; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_JACOB_AND_ESAU"] = 310; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_ISAAC"] = 311; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_MAGDALENE"] = 312; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_CAIN"] = 313; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_JUDAS"] = 314; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_BLUE_BABY"] = 315; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_EVE"] = 316; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_SAMSON"] = 317; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_AZAZEL"] = 318; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_LAZARUS"] = 319; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_EDEN"] = 320; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_THE_LOST"] = 321; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_LILITH"] = 322; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_KEEPER"] = 323; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_APOLLYON"] = 324; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_THE_FORGOTTEN"] = 325; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_BETHANY"] = 326; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_THE_LAMB_WITH_T_JACOB_AND_ESAU"] = 327; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_BETHANY"] = 328; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_JACOB_AND_ESAU"] = 329; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_ISAAC"] = 330; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_MAGDALENE"] = 331; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_CAIN"] = 332; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_JUDAS"] = 333; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_BLUE_BABY"] = 334; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_EVE"] = 335; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_SAMSON"] = 336; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_AZAZEL"] = 337; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_LAZARUS"] = 338; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_EDEN"] = 339; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_THE_LOST"] = 340; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_LILITH"] = 341; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_KEEPER"] = 342; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_APOLLYON"] = 343; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_THE_FORGOTTEN"] = 344; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_BETHANY"] = 345; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MEGA_SATAN_WITH_T_JACOB_AND_ESAU"] = 346; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_BETHANY"] = 347; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_JACOB_AND_ESAU"] = 348; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_ISAAC"] = 349; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_MAGDALENE"] = 350; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_CAIN"] = 351; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_JUDAS"] = 352; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_BLUE_BABY"] = 353; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_EVE"] = 354; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_SAMSON"] = 355; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_AZAZEL"] = 356; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_LAZARUS"] = 357; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_EDEN"] = 358; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_THE_LOST"] = 359; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_LILITH"] = 360; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_KEEPER"] = 361; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_APOLLYON"] = 362; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_THE_FORGOTTEN"] = 363; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_BETHANY"] = 364; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_CLEARED_WITH_T_JACOB_AND_ESAU"] = 365; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_BETHANY"] = 366; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_JACOB_AND_ESAU"] = 367; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_ISAAC"] = 368; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_MAGDALENE"] = 369; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_CAIN"] = 370; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_JUDAS"] = 371; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_BLUE_BABY"] = 372; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_EVE"] = 373; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_SAMSON"] = 374; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_AZAZEL"] = 375; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_LAZARUS"] = 376; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_EDEN"] = 377; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_THE_LOST"] = 378; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_LILITH"] = 379; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_KEEPER"] = 380; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_APOLLYON"] = 381; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_THE_FORGOTTEN"] = 382; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_BETHANY"] = 383; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_HUSH_WITH_T_JACOB_AND_ESAU"] = 384; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_BETHANY"] = 385;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_JACOB_AND_ESAU"] = 386;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_ISAAC"] = 387;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_MAGDALENE"] = 388;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_CAIN"] = 389;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_JUDAS"] = 390;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_BLUE_BABY"] = 391;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_EVE"] = 392;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_SAMSON"] = 393;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_AZAZEL"] = 394;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_LAZARUS"] = 395;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_EDEN"] = 396;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_THE_LOST"] = 397;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_LILITH"] = 398;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_KEEPER"] = 399;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_THE_FORGOTTEN"] = 400;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_BETHANY"] = 401;
	simplifiedeventsenum["GREED_MODE_COINS_DONATED_WITH_T_JACOB_AND_ESAU"] = 402;
	simplifiedeventsenum["KILL_DELIRIUM_WITH_BETHANY"] = 404; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_JACOB_AND_ESAU"] = 405; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_ISAAC"] = 406; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_MAGDALENE"] = 407; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_CAIN"] = 408; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_JUDAS"] = 409; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_BLUE_BABY"] = 410; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_EVE"] = 411; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_SAMSON"] = 412; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_AZAZEL"] = 413; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_LAZARUS"] = 414; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_EDEN"] = 415; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_THE_LOST"] = 416; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_LILITH"] = 417; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_KEEPER"] = 418; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_APOLLYON"] = 419; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_THE_FORGOTTEN"] = 420; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_BETHANY"] = 421; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_DELIRIUM_WITH_T_JACOB_AND_ESAU"] = 422; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_ISAAC"] = 423; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_MAGDALENE"] = 424; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_CAIN"] = 425; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_JUDAS"] = 426; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_BLUE_BABY"] = 427; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_EVE"] = 428; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_SAMSON"] = 429; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_AZAZEL"] = 430; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_LAZARUS"] = 431; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_EDEN"] = 432; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_THE_LOST"] = 433; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_LILITH"] = 434; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_KEEPER"] = 435; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_APOLLYON"] = 436; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_THE_FORGOTTEN"] = 437; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_BETHANY"] = 438; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_JACOB_AND_ESAU"] = 439; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_ISAAC"] = 440; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_MAGDALENE"] = 441; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_CAIN"] = 442; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_JUDAS"] = 443; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_BLUE_BABY"] = 444; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_EVE"] = 445; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_SAMSON"] = 446; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_AZAZEL"] = 447; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_LAZARUS"] = 448; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_EDEN"] = 449; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_THE_LOST"] = 450; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_LILITH"] = 451; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_KEEPER"] = 452; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_APOLLYON"] = 453; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_THE_FORGOTTEN"] = 454; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_BETHANY"] = 455; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_MOTHER_WITH_T_JACOB_AND_ESAU"] = 456; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_ISAAC"] = 457; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_MAGDALENE"] = 458; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_CAIN"] = 459; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_JUDAS"] = 460; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_BLUE_BABY"] = 461; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_EVE"] = 462; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_SAMSON"] = 463; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_AZAZEL"] = 464; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_LAZARUS"] = 465; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_EDEN"] = 466; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_THE_LOST"] = 467; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_LILITH"] = 468; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_KEEPER"] = 469; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_APOLLYON"] = 470; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_THE_FORGOTTEN"] = 471; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_BETHANY"] = 472; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_JACOB_AND_ESAU"] = 473; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_ISAAC"] = 474; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_MAGDALENE"] = 475; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_CAIN"] = 476; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_JUDAS"] = 477; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_BLUE_BABY"] = 478; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_EVE"] = 479; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_SAMSON"] = 480; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_AZAZEL"] = 481; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_LAZARUS"] = 482; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_EDEN"] = 483; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_THE_LOST"] = 484; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_LILITH"] = 485; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_KEEPER"] = 486; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_APOLLYON"] = 487; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_THE_FORGOTTEN"] = 488; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_BETHANY"] = 489; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["KILL_BEAST_WITH_T_JACOB_AND_ESAU"] = 490; //(0 for not done. 1 for normal difficulty; 2 for hard)
	simplifiedeventsenum["MOTHER_KILLS"] = 491; //Killing endboss of Corpse 2 (Does only count instances where the kill is combined with run "progression".aka in final room and spawning a trapdoor / chest / trophy)
	simplifiedeventsenum["BEAST_KILLS"] = 492; //Killing the endboss of Home
	simplifiedeventsenum["BABY_PLUM_KILLS"] = 493;
	simplifiedeventsenum["BATTERY_BUMS_KILLED"] = 494;
	simplifiedeventsenum["BATTERY_BUM_COLLECTIBLE_PAYOUTS"] = 495; //counts how often the battery bum paid out with a collectible item
}


extern unordered_map<string, int > Achievements; //1 unlocked != 1 locked
extern unordered_map<string, XMLAttributes > AchievConditionsById;
extern unordered_map<int, vector<string> > ConditionsByAchievId;
extern unordered_map<string, unordered_map<int,int> > ConditionCounters; //conditionid,map<achievementid,counter>
extern unordered_map<int, unordered_map<int,vector<string>> > CompletionMarkListeners; //marktype, map<charatype,vector<conditionalid>>
extern unordered_map<int, unordered_map<int, vector<string>> > EventCounterListeners; //marktype, map<charatype,vector<conditionalid>>
extern unordered_map<tuple<int, int, int>, unordered_map<int, vector<string>>> BossDeathListeners; //marktype, map<charatype,vector<conditionalid>>
extern string achivjsonpath;

extern int toint(string str);


inline bool CheckAchievCompleted(int achievementid) {
	bool done = true;
	for each (string condid in ConditionsByAchievId[achievementid]) {
		if (ConditionCounters.count(condid) >= 0) {
			done = false;
			break;
		}
	}
	if (done) {
		string achievement = XMLStuff.AchievementData->nodes[achievementid]["name"] + XMLStuff.AchievementData->nodes[achievementid]["sourceid"];
		g_Manager->GetPersistentGameData()->TryUnlock(achievementid);
	}
	return done;
}

inline void IncreaseConditionCounter(string* conditionalid) {
	if (ConditionCounters.count(*conditionalid) > 0) {
		std::vector<int> keysToRemove;
		for each (auto & counter  in ConditionCounters[*conditionalid]){
			int achievementid = counter.first;
			if (counter.second == 0) {
				ConditionCounters[*conditionalid][achievementid] = 1;
				keysToRemove.push_back(counter.first);
			}
			else if (counter.second < 0) {
				ConditionCounters[*conditionalid][achievementid] += 1;
			}
		}
		for (int key : keysToRemove) {
			ConditionCounters[*conditionalid].erase(key);
		}
	}
}

inline void AddMarkTracker(string conditionid, int marktype, int charaid = 0) {
	CompletionMarkListeners[marktype][charaid].push_back(conditionid);
}

inline void RunTrackersForMark(int marktype, int charaid = 0) {
	for each (string conditionid in CompletionMarkListeners[marktype][charaid]) {
		IncreaseConditionCounter(&conditionid);
	}
	if (charaid > 0) {
		for each (string conditionid in CompletionMarkListeners[marktype][0]) {
			IncreaseConditionCounter(&conditionid);
		}
	}
}

inline void AddBossDeathTracker(string conditionid, tuple<int, int, int>* boss, int charaid = 0) {
	BossDeathListeners[*boss][charaid].push_back(conditionid);
}

inline void RunTrackersForBossDeath(tuple<int, int, int>* boss,int charaid = 0) {
	for each (string conditionid in BossDeathListeners[*boss][0]) {
		IncreaseConditionCounter(&conditionid);
	}
	if (charaid > 0) {
		for each (string conditionid in BossDeathListeners[*boss][0]) {
			IncreaseConditionCounter(&conditionid);
		}
	}
}

inline void AddEventCounterTracker(string conditionid,int eventid,int charaid = 0) {
	EventCounterListeners[eventid][charaid].push_back(conditionid);
}

inline void RunTrackersForEventCounter(int eventid, int charaid = 0) {
	for each (string conditionid in EventCounterListeners[eventid][charaid]) {
		IncreaseConditionCounter(&conditionid);
	}
	if (charaid > 0) {
		for each (string conditionid in EventCounterListeners[eventid][0]) {
			IncreaseConditionCounter(&conditionid);
		}
	}
}



inline bool ProcessCondition(XMLAttributes daddy,XMLAttributes node) {
	string conditionidx;
	int completionchara = 0;
	if (node.count("characondition") > 0) {
		completionchara = XMLStuff.PlayerData->byname[node["characondition"]];
	}

	if (node.count("markcondition") > 0) {
		printf("[Achiev] Mark tracker for '%s' with condition %s \n", node["characondition"].c_str(), node["markcondition"].c_str());
		conditionidx = node["count"] + "/" + daddy["name"] + "/" + node["markcondition"] + "/" + node["characondition"];
		AddMarkTracker(conditionidx, reversemarksenum[node["markcondition"]], completionchara);
	}
	else if (node.count("eventcondition") > 0) {
		printf("[Achiev] Event tracker for '%s' with condition %s \n", node["eventcondition"].c_str(), node["eventcondition"].c_str());
		conditionidx = node["count"] + "/" + daddy["name"] + "/" + node["markcondition"] + "/" + node["eventcondition"];
		AddEventCounterTracker(node["count"] + "/" + daddy["name"] + "/" + node["markcondition"] + "/" + node["eventcondition"], simplifiedeventsenum[node["eventcondition"]], completionchara);
	}

	if (conditionidx.length() > 0) {
		AchievConditionsById[conditionidx] = node;
		ConditionsByAchievId[toint(daddy["id"])].push_back(conditionidx);
	}
	
}


inline void InitAchievs() {
	Achievements.clear();
	initsimplifiedeventsenum();
	CompletionMarkListeners.clear();
	EventCounterListeners.clear();
	BossDeathListeners.clear();
	for each (auto & att in XMLStuff.AchievementData->nodes)
	{
		if (att.first > 638) {
			XMLAttributes node = att.second;
			string idx = node["name"] + node["sourceid"];
			Achievements[idx] = 0;
			ProcessCondition(node, node); //single node conditions embed into the achiev node itself
			for each (auto & cond in XMLStuff.AchievementData->childs[att.first]["condition"]) {
				ProcessCondition(node, cond);
			}
		}
	}
}




#endif