struct Manager;
struct Game;
struct Vector;
struct PlayerManager;
struct PersistentGameData;
struct Music;
struct Entity;
struct Entity_Player;
struct GridEntity;
struct GridEntity_Rock;
struct Room;
struct Level;
struct Camera;
struct HUD;
struct PlayerHUD;
struct VTable_Entity;
struct VTable_EntityPlayer;

/* 
enum eStages 
{
    MAX_STAGES = 36
};
*/

struct Globals
{
    
};

struct Manager
{
	char unk1[0x13];
	PersistentGameData _persistentGameData;
};

struct PersistentGameData
{
	
};

struct CompletionWidget 
{
	
};

struct Menu_Character 
{
	int status; //1 seeds screen //4 transitioning to tainted
	int lockedflags; //I think? 65792 is unlocked 65536 is locked
	int difficulty;
	int unk4;
	int charaslot;
	float unk5; //dunno if these are floats even just using them as paddiong (they are usually 0)
	float unk6; //dunno if these are floats even just using them as paddiong (they are usually 0)
	int unk7;
	int unk8;
};

struct Room
{
    
};

struct RoomConfig 
{
    int32_t StageId; // 0x0
    int32_t Mode; // 0x4
    int32_t Type; // 0x8
    int32_t Variant; // 0xC
    int32_t Subtype; // 0x10
    const char* Name; // 0x14
    int32_t unk1; // 0x18
    int32_t unk2; // 0x1C
    int32_t unk3; // 0x20
    int32_t unk4; // 0x24
    int32_t unk5; // 0x28
    int32_t unk6; // 0x2C
    float InitialWeight; // 0x30
    float Weight; // 0x34
    int32_t Doors; // 0x38
    int32_t unk7; // 0x3C
    int32_t unk8; // 0x40
    int16_t SpawnCount; // 0x44
    int8_t Width; // 0x46
    int8_t Height; // 0x47
    int32_t Shape; // 0x48
    int32_t unk9; // 0x4C
    int32_t unk10; // 0x50
    int32_t unk11; // 0x54
    int32_t unk12; // 0x58
};

union uIsaacString
{
    char small_string[16];
    char* long_string;
};

struct IsaacString 
{
    uIsaacString value;
    uint32_t size; // 0x10
    uint32_t unk; // 0x14
}; // 0x18

struct RoomConfigs
{
    uint32_t stageId; // 0x0
    IsaacString stageName; // 0x4
    IsaacString playerSpot; // 0x1C
    IsaacString bossSpot; // 0x34
    IsaacString suffix; // 0x4C
    IsaacString xmlFileName; // 0x64
    RoomConfig* configs; // 0x7C
    uint32_t nbRooms; // 0x80 (includes disabled rooms)
    void* unk1; // 0x84
    uint32_t unk2; // 0x88
    uint8_t unk3; // 0x8C, flag for Greed Mode ? Accessed through byte ptr ds:[]
    char unk4[0x3]; // 0x8D, padding ?
    IsaacString greedModeXmlFileName; // 0x90
    char unk5[0x8]; // 0xA8
    void* unk6; // 0xB0
    char unk7[0x8]; // 0xB4
    uint32_t music; // 0xBC
    uint32_t backdrop; // 0xC0
}; // 0xC4

struct RoomConfigHolder 
{
    RoomConfigs configs[36];
}; // 0xC4 * 36

struct PlayerManager
{
	
};


struct RoomDescriptor 
{
    int32_t GridIndex; // 0x0
    int32_t SafeGridIndex; // 0x4
    int32_t ListIndex; // 0x8
    int32_t unk0; // 0xC
    RoomConfig* Data; // 0x10
    RoomConfig* OverrideData; // 0x14
    int32_t AllowedDoors; // 0x18
    int32_t Doors[8]; // 0x1C
    int32_t DisplayFlags; // 0x3C
    int32_t VisitedCount; // 0x40
    int32_t Flags; // 0x44
    int16_t unk9; // 0x48
    int16_t ClearCount; // 0x4A
    int32_t unk10; // 0x4C
    int32_t PoopCount; // 0x50
    int32_t PitsCount; // 0x54
    int32_t DecorationSeed; // 0x58
    int32_t SpawnSeed; // 0x5C
    int32_t AwardSeed; // 0x60
    int32_t unk11; // 0x64
    int32_t unk12; // 0x68
    int32_t unk13; // 0x6C
    int32_t unk14; 
    int32_t unk15; 
    int32_t unk16; 
    int32_t unk17; // 0x70 -> 0x80
    int32_t unk18; 
    int32_t unk19; 
    int32_t unk20; 
    int32_t unk21; // 0x80 -> 0x90
    int32_t unk22; 
    int32_t unk23; 
    int32_t unk24; 
    int32_t unk25; // 0x90 -> 0xA0
    int32_t unk26; // 0xA0
    int32_t unk27; // 0xA4
    int32_t unk28; // 0xA8
    int16_t ShopItemIdx; // 0xAC
    int16_t ShopItemDiscountIdx; // 0xAE
    int32_t DeliriumDistance; // 0xB0
    int32_t unk29; // 0xB4
}; // 0xB8 (Checked in assembly)

struct Game 
{
	uint32_t _stage; // 0x0
	uint32_t _stageType; // 0x4
	uint32_t unk; // 0x8
	uint32_t _curses; // 0xC
    RoomDescriptor _gridRooms[507]; // 0x14
    RoomDescriptor _negativeGridRooms[18]; // 0x16c7c
    uint32_t _roomOffset[507]; // 0x1796c
    uint32_t _nbRooms; // 0x1815C
    uint32_t _startingRoomIdx; // 0x18160
    Room* _room; // 0x18190
    uint32_t _currentRoomIdx; // 0x18194;
    uint32_t _lastRoomIdx; // 0x18198
    uint32_t _currentDimensionIdx; // 0x1819C
    uint32_t _lastRoomDimensionIdx; // 0x181A0
    uint32_t _leaveDoor; // 0x181A8
    uint32_t _enterDoor; // 0x181AC
    uint32_t _greedModeTreasureRoomIdx; // 0x18200
    RoomConfigHolder _roomConfigs; // 0x1879C
    PlayerManager _playerManager; // 0x1AB40
    uint32_t _difficulty; // 0x243624
};

typedef Game Level;

struct Vector
{
	float x;
	float y;
};


struct Entity 
{
	VTable_Entity *_vtable; //0x0, len 0x8
	char pad0[357];
	bool _visible; // 0x169 
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

struct GridEntity 
{
	int _unk; 
	GridEntityType _type; //type?
	int _variant;
	int _state;
	int _unk2;
	int _varData;
	int _unk3;
	
};

struct GridEntity_Rock 
{
	GridEntity _base;
};


struct Entity_Slot
{
	Entity _entity;
};

struct Camera 
{
	
};

struct HUD 
{
	
};

struct PlayerHUD 
{
	HUD _base;
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

struct LevelGenerator
{
    
};

struct Level
{
    
};

struct LevelGenerator_Room
{
    char pad0[0x8]; // 0x0
    uint32_t _gridColIdx; // 0x8
    uint32_t _gridLineIdx; // 0xC
    char pad1[0xC]; // 0x10
    uint32_t _doors; // 0x1C
};

struct RNG
{
  unsigned int _seed;
};

struct ModManager
{
};

struct ANM2
{
};

struct SFXManager
{
};
struct Music 
{
};