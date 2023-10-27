#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CreateWeightedOutcomePicker) {
	lua::place<WeightedOutcomePicker>(L, lua::metatables::WeightedOutcomePickerMT);
	return 1;
}

LUA_FUNCTION(Lua_WeightedOutcomePicker_AddOutcomeWeight) {
	WeightedOutcomePicker* picker = lua::GetUserdata<WeightedOutcomePicker*>(L, 1, lua::metatables::WeightedOutcomePickerMT);

	uint32_t value = (uint32_t)luaL_checknumber(L, 2);
	uint32_t weight = (uint32_t)luaL_checknumber(L, 3);

	WeightedOutcomePicker_Outcome outcome{ value, weight };
	picker->AddOutcomeWeight(outcome, false);

	return 0;
}

LUA_FUNCTION(Lua_WeightedOutcomePicker_PickOutcome) {
	WeightedOutcomePicker* picker = lua::GetUserdata<WeightedOutcomePicker*>(L, 1, lua::metatables::WeightedOutcomePickerMT);
	RNG* rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");

	uint32_t result = picker->PickOutcome(*rng);
	lua_pushinteger(L, result);

	rng->Next();

	return 1;
}

LUA_FUNCTION(Lua_WeightedOutcomePicker_gc) {
	WeightedOutcomePicker* picker = lua::GetUserdata<WeightedOutcomePicker*>(L, 1, lua::metatables::WeightedOutcomePickerMT);
	picker->~WeightedOutcomePicker();
	return 0;
}

static void RegisterWeightedOutcomePicker(lua_State* L) {
	lua_register(L, lua::metatables::WeightedOutcomePickerMT, Lua_CreateWeightedOutcomePicker);

	luaL_Reg functions[] = {
		{ "AddOutcomeWeight", Lua_WeightedOutcomePicker_AddOutcomeWeight },
		{ "PickOutcome", Lua_WeightedOutcomePicker_PickOutcome },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::WeightedOutcomePickerMT, lua::metatables::WeightedOutcomePickerMT, functions, Lua_WeightedOutcomePicker_gc);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterWeightedOutcomePicker(state);
}