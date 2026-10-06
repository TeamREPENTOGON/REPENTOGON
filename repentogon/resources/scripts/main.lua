function RegisterMod(modname, apiversion)
	local mod = {
		Name = modname,
		AddCallback = function(self, callbackId, fn, param)
			Isaac.AddCallback(self, callbackId, fn, param)
		end,
		AddPriorityCallback = function(self, callbackId, priority, fn, param)
			Isaac.AddPriorityCallback(self, callbackId, priority, fn, param)
		end,
		RemoveCallback = function(self, callbackId, fn)
			Isaac.RemoveCallback(self, callbackId, fn)
		end,
		SaveData = function(self, data)
			Isaac.SaveModData(self, data)
		end,
		LoadData = function(self)
			return Isaac.LoadModData(self)
		end,
		HasData = function(self)
			return Isaac.HasModData(self)
		end,
		RemoveData = function(self)
			Isaac.RemoveModData(self)
		end
	}
	Isaac.RegisterMod(mod, modname, apiversion)
	return mod
end

function StartDebug()
	local ok, m = pcall(require, 'mobdebug') 
	if ok and m then
		m.start()
	else
		Isaac.DebugString("Failed to start debugging.")
		-- m is now the error 
		-- Isaac.DebugString(m)
	end
end

local debug_getinfo = debug.getinfo

local function checktype(index, val, typ, level)
	local t = type(val)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level).name, typ, t), level+1)
	end
end

local function checknumber(index, val, level) checktype(index, val, "number", (level or 2)+1) end
local function checkfunction(index, val, level) checktype(index, val, "function", (level or 2)+1) end
local function checkstring(index, val, level) checktype(index, val, "string", (level or 2)+1) end
local function checktable(index, val, level) checktype(index, val, "table", (level or 2)+1) end

------------------------------------------------------------
-- Callbacks

local Callbacks = {}

local defaultCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

function _RunCallback(callbackId, param, ...)
	local callbacks = Callbacks[callbackId]
	if callbacks then
		local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams
		
		for _,v in ipairs(callbacks) do
			if matchFunc(param, v.Param) then
				local result = v.Function(v.Mod, ...)
				if result ~= nil  then
					return result
				end
			end
		end
	end
end

function _UnloadMod(mod)
	Isaac.RunCallback(ModCallbacks.MC_PRE_MOD_UNLOAD, mod)
	
	for callbackId,callbacks in pairs(Callbacks) do
		for i=#callbacks,1,-1 do
			if callbacks[i].Mod == mod then
				table.remove(callbacks, i)
			end
		end
		
		if #callbacks == 0 then
			if type(callbackId) == "number" then
				-- No more functions left, disable this callback
				Isaac.SetBuiltInCallbackState(callbackId, false)
			end
		end
	end
end

rawset(Isaac, "AddPriorityCallback", function(mod, callbackId, priority, fn, param)
	checknumber(3, priority)
	checkfunction(4, fn)
	
	local callbacks = Isaac.GetCallbacks(callbackId, true)
	local wasEmpty = #callbacks == 0
	
	local pos = #callbacks+1
	for i=#callbacks,1,-1 do
		if callbacks[i].Priority <= priority then
			break
		else
			pos = pos-1
		end
	end
	
	table.insert(callbacks, pos, {Mod = mod, Function = fn, Priority = priority, Param = param})
	
	if wasEmpty then
		if type(callbackId) == "number" then
			-- Enable this callback
			Isaac.SetBuiltInCallbackState(callbackId, true)
		end
	end
end)

rawset(Isaac, "AddCallback", function(mod, callbackId, fn, param)
	checkfunction(3, fn)
	Isaac.AddPriorityCallback(mod, callbackId, CallbackPriority.DEFAULT, fn, param)
end)

rawset(Isaac, "RemoveCallback", function(mod, callbackId, fn)
	checkfunction(3, fn)
	
	local callbacks = Callbacks[callbackId]
	if callbacks then
		for i=#callbacks,1,-1 do
			if callbacks[i].Function == fn then
				table.remove(callbacks, i)
			end
		end
		
		-- No more functions left, disable this callback
		if not next(callbacks) then
			if type(callbackId) == "number" then
				Isaac.SetBuiltInCallbackState(callbackId, false)
			end
		end
	end
end)

rawset(Isaac, "GetCallbacks", function(callbackId, createIfMissing)
	if createIfMissing and not Callbacks[callbackId] then
		Callbacks[callbackId] = setmetatable({}, defaultCallbackMeta)
	end
	
	return Callbacks[callbackId] or {}
end)

local RunCallback = _RunCallback

rawset(Isaac, "RunCallbackWithParam", RunCallback)
rawset(Isaac, "RunCallback", function(callbackID, ...) return RunCallback(callbackID, nil, ...) end)

------------------------------------------------------------
-- Constants

REPENTANCE = true
REPENTANCE_PLUS = true

------------------------------------------------------------
-- Compatibility wrappers begin here

local META, META0
local function BeginClass(T)
	META = {}
	if type(T) == "function" then
		META0 = getmetatable(T())
	else
		META0 = getmetatable(T).__class
	end
end

local function EndClass()
	local oldIndex = META0.__index
	local newMeta = META
	
	rawset(META0, "__index", function(self, k)
		return newMeta[k] or oldIndex(self, k)
	end)
end

local function tobitset128(n)
	if type(n) == "number" then
		return BitSet128(n, 0)
	else
		return n
	end
end

-- Isaac -----------------------------------------------

-- EntityPlayer Isaac.GetPlayer(int ID = 0)
-- table Isaac.QueryRadius(Vector Position, float Radius, int Partitions = 0xFFFFFFFF)
-- table Isaac.FindByType(EntityType Type, int Variant = -1, int SubType = -1, bool Cache = false, bool IgnoreFriendly = false)
-- int Isaac.CountEntities(Entity Spawner, EntityType Type = EntityType.ENTITY_NULL, int Variant = -1, int SubType = -1)

-- int Isaac.GetPlayerTypeByName(string Name, boolean IsBSkin = false)

---------------------------------------------------------
BeginClass(EntityPlayer)

-- void	EntityPlayer:AddCollectible(CollectibleType Type, int Charge = 0, boolean AddConsumables = true, ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY, int VarData = 0)
-- * Slot: Sets the active slot this collectible should be added to
-- * VarData: Sets the variable data for this collectible (this is used to store extra data for some active items like the number of uses for Jar of Wisps)
local Entity_Player_AddCollectible = META0.AddCollectible
function META:AddCollectible(id, charge, addConsumables, activeSlot, varData, pool)
	Entity_Player_AddCollectible(self, id, charge or 0, addConsumables or addConsumables == nil, activeSlot or 0, varData or 0, pool or 0)
end

-- void	EntityPlayer:RemoveCollectible(CollectibleType Type, bool IgnoreModifiers = false, ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY, bool RemoveFromPlayerForm = true)
-- * IgnoreModifiers: Ignores collectible effects granted by other items (i.e. Void)
-- * Slot: Sets the active slot this collectible should be removed from
-- * RemoveFromPlayerForm: If successfully removed and part of a transformation, decrease that transformation's counter by 1
local Entity_Player_RemoveCollectible = META0.RemoveCollectible
function META:RemoveCollectible(id, ignoreModifiers, activeSlot, removeFromPlayerForm)
	Entity_Player_RemoveCollectible(self, id, ignoreModifiers, activeSlot or 0, removeFromPlayerForm ~= false)
end

-- void	EntityPlayer:AddTrinket(TrinketType Type, boolean AddConsumables = true)
local Entity_Player_AddTrinket = META0.AddTrinket
function META:AddTrinket(id, addConsumables)
	Entity_Player_AddTrinket(self, id, addConsumables or addConsumables == nil)
end

-- CollectibleType EntityPlayer:GetActiveItem(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_GetActiveItem = META0.GetActiveItem
function META:GetActiveItem(id)
	return Entity_Player_GetActiveItem(self, id or 0)
end

-- int EntityPlayer:GetActiveCharge(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_GetActiveCharge = META0.GetActiveCharge
function META:GetActiveCharge(id)
	return Entity_Player_GetActiveCharge(self, id or 0)
end

-- int EntityPlayer:GetBatteryCharge(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_GetBatteryCharge = META0.GetBatteryCharge
function META:GetBatteryCharge(id)
	return Entity_Player_GetBatteryCharge(self, id or 0)
end

-- int EntityPlayer:GetActiveSubCharge(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_GetActiveSubCharge = META0.GetActiveSubCharge
function META:GetActiveSubCharge(id)
	return Entity_Player_GetActiveSubCharge(self, id or 0)
end

-- void EntityPlayer:SetActiveCharge(int Charge, ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_SetActiveCharge = META0.SetActiveCharge
function META:SetActiveCharge(charge, id)
	Entity_Player_SetActiveCharge(self, charge, id or 0)
end

-- void EntityPlayer:DischargeActiveItem(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_DischargeActiveItem = META0.DischargeActiveItem
function META:DischargeActiveItem(id)
	Entity_Player_DischargeActiveItem(self, id or 0)
end

-- boolean EntityPlayer:NeedsCharge(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local Entity_Player_NeedsCharge = META0.NeedsCharge
function META:NeedsCharge(id)
	return Entity_Player_NeedsCharge(self, id or 0)
end

-- boolean EntityPlayer:FullCharge(ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY, boolean Force = false)
-- * Force: If set, items will always be charged even if they normally cannot be recharged by batteries
local Entity_Player_FullCharge = META0.FullCharge
function META:FullCharge(id, force)
	return Entity_Player_FullCharge(self, id or 0, force) ~= 0
end

-- void	EntityPlayer:UseActiveItem(CollectibleType Item, UseFlag UseFlags = 0, ActiveSlot Slot = -1, int CustomVarData = 0)
--   or
-- void	EntityPlayer:UseActiveItem(CollectibleType Item, boolean ShowAnim = false, boolean KeepActiveItem = false, boolean AllowNonMainPlayer = true, boolean ToAddCostume = false, ActiveSlot Slot = -1, int CustomVarData = 0)
-- * Slot: The active slot this item was used from (set to -1 if this item wasn't triggered by any active slot)
local Entity_Player_UseActiveItem = META0.UseActiveItem
function META:UseActiveItem(item, showAnim, keepActive, allowNonMain, addCostume, activeSlot, customVarData)
	if type(showAnim) == "number" then
		-- Repentance version
		local useFlags = showAnim
		activeSlot = keepActive
		customVarData = allowNonMain
		
		Entity_Player_UseActiveItem(self, item, useFlags, activeSlot or -1, customVarData or 0)
	else
		-- AB+ backwards compatibility
		local useFlags = 0
		if showAnim == false then useFlags = useFlags + 1 end
		if keepActive == false then useFlags = useFlags + 16 end
		if allowNonMain then useFlags = useFlags + 8 end
		if addCostume == false then useFlags = useFlags + 2 end
		if customVarData then useFlags = useFlags + 1024 end
		
		Entity_Player_UseActiveItem(self, item, useFlags, activeSlot or -1, customVarData or 0)
	end
end

-- void	EntityPlayer:UseCard(Card ID, UseFlag UseFlags = 0)
local Entity_Player_UseCard = META0.UseCard
function META:UseCard(id, useFlags)
	Entity_Player_UseCard(self, id, useFlags or 0)
end

-- void	EntityPlayer:UsePill(PillEffect ID, PillColor PillColor, UseFlag UseFlags = 0)
local Entity_Player_UsePill = META0.UsePill
function META:UsePill(id, color, useFlags)
	Entity_Player_UsePill(self, id, color, useFlags or 0)
end

-- boolean EntityPlayer:HasInvincibility(DamageFlag Flags = 0)
local Entity_Player_HasInvincibility = META0.HasInvincibility
function META:HasInvincibility(damageFlags)
	return Entity_Player_HasInvincibility(self, damageFlags or 0)
end

-- MultiShotParams EntityPlayer:GetMultiShotParams(WeaponType WeaponType = WeaponType.WEAPON_TEARS)
local Entity_Player_GetMultiShotParams = META0.GetMultiShotParams
function META:GetMultiShotParams(weaponType)
	return Entity_Player_GetMultiShotParams(self, weaponType or 1)
end

-- boolean EntityPlayer:CanAddCollectible(CollectibleType Type = CollectibleType.COLLECTIBLE_NULL)
local Entity_Player_CanAddCollectible = META0.CanAddCollectible
function META:CanAddCollectible(item)
	return Entity_Player_CanAddCollectible(self, item or 0)
end

-- EntityBomb EntityPlayer:FireBomb(Vector Position, Vector Velocity, Entity Source = nil)
local Entity_Player_FireBomb = META0.FireBomb
function META:FireBomb(pos, vel, source)
	return Entity_Player_FireBomb(self, pos, vel, source)
end

-- EntityLaser EntityPlayer:FireBrimstone(Vector Position, Entity Source = nil, float DamageMultiplier = 1)
local Entity_Player_FireBrimstone = META0.FireBrimstone
function META:FireBrimstone(pos, source, mul)
	return Entity_Player_FireBrimstone(self, pos, source, mul or 1)
end

-- EntityTear EntityPlayer:FireTear(Vector Position, Vector Velocity, boolean CanBeEye = true, boolean NoTractorBeam = false, boolean CanTriggerStreakEnd = true, Entity Source = nil, float DamageMultiplier = 1)
local Entity_Player_FireTear = META0.FireTear
function META:FireTear(pos, vel, canBeEye, noTractorBeam, canTriggerStreakEnd, source, mul)
	local flags = 0
	if canBeEye == false then flags = flags + 1 end
	if noTractorBeam then flags = flags + 2 end
	if canTriggerStreakEnd == false then flags = flags + 4 end
	return Entity_Player_FireTear(self, pos, vel, flags, source, mul or 1)
end

-- EntityLaser EntityPlayer:FireTechLaser(Vector Position, LaserOffset OffsetID, Vector Direction, boolean LeftEye, boolean OneHit = false, Entity Source = nil, float DamageMultiplier = 1)
local Entity_Player_FireTechLaser = META0.FireTechLaser
function META:FireTechLaser(pos, offsetId, dir, leftEye, oneHit, source, mul)
	return Entity_Player_FireTechLaser(self, pos, offsetId, dir, leftEye, oneHit, source, mul or 1)
end

-- EntityLaser EntityPlayer:FireTechXLaser(Vector Position, Vector Direction, float Radius, Entity Source = nil, float DamageMultiplier = 1)
local Entity_Player_FireTechXLaser = META0.FireTechXLaser
function META:FireTechXLaser(pos, dir, radius, source, mul)
	return Entity_Player_FireTechXLaser(self, pos, dir, radius, source, mul or 1)
end

-- TearParams EntityPlayer:GetTearHitParams(WeaponType WeaponType, float DamageScale = 1, int TearDisplacement = 1, Entity Source = nil)
local Entity_Player_GetTearHitParams = META0.GetTearHitParams
function META:GetTearHitParams(weapon, scale, disp, src)
	return Entity_Player_GetTearHitParams(self, weapon, scale or 1, disp or 1, src)
end

-- void EntityPlayer:AnimateCard(Card ID, string AnimName = "Pickup")
local Entity_Player_AnimateCard = META0.AnimateCard
function META:AnimateCard(id, anim)
	return Entity_Player_AnimateCard(self, id, anim or "Pickup")
end

-- void EntityPlayer:AnimatePill(PillColor ID, string AnimName = "Pickup")
local Entity_Player_AnimatePill = META0.AnimatePill
function META:AnimatePill(id, anim)
	return Entity_Player_AnimatePill(self, id, anim or "Pickup")
end

-- void EntityPlayer:AnimateTrinket(TrinketType ID, string AnimName = "Pickup", string SpriteAnimName = "PlayerPickupSparkle")
local Entity_Player_AnimateTrinket = META0.AnimateTrinket
function META:AnimateTrinket(id, anim, spriteAnim)
	return Entity_Player_AnimateTrinket(self, id, anim or "Pickup", spriteAnim or "PlayerPickupSparkle")
end

-- void EntityPlayer:AnimateCollectible(CollectibleType ID, string AnimName = "Pickup", string SpriteAnimName = "PlayerPickupSparkle")
local Entity_Player_AnimateCollectible = META0.AnimateCollectible
function META:AnimateCollectible(id, anim, spriteAnim)
	return Entity_Player_AnimateCollectible(self, id, anim or "Pickup", spriteAnim or "PlayerPickupSparkle")
end

-- boolean EntityPlayer:HasCollectible(CollectibleType Type, boolean IgnoreModifiers = false)
-- * IgnoreModifiers: If set to true, only counts collectibles the player actually owns and ignores effects granted by items like Zodiac, 3 Dollar Bill and Lemegeton

-- int EntityPlayer:GetCollectibleNum(CollectibleType Type, boolean IgnoreModifiers = false)
-- * IgnoreModifiers: Same as above

-- boolean EntityPlayer:HasTrinket(TrinketType Type, boolean IgnoreModifiers = false)
-- * IgnoreModifiers: If set to true, only counts trinkets the player actually holds and ignores effects granted by other items

-- Backwards compatibility
META.GetMaxPoketItems = META0.GetMaxPocketItems
META.DropPoketItem = META0.DropPocketItem

-- void EntityPlayer:ChangePlayerType(PlayerType Type)

-- void EntityPlayer:AddBrokenHearts(int Num)
-- int EntityPlayer:GetBrokenHearts()
-- void EntityPlayer:AddRottenHearts(int Num)
-- int EntityPlayer:GetRottenHearts()

-- void EntityPlayer:AddSoulCharge(int Num)
-- void EntityPlayer:SetSoulCharge(int Num)
-- int EntityPlayer:GetSoulCharge()
-- int EntityPlayer:GetEffectiveSoulCharge()

-- void EntityPlayer:AddBloodCharge(int Num)
-- void EntityPlayer:SetBloodCharge(int Num)
-- int EntityPlayer:GetBloodCharge()
-- int EntityPlayer:GetEffectiveBloodCharge()

-- boolean EntityPlayer:CanPickRottenHearts()

-- EntityPlayer EntityPlayer:GetMainTwin()
-- EntityPlayer EntityPlayer:GetOtherTwin()

-- boolean EntityPlayer:TryHoldEntity(Entity Ent)
-- Entity EntityPlayer:ThrowHeldEntity(Vector Velocity)

-- EntityFamiliar EntityPlayer:AddFriendlyDip(int Subtype, Vector Position)
-- EntityFamiliar EntityPlayer:AddWisp(int Subtype, Vector Position, boolean AdjustOrbitLayer = false, boolean DontUpdate = false)
-- EntityFamiliar EntityPlayer:AddItemWisp(int Subtype, Vector Position, boolean AdjustOrbitLayer = false)
-- EntityFamiliar EntityPlayer:AddSwarmFlyOrbital(Vector Position)

-- int EntityPlayer:GetNumGigaBombs()
-- void EntityPlayer:AddGigaBombs(int Num)

-- CollectibleType EntityPlayer:GetModelingClayEffect()
-- void EntityPlayer:AddCurseMistEffect()
-- void EntityPlayer:RemoveCurseMistEffect()
-- boolean EntityPlayer:HasCurseMistEffect()

-- boolean EntityPlayer:IsCoopGhost()

-- EntityFamiliar EntityPlayer:AddMinisaac(Vector Position, boolean PlayAnim = true)
local Entity_Player_AddMinisaac = META0.AddMinisaac
function META:AddMinisaac(pos, playAnim)
	return Entity_Player_AddMinisaac(self, pos, playAnim ~= false)
end

-- void EntityPlayer:TriggerBookOfVirtues(CollectibleType Type = CollectibleType.COLLECTIBLE_NULL, int Charge = 0)
local Entity_Player_TriggerBookOfVirtues = META0.TriggerBookOfVirtues
function META:TriggerBookOfVirtues(id, charge)
	Entity_Player_TriggerBookOfVirtues(self, id or 0, charge or 0)
end

-- void EntityPlayer:SetPocketActiveItem(CollectibleType Type, ActiveSlot Slot = ActiveSlot.SLOT_POCKET, boolean KeepInPools = false)
local Entity_Player_SetPocketActiveItem = META0.SetPocketActiveItem
function META:SetPocketActiveItem(id, slot, keep)
	Entity_Player_SetPocketActiveItem(self, id, slot or ActiveSlot.SLOT_POCKET, keep)
end

EndClass()

---------------------------------------------------------

Game_0 = nil

if not _LUADEBUG then
	debug = nil
	arg = nil
	dofile = nil
	loadfile = nil
end
