--todo
--[[
* GetMaxChallnegeWaves
* SetMaxChallengeWaves
* IsActive
* GetNextWave
* GetNextWaves
]]

ffi.cdef[[
typedef struct {void *_;} Ambush;

void L_Ambush_StartChallenge();
void L_Ambush_SpawnBossrushWave();
void L_Ambush_SpawnWave();
int L_Ambush_GetMaxBossrushWaves();
void L_Ambush_SetMaxBossrushWaves(int);
int L_Ambush_GetMaxChallengeWaves();
void L_Ambush_SetMaxChallengeWaves(int);
int L_Ambush_GetCurrentWave();
bool L_Ambush_IsActive();

void L_Ambush_GetNextWaves();
]]

local repentogon = ffidll

Ambush = {}

Ambush.StartChallenge = function() repentogon.L_Ambush_StartChallenge() end

Ambush.SpawnBossushWave = function() repentogon.L_Ambush_SpawnBossrushWave() end

Ambush.SpawnWave = function() repentogon.L_Ambush_SpawnWave() end

Ambush.GetMaxBossrushWaves = function() return repentogon.L_Ambush_GetMaxBossrushWaves() end

Ambush.SetMaxBossrushWaves = function(amount)
	ffichecks.checknumber(1, amount)
	if amount > 25 then
		amount = 25 
	end
	repentogon.L_Ambush_SetMaxBossrushWaves(amount)
end

--todo
Ambush.GetMaxChallengeWaves = function() return repentogon.L_Ambush_GetMaxChallengeWaves() end

--todo
Ambush.SetMaxChallengeWaves = function(amount)
	ffichecks.checknumber(1, amount)
	repentogon.L_Ambush_SetMaxChallengeWaves(amount)
end

Ambush.GetCurrentWave = function() return repentogon.L_Ambush_GetCurrentWave() end

--todo
Ambush.IsActive = function() return repentogon.L_Ambush_IsActive() end

--todo
--Ambush.GetNextWave = function() return repentogon.L_Ambush_GetNextWave() end

--todo
Ambush.GetNextWaves = function() return repentogon.L_Ambush_GetNextWaves() end