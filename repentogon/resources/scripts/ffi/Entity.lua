ffi.cdef[[
	void L_Entity_AddBurn(Entity*, const EntityRef&, int, float);
	void L_Entity_AddCharm(Entity*, const EntityRef&, int);
	void L_Entity_AddConfustion(Entity*, const EntityRef&, int);
	void L_Entity_AddFreeze(Entity*, const EntityRef&, int);
	void L_Entity_AddHealth(Entity*, float);
	void L_Entity_AddMidasFreeze(Entity*, const EntityRef&, int);
	void L_Entity_AddPoison(Entity*, const EntityRef&, int, float);
	void L_Entity_AddShrink(Entity*, const EntityRef&, int);
	void L_Entity_AddSlowing(Entity*, const EntityRef&, int, ColorMod*);
	void L_Entity_AddVelocity(Entity*, Vector*);
	void L_Entity_BloodExplode(Entity*);
	void L_Entity_ClearFlags(Entity*, unsigned long long*);
	bool L_Entity_CollidesWithGrid(Entity*);
	void L_Entity_Die(Entity*);
	bool L_Entity_Exists(Entity*);
	int L_Entity_GetBossID(Entity*);
	ColorMod* L_Entity_GetColorFunc(Entity*);
	RNG* L_Entity_GetDropRNG(Entity*);
	unsigned long long* L_Entity_GetFlags(Entity*);
	Entity* L_Entity_GetLastChild(Entity*);
	Entity* L_Entity_GetLastParent(Entity*);
	ANM2* L_Entity_GetSprite(Entity*);
	bool L_Entity_HasCommonParentWithEntity(Entity*, Entity*);
	bool L_Entity_HasEntityFlags(Entity*, unsigned long long*);
	bool L_Entity_HasFullHealth(Entity*);
	bool L_Entity_HasMortalDamage(Entity*);
	bool L_Entity_IsActiveEnemy(Entity*, bool);
	bool L_Entity_IsDead(Entity*);
	bool L_Entity_IsEnemy(Entity*);
	bool L_Entity_IsFlying(Entity*);
	bool L_Entity_IsFrame(Entity*, int);
	bool L_Entity_IsVisible(Entity*);
	bool L_Entity_IsVulnerableEnemy(Entity*);
	void L_Entity_Kill(Entity*);
	void L_Entity_MultiplyFriction(Entity*);
	void L_Entity_PostRender(Entity*);
	void L_Entity_Remove(Entity*);
	void L_Entity_RemoveStatusEffects(Entity*);
	void L_Entity_Render(Entity*, Vector*);
	bool L_Entity_RenderShadowLayer(Entity*, Vector*);
	void L_Entity_SetColorFunc(Entity*, ColorMod*, int, int, bool, bool);
	void L_Entity_SetSizeFunc(Entity*, float, Vector*, int);
	void L_Entity_SetSpriteFrame(Entity*, char*, int);
	bool L_Entity_TakeDamage(Entity*, float, unsigned long long*, EntityRef*, int);
	void L_Entity_Update(Entity*);
	Entity* L_Entity_GetChild(Entity*);
	void L_Entity_SetChild(Entity*, Entity*);
	Entity* L_Entity_GetParent(Entity*);
	void L_Entity_SetParent(Entity*, Entity*);
	ColorMod* L_Entity_GetColorField(Entity*);
	void L_Entity_SetColorField(Entity*, ColorMod*);
	float L_Entity_GetDepthOffset(Entity*);
	void L_Entity_SetDepthOffset(Entity*, float);
	unsigned int L_Entity_GetDropSeed(Entity*);
	void L_Entity_SetDropSeed(Entity*, unsigned int);
	unsigned int L_Entity_GetEntityCollisionClass(Entity*);
	void L_Entity_SetEntityCollisionClass(Entity*, unsigned int);
	unsigned int L_Entity_GetGridCollisionClass(Entity*);
	void L_Entity_SetGridCollisionClass(Entity*, unsigned int);
	bool L_Entity_GetFlipX(Entity*);
	void L_Entity_SetFlipX(Entity*, bool);
	unsigned int L_Entity_GetFrameCount(Entity*);
	void L_Entity_SetFrameCount(Entity*, unsigned int);
	float L_Entity_GetFriction(Entity*);
	void L_Entity_SetFriction(Entity*, float);
	float L_Entity_GetHitPoints(Entity*);
	void L_Entity_SetHitPoints(Entity*, float);
	float L_Entity_GetMaxHitPoints(Entity*);
	void L_Entity_SetMaxHitPoints(Entity*, float);
	unsigned int L_Entity_GetIndex(Entity*);
	unsigned int L_Entity_GetInitSeed(Entity*);
	void L_Entity_SetInitSeed(Entity*, unsigned int);
	float L_Entity_GetMass(Entity*);
	void L_Entity_SetMass(Entity*, float);
	Vector* L_Entity_GetPosition(Entity*);
	void L_Entity_SetPosition(Entity*, Vector*);
	Vector* L_Entity_GetPositionOffset(Entity*);
	void L_Entity_SetPositionOffset(Entity*, Vector*);
	int L_Entity_GetRenderZOffset(Entity*);
	void L_Entity_SetRenderZOffset(Entity*, int);
	float L_Entity_GetSizeField(Entity*);
	void L_Entity_SetSizeField(Entity*, float);
	Vector* L_Entity_GetSizeMulti(Entity*);
	void L_Entity_SetSizeMulti(Entity*, Vector*);
	int L_Entity_GetSortingLayer(Entity*);
	void L_Entity_SetSortingLayer(Entity*, int);
	Entity* L_Entity_GetSpawnerEntity(Entity*);
	void L_Entity_SetSpawnerEntity(Entity*, Entity*);
	unsigned int L_Entity_GetSpawnerType(Entity*);
	void L_Entity_SetSpawnerType(Entity*, unsigned int);
	unsigned int L_Entity_GetSpawnerVariant(Entity*);
	void L_Entity_SetSpawnerVariant(Entity*, unsigned int);
	unsigned int L_Entity_GetSpawnGridIndex(Entity*);
	void L_Entity_SetSpawnGridIndex(Entity*, unsigned int);
	Vector* L_Entity_GetSpriteOffset(Entity*);
	void L_Entity_SetSpriteOffset(Entity*, Vector*);
	ColorMod* L_Entity_GetSplatColor(Entity*);
	void L_Entity_SetSplatColor(Entity*, ColorMod*);
	Vector* L_Entity_GetSpriteScale(Entity*);
	void L_Entity_SetSpriteScale(Entity*, Vector*);
	float L_Entity_GetSpriteRotation(Entity*);
	void L_Entity_SetSpriteRotation(Entity*, float);
	Entity* L_Entity_GetTarget(Entity*);
	void L_Entity_SetTarget(Entity*, Entity*);
	Vector* L_Entity_GetTargetPosition(Entity*);
	void L_Entity_SetTargetPosition(Entity*, Vector*);
	unsigned int L_Entity_GetType(Entity*);
	void L_Entity_SetType(Entity*, unsigned int);
	unsigned int L_Entity_GetVariant(Entity*);
	void L_Entity_SetVariant(Entity*, unsigned int);
	unsigned int L_Entity_GetSubType(Entity*);
	void L_Entity_SetSubType(Entity*, unsigned int);
	Vector* L_Entity_GetVelocity(Entity*);
	void L_Entity_SetVelocity(Entity*, Vector*);
	bool L_Entity_GetVisible(Entity*);
	void L_Entity_SetVisible(Entity*, bool);
]]

local repentogon = ffidll
local lffi = ffi

local EntityFuncs = {}

function EntityFuncs:AddBurn(ref, duration, damage)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	damage = ffichecks.optnumber(4, damage, 3.5)
	repentogon.L_Entity_AddBurn(self, ref, duration, damage)
end

function EntityFuncs:AddCharm(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddCharm(self, ref, duration)
end

function EntityFuncs:AddConfusion(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddConfustion(self, ref, duration)
end

function EntityFuncs:AddFreeze(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddFreeze(self, ref, duration)
end

function EntityFuncs:AddHealth(value)
	ffichecks.checknumber(2, value)
	repentogon.L_Entity_AddHealth(self, value)
end

function EntityFuncs:AddMidasFreeze(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddMidasFreeze(self, ref, duration)
end

function EntityFuncs:AddPoison(ref, duration, damage)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	damage = ffichecks.optnumber(4, damage, 3.5)
	repentogon.L_Entity_AddPoison(self, ref, duration, damage)
end

function EntityFuncs:AddShrink(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddShrink(self, ref, duration)
end

function EntityFuncs:AddSlowing(ref, duration)
	ffichecks.checkcdata(2, ref, "EntityRef")
	ffichecks.checknumber(3, duration)
	repentogon.L_Entity_AddSlowing(self, ref, duration)
end

function EntityFuncs:AddVelocity(value)
	ffichecks.checkcdata(2, value, "Vector")
	repentogon.L_Entity_AddVelocity(self, value)
end

function EntityFuncs:BloodExplode()
	repentogon.L_Entity_BloodExplode(self)
end

function EntityFuncs:ClearFlags(flags)
	ffichecks.checknumber(2, flags)
	repentogon.L_Entity_ClearFlags(self, flags)
end

function EntityFuncs:CollidesWithGrid()
	return repentogon.L_Entity_CollidesWithGrid(self)
end

function EntityFuncs:Die()
	repentogon.L_Entity_Die(self)
end

function EntityFuncs:Exists()
	return repentogon.L_Entity_Exists(self)
end

function EntityFuncs:GetBossID()
	return repentogon.L_Entity_GetBossID(self)
end

function EntityFuncs:GetColor()
	return repentogon.L_Entity_GetColorFunc(self)
end

function EntityFuncs:GetDropRNG()
	return repentogon.L_Entity_GetDropRNG(self)
end

function EntityFuncs:GetFlags()
	return repentogon.L_Entity_GetFlags(self)
end

function EntityFuncs:GetLastChild()
	return repentogon.L_Entity_GetLastChild(self)
end

function EntityFuncs:GetLastParnt()
	return repentogon.L_Entity_GetLastParent(self)
end

function EntityFuncs:GetSprite()
	return repentogon.L_Entity_GetSprite(self)
end

function EntityFuncs:HasCommonParentWithEntity(entity)
	ffichecks.checkcdata(2, entity, "Entity")
	return repentogon.L_Entity_HasCommonParentWithEntity(self, entity)
end

function EntityFuncs:HasEntityFlags(flags)
	ffichecks.checknumber(2, flags)
	return repentogon.L_Entity_HasEntityFlags(self, flags)
end

function EntityFuncs:HasFullHealth()
	return repentogon.L_Entity_HasFullHealth(self)
end

function EntityFuncs:HasMortalDamage()
	return repentogon.L_Entity_HasMortalDamage(self)
end

function EntityFuncs:IsActiveEnemy()
	return repentogon.L_Entity_IsActiveEnemy(self)
end

function EntityFuncs:IsDead()
	return repentogon.L_Entity_IsDead(self)
end

function EntityFuncs:IsEnemy()
	return repentogon.L_Entity_IsEnemy(self)
end

function EntityFuncs:IsFlying()
	return repentogon.L_Entity_IsFlying(self)
end

function EntityFuncs:IsFrame(frame)
	ffichecks.checknumber(2, frame)
	return repentogon.L_Entity_IsFrame(self, frame)
end

function EntityFuncs:IsVisible()
	return repentogon.L_Entity_IsVisible(self)
end

function EntityFuncs:IsVulnerableEnemy()
	return repentogon.L_Entity_IsVulnerableEnemy(self)
end

function EntityFuncs:Kill()
	repentogon.L_Entity_Kill(self)
end

function EntityFuncs:MultiplyFriction(value)
	ffichecks.checknumber(2, value)
	repentogon.L_Entity_MultiplyFriction(self, value)
end

function EntityFuncs:PostRender()
	repentogon.L_Entity_PostRender(self)
end

function EntityFuncs:Remove()
	repentogon.L_Entity_Remove(self)
end

function EntityFuncs:RemoveStatusEffects()
	repentogon.L_Entity_RemoveStatusEffects(self)
end

function EntityFuncs:Render(offset)
	ffichecks.checkcdata(2, offset, "Vector")
	repentogon.L_Entity_Render(self, offset)
end

function EntityFuncs:RenderShadowLayer(offset)
	ffichecks.checkcdata(2, offset, "Vector")
	return repentogon.L_Entity_RenderShadowLayer(self, offset)
end

function EntityFuncs:SetColor(color, duration, priority, fadeout, shared)
	ffichecks.checkcdata(2, color, "ColorMod")
	ffichecks.checknumber(3, duration)
	ffichecks.checknumber(4, priority)
	ffichecks.checkboolean(5, fadeout)
	ffichecks.checkboolean(6, shared)
	repentogon.L_Entity_SetColorFunc(self, color, duration, priority, fadeout, shared)
end

function EntityFuncs:SetSize(size, sizeMulti, numGridPoints)
	ffichecks.checknumber(2, size)
	ffichecks.checkcdata(3, sizeMulti, "Vector")
	ffichecks.checknumber(4, numGridPoints)
	repentogon.L_Entity_SetSizeFunc(self, size, sizeMulti, numGridPoints)
end

function EntityFuncs:SetSpriteFrame(name, frame)
	ffichecks.checkstring(2, name)
	ffichecks.checknumber(3, frame)
	repentogon.L_Entity_SetSpriteFrame(self, name, frame)
end

function EntityFuncs:TakeDamage(damage, flags, source, countdown)
	ffichecks.checknumber(2, damage)
	ffichecks.checknumber(3, flags)
	ffichecks.checkcdata(4, source, "EntityRef")
	ffichecks.checknumber(5, countdown)
	return repentogon.L_Entity_TakeDamage(self, damage, flags, source, countdown)
end

function EntityFuncs:Update()
	repentogon.L_Entity_Update(self)
end

local getkeys = {
    Child = repentogon.L_Entity_GetChild,
	Parent = repentogon.L_Entity_GetParent,
	Color = repentogon.L_Entity_GetColorField,
	DepthOffset = repentogon.L_Entity_GetDepthOffset,
	DropSeed = repentogon.L_Entity_GetDropSeed,
	EntityCollisionClass = repentogon.L_Entity_GetEntityCollisionClass,
	GridCollisionClass = repentogon.L_Entity_GetGridCollisionClass,
	FlipX = repentogon.L_Entity_GetFlipX,
	FrameCount = repentogon.L_Entity_GetFrameCount,
	Friction = repentogon.L_Entity_GetFriction,
	HitPoints = repentogon.L_Entity_GetHitPoints,
	Index = repentogon.L_Entity_GetIndex,
	InitSeed = repentogon.L_Entity_GetInitSeed,
	Mass = repentogon.L_Entity_GetMass,
	Position = repentogon.L_Entity_GetPosition,
	PositionOffset = repentogon.L_Entity_GetPositionOffset,
	RenderZOffset = repentogon.L_Entity_GetRenderZOffset,
	Size = repentogon.L_Entity_GetSizeField,
	SizeMulti = repentogon.L_Entity_GetSizeMulti,
	SortingLayer = repentogon.L_Entity_GetSortingLayer,
	SpawnerEntity = repentogon.L_Entity_GetSpawnerEntity,
	SpawnerType = repentogon.L_Entity_GetSpawnerType,
	SpawnerVariant = repentogon.L_Entity_GetSpawnerVariant,
	SpawnGridIndex = repentogon.L_Entity_GetSpawnGridIndex,
	SpriteOffset = repentogon.L_Entity_GetSpriteOffset,
	SplatColor = repentogon.L_Entity_GetSplatColor,
	SpriteRotation = repentogon.L_Entity_GetSpriteRotation,
	SpriteScale = repentogon.L_Entity_GetSpriteScale,
	Target = repentogon.L_Entity_GetTarget,
	TargetPosition = repentogon.L_Entity_GetTargetPosition,
	Type = repentogon.L_Entity_GetType,
	Variant = repentogon.L_Entity_GetVariant,
	SubType = repentogon.L_Entity_GetSubType,
	Velocity = repentogon.L_Entity_GetVelocity,
	Visible = repentogon.L_Entity_GetVisible,
}

local function doCDataFunc(entity, cdata, ctype, cfunc)
	ffichecks.checkcdata(2, cdata, ctype)
	cfunc(self, cdata)
end

local setkeys = {
    Child = function(self, cdata) doCDataFunc(self, cdata, "Entity", repentogon.L_Entity_SetChild) end,
	Parent = function(self, cdata) doCDataFunc(self, cdata, "Entity", repentogon.L_Entity_SetParent) end,
	Color = function(self, cdata) doCDataFunc(self, cdata, "ColorMod", repentogon.L_Entity_SetColorField) end,
	DepthOffset = repentogon.L_Entity_SetDepthOffset,
	DropSeed = repentogon.L_Entity_SetDropSeed,
	EntityCollisionClass = repentogon.L_Entity_SetEntityCollisionClass,
	GridCollisionClass = repentogon.L_Entity_SetGridCollisionClass,
	FlipX = repentogon.L_Entity_SetFlipX,
	FrameCount = repentogon.L_Entity_SetFrameCount,
	Friction = repentogon.L_Entity_SetFriction,
	HitPoints = repentogon.L_Entity_SetHitPoints,
	InitSeed = repentogon.L_Entity_SetInitSeed,
	Mass = repentogon.L_Entity_SetMass,
	Position = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetPosition) end,
	PositionOffset = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetPositionOffset) end,
	RenderZOffset = repentogon.L_Entity_SetRenderZOffset,
	Size = repentogon.L_Entity_SetSizeField,
	SizeMulti = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetSizeMulti) end,
	SortingLayer = repentogon.L_Entity_SetSortingLayer,
	SpawnerEntity = function(self, cdata) doCDataFunc(self, cdata, "Entity", repentogon.L_Entity_SetSpawnerEntity) end
	SpawnerType = repentogon.L_Entity_SetSpawnerType,
	SpawnerVariant = repentogon.L_Entity_SetSpawnerVariant,
	SpawnGridIndex = repentogon.L_Entity_SetSpawnGridIndex,
	SpriteOffset = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetSpriteOffset) end,
	SplatColor = repentogon.L_Entity_SetSplatColor,
	SpriteRotation = repentogon.L_Entity_SetSpriteRotation,
	SpriteScale = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetSpriteScale) end,
	Target = function(self, cdata) doCDataFunc(self, cdata, "Entity", repentogon.L_Entity_SetTarget) end,
	TargetPosition = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetTargetPosition) end,
	Type = repentogon.L_Entity_SetType,
	Variant = repentogon.L_Entity_SetVariant,
	SubType = repentogon.L_Entity_SetSubType,
	Velocity = function(self, cdata) doCDataFunc(self, cdata, "Vector", repentogon.L_Entity_SetVelocity) end,
	Visible = repentogon.L_Entity_SetVisible,
}

local EntityMT = lffi.metatype("Entity", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        end
		return EntityFuncs[key]
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
})

Entity = setmetatable({}, EntityMT)