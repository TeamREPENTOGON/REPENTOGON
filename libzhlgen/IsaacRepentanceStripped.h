struct Manager;
struct Game;
struct Vector;
struct PlayerManager;
struct Entity;
struct Entity_Player;
struct GridEntity;
struct GridEntity_Rock;
struct Room;
struct Camera;
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
	
	char pad0[752];
	float _timeScale;
};


struct EntityRef
{
  int  _type; //TODO: EntityType enum
  int _variant;
  int _spawnerType; //TODO: ditto
  unsigned int _spawnerVariant;
  Vector _position;
  Vector _velocity;
  unsigned int _flags;
  Entity *_entity;
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
	VTable_EntityPlayer *_vtable;
	Entity _entity;
};

struct VTable_EntityPlayer
{
	void (__thiscall *Free)(Entity_Player *);
	void (__thiscall *Init)(Entity_Player *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
	void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity_Player *);
};

struct GridEntity {
	int _unk; 
	GridEntityType _type; //type?
	int _variant;
	int _state;
	int _unk2;
	int _varData;
	int _unk3;
	
};

struct GridEntity_Rock {
	GridEntity _base;
};


struct Entity_Slot
{
	Entity _entity;
};

struct Camera {
	
};

struct LuaEngine
{
	char pad0[24];
	lua_State* _state; // 0x18
};

struct PosVel 
{
    Vector pos;
    Vector vel;
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