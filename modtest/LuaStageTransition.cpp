#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"

static const char* StageTransitionMT = "StageTransitionMT";

LUA_FUNCTION(lua_StageTransition_SetSame) {
	StageTransition* transition = *lua::GetUserdata<StageTransition**>(L, 1, StageTransitionMT);
	bool sameStage = lua_toboolean(L, 2);

	transition->_sameStage = sameStage;
	return 0;
}

LUA_FUNCTION(lua_StageTransition_GetSame) {
	StageTransition* transition = *lua::GetUserdata<StageTransition**>(L, 1, StageTransitionMT);
	lua_pushboolean(L, transition->_sameStage);
	return 1;
}

LUA_FUNCTION(lua_GameGetStageTransition) {
	StageTransition** transition = (StageTransition**)lua_newuserdata(L, sizeof(StageTransition*));
	*transition = g_Game->GetStageTransition();
	luaL_setmetatable(L, StageTransitionMT);
	return 1;
}

static luaL_Reg StageTransitionFunctions[] = {
	{ "SetSameStage", lua_StageTransition_SetSame },
	{ "GetSameStage", lua_StageTransition_GetSame },
	{ NULL, NULL }
};

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterNewClass(_state, "StageTransition", StageTransitionMT, StageTransitionFunctions);
	lua::RegisterFunction(_state, lua::Metatables::GAME, "GetStageTransition", lua_GameGetStageTransition);
}