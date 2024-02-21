ffi.cdef[[
	void L_EntityPickup_AppearFast(Entity_Pickup*);
	bool L_EntityPickup_CanReroll(Entity_Pickup*);
	int L_EntityPickup_GetCoinValue(Entity_Pickup*);
	bool L_EntityPickup_IsShopItem(Entity_Pickup*);
	void L_EntityPickup_Morph(Entity_Pickup*, int, int, int, bool, bool, bool);
	void L_EntityPickup_PlayDropSound(Entity_Pickup*);
	void L_EntityPickup_PlayPickupSound(Entity_Pickup*);
	bool L_EntityPickup_TryOpenChest(Entity_Pickup*, Entity_Player*);
	
	bool L_EntityPickup_GetAutoUpdatePrice(Entity_Pickup*);
	void L_EntityPickup_SetAutoUpdatePrice(Entity_Pickup*, bool);
	int L_EntityPickup_GetCharge(Entity_Pickup*);
	void L_EntityPickup_SetCharge(Entity_Pickup*, int);
	int L_EntityPickup_GetOptionsPickupIndex(Entity_Pickup*);
	void L_EntityPickup_SetOptionsPickupIndex(Entity_Pickup*, int);
	int L_EntityPickup_GetPrice(Entity_Pickup*);
	void L_EntityPickup_SetPrice(Entity_Pickup*, int);
	int L_EntityPickup_GetShopItemId(Entity_Pickup*);
	void L_EntityPickup_SetShopItemId(Entity_Pickup*, int);
	int L_EntityPickup_GetState(Entity_Pickup*);
	void L_EntityPickup_SetState(Entity_Pickup*, int);
	int L_EntityPickup_GetTimeout(Entity_Pickup*);
	void L_EntityPickup_SetTimeout(Entity_Pickup*, int);
	bool L_EntityPickup_GetTouched(Entity_Pickup*);
	void L_EntityPickup_SetTouched(Entity_Pickup*, bool);
	int L_EntityPickup_GetWait(Entity_Pickup*);
	void L_EntityPickup_SetWait(Entity_Pickup*, int);
]]

local repentogon = ffidll
local lffi = ffi

local EntityPickupFuncs = {}

function EntityPickupFuncs:AppearFast()
	L_EntityPickup_AppearFast(self)
end

function EntityPickupFuncs:CanReroll()
	return L_EntityPickup_CanReroll(self)
end

function EntityPickupFuncs:GetCoinValue()
	return L_EntityPickup_GetCoinValue(self)
end

function EntityPickupFuncs:IsShopItem()
	return L_EntityPickup_IsShopItem(self)
end

function EntityPickupFuncs:Morph(entType, variant, subtype, keepPrice, keepSeed, ignoreModifiers)
	ffichecks.checknumber(2, entType)
	ffichecks.checknumber(3, variant)
	ffichecks.checknumber(4, subtype)
	keepPrice = ffichecks.optboolean(keepPrice, false)
	keepSeed = ffichecks.optboolean(keepSeed, false)
	ignoreModifiers = ffichecks.optboolean(ignoreModifiers, false)
	
	L_EntityPickup_Morph(self, entType, variant, subtype, keepPrice, keepSeed, ignoreModifiers)
end

function EntityPickupFuncs:PlayDropSound()
	L_EntityPickup_PlayDropSound(self)
end

function EntityPickupFuncs:PlayPickupSound()
	L_EntityPickup_PlayPickupSound(self)
end

function EntityPickupFuncs:TryOpenChest(player)
	player = ffichecks.optcdata(player, "Entity_Player", nil)
	
	return L_EntityPickup_TryOpenCHest(self, player)
end

local getkeys = {
    AutoUpdatePrice = repentogon.L_EntityPickup_GetAutoUpdatePrice,
	Charge = repentogon.L_EntityPickup_GetCharge,
	OptionsPickupIndex = repentogon.L_EntityPickup_GetOptionsPickupIndex,
	Price = repentogon.L_EntityPickup_GetPrice,
	ShopItemId = repentogon.L_EntityPickup_GetShopItemId,
	State = repentogon.L_EntityPickup_GetState,
	Timeout = repentogon.L_EntityPickup_GetTimeout,
	Touched = repentogon.L_EntityPickup_GetTouched,
	Wait = repentogon.L_EntityPickup_GetWait,
}

local setkeys = {
    AutoUpdatePrice = repentogon.L_EntityPickup_SetAutoUpdatePrice,
	Charge = repentogon.L_EntityPickup_SetCharge,
	OptionsPickupIndex = repentogon.L_EntityPickup_SetOptionsPickupIndex,
	Price = repentogon.L_EntityPickup_SetPrice,
	ShopItemId = repentogon.L_EntityPickup_SetShopItemId,
	State = repentogon.L_EntityPickup_SetState,
	Timeout = repentogon.L_EntityPickup_SetTimeout,
	Touched = repentogon.L_EntityPickup_SetTouched,
	Wait = repentogon.L_EntityPickup_SetWait,
}

local EntityPickupMT
EntityPickupMT = lffi.metatype("Entity_Pickup", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif EntityPickupFuncs[key] ~= nil then
			return EntityPickupFuncs[key]
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
