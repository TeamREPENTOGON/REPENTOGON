ffi.cdef [[
    void* L_PlayerManager_FirstCollectibleOwner(int, bool);
    bool L_PlayerManager_AnyoneHasCollectible(int, bool);
    void* L_PlayerManager_SpawnCoPlayer2(int);
    bool L_PlayerManager_IsCoopPlay();
    int L_PlayerManager_GetNumCollectibles(int, bool);
    int L_PlayerManager_GetTotalTrinketMultiplier(int);
    void* L_PlayerManager_FirstTrinketOwner(int, bool);
    void L_PlayerManager_TriggerRoomClear();
    bool L_PlayerManager_AnyoneHasTrinket(int, bool);
    int L_PlayerManager_GetPlayerCount();
    void* L_PlayerManager_GetPlayerAt(int);
    void* L_PlayerManager_GetEsauJrState(int);
    void* L_PlayerManager_FirstPlayerByType(unsigned int);
    void* L_PlayerManager_FirstBirthrightOwner();
    bool L_PlayerManager_AnyPlayerTypeHasBirthright(unsigned int);
    bool L_PlayerManager_AnyPlayerTypeHasTrinket(unsigned int, int, bool);
    bool L_PlayerManager_AnyPlayerTypeHasCollectible(unsigned int, int, bool);
    void L_PlayerManager_SpawnSelectedBaby(int, int);
    void* L_PlayerManager_GetRandomCollectibleOwner(int, unsigned int, struct RNG**);
    void* L_PlayerManager_GetRandomTrinketOwner(int, unsigned int, struct RNG**);
]]

local ffi = ffi
local repentogon = ffidll

local cfuncs = {
    PushPlayer = __Lua_PlayerManager_PushPlayer,
    RemoveCoPlayer = __Lua_PlayerManager_RemoveCoPlayer,
}

local uintptr = ffi.typeof("uintptr_t")
local rngOut = ffi.new("struct RNG*[1]")

local function ToPlayer(ptr)
    if ptr == nil then
        return nil
    end
    return cfuncs.PushPlayer(tonumber(ffi.cast(uintptr, ptr)))
end

PlayerManager = {
    AnyoneHasCollectible = function(collectible, ignoreModifiers)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_PlayerManager_AnyoneHasCollectible(collectible, ffichecks.optboolean(ignoreModifiers, false))
    end,
    AnyoneHasTrinket = function(trinket, ignoreModifiers)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_PlayerManager_AnyoneHasTrinket(trinket, ffichecks.optboolean(ignoreModifiers, false))
    end,
    AnyoneIsPlayerType = function(playerType)
        ffichecks.checkinteger(1, playerType)
        return repentogon.L_PlayerManager_FirstPlayerByType(playerType) ~= nil
    end,
    AnyPlayerTypeHasBirthright = function(playerType)
        ffichecks.checkinteger(1, playerType)
        return repentogon.L_PlayerManager_AnyPlayerTypeHasBirthright(playerType)
    end,
    AnyPlayerTypeHasCollectible = function(playerType, collectible, ignoreModifiers)
        ffichecks.checkinteger(1, playerType)
        ffichecks.checkinteger(2, collectible)
        return repentogon.L_PlayerManager_AnyPlayerTypeHasCollectible(playerType, collectible, ffichecks.optboolean(ignoreModifiers, false))
    end,
    AnyPlayerTypeHasTrinket = function(playerType, trinket, ignoreModifiers)
        ffichecks.checkinteger(1, playerType)
        ffichecks.checkinteger(2, trinket)
        return repentogon.L_PlayerManager_AnyPlayerTypeHasTrinket(playerType, trinket, ffichecks.optboolean(ignoreModifiers, false))
    end,
    FirstBirthrightOwner = function(playerType)
        ffichecks.checkinteger(1, playerType)
        return ToPlayer(repentogon.L_PlayerManager_FirstBirthrightOwner())
    end,
    FirstCollectibleOwner = function(collectible, lazSharedGlobalTag)
        ffichecks.checkinteger(1, collectible)
        return ToPlayer(repentogon.L_PlayerManager_FirstCollectibleOwner(collectible, ffichecks.optboolean(lazSharedGlobalTag, true)))
    end,
    FirstPlayerByType = function(playerType)
        ffichecks.checkinteger(1, playerType)
        return ToPlayer(repentogon.L_PlayerManager_FirstPlayerByType(playerType))
    end,
    FirstTrinketOwner = function(trinket, arg2, arg3)
        ffichecks.checkinteger(1, trinket)
        local legacySignature = arg2 == nil or ffichecks.iscdata(arg2, "RNG")
        local lazSharedGlobalTag = arg2
        if legacySignature then
            lazSharedGlobalTag = arg3
        end
        lazSharedGlobalTag = ffichecks.optboolean(lazSharedGlobalTag, true)
        return ToPlayer(repentogon.L_PlayerManager_FirstTrinketOwner(trinket, lazSharedGlobalTag))
    end,
    GetEsauJrState = function(index)
        index = ffichecks.optnumber(index, 0)
        ffichecks.checkinteger(1, index)
        if index < 0 or index > 3 then
            error(string.format("Invalid index %d", index), 2)
        end
        return ToPlayer(repentogon.L_PlayerManager_GetEsauJrState(index))
    end,
    GetNumCollectibles = function(collectible, ignoreModifiers)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_PlayerManager_GetNumCollectibles(collectible, ffichecks.optboolean(ignoreModifiers, false))
    end,
    GetPlayers = function()
        local players = {}
        for i = 0, repentogon.L_PlayerManager_GetPlayerCount() - 1 do
            players[i + 1] = ToPlayer(repentogon.L_PlayerManager_GetPlayerAt(i))
        end
        return players
    end,
    GetRandomCollectibleOwner = function(collectible, seed)
        ffichecks.checkinteger(1, collectible)
        ffichecks.checkinteger(2, seed)
        local player = repentogon.L_PlayerManager_GetRandomCollectibleOwner(collectible, seed, rngOut)
        if player == nil then
            return nil, nil
        end
        return ToPlayer(player), rngOut[0]
    end,
    GetRandomTrinketOwner = function(trinket, seed)
        ffichecks.checkinteger(1, trinket)
        ffichecks.checkinteger(2, seed)
        local player = repentogon.L_PlayerManager_GetRandomTrinketOwner(trinket, seed, rngOut)
        if player == nil then
            return nil, nil
        end
        return ToPlayer(player), rngOut[0]
    end,
    GetTotalTrinketMultiplier = function(trinket)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_PlayerManager_GetTotalTrinketMultiplier(trinket)
    end,
    IsCoopPlay = function()
        return repentogon.L_PlayerManager_IsCoopPlay()
    end,
    RemoveCoPlayer = function(player)
        cfuncs.RemoveCoPlayer(player)
    end,
    SpawnCoPlayer2 = function(playerType)
        ffichecks.checkinteger(1, playerType)
        return ToPlayer(repentogon.L_PlayerManager_SpawnCoPlayer2(playerType))
    end,
    SpawnSelectedBaby = function(babyType, controllerIndex)
        ffichecks.checkinteger(1, babyType)
        ffichecks.checkinteger(2, controllerIndex)
        repentogon.L_PlayerManager_SpawnSelectedBaby(babyType, controllerIndex)
    end,
    TriggerRoomClear = function()
        repentogon.L_PlayerManager_TriggerRoomClear()
    end,
}

__Lua_PlayerManager_PushPlayer = nil
__Lua_PlayerManager_RemoveCoPlayer = nil
