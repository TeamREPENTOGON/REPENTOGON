#include "IsaacRepentance.h"
#include "HookSystem_private.h"

namespace _func0
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
		call _func0::func
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

namespace _func1
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
		call _func2::func
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

namespace _func3
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
		call _func3::func
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

namespace _func4
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
		call _func5::func
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

namespace _func6
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
		call _func6::func
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

namespace _func7
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

Game **__ptr_g_Game;

namespace _var8
{
	static VariableDefinition varObj("g_Game", ".e8????????c745fcffffffffa3(????????)e8", &__ptr_g_Game);
}

namespace _func9
{
	static void *func = 0;
	static const short *argdata = NULL;
	static FunctionDefinition funcObj("Manager::Update", typeid(void (*)()), "558bec83e4??83ec??56578b3d", argdata, 0, 6, &func);
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
		call _func9::func
		add esp, 0
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

namespace _var10
{
	static VariableDefinition varObj("g_Manager", ".e8????????83c414898544feffff8bc8c745fc05000000e8????????c745fcffffffffa3(????????)e8????????e8????????", &__ptr_g_Manager);
}

namespace _func11
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
	static short argdata[] = {0x101};
	static FunctionDefinition funcObj("LuaEngine::RegisterClasses", typeid(void (LuaEngine::*)()), "558bec6aff68????????64a1000000005081ec????????535657a1????????33c5508d45f464a3000000008bd98b53188d4de4e8????????5068????????8d4decc745fc00000000e8????????6a00c645fc018b354462a00068????????ff75ecffd68b4dec83c40cba????????68????????e8????????8b4dec83c404ba????????68????????e8????????83c4048d4dec68????????68????????e8????????508d4dcce8????????c645fc028d4dccff75d0e8????????c645fc038d4decff75f0e8????????c745fc040000008d4de4ff75e8e8????????c745fc????????8d4de48b5318e8???????? ", argdata, 1, 5, &func);
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
		call _func12::func
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

namespace _var13
{
	static VariableDefinition varObj("g_LuaEngine", "a1(????????)8b40", &__ptr_g_LuaEngine);
}

