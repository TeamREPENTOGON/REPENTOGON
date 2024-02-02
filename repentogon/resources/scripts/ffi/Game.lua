ffi.cdef[[
// stubs
typedef struct {void *_;} Ambush; 
typedef struct {void *_;} BitSet128; 
typedef struct {void *_;} Font; 
typedef struct {void *_;} EntityDesc; 
typedef struct {void *_;} Entity_NPC; 
typedef struct {void *_;} Entity_Player; 
typedef struct {void *_;} HUD; 
typedef struct {void *_;} ItemOverlay;
typedef struct {void *_;} ItemPool;
typedef struct {void *_;} Level;
typedef struct {void *_;} Room;
typedef struct {void *_;} Seeds;

void L_Game_AddDevilRoomDeal();
void L_Game_AddEncounteredBoss(unsigned int, unsigned int);
void L_Game_AddPixelation(int);
void L_Game_AddStageWithoutDamage();
void L_Game_AddStagesWithoutHeartsPicked();
void L_Game_AddTreasureRoomsVisited();
void L_Game_BombDamage(Vector*, float, float, bool, Entity*, BitSet128*, unsigned int, unsigned int, bool);
void L_Game_BombExplosionEffects(Vector*, float, BitSet128*, ColorMod*, Entity*, float, bool, unsigned int, unsigned int, bool);
void L_Game_BombTearflagEffects(Vector*, float, BitSet128*, Entity*, float);
void L_Game_ButterBeanFart(Vector*, float, Entity*, bool, bool);
void L_Game_ChangeRoom(int, int);
void L_Game_CharmFart(Vector*, float, Entity*);
void L_Game_ClearDonationModAngel();
void L_Game_ClearDonationModGreed();
void L_Game_ClearStagesWithoutDamage();
void L_Game_ClearStagesWithoutHeartsPicked();
void L_Game_Darken(float, int);
void L_Game_DonateAngel(int);
void L_Game_DonateGreed(int);
void L_Game_End(unsigned int);
void L_Game_Fadein(float);
void L_Game_Fadeout(float, unsigned int);
void L_Game_Fart(Vector*, float, Entity*, float, int, ColorMod*);
void L_Game_FinishChallenge();
Ambush* L_Game_GetAmbush();
float L_Game_GetDarknessModifier();
int L_Game_GetDevilRoomDeals();
int L_Game_GetDonationModAngel();
int L_Game_GetDonationModGreed();
Font* L_Game_GetFont();
unsigned int L_Game_GetFrameCount();
unsigned int L_Game_GetGreedBossWaveNum();
unsigned int L_Game_GetGreedWavesNum();
HUD* L_Game_GetHUD();
ItemOverlay* L_Game_GetItemOverlay();
ItemPool* L_Game_GetItemPool();
unsigned int L_Game_GetLastDevilRoomStage();
unsigned int L_Game_GetLastLevelWithDamage();
unsigned int L_Game_GetLastLevelWithoutHalfHp();
Level* L_Game_GetLevel();
Entity_Player* L_Game_GetNearestPlayer(Vector*);
unsigned int L_Game_GetNumEncounteredBosses();
unsigned int L_Game_GetNumPlayers();
Entity_Player* L_Game_GetPlayer(int);
Entity_Player* L_Game_GetRandomPlayer(Vector*, float);
Room* L_Game_GetRoom();
int L_Game_GetScreenShakeCountdown();
Seeds* L_Game_GetSeeds();
int L_Game_GetStagesWithoutDamage();
int L_Game_GetStagesWithoutHeartsPicked();
bool L_Game_GetStateFlag(unsigned int);
float L_Game_GetTargetDarkness();
int L_Game_GetTreasureRoomVisitCount();
int L_Game_GetVictoryLap();
bool L_Game_HasEncounteredBoss(int, int);
bool L_Game_HasHallucination();
bool L_Game_IsGreedMode();
bool L_Game_IsPaused();
void L_Game_MakeShockwave(Vector*, float, float, int);
void L_Game_MoveToRandomRoom(bool, unsigned int, Entity_Player*);
void L_Game_NextVictoryLap();
void L_Game_Render();
bool L_Game_RerollEnemy(Entity*);
void L_Game_RerollLevelCollectibles();
void L_Game_RerollLevelPickups(unsigned int);
void L_Game_SetLastDevilRoomStage(int);
void L_Game_SetLastLevelWithDamage(int);
void L_Game_SetLastLevelWithoutHalfHp(int);
void L_Game_SetStateFlag(unsigned int, bool);
void L_Game_ShakeScreen(int);
void L_Game_ShowFortune();
void L_Game_ShowHallucination(int, unsigned int);
void L_Game_ShowRule();
Entity* L_Game_Spawn(unsigned int, unsigned int, Vector*, Vector*, Entity*, unsigned int, unsigned int);
Entity_NPC* L_Game_SpawnEntityDesc(EntityDesc*, Vector*, Entity*);
void L_Game_SpawnParticles(Vector*, unsigned int, unsigned int, float, ColorMod*, float, int);
void L_Game_StartRoomTransition(int, int, unsigned int, Entity_Player*);
void L_Game_StartStageTransition(bool, unsigned int, Entity_Player*);
void L_Game_Update();
void L_Game_UpdateStrangeAttractor(Vector*, float, float);
int L_Game_GetBlueWombParTime();
void L_Game_SetBlueWombParTime(int);
int L_Game_GetBossRushParTime();
void L_Game_SetBossRushParTime(int);
int L_Game_GetChallenge();
void L_Game_SetChallenge(int);
int L_Game_GetDifficulty();
void L_Game_SetDifficulty(int);
Vector* L_Game_GetScreenShakeOffset();
void L_Game_SetScreenShakeOffset(Vector*);
int L_Game_GetTimeCounter();
void L_Game_SetTimeCounter();
]]
local repentogon = ffidll

GameFuncs = {}

function GameFuncs:AddDevilRoomDeal() 
    repentogon.L_Game_AddDevilRoomDeal()
end

function GameFuncs:AddEncounteredBoss(id, variant)
	ffichecks.checknumber(1, id)
	variant = ffichecks.optnumber(2, variant, 0)
    repentogon.L_Game_AddEncounteredBoss(id, variant)
end

function GameFuncs:AddPixelation(duration)
	ffichecks.checknumber(1, duration)
    repentogon.L_Game_AddPixelation(duration)
end

function GameFuncs:AddStageWithoutDamage() 
    repentogon.L_Game_AddStageWithoutDamage()
end

function GameFuncs:AddStagesWithoutHeartsPicked() 
    repentogon.L_Game_AddStagesWithoutHeartsPicked()
end

function GameFuncs:AddTreasureRoomsVisited() 
    repentogon.L_Game_AddTreasureRoomsVisited()
end

function GameFuncs:BombDamage(pos, damage, radius, linecheck, source, tearflags, damageflags, damagesource)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, damage)
	ffichecks.checknumber(3, radius)
	linecheck = ffichecks.optboolean(linecheck, true)
	source = ffichecks.optcdata(source, "Entity", nil)
	tearflags = ffichecks.optcdata(tearflags, "BitSet128", BitSet128.Default)
	damageflags = ffichecks.optnumber(damageflags, 0)
	damagesource = ffichecks.optboolean(damagesource, false)
	
    repentogon.L_Game_BombDamage(pos, damage, radius, linecheck, source, tearflags, damageflags, damagesource);
end

function GameFuncs:BombExplosionEffects(pos, damage, tearflags, color, source, radiusmult, linecheck, damagesource, damageflags)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, damage)
	tearflags = ffichecks.optcdata(tearflags, "BitSet128", BitSet128.Default)
	color = ffichecks.optcdata(color, "ColorMod", Color.Default)
	source = ffichecks.optcdata(source, "Entity", nil)
	radiusmult = ffichecks.optnumber(radiusmult, 1)
	linecheck = ffichecks.optboolean(linecheck, true)
	damagesource = ffichecks.optboolean(damagesource, false)
	damageflags = ffichecks.optnumber(damageflags, 0)
	
    repentogon.L_Game_BombExplosionEffects(pos, damage, tearflags, color, source, radiusmult, linecheck, damagesource, damageflags)
end

function GameFuncs:BombTearflagEffects(pos, radius, tearflags, source, radiusmult)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, radius)
	tearflags = ffichecks.optcdata(tearflags, "BitSet128", BitSet128.Default)
	source = ffichecks.optcdata(source, "Entity", nil)
	radiusmult = ffichecks.optnumber(radiusmult, 1)
	
    repentogon.L_Game_BombTearflagEffects(pos, radius, tearflags, source, radiusmult)
end

function GameFuncs:ButterBeanFart(pos, radius, source, showeffect, dosuperknockback)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, radius)
	source = ffichecks.optcdata(source, "Entity", nil)
	showeffect = ffichecks.optboolean(showeffect, true)
	dosuperknockback = ffichecks.optboolean(dosuperknockback, false)
	
    repentogon.L_Game_ButterBeanFart(pos, radius, source, showeffect, dosuperknockback)
end

function GameFuncs:ChangeRoom(idx, dimension)
	ffichecks.checknumber(1, idx)
	dimension = ffichecks.optnumber(dimension, -1)
	
    repentogon.L_Game_ChangeRoom(idx, dimension)
end

function GameFuncs:CharmFart(pos, radius, source)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, radius)
	source = ffichecks.optcdata(source, "Entity", nil)
	
    repentogon.L_Game_ButterBeanFart(pos, radius, source)
end

function GameFuncs:ClearDonationModAngel() 
    repentogon.L_Game_ClearDonationModAngel()
end

function GameFuncs:ClearDonationModGreed() 
    repentogon.L_Game_ClearDonationModGreed()
end

function GameFuncs:ClearStagesWithoutDamage() 
    repentogon.L_Game_ClearStagesWithoutDamage()
end

function GameFuncs:ClearStagesWithoutHeartsPicked() 
    repentogon.L_Game_ClearStagesWithoutHeartsPicked()
end

function GameFuncs:Darken(darkness, timeout)
	ffichecks.checknumber(1, darkness)
	ffichecks.checknumber(2, timeout)
	
    repentogon.L_Game_Darken(darkness, timeout)
end

function GameFuncs:DonateAngel(amount)
	ffichecks.checknumber(1, amount)
	
    repentogon.L_Game_DonateAngel(amount)
end

function GameFuncs:DonateGreed(amount)
	ffichecks.checknumber(1, amount)
	
    repentogon.L_Game_DonateGreed(amount)
end

function GameFuncs:End(id)
	ffichecks.checknumber(1, id)
	
    repentogon.L_Game_End(id)
end

function GameFuncs:Fadein(speed)
	ffichecks.checknumber(1, speed)
	
    repentogon.L_Game_Fadein(speed)
end

function GameFuncs:Fadeout(speed, target)
	ffichecks.checknumber(1, speed)
	ffichecks.checknumber(2, target)
	
    repentogon.L_Game_Fadeout(speed, target)
end

function GameFuncs:Fart(pos, radius, source, scale, subtype, color)
	ffichecks.checkcdata(1, pos, "Vector")
	radius = ffichecks.optnumber(radius, 85)
	source = ffichecks.optcdata(source, "Entity", nil)
	scale = ffichecks.optnumber(scale, 1)
	subtype = ffichecks.optnumber(subtype, 0)
	color = ffichecks.optcdata(color, "ColorMod", Color.Default)

    repentogon.L_Game_Fart(pos, radius, source, scale, subtype, color)
end

function GameFuncs:FinishChallenge()
	repentogon.L_Game_FinishChallenge()
end

function GameFuncs:GetAmbush()
	return repentogon.L_Game_GetAmbush()
end

function GameFuncs:GetDarknessModifier()
	return repentogon.L_Game_GetDarknessModifier()
end

function GameFuncs:GetDevilRoomDeals()
	return repentogon.L_Game_GetDevilRoomDeals()
end

function GameFuncs:GetDonationModAngel()
	return repentogon.L_Game_GetDonationModAngel()
end

function GameFuncs:GetDonationModGreed()
	return repentogon.L_Game_GetDonationModGreed()
end

function GameFuncs:GetFont()
	return repentogon.L_Game_GetFont()
end

function GameFuncs:GetFrameCount()
	return repentogon.L_Game_GetFrameCount()
end

function GameFuncs:GetGreedBossWaveNum()
	return repentogon.L_Game_GetGreedBossWaveNum()
end

function GameFuncs:GetGreedWavesNum()
	return repentogon.L_Game_GetGreedWavesNum()
end

function GameFuncs:GetHUD()
	return repentogon.L_Game_GetHUD()
end

function GameFuncs:GetItemOverlay()
	return repentogon.L_Game_GetItemOverlay()
end

function GameFuncs:GetItemPool()
	return repentogon.L_Game_GetItemPool()
end

function GameFuncs:GetLastDevilRoomStage()
	return repentogon.L_Game_GetLastDevilRoomStage()
end

function GameFuncs:GetLastLevelWithDamage()
	return repentogon.L_Game_GetLastLevelWithDamage()
end

function GameFuncs:GetLastLevelWithoutHalfHp()
	return repentogon.L_Game_GetLastLevelWithoutHalfHp()
end

function GameFuncs:GetLevel()
	return repentogon.L_Game_GetLevel()
end

function GameFuncs:GetNearestPlayer()
	return repentogon.L_Game_GetNearestPlayer()
end

function GameFuncs:GetNumEncounteredBosses()
	return repentogon.L_Game_GetNumEncounteredBosses()
end

function GameFuncs:GetNumPlayers()
	return repentogon.L_Game_GetNumPlayers()
end

function GameFuncs:GetPlayer(idx)
	ffichecks.checknumber(1, idx)
	return repentogon.L_Game_GetPlayer(idx)
end

function GameFuncs:GetRandomPlayer(pos, radius)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, radius)
	
	return repentogon.L_Game_GetRandomPlayerPlayer(pos, radius)
end

function GameFuncs:GetRoom()
	return repentogon.L_Game_GetRoom()
end

function GameFuncs:GetScreenShakeCountdown()
	return repentogon.L_Game_GetScreenShakeCountdown()
end

function GameFuncs:GetSeeds()
	return repentogon.L_Game_GetSeeds()
end

function GameFuncs:GetStagesWithoutDamage()
	return repentogon.L_Game_GetStagesWithoutDamage()
end

function GameFuncs:GetStagesWithoutHeartsPicked()
	return repentogon.L_Game_GetStagesWithoutHeartsPicked()
end

function GameFuncs:GetStateFlag(flag)
	ffichecks.checkboolean(flag)
	
	return repentogon.L_Game_GetStateFlag(flag)
end

function GameFuncs:GetTargetDarkness()
	return repentogon.L_Game_GetTargetDarkness()
end

function GameFuncs:GetTreasureRoomVisitCount()
	return repentogon.L_Game_GetTreasureRoomVisitCount()
end

function GameFuncs:GetVictoryLap()
	return repentogon.L_Game_GetVictoryLap()
end

function GameFuncs:HasEncounteredBoss(id, variant)
	ffichecks.checknumber(1, id)
	local fVariant = ffichecks.optnumber(variant, 0)
	
	return repentogon.L_Game_HasEncounteredBoss(id, fVariant)
end

function GameFuncs:HasHallucination()
	return repentogon.L_Game_HasHallucination()
end

function GameFuncs:IsGreedMode()
	return repentogon.L_Game_IsGreedMode()
end

function GameFuncs:IsPaused()
	return repentogon.L_Game_IsPaused()
end

function GameFuncs:MakeShockwave(pos, amplitude, speed, duration)
	ffichecks.checknumber(1, pos)
	ffichecks.checknumber(2, amplitude)
	ffichecks.checknumber(3, speed)
	ffichecks.checknumber(4, duration)
	
	repentogon.L_Game_MakeShockwave(pos, amplitude, speed, duration)
end

function GameFuncs:MoveToRandomRoom(ierror, seed, player)
	ffichecks.checkboolean(1, ierror)
	ffichecks.checknumber(2, seed)
	ffichecks.checkcdata(3, player, "EntityPlayer")
	
	repentogon.L_Game_MoveToRandomRoom(ierror, seed, player)
end

function GameFuncs:NextVictoryLap()
	repentogon.L_Game_NextVictoryLap()
end

function GameFuncs:Render()
	repentogon.L_Game_Render()
end

function GameFuncs:RerollEnemy(enemy)
	ffichecks.checkcdata(1, enemy, "Entity")
	
	repentogon.L_Game_RerollEnemy(enemy)
end

function GameFuncs:RerollLevelCollectibles()
	repentogon.L_Game_RerollLevelCollectibles()
end

function GameFuncs:RerollLevelPickups(seed)
	ffichecks.checknumber(1, seed)
	repentogon.L_Game_RerollLevelPickups(seed)
end

function GameFuncs:SetLastDevilRoomStage(stage)
	ffichecks.checknumber(1, stage)
	repentogon.L_Game_SetLastDevilRoomStage(stage)
end

function GameFuncs:SetLastLevelWithDamage(stage)
	ffichecks.checknumber(1, stage)
	repentogon.L_Game_SetLastLevelWithDamage(stage)
end

function GameFuncs:SetLastLevelWithoutHalfHp(stage)
	ffichecks.checknumber(1, stage)
	repentogon.L_Game_SetLastLevelWithoutHalfHp(stage)
end

function GameFuncs:SetStateFlag(flag)
	ffichecks.checknumber(1, flag)
	repentogon.L_Game_SetStateFlag(flag)
end

function GameFuncs:ShakeScreen(timeout)
	ffichecks.checknumber(1, timeout)
	repentogon.L_Game_ShakeScreen(timeout)
end

function GameFuncs:ShowFortune()
	repentogon.L_Game_ShowFortune()
end

function GameFuncs:ShowHallucination(framecount, backdrop)
	ffichecks.checknumber(1, framecount)
	ffichecks.checknumber(2, backdrop)
	repentogon.L_Game_ShowHallucination(framecount, backdrop)
end

function GameFuncs:ShowRule()
	repentogon.L_Game_ShowRule()
end

function GameFuncs:Spawn(id, variant, pos, velocity, spawner, subtype, seed)
	ffichecks.checknumber(1, id)
	ffichecks.checknumber(2, variant)
	ffichecks.checkcdata(3, pos, "Vector")
	ffichecks.checkcdata(4, velocity, "Vector")
	ffichecks.checkcdata(5, spawner, "Entity", true)
	ffichecks.checknumber(6, subtype)
	ffichecks.checknumber(7, seed)
	
    return repentogon.L_Game_Spawn(id, variant, pos, velocity, spawner, subtype, seed)
end

function GameFuncs:SpawnParticles(pos, variant, count, speed, color, height, subtype)
	ffichecks.checkcdata(1, pos, "Vector")
	ffichecks.checknumber(2, variant)
	ffichecks.checknumber(3, count)
	ffichecks.checknumber(4, speed)
	color = ffichecks.optcdata(color, "ColorMod", Color.Default)
	height = ffichecks.optnumber(height, 100000)
	subtype = ffichecks.optnumber(subtype, 0)
	
    repentogon.L_Game_SpawnParticles(pos, variant, count, speed, color, height, subtype)
end

function GameFuncs:StartRoomTransition(idx, direction, animation, player, dimension)
	ffichecks.checknumber(1, idx)
	ffichecks.checknumber(2, direction)
	animation = ffichecks.optnumber(animation, 0)
	player = ffichecks.optcdata(player, "EntityPlayer", nil)
	dimension = ffichecks.optnumber(dimension, -1)
	
    repentogon.L_Game_StartRoomTransition(idx, direction, animation, player, dimension)
end

function GameFuncs:StartStageTransition(samestage, transitionoverride, player)
	samestage = ffichecks.optboolean(samestage, false)
	transitionoverride = ffichecks.optnumber(transitionoverride, 0)
	player = ffichecks.optcdata(player, "EntityPlayer", nil)
	
	repentogon.L_Game_StartStageTransition(animation, transitionoverride, player)
end

function GameFuncs:Update()
	repentogon.L_Game_Update()
end

function GameFuncs:UpdateStrangeAttractor(pos, force, radius)
	ffichecks.checkcdata(1, pos, "Vector")
	force = ffichecks.optnumber(force, 10)
	radius = ffichecks.optnumber(radius, 250)
	
	repentogon.L_Game_UpdateStrangeAttractor(pos, force, radius)
end

local getkeys = {
    BlueWombParTime = repentogon.L_Game_GetBlueWombParTime,
	BossRushParTime = repentogon.L_Game_GetBossRushParTime,
	Challenge = repentogon.L_Game_GetChallenge,
	Difficulty = repentogon.L_Game_GetDifficulty,
	ScreenShakeOffset = repentogon.L_Game_GetScreenShakeOffset,
	TimeCounter = repentogon.L_Game_GetTimeCounter
}

-- FFI will automatically reject incompatible types.
-- On the other hand, we want to do some Lua-side checks for some functions so we can cleanly error.
local setkeys = {
    BlueWombParTime = repentogon.L_Game_SetBlueWombParTime,
	BossRushParTime = repentogon.L_Game_SetBossRushParTime,
	Challenge = repentogon.L_Game_SetChallenge,
	Difficulty = repentogon.L_Game_SetDifficulty,
	ScreenShakeOffset = function(val)
        ffichecks.checkcdata(1, val, "Vector")
        repentogon.L_Game_SetScreenShakeOffset(val)
    end,
	TimeCounter = repentogon.L_Game_SetTimeCounter
}

local GameMT
GameMT = {
	__index = function(tbl, key)
        if getkeys[key] ~= nil then
            return getkeys[key]()
        end
		return GameFuncs[key];
    end,
    
    __newindex = function(tbl, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
}

Game = setmetatable({}, {
	__call = function()
        return setmetatable({}, GameMT)
    end
})
