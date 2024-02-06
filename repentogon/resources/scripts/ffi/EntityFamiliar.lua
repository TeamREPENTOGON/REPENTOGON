ffi.cdef[[
	void L_EntityFamiliar_AddCoins(Entity_Familiar*, int);
	void L_EntityFamiliar_AddHearts(Entity_Familiar*, int);
	void L_EntityFamiliar_AddKeys(Entity_Familiar*, int);
	void L_EntityFamiliar_AddToDelayed(Entity_Familiar*);
	void L_EntityFamiliar_AddToFollowers(Entity_Familiar*);
	void L_EntityFamiliar_AddToOrbit(Entity_Familiar*, int);
	Entity_Tear* L_EntityFamiliar_FireProjectile(Entity_Familiar*, Vector*);
	void L_EntityFamiliar_FollowParent(Entity_Familiar*);
	void L_EntityFamiliar_FollowPosition(Entity_Familiar*, Vector*);
	Vector* L_EntityFamiliar_StaticGetOrbitDistance(int);
	Vector L_EntityFamiliar_GetOrbitPosition(Entity_Familiar*, Vector*);
	void L_EntityFamiliar_MoveDelayed(Entity_Familiar*, int);
	void L_EntityFamiliar_MoveDiagonally(Entity_Familiar*, float);
	void L_EntityFamiliar_PickEnemyTarget(Entity_Familiar*, float, int, Vector*, float);
	void L_EntityFamiliar_PlayChargeAnim(Entity_Familiar*, unsigned int);
	void L_EntityFamiliar_PlayFloatAnim(Entity_Familiar*, unsigned int);
	void L_EntityFamiliar_PlayShootAnim(Entity_Familiar*, unsigned int);
	unsigned int L_EntityFamiliar_RecalculateOrbitOffset(Entity_Familiar*, int, bool);
	void L_EntityFamiliar_RemoveFromDelayed(Entity_Familiar*);
	void L_EntityFamiliar_RemoveFromFollowers(Entity_Familiar*);
	void L_EntityFamiliar_RemoveFromOrbit(Entity_Familiar*);
	void L_EntityFamiliar_Shoot(Entity_Familiar*);
	int L_EntityFamiliar_GetCoins(Entity_Familiar*);
	void L_EntityFamiliar_SetCoins(Entity_Familiar*, int);
	unsigned int L_EntityFamiliar_GetFireCooldown(Entity_Familiar*);
	void L_EntityFamiliar_SetFireCooldown(Entity_Familiar*, unsigned int);
	unsigned int L_EntityFamiliar_GetHeadFrameDelay(Entity_Familiar*);
	void L_EntityFamiliar_SetHeadFrameDelay(Entity_Familiar*, unsigned intint);
	int L_EntityFamiliar_GetHearts(Entity_Familiar*);
	void L_EntityFamiliar_SetHearts(Entity_Familiar*, int);
	bool L_EntityFamiliar_GetIsDelayed(Entity_Familiar*);
	void L_EntityFamiliar_SetIsDelayed(Entity_Familiar*, bool);
	bool L_EntityFamiliar_GetIsFollower(Entity_Familiar*);
	void L_EntityFamiliar_SetIsFollower(Entity_Familiar*, bool);
	int L_EntityFamiliar_GetKeys(Entity_Familiar*);
	void L_EntityFamiliar_SetKeys(Entity_Familiar*, int);
	unsigned int L_EntityFamiliar_GetLastDirection(Entity_Familiar*);
	void L_EntityFamiliar_SetLastDirection(Entity_Familiar*, unsigned int);
	unsigned int L_EntityFamiliar_GetMoveDirection(Entity_Familiar*);
	void L_EntityFamiliar_SetMoveDirection(Entity_Familiar*, unsigned int);
	float L_EntityFamiliar_GetOrbitAngleOffset(Entity_Familiar*);
	void L_EntityFamiliar_SetOrbitAngleOffset(Entity_Familiar*, float);
	Vector* L_EntityFamiliar_GetOrbitDistance(Entity_Familiar*);
	void L_EntityFamiliar_SetOrbitDistance(Entity_Familiar*, Vector*);
	int L_EntityFamiliar_GetOrbitLayer(Entity_Familiar*);
	void L_EntityFamiliar_SetOrbitLayer(Entity_Familiar*, int);
	float L_EntityFamiliar_GetOrbitSpeed(Entity_Familiar*);
	void L_EntityFamiliar_SetOrbitSpeed(Entity_Familiar*, float);
	Entity_Player* L_EntityFamiliar_GetPlayer(Entity_Familiar*);
	void L_EntityFamiliar_SetPlayer(Entity_Familiar*, Entity_Player*);
	int L_EntityFamiliar_GetRoomClearCount(Entity_Familiar*);
	void L_EntityFamiliar_SetRoomClearCount(Entity_Familiar*, int);
	unsigned int L_EntityFamiliar_GetShootDirection(Entity_Familiar*);
	void L_EntityFamiliar_SetShootDirection(Entity_Familiar*, unsigned int);
	int L_EntityFamiliar_GetState(Entity_Familiar*);
	void L_EntityFamiliar_SetState(Entity_Familiar*, int);
]]

local repentogon = ffidll
local lffi = ffi

local EntityFamiliarFuncs = {}

function EntityFamiliarFuncs:AddCoins(value)
	ffichecks.checknumber(2, value)
	L_EntityFamiliar_AddCoins(self, value)
end

function EntityFamiliarFuncs:AddHearts(value)
	ffichecks.checknumber(2, value)
	L_EntityFamiliar_AddHearts(self, value)
end

function EntityFamiliarFuncs:AddKeys(value)
	ffichecks.checknumber(2, value)
	L_EntityFamiliar_AddKeys(self, value)
end

function EntityFamiliarFuncs:AddToDelayed()
	L_EntityFamiliar_AddToDelayed(self)
end

function EntityFamiliarFuncs:AddToFollowers()
	L_EntityFamiliar_AddToFollowers(self)
end

function EntityFamiliarFuncs:AddToOrbit(layer)
	ffichecks.checknumber(2, layer)
	L_EntityFamiliar_AddToOrbit(self, layer)
end

function EntityFamiliarFuncs:FireProjectile(dir)
	ffichecks.checkcdata(2, dir, "Vector")
	return ffichecks.fixreturn(L_EntityFamiliar_FireProjectile(self, dir))
end

function EntityFamiliarFuncs:FollowParent()
	L_EntityFamiliar_FollowParent(self)
end

function EntityFamiliarFuncs:FollowPosition(pos)
	ffichecks.checkcdata(2, pos, "Vector")
	L_EntityFamiliar_FollowPosition(self, pos)
end

function EntityFamiliarFuncs:MoveDelayed(value)
	ffichecks.checknumber(2, value)
	L_EntityFamiliar_MoveDelayed(self, value)
end

function EntityFamiliarFuncs:MoveDiagonally(value)
	ffichecks.checknumber(2, value)
	L_EntityFamiliar_MoveDiagonally(self, value)
end

function EntityFamiliarFuncs:PickEnemyTarget(maxDistance, frameInterval, flags, coneDir, coneAngle)
	ffichecks.checknumber(2, maxDistance)
	frameInterval = ffichecks.optnumber(frameInterval, 13)
	flags = ffichecks.optnumber(flags, 0)
	coneDir = ffichecks.optcdata(coneDir, "Vector", Vector.Zero)
	coneAngle = ffichecks.optnumber(coneAngle, 15)
	L_EntityFamiliar_PickEnemyTarget(self, maxDistance, frameInterval, flags, coneDir, coneAngle)
end

function EntityFamiliarFuncs:PlayChargeAnim(dir)
	ffichecks.checknumber(2, dir)
	L_EntityFamiliar_PlayChargeAnim(self, dir)
end

function EntityFamiliarFuncs:PlayFloatAnim(dir)
	ffichecks.checknumber(2, dir)
	L_EntityFamiliar_PlayFloatAnim(self, dir)
end

function EntityFamiliarFuncs:PlayShootAnim(dir)
	ffichecks.checknumber(2, dir)
	L_EntityFamiliar_PlayShootAnim(self, dir)
end

function EntityFamiliarFuncs:RecalculateOrbitOffset(layer, add)
	ffichecks.checknumber(2, layer)
	ffichecks.checkboolean(3, add)
	L_EntityFamiliar_RecalculateOrbitOffset(self, layer, add)
end

function EntityFamiliarFuncs:RemoveFromDelayed()
	L_EntityFamiliar_RemoveFromDelayed(self)
end

function EntityFamiliarFuncs:RemoveFromFollowers()
	L_EntityFamiliar_RemoveFromFollowers(self)
end

function EntityFamiliarFuncs:RemoveFromOrbit()
	L_EntityFamiliar_RemoveFromOrbit(self)
end

function EntityFamiliarFuncs:Shoot()
	L_EntityFamiliar_Shoot(self)
end

local getkeys = {
    Coins = repentogon.L_EntityFamiliar_GetCoins,
	FireCooldown = repentogon.L_EntityFamiliar_GetFireCooldown,
	HeadFrameDelay = repentogon.L_EntityFamiliar_GetHeadFrameDelay,
	Hearts = repentogon.L_EntityFamiliar_GetHearts,
	IsDelayed = repentogon.L_EntityFamiliar_GetIsDelayed,
	IsFollower = repentogon.L_EntityFamiliar_GetIsFollower,
	Keys = repentogon.L_EntityFamiliar_GetKeys,
	LastDirection = repentogon.L_EntityFamiliar_GetLastDirection,
	MoveDirection = repentogon.L_EntityFamiliar_GetMoveDirection,
	OrbitAngleOffset = repentogon.L_EntityFamiliar_GetOrbitAngleOffset,
	OrbitDistance = repentogon.L_EntityFamiliar_GetOrbitDistance,
	OrbitLayer = repentogon.L_EntityFamiliar_GetOrbitLayer,
	OrbitSpeed = repentogon.L_EntityFamiliar_GetOrbitSpeed,
	Player = repentogon.L_EntityFamiliar_GetPlayer,
	RoomClearCount = repentogon.L_EntityFamiliar_GetRoomClearCount,
	ShootDirection = repentogon.L_EntityFamiliar_GetShootDirection,
	State = repentogon.L_EntityFamiliar_GetState,
}

local setkeys = {
	Coins = repentogon.L_EntityFamiliar_SetCoins,
	FireCooldown = repentogon.L_EntityFamiliar_SetFireCooldown,
	HeadFrameDelay = repentogon.L_EntityFamiliar_SetHeadFrameDelay,
	Hearts = repentogon.L_EntityFamiliar_SetHearts,
	IsDelayed = repentogon.L_EntityFamiliar_SetIsDelayed,
	IsFollower = repentogon.L_EntityFamiliar_SetIsFollower,
	Keys = repentogon.L_EntityFamiliar_SetKeys,
	LastDirection = repentogon.L_EntityFamiliar_SetLastDirection,
	MoveDirection = repentogon.L_EntityFamiliar_SetMoveDirection,
	OrbitAngleOffset = repentogon.L_EntityFamiliar_SetOrbitAngleOffset,
	OrbitDistance = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Vector", repentogon.L_EntityFamiliar_SetOrbitDistance) end,
	OrbitLayer = repentogon.L_EntityFamiliar_SetOrbitLayer,
	OrbitSpeed = repentogon.L_EntityFamiliar_SetOrbitSpeed,
	Player = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "Entity_Player", repentogon.L_EntityFamiliar_SetPlayer) end,
	RoomClearCount = repentogon.L_EntityFamiliar_SetRoomClearCount,
	ShootDirection = repentogon.L_EntityFamiliar_SetShootDirection,
	State = repentogon.L_EntityFamiliar_SetState,
}

local EntityFamiliarMT
EntityFamiliarMT = lffi.metatype("Entity_Familiar", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityFamiliarFuncs[key] ~= nil then
			return EntityFamiliarFuncs[key]
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