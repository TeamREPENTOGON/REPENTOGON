struct Manager;
struct Game;
struct Entity;
struct VTable_Entity;

struct Globals
{
};

struct Manager
{
	
};

struct Game 
{
	
};

struct Entity 
{
	VTable_Entity *_vtable;
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

/*struct LuaEngine
{
	int _asdfg; // 0x0
	int _callbackStates; // 0x4
	int _callbackCallFunc; // 0x8
	int _unloadmodFunc; // 0xC
	// lua_State _state; // 0x10
	char unk0[4]; // 0x10
	bool _debug; // 0x14
	char pad0[3]; // 0x15
	lua_State* _state; // 0x18
	unsigned int _hashKey; // Originally misaligned
	int _GCStepSize;
	int _GCMemTrail[10];
	int _GCMemTrailPointer;
	int _GCStepCooldown;
	int _lastMemSize;
	unsigned int m_MemHistory[128];
	unsigned int m_MemHistoryTail;
	unsigned int m_MemHistorySize;
	unsigned int m_MemHistoryCountdown;
};
*/