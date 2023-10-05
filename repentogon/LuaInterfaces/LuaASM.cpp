#include <cstdio>

#include <lua.hpp>

#include "ASMPatcher.hpp"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include "SigScan.h"
#include <Zydis/Zydis.h>

static int Lua_Disassemble(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		return luaL_error(L, "Lua_Disassemble: expected one parameter (the address), got %d parameters\n", n);
	}

	lua_Integer addr = luaL_checkinteger(L, 1);
	ZydisDisassembledInstruction instruction;
	ZyanStatus status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LEGACY_32, addr, (void*)addr, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
	if (ZYAN_SUCCESS(status)) {
		lua_pushboolean(L, 1);
		lua_pushstring(L, instruction.text);
		lua_pushinteger(L, instruction.info.length);
		lua_pushlightuserdata(L, (void*)addr);
		return 4;
	} else {
		lua_pushboolean(L, 0);
		return 1;
	}
}

static int Lua_TestNop(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		return luaL_error(L, "Lua_TestNop: expected one parameter (the address), got %d parameters\n", n);
	}

	lua_Integer addr = luaL_checkinteger(L, 1);
	sASMPatcher.PatchAt((void*)addr, "\x90");

	return 0;
}

static constexpr const char* ASMPatchTestMT = "ASMPatchTest";

static int Lua_NewASMPatch(lua_State* L) {
	ASMPatch* patch = new ASMPatch;
	ASMPatch** ud = (ASMPatch**)lua_newuserdata(L, sizeof(ASMPatch*));
	*ud = patch;
	luaL_setmetatable(L, ASMPatchTestMT);
	return 1;
}

static int Lua_ASMPatch_AddBytes(lua_State* L) {
	ASMPatch** ud = (ASMPatch**)luaL_checkudata(L, 1, ASMPatchTestMT);
	ASMPatch* patch = *ud;

	int top = lua_gettop(L);
	std::vector<char> bytes;
	for (int i = 2; i <= top; ++i) {
		lua_Integer value = luaL_checkinteger(L, i);
		if (value > 255) {
			return luaL_error(L, "Invalid byte %d\n", value);
		}

		bytes.push_back((char)value);
	}

	bytes.push_back('\0');
	patch->AddBytes(bytes.data());

	return 0;
}

static int Lua_ASMPatch_AddJump(lua_State* L) {
	ASMPatch** ud = (ASMPatch**)luaL_checkudata(L, 1, ASMPatchTestMT);
	ASMPatch* patch = *ud;

	lua_Integer addr = luaL_checkinteger(L, 2);
	patch->AddRelativeJump((void*)addr);

	return 0;
}

static int Lua_ASMPatch_AddCJump(lua_State* L) {
	ASMPatch** ud = (ASMPatch**)luaL_checkudata(L, 1, ASMPatchTestMT);
	ASMPatch* patch = *ud;

	ASMPatcher::CondJumps cond = (ASMPatcher::CondJumps)luaL_checkinteger(L, 2);
	lua_Integer addr = luaL_checkinteger(L, 3);
	patch->AddConditionalRelativeJump(cond, (void*)addr);

	return 0;
}

static int Lua_ASMPatch_ToASM(lua_State* L) {
	ASMPatch** ud = (ASMPatch**)luaL_checkudata(L, 1, ASMPatchTestMT);
	ASMPatch* patch = *ud;

	char* base = (char*)luaL_checkinteger(L, 2);
	char* at = (char*)luaL_checkinteger(L, 3);
	std::unique_ptr<char[]> result = patch->ToASM(base);

	char* start = result.get();
	ZyanU64 addrBase = (ZyanU64)at;

	lua_newtable(L);
	int i = 1;
	size_t read = 0;
	while (read < patch->Length()) {
		ZydisDisassembledInstruction instruction;
		ZyanStatus status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LEGACY_32, addrBase, start, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
		if (!ZYAN_SUCCESS(status)) {
			lua_pop(L, 1);
			return luaL_error(L, "Error while disassembling instruction");
		}

		lua_newtable(L);
		lua_pushstring(L, "ins");
		lua_pushstring(L, instruction.text);
		lua_rawset(L, -3);

		lua_pushstring(L, "runtime_addr");
		lua_pushinteger(L, instruction.runtime_address);
		lua_rawset(L, -3);

		lua_pushstring(L, "x86");
		
		char buffer[4096];
		int written = 0;
		for (int j = 0; j < instruction.info.length; ++j) {
			written += sprintf(buffer + written, "%hhx", start[j]);
		}
		buffer[written] = '\0';
		lua_pushstring(L, buffer);
		lua_rawset(L, -3);

		lua_seti(L, -2, i);
		
		++i;
		start += instruction.info.length;
		read += instruction.info.length;
		addrBase += instruction.info.length;
	}

	return 1;
}

static int Lua_ASMPatch___gc(lua_State* L) {
	ASMPatch** ud = (ASMPatch**)luaL_checkudata(L, 1, ASMPatchTestMT);
	ASMPatch* patch = *ud;
	delete patch;
	return 0;
}

static void RegisterASMPatchTest(lua_State* L) {
	lua::LuaStackProtector protector(L);
	luaL_newmetatable(L, ASMPatchTestMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, Lua_ASMPatch___gc);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "AddBytes", Lua_ASMPatch_AddBytes },
		{ "AddJump", Lua_ASMPatch_AddJump },
		{ "AddCJump", Lua_ASMPatch_AddCJump },
		{ "ToASM", Lua_ASMPatch_ToASM },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

	lua_newtable(L);
	lua_pushstring(L, "JO");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JO);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNO");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNO);
	lua_rawset(L, -3);
	lua_pushstring(L, "JB");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JB);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNAE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNAE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JC");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JC);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNB");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNB);
	lua_rawset(L, -3);
	lua_pushstring(L, "JAE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JAE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNC");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNC);
	lua_rawset(L, -3);
	lua_pushstring(L, "JZ");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JZ);
	lua_rawset(L, -3);
	lua_pushstring(L, "JE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNZ");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNZ);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JBE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JBE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNA");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNA);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNBE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNBE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JA");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JA);
	lua_rawset(L, -3);
	lua_pushstring(L, "JS");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JS);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNS");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNS);
	lua_rawset(L, -3);
	lua_pushstring(L, "JP");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JP);
	lua_rawset(L, -3);
	lua_pushstring(L, "JPE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JPE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNP");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNP);
	lua_rawset(L, -3);
	lua_pushstring(L, "JPO");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JPO);
	lua_rawset(L, -3);
	lua_pushstring(L, "JL");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JL);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNGE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNGE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNL");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNL);
	lua_rawset(L, -3);
	lua_pushstring(L, "JGE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JGE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JLE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JLE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNG");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNG);
	lua_rawset(L, -3);
	lua_pushstring(L, "JNLE");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JNLE);
	lua_rawset(L, -3);
	lua_pushstring(L, "JG");
	lua_pushinteger(L, (uint32_t)ASMPatcher::CondJumps::JG);
	lua_rawset(L, -3);
	lua_setglobal(L, "CondJumps");
	// lua_pop(L, 1);

	lua_register(L, "ASMPatch", Lua_NewASMPatch);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);

	void* play = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (play) {
		lua_pushinteger(state, (lua_Integer)play);
		lua_setglobal(state, "PlayPage");
		lua_register(state, "TestNop", Lua_TestNop);
	}

	RegisterASMPatchTest(state);

	lua_register(state, "Disassemble", Lua_Disassemble);
};