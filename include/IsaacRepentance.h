#pragma once

#pragma warning( disable : 4722 )

#ifdef LIBZHL_EXPORTS
#define LIBZHL_API __declspec(dllexport)
#else
#define LIBZHL_API __declspec(dllimport)
#endif

#define LIBZHL_INTERFACE __declspec(novtable)

#define MOD_EXPORT extern "C" __declspec(dllexport)

__declspec(noreturn) inline void __cdecl __NOP() {}
#define LIBZHL_PLACEHOLDER {__NOP();}


#include <windows.h>
//#include "rapidxml.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <deque>
#include <stdarg.h>
#include <unordered_map>
#include <math.h>
#include <queue>
#undef LoadImage

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

/*struct CAchievement;
struct RandomAmount;
struct Point;

typedef std::pair<int, int> std_pair_int_int;
typedef std::pair<float, float> std_pair_float_float;
typedef std::pair<std::string, int> std_pair_std_string_int;
typedef std::pair<std::string, RandomAmount> std_pair_std_string_RandomAmount;
typedef std::pair<std::string, std::string> std_pair_std_string_std_string;
typedef std::pair<CAchievement*, Point> std_pair_CAchievement_ptr_Point;
typedef std::map<std::string, std::string> std_map_std_string_std_string;
typedef std::map<std::string, int> std_map_std_string_int;
typedef std::map<std::string, bool> std_map_std_string_bool;
*/



	
static DWORD GetBaseAddress()
{
	return (DWORD)GetModuleHandle(NULL);
}


struct Entity
{
};

struct Globals
{
};

struct lua_State
{
};

struct Manager
{
	LIBZHL_API static void __stdcall Update();
	
};

struct LuaEngine;

struct LuaEngine
{
	lua_State *GetState() {return &_state;}

	LIBZHL_API void Init(bool Debug);
	
	lua_State _state;
};

struct Entity_Slot : Entity
{
};

struct Entity_Player;

struct Entity_Player : Entity
{
	LIBZHL_API void AddCollectible(int type, int charge, bool firsttime, int slot, int vardata);
	LIBZHL_API void AddBombs(int amount);
	LIBZHL_API void AddKeys(int amount);
	LIBZHL_API void AddJarFlies(int amount);
	LIBZHL_API void AddPrettyFly();
	LIBZHL_API void AddCoins(int amount);
	
};

struct Game;

struct Game
{
	LIBZHL_API bool IsPaused();
	LIBZHL_API void ShakeScreen(int timeout);
	
};

extern LIBZHL_API Game **__ptr_g_Game;
#define g_Game (*__ptr_g_Game)
extern LIBZHL_API Manager **__ptr_g_Manager;
#define g_Manager (*__ptr_g_Manager)
extern LIBZHL_API LuaEngine **__ptr_g_LuaEngine;
#define g_LuaEngine (*__ptr_g_LuaEngine)


