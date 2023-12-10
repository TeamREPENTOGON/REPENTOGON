#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"


LUA_FUNCTION(lua_StageTransition_SetSame) {
	StageTransition* transition = g_Game->GetStageTransition();
	bool sameStage = lua::luaL_checkboolean(L, 1);

	transition->_sameStage = sameStage;
	return 0;
}

LUA_FUNCTION(lua_StageTransition_GetSame) {
	StageTransition* transition = g_Game->GetStageTransition();
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
	lua_newtable(_state);
	//lua::RegisterFunction(_state, lua::Metatables::GAME, "GetStageTransition", lua_GameGetStageTransition);
	lua::TableAssoc(_state, "SetSameStage", lua_StageTransition_SetSame);
	lua::TableAssoc(_state, "GetSameStage", lua_StageTransition_GetSame);

	lua_setglobal(_state, "StageTransition");

	//lua::RegisterNewClass(_state, lua::metatables::StageTransitionMT, lua::metatables::StageTransitionMT, StageTransitionFunctions);
}