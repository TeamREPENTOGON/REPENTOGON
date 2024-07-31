#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GenericPromptConstructor) {
	lua::place<GenericPrompt>(L, lua::metatables::GenericPromptMT);
	luaL_setmetatable(L, lua::metatables::GenericPromptMT);
	return 1;
}

LUA_FUNCTION(Lua_GenericPromptInitialize) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	const bool smallPrompt = lua::luaL_optboolean(L, 2, false);

	genericPrompt->Initialize(smallPrompt);
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptShow) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	genericPrompt->Show();
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptIsActive) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	lua_pushboolean(L, genericPrompt->IsActive());
	return 1;
}

LUA_FUNCTION(Lua_GenericPromptSetImageToVictoryRun) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	genericPrompt->SetImageToVictoryRun(nullptr);
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptUpdate) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	bool processInput = lua_toboolean(L, 2);
	genericPrompt->Update(processInput);
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptRender) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	genericPrompt->Render();
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptSetText) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	const char* param_1 = luaL_optstring(L, 2, "");
	const char* param_2 = luaL_optstring(L, 3, "");
	const char* param_3 = luaL_optstring(L, 4, "");
	const char* param_4 = luaL_optstring(L, 5, "");
	const char* param_5 = luaL_optstring(L, 6, "");
	genericPrompt->SetText(param_1, param_2, param_3, param_4, param_5);
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptDisappear) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	genericPrompt->Disappear(&genericPrompt->_anm2);
	return 0;
}

LUA_FUNCTION(Lua_GenericPromptGetSprite) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	lua::luabridge::UserdataPtr::push(L, &genericPrompt->_anm2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_GenericPromptGetCurrentSelection) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	lua_pushinteger(L, genericPrompt->_currentSelection);
	return 1;
}

LUA_FUNCTION(Lua_GenericPromptGetSubmittedSelection) {
	auto* genericPrompt = lua::GetUserdata<GenericPrompt*>(L, 1, lua::metatables::GenericPromptMT);
	lua_pushinteger(L, genericPrompt->_submittedSelection);
	return 1;
}

static void RegisterGenericPrompt(lua_State* L) {

	luaL_Reg functions[] = {
		{ "Initialize", Lua_GenericPromptInitialize },
		{ "Show", Lua_GenericPromptShow },
		{ "IsActive", Lua_GenericPromptIsActive },
		{ "SetImageToVictoryRun", Lua_GenericPromptSetImageToVictoryRun },
		{ "Update", Lua_GenericPromptUpdate },
		{ "Render", Lua_GenericPromptRender },
		{ "SetText", Lua_GenericPromptSetText },
		//{ "Disappear", Lua_GenericPromptDisappear },
		{ "GetSprite", Lua_GenericPromptGetSprite },
		{ "GetCurrentSelection", Lua_GenericPromptGetCurrentSelection },
		{ "GetSubmittedSelection", Lua_GenericPromptGetSubmittedSelection },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::GenericPromptMT, lua::metatables::GenericPromptMT, functions);
	lua_register(L, lua::metatables::GenericPromptMT, Lua_GenericPromptConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterGenericPrompt(_state);
}