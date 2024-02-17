ffi.cdef[[
	void L_EntityProjectile_AddChangeFlags(Entity_Projectile*, unsigned long long);
	void L_EntityProjectile_AddFallingAccel(Entity_Projectile*, float);
	void L_EntityProjectile_AddFallingSpeed(Entity_Projectile*, float);
	void L_EntityProjectile_AddHeight(Entity_Projectile*, float);
	void L_EntityProjectile_AddProjectileFlags(Entity_Projectile*, unsigned long long);
	void L_EntityProjectile_AddScale(Entity_Projectile*, float);
	void L_EntityProjectile_ClearProjectileFlags(Entity_Projectile*, unsigned long long);
	void L_EntityProjectile_HasProjectileFlags(Entity_Projectile*, unsigned long long);
	
	float L_EntityProjectile_GetAcceleration(Entity_Projectile*);
	void L_EntityProjectile_SetAcceleration(Entity_Projectile*, float);
	unsigned long long L_EntityProjectile_GetChangeFlags(Entity_Projectile*);
	void L_EntityProjectile_SetChangeFlags(Entity_Projectile*, unsigned long long);
	int L_EntityProjectile_GetChangeTimeout(Entity_Projectile*);
	void L_EntityProjectile_SetChangeTimeout(Entity_Projectile*, int);
	float L_EntityProjectile_GetChangeVelocity(Entity_Projectile*);
	void L_EntityProjectile_SetChangeVelocity(Entity_Projectile*, float);
	float L_EntityProjectile_GetCurvingStrength(Entity_Projectile*);
	void L_EntityProjectile_SetCurvingStrength(Entity_Projectile*, float);
	float L_EntityProjectile_GetDamage(Entity_Projectile*);
	void L_EntityProjectile_SetDamage(Entity_Projectile*, float);
	float L_EntityProjectile_GetDepthOffset(Entity_Projectile*);
	void L_EntityProjectile_SetDepthOffset(Entity_Projectile*, float);
	float L_EntityProjectile_GetFallingAccel(Entity_Projectile*);
	void L_EntityProjectile_SetFallingAccel(Entity_Projectile*, float);
	float L_EntityProjectile_GetFallingSpeed(Entity_Projectile*);
	void L_EntityProjectile_SetFallingSpeed(Entity_Projectile*, float);
	float L_EntityProjectile_GetHeight(Entity_Projectile*);
	void L_EntityProjectile_SetHeight(Entity_Projectile*, float);
	float L_EntityProjectile_GetHomingStrength(Entity_Projectile*);
	void L_EntityProjectile_SetHomingStrength(Entity_Projectile*, float);
	unsigned long long L_EntityProjectile_GetProjectileFlags(Entity_Projectile*);
	void L_EntityProjectile_SetProjectileFlags(Entity_Projectile*, unsigned long long);
	float L_EntityProjectile_GetScale(Entity_Projectile*);
	void L_EntityProjectile_SetScale(Entity_Projectile*, float);
	int L_EntityProjectile_GetWiggleFrameOffset(Entity_Projectile*);
	void L_EntityProjectile_SetWiggleFrameOffset(Entity_Projectile*, int);
	
]]

local repentogon = ffidll
local lffi = ffi

local EntityProjectileFuncs = {}

function EntityProjectileFuncs:AddChangeFlags(flags)
	ffichecks.checknumber(2, flags)
	L_EntityProjectile_AddChangeFlags(self, flags)
end

function EntityProjectileFuncs:AddFallingAccel(value)
	ffichecks.checknumber(2, value)
	L_EntityProjectile_AddFallingAccel(self, value)
end

function EntityProjectileFuncs:AddFallingSpeed(value)
	ffichecks.checknumber(2, value)
	L_EntityProjectile_AddFallingSpeed(self, value)
end

function EntityProjectileFuncs:AddHeight(value)
	ffichecks.checknumber(2, value)
	L_EntityProjectile_AddHeight(self, value)
end

function EntityProjectileFuncs:AddProjectileFlags(flags)
	ffichecks.checknumber(2, flags)
	L_EntityProjectile_AddProjectileFlagsFlags(self, flags)
end

function EntityProjectileFuncs:AddFallingAccel(value)
	ffichecks.checknumber(2, value)
	L_EntityProjectile_AddFallingAccel(self, value)
end

function EntityProjectileFuncs:AddScale(value)
	ffichecks.checknumber(2, value)
	L_EntityProjectile_AddScale(self, value)
end

function EntityProjectileFuncs:ClearProjectileFlags(flags)
	ffichecks.checknumber(2, flags)
	L_EntityProjectile_ClearProjectileFlagsChangeFlags(self, flags)
end

function EntityProjectileFuncs:HasProjectileFlags(flags)
	ffichecks.checknumber(2, flags)
	return L_EntityProjectile_HasProjectileFlags(self, flags)
end

local getkeys = {
    Acceleration = repentogon.L_EntityProjectile_GetAcceleration,
	ChangeFlags = repentogon.L_EntityProjectile_GetChangeFlags,
	ChangeTimeout = repentogon.L_EntityProjectile_GetChangeTimeout,
	ChangeVelocity = repentogon.L_EntityProjectile_GetChangeVelocity,
	CurvingStrength = repentogon.L_EntityProjectile_GetCurvingStrength,
	Damage = repentogon.L_EntityProjectile_GetDamage,
	DepthOffset = repentogon.L_EntityProjectile_GetDepthOffset,
	FallingAccel = repentogon.L_EntityProjectile_GetFallingAccel,
	FallingSpeed = repentogon.L_EntityProjectile_GetFallingSpeed,
	Height = repentogon.L_EntityProjectile_GetHeight,
	HomingStrength = repentogon.L_EntityProjectile_GetHomingStrength,
	ProjectileFlags = repentogon.L_EntityProjectile_GetProjectileFlags,
	Scale = repentogon.L_EntityProjectile_GetScale,
	WiggleFrameOffset = repentogon.L_EntityProjectile_GetWiggleFrameOffset,
}

local setkeys = {
    Acceleration = repentogon.L_EntityProjectile_SetAcceleration,
	ChangeFlags = repentogon.L_EntityProjectile_SetChangeFlags,
	ChangeTimeout = repentogon.L_EntityProjectile_SetChangeTimeout,
	ChangeVelocity = repentogon.L_EntityProjectile_SetChangeVelocity,
	CurvingStrength = repentogon.L_EntityProjectile_SetCurvingStrength,
	Damage = repentogon.L_EntityProjectile_SetDamage,
	DepthOffset = repentogon.L_EntityProjectile_SetDepthOffset,
	FallingAccel = repentogon.L_EntityProjectile_SetFallingAccel,
	FallingSpeed = repentogon.L_EntityProjectile_SetFallingSpeed,
	Height = repentogon.L_EntityProjectile_SetHeight,
	HomingStrength = repentogon.L_EntityProjectile_SetHomingStrength,
	ProjectileFlags = repentogon.L_EntityProjectile_SetProjectileFlags,
	Scale = repentogon.L_EntityProjectile_SetScale,
	WiggleFrameOffset = repentogon.L_EntityProjectile_SetWiggleFrameOffset,
}

local EntityProjectileMT
EntityProjectileMT = lffi.metatype("Entity_Projectile", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityProjectileFuncs[key] ~= nil then
			return EntityProjectileFuncs[key]
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
