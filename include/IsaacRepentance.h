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
#include <cstdarg>
#include <unordered_map>
#include <cmath>
#include <queue>
    #include <cstdint>
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

struct Entity;

struct EntityRef
{
	int _type;
	int _variant;
	int _spawnerType;
	unsigned int _spawnerVariant;
	Vector _position;
	Vector _velocity;
	unsigned int _flags;
	Entity *_entity;
};

struct PosVel
{
	PosVel() : pos(Vector()), vel(Vector()) {}
	PosVel(Vector _pos, Vector _vel) : pos(_pos), vel(_vel) {}

	PosVel operator+(const PosVel& other)
	{
		return PosVel(pos + other.pos, vel + other.vel);
	}
	
	PosVel operator-(const PosVel& other)
	{		
		return PosVel(pos - other.pos, vel - other.vel);
	}
	
	PosVel operator/(float amount)
	{		
		return PosVel(pos / amount, vel / amount);
	}
	
	PosVel operator*(float amount)
	{		
		return PosVel(pos * amount, vel * amount);
	}


	Vector pos;
	Vector vel;
};
	

enum WeaponType
{
	WEAPON_NULL = 0x0,
	WEAPON_TEARS = 0x1,
	WEAPON_BRIMSTONE = 0x2,
	WEAPON_LASER = 0x3,
	WEAPON_KNIFE = 0x4,
	WEAPON_BOMBS = 0x5,
	WEAPON_ROCKETS = 0x6,
	WEAPON_MONSTROS_LUNGS = 0x7,
	WEAPON_LUDOVICO_TECHNIQUE = 0x8,
	WEAPON_TECH_X = 0x9,
	WEAPON_BONE = 0xA,
	WEAPON_NOTCHED_AXE = 0xB,
	WEAPON_URN_OF_SOULS = 0xC,
	WEAPON_SPIRIT_SWORD = 0xD,
	WEAPON_FETUS = 0xE,
 		WEAPON_UMBILICAL_WHIP = 0xF,
	NUM_WEAPON_TYPES = 0x10,
};

enum GridEntityType {
	GRID_NULL = 0x0,
	GRID_DECORATION = 0x1,
	GRID_ROCK = 0x2,
	GRID_ROCKB = 0x3,	
	GRID_ROCKT = 0x4,	
	GRID_ROCK_BOMB = 0x5,	
	GRID_ROCK_ALT = 0x6,	
	GRID_PIT = 0x7,
	GRID_SPIKES = 0x8,
	GRID_SPIKES_ONOFF = 0x9,
	GRID_SPIDERWEB = 0xA,
	GRID_LOCK = 0xB,
	GRID_TNT = 0xC,
	GRID_FIREPLACE = 0xD,
	GRID_POOP = 0xE,
	GRID_WALL = 0xF,
	GRID_DOOR = 0x10,
	GRID_TRAPDOOR = 0x11,
	GRID_STAIRS = 0x12,
	GRID_GRAVITY = 0x13,
	GRID_PRESSURE_PLATE = 0x14,
	GRID_STATUE = 0x15,
	GRID_ROCK_SS = 0x16,
	GRID_TELEPORTER = 0x17,
	GRID_PILLAR = 0x18,
	GRID_ROCK_SPIKED = 0x19,
	GRID_ROCK_ALT2 = 0x1A,
	GRID_ROCK_GOLD = 0x1B,
};

static DWORD GetBaseAddress()
{
	return (DWORD)GetModuleHandle(NULL);
}


struct Globals
{
};

struct EntityRef;

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
	LIBZHL_API virtual bool TakeDamage(float Damage, unsigned __int64 DamageFlags, EntityRef *Source, int DamageCountdown);
	
	char pad0[752];
	float _timeScale;
};

struct Entity_Slot : Entity
{
};

struct GridEntity
{
	int _unk;
	GridEntityType _type;
	int _variant;
	int _state;
	int _unk2;
	int _varData;
	int _unk3;
};

struct GridEntity_Rock;

struct GridEntity_Rock : GridEntity
{
	LIBZHL_API void Update();
	LIBZHL_API bool Destroy(bool Immediate);
	
};

struct PlayerManager;
struct Entity_Player;

struct PlayerManager
{
	LIBZHL_API Entity_Player *SpawnCoPlayer(int unk);
	LIBZHL_API Entity_Player *SpawnCoPlayer2(int unk);
	
};

struct Manager
{
	LIBZHL_API void __stdcall Update();
	
};

struct Room
{
	LIBZHL_API float __stdcall GetDevilRoomChance();
	
};

struct Vector;
struct Room;
struct Camera;

struct Camera
{
	LIBZHL_API void constructor(Room *room);
	LIBZHL_API void SetFocusPosition(const Vector &pos);
	
};

struct LuaEngine;

struct LuaEngine
{
	LIBZHL_API void Init(bool Debug);
	LIBZHL_API void RegisterClasses();
	
	char pad0[24];
	lua_State *_state;
};

struct Weapon_MultiShotParams
{
	int16_t numTears;
	int16_t unk1;
	float unk2;
	float unk3;
	float unk4;
	float unk5;
	int32_t unk6;
	float unk7;
	bool unk8;
	bool unk9;
	bool unk10;
	char pad0;
	int16_t unk11;
	char pad1[2];
};

struct Game;

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
	LIBZHL_API Entity_Player *GetPlayer(unsigned int Index);
	LIBZHL_API void __stdcall Update();
	
	int _floorNum;
	bool _altFloor;
	char unk1[3];
	int _curses;
};

struct LIBZHL_INTERFACE Entity_Player : Entity
{
	virtual ~Entity_Player() {}
	LIBZHL_API virtual void Init(unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
	virtual void PreUpdate() LIBZHL_PLACEHOLDER
	LIBZHL_API virtual void Update();
	LIBZHL_API void AddCollectible(int type, int charge, bool firsttime, int slot, int vardata);
	LIBZHL_API void AddBombs(int amount);
	LIBZHL_API void AddKeys(int amount);
	LIBZHL_API void AddJarFlies(int amount);
	LIBZHL_API void AddPrettyFly();
	LIBZHL_API void AddCoins(int amount);
	LIBZHL_API static PosVel __cdecl GetMultiShotPositionVelocity(int loopIndex, WeaponType weaponType, Vector shotDirection, float shotSpeed, Weapon_MultiShotParams multiShotParams);
	LIBZHL_API Weapon_MultiShotParams GetMultiShotParams(WeaponType weaponType);
	LIBZHL_API virtual bool TakeDamage(float Damage, unsigned __int64 DamageFlags, EntityRef *Source, int DamageCountdown);
	
};

struct GridEntity;

LIBZHL_API GridEntity *__stdcall CreateGridEntity(GridEntityType type, unsigned int seed);
LIBZHL_API unsigned int __stdcall Random();

extern LIBZHL_API Game **__ptr_g_Game;
#define g_Game (*__ptr_g_Game)
extern LIBZHL_API LuaEngine **__ptr_g_LuaEngine;
#define g_LuaEngine (*__ptr_g_LuaEngine)
extern LIBZHL_API Manager **__ptr_g_Manager;
#define g_Manager (*__ptr_g_Manager)


