#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PathfinderGetCanCrushRocks)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	lua_pushboolean(L, pathfinder->_canCrushRocks);
	return 1;
}

LUA_FUNCTION(Lua_PathfinderSetCanCrushRocks)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	pathfinder->_canCrushRocks = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderGetEvadeMovementCountdown)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	lua_pushboolean(L, pathfinder->_evadeMovementCountdown);
	return 1;
}

LUA_FUNCTION(Lua_PathfinderSetEvadeMovementCountdown)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	pathfinder->_evadeMovementCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderEvadeTarget)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const bool ignoreEffects = lua::luaL_checkboolean(L, 3);
	pathfinder->EvadeTarget(pos, ignoreEffects);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderFindGridPath)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const float speed = (float)luaL_checknumber(L, 3);
	const int pathmarker = (int)luaL_checkinteger(L, 4);
	const bool directPath = lua::luaL_checkboolean(L, 5);
	pathfinder->FindGridPath(pos, speed, pathmarker, directPath);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderHasDirectPath)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	lua_pushboolean(L, pathfinder->_hasDirectPath);
	return 1;
}

LUA_FUNCTION(Lua_PathfinderHasPathToPos)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");;
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const bool ignorePoop = lua::luaL_checkboolean(L, 3);
	lua_pushboolean(L, pathfinder->HasPathToPos(pos, ignorePoop));
	return 1;
}

LUA_FUNCTION(Lua_PathfinderMoveRandomly)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	const bool ignoreEffects = lua::luaL_checkboolean(L, 2);
	pathfinder->MoveRandomly(ignoreEffects);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderMoveRandomlyAxisAligned)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	const float speed = (float)luaL_checknumber(L, 2);
	const bool ignoreEffects = lua::luaL_checkboolean(L, 3);
	pathfinder->MoveRandomlyAxisAligned(speed, ignoreEffects);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderMoveRandomlyBoss)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");;
	const bool ignoreEffects = lua::luaL_checkboolean(L, 2);
	pathfinder->MoveRandomlyBoss(ignoreEffects);
	return 0;
}

LUA_FUNCTION(Lua_PathfinderReset)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	pathfinder->Reset();
	return 0;
}

LUA_FUNCTION(Lua_PathfinderResetMovementTarget)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");;
	pathfinder->ResetMovementTarget();
	return 0;
}

LUA_FUNCTION(Lua_PathfinderUpdateGridIndex)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");;
	pathfinder->UpdateGridIndex();
	return 0;
}

LUA_FUNCTION(Lua_PathfinderSimulatePlayerMovement)
{
	NPCAI_Pathfinder* pathfinder = lua::GetLuabridgeUserdata<NPCAI_Pathfinder*>(L, 1, lua::Metatables::PATHFINDER, "Pathfinder");
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const float speed = (float)luaL_checknumber(L, 3);
	pathfinder->SimulatePlayerMovement(pos, speed, false);
	return 0;
}

/*
HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetCanCrushRocks", Lua_PathfinderGetCanCrushRocks },
		//{ "SetCanCrushRocks", Lua_PathfinderSetCanCrushRocks },
		//{ "GetEvadeMovementCountdown", Lua_PathfinderGetEvadeMovementCountdown },
		{ "SetEvadeMovementCountdown", Lua_PathfinderSetEvadeMovementCountdown },
		//{ "EvadeTarget", Lua_PathfinderEvadeTarget },
		//{ "FindGridPath", Lua_PathfinderFindGridPath },
		//{ "HasDirectPath", Lua_PathfinderHasDirectPath },
		//{ "HasPathToPos", Lua_PathfinderHasPathToPos },
		//{ "MoveRandomly", Lua_PathfinderMoveRandomly },
		//{ "MoveRandomlyAxisAligned", Lua_PathfinderMoveRandomlyAxisAligned },
		//{ "MoveRandomlyBoss", Lua_PathfinderMoveRandomlyBoss },
		//{ "Reset", Lua_PathfinderReset },
		//{ "ResetMovementTarget", Lua_PathfinderResetMovementTarget },
		//{ "UpdateGridIndex", Lua_PathfinderUpdateGridIndex },
		{ "SimulatePlayerMovement", Lua_PathfinderSimulatePlayerMovement },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::PATHFINDER, functions);
}
*/


