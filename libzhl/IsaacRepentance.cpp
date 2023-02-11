#include "IsaacRepentance.h"
#include "HookSystem_private.h"

namespace _func0
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("Camera::constructor", typeid(void (Camera::*)(Room *)), "538bdc83ec0883e4f883c404558b6b04896c24048bec6aff68????????64a100000000505383ec285657a1????????33c5508d45f464a3000000008bf18975d46a3ce8????????", argdata, 2, 5, &func);
}

__declspec(naked) void Camera::constructor(Room *room)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// room
		call _func0::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func1
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("Camera::SetFocusPosition", typeid(void (Camera::*)(const Vector &)), "558bec51568bf1578b7e088b4e043bf975??85ff", argdata, 2, 5, &func);
}

__declspec(naked) void Camera::SetFocusPosition(const Vector &pos)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// pos
		call _func1::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func2
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Entity::constructor", typeid(void (Entity::*)()), "558bec6aff68????????64a1000000005083ec085657a1????????33c5508d45f464a3000000008bf9897df0c707????????c7470800000000", argdata, 1, 5, &func);
}

__declspec(naked) void Entity::constructor()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func2::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func3
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity::Init", typeid(void (Entity::*)(unsigned int , unsigned int , unsigned int , unsigned int )), "558bec83ec308b4508f30f7e05????????568bf1578946288b450c", argdata, 5, 5, &func);
}

__declspec(naked) void Entity::Init(unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// initSeed
		push [ebp+16]		// subtype
		push [ebp+12]		// variant
		push [ebp+8]		// type
		call _func3::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func4
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Entity::Update", typeid(void (Entity::*)()), "558bec83e4f081ecd8010000a1????????33c4", argdata, 1, 5, &func);
}

__declspec(naked) void Entity::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func4::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func5
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x2ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity::TakeDamage", typeid(bool (Entity::*)(float , unsigned __int64 , EntityRef *, int )), "558bec83e4??83ec??5356578bf9837f28", argdata, 5, 1, &func);
}

__declspec(naked) bool Entity::TakeDamage(float Damage, unsigned __int64 DamageFlags, EntityRef *Source, int DamageCountdown)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+24]		// DamageCountdown
		push [ebp+20]		// Source
		push [ebp+16]		// DamageFlags
		push [ebp+12]		// DamageFlags
		push [ebp+8]		// Damage
		call _func5::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 20
	}
}

namespace _func6
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddCollectible", typeid(void (Entity_Player::*)(int , int , bool , int , int )), "558bec6a??68????????64a1????????5081ec????????a1????????33c58945f05657508d45f464a3????????8bf18975bc", argdata, 6, 5, &func);
}

__declspec(naked) void Entity_Player::AddCollectible(int type, int charge, bool firsttime, int slot, int vardata)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+24]		// vardata
		push [ebp+20]		// slot
		push [ebp+16]		// firsttime
		push [ebp+12]		// charge
		push [ebp+8]		// type
		call _func6::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 20
	}
}

namespace _func7
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddBombs", typeid(void (Entity_Player::*)(int )), "558bec83e4??8b550883ec??56", argdata, 2, 5, &func);
}

__declspec(naked) void Entity_Player::AddBombs(int amount)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// amount
		push ecx			// this
		call _func7::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func8
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddKeys", typeid(void (Entity_Player::*)(int )), "558bec83ec??8b45088d", argdata, 2, 5, &func);
}

__declspec(naked) void Entity_Player::AddKeys(int amount)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// amount
		push ecx			// this
		call _func8::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func9
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddJarFlies", typeid(void (Entity_Player::*)(int )), "558bec8b91????????b8", argdata, 2, 5, &func);
}

__declspec(naked) void Entity_Player::AddJarFlies(int amount)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// amount
		push ecx			// this
		call _func9::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func10
{
	static void *func = 0;
	static short argdata[] = {0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddPrettyFly", typeid(void (Entity_Player::*)()), "568bf18b86????????ff", argdata, 1, 5, &func);
}

__declspec(naked) void Entity_Player::AddPrettyFly()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push ecx			// this
		call _func10::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func11
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::AddCoins", typeid(void (Entity_Player::*)(int )), "558bec83ec??5356578b7d088bd985", argdata, 2, 5, &func);
}

__declspec(naked) void Entity_Player::AddCoins(int amount)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// amount
		push ecx			// this
		call _func11::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func12
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff, 0x1ff, 0x2ff, 0x1ff, 0x9ff};
	static FunctionDefinition funcObj("Entity_Player::GetMultiShotPositionVelocity", typeid(PosVel (*)(int , WeaponType , Vector , float , Weapon_MultiShotParams )), "558bec83e4??83ec??8b4d", argdata, 6, 2, &func);
}

__declspec(naked) PosVel Entity_Player::GetMultiShotPositionVelocity(int loopIndex, WeaponType weaponType, Vector shotDirection, float shotSpeed, Weapon_MultiShotParams multiShotParams)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+64]		// multiShotParams
		push [ebp+60]		// multiShotParams
		push [ebp+56]		// multiShotParams
		push [ebp+52]		// multiShotParams
		push [ebp+48]		// multiShotParams
		push [ebp+44]		// multiShotParams
		push [ebp+40]		// multiShotParams
		push [ebp+36]		// multiShotParams
		push [ebp+32]		// multiShotParams
		push [ebp+28]		// shotSpeed
		push [ebp+24]		// shotDirection
		push [ebp+20]		// shotDirection
		push [ebp+16]		// weaponType
		push [ebp+12]		// loopIndex
		push [ebp+8]		// implicit_output
		call _func12::func
		add esp, 60
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func13
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::GetMultiShotParams", typeid(Weapon_MultiShotParams (Entity_Player::*)(WeaponType )), "558bec83e4??83ec??8b450853568bf1", argdata, 3, 1, &func);
}

__declspec(naked) Weapon_MultiShotParams Entity_Player::GetMultiShotParams(WeaponType weaponType)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+12]		// weaponType
		push [ebp+8]		// implicit_output
		call _func13::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 8
	}
}

namespace _func14
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::Init", typeid(void (Entity_Player::*)(unsigned int , unsigned int , unsigned int , unsigned int )), "558bec6aff68????????64a1000000005081eca0010000a1????????33c58945f05657508d45f464a3000000008bf989bd74fffffff30f7e05????????", argdata, 5, 5, &func);
}

__declspec(naked) void Entity_Player::Init(unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// initSeed
		push [ebp+16]		// subtype
		push [ebp+12]		// variant
		push [ebp+8]		// type
		call _func14::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func15
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Entity_Player::Update", typeid(void (Entity_Player::*)()), "558bec6aff68????????64a1000000005081ec8c010000a1????????33c58945f0", argdata, 1, 5, &func);
}

__declspec(naked) void Entity_Player::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func15::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func16
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x2ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Entity_Player::TakeDamage", typeid(bool (Entity_Player::*)(float , unsigned __int64 , EntityRef *, int )), "558bec6aff68????????64a1????????5081ec????????a1????????33c58945f05657508d45f464a3????????8bf989bd????????83bf??????????", argdata, 5, 1, &func);
}

__declspec(naked) bool Entity_Player::TakeDamage(float Damage, unsigned __int64 DamageFlags, EntityRef *Source, int DamageCountdown)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+24]		// DamageCountdown
		push [ebp+20]		// Source
		push [ebp+16]		// DamageFlags
		push [ebp+12]		// DamageFlags
		push [ebp+8]		// Damage
		call _func16::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 20
	}
}

namespace _func17
{
	static void *func = 0;
	static short argdata[] = {0x100};
	static FunctionDefinition funcObj("Game::constructor", typeid(void (Game::*)()), "538bdc83ec0883e4f883c404558b6b04896c24048bec6aff68????????64a100000000505383ec105657a1????????33c5508d45f464a3000000008bf9897dec897de468????????", argdata, 1, 5, &func);
}

__declspec(naked) void Game::constructor()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		mov eax, ecx	// this
		call _func17::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func18
{
	static void *func = 0;
	static short argdata[] = {0x100};
	static FunctionDefinition funcObj("Game::IsPaused", typeid(bool (Game::*)()), "83b9??????????8b15", argdata, 1, 1, &func);
}

__declspec(naked) bool Game::IsPaused()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		mov eax, ecx	// this
		call _func18::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func19
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Game::Spawn", typeid(Entity *(Game::*)(unsigned int , unsigned int , const Vector &, const Vector &, Entity *, unsigned int , unsigned int , unsigned int )), "558bec83e4f883ec6c8b450853568b751857894424148bf98d4520897c2410508d451c508b451456508b4510508d450c508d44242c50e8????????", argdata, 9, 1, &func);
}

__declspec(naked) Entity *Game::Spawn(unsigned int type, unsigned int variant, const Vector &position, const Vector &velocity, Entity *spawner, unsigned int subtype, unsigned int seed, unsigned int unk)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+36]		// unk
		push [ebp+32]		// seed
		push [ebp+28]		// subtype
		push [ebp+24]		// spawner
		push [ebp+20]		// velocity
		push [ebp+16]		// position
		push [ebp+12]		// variant
		push [ebp+8]		// type
		call _func19::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 32
	}
}

namespace _func20
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("Game::ShakeScreen", typeid(void (Game::*)(int )), "558beca1????????538b", argdata, 2, 5, &func);
}

__declspec(naked) void Game::ShakeScreen(int timeout)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// timeout
		call _func20::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func21
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Game::MakeShockwave", typeid(void (Game::*)(const Vector &, float , float , int )), "558bec83ec088b45085657f30f1000f30f104804a1????????", argdata, 5, 5, &func);
}

__declspec(naked) void Game::MakeShockwave(const Vector &pos, float amplitude, float speed, int duration)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// duration
		push [ebp+16]		// speed
		push [ebp+12]		// amplitude
		push [ebp+8]		// pos
		call _func21::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func22
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("Game::GetPlayer", typeid(Entity_Player *(Game::*)(unsigned int )), "558bec5156578bf98b97????????8b", argdata, 2, 1, &func);
}

__declspec(naked) Entity_Player *Game::GetPlayer(unsigned int Index)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// Index
		call _func22::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func23
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Game::Update", typeid(void (Game::*)()), "558bec83e4f083ec38a1????????33c4", argdata, 1, 5, &func);
}

__declspec(naked) void Game::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func23::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func24
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Game::AchievementUnlocksDisallowed", typeid(bool (Game::*)()), "568bf180be04bb0100008d8e04bb010075??e8????????84c0", argdata, 1, 1, &func);
}

__declspec(naked) bool Game::AchievementUnlocksDisallowed()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		call _func24::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func25
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("Game::PlaceRoom", typeid(bool (Game::*)(LevelGenerator_Room *, RoomConfig *, unsigned int , int )), "558bec83ec??53568b750c578bf9", argdata, 5, 1, &func);
}

__declspec(naked) bool Game::PlaceRoom(LevelGenerator_Room *room, RoomConfig *data, unsigned int Seed, int dimension)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// dimension
		push [ebp+16]		// Seed
		push [ebp+12]		// data
		push [ebp+8]		// room
		call _func25::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func26
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("Game::UpdateVisibility", typeid(void (Game::*)()), "558bec83e4??515356578bf96a", argdata, 1, 5, &func);
}

__declspec(naked) void Game::UpdateVisibility()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func26::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

Game **__ptr_g_Game;

namespace _var27
{
	static VariableDefinition varObj("g_Game", ".e8????????c745fcffffffffa3(????????)e8", &__ptr_g_Game);
}

namespace _func28
{
	static void *func = 0;
	static short argdata[] = {0x1ff, 0x1ff};
	static FunctionDefinition funcObj("CreateGridEntity", typeid(GridEntity *(*)(GridEntityType , unsigned int )), "558bec6aff68????????64a1000000005051535657a1????????33c5508d45f464a3000000008bda8bf983ff1b0f87????????", argdata, 2, 0, &func);
}

__declspec(naked) GridEntity *__stdcall CreateGridEntity(GridEntityType type, unsigned int seed)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+12]		// seed
		push [ebp+8]		// type
		call _func28::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 8
	}
}

namespace _func29
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("GridEntity_Rock::Update", typeid(void (GridEntity_Rock::*)()), "558bec6aff68????????64a1000000005081ec88000000a1????????33c58945f05657508d45f464a3000000008bf9897da48b57048b0d????????83fa0474??", argdata, 1, 5, &func);
}

__declspec(naked) void GridEntity_Rock::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func29::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func30
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("GridEntity_Rock::Destroy", typeid(bool (GridEntity_Rock::*)(bool )), "558bec83ec40a1????????33c58945fc538b1d????????56", argdata, 2, 1, &func);
}

__declspec(naked) bool GridEntity_Rock::Destroy(bool Immediate)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// Immediate
		call _func30::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func31
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("HUD::Render", typeid(void (HUD::*)()), "538bdc83ec0883e4f883c404558b6b04896c24048bec6aff68????????64a100000000505381ece8010000a1????????33c58945ec5657508d45f464a3000000008bf989bd38ffffff80bfb63e0c0000", argdata, 1, 5, &func);
}

__declspec(naked) void HUD::Render()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func31::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func32
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("HUD::Update", typeid(void (HUD::*)()), "558bec515356578bf980bfbc3e0c000074??ff8fc03e0c00", argdata, 1, 5, &func);
}

__declspec(naked) void HUD::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func32::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func33
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("HUD::PostUpdate", typeid(void (HUD::*)()), "558bec83e4f85153568bd9578bf3bf08000000833e0074??8bcee8????????81c6b0060000", argdata, 1, 5, &func);
}

__declspec(naked) void HUD::PostUpdate()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func33::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func34
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("HUD::LoadGraphics", typeid(void (HUD::*)()), "538bdc83ec0883e4f883c404558b6b04896c24048bec6aff68????????64a100000000505381ec88000000a1????????33c58945ec5657508d45f464a3000000008bf180beb73e0c0000", argdata, 1, 5, &func);
}

__declspec(naked) void HUD::LoadGraphics()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func34::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func35
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("LevelGenerator::PlaceRoom", typeid(int (LevelGenerator::*)(LevelGenerator_Room *)), "558bec83e4??83ec??538b5d08b8????????56", argdata, 2, 1, &func);
}

__declspec(naked) int LevelGenerator::PlaceRoom(LevelGenerator_Room *room)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// room
		call _func35::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func36
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("LevelGenerator::CreateRoom", typeid(LevelGenerator_Room *(LevelGenerator::*)(int , int , int , int , int , Direction )), "558BEC6AFF68????????64A1????????5083EC50A1????????33C58945F05657508D45F464A3????????8BF98B4510508B450883EC088BCC89018B450C8941048D4DA8E8????????508BCFC745??????????", argdata, 7, 1, &func);
}

__declspec(naked) LevelGenerator_Room *LevelGenerator::CreateRoom(int x, int y, int shape, int connectX, int connectY, Direction connectDir)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+28]		// connectDir
		push [ebp+24]		// connectY
		push [ebp+20]		// connectX
		push [ebp+16]		// shape
		push [ebp+12]		// y
		push [ebp+8]		// x
		call _func36::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 24
	}
}

namespace _func37
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("LevelGenerator::Generate", typeid(void (LevelGenerator::*)(int , bool , bool , bool , const unsigned int &, int , LevelGenerator_Room *)), "558bec83ec??8a45", argdata, 8, 5, &func);
}

__declspec(naked) void LevelGenerator::Generate(int unk, bool unk2, bool unk3, bool unk4, const unsigned int &allowedShapes, int unk5, LevelGenerator_Room *startRoom)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+32]		// startRoom
		push [ebp+28]		// unk5
		push [ebp+24]		// allowedShapes
		push [ebp+20]		// unk4
		push [ebp+16]		// unk3
		push [ebp+12]		// unk2
		push [ebp+8]		// unk
		call _func37::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 28
	}
}

namespace _func38
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("LevelGenerator::GetNewEndRoom", typeid(LevelGenerator_Room *(LevelGenerator::*)(int , const unsigned int &)), "558bec83e4??515356578bf98bb7", argdata, 3, 1, &func);
}

__declspec(naked) LevelGenerator_Room *LevelGenerator::GetNewEndRoom(int roomShape, const unsigned int &possibleDoors)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+12]		// possibleDoors
		push [ebp+8]		// roomShape
		call _func38::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 8
	}
}

namespace _func39
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("LuaEngine::Init", typeid(void (LuaEngine::*)(bool )), "558bec83e4??83ec??56578bf9????????ff", argdata, 2, 5, &func);
}

__declspec(naked) void LuaEngine::Init(bool Debug)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// Debug
		call _func39::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func40
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("LuaEngine::RegisterClasses", typeid(void (LuaEngine::*)()), "558bec6aff68????????64a1000000005081ec3c01000053", argdata, 1, 5, &func);
}

__declspec(naked) void LuaEngine::RegisterClasses()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func40::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

LuaEngine **__ptr_g_LuaEngine;

namespace _var41
{
	static VariableDefinition varObj("g_LuaEngine", "a1(????????)8b40", &__ptr_g_LuaEngine);
}

namespace _func42
{
	static void *func = 0;
	static const short *argdata = NULL;
	static FunctionDefinition funcObj("Manager::Update", typeid(void (Manager::*)()), "558bec83e4??83ec??56578b3d", argdata, 0, 4, &func);
}

__declspec(naked) void Manager::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func42::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

Manager **__ptr_g_Manager;

namespace _var43
{
	static VariableDefinition varObj("g_Manager", ".e8????????83c414898544feffff8bc8c745fc05000000e8????????c745fcffffffffa3(????????)e8????????e8????????", &__ptr_g_Manager);
}

namespace _func44
{
	static void *func = 0;
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("PlayerHUD::Update", typeid(void (PlayerHUD::*)()), "538bdc83ec0883e4f883c404558b6b04896c24048bec6aff68????????64a100000000505381ecd8040000a1????????33c58945ec5657508d45f464a300000000", argdata, 1, 5, &func);
}

__declspec(naked) void PlayerHUD::Update()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		call _func44::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func45
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("PlayerHUD::RenderActiveItem", typeid(void (PlayerHUD::*)(unsigned int , const Vector &, float , float )), "558bec83e4f081ecc802000056578b39f30f115c241c898c24a0000000897c241880bf881f0000000f85????????8b55088b4904898c24dc0000008d04d5000000002bc2", argdata, 5, 5, &func);
}

__declspec(naked) void PlayerHUD::RenderActiveItem(unsigned int slot, const Vector &pos, float alpha, float unk4)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push eax
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// unk4
		push [ebp+16]		// alpha
		push [ebp+12]		// pos
		push [ebp+8]		// slot
		call _func45::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop eax
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func46
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("PlayerManager::SpawnCoPlayer", typeid(Entity_Player *(PlayerManager::*)(int )), "558bec6aff68????????64a1000000005083ec20535657a1????????33c5508d45f464a3000000008bf9897de08b47142b4710c1f8024083f84076??68????????", argdata, 2, 1, &func);
}

__declspec(naked) Entity_Player *PlayerManager::SpawnCoPlayer(int unk)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// unk
		call _func46::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func47
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff};
	static FunctionDefinition funcObj("PlayerManager::SpawnCoPlayer2", typeid(Entity_Player *(PlayerManager::*)(int )), "558bec6aff68????????64a1000000005083ec10535657a1????????33c5508d45f464a3000000008bf9837d08ff0f85????????6880250000e8????????", argdata, 2, 1, &func);
}

__declspec(naked) Entity_Player *PlayerManager::SpawnCoPlayer2(int unk)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+8]		// unk
		call _func47::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 4
	}
}

namespace _func48
{
	static void *func = 0;
	static const short *argdata = NULL;
	static FunctionDefinition funcObj("Random", typeid(unsigned int (*)()), "a1????????3d70020000", argdata, 0, 0, &func);
}

__declspec(naked) unsigned int __stdcall Random()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		call _func48::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func49
{
	static void *func = 0;
	static const short *argdata = NULL;
	static FunctionDefinition funcObj("Room::GetDevilRoomChance", typeid(float (Room::*)()), "558bec83ec08f30f1005????????56578bf9f30f1145fc8b0d????????", argdata, 0, 0, &func);
}

__declspec(naked) float Room::GetDevilRoomChance()
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		call _func49::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		ret
	}
}

namespace _func50
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("RoomConfigHolder::GetRoomByStageTypeAndVariant", typeid(RoomConfig *(RoomConfigHolder::*)(uint32_t , uint32_t , uint32_t , int32_t )), "558bec518b55145356578b7d0869f7c4000000", argdata, 5, 1, &func);
}

__declspec(naked) RoomConfig *RoomConfigHolder::GetRoomByStageTypeAndVariant(uint32_t stage, uint32_t type, uint32_t variant, int32_t difficulty)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+20]		// difficulty
		push [ebp+16]		// variant
		push [ebp+12]		// type
		push [ebp+8]		// stage
		call _func50::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 16
	}
}

namespace _func51
{
	static void *func = 0;
	static short argdata[] = {0x101, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};
	static FunctionDefinition funcObj("RoomConfigHolder::GetRandomRoom", typeid(RoomConfig *(RoomConfigHolder::*)(unsigned int , bool , int , int , int , unsigned int , unsigned int , int , int , const unsigned int &, unsigned int , int )), "558bec6aff68????????64a1????????5083ec74a1????????33c58945f0535657508d45f464a3????????894dd08b45088945b88a450c8845cf88", argdata, 13, 1, &func);
}

__declspec(naked) RoomConfig *RoomConfigHolder::GetRandomRoom(unsigned int seed, bool reduceWeight, int stage, int roomType, int roomShape, unsigned int unk1, unsigned int unk2, int minDifficulty, int maxDifficulty, const unsigned int &requiredDoors, unsigned int roomSubtype, int unk3)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push edx
		push ecx
		push ebx
		push esi
		push edi
		push [ebp+52]		// unk3
		push [ebp+48]		// roomSubtype
		push [ebp+44]		// requiredDoors
		push [ebp+40]		// maxDifficulty
		push [ebp+36]		// minDifficulty
		push [ebp+32]		// unk2
		push [ebp+28]		// unk1
		push [ebp+24]		// roomShape
		push [ebp+20]		// roomType
		push [ebp+16]		// stage
		push [ebp+12]		// reduceWeight
		push [ebp+8]		// seed
		call _func51::func
		pop edi
		pop esi
		pop ebx
		pop ecx
		pop edx
		mov esp, ebp
		pop ebp
		retn 48
	}
}

