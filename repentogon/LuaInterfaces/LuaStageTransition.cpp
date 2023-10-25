#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"


LUA_FUNCTION(lua_StageTransition_SetSame) {
	StageTransition* transition = *lua::GetUserdata<StageTransition**>(L, 1, lua::metatables::StageTransitionMT);
	bool sameStage = lua_toboolean(L, 2);

	transition->_sameStage = sameStage;
	return 0;
}

LUA_FUNCTION(lua_StageTransition_GetSame) {
	StageTransition* transition = *lua::GetUserdata<StageTransition**>(L, 1, lua::metatables::StageTransitionMT);
	lua_pushboolean(L, transition->_sameStage);
	return 1;
}

LUA_FUNCTION(lua_GameGetStageTransition) {
	StageTransition** transition = (StageTransition**)lua_newuserdata(L, sizeof(StageTransition*));
	*transition = g_Game->GetStageTransition();
	luaL_setmetatable(L, lua::metatables::StageTransitionMT);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterFunction(_state, lua::Metatables::GAME, "GetStageTransition", lua_GameGetStageTransition);

	luaL_Reg StageTransitionFunctions[] = {
		{ "SetSameStage", lua_StageTransition_SetSame },
		{ "GetSameStage", lua_StageTransition_GetSame },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(_state, lua::metatables::StageTransitionMT, lua::metatables::StageTransitionMT, StageTransitionFunctions);
}