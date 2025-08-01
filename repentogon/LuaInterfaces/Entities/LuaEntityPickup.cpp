#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PickupSetAlternatePedestal) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int pedestalType = (int)luaL_checkinteger(L, 2);

	pickup->SetAlternatePedestal(pedestalType);

	return 0;
}

LUA_FUNCTION(Lua_PickupTryRemoveCollectible) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->TryRemoveCollectible());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetForceBlind) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool setBlind = lua::luaL_checkboolean(L, 2);

	pickup->SetForceBlind(setBlind);

	return 0;
}

LUA_FUNCTION(Lua_PickupIsBlind) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool checkForcedBlindOnly = lua::luaL_optboolean(L, 2, true); // wish I implemented that method properly before...

	bool isBlind = false;
	if (pickup->_variant == 100) {
		if (checkForcedBlindOnly) {
			isBlind = pickup->IsBlind();
		}
		else if (pickup->IsBlind() || !pickup->_sprite._layerState[1]._spriteSheetPath.compare("gfx/Items/Collectibles/questionmark.png")) {
			isBlind = true;
		}
	}
	

	lua_pushboolean(L, isBlind);
	return 1;
}

LUA_FUNCTION(Lua_PickupGetVarData) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, *pickup->GetVarData());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetVarData) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	*pickup->GetVarData() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PickupSetNewOptionsPickupIndex) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, pickup->SetNewOptionsPickupIndex());

	return 1;
}

LUA_FUNCTION(Lua_PickupTryInitOptionCycle) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int numCycle = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, pickup->TryInitOptionCycle(numCycle));

	return 1;
}

LUA_FUNCTION(Lua_PickupGetDropDelay)
{
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, *pickup->GetDropDelay());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetDropDelay)
{
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	*pickup->GetDropDelay() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PickupCanReroll)
{
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->CanReroll());

	return 1;
}

LUA_FUNCTION(Lua_PickupGetRandomVelocity) {
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	RNG* rng = nullptr;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		lua::GetLuabridgeUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	}
	int velType = (int)luaL_optinteger(L, 3, 0);

	Vector velocity;
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *Entity_Pickup::GetRandomPickupVelocity(velocity, pos, rng, velType);

	return 1;
}

LUA_FUNCTION(Lua_PickupMakeShopItem) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int shopItemID = (int)luaL_checkinteger(L, 2);

	pickup->MakeShopItem(shopItemID);

	return 0;
}

// reimplementation with error checking
LUA_FUNCTION(Lua_PickupAddCycleCollectible) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int id = (int)luaL_checkinteger(L, 2);

	if (g_Manager->_itemConfig.GetCollectible(id) == nullptr) {
		std::string error("Invalid collectible ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}

	bool res = false;
	if (pickup->_cycleCollectibleCount < 8) {
		pickup->_cycleCollectibleList[pickup->_cycleCollectibleCount] = id;
		pickup->_cycleCollectibleCount += 1;
		res = true;
	}

	lua_pushboolean(L, res);
	return 1;
}

LUA_FUNCTION(Lua_PickupTryFlip) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->TryFlip(nullptr, 0));
	return 1;
}

LUA_FUNCTION(Lua_PickupGetPriceSprite) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua::luabridge::UserdataPtr::push(L, &pickup->_priceANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_PickupGetAlternatePedestal) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, pickup->GetAlternatePedestal());
	return 1;
}

LUA_FUNCTION(Lua_PickupGetCollectibleCycle) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");

	lua_newtable(L);

	for (unsigned int i = 0; i < pickup->_cycleCollectibleCount; i++) {
		lua_pushinteger(L, pickup->_cycleCollectibleList[i]);
		lua_rawseti(L, -2, i + 1);
	}

	return 1;
}

LUA_FUNCTION(Lua_PickupRemoveCollectibleCycle) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	for (int i = 0; i < 7; i++) {
		pickup->_cycleCollectibleList[i] = 0; 
	}
	pickup->_cycleCollectibleCount = 0;

	return 0;
}
LUA_FUNCTION(Lua_PickupGetPickupGhost) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	Entity_Effect* pickupGhost = pickup->_pickupGhost;
	lua::luabridge::UserdataPtr::push(L, pickupGhost, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	
	return 1;
}

LUA_FUNCTION(Lua_PickupUpdatePickupGhosts) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	pickup->UpdatePickupGhosts();
	return 0;
}

/*LUA_FUNCTION(Lua_PickupIsChest) {
	Entity_Pickup* pickup = lua::GetRawUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	unsigned int variant = (unsigned int)luaL_optinteger(L, 2, pickup->_variant);
	lua_pushboolean(L, pickup->IsChest(variant));
	return 1;
}
*/


LUA_FUNCTION(Lua_PickupTriggerTheresOptionsPickup) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	pickup->TriggerTheresOptionsPickup();
	return 0;
}

LUA_FUNCTION(Lua_PickupGetMegaChestLeftCollectible) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	Entity_Pickup* collectible = pickup->_megaChestCollectible[0];
	lua::luabridge::UserdataPtr::push(L, collectible, lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));

	return 1;
}

LUA_FUNCTION(Lua_PickupGetMegaChestRightCollectible) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	Entity_Pickup* collectible = pickup->_megaChestCollectible[1];
	lua::luabridge::UserdataPtr::push(L, collectible, lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));

	return 1;
}

LUA_FUNCTION(Lua_PickupGetMegaChestOtherCollectible) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	if (pickup->_megaChestCollectible[0] != nullptr) {
		lua::luabridge::UserdataPtr::push(L, pickup->_megaChestCollectible[0], lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));
		lua_pushboolean(L, false);
	}
	else if (pickup->_megaChestCollectible[1] != nullptr) {
		lua::luabridge::UserdataPtr::push(L, pickup->_megaChestCollectible[1], lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));
		lua_pushboolean(L, true);
	}
	else {
		lua_pushnil(L);
		lua_pushnil(L);
	}

	return 2;
}

LUA_FUNCTION(Lua_PickupGetFlippedCollectibleID) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");

	if (pickup->_variant == 100 && pickup->_flipSaveState.saveState != nullptr) {
		lua_pushinteger(L, pickup->_flipSaveState.saveState->subtype);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_PickupInitFlipState) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	const CollectibleType collectID = (CollectibleType)luaL_optinteger(L, 2, 0);
	bool setupCollectibleGraphics = lua::luaL_optboolean(L, 3, true);
	pickup->InitFlipState(collectID, setupCollectibleGraphics);
	return 0;
}

LUA_FUNCTION(Lua_PickupHasFlipData) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->_variant == 100 && pickup->_flipSaveState.saveState != nullptr);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "AddCollectibleCycle", Lua_PickupAddCycleCollectible },
		{ "CanReroll", Lua_PickupCanReroll },
		{ "IsBlind", Lua_PickupIsBlind },
		{ "SetAlternatePedestal", Lua_PickupSetAlternatePedestal },
		{ "TryRemoveCollectible", Lua_PickupTryRemoveCollectible },
		{ "SetForceBlind", Lua_PickupSetForceBlind },
		{ "GetVarData", Lua_PickupGetVarData },
		{ "SetVarData", Lua_PickupSetVarData },
		{ "SetNewOptionsPickupIndex", Lua_PickupSetNewOptionsPickupIndex },
		{ "TryInitOptionCycle", Lua_PickupTryInitOptionCycle },
		{ "GetDropDelay", Lua_PickupGetDropDelay },
		{ "SetDropDelay", Lua_PickupSetDropDelay },
		{ "GetRandomPickupVelocity", Lua_PickupGetRandomVelocity },
		{ "MakeShopItem", Lua_PickupMakeShopItem },
		// i REALLY want a TryReroll but even looking through UseActiveItem i can't tell how the new id is determined
		{ "TryFlip", Lua_PickupTryFlip },
		{ "GetPriceSprite", Lua_PickupGetPriceSprite },
		{ "GetAlternatePedestal", Lua_PickupGetAlternatePedestal },
		{ "GetCollectibleCycle", Lua_PickupGetCollectibleCycle },
		{ "RemoveCollectibleCycle", Lua_PickupRemoveCollectibleCycle },
		//{ "IsChest", Lua_PickupIsChest },
		{ "GetPickupGhost", Lua_PickupGetPickupGhost },
		{ "UpdatePickupGhosts", Lua_PickupUpdatePickupGhosts },
		{ "TriggerTheresOptionsPickup", Lua_PickupTriggerTheresOptionsPickup },
		{ "GetMegaChestLeftCollectible", Lua_PickupGetMegaChestLeftCollectible },
		{ "GetMegaChestRightCollectible", Lua_PickupGetMegaChestRightCollectible },
		{ "GetMegaChestOtherCollectible", Lua_PickupGetMegaChestOtherCollectible },
		{ "GetFlipCollectible", Lua_PickupGetFlippedCollectibleID },
		{ "InitFlipState", Lua_PickupInitFlipState },
		{ "HasFlipData", Lua_PickupHasFlipData },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PICKUP, functions);

	lua::RegisterGlobalClassFunction(_state, "EntityPickup", "GetRandomPickupVelocity", Lua_PickupGetRandomVelocity);
}