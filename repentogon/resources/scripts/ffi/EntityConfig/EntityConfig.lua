ffi.cdef [[
    struct EntityConfigEntity* L_EntityConfig_GetEntity(int, int, int);
    struct EntityConfigPlayer* L_EntityConfig_GetPlayer(int);
    int L_EntityConfig_GetPlayerCount();
    struct EntityConfigBaby* L_EntityConfig_GetBaby(int);
    int L_EntityConfig_GetBabyCount();
]]

local repentogon = ffidll

EntityConfig = {
    GetBaby = function(id)
        id = ffichecks.checkinteger(1, id)
        local result = repentogon.L_EntityConfig_GetBaby(id) return result
    end,
    GetEntity = function(type, variant, subType)
        type = ffichecks.checkinteger(1, type)
        local result = repentogon.L_EntityConfig_GetEntity(type, ffichecks.optnumber(variant, -1), ffichecks.optnumber(subType, -1)) return result
    end,
    GetMaxBabyID = function()
        return repentogon.L_EntityConfig_GetBabyCount() - 1
    end,
    GetMaxPlayerType = function()
        return repentogon.L_EntityConfig_GetPlayerCount() - 1
    end,
    GetPlayer = function(playerType)
        playerType = ffichecks.checkinteger(1, playerType)
        local result = repentogon.L_EntityConfig_GetPlayer(playerType) return result
    end,
}
