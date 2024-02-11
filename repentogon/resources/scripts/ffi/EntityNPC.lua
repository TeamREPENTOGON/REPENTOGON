ffi.cdef[[
	void L_EntityNPC_AnimWalkFrame(Entity_NPC*, const char*, const char*);
	Vector* L_EntityNPC_CalculateTargetPosition(Entity_NPC*, float);
	bool L_EntityNPC_CanBeDamagedFromVelocity(Entity_NPC*, Vector*);
	bool L_EntityNPC_CanReroll(Entity_NPC*);
	Entity_Projectile* L_EntityNPC_FireBossProjectiles(Entity_NPC*, int, Vector*, float, ProjectileParams*);
	void L_EntityNPC_FireProjectiles(Entity_NPC*, Vector*, Vector*, unsigned int, ProjectileParams*);
	int L_EntityNPC_GetAliveEnemyCount(Entity_NPC*);
	int L_EntityNPC_GetBossColorIdx(Entity_NPC*);
	int L_EntityNPC_GetChampionColorIdx(Entity_NPC*);
	Entity* L_EntityNPC_GetPlayerTarget(Entity_NPC*);
	bool L_EntityNPC_IsBoss(Entity_NPC*);
	bool L_EntityNPC_IsChampion(Entity_NPC*);
	void L_EntityNPC_KillUnique(Entity_NPC*);
	void L_EntityNPC_MakeChampion(Entity_NPC*, unsigned int, int, bool);
	Entity_Effect* L_EntityNPC_MakeSplat(Entity_NPC*, float);
	bool L_EntityNPC_Morph(Entity_NPC*, int, int, int, int);
	void L_EntityNPC_PlaySound(Entity_NPC*, int, float, int, bool, float);
	EntityList_EL* L_EntityNPC_QueryNPCsGroup(Entity_NPC*, int);
	EntityList_EL* L_EntityNPC_QueryNPCsSpawnerType(Entity_NPC*, int, int, bool);
	EntityList_EL* L_EntityNPC_QueryNPCsType(Entity_NPC*, int, int);
	void L_EntityNPC_ResetPathFinderTarget(Entity_NPC*);
	Entity_NPC* L_EntityNPC_ThrowSpider(Vector*, Entity*, Vector*, bool);
	bool L_EntityNPC_GetCanShutDoors(Entity_NPC*);
	void L_EntityNPC_SetCanShutDoors(Entity_NPC*, bool);
	Entity_NPC* L_EntityNPC_GetChildNPC(Entity_NPC*);
	void L_EntityNPC_SetChildNPC(Entity_NPC*, Entity_NPC*);
	Entity_NPC* L_EntityNPC_GetParentNPC(Entity_NPC*);
	void L_EntityNPC_SetParentNPC(Entity_NPC*, Entity_NPC*);
	Entity* L_EntityNPC_GetEntityRef(Entity_NPC*);
	void L_EntityNPC_SetEntityRef(Entity_NPC*, Entity*);
	int L_EntityNPC_GetGroupIdx(Entity_NPC*);
	void L_EntityNPC_SetGroupIdx(Entity_NPC*, int);
	int L_EntityNPC_GetI1(Entity_NPC*);
	void L_EntityNPC_SetI1(Entity_NPC*, int);
	int L_EntityNPC_GetI2(Entity_NPC*);
	void L_EntityNPC_SetI2(Entity_NPC*, int);
	Vector* L_EntityNPC_GetV1(Entity_NPC*);
	void L_EntityNPC_SetV1(Entity_NPC*, Vector*);
	Vector* L_EntityNPC_GetV2(Entity_NPC*);
	void L_EntityNPC_SetV2(Entity_NPC*, Vector*);
	NPCAI_Pathfinder* L_EntityNPC_GetPathfinder(Entity_NPC*);
	int L_EntityNPC_GetProjectileCooldown(Entity_NPC*);
	void L_EntityNPC_SetProjectileCooldown(Entity_NPC*, int);
	float L_EntityNPC_GetScale(Entity_NPC*);
	void L_EntityNPC_SetScale(Entity_NPC*, float);
	int L_EntityNPC_GetState(Entity_NPC*);
	void L_EntityNPC_SetState(Entity_NPC*, int);
	int L_EntityNPC_GetStateFrame(Entity_NPC*);
	void L_EntityNPC_SetStateFrame(Entity_NPC*, int);
]]

local repentogon = ffidll
local lffi = ffi

local EntityNPCFuncs = {}

function EntityNPCFuncs:AnimWalkFrame(horizontal, vertical, threshold)
	ffichecks.checkstring(2, horizontal)
	ffichecks.checkstring(3, vertical)
	ffichecks.checknumber(4, threshold)
	repentogon.L_EntityNPC_AnimWalkFrame(self, horizontal, vertical, threshold)
end

function EntityNPCFuncs:CalculateTargetPosition(limit)
	ffichecks.checknumber(2, limit)
	local ret = ffichecks.fixreturn(repentogon.L_EntityNPC_CalculateTargetPosition(self, limit))
	-- to convert the temporary Vector made c-side into one lua-side
	if ret ~= nil then
		ret = Vector(ret.X, ret.Y)
	end
	return ret
end

function EntityNPCFuncs:CanBeDamagedFromVelocity(velocity)
	ffichecks.checkcdata(velocity, "Vector")
	return repentogon.L_EntityNPC_CanBeDamagedFromVelocity(self, velocity)
end

function EntityNPCFuncs:FireBossProjectiles(num, target, trajectoryMod, params)
	ffichecks.checknumber(2, num)
	ffichecks.checkcdata(target, "Vector")
	ffichecks.checknumber(4, trajectoryMod)
	ffichecks.checkcdata(params, "ProjectileParams")
	return ffichecks.fixreturn(repentogon.L_EntityNPC_FireBossProjectiles(self, num, target, trajectoryMod, params)
end

function EntityNPCFuncs:FireProjectiles(pos, velocity, mode, params)
	ffichecks.checkcdata(pos, "Vector")
	ffichecks.checkcdata(velocity, "Vector")
	ffichecks.checknumber(4, mode)
	ffichecks.checkcdata(params, "ProjectileParams")
	repentogon.L_EntityNPC_FireProjectiles(self, pos, velocity, mode, params)
end

function EntityNPCFuncs:GetAliveEnemiesCount()
	return repentogon.L_EntityNPC_GetAliveEnemyCount(self)
end

function EntityNPCFuncs:GetBossColorIdx()
	return repentogon.L_EntityNPC_GetBossColorIdx(self)
end

function EntityNPCFuncs:GetChampionColorIdx()
	return repentogon.L_EntityNPC_GetChampionColorIdx(self)
end

function EntityNPCFuncs:GetPlayerTarget()
	return ffichecks.fixreturn(repentogon.L_EntityNPC_GetPlayerTarget(self))
end

function EntityNPCFuncs:IsBoss()
	return repentogon.L_EntityNPC_IsBoss(self)
end

function EntityNPCFuncs:IsChampion()
	return repentogon.L_EntityNPC_IsChampion(self)
end

function EntityNPCFuncs:KillUnique()
	return repentogon.L_EntityNPC_KillUnique(self)
end

function EntityNPCFuncs:MakeChampion(seed, colorIdx, init)
	ffichecks.checknumber(2, seed)
	ffichecks.checknumber(3, colorIdx)
	ffichecks.checkboolean(4, init)
	repentogon.L_EntityNPC_MakeChampion(self, seed, colorIdx, init)
end

function EntityNPCFuncs:MakeSplat()
	return ffichecks.fixreturn(repentogon.L_EntityNPC_MakeSplat(self))
end

function EntityNPCFuncs:Morph(entType, variant, subtype, colorIdx)
	ffichecks.checknumber(2, entType)
	ffichecks.checknumber(3, variant)
	ffichecks.checknumber(4, subtype)
	ffichecks.checknumber(5, colorIdx)
	return repentogon.L_EntityNPC_Morph(self, entType, variant, subtype, colorIdx)
end

function EntityNPCFuncs:PlaySound(id, volume, delay, loop, pitch)
	ffichecks.checknumber(2, id)
	ffichecks.checknumber(3, volume)
	ffichecks.checknumber(4, delay)
	ffichecks.checkboolean(5, loop)
	ffichecks.checknumber(6, pitch)
	repentogon.L_EntityNPC_PlaySound(self, id, volume, delay, loop, pitch)
end

function EntityNPCFuncs:QueryNPCsGroup(idx)
	ffichecks.checknumber(2, idx)
	return ffichecks.fixreturn(repentogon.L_EntityNPC_QueryNPCsGroup(self, idx))
end

function EntityNPCFuncs:QueryNPCsSpawnerType(spawnerType, entType, onlyEnemies)
	ffichecks.checknumber(2, spawnerType)
	ffichecks.checknumber(3, entType)
	ffichecks.checkboolean(4, onlyEnemies)
	return ffichecks.fixreturn(repentogon.L_EntityNPC_QueryNPCsSpawnerType(self, spawnerType, entType, onlyEnemies))
end

function EntityNPCFuncs:QueryNPCsType(entType, variant)
	ffichecks.checknumber(2, entType)
	ffichecks.checknumber(3, variant)
	return ffichecks.fixreturn(repentogon.L_EntityNPC_QueryNPCsType(self, entType, variant))
end

function EntityNPCFuncs:ResetPathFinderTarget()
	repentogon.L_EntityNPC_ResetPathFinderTarget(self)
end

function EntityNPCFuncs:ThrowSpider(pos, spawner, target, big)
	ffichecks.checkcdata(pos, "Velocity")
	ffichecks.checkcdata(spawner, "Entity")
	ffichecks.checkcdata(target, "Velocity")
	ffichecks.checkboolean(4, big)
	return ffichecks.fixreturn(repentogon.L_EntityNPC_ThrowSpider(self, pos, spawner, target, big))
end

local getkeys = {
    CanShutDoors = repentogon.L_EntityNPC_GetCanShutDoors,
	ChildNPC = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetChildNPC(self)) end,
	ParentNPC = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetParentNPC(self)) end,
	EntityRef = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetEntityRef(self)) end,
	GroupIdx = repentogon.L_EntityNPC_GetGroupIdx,
	I1 = repentogon.L_EntityNPC_GetI1,
	I2 = repentogon.L_EntityNPC_GetI2,
	V1 = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetV1(self)) end,
	V2 = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetV2(self)) end,
	Pathfinder = function(self) return ffichecks.fixreturn(repentogon.L_EntityNPC_GetPathfinder(self)) end,
	ProjectileCooldown = repentogon.L_EntityNPC_GetProjectileCooldown,
	Scale = repentogon.L_EntityNPC_GetScale,
	State = repentogon.L_EntityNPC_GetState,
	StateFrame = repentogon.L_EntityNPC_GetStateFrame,
}

local setkeys = {
    CanShutDoors = repentogon.L_EntityNPC_SetCanShutDoors,
	ChildNPC = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Entity_NPC", repentogon.L_EntityNPC_SetChildNPC) end,
	ParentNPC = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Entity_NPC", repentogon.L_EntityNPC_SetParentNPC) end,
	EntityRef = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Entity", repentogon.L_EntityNPC_SetEntityRef) end,
	GroupIdx = repentogon.L_EntityNPC_SetGroupIdx,
	I1 = repentogon.L_EntityNPC_SetI1,
	I2 = repentogon.L_EntityNPC_SetI2,
	V1 = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Vector", repentogon.L_EntityNPC_SetV1) end,
	V2 = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Vector", repentogon.L_EntityNPC_SetV2) end,
	ProjectileCooldown = repentogon.L_EntityNPC_SetProjectileCooldown,
	Scale = repentogon.L_EntityNPC_SetScale,
	State = repentogon.L_EntityNPC_SetState,
	StateFrame = repentogon.L_EntityNPC_SetStateFrame,
}

local EntityNPCMT
EntityNPCMT = lffi.metatype("Entity_NPC", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityNPCFuncs[key] ~= nil then
			return EntityNPCFuncs[key]
		elseif EntityFuncs[key] ~= nil then
			return EntityFuncs[key]
		else
			error(string.format("attempted to access nil value '%s'", key))
		end
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
})
