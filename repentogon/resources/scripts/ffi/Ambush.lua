ffi.cdef[[
    int L_Ambush_GetCurrentWave();
    int L_Ambush_GetMaxBossChallengeWaves();
    int L_Ambush_GetMaxBossrushWaves();
    int L_Ambush_GetMaxChallengeWaves();
    struct RoomConfigRoom* L_Ambush_GetNextWave();
    int L_Ambush_GetRemainingWaves();
    int L_Ambush_GetNextWaves(struct RoomConfigRoom**, int);
    void L_Ambush_SetMaxBossChallengeWaves(int);
    void L_Ambush_SetMaxBossrushWaves(int);
    void L_Ambush_SetMaxChallengeWaves(int);
    void L_Ambush_SpawnBossrushWave();
    void L_Ambush_SpawnWave();
    void L_Ambush_StartChallenge();
]]
local ffi = ffi
local repentogon = ffidll
local room = nil

Ambush = {
    GetCurrentWave = function()
        return repentogon.L_Ambush_GetCurrentWave()
    end,
    GetMaxBossChallengeWaves = function()
        return repentogon.L_Ambush_GetMaxBossChallengeWaves()
    end,
    GetMaxBossrushWaves = function()
        return repentogon.L_Ambush_GetMaxBossrushWaves()
    end,
    GetMaxChallengeWaves = function()
        return repentogon.L_Ambush_GetMaxChallengeWaves()
    end,
    GetNextWave = function()
        if not room then
            room = Game():GetRoom()
        end
        if ffi.getprivate(room, "RoomDescriptor").Data.Type ~= 11 then
            error("Cannot get Ambush wave information outside of a (boss) challenge room", 2)
        end
        return repentogon.L_Ambush_GetNextWave();
    end,
    GetNextWaves = function()
        if not room then
            room = Game():GetRoom()
        end
        if ffi.getprivate(room, "RoomDescriptor").Data.Type ~= 11 then
            error("Cannot get Ambush wave information outside of a (boss) challenge room", 2)
        end
        local max = repentogon.L_Ambush_GetRemainingWaves()
        local result = {}
        if max == 0 then return result end
        local rooms = ffi.new("struct RoomConfigRoom*[?]", max)
        local count = repentogon.L_Ambush_GetNextWaves(rooms, max)
        for i = 0, count - 1 do
            result[i + 1] = rooms[i]
        end
        return result
    end,
    SetMaxBossChallengeWaves = function(waves)
        waves = ffichecks.checkinteger(1, waves)
        repentogon.L_Ambush_SetMaxBossChallengeWaves(waves)
    end,
    SetMaxBossrushWaves = function(waves)
        waves = ffichecks.checkinteger(1, waves)
        waves = math.min(25, waves);
        repentogon.L_Ambush_SetMaxBossrushWaves(waves)
    end,
    SetMaxChallengeWaves = function(waves)
        waves = ffichecks.checkinteger(1, waves)
        repentogon.L_Ambush_SetMaxChallengeWaves(waves)
    end,
    SpawnBossrushWave = function()
        repentogon.L_Ambush_SpawnBossrushWave()
    end,
    SpawnWave = function()
        repentogon.L_Ambush_SpawnWave()
    end,  
    StartChallenge = function()
        repentogon.L_Ambush_StartChallenge()
    end
}