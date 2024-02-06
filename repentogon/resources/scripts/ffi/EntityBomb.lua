ffi.cdef[[
	typedef struct {void *_;} BitSet128;
	
	void L_EntityBomb_AddTearFlags(Entity_Bomb*, BitSet128*);
	bool L_EntityBomb_HasTearFlags(Entity_Bomb*, BitSet128*);
	void L_EntityBomb_SetExplosionCountdown(Entity_Bomb*, int);
	float L_EntityBomb_GetExplosionDamage(Entity_Bomb*);
	void L_EntityBomb_SetExplosionDamage(Entity_Bomb*, float);
	bool L_EntityBomb_GetIsFetus(Entity_Bomb*);
	void L_EntityBomb_SetIsFetus(Entity_Bomb*, bool);
	BitSet128* L_EntityBomb_GetFlags(Entity_Bomb*);
	void L_EntityBomb_SetFlags(Entity_Bomb*, BitSet128*);
	float L_EntityBomb_GetRadiusMultiplier(Entity_Bomb*);
	void L_EntityBomb_SetRadiusMultiplier(Entity_Bomb*, float);
]]

local repentogon = ffidll
local lffi = ffi

local EntityBombFuncs = {}

function EntityBombFuncs:GetTearFlags()
	return L_EntityBomb_GetTearFlags(self)
end

function EntityBombFuncs:AddTearFlags(tearflags)
	ffichecks.checkcdata(2, tearflags, "BitSet128")
	repentogon.L_EntityBomb_AddTearFlags(self, tearflags)
end

function EntityBombFuncs:ClearTearFlags(tearflags)
	ffichecks.checkcdata(2, tearflags, "BitSet128")
	repentogon.L_EntityBomb_ClearTearFlags(self, tearflags)
end

function EntityBombFuncs:HasTearFlags(tearflags)
	ffichecks.checkcdata(2, tearflags, "BitSet128")
	return repentogon.L_EntityBomb_HasTearFlags(self, tearflags)
end

function EntityBombFuncs:SetExplosionCountdown(value)
	ffichecks.checknumber(2, value)
	repentogon.L_EntityBomb_SetExplosionCountdown(self, value)
end

local getkeys = {
    ExplosionDamage = repentogon.L_EntityBomb_GetExplosionDamage,
	Flags = repentogon.L_EntityBomb_GetFlags,
	IsFetus = repentogon.L_EntityBomb_GetIsFetus,
	RadiusMultiplier = repentogon.L_EntityBomb_GetRadiusMultiplier,
}

local setkeys = {
	ExplosionDamage = repentogon.L_EntityBomb_SetExplosionDamage,
    Flags = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "BitSet128", repentogon.L_EntityBomb_SetFlags) end,
	IsFetus = repentogon.L_EntityBomb_SetIsFetus,
	RadiusMultiplier = repentogon.L_EntityBomb_SetRadiusMultiplier,
}

local EntityBombMT
EntityBombMT = lffi.metatype("Entity_Bomb", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityBombFuncs[key] ~= nil then
			return EntityBombFuncs[key]
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
