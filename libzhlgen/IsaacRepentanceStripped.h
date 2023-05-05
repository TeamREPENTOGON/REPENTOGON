struct Globals
{
    
};

struct VTable_Entity
{
	void (__thiscall *Free)(Entity *);
    void (__thiscall *Init)(Entity *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
    void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity *);
};

struct VTable_EntityPlayer
{
	void (__thiscall *Free)(Entity_Player *);
	void (__thiscall *Init)(Entity_Player *, unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed);
	void (__thiscall *PreUpdate)(Entity *);
    void (__thiscall *Update)(Entity_Player *);
};

struct KAGE_File
{
    void* vtable;
    uint32_t unk1;
    FILE* f;
    uint32_t unk2;
};