#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_Game_AddDevilRoomDeal() {
		g_Game->AddDevilRoomDeal();
	}

	void L_Game_AddEncounteredBoss(unsigned int Boss, unsigned int Variant) {
		g_Game->AddEncounteredBoss(Boss, Variant);
	}

	void L_Game_AddPixelation(int Duration) {
		g_Game->AddPixelation(Duration);
	}

	void L_Game_AddStageWithoutDamage() {
		g_Game->_stagesWithoutDamage += 1;
	}

	void L_Game_AddStagesWithoutHeartsPicked() {
		g_Game->_stagesWithoutHeartsPicked += 1;
	}

	void L_Game_AddTreasureRoomsVisited() {
		g_Game->_treasureRoomsVisited += 1;
	}

	void L_Game_BombDamage(Vector* Position, float Damage, float Radius, bool LineCheck, Entity* Source, BitSet128* TearFlags, unsigned int DamageFlagsL, unsigned int DamageFlagsH, bool DamageSource) {
		g_Game->BombDamage(Position, Damage, Radius, LineCheck, Source, *TearFlags, DamageFlagsL, DamageFlagsH, DamageSource);
	}

	void L_Game_BombExplosionEffects(Vector* Position, float Damage, BitSet128* TearFlags, ColorMod* Color, Entity* Source, float RadiusMult, bool LineCheck, unsigned int DamageFlagsL, unsigned int DamageFlagsH, bool DamageSource) {
		g_Game->BombExplosionEffects(Position, Damage, *TearFlags, Color, Source, RadiusMult, LineCheck, DamageFlagsL, DamageFlagsH, DamageSource);
	}

	void L_Game_BombTearflagEffects(Vector* Position, float Radius, BitSet128* TearFlags, Entity* Source, float RadiusMult) {
		g_Game->BombTearflagEffects(Position, Radius, *TearFlags, Source, RadiusMult);
	}

	void L_Game_ButterBeanFart(Vector* Position, float Radius, Entity* Source, bool ShowEffect, bool DoSuperKnockback) {
		g_Game->ButterBeanFart(Position, Radius, Source, ShowEffect, DoSuperKnockback);
	}

	void L_Game_ChangeRoom(int RoomIndex, int Dimension) {
		g_Game->ChangeRoom(RoomIndex, Dimension);
	}

	void L_Game_CharmFart(Vector* Position, float Radius, Entity* Source) {
		g_Game->CharmFart(Position, Radius, Source);
	}

	void L_Game_ClearDonationModAngel() {
		g_Game->_donationModAngel = 0;
	}

	void L_Game_ClearDonationModGreed() {
		g_Game->ClearDonationModGreed();
	}

	void L_Game_ClearStagesWithoutDamage() {
		g_Game->_stagesWithoutDamage = 0;
	}

	void L_Game_ClearStagesWithoutHeartsPicked() {
		g_Game->_stagesWithoutHeartsPicked = 0;
	}

	void L_Game_Darken(float Darkness, int Timeout) {
		g_Game->Darken(Darkness, Timeout);
	}

	void L_Game_DonateAngel(int Donate) {
		g_Game->_donationModAngel += Donate;
	}

	void L_Game_DonateGreed(int Donate) {
		g_Game->_donationModGreed += Donate;
	}

	void L_Game_End(unsigned int Ending) {
		g_Game->End(Ending);
	}

	void L_Game_Fadein(float Speed) {
		g_Game->Fadein(Speed);
	}

	void L_Game_Fadeout(float Speed, unsigned int FadeoutTarget) {
		g_Game->Fadeout(Speed, FadeoutTarget);
	}

	void L_Game_Fart(Vector* Position, float Radius, Entity* Source, float FartScale, int FartSubType, ColorMod* FartColor) {
		g_Game->Fart(Position, Radius, Source, FartScale, FartSubType, FartColor);
	}

	void L_Game_FinishChallenge() {
		g_Game->FinishChallenge();
	}

	Ambush* L_Game_GetAmbush() {
		return &g_Game->_ambush;
	}

	float L_Game_GetDarknessModifier() {
		return g_Game->_darknessModifier;
	}

	int L_Game_GetDevilRoomDeals() {
		return g_Game->_devilRoomDeals;
	}

	int L_Game_GetDonationModAngel() {
		return g_Game->_donationModAngel;
	}

	int L_Game_GetDonationModGreed() {
		return g_Game->_donationModGreed;
	}

	Font* L_Game_GetFont() {
		return &g_Game->_font_Droid;
	}

	unsigned int L_Game_GetFrameCount() {
		return g_Game->_frameCount;
	}

	unsigned int L_Game_GetGreedBossWaveNum() {
		return g_Game->GetGreedBossWaveNum();
	}

	unsigned int L_Game_GetGreedWavesNum() {
		return g_Game->GetGreedWavesNum();
	}

	HUD* L_Game_GetHUD() {
		return &g_Game->_hud;
	}

	ItemOverlay* L_Game_GetItemOverlay() {
		return &g_Game->_itemOverlay;
	}

	ItemPool* L_Game_GetItemPool() {
		return &g_Game->_itemPool;
	}

	unsigned int L_Game_GetLastDevilRoomStage() {
		return g_Game->_lastDevilRoomStage;
	}

	unsigned int L_Game_GetLastLevelWithDamage() {
		return g_Game->_lastLevelWithDamage;
	}

	unsigned int L_Game_GetLastLevelWithoutHalfHp() {
		return g_Game->_lastLevelWithoutHalfHp;
	}

	Level* L_Game_GetLevel() {
		// we've gotta fucking change this, man
		return (Level*)&g_Game;
	}

	Entity_Player* L_Game_GetNearestPlayer(Vector* Pos) {
		return g_Game->GetNearestPlayer(Pos);
	}

	unsigned int L_Game_GetNumEncounteredBosses() {
		return g_Game->GetNumEncounteredBosses();
	}

	unsigned int L_Game_GetNumPlayers() {
		return g_Game->GetNumPlayers();
	}

	Entity_Player* L_Game_GetPlayer(int Index) {
		return g_Game->GetPlayer(Index);
	}

	Entity_Player* L_Game_GetRandomPlayer(Vector* Pos, float Radius) {
		return g_Game->GetRandomPlayer(Pos, Radius);
	}

	Room* L_Game_GetRoom() {
		return g_Game->_level._room;
	}

	int L_Game_GetScreenShakeCountdown() {
		return g_Game->_screenShakeCountdown;
	}

	Seeds* L_Game_GetSeeds() {
		return &g_Game->_seedEffects;
	}

	int L_Game_GetStagesWithoutDamage() {
		return g_Game->_stagesWithoutDamage;
	}

	int L_Game_GetStagesWithoutHeartsPicked() {
		return g_Game->_stagesWithoutHeartsPicked;
	}

	bool L_Game_GetStateFlag(unsigned int GameStateFlag) {
		return g_Game->GetStateFlag(GameStateFlag);
	}

	float L_Game_GetTargetDarkness() {
		return g_Game->_targetDarkness;
	}

	int L_Game_GetTreasureRoomVisitCount() {
		return g_Game->_treasureRoomsVisited;
	}

	int L_Game_GetVictoryLap() {
		return *g_Game->GetVictoryLap();
	}

	bool L_Game_HasEncounteredBoss(int Boss, int Variant) {
		return g_Game->HasEncounteredBoss(Boss, Variant);
	}

	bool L_Game_HasHallucination() {
		return g_Game->_hallucinationTimeout > 0;
	}

	bool L_Game_IsGreedMode() {
		return g_Game->IsGreedMode();
	}

	bool L_Game_IsPaused() {
		return g_Game->IsPaused();
	}

	void L_Game_MakeShockwave(Vector* Position, float Amplitude, float Speed, int Duration) {
		g_Game->MakeShockwave(*Position, Amplitude, Speed, Duration);
	}

	void L_Game_MoveToRandomRoom(bool IAMErrorRoom, unsigned int Seed, Entity_Player* Player) {
		g_Game->MoveToRandomRoom(IAMErrorRoom, Seed, Player);
	}

	void L_Game_NextVictoryLap() {
		g_Game->NextVictoryLap();
	}

	void L_Game_Render() {
		g_Game->Render();
	}

	bool L_Game_RerollEnemy(Entity* Enemy) {
		return g_Game->RerollEnemy(Enemy, false);
	}

	void L_Game_RerollLevelCollectibles() {
		g_Game->RerollLevelCollectibles();
	}

	void L_Game_RerollLevelPickups(unsigned int Seed) {
		g_Game->RerollLevelPickups(Seed);
	}

	void L_Game_SetLastDevilRoomStage(int Stage) {
		g_Game->_lastDevilRoomStage = Stage;
	}

	void L_Game_SetLastLevelWithDamage(int Stage) {
		g_Game->_lastLevelWithDamage = Stage;
	}

	void L_Game_SetLastLevelWithoutHalfHp(int Stage) {
		g_Game->_lastLevelWithoutHalfHp = Stage;
	}

	void L_Game_SetStateFlag(unsigned int GameStateFlag, bool Val) {
		g_Game->SetStateFlag(GameStateFlag, Val);
	}

	void L_Game_ShakeScreen(int Timeout) {
		g_Game->ShakeScreen(Timeout);
	}

	void L_Game_ShowFortune() {
		g_Game->ShowFortuneRandom("Fortune_General");
	}

	void L_Game_ShowHallucination(int FrameCount, unsigned int BackdropType) {
		g_Game->ShowHallucination(FrameCount, BackdropType);
	}

	void L_Game_ShowRule() {
		g_Game->ShowFortuneRandom("Fortune_Rules");
	}

	Entity* L_Game_Spawn(unsigned int Type, unsigned int Variant, Vector* Position, Vector* Velocity, Entity* Spawner, unsigned int SubType, unsigned int Seed) {
		return g_Game->Spawn(Type, Variant, *Position, *Velocity, Spawner, SubType, Seed, 0);
	}

	Entity_NPC* L_Game_SpawnEntityDesc(EntityDesc* Desc, Vector* Position, Entity* Spawner) {
		return g_Game->SpawnEntityDesc(Desc, Position, Spawner);
	}

	void L_Game_SpawnParticles(Vector* Pos, unsigned int ParticleType, unsigned int NumParticles, float Speed, ColorMod* Color, float Height, int SubType) {
		g_Game->SpawnParticles(Pos, ParticleType, NumParticles, Speed, Color, Height, SubType);
	}

	void L_Game_StartRoomTransition(int RoomIndex, int Direction, unsigned int Animation, Entity_Player* Player, int Dimension) {
		g_Game->StartRoomTransition(RoomIndex, Direction, Animation, Player, Dimension);
	}

	void L_Game_StartStageTransition(bool SameStage, int TransitionOverride, Entity_Player* Player) {
		g_Game->StartStageTransition(SameStage, TransitionOverride, Player);
	}

	void L_Game_Update() {
		g_Game->Update();
	}

	void L_Game_UpdateStrangeAttractor(Vector* Position, float Force, float Radius) {
		g_Game->UpdateStrangeAttractor(Position, Force, Radius);
	}

	int L_Game_GetBlueWombParTime() {
		return g_Game->_blueWombParTime;
	}

	void L_Game_SetBlueWombParTime(int value) {
		g_Game->_blueWombParTime = value;
	}

	int L_Game_GetBossRushParTime() {
		return g_Game->_bossRushParTime;
	}

	void L_Game_SetBossRushParTime(int value) {
		g_Game->_bossRushParTime = value;
	}

	int L_Game_GetChallenge() {
		return g_Game->_challenge;
	}

	void L_Game_SetChallenge(int value) {
		g_Game->_challenge = value;
	}

	int L_Game_GetDifficulty() {
		return g_Game->_difficulty;
	}

	void L_Game_SetDifficulty(int value) {
		g_Game->_difficulty = value;
	}

	Vector*  L_Game_GetScreenShakeOffset() {
		return &g_Game->_screenShakeOffset;
	}

	void L_Game_SetScreenShakeOffset(Vector* offset) {
		g_Game->_screenShakeOffset = *offset;
	}

	int L_Game_GetTimeCounter() {
		return g_Game->_timeCounter;
	}

	void L_Game_SetTimeCounter(int value) {
		g_Game->_timeCounter = value;
	}
}