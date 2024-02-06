ffi.cdef[[
	void L_EntityEffect_FollowParent(Entity_Effect*, Entity*);
	bool L_EntityEffect_StaticIsPlayerCreep(int);
	void L_EntityEffect_SetRadii(Entity_Effect*, float, float);
	void L_EntityEffect_SetTimeoutFunc(Entity_Effect*, int);
	unsigned int L_EntityEffect_GetDamageSource(Entity_Effect*);
	void L_EntityEffect_SetDamageSource(Entity_Effect*, unsigned int);
	float L_EntityEffect_GetFallingAcceleration(Entity_Effect*);
	void L_EntityEffect_SetFallingAcceleration(Entity_Effect*, float);
	float L_EntityEffect_GetFallingSpeed(Entity_Effect*);
	void L_EntityEffect_SetFallingSpeed(Entity_Effect*, float);
	bool L_EntityEffect_GetIsFollowing(Entity_Effect*);
	void L_EntityEffect_SetIsFollowing(Entity_Effect*, bool);
	int L_EntityEffect_GetLifespan(Entity_Effect*);
	void L_EntityEffect_SetLifespan(Entity_Effect*, int);
	float L_EntityEffect_GetHeight(Entity_Effect*);
	void L_EntityEffect_SetHeight(Entity_Effect*, float);
	float L_EntityEffect_GetMinRadius(Entity_Effect*);
	void L_EntityEffect_SetMinRadius(Entity_Effect*, float);
	float L_EntityEffect_GetMaxRadius(Entity_Effect*);
	void L_EntityEffect_SetMaxRadius(Entity_Effect*, float);
	Vector* L_EntityEffect_GetParentOffset(Entity_Effect*);
	void L_EntityEffect_SetParentOffset(Entity_Effect*, Vector*);
	float L_EntityEffect_GetRotation(Entity_Effect*);
	void L_EntityEffect_SetRotation(Entity_Effect*, float);
	float L_EntityEffect_GetScale(Entity_Effect*);
	void L_EntityEffect_SetScale(Entity_Effect*, float);
	int L_EntityEffect_GetState(Entity_Effect*);
	void L_EntityEffect_SetState(Entity_Effect*, int);
	int L_EntityEffect_GetTimeout(Entity_Effect*);
	void L_EntityEffect_SetTimeout(Entity_Effect*, int);
]]

local repentogon = ffidll
local lffi = ffi

local EntityEffectFuncs = {}

function EntityEffectFuncs:FollowParent(parent)
	ffichecks.checkcdata(2, parent, "Entity")
	L_EntityEffect_FollowParent(self, parent)
end

function EntityEffectFuncs:SetRadii(minR, maxR)
	ffichecks.checknumber(2, minR)
	ffichecks.checknumber(3, maxR)
	L_EntityEffect_SetRadii(self, minR, maxR)
end

function EntityEffectFuncs:SetTimeout(timeout)
	ffichecks.checknumber(2, timeout)
	L_EntityEffect_SetTimeoutFunc(self, timeout)
end

local getkeys = {
    DamageSource = repentogon.L_EntityEffect_GetDamageSource,
	FallingAcceleration = repentogon.L_EntityEffect_GetFallingAcceleration,
	FallingSpeed = repentogon.L_EntityEffect_GetFallingSpeed,
	IsFollowing = repentogon.L_EntityEffect_GetIsFollowing,
	Lifespan = repentogon.L_EntityEffect_GetLifespan,
	m_Height = repentogon.L_EntityEffect_GetHeight,
	MinRadius = repentogon.L_EntityEffect_GetMinRadius,
	MaxRadius = repentogon.L_EntityEffect_GetMaxRadius,
	ParentOffset = repentogon.L_EntityEffect_GetParentOffset,
	Rotation = repentogon.L_EntityEffect_GetRotation,
	Scale = repentogon.L_EntityEffect_GetScale,
	State = repentogon.L_EntityEffect_GetState,
	Timeout = repentogon.L_EntityEffect_GetTimeout,
}

local setkeys = {
    DamageSource = repentogon.L_EntityEffect_SetDamageSource,
	FallingAcceleration = repentogon.L_EntityEffect_SetFallingAcceleration,
	FallingSpeed = repentogon.L_EntityEffect_SetFallingSpeed,
	IsFollowing = repentogon.L_EntityEffect_SetIsFollowing,
	Lifespan = repentogon.L_EntityEffect_SetLifespan,
	m_Height = repentogon.L_EntityEffect_SetHeight,
	MinRadius = repentogon.L_EntityEffect_SetMinRadius,
	MaxRadius = repentogon.L_EntityEffect_SetMaxRadius,
	ParentOffset = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Vector", repentogon.L_EntityEffect_SetParentOffset) end,
	Rotation = repentogon.L_EntityEffect_SetRotation,
	Scale = repentogon.L_EntityEffect_SetScale,
	State = repentogon.L_EntityEffect_SetState,
	Timeout = repentogon.L_EntityEffect_SetTimeout,
}

local EntityEffectMT
EntityEffectMT = lffi.metatype("Entity_Effect", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityEffectFuncs[key] ~= nil then
			return EntityEffectFuncs[key]
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

EntityFamiliar = {
	IsPlayerCreep = function(variant)
		ffichecks.checknumber(1, variant)
		return L_EntityEffect_IsPlayerCreep(variant)
	end
}