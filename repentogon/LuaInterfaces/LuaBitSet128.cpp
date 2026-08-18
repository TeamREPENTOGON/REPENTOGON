#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem.h"

static void* BitSet128Key = nullptr;

inline int highSplit(uint64_t num) {
	return static_cast<int>(num >> 32);
};

inline int lowSplit(uint64_t num) {
	return static_cast<int>(num & 0xFFFFFFFFu);
};

LUA_FUNCTION(Lua_BitSet128_Constructor) {
	uint64_t l = lua::luaL_checkuint64(L, 2);
	uint64_t h = lua::luaL_checkuint64(L, 3);

	BitSet128 bit(lowSplit(l), highSplit(l), lowSplit(h), highSplit(h));
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, bit);
	return 1;
}

static void OverrideRNGConstructor(lua_State* L) {
	lua::LuaStackProtector protector(L);

	int result = lua_getglobal(L, "BitSet128");
	if (result == LUA_TNIL) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] No global \"BitSet128\" in Lua environment\n");
		return;
	}
	else if (result != LUA_TTABLE) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] Global \"BitSet128\" is not a table\n");
		return;
	}

	result = lua_getmetatable(L, -1);
	if (result == 0) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] Global \"BitSet128\" has no metatable\n");
		return;
	}

	lua_pushcfunction(L, Lua_BitSet128_Constructor);
	lua_setfield(L, -2, "__call");
	lua_pop(L, 2);
	return;
}

LUA_FUNCTION(Lua_BitSet128_metabnot) {
	BitSet128* bit = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	BitSet128 result = BitSet128(~bit->ll, ~bit->lh, ~bit->hl, ~bit->hh);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metabor) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	BitSet128 result = BitSet128(b1->ll | b2 -> ll, b1 -> lh | b2 -> lh, b1 -> hl | b2 -> hl, b1 -> hh | b2 -> hh);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metaband) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	BitSet128 result = BitSet128(b1->ll & b2->ll, b1->lh & b2->lh, b1->hl & b2->hl, b1->hh & b2->hh);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metabxor) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	BitSet128 result = BitSet128(b1->ll ^ b2->ll, b1->lh ^ b2->lh, b1->hl ^ b2->hl, b1->hh ^ b2->hh);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metashl) {
	BitSet128* bit = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	int shift = (int)luaL_checknumber(L, 2);

	BitSet128 result = BitSet128(bit->ll << shift, bit->lh << shift, bit->hl << shift, bit->hh << shift);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metashr) {
	BitSet128* bit = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	int shift = (int)luaL_checknumber(L, 2);

	BitSet128 result = BitSet128(bit->ll >> shift, bit->lh >> shift, bit->hl >> shift, bit->hh >> shift);
	lua::luabridge::UserdataValue<BitSet128>::push(L, BitSet128Key, result);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metaeq) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	lua_pushboolean(L, b1->ll == b2->ll && b1->lh == b2->lh && b1->hl == b2->hl && b1->hh == b2->hh);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metalt) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	lua_pushboolean(L, b1->hh != b2->hh ? b1->hh < b2->hh :
					   b1->hl != b2->hl ? b1->hl < b2->hl :
					   b1->lh != b2->lh ? b1->lh < b2->lh :
					   b1->ll < b2->ll);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_metale) {
	BitSet128* b1 = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	BitSet128* b2 = lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");

	lua_pushboolean(L, b1->hh != b2->hh ? b1->hh <= b2->hh :
					   b1->hl != b2->hl ? b1->hl <= b2->hl :
					   b1->lh != b2->lh ? b1->lh <= b2->lh :
					   b1->ll <= b2->ll);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_Get) {
	BitSet128* bit = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	int pos = (int)luaL_checknumber(L, 2);

	unsigned int p;
	switch (pos / 32) {
	case 0: p = bit->ll; break;
	case 1: p = bit->lh; break;
	case 2: p = bit->hl; break;
	case 3: p = bit->hh; break;
	}

	lua_pushboolean(L, (p >> pos % 32) & 1u);
	return 1;
};

LUA_FUNCTION(Lua_BitSet128_Set) {
	BitSet128* bit = lua::GetLuabridgeUserdata<BitSet128*>(L, 1, lua::Metatables::BITSET_128, lua::Metatables::CONST_BITSET_128, "BitSet128");
	int pos = (int)luaL_checknumber(L, 2);
	bool state = lua::luaL_checkboolean(L, 3);

	unsigned int* p;
	switch (pos / 32) {
		case 0: p = &bit->ll; break;
		case 1: p = &bit->lh; break;
		case 2: p = &bit->hl; break;
		case 3: p = &bit->hh; break;
	}
	if (state)
		*p |= 1u << pos % 32;
	else
		*p &= ~(1u << pos % 32);

	return 0;
};

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	BitSet128Key = lua::GetMetatableKey(lua::Metatables::BITSET_128);

	luaL_Reg functions[] = {
		{ "Get", Lua_BitSet128_Get },
		{ "Set", Lua_BitSet128_Set },
		{ "__bnot", Lua_BitSet128_metabnot },
		{ "__bor", Lua_BitSet128_metabor },
		{ "__band", Lua_BitSet128_metaband },
		{ "__bxor", Lua_BitSet128_metabxor },
		{ "__shl", Lua_BitSet128_metashl },
		{ "__shr", Lua_BitSet128_metashr },
		{ "__eq", Lua_BitSet128_metaeq },
		{ "__lt", Lua_BitSet128_metalt },
		{ "__le", Lua_BitSet128_metale },
		{ NULL, NULL }
	};

	OverrideRNGConstructor(_state);
	lua::RegisterFunctions(_state, lua::Metatables::BITSET_128, functions);
	lua::RegisterFunctions(_state, lua::Metatables::CONST_BITSET_128, functions);
}