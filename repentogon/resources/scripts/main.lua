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

-- Vector.Zero
rawset(Vector, "Zero", Vector(0,0))

-- Vector.One
rawset(Vector, "One", Vector(1,1))

-- Color.Default
rawset(Color, "Default", Color(1,1,1,1,0,0,0))

-- KColor presets
rawset(KColor, "Black", KColor(0, 0, 0, 1))
rawset(KColor, "Red", KColor(1, 0, 0, 1))
rawset(KColor, "Green", KColor(0, 1, 0, 1))
rawset(KColor, "Blue", KColor(0, 0, 1, 1))
rawset(KColor, "Yellow", KColor(1, 1, 0, 1))
rawset(KColor, "Cyan", KColor(0, 1, 1, 1))
rawset(KColor, "Magenta", KColor(1, 0, 1, 1))
rawset(KColor, "White", KColor(1, 1, 1, 1))
rawset(KColor, "Transparent", KColor(0, 0, 0, 0))

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
BeginClass(Font)

local Font_GetStringWidth = META0.GetStringWidth
local Font_DrawString = META0.DrawString

-- Legacy
function META:GetStringWidthUTF8(str)
	return Font_GetStringWidth(self, str)
end

function META:DrawString(str, x, y, param4, param5, param6, param7)
	-- Legacy call (param4 is KColor)
	if type(param4) == "userdata" then
		return self:DrawStringScaled(str, x, y, 1.0, 1.0, param4, param5 or 0, param6)
	-- New call
	else
		return Font_DrawString(self, str, x, y, param4, param5, param6, param7)
	end
end

-- Legacy
function META:DrawStringScaled(str, x, y, sx, sy, col, boxWidth, center)
	align = DrawStringAlignment.TOP_LEFT
	if boxWidth == nil then
		boxWidth = 0
	end
	if boxWidth ~= 0 then
		strWidth = self:GetStringWidth(str)
		if center == true then
			align = DrawStringAlignment.TOP_CENTER
			x = x + ( boxWidth * 0.5 )
		else
			align = DrawStringAlignment.TOP_RIGHT
			x = x + boxWidth
		end
	end
	settings = FontRenderSettings()
	settings:SetAlignment(align)
	return Font_DrawString(self, str, x, y, sx, sy, col, settings)
end

-- Legacy
function META:DrawStringUTF8(str, x, y, col, boxWidth, center)
	return self:DrawStringScaled(str, x, y, 1.0, 1.0, col, boxWidth or 0, center)
end

-- Legacy
function META:DrawStringScaledUTF8(str, x, y, sx, sy, col, boxWidth, center)
	return self:DrawStringScaled(str, x, y, sx, sy, col, boxWidth or 0, center)
end

EndClass()

---------------------------------------------------------
BeginClass(ItemPool)

-- CollectibleType ItemPool:GetCollectible(ItemPoolType PoolType, boolean Decrease = false, int Seed = Random(), CollectibleType DefaultItem = CollectibleType.COLLECTIBLE_NULL)
local ItemPool_GetCollectible = META0.GetCollectible
function META:GetCollectible(poolType, decrease, seed, defaultItem)
	return ItemPool_GetCollectible(self, poolType, seed or Random(), (decrease and 0) or 1, defaultItem or 0)
end

-- TrinketType ItemPool:GetTrinket(boolean DontAdvanceRNG = false)
local ItemPool_GetTrinket = META0.GetTrinket
function META:GetTrinket(noAdvance)
	return ItemPool_GetTrinket(self, noAdvance)
end

-- PillEffect ItemPool:GetPillEffect(PillColor PillColor, EntityPlayer Player = nil)
local ItemPool_GetPillEffect = META0.GetPillEffect
function META:GetPillEffect(pillColor, player)
	return ItemPool_GetPillEffect(self, pillColor, player)
end

EndClass()

---------------------------------------------------------
BeginClass(SFXManager)

-- void SFXManager:Play(SoundEffect ID, float Volume = 1, int FrameDelay = 2, boolean Loop = false, float Pitch = 1, float Pan = 0)
local SFXManager_Play = META0.Play
function META:Play(sound, volume, frameDelay, loop, pitch, pan)
	SFXManager_Play(self, sound, volume or 1, frameDelay or 2, loop, pitch or 1, pan or 0)
end

EndClass()

---------------------------------------------------------
BeginClass(HUD)

-- void HUD:FlashChargeBar(EntityPlayer Player, ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local HUD_FlashChargeBar = META0.FlashChargeBar
function META:FlashChargeBar(player, slot)
	HUD_FlashChargeBar(self, player, slot or 0)
end

-- void HUD:InvalidateActiveItem(EntityPlayer Player, ActiveSlot Slot = ActiveSlot.SLOT_PRIMARY)
local HUD_InvalidateActiveItem = META0.InvalidateActiveItem
function META:InvalidateActiveItem(player, slot)
	HUD_InvalidateActiveItem(self, player, slot or 0)
end

EndClass()

---------------------------------------------------------
BeginClass(TemporaryEffects)

-- void TemporaryEffects:AddCollectibleEffect(CollectibleType CollectibleType, boolean AddCostume = true, int Count = 1)
local TemporaryEffects_AddCollectibleEffect = META0.AddCollectibleEffect
function META:AddCollectibleEffect(id, addCostume, num)
	TemporaryEffects_AddCollectibleEffect(self, id, addCostume or addCostume == nil, num or 1)
end

-- void TemporaryEffects:AddTrinketEffect(TrinketType TrinketType, boolean AddCostume = true, int Count = 1)
local TemporaryEffects_AddTrinketEffect = META0.AddTrinketEffect
function META:AddTrinketEffect(id, addCostume, num)
	TemporaryEffects_AddTrinketEffect(self, id, addCostume or addCostume == nil, num or 1)
end

-- void TemporaryEffects:AddNullEffect(NullItemID NullId, boolean AddCostume = true, int Count = 1)
local TemporaryEffects_AddNullEffect = META0.AddNullEffect
function META:AddNullEffect(id, addCostume, num)
	TemporaryEffects_AddNullEffect(self, id, addCostume or addCostume == nil, num or 1)
end

-- void TemporaryEffects:RemoveCollectibleEffect(CollectibleType CollectibleType, int Count = 1)
-- * Count=-1 removes all instances of that effect
local TemporaryEffects_RemoveCollectibleEffect = META0.RemoveCollectibleEffect
function META:RemoveCollectibleEffect(id, num)
	TemporaryEffects_RemoveCollectibleEffect(self, id, num or 1)
end

-- void TemporaryEffects:RemoveTrinketEffect(TrinketType TrinketType, int Count = 1)
-- * Count=-1 removes all instances of that effect
local TemporaryEffects_RemoveTrinketEffect = META0.RemoveTrinketEffect
function META:RemoveTrinketEffect(id, num)
	TemporaryEffects_RemoveTrinketEffect(self, id, num or 1)
end

-- void TemporaryEffects:RemoveNullEffect(NullItemID NullId, int Count = 1)
-- * Count=-1 removes all instances of that effect
local TemporaryEffects_RemoveNullEffect = META0.RemoveNullEffect
function META:RemoveNullEffect(id, num)
	TemporaryEffects_RemoveNullEffect(self, id, num or 1)
end

EndClass()

---------------------------------------------------------
BeginClass(Room)

-- Vector Room:FindFreePickupSpawnPosition(Vector Pos, float InitialStep = 0, boolean AvoidActiveEntities = false, boolean AllowPits = false, boolean avoidDoors = false)
local Room_FindFreePickupSpawnPosition = META0.FindFreePickupSpawnPosition
function META:FindFreePickupSpawnPosition(pos, initStep, avoidActive, allowPits, avoidDoors)
	return Room_FindFreePickupSpawnPosition(self, pos, initStep or 0, avoidActive, allowPits, avoidDoors or 0)
end

-- boolean, Vector Room:CheckLine(Vector Pos1, Vector Pos2, LinecheckMode Mode, int GridPathThreshold = 0, boolean IgnoreWalls = false, boolean IgnoreCrushable = false)
-- * Returns
--      boolean: true if there are no obstructions between Pos1 and Pos2, false otherwise
--      Vector: first hit position from Pos1 to Pos2 (returns Pos2 if the line didn't hit anything)
--local Room_CheckLine = META0.CheckLine
--function META:CheckLine(pos1, pos2, mode, gridPathThreshold, ignoreWalls, ignoreCrushable)
--	local out = Vector(0, 0)
--	local ok = Room_CheckLine(self, pos1, pos2, mode, gridPathThreshold or 0, ignoreWalls, ignoreCrushable, out)
--	return ok, out
--end

-- boolean Room:TrySpawnBlueWombDoor(boolean FirstTime = true, boolean IgnoreTime = false, boolean Force = false)
local Room_TrySpawnBlueWombDoor = META0.TrySpawnBlueWombDoor
function META:TrySpawnBlueWombDoor(firstTime, ignoreTime, force)
	return Room_TrySpawnBlueWombDoor(self, firstTime or firstTime == nil, ignoreTime, force)
end

-- void Room:MamaMegaExplosion(Vector Position = Vector.Zero, EntityPlayer Player = nil)
local Room_MamaMegaExplosion = META0.MamaMegaExplosion
function META:MamaMegaExplosion(position, player)
	return Room_MamaMegaExplosion(self, position or Vector.Zero, player)
end

EndClass()

---------------------------------------------------------
BeginClass(MusicManager)

-- void	MusicManager:Play(Music ID, float Volume = 1)
local MusicManager_Play = META0.Play
function META:Play(id, volume)
	MusicManager_Play(self, id, volume or 1)
end

-- void	MusicManager:Fadein(Music ID, float Volume = 1, float FadeRate = 0.08)
local MusicManager_Fadein = META0.Fadein
function META:Fadein(id, volume, fadeRate)
	MusicManager_Fadein(self, id, volume or 1, fadeRate or 0.08)
end

-- void	MusicManager:Crossfade(Music ID, float FadeRate = 0.08)
local MusicManager_Crossfade = META0.Crossfade
function META:Crossfade(id, fadeRate)
	MusicManager_Crossfade(self, id, fadeRate or 0.08)
end

-- void	MusicManager:Fadeout(float FadeRate = 0.08)
local MusicManager_Fadeout = META0.Fadeout
function META:Fadeout(fadeRate)
	MusicManager_Fadeout(self, fadeRate or 0.08)
end

-- void	MusicManager:EnableLayer(int LayerId = 0, boolean Instant = false)
local MusicManager_EnableLayer = META0.EnableLayer
function META:EnableLayer(id, instant)
	MusicManager_EnableLayer(self, id or 0, instant)
end

-- void	MusicManager:DisableLayer(int LayerId = 0)
local MusicManager_DisableLayer = META0.DisableLayer
function META:DisableLayer(id)
	MusicManager_DisableLayer(self, id or 0)
end

-- boolean MusicManager:IsLayerEnabled(int LayerId = 0)
local MusicManager_IsLayerEnabled = META0.IsLayerEnabled
function META:IsLayerEnabled(id)
	return MusicManager_IsLayerEnabled(self, id or 0)
end

-- void	MusicManager:VolumeSlide(float TargetVolume, float FadeRate = 0.08)
local MusicManager_VolumeSlide = META0.VolumeSlide
function META:VolumeSlide(vol, fadeRate)
	return MusicManager_VolumeSlide(self, vol, fadeRate or 0.08)
end

EndClass()

---------------------------------------------------------
BeginClass(Game)

-- void Game:ChangeRoom(int RoomIndex, int Dimension = -1)
local Game_ChangeRoom = META0.ChangeRoom
function META:ChangeRoom(idx, dim)
	Game_ChangeRoom(self, idx, dim or -1)
end

-- void Game:Fart(Vector Position, float Radius = 85, Entity Source = nil, float FartScale = 1, int FartSubType = 0, Color FartColor = Color.Default)
local Game_Fart = META0.Fart
function META:Fart(pos, radius, source, scale, subType, color)
	Game_Fart(self, pos, radius or 85, source, scale or 1, subType or 0, color or Color.Default)
end

-- void Game:BombDamage(Vector Position, float Damage, float Radius, boolean LineCheck = true, Entity Source = nil, BitSet128 TearFlags = TearFlags.TEAR_NORMAL, int DamageFlags = DamageFlag.DAMAGE_EXPLOSION, boolean DamageSource = false)
local Game_BombDamage = META0.BombDamage
function META:BombDamage(pos, damage, radius, lineCheck, source, tearFlags, damageFlags, damageSource)
	Game_BombDamage(self, pos, damage, radius, lineCheck ~= false, source, tobitset128(tearFlags or TearFlags.TEAR_NORMAL), damageFlags or DamageFlag.DAMAGE_EXPLOSION, damageSource)
end

-- void	Game:BombExplosionEffects(Vector Position, float Damage, BitSet128 TearFlags = TearFlags.TEAR_NORMAL, Color Color = Color.Default, Entity Source = nil, float RadiusMult = 1, boolean LineCheck = true, boolean DamageSource = false, int DamageFlags = DamageFlag.DAMAGE_EXPLOSION)
local Game_BombExplosionEffects = META0.BombExplosionEffects
function META:BombExplosionEffects(pos, damage, tearFlags, color, source, radiusMult, lineCheck, damageSource, damageFlags)
	Game_BombExplosionEffects(self, pos, damage, tobitset128(tearFlags or TearFlags.TEAR_NORMAL), color or Color.Default, source, radiusMult or 1, lineCheck ~= false, damageFlags or DamageFlag.DAMAGE_EXPLOSION, damageSource)
end

-- void	Game::BombTearflagEffects(Vector Position, float Radius, int TearFlags, Entity Source = nil, float RadiusMult = 1)
local Game_BombTearflagEffects = META0.BombTearflagEffects
function META:BombTearflagEffects(pos, radius, tearFlags, source, radiusMult)
	Game_BombTearflagEffects(self, pos, radius, tearFlags, source, radiusMult or 1)
end

-- void Game:SpawnParticles(Vector Pos, EffectVariant ParticleType, int NumParticles, float Speed, Color Color = Color.Default, float Height = 100000, int SubType = 0)
local Game_SpawnParticles = META0.SpawnParticles
function META:SpawnParticles(pos, variant, num, speed, color, height, subType)
	Game_SpawnParticles(self, pos, variant, num, speed, color or Color.Default, height or 100000, subType or 0)
end

-- void Game:StartRoomTransition(int RoomIndex, Direction Direction, RoomTransitionAnim Animation = RoomTransitionAnim.WALK, EntityPlayer Player = nil, int Dimension = -1)
local Game_StartRoomTransition = META0.StartRoomTransition
function META:StartRoomTransition(roomIdx, dir, anim, player, dim)
	Game_StartRoomTransition(self, roomIdx, dir, anim or RoomTransitionAnim.WALK, player, dim or -1)
end

-- void	Game:UpdateStrangeAttractor(Vector Position, float Force = 10, float Radius = 250)
local Game_UpdateStrangeAttractor = META0.UpdateStrangeAttractor
function META:UpdateStrangeAttractor(pos, force, radius)
	Game_UpdateStrangeAttractor(self, pos, force or 10, radius or 250)
end

-- void Game:ShowHallucination(int FrameCount, BackdropType Backdrop = BackdropType.NUM_BACKDROPS)
local Game_ShowHallucination = META0.ShowHallucination
function META:ShowHallucination(frameCount, backdrop)
	Game_ShowHallucination(self, frameCount, backdrop or BackdropType.NUM_BACKDROPS)
end

-- void Game:Fadein(float Speed, bool ShowIcon = true, const Color & FadeColor = KAGE::Graphics::Colors.Black)
-- local Game_Fadein = META0.Fadein
-- function META:Fadein(speed, show_icon, color)
-- 	Game_Fadein( self, speed, show_icon or true, color or KColor(0,0,0,255) )
-- end

-- void Game:Fadeout(float Speed, eFadeoutTarget Target, const Color & FadeColor = KAGE::Graphics::Colors.Black)
-- local Game_Fadeout = META0.Fadeout
-- function META:Fadeout(speed, target, color)
-- 	Game_Fadeout( self, speed, target, color or KColor(0,0,0,255) )
-- end

EndClass()

---------------------------------------------------------
BeginClass(Level)

-- const RoomDescriptor Level:GetRoomByIdx(int RoomIdx, int Dimension = -1)
-- * Dimension: ID of the dimension to get the room from
--      -1: Current dimension
--      0: Main dimension
--      1: Secondary dimension, used by Downpour mirror dimension and Mines escape sequence
--      2: Death Certificate dimension
local Level_GetRoomByIdx = META0.GetRoomByIdx
function META:GetRoomByIdx(idx, dim)
	return Level_GetRoomByIdx(self, idx, dim or -1)
end

-- int Level:QueryRoomTypeIndex(RoomType RoomType, boolean Visited, RNG rng, boolean IgnoreGroup = false)
-- * IgnoreGroup: If set to true, includes rooms that do not have the same group ID as the current room (currently unused)
local Level_QueryRoomTypeIndex = META0.QueryRoomTypeIndex
function META:QueryRoomTypeIndex(roomType, visited, rng, ignoreGroup)
	return Level_QueryRoomTypeIndex(self, roomType, visited, rng, ignoreGroup)
end

-- void Level:ChangeRoom(int RoomIndex, int Dimension = -1)
local Level_ChangeRoom = META0.ChangeRoom
function META:ChangeRoom(idx, dim)
	Level_ChangeRoom(self, idx, dim or -1)
end

EndClass()

BeginClass(Sprite)

-- void Sprite:SetFrame(int Frame)
-- void Sprite:SetFrame(string Anim, int Frame)
local Sprite_SetFrame = META0.SetFrame
local Sprite_SetFrame_1 = META0.SetFrame_1
function META:SetFrame(a, b)
	if type(a) == "number" then
		Sprite_SetFrame_1(self, a)
	else
		Sprite_SetFrame(self, a, b)
	end
end

-- void Sprite:Render(Vector Pos, Vector TopLeftClamp = Vector.Zero, Vector BottomRightClamp = Vector.Zero)
local Sprite_Render = META0.Render
function META:Render(pos, tl, br)
	Sprite_Render(self, pos, tl or Vector.Zero, br or Vector.Zero)
end

-- void Sprite:RenderLayer(int LayerId, Vector Pos, Vector TopLeftClamp = Vector.Zero, Vector BottomRightClamp = Vector.Zero)
local Sprite_RenderLayer = META0.RenderLayer
function META:RenderLayer(layer, pos, tl, br)
	Sprite_RenderLayer(self, layer, pos, tl or Vector.Zero, br or Vector.Zero)
end

-- boolean Sprite:IsFinished(string Anim = "")
local Sprite_IsFinished = META0.IsFinished
function META:IsFinished(anim)
	return Sprite_IsFinished(self, anim or "")
end

-- boolean Sprite:IsPlaying(string Anim = "")
local Sprite_IsPlaying = META0.IsPlaying
function META:IsPlaying(anim)
	return Sprite_IsPlaying(self, anim or "")
end

-- boolean Sprite:IsOverlayFinished(string Anim = "")
local Sprite_IsOverlayFinished = META0.IsOverlayFinished
function META:IsOverlayFinished(anim)
	return Sprite_IsOverlayFinished(self, anim or "")
end

-- boolean Sprite:IsOverlayPlaying(string Anim = "")
local Sprite_IsOverlayPlaying = META0.IsOverlayPlaying
function META:IsOverlayPlaying(anim)
	return Sprite_IsOverlayPlaying(self, anim or "")
end

-- void Sprite:Load(string Path, boolean LoadGraphics = true)
local Sprite_Load = META0.Load
function META:Load(path, loadGraphics)
	Sprite_Load(self, path, loadGraphics ~= false)
end

-- void Sprite:PlayRandom(int Seed = Random())
local Sprite_PlayRandom = META0.PlayRandom
function META:PlayRandom(seed)
	Sprite_PlayRandom(self, seed or Random())
end

-- void Sprite:SetAnimation(string Anim, boolean Reset = true)
local Sprite_SetAnimation = META0.SetAnimation
function META:SetAnimation(anim, reset)
	Sprite_SetAnimation(self, anim, reset ~= false)
end

-- void Sprite:SetOverlayAnimation(string Anim, boolean Reset = true)
local Sprite_SetOverlayAnimation = META0.SetOverlayAnimation
function META:SetOverlayAnimation(anim, reset)
	Sprite_SetOverlayAnimation(self, anim, reset ~= false)
end

-- KColor Sprite:GetTexel(Vector SamplePos, Vector RenderPos, float AlphaThreshold = 0.01, int LayerId  = -1)
-- local Sprite_GetTexel = META0.GetTexel
-- function META:GetTexel(samplePos, renderPos, alphaThreshold, layerId)
-- 	return Sprite_GetTexel(self, samplePos, renderPos, alphaThreshold or 0.01, layerId or -1)
-- end

EndClass()

---------------------------------------------------------
BeginClass(EntityTear)

-- void EntityTear:AddTearFlags(BitSet128 Flags)
--	Adds the specified tear flags
function META:AddTearFlags(f)
	self.TearFlags = self.TearFlags | f
end

-- void EntityTear:ClearTearFlags(BitSet128 Flags)
--	Removes the specified tear flags
function META:ClearTearFlags(f)
	self.TearFlags = self.TearFlags & ~f
end

-- boolean EntityTear:HasTearFlags(BitSet128 Flags)
--	Returns true if we have any of the specified tear flags
function META:HasTearFlags(f)
	return self.TearFlags & f ~= TearFlags.TEAR_NORMAL
end

EndClass()

---------------------------------------------------------
BeginClass(EntityBomb)

-- void EntityBomb:AddTearFlags(BitSet128 Flags)
--	Adds the specified tear flags
function META:AddTearFlags(f)
	self.Flags = self.Flags | f
end

-- void EntityBomb:ClearTearFlags(BitSet128 Flags)
--	Removes the specified tear flags
function META:ClearTearFlags(f)
	self.Flags = self.Flags & ~f
end

-- boolean EntityBomb:HasTearFlags(BitSet128 Flags)
--	Returns true if we have any of the specified tear flags
function META:HasTearFlags(f)
	return self.Flags & f ~= TearFlags.TEAR_NORMAL
end

EndClass()

---------------------------------------------------------
BeginClass(EntityKnife)

-- void EntityKnife:AddTearFlags(BitSet128 Flags)
--	Adds the specified tear flags
function META:AddTearFlags(f)
	self.TearFlags = self.TearFlags | f
end

-- void EntityKnife:ClearTearFlags(BitSet128 Flags)
--	Removes the specified tear flags
function META:ClearTearFlags(f)
	self.TearFlags = self.TearFlags & ~f
end

-- boolean EntityKnife:HasTearFlags(BitSet128 Flags)
--	Returns true if we have any of the specified tear flags
function META:HasTearFlags(f)
	return self.TearFlags & f ~= TearFlags.TEAR_NORMAL
end

EndClass()

---------------------------------------------------------
BeginClass(EntityLaser)

-- void EntityLaser:AddTearFlags(BitSet128 Flags)
--	Adds the specified tear flags
function META:AddTearFlags(f)
	self.TearFlags = self.TearFlags | f
end

-- void EntityLaser:ClearTearFlags(BitSet128 Flags)
--	Removes the specified tear flags
function META:ClearTearFlags(f)
	self.TearFlags = self.TearFlags & ~f
end

-- boolean EntityLaser:HasTearFlags(BitSet128 Flags)
--	Returns true if we have any of the specified tear flags
function META:HasTearFlags(f)
	return self.TearFlags & f ~= TearFlags.TEAR_NORMAL
end

EndClass()

---------------------------------------------------------
BeginClass(EntityProjectile)

-- void EntityProjectile:ClearProjectileFlags(int Flags)
--	Removes the specified projectile flags
function META:ClearProjectileFlags(f)
	self.ProjectileFlags = self.ProjectileFlags & ~f
end

-- boolean EntityProjectile:HasProjectileFlags(int Flags)
--	Returns true if we have any of the specified projectile flags
function META:HasProjectileFlags(f)
	return self.ProjectileFlags & f ~= 0
end

EndClass()

---------------------------------------------------------
BeginClass(EntityFamiliar)

-- void	EntityFamiliar:PickEnemyTarget(float MaxDistance, int FrameInterval = 13, int Flags = 0, Vector ConeDir = Vector.Zero, float ConeAngle = 15)
-- * Flags: A combination of the following flags (none of these are set by default)
--       1: Allow switching to a better target even if we already have one
--       2: Don't prioritize enemies that are close to our owner
--       4: Prioritize enemies with higher HP
--       8: Prioritize enemies with lower HP
--       16: Give lower priority to our current target (this makes us more likely to switch between targets)
-- * ConeDir: If ~= Vector.Zero, searches for targets in a cone pointing in this direction
-- * ConeAngle: If ConeDir ~= Vector.Zero, sets the half angle of the search cone in degrees (45 results in a search angle of 90 degrees)
local Entity_Familiar_PickEnemyTarget = META0.PickEnemyTarget
function META:PickEnemyTarget(maxDist, frameInterval, flags, coneDir, coneAngle)
	Entity_Familiar_PickEnemyTarget(self, maxDist, frameInterval or 13, flags or 0, coneDir or Vector(0, 0), coneAngle or 15)
end

EndClass()

---------------------------------------------------------
BeginClass(EntityNPC)

-- void	EntityNPC:MakeChampion(int Seed, ChampionColor ChampionColorIdx = -1, boolean Init = false)
-- * ChampionColorIdx: The type of champion to turn this enemy into (-1 results in a random champion type)
-- * Init: Set to true when called while initializing the enemy, false otherwise
local Entity_NPC_MakeChampion = META0.MakeChampion
function META:MakeChampion(seed, championType, init)
	Entity_NPC_MakeChampion(self, seed, championType or -1, init)
end
		
EndClass()

---------------------------------------------------------
BeginClass(EntityPickup)

-- boolean EntityPickup:TryOpenChest(EntityPlayer Player = nil)
-- * Player: The player that opened this chest
local Entity_Pickup_TryOpenChest = META0.TryOpenChest
function META:TryOpenChest(player)
	return Entity_Pickup_TryOpenChest(self, player)
end

-- void	EntityPickup::Morph(EntityType Type, int Variant, int SubType, boolean KeepPrice = false, boolean KeepSeed = false, boolean IgnoreModifiers = false)
-- * KeepSeed: If set to true, keeps the initial RNG seed of the pickup instead of rerolling it
-- * IgnoreModifiers: If set to true, ignores item effects that might turn this pickup into something other than the specificed variant and subtype

EndClass()
 
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

-- EntityKnife EntityPlayer:FireKnife(Entity Parent, float RotationOffset = 0, boolean CantOverwrite = false, int SubType = 0, int Variant = 0)
local Entity_Player_FireKnife = META0.FireKnife
function META:FireKnife(parent, rotationOffset, cantOverwrite, subType, variant)
	return Entity_Player_FireKnife(self, parent, variant or 0, rotationOffset or 0, cantOverwrite, subType or 0)
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

-- void EntityPlayer:AnimatePickup(Sprite Sprite, boolean HideShadow = false, string AnimName = "Pickup")
local Entity_Player_AnimatePickup = META0.AnimatePickup
function META:AnimatePickup(sprite, hideShadow, anim)
	return Entity_Player_AnimatePickup(self, sprite, hideShadow, anim or "Pickup")
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

-- EntityFamiliar EntityPlayer:ThrowFriendlyDip(int Subtype, Vector Position, Vector Target = Vector.Zero)
local Entity_Player_ThrowFriendlyDip = META0.ThrowFriendlyDip
function META:ThrowFriendlyDip(subtype, pos, target)
	return Entity_Player_ThrowFriendlyDip(self, subtype, pos, target)
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

Game = Game_0
Game_0 = nil

if not _LUADEBUG then
	debug = nil
	arg = nil
	dofile = nil
	loadfile = nil
end
