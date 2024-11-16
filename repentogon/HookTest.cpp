
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>
#include <fstream>


#include <lua.hpp>
#include "LuaCore.h"
#include "Log.h"

#include "ImGuiFeatures/LogViewer.h"

extern LogViewer logViewer;

/*
HOOK_METHOD(Game, StartStageTransition, (bool samestage, int animation, Entity_Player *player) -> void) {
	logViewer.AddLog("[REPENTOGON]", "Game::StageStageTransition: samestage %c, animation: %d\n", samestage, animation);
	super(samestage, animation, player);
}
*/

/*
HOOK_METHOD(Level, SetStage, (int stageid, int alt) -> void) {
	logViewer.AddLog("[REPENTOGON]", "Level::SetStage: stageid: %d, alt: %d\n", stageid, alt);
	super(stageid, alt);
}
*/

/*HOOK_METHOD(Game, MakeShockwave, (const Vector &pos, float amp, float speed, int dur) -> void) {
	super(pos, amp, speed, dur);
	ZHL::Log("%g %g %g %g %d\n", pos.x,pos.y, amp, speed, dur);
}
*/

/*
HOOK_METHOD(Game, Update, () -> void) {
	super();
	ZHL::Log("I'm a buff baby that can dance like a man,\n");
	ZHL::Log("I can shake-ah my fanny, I can shake-ah my can!\n");
	ZHL::Log("I'm a tough tootin' baby, I can punch-ah yo buns!\n");
	ZHL::Log("Punch-ah yo buns, I can punch-ah yo buns!\n");
	
}
*/

/*HOOK_METHOD(GridEntity_Rock, Update, () -> void) {
	super();
	ZHL::Log("GridRock unk: %d type %d variant %d\n", _unk, _type, _variant);
}
*/


/*HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {
	super(type, variant, subtype, initSeed);
	ZHL::Log("Entity_Player::Init type %d variant %d sub %d and this thing %d\n", type,variant,subtype,initSeed);
};
*/

/*HOOK_METHOD(Entity_Player, Update, () -> void) {
	Weapon* wep = this->GetWeapon();
	int mem = wep->GetFireDelay();
	if (wep != nullptr) {
		ZHL::Log("wep delay %d\n", mem);
	}
	super();
}
*/

/*HOOK_STATIC(Isaac, CreateWeapon, (WeaponType wepType, Entity* ent) -> Weapon*) {
	Weapon* ret = super(wepType, ent);
	ZHL::Log("createweapon %d %d\n", wepType, ret->GetWeaponType());
	return ret;
}
*/

/*HOOK_METHOD(Weapon_MonstrosLung, Fire, (const Vector& pos, bool unk1, bool unk2)-> void) {
	ZHL::Log("Weapon::Fire %f %f %d %d %f\n", pos.x, pos.y, unk1, unk2, this->GetCharge());
	ZHL::Log("Weapon::DirVector: %f %f\n", this->GetDirection()->x, this->GetDirection()->y);
	super(pos, unk1, unk2);
}
*/

/*HOOK_METHOD(PlayerManager, SpawnCoPlayer2, (int unk) -> Entity_Player*) {
	ZHL::Log("PlayerManager::SpawnCoPlayer2 %d\n", unk);
	Entity_Player* ret = super(unk);

	return ret;
}
*/

/*HOOK_GLOBAL(CreateGridEntity, (GridEntityType type, unsigned int seed) -> GridEntity*) {
	GridEntity* ret = super(type, seed);
	ZHL::Log("CreateGridEntity type: %d, seed %d\n", type, seed);

	return ret;
}
*/

/*HOOK_METHOD(Game, Spawn, (unsigned int type, unsigned int variant, const Vector &pos, const Vector &vel, Entity* spawner, unsigned int sub, unsigned int seed, unsigned int unk) -> Entity*) {
	Entity* ret = super(type,variant,pos,vel,spawner,sub,seed, unk);
	ZHL::Log("Game::Spawn %d, %d, %f, %f, %f, %f,%d,%d,%d\n", type, variant, pos.x,pos.y, vel.x, vel.y, sub, seed, unk);

	return ret;
}
*/


/*HOOK_METHOD(Entity, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {
	super(type, variant, subtype, initSeed);
	ZHL::Log("Entity::Init type %d variant %d sub %d and this thing %d\n", type,variant,subtype,initSeed);
}
*/

/*
HOOK_METHOD(Entity, Update, () -> void) {
	super();
	//ZHL::Log("hellp");
}
*/

/*HOOK_METHOD(Room, GetDevilRoomChance, () -> float) {
	float ret = super();

	ret = 0.134f;
	ZHL::Log("GetDevilRoomChance: %f\n", ret);
	return ret;
}
*/

/*HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	super(109, charge, firsttime, slot, vardata);

	
	Weapon_MultiShotParams params =  g_Game->GetPlayer(0)->GetMultiShotParams(WEAPON_TEARS);
	ZHL::Log("%d\n", params.numTears);
	PosVel shotPosVel = g_Game->GetPlayer(0)->GetMultiShotPositionVelocity(0, WEAPON_TEARS, Vector(10, 0), 1.0f, params);
	ZHL::Log("(%f, %f) (%f, %f)", shotPosVel.pos.x, shotPosVel.pos.y, shotPosVel.vel.x, shotPosVel.vel.y);

}*/


/*HOOK_METHOD(Entity_Player, AddCoins, (int amount) -> void) {
    //ZHL::Log("EntityPlayer::AddCoins call %d\n", amount);
}

HOOK_METHOD(Entity_Player, AddBombs, (int amount) -> void) {
    //ZHL::Log("EntityPlayer::AddBombs call %d\n", amount);
	super(amount);
}

HOOK_METHOD(Entity_Player, AddKeys, (int amount) -> void) {
    //ZHL::Log("EntityPlayer::AddKeys call %d\n", amount); // please
	super(amount);
}*/

/*
HOOK_METHOD(Entity_Player, AddJarFlies, (int amount) -> void) {
    ZHL::Log("EntityPlayer::AddJarFlies call %d\n", amount); // func for lua?
}

HOOK_METHOD(Entity_Player, AddPrettyFly, () -> void) {
    ZHL::Log("EntityPlayer::AddPrettyFly call \n"); // func for lua?
}
*/

/*HOOK_STATIC(Entity_Player, GetMultiShotPositionVelocity, (int loopIndex, WeaponType weaponType, Vector shotDirection, float shotSpeed, Weapon_MultiShotParams multiShotParams) -> PosVel) {
	ZHL::Log("GetMultiShotPositionVelocity hook!\n");
	ZHL::Log("loopIndex %d weaponType %d ShotDirection (%f,%f) shotSpeed %f)", loopIndex, weaponType, shotDirection.x, shotDirection.y, shotSpeed);
	return super(loopIndex, weaponType, shotDirection, shotSpeed, multiShotParams);
}*/


/*HOOK_METHOD(PersistentGameData, IncreaseEventCounter, (int eEvent, int num) -> void) {
	ZHL::Log("IncreaseEventCounter %d %d\n", eEvent, num);
	super(eEvent, num);
}
*/

/*
HOOK_STATIC(Manager, RecordPlayerCompletion, (int eEvent) -> void, __stdcall) {
	super(eEvent);
}
*/

/* //Sinful stuff, DONT JUDGE! (or at least do it in silence)
int Lua_TEST(lua_State* L)
{
		int n = lua_gettop(L);
		int menuid = luaL_checkinteger(L, 1);
		char* x = (char *)(g_Manager + menuid); //(171924*4) anm2
		int* y = (int *)(g_Manager + menuid);
		float* z = (float *)(g_Manager + menuid);
		long* zz = (long *)(g_Manager + menuid);
		ZHL::Log("tests: %s \n", x);
		ZHL::Log("testd: %d \n", *y);
		ZHL::Log("testf: %f \n", *z);
		ZHL::Log("testl: %l \n", *zz);
		ZHL::Log("--------------------- \n");
		return 0;
}

 int RegisterTest(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "Test");
	lua_pushcfunction(L, Lua_TEST);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return 0;
}
HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterTest(state);
}
*/

/*HOOK_METHOD(ShapeRenderer, RenderLine, (Vector* pos1, Vector* pos2, KColor* col1, KColor* col2, const float& thickness) -> int) { // why the FUCK is this float a POINTER
	ZHL::Log("ShapeRenderer::RenderLine hook!\n");
	ZHL::Log("pos1 (%f, %f) pos2 (%f, %f) unk %f\n", pos1->x, pos1->y, pos2->x, pos2->y, thickness);
	return super(pos1, pos2, col1, col2, thickness);
}*/

/*HOOK_METHOD(PlayerHUDActiveItem, RenderGfx, (SourceQuad* source, DestinationQuad* dest, const ColorMod& color) -> void) {
	ZHL::Log("RenderGfx: %f, %f %f %f %f, %f %f %f\n", source->_topLeft.x, source->_topLeft.y, source->_topRight.x, source->_topRight.y, source->_bottomLeft.x, source->_bottomLeft.y, source->_bottomRight.x, source->_bottomRight.y);
	source->_topLeft.x = source->_topLeft.x + 16.0f;
	source->_topRight.x = source->_topRight.x + 16.0f;
	source->_bottomLeft.x = source->_bottomLeft.x + 16.0f;
	source->_bottomRight.x = source->_bottomRight.x + 16.0f;
	
	super(source, dest, color);
}
*/

/*HOOK_STATIC(GridEntity_Rock, SpawnDrops, (const Vector& pos, int GridEntityType, int GridEntityVariant, unsigned int Seed, bool Unk, int BackdropType) -> void) {
	ZHL::Log("GridEntity::SpawnDrops %f %f, %d %d %d %d %d \n", pos.x, pos.y, GridEntityType, GridEntityVariant, Seed, Unk, BackdropType );
	super(pos, GridEntityType, GridEntityVariant, Seed, Unk, BackdropType);
}
*/

extern void LuaReset();
bool netStartInitialized = false; // temp
/*HOOK_METHOD(Manager, ExecuteStartGame, () -> void) {
	if (this->GetNetStart() && !netStartInitialized) {
		ZHL::Log("Resetting Lua- please be patient...\n");
		//LuaReset();
		ZHL::Log("Done.\n");
		netStartInitialized = true;
	}
	super();
}*/

// Too crashy... let's try earlier.
/*HOOK_METHOD(Game, NetStart, (void* unk, int challenge, Seeds seed, unsigned int difficulty, GameState* state) -> void) {
	if (!netStartInitialized) {
		ZHL::Log("Resetting Lua- please be patient...\n");
		LuaReset();
		ZHL::Log("Done.\n"); 

		netStartInitialized = true;
	}
	super(unk, challenge, seed, difficulty, state);
}*/

/*
HOOK_METHOD(LevelGenerator, get_neighbor_candidates, (vector_LevelGenRoom* neighbors, uint32_t generationIndex, bool unk) -> void) {
	super(neighbors, generationIndex, unk);
	LevelGenerator_Room& room = (*GetAllRooms())[generationIndex];
	// FILE* f = fopen("repentogon.log", "a");
	ZHL::Logger logger;
	logger.Log("Computing neighbors for room at position %d (assumed to be at (%d, %d))\n", generationIndex, room._gridColIdx, room._gridLineIdx);
	for (size_t i = 0; i < neighbors->size(); ++i) {
		logger.Log("\t(%d, %d, %d)\n", (*neighbors)[i]._gridColIdx, (*neighbors)[i]._gridLineIdx, (*neighbors)[i]._shape);
	}
}
*/

/*HOOK_METHOD(Entity, GetCollisionCapsule, (Capsule* capsule, const Vector& vec)-> Capsule*) {
	Capsule* ret = super(capsule, vec);
	ZHL::Log("GetCollisionCapsule: %f %f %f %f\n", ret->GetVec1()->x, ret->GetVec1()->y, ret->GetVec2()->x, ret->GetVec2()->y);
	return ret;
}
*/
HOOK_METHOD(Leaderboard, ProcessInput, () -> void) {
	super();

	if (_dailyChallenge._id == 1 && _isLeaderboardClosing == 0) {
		if (g_InputManagerBase.IsActionTriggered(14, -1, 0)) {
			//g_Game->FadeOut(.08f, 2);
			_displayState = 4;
			_isLeaderboardClosing = true;
			_dailyChallenge._id = 0;
		}
	}
}