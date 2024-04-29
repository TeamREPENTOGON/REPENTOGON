#include "ASMPatcher.hpp"
#include "libzhl.h"
#include "HookSystem.h"
#include "HookSystem_private.h"
#include "MiscFunctions.h"
#include "SigScan.h"
#include "lua.hpp"

#include <stdio.h>
#include <glad/glad.h>

#include "REPENTOGONOptions.h"
#include "Patches/ASMPatches.h"
#include "LuaInterfaces/LuaRender.h"

/********************************************************************************
HOOKING

The following macros are used to declare function hooks:
	HOOK_METHOD
	HOOK_STATIC
	HOOK_GLOBAL


The syntax of HOOK_METHOD and HOOK_STATIC is as follows:
	HOOK_METHOD(class name, function name, (arguments) -> return type)
	{
		function body
	}

All hookable class methods are preceded by LIBZHL_API in Isaac.h
If the method is static, use HOOK_STATIC, otherwise use HOOK_METHOD

Writing a hook for a non static class method works pretty much like writing a method
for that class, meaning that the "this" pointer is available and you can access class
members directly without using . or ->

All hooks give access to a special function named super(), which calls the original function


The syntax of HOOK_GLOBAL is pretty much the same, just without a class name:
	HOOK_GLOBAL(function name, (arguments) -> return type)
	{
		function body
	}

It is used to hook into global functions that don't belong to any class

super() doesn't necessarily need to receive the original arguments passed to the hooked function, in fact you can even decide
not to call it all if you want to completely overwrite the behavior of the function in certain cases


When more than one hook are attached to the same function, the last created hook will be executed first, and calling super() in that hook
will call the previous hook instead of the original function:

	Hook2
		code before super()
		Hook1
			code before super()
			original code
			code after super()
		end of Hook 1
		code after super()
	end of Hook 2

You can control the order in which hooks are called by giving them a priority number using the following macros:
	HOOK_METHOD_PRIORITY(class name, function name, priority, (arguments) -> return type)
	HOOK_STATIC_PRIORITY(class name, function name, priority, (arguments) -> return type)
	HOOK_GLOBAL_PRIORITY(function name, priority, (arguments) -> return type)

Hooks with a lower priority number will run first (yeah it's kinda backwards, sorry about that), while hooks with
a high priority number will run last, right before the original function is called
The default priority is 0, and priority numbers can be negative

********************************************************************************/

// Taken from lua.h in Lua 5.3.3
struct override_lua_Debug {
	int event;
	const char* name;	/* (n) */
	const char* namewhat;	/* (n) 'global', 'local', 'field', 'method' */
	const char* what;	/* (S) 'Lua', 'C', 'main', 'tail' */
	const char* source;	/* (S) */
	int currentline;	/* (l) */
	int linedefined;	/* (S) */
	int lastlinedefined;	/* (S) */
	unsigned char nups;	/* (u) number of upvalues */
	unsigned char nparams;/* (u) number of parameters */
	char isvararg;        /* (u) */
	char istailcall;	/* (t) */
	char short_src[LUA_IDSIZE]; /* (S) */
	/* private part */
	struct CallInfo* i_ci;  /* active function */
};

static int __cdecl OverrideGetInfo(lua_State* L, const char* what, override_lua_Debug* ar) {
	lua_Debug debug = *(lua_Debug*)ar;
	int result = lua_getinfo(L, what, &debug);
	ar->event = debug.event;
	ar->name = debug.name;
	ar->namewhat = debug.namewhat;
	ar->what = debug.what;
	ar->source = debug.source;
	ar->currentline = debug.currentline;
	ar->linedefined = debug.linedefined;
	ar->lastlinedefined = debug.lastlinedefined;
	ar->nups = debug.nups;
	ar->nparams = debug.nparams;
	ar->isvararg = debug.isvararg;
	ar->istailcall = debug.istailcall;
	memcpy(ar->short_src, debug.short_src, LUA_IDSIZE);
	ar->i_ci = debug.i_ci;
	return result;
}

static void FixLuaDump()
{
	const char* sig = "558BEC83E4F881ECA4020000"
		"A1????????33C4898424A0020000833D????????00"
		"535657894C24100F848A00000068????????6A00E896290D00"
		"8B3D????????83C4088B1D????????33F60F1F4400008D442420"
		"5056FF7718FFD383C40C85C074568D4424205068????????FF7718FF15????????83C40C";
	SigScan scanner(sig);
	// SigScan scanner("8b442424b9????????85c00f45c88d44244451ff742438");
	if (!scanner.Scan()) {
		fprintf(stderr, "Unable to find Lua write minidump function\n");
		exit(-2);
	}

	const char* protectedBase = (const char*)scanner.GetAddress();
	MEMORY_BASIC_INFORMATION info;
	if (VirtualQuery(protectedBase, &info, sizeof(info)) == 0) {
		fprintf(stderr, "VirtualQuery error %d\n", GetLastError());
		exit(-2);
	}

	DWORD protect;
	if (!VirtualProtect(info.BaseAddress, info.RegionSize, PAGE_READWRITE, &protect)) {
		fprintf(stderr, "VirtualProtect error %d\n", GetLastError());
		exit(-2);
	}

	char* base = const_cast<char*>(protectedBase);
	void* target = (void*)OverrideGetInfo;
	char* getInfoCall = base + (strlen(sig) / 2) - 9; // Base of call lua_getinfo, FF15 ????????
	char buffer[5] = { '\xE8', 0, 0, 0, 0};
	ptrdiff_t diff = (ptrdiff_t)target - (ptrdiff_t)(getInfoCall + 5);
	memcpy(buffer + 1, &diff, sizeof(diff));
	memcpy(getInfoCall, buffer, 5);
	getInfoCall[5] = '\x90';
	/* size_t lineDefinedOffset = 22;
	size_t shortSrcOffset = 17;

	base[lineDefinedOffset] = '\x3C';
	base[shortSrcOffset] = '\x4A'; */

	DWORD dummy;
	if (!VirtualProtect(info.BaseAddress, info.RegionSize, protect, &dummy)) {
		fprintf(stderr, "VirtualProtect error %d\n", GetLastError());
	}

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}

// This small function loads all the hooks and must be present in every mod
MOD_EXPORT int ModInit(int argc, char** argv)
{
	ZHL::ClearLogFile();
	ZHL::Logger logger(true);
	logger.Log("REPENTOGON: ModInit\n");

	logger.Log("REPENTOGON: Initializing options\n");
	repentogonOptions.Init();
	logger.Log("REPENTOGON: Initialized options\n");
	REPENTOGON::UpdateProgressDisplay("Perform ASMPatches");
	logger.Log("REPENTOGON: Performing ASM patches\n");
	PerformASMPatches();
	logger.Log("REPENTOGON: Done performing ASM patches\n");

	REPENTOGON::UpdateProgressDisplay("Initialize Shaders");

	logger.Log("REPENTOGON: Initializing shaders\n");
	LuaRender::InitShaders();
	logger.Log("REPENTOGON: Initialized shaders\n");
	logger.Log("REPENTOGON: Initializing ZHL definitions\n");
	Definition::Init();
	logger.Log("REPENTOGON: Initialized ZHL definitions\n");

	logger.Log("REPENTOGON: Fixing LuaDump for Lua 5.4\n");
	FixLuaDump();	//moved to before hooks are added, allows hooking onto exception handler
	logger.Log("REPENTOGON: Fixed LuaDump for Lua 5.4\n");

	logger.Log("REPENTOGON: Installing ZHL hooks\n");
	ZHL::Init();
	logger.Log("REPENTOGON: Installed ZHL hooks\n");

	printf(":REPENTOGON:\n");

	REPENTOGON::UpdateProgressDisplay("ModInit done");
	SigScan::FlushCache();
	return 0;
}

