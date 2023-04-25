struct Manager;
struct Game;
struct ANM2;
struct Vector;
struct NullFrame;
struct PlayerManager;
struct MenuManager;
struct PauseScreen;
struct PersistentGameData;
struct Music;
struct Entity;
struct EntityConfig;
struct Entity_Player;
struct History;
struct ColorMod;
struct Entity_NPC;

struct Entity_Pickup;
struct ModEntry;
struct Entity_Familiar;
struct GridEntity;
struct GridEntity_Rock;
struct Room;
struct Level;
struct Camera;
struct Ambush;
struct BossPool;
struct HUD;
struct PlayerHUD;
struct VTable_Entity;
struct VTable_EntityPlayer;
struct ShapeRenderer;

struct RapidXML;
struct xmlnode_rep;
struct xmldocument_rep;

/* 
enum eStages 
{
    MAX_STAGES = 36
};
*/

struct StaticArray
{
    void* data;
    uint16_t size;
};

struct Globals
{
    
};

struct Manager
{
	char unk1[0x13];
	PersistentGameData _persistentGameData;
	StringTable _stringTable; //0x4a234
};

struct ANM2
{
	IsaacString _filename;
	char _pad1[27];
	AnimationState _animState;
	AnimationState _overlayAnimState;
};

struct LayerState
{
	//char pad[0x90];
	//float _cropY;
	char pad[0x74];
	bool _something; 
	//0x74
};

struct NullFrame {
	
};

struct AnimationState
{
	
};

struct MenuManager
{
	
};

struct PersistentGameData
{
	
};

struct ModEntry
{

};

struct EntityConfig
{
	
};

struct CompletionWidget 
{
	int CharacterId;
	//ANM2* CompletionMarkAnm; //added a getspritefunction with a +4 offset, couldnt get this shit to work
};

struct Menu_Title
{
	
};

struct Menu_Game
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

struct IsaacString 
{
    char text[16];
    uint32_t size; // 0x10
    uint32_t unk; // 0x14
}; // 0x18

struct RoomEntry
{
    uint32_t type; // 0x0
    uint16_t variant; // 0x4
    uint16_t subtype; // 0x6
    float weight; // 0x8
}; // 0xC

struct RoomSpawn 
{
    uint16_t X; // 0x0
    uint16_t Y; // 0x2
    StaticArray Entries;
    char pad[2]; // 0xA
    float SumWeights; // 0xC
}; // 0x10 (absolutely sure)

struct RoomConfig 
{
    int32_t StageId; // 0x0
    int32_t Mode; // 0x4
    int32_t Type; // 0x8
    int32_t Variant; // 0xC
    int32_t Subtype; // 0x10
    IsaacString Name; // 0x14
    int32_t Difficulty; // 0x2C
    float InitialWeight; // 0x30
    float Weight; // 0x34
    int32_t Doors; // 0x38
    int32_t unk7; // 0x3C
    StaticArray Spawns; // 0x40
    int8_t Width; // 0x46
    int8_t Height; // 0x47
    int32_t Shape; // 0x48
    int32_t unk9; // 0x4C
    int32_t unk10; // 0x50
    int32_t unk11; // 0x54
    int32_t unk12; // 0x58
}; // 0x5C

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

struct RNG
{
  unsigned int _seed; //0x0
  char pad0[0xC]; // probably ShiftIdx?
}; //0x10

struct PlayerManager
{
	RNG _rng; //0x0
};

struct Console_HistoryEntry
{
	IsaacString _text;
};

struct Console
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



struct EntityList_EL
{
	bool _sublist;
	Entity** _data;
	unsigned int _capacity;
	unsigned int _size;
};

struct EntityList
{
	
};

struct Room
{
	char pad0[0x4];
	RoomDescriptor* _descriptor; //0x04
    char pad1[0x1C];	
    GridEntity* _gridEntities[0x1BF]; // 0x24
    char pad2[0x4]; // 0x720
    GridEntity* _doors[8]; // 0x724
    uint32_t _doorGridPositions[8]; // 0x744
	Camera *_Camera; //0x11F8
	bool _redHeartDamage; //0x120C
	EntityList _entityList; //0x1218
	TemporaryEffects _temporaryEffects; //0x71F4
};

struct DailyChallenge
{
	unsigned int _id; //0x0
	unsigned int _seed; //0x4
};

struct Vector
{
	float x;
	float y;
};


struct Entity 
{
	VTable_Entity *_vtable; //0x0, len 0x8
	char pad0[28];
	unsigned int _index; //0x20
	unsigned int _collisionIndex; //0x24
	unsigned int _type; //0x28
	unsigned int _variant; //0x2c
	unsigned int _subtype; //0x30
	unsigned int _shadowSize; //0x150   
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

struct VTable_Entity
{
	void (__thiscall *Free)(Entity *);
    void (__thiscall *Init)(Entity *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
    void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity *);
};


struct Capsule 
{
	
};

struct Entity_Player
{
	VTable_EntityPlayer *_vtable;
	Entity _entity;
};

struct ActiveItemDesc
{
	int _item;
	int _charge;
	int _batteryCharge;
	int _subCharge;
	int _timeRechargeCooldown;
	int _partialCharge;
	int _varData;
	
};

struct History
{
	
};

struct ItemConfig_Item
{
	int _type; //0x0
	int _id; //0x4
};

struct TemporaryEffect
{
	ItemConfig_Item *_item;
	int _unk;
	int _unk2;
};

struct TemporaryEffects
{
	int _unk;
	std::vector<TemporaryEffect> _effects;
	int _unk2;
	int _disabled;
	Entity_Player* _player;
};

struct Entity_Pickup
{
	Entity _entity;
};

struct Entity_Familiar
{
	Entity _entity;
};

struct VTable_EntityPlayer
{
	void (__thiscall *Free)(Entity_Player *);
	void (__thiscall *Init)(Entity_Player *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
	void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity_Player *);
};

struct VTable_GridEntity
{
    
};

struct Entity_Tear 
{
	Entity _entity;
};

struct Entity_Projectile
{
	Entity _entity;
};

struct Entity_Knife
{
	Entity _entity;
};

struct Entity_Effect
{
	Entity _entity;
};

struct Entity_Bomb
{
	Entity _entity;
};

struct Entity_Slot
{
	Entity _entity;
};

struct Entity_Pickup
{
	Entity _entity;
};

struct Entity_NPC
{
	Entity _entity;
};

struct GridEntityDesc
{
	int _type;
	int _variant;
	int _state;
	int _spawnCount;
	unsigned int _spawnSeed;
	int _varData;
	bool _initialized;
	unsigned int _variableSeed;

};

struct GridEntity 
{
    VTable_GridEntity* _vtable;
	int _unk; 
	GridEntityType _type; // 0x4
	int _variant; // 0x8
	int _state;
	int _unk2;
	int _varData;
	int _unk3;
	unsigned int _collisionClass; // 0x3C
	
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

struct Ambush
{
	
};

struct BossPool
{
};

struct HUD 
{
	PlayerHUD _playerHUD[7];
};

struct PlayerHUD 
{
	Entity_Player* _player;
};

struct RunCallbackRegistry
{
    lua_State* state; // 0x0
    int32_t key; // 0x4
};

struct LuaEngine
{
	char pad0[0x10];
    RunCallbackRegistry* runCallbackRegistry; // 0x10
    char pad1[0x4]; // 0x14
	lua_State* _state; // 0x18
    int32_t _runCallbackRegistryKey; // 0x1C
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

struct ModManager
{

};

struct MenuManager
{
};

struct PauseScreen
{
	int status; //0 closed 1 main pause screen 2 options menu
};

struct Minimap
{

};

struct SFXManager
{

};

struct Music 
{

};

struct Isaac
{
	
};

struct ChallengeParam
{
	IsaacString _name; //0x0
	std_set_int _roomset; //0x18
};

struct ProceduralItemManager
{
	
};

struct Game 
{
	uint32_t _stage; // 0x0
	uint32_t _stageType; // 0x4
	bool _bossChallenge; //0x8
	bool _devilRoomDisabled; //0x9
	char _pad0[2]; // probably nothing?
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
	uint32_t _lastBossRoomListIdx; //0X181A4
    uint32_t _leaveDoor; // 0x181A8
    uint32_t _enterDoor; // 0x181AC
	uint32_t _greedModeWave; // 0x181C4
	unsigned int _stateFlags; // 0x1822C
    uint32_t _greedModeTreasureRoomIdx; // 0x18200
    RoomConfigHolder _roomConfigs; // 0x1879C
    PlayerManager _playerManager; // 0x1AB40 
	Console _console; // 0x1BB60
	ItemOverlay _itemOverlay; // 0x1BCC0
	int _treasureRoomsVisited; // 0x1C3178
	int _planetariumsVisited; // 0x1C317C
	Ambush _ambush; // 0x1c31b8
    uint32_t _difficulty; // 0x243624
	ProceduralItemManager _proceduralItemManager; // 0x283c20
};

typedef Game Level;

struct ColorMod
{	
	float _tint[4]; //0x0
	float _colorize[4]; //0x10
	float _offset[3]; //0x20
}; //0x2C

struct StringTable
{
	uint32_t language;
};

struct ItemOverlay
{
	
};

struct NPCAI_Pathfinder 
{
	
};

struct KColor
{
    float _red;
    float _green;
    float _blue;
    float _alpha;
};

struct DrawStringEntry
{
    const char* _text; // 0x0
    float _x; // 0x4
    float _y; // 0x8
    KColor _color; // 0xC
    int _boxWidth; // 0x1C
    bool _center; // 0x20
}; // 0x24 (not confirmed)

struct DrawStringScaledEntry
{
    const char* _text; // 0x0
    float _x; // 0x4
    float _y; // 0x8
    float _scaleX; // 0xC
    float _scaleY; // 0x10
    KColor _color; // 0x1C
    int _boxWidth; // 0x24
    bool _center; // 0x28
}; // 0x2C (not confirmed)

struct Font
{
    
};

struct Backdrop {
    
};

struct KAGE_File
{
    void* vtable;
    uint32_t unk1;
    FILE* f;
    uint32_t unk2;
};

struct UnknownGameStartStruct {
	char pad0[0x5c]; //ANYONE KNOW WHAT THIS IS???? ANYONE????????
};

struct RoomTransition {
};

struct DestinationQuad {
	Vector _topLeft;
	Vector _topRight;
	Vector _bottomLeft;
	Vector _bottomRight;
};

struct ShapeRenderer {
};