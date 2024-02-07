ffi.cdef[[
	void L_EntityKnife_AddTearFlags(Entity_Knife*, BitSet128*);
	void L_EntityKnife_ClearTearFlags(Entity_Knife*, BitSet128*);
	bool L_EntityKnife_HasTearFlags(Entity_Knife*, BitSet128*);
	float L_EntityKnife_GetKnifeDistance(Entity_Knife*);
	float L_EntityKnife_GetKnifeVelocity(Entity_Knife*);
	int L_EntityKnife_GetRenderZ(Entity_Knife*);
	bool L_EntityKnife_IsFlying(Entity_Knife*);
	void L_EntityKnife_Reset(Entity_Knife*);
	void L_EntityKnife_Shoot(Entity_Knife*, float, float);
	float L_EntityKnife_GetCharge(Entity_Knife*);
	void L_EntityKnife_SetCharge(Entity_Knife*, float);
	float L_EntityKnife_GetMaxDistance(Entity_Knife*);
	void L_EntityKnife_SetMaxDistance(Entity_Knife*, float);
	float L_EntityKnife_GetPathFollowSpeed(Entity_Knife*);
	void L_EntityKnife_SetPathFollowSpeed(Entity_Knife*, float);
	float L_EntityKnife_GetPathOffset(Entity_Knife*);
	void L_EntityKnife_SetPathOffset(Entity_Knife*, float);
	float L_EntityKnife_GetRotation(Entity_Knife*);
	void L_EntityKnife_SetRotation(Entity_Knife*, float);
	float L_EntityKnife_GetRotationOffset(Entity_Knife*);
	void L_EntityKnife_SetRotationOffset(Entity_Knife*, float);
	float L_EntityKnife_GetScale(Entity_Knife*);
	void L_EntityKnife_SetScale(Entity_Knife*, float);
	BitSet128* L_EntityKnife_GetTearFlags(Entity_Knife*);
	void L_EntityKnife_SetFlags(Entity_Knife*, BitSet128*)
]]

local repentogon = ffidll
local lffi = ffi

local EntityKnifeFuncs = {}

function EntityKnifeFuncs:AddTearFlags(flags)
	ffichecks.checkcdata(2, flags, "BitSet128")
	L_EntityKnife_AddTearFlags(self, flags)
end

function EntityKnifeFuncs:ClearTearFlags(flags)
	ffichecks.checkcdata(2, flags, "BitSet128")
	L_EntityKnife_ClearTearFlags(self, flags)
end

function EntityKnifeFuncs:HasTearFlags(flags)
	ffichecks.checkcdata(2, flags, "BitSet128")
	return L_EntityKnife_HasTearFlags(self, flags)
end

function EntityKnifeFuncs:GetKnifeDistance()
	return L_EntityKnife_GetKnifeDistance(self)
end

function EntityKnifeFuncs:GetKnifeVelocity()
	return L_EntityKnife_GetKnifeVelocity(self)
end

function EntityKnifeFuncs:GetRenderZ()
	return L_EntityKnife_GetRenderZ(self)
end

function EntityKnifeFuncs:IsFlying()
	return L_EntityKnife_IsFlying(self)
end

function EntityKnifeFuncs:Reset()
	L_EntityKnife_Reset(self)
end

function EntityKnifeFuncs:SetPathFollowspeed(value)
	ffichecks.checknumber(2, value)
	L_EntityKnife_SetPathFollowSpeed(self, value)
end

function EntityKnifeFuncs:Shoot(charge, range)
	ffichecks.checknumber(2, charge)
	ffichecks.checknumber(3, range)
	L_EntityKnife_Shoot(self, charge, range)
end

local getkeys = {
    Charge = repentogon.L_EntityKnife_GetCharge,
	MaxDistance = repentogon.L_EntityKnife_GetMaxDistance,
	PathFollowSpeed = repentogon.L_EntityKnife_GetPathFollowSpeed,
	PathOffset = repentogon.L_EntityKnife_GetPathOffset,
	Rotation = repentogon.L_EntityKnife_GetRotation,
	RotationOffset = repentogon.L_EntityKnife_GetRotationOffset,
	Scale = repentogon.L_EntityKnife_GetScale,
	TearFlags = repentogon.L_EntityKnife_GetTearFlags,
}

local setkeys = {
    Charge = repentogon.L_EntityKnife_SetCharge,
	MaxDistance = repentogon.L_EntityKnife_SetMaxDistance,
	PathFollowSpeed = repentogon.L_EntityKnife_SetPathFollowSpeed,
	PathOffset = repentogon.L_EntityKnife_SetPathOffset,
	Rotation = repentogon.L_EntityKnife_SetRotation,
	RotationOffset = repentogon.L_EntityKnife_SetRotationOffset,
	Scale = repentogon.L_EntityKnife_SetScale,
	TearFlags = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "BitSet128", repentogon.L_EntityKnife_SetTearFlags) end,
}

local EntityKnifeMT
EntityKnifeMT = lffi.metatype("Entity_Knife", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityKnifeFuncs[key] ~= nil then
			return EntityKnifeFuncs[key]
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

EntityKnife = {
	IsPlayerCreep = function(variant)
		ffichecks.checknumber(1, variant)
		return L_EntityKnife_IsPlayerCreep(variant)
	end
}