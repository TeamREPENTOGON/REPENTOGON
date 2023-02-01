struct Manager;
struct Game;
struct Vector;
struct PlayerManager;
struct Entity;
struct Entity_Player;
struct Room;
struct VTable_Entity;
struct VTable_EntityPlayer;

struct Globals
{
};

struct Manager
{
	
};

struct Game 
{
	int _floorNum;
	bool _altFloor;
	char unk1[0x03];
	int _curses;
};

struct Vector
{
	float x;
	float y;
};

struct PlayerManager
{
	
};

struct Entity 
{
	VTable_Entity *_vtable;
};

struct Room
{
	
};

struct VTable_Entity
{
	void (__thiscall *Free)(Entity *);
    void (__thiscall *Init)(Entity *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
    void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity *);
};

struct Entity_Player
{
	Entity _entity;
};

struct Entity_Slot
{
	Entity _entity;
};

struct LuaEngine
{
	char pad0[24];
	lua_State* _state; // 0x18
};
