
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>


#include <lua.hpp>
#include "LuaCore.h"
/*
HOOK_GLOBAL(GetXMLNode,(char* thi,int whatevs,char* name)->char**) {
	printf("name: %s \n", name);
	char** returnval = super(thi,whatevs,name);
	printf("ballU: %s \n",*returnval);
	return returnval;
}*/

//TODO 0x1C _numBossesPerWave
HOOK_METHOD(Ambush, SpawnBossrushWave, () -> void) {
	*(int*)(char*)(this + 0x18) = 30;
	printf("Unknown @ 0x18 value is %d\n", *(int*)(char*)(this + 0x18));
	printf("Unknown @ 0x24 value is %d\n", *(int*)(char*)(this + 0x24));
	super();
}

HOOK_METHOD(Ambush, SpawnWave, () -> void) {
	printf("Unknown @ 0x18 value is %d\n", *(int*)(char*)(this + 0x18));
	printf("Unknown @ 0x24 value is %d\n", *(int*)(char*)(this + 0x24));
	super();
}

//Completion Makrs Test
HOOK_METHOD(PauseScreen, Render, () -> void) {
	super();
	int playertype = g_Game->GetPlayer(0)->GetPlayerType();
	if ((playertype > 40) && (this->status !=2)) {
		NullFrame* nul = this->GetANM2()->GetNullFrame("CompletionWidget"); 
		Vector* widgtpos = nul->GetPos();
		Vector* widgtscale = nul->GetScale();
		CompletionWidget* cmpl = this->GetCompletionWidget();

		ANM2* anm = cmpl->GetANM2();

		for (int i = 1; i <= 11; i++) {
			anm->SetLayerFrame(i, 1);
		}
		anm->Update();
		cmpl->Render(new Vector((g_WIDTH * 0.6) + widgtpos->x, (g_HEIGHT * 0.5) +widgtpos->y), widgtscale);
	}
}

int selectedchar = 0;
HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	selectedchar = CharacterId;
	super(CharacterId, RenderPos, DefaultSprite);
}
HOOK_METHOD(Menu_Character, Render, () -> void) {
	super();
	CompletionWidget* cmpl = this->GetCompletionWidget();
	if(this->charaslot > 17){
		Vector* ref = (Vector *)(g_MenuManager + 60);
		Vector* cpos = new Vector(ref->x - 80, ref->y + 894);
		ANM2* anm = cmpl->GetANM2();
		
		for (int i = 1; i <= 11; i++) {
			anm->SetLayerFrame(i, 1);
		}
		anm->Update();	
		cmpl->Render(new Vector(ref->x + 80, ref->y + 860), new Vector(1, 1));

	}
}
//Completion Makrs Test END

HOOK_METHOD(ANM2, SetLayerFrame, (int x,int y) -> void) {
	//this->Render(new Vector(100, 100), new Vector(0, 0), new Vector(0, 0)); // this crashes
	super(x,y);
}

HOOK_METHOD(ANM2, Render, (Vector* x,Vector* y, Vector* z) -> void) {
	for (int i = 1; i == 101; i++) {
        this->SetLayerFrame(i, 1);
	}
	super(x,y,z);
}

HOOK_METHOD(Game, StartStageTransition, (bool samestage, int animation, Entity_Player *player) -> void) {
	printf("Stage Transition \n");
	super(samestage, animation, player);
}

HOOK_METHOD(Level, SetStage, (int stageid, int alt) -> void) {
	printf("Setting Stage \n");
	super(stageid, alt);
}


/*HOOK_METHOD(Game, MakeShockwave, (const Vector &pos, float amp, float speed, int dur) -> void) {
	super(pos, amp, speed, dur);
	printf("%g %g %g %g %d\n", pos.x,pos.y, amp, speed, dur);
}
*/

HOOK_METHOD(Game, Update, () -> void) {
	super();
	/*printf("I'm a buff baby that can dance like a man,\n");
	printf("I can shake-ah my fanny, I can shake-ah my can!\n");
	printf("I'm a tough tootin' baby, I can punch-ah yo buns!\n");
	printf("Punch-ah yo buns, I can punch-ah yo buns!\n");
	*/
}

/*HOOK_METHOD(GridEntity_Rock, Update, () -> void) {
	super();
	printf("GridRock unk: %d type %d variant %d\n", _unk, _type, _variant);
}
*/


/*HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {
	super(type, variant, subtype, initSeed);
	printf("Entity_Player::Init type %d variant %d sub %d and this thing %d\n", type,variant,subtype,initSeed);
};
*/

/*HOOK_METHOD(Entity_Player, Update, () -> void) {
	super();
}
*/

/*HOOK_METHOD(PlayerManager, SpawnCoPlayer2, (int unk) -> Entity_Player*) {
	printf("PlayerManager::SpawnCoPlayer2 %d\n", unk);
	Entity_Player* ret = super(unk);

	return ret;
}
*/

/*HOOK_GLOBAL(CreateGridEntity, (GridEntityType type, unsigned int seed) -> GridEntity*) {
	GridEntity* ret = super(type, seed);
	printf("CreateGridEntity type: %d, seed %d\n", type, seed);

	return ret;
}
*/

/*HOOK_METHOD(Game, Spawn, (unsigned int type, unsigned int variant, const Vector &pos, const Vector &vel, Entity* spawner, unsigned int sub, unsigned int seed, unsigned int unk) -> Entity*) {
	Entity* ret = super(type,variant,pos,vel,spawner,sub,seed, unk);
	printf("Game::Spawn %d, %d, %f, %f, %f, %f,%d,%d,%d\n", type, variant, pos.x,pos.y, vel.x, vel.y, sub, seed, unk);

	return ret;
}
*/


/*HOOK_METHOD(Entity, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {
	super(type, variant, subtype, initSeed);
	printf("Entity::Init type %d variant %d sub %d and this thing %d\n", type,variant,subtype,initSeed);
}
*/

HOOK_METHOD(Entity, Update, () -> void) {
	super();
	//printf("hellp");
}

/*HOOK_METHOD(Room, GetDevilRoomChance, () -> float) {
	float ret = super();

	ret = 0.134f;
	printf("GetDevilRoomChance: %f\n", ret);
	return ret;
}
*/

/*HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	super(109, charge, firsttime, slot, vardata);

	
	Weapon_MultiShotParams params =  g_Game->GetPlayer(0)->GetMultiShotParams(WEAPON_TEARS);
	printf("%d\n", params.numTears);
	PosVel shotPosVel = g_Game->GetPlayer(0)->GetMultiShotPositionVelocity(0, WEAPON_TEARS, Vector(10, 0), 1.0f, params);
	printf("(%f, %f) (%f, %f)", shotPosVel.pos.x, shotPosVel.pos.y, shotPosVel.vel.x, shotPosVel.vel.y);

}*/


/*HOOK_METHOD(Entity_Player, AddCoins, (int amount) -> void) {
    //printf("EntityPlayer::AddCoins call %d\n", amount);
}

HOOK_METHOD(Entity_Player, AddBombs, (int amount) -> void) {
    //printf("EntityPlayer::AddBombs call %d\n", amount);
	super(amount);
}

HOOK_METHOD(Entity_Player, AddKeys, (int amount) -> void) {
    //printf("EntityPlayer::AddKeys call %d\n", amount); // please
	super(amount);
}*/

/*
HOOK_METHOD(Entity_Player, AddJarFlies, (int amount) -> void) {
    printf("EntityPlayer::AddJarFlies call %d\n", amount); // func for lua?
}

HOOK_METHOD(Entity_Player, AddPrettyFly, () -> void) {
    printf("EntityPlayer::AddPrettyFly call \n"); // func for lua?
}
*/

/*HOOK_STATIC(Entity_Player, GetMultiShotPositionVelocity, (int loopIndex, WeaponType weaponType, Vector shotDirection, float shotSpeed, Weapon_MultiShotParams multiShotParams) -> PosVel) {
	printf("GetMultiShotPositionVelocity hook!\n");
	printf("loopIndex %d weaponType %d ShotDirection (%f,%f) shotSpeed %f)", loopIndex, weaponType, shotDirection.x, shotDirection.y, shotSpeed);
	return super(loopIndex, weaponType, shotDirection, shotSpeed, multiShotParams);
}*/


/*HOOK_METHOD(PersistentGameData, IncreaseEventCounter, (int eEvent, int num) -> void) {
	printf("IncreaseEventCounter %d %d\n", eEvent, num);
	super(eEvent, num);
}
*/

HOOK_METHOD(Manager, AchievementUnlocksDisallowed, (bool unk) -> bool) {
	return false;
}
HOOK_METHOD(Manager, RecordPlayerCompletion, (int eEvent) -> void) {
	super(eEvent);
}


/* //Sinful stuff, DONT JUDGE! (or at least do it in silence)
int Lua_TEST(lua_State* L)
{
		int n = lua_gettop(L);
		int menuid = luaL_checkinteger(L, 1);
		char* x = (char *)(g_Manager + menuid); //(171924*4) anm2
		int* y = (int *)(g_Manager + menuid);
		float* z = (float *)(g_Manager + menuid);
		long* zz = (long *)(g_Manager + menuid);
		printf("tests: %s \n", x);
		printf("testd: %d \n", *y);
		printf("testf: %f \n", *z);
		printf("testl: %l \n", *zz);
		printf("--------------------- \n");
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