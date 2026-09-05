#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

#pragma region DLL export

extern "C" {
	__declspec(dllexport) void L_Game_Fadein(Game* game, float speed, bool showIcon, KColor* color) {
		game->FadeIn(speed, showIcon, color);
	}

	__declspec(dllexport) void L_Game_Fadeout(Game* game, float speed, int fadeoutTarget, KColor* color) {
		game->FadeOut(speed, fadeoutTarget, color);
	}
}

#pragma endregion

LUA_FUNCTION(Lua_GameFadein)
{
	Game* game = LuaGame::Get(L, 1);
	float speed = (float)luaL_checknumber(L, 2);
	bool showIcon = !lua_isnoneornil(L, 3) ? lua_toboolean(L, 3) : true;
	KColor* optColor = LuaKColor::GetOpt(L, 4);
	KColor color = optColor ? *optColor : KColor(0, 0, 0, 1);

	L_Game_Fadein(game, speed, showIcon, &color);
	return 0;
}

LUA_FUNCTION(Lua_GameFadeout)
{
	Game* game = LuaGame::Get(L, 1);
	float speed = (float)luaL_checknumber(L, 2);
	int fadeoutTarget = (int)luaL_checkinteger(L, 3);
	KColor* optColor = LuaKColor::GetOpt(L, 4);
	KColor color = optColor ? *optColor : KColor(0, 0, 0, 1);

	L_Game_Fadeout(game, speed, fadeoutTarget, &color);
	return 0;
}

LUA_FUNCTION(Lua_GameBombDamage)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos =  lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float damage = (float)luaL_checknumber(L, 3);
	float radius = (float)luaL_checknumber(L, 4);
	bool lineCheck = lua::luaL_optboolean(L, 5, true);
	Entity* source = nullptr;
	if (lua_type(L, 6) == LUA_TUSERDATA) {
		source = lua::GetLuabridgeUserdata<Entity*>(L, 6, lua::Metatables::ENTITY, "Entity");
	}
	BitSet128 tearFlags;
	if (lua_type(L, 7) == LUA_TCDATA) {
		tearFlags = *lua::GetCData<BitSet128*>(L, 7, lua::ffi::CData[lua::ffi::CDataID::BITSET_128], "BitSet128");
	}
	unsigned long long damageFlags = (unsigned long long)luaL_optinteger(L, 8, eDamageFlag::DAMAGE_EXPLOSION);
	bool damageSource = lua::luaL_optboolean(L, 9, false);

	game->BombDamage(pos, damage, radius, lineCheck, source, tearFlags, damageFlags, damageSource);	

	return 0;
}

LUA_FUNCTION(Lua_GameBombExplosionEffects)
{
	// Note: the docs are wrong here (for the signature), damageFlags and damageSource are swapped. See main.lua:560-562
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float damage = (float)luaL_checknumber(L, 3);
	BitSet128 tearFlags;
	if (lua_type(L, 4) == LUA_TCDATA) {
		tearFlags = *lua::GetCData<BitSet128*>(L, 4, lua::ffi::CData[lua::ffi::CDataID::BITSET_128], "BitSet128");
	}
	ColorMod color;
	if (lua_type(L, 5) == LUA_TCDATA) {
		color = *lua::GetCData<ColorMod*>(L, 5, lua::ffi::CData[lua::ffi::CDataID::COLOR], "Color");
	}
	Entity* source = nullptr;
	if (lua_type(L, 6) == LUA_TUSERDATA) {
		source = lua::GetLuabridgeUserdata<Entity*>(L, 6, lua::Metatables::ENTITY, "Entity");
	}
	float radiusMult = (float)luaL_optnumber(L, 7, 1);
	bool lineCheck = lua::luaL_optboolean(L, 8, true);
	unsigned long long damageFlags = (unsigned long long)luaL_optinteger(L, 9, eDamageFlag::DAMAGE_EXPLOSION);
	bool damageSource = lua::luaL_optboolean(L, 10, false);

	game->BombExplosionEffects(pos, damage, tearFlags, &color, source, radiusMult, lineCheck, damageFlags, damageSource);

	return 0;
}

LUA_FUNCTION(Lua_GameBombTearflagEffects)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float radius = (float)luaL_checknumber(L, 3);
	BitSet128* tearFlags = lua::GetCData<BitSet128*>(L, 4, lua::ffi::CData[lua::ffi::CDataID::BITSET_128], "BitSet128");
	Entity* source = nullptr;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		source = lua::GetLuabridgeUserdata<Entity*>(L, 5, lua::Metatables::ENTITY, "Entity");
	}
	float radiusMult = (float)luaL_optnumber(L, 6, 1);

	game->BombTearflagEffects(pos, radius, *tearFlags, source, radiusMult);

	return 0;
}

LUA_FUNCTION(Lua_GameButterBeanFart)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float radius = (float)luaL_checknumber(L, 3);
	Entity* source = lua::GetLuabridgeUserdata<Entity*>(L, 4, lua::Metatables::ENTITY, "Entity");
	bool showEffect = lua::luaL_checkboolean(L, 5);
	bool doSuperKnockback = lua::luaL_checkboolean(L, 6);

	game->ButterBeanFart(pos, radius, source, showEffect, doSuperKnockback);

	return 0;
}

LUA_FUNCTION(Lua_GameCharmFart)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float radius = (float)luaL_checknumber(L, 3);
	Entity* source = lua::GetLuabridgeUserdata<Entity*>(L, 4, lua::Metatables::ENTITY, "Entity");

	game->CharmFart(pos, radius, source);

	return 0;
}

LUA_FUNCTION(Lua_GameFart)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float radius = (float)luaL_optnumber(L, 3, 85);
	Entity* source = nullptr;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		source = lua::GetLuabridgeUserdata<Entity*>(L, 4, lua::Metatables::ENTITY, "Entity");
	}
	float fartScale = (float)luaL_optnumber(L, 5, 1);
	int fartSubType = (int)luaL_optinteger(L, 6, 1);
	ColorMod color;
	if (lua_type(L, 7) == LUA_TCDATA) {
		color = *lua::GetCData<ColorMod*>(L, 7, lua::ffi::CData[lua::ffi::CDataID::COLOR], "Color");
	}

	game->Fart(pos, radius, source, fartScale, fartSubType, color);

	return 0;
}


LUA_FUNCTION(Lua_GameGetNearestPlayer)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	
	lua::luabridge::UserdataPtr::push(L, game->GetNearestPlayer(pos), lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

LUA_FUNCTION(Lua_GameGetRandomPlayer)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float radius = (float)luaL_checknumber(L, 3);

	lua::luabridge::UserdataPtr::push(L, game->GetRandomPlayer(pos, radius), lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

LUA_FUNCTION(Lua_GameMakeShockwave)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float amplitude = (float)luaL_checknumber(L, 3);
	float speed = (float)luaL_checknumber(L, 4);
	int duration = (int)luaL_checkinteger(L, 5);
	
	game->MakeShockwave(*pos, amplitude, speed, duration);

	return 0;
}

LUA_FUNCTION(Lua_GameSpawn)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int type = (int)luaL_checkinteger(L, 2);
	int variant = (int)luaL_checkinteger(L, 3);
	Vector* pos = lua::GetCData<Vector*>(L, 4, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	Vector* vel = lua::GetCData<Vector*>(L, 5, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	Entity* spawner = lua::GetLuabridgeUserdata<Entity*>(L, 6, lua::Metatables::ENTITY, "Entity");
	int subtype = (int)luaL_checkinteger(L, 7);
	int seed = (int)luaL_checkinteger(L, 8);

	game->Spawn(type, variant, *pos, *vel, spawner, subtype, seed, 0);

	return 0;
}

LUA_FUNCTION(Lua_GameSpawnParticles)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	int variant = (int)luaL_checkinteger(L, 3);
	int num = (int)luaL_checkinteger(L, 4);
	float speed = (float)luaL_checknumber(L, 5);
	ColorMod color;
	if (lua_type(L, 6) == LUA_TCDATA) {
		color = *lua::GetCData<ColorMod*>(L, 6, lua::ffi::CData[lua::ffi::CDataID::COLOR], "Color");
	}
	float height = (float)luaL_optnumber(L, 7, 100000);
	int subtype = (int)luaL_optinteger(L, 8, 0);

	game->SpawnParticles(pos, variant, num, speed, color, height, subtype);

	return 0;
}

LUA_FUNCTION(Lua_GameUpdateStrangeAttractor)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	float force = (float)luaL_optnumber(L, 3, 10);
	float radius = (float)luaL_optnumber(L, 4, 250);

	game->UpdateStrangeAttractor(pos, force, radius);

	return 0;
}

LUA_FUNCTION(Lua_GameGetScreenShakeOffset) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	lua::ffi::pushCdata(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR], game->_screenShakeOffset);
	return 1;
}

LUA_FUNCTION(Lua_GameAchievementUnlocksDisallowed)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->AchievementUnlocksDisallowed());

	return 1;
}

LUA_FUNCTION(Lua_GameIsPauseMenuOpen)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsPauseMenuOpen());

	return 1;
}

LUA_FUNCTION(Lua_GameGetPauseMenuState)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPauseMenu()->state);

	return 1;
}

LUA_FUNCTION(Lua_GameGetPlanetariumsVisited)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPlanetariumsVisited());

	return 1;
}

LUA_FUNCTION(Lua_GameIsHardMode)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsHardMode());

	return 1;
}

LUA_FUNCTION(Lua_GameGetLastDevilRoomStageFix)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastDevilRoomStage());

	return 1;
}

LUA_FUNCTION(Lua_GetLastLevelWithDamageFix)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithDamage());

	return 1;
}

LUA_FUNCTION(Lua_GetLastLevelWithoutHalfHpFix)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithoutHalfHp());

	return 1;
}

LUA_FUNCTION(Lua_GameGetDebugFlags)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, *game->GetDebugFlags());

	return 1;
}

LUA_FUNCTION(Lua_GameAddDebugFlags)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flags = (unsigned int)luaL_checkinteger(L, 2);
	*game->GetDebugFlags() |= flags;
	return 0;
}

LUA_FUNCTION(Lua_GameSpawnBombCrater) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	const float radius = (const float)luaL_optnumber(L, 3, 1.0f);
	Entity* crater = game->SpawnBombCrater(pos, radius);

	lua::luabridge::UserdataPtr::push(L, crater, lua::GetMetatableKey(lua::Metatables::ENTITY));

	return 1;
}

LUA_FUNCTION(Lua_GameDevolveEnemy) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	game->DevolveEnemy(ent, nullptr);

	return 0;
}

LUA_FUNCTION(lua_GameStartStageTransition) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	bool sameStage = lua::luaL_checkboolean(L, 2);
	int transition = (int)luaL_checkinteger(L, 3);
	Entity_Player* player = nullptr;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY_PLAYER, "Player");
	}

	game->StartStageTransition(sameStage, transition, player);
	return 0;
}

LUA_FUNCTION(Lua_GameIsGreedBoss)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedBoss());

	return 1;
}

LUA_FUNCTION(Lua_GameIsGreedFinalBoss)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedFinalBoss());

	return 1;
}

LUA_FUNCTION(Lua_GameAddErasedEnemy) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
		game->AddErasedEnemy(entity);
	} else {
		int type = (int)luaL_checkinteger(L, 2);
		int variant = (int)luaL_checkinteger(L, 3);
		// The game ignores subtypes for erased enemies do not believe the lies
		game->AddErasedEnemyByIds(type, variant, 0);
	}
	return 0;
}

LUA_FUNCTION(Lua_GameRemoveErasedEnemy) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int type = (int)luaL_checkinteger(L, 2);
	int variant = (int)luaL_optinteger(L, 3, -1);
	game->RemoveErasedEnemy(type, variant, 0);
	return 0;
}

LUA_FUNCTION(lua_GameIsErased) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

		bool wasErased = game->IsErased(entity->_type, entity->_variant, entity->_subtype);
		lua_pushboolean(L, wasErased);
	}
	else {
		int type = (int)luaL_checkinteger(L, 2);
		int variant = (int)luaL_optinteger(L, 3, -1);
		int subtype = (int)luaL_optinteger(L, 4, -1);  // useless, basically ignored by the game

		bool wasErased = game->IsErased(type, variant, subtype);
		lua_pushboolean(L, wasErased);
	}

	return 1;
}

LUA_FUNCTION(Lua_GameGetCurrentColorModifier)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetCurrentColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameGetTargetColorModifier)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetTargetColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameGetLerpColorModifier)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetLerpColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameSetColorModifier)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* pColor = lua::GetRawUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);
	bool lerp = lua::luaL_optboolean(L, 3, true);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	game->SetColorModifier(pColor, lerp, rate);

	return 0;
}

LUA_FUNCTION(Lua_GameIsRerun) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->_isRerun);

	return 1;
}

LUA_FUNCTION(Lua_GameGetPlayer) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int idx = (int)luaL_optinteger(L, 2, 0);

	if (!g_Game || g_Game->_playerManager._playerList.size() == 0) {
		lua_pushnil(L);
	}
	else
	{
		if (idx < 0)
			idx = 0;
		Entity_Player* player = game->GetPlayer(idx);
		if (!player) {
			lua_pushnil(L);
		}
		else
		{
			lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_ShowGenericLeaderboard) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_leaderboard.Show(1, &game->_scoreSheet, false, false);
	return 0;
}

// Reimplementation
// Fix for original MoveToRandomRoom function, that prevents crashes caused by a seed = 0
LUA_FUNCTION(Lua_MoveToRandomRoom) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	bool iAmErrorRoom = lua_toboolean(L, 2);
	int seed = (int)luaL_checkinteger(L, 3);
	if (seed == 0)
	{
		return luaL_error(L, "The given seed is not valid");
	}

	Entity_Player* player = nullptr;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY_PLAYER, "Player");
	}

	game->MoveToRandomRoom(iAmErrorRoom, seed, player);
	return 0;
}

LUA_FUNCTION(Lua_SetDonationModAngel) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_donationModAngel = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_SetDonationModGreed) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_donationModGreed = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_SetBloom) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	const int time = (int)luaL_checkinteger(L, 2);
	const float strength = (float)luaL_checknumber(L, 3);
	game->SetBloom(time, strength);

	return 0;
}

LUA_FUNCTION(Lua_SetDizzyAmount)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	float targetIntensity = (float)luaL_checknumber(L, 2);
	float currentIntensity = (float)luaL_optnumber(L, 3, game->_dizzyIntensity);

	game->_dizzyTargetIntensity = targetIntensity;
	game->_dizzyIntensity = currentIntensity;

	return 0;
}

LUA_FUNCTION(Lua_GetDizzyAmount)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushnumber(L, game->_dizzyIntensity);

	return 1;
}

LUA_FUNCTION(Lua_GameAddShopVisits) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int visitCount = (int)luaL_checkinteger(L, 2);
	game->_shopVisits += visitCount;

	if (game->_shopVisits >= 6 && !game->IsGreedMode()) {
		g_Manager->GetPersistentGameData()->TryUnlock(379); // Unlock schoolbag
	}
	return 0;
}

LUA_FUNCTION(Lua_GameGetShopVisits) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->_shopVisits);

	return 1;
}

LUA_FUNCTION(Lua_ClearErasedEnemies) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	game->_erasedEntities.clear();

	return 0;
}

LUA_FUNCTION(Lua_RecordPlayerCompletion) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int event = (int)luaL_checkinteger(L, 2);
	if (event < 0 || event > 17) {
		return luaL_error(L, "Bad CompletionType %d (valid range is 0-17)", event);
	}
	g_Manager->RecordPlayerCompletion(event);

	return 0;
}

LUA_FUNCTION(Lua_GetGenericPrompt) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	auto* toLua = (GenericPrompt*)lua_newuserdata(L, sizeof(GenericPrompt));
	*toLua = *game->GetGenericPrompt(); //
	luaL_setmetatable(L, lua::metatables::GenericPromptMT);
	return 1;
}

LUA_FUNCTION(Lua_ChainLightning) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");
	const float baseDamage = (float)luaL_optnumber(L, 3, 3.5f);
	BitSet128 flags { 0, 0, 0 ,0 };
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		flags = *lua::GetCData<BitSet128*>(L, 4, lua::ffi::CData[lua::ffi::CDataID::BITSET_128], "BitSet128");
	}
	Entity* spawner = nullptr;

	if (lua_type(L, 5) == LUA_TUSERDATA) {
		spawner = lua::GetLuabridgeUserdata<Entity*>(L, 5, lua::Metatables::ENTITY, "Entity");
	}

	lua::luabridge::UserdataPtr::push(L, game->ChainLightning(pos, baseDamage, flags, spawner), lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

LUA_FUNCTION(Lua_IsStartingFromState) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->_isStartingFromState);

	return 1;
}

LUA_FUNCTION(Lua_SetDifficulty) {
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int difficulty = luaL_checkinteger(L, 2);
	if (difficulty >= 0 && difficulty <= 3) {
		game->_difficulty = difficulty;
	}

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	luaL_Reg functions[] = {
		{ "Fadein", Lua_GameFadein },
		{ "Fadeout", Lua_GameFadeout },
		{ "BombDamage", Lua_GameBombDamage },
		{ "BombExplosionEffects", Lua_GameBombExplosionEffects },
		{ "BombTearflagEffects", Lua_GameBombTearflagEffects },
		{ "ButterBeanFart", Lua_GameButterBeanFart },
		{ "CharmFart", Lua_GameCharmFart },
		{ "Fart", Lua_GameFart },
		{ "GetNearestPlayer", Lua_GameGetNearestPlayer },
		{ "GetRandomPlayer", Lua_GameGetRandomPlayer },
		{ "MakeShockwave", Lua_GameMakeShockwave },
		{ "Spawn" , Lua_GameSpawn },
		{ "SpawnParticles" , Lua_GameSpawnParticles },
		{ "UpdateStrangeAttractor" , Lua_GameUpdateStrangeAttractor },
		{ "ClearErasedEnemies", Lua_ClearErasedEnemies },
		{ "AddShopVisits", Lua_GameAddShopVisits },
		{ "GetShopVisits", Lua_GameGetShopVisits },
		{ "AchievementUnlocksDisallowed", Lua_GameAchievementUnlocksDisallowed},
		{ "IsPauseMenuOpen", Lua_GameIsPauseMenuOpen},
		{ "GetPauseMenuState", Lua_GameGetPauseMenuState},
		{ "GetPlanetariumsVisited", Lua_GameGetPlanetariumsVisited},
		{ "IsHardMode", Lua_GameIsHardMode},
		{ "GetLastDevilRoomStage", Lua_GameGetLastDevilRoomStageFix},
		{ "GetLastLevelWithDamage", Lua_GetLastLevelWithDamageFix},
		{ "GetLastLevelWithoutHalfHp", Lua_GetLastLevelWithoutHalfHpFix},
		{ "GetDebugFlags", Lua_GameGetDebugFlags},
		{ "AddDebugFlags", Lua_GameAddDebugFlags},
		{ "SpawnBombCrater", Lua_GameSpawnBombCrater},
		{ "DevolveEnemy", Lua_GameDevolveEnemy},
		{ "IsGreedBoss", Lua_GameIsGreedBoss},
		{ "IsGreedFinalBoss", Lua_GameIsGreedFinalBoss},
		{ "StartStageTransition", lua_GameStartStageTransition},
		{ "AddErasedEnemy", Lua_GameAddErasedEnemy},
		{ "RemoveErasedEnemy", Lua_GameRemoveErasedEnemy},
		{ "IsErased", lua_GameIsErased},
		{ "GetCurrentColorModifier", Lua_GameGetCurrentColorModifier},
		{ "GetTargetColorModifier", Lua_GameGetTargetColorModifier},
		{ "GetLerpColorModifier", Lua_GameGetLerpColorModifier},
		{ "SetColorModifier", Lua_GameSetColorModifier},
		{ "IsRerun", Lua_GameIsRerun},
		{ "GetPlayer", Lua_GameGetPlayer},
		{ "ShowGenericLeaderboard", Lua_ShowGenericLeaderboard},
		{ "MoveToRandomRoom", Lua_MoveToRandomRoom},
		{ "SetDonationModAngel", Lua_SetDonationModAngel},
		{ "SetDonationModGreed", Lua_SetDonationModGreed},
		{ "SetBloom", Lua_SetBloom},
		{ "GetDizzyAmount", Lua_GetDizzyAmount},
		{ "SetDizzyAmount", Lua_SetDizzyAmount},
		{ "RecordPlayerCompletion", Lua_RecordPlayerCompletion},
		{ "GetGenericPrompt", Lua_GetGenericPrompt},
		{ "ChainLightning", Lua_ChainLightning},
		{ "IsStartingFromState", Lua_IsStartingFromState},
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GAME, functions);

	lua::RegisterVariableSetter(_state, lua::Metatables::GAME, "Difficulty", Lua_SetDifficulty);
	lua::RegisterVariableGetter(_state, lua::Metatables::GAME, "ScreenShakeOffset", Lua_GameGetScreenShakeOffset);
}
