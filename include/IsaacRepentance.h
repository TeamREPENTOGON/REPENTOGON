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

extern "C"
{
	#include "lstate.h"
}

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


struct PlayerManager;
struct Entity_Player;

struct PlayerManager
{
	LIBZHL_API Entity_Player *SpawnCoPlayer(int unk);
	LIBZHL_API Entity_Player *SpawnCoPlayer2(int unk);
	
};

struct LuaEngine;

struct LuaEngine
{
	LIBZHL_API void Init(bool Debug);
	LIBZHL_API void RegisterClasses();
	
	char pad0[24];
	lua_State *_state;
};

struct Manager
{
	LIBZHL_API void __stdcall Update();
	
};

struct Globals
{
};

struct Entity;

struct LIBZHL_INTERFACE Entity
{
	Entity() 
	{
		this->constructor();
	}

	virtual ~Entity() {}
	LIBZHL_API virtual void Init(unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
	virtual void PreUpdate() LIBZHL_PLACEHOLDER
	LIBZHL_API virtual void Update();
	LIBZHL_API void constructor();
	
};

struct Entity_Slot : Entity
{
};

struct Room
{
	LIBZHL_API float __stdcall GetDevilRoomChance();
	
};

struct Vector
{
	Vector() : x(0.f), y(0.f) {}
	Vector(float _x, float _y) : x(_x), y(_y) {}
	
	Vector operator+(const Vector& other)
	{
		return Vector(x + other.x, y + other.y);
	}
	
	Vector operator-(const Vector& other)
	{		
		return Vector(x - other.x, y - other.y);
	}
	
	Vector operator/(float amount)
	{		
		return Vector(x / amount, y / amount);
	}
	
	Vector operator*(float amount)
	{		
		return Vector(x * amount, y * amount);
	}


	float x;
	float y;
};

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
struct Vector;

struct Game
{
	Game()
	{
		this->constructor();
	}

	LIBZHL_API void constructor();
	LIBZHL_API bool IsPaused();
	LIBZHL_API Entity *Spawn(unsigned int type, unsigned int variant, const Vector &position, const Vector &velocity, Entity *spawner, unsigned int subtype, unsigned int seed, unsigned int unk);
	LIBZHL_API void ShakeScreen(int timeout);
	LIBZHL_API void MakeShockwave(const Vector &pos, float amplitude, float speed, int duration);
	LIBZHL_API void __stdcall Update();
	
	int _floorNum;
	bool _altFloor;
	char unk1[3];
	int _curses;
};

struct Camera;
struct Room;

struct Camera
{
	LIBZHL_API void constructor(Room *room);
	LIBZHL_API void SetFocusPosition(const Vector &pos);
	
};

LIBZHL_API unsigned int __stdcall Random();

extern LIBZHL_API Game **__ptr_g_Game;
#define g_Game (*__ptr_g_Game)
extern LIBZHL_API LuaEngine **__ptr_g_LuaEngine;
#define g_LuaEngine (*__ptr_g_LuaEngine)
extern LIBZHL_API Manager **__ptr_g_Manager;
#define g_Manager (*__ptr_g_Manager)


