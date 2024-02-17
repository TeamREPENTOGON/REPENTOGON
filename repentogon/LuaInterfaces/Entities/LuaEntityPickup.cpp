#include "IsaacRepentance.h"
#include "LuaCore.h"

const int coinValues[8] = { 1, 1, 5, 10, 2, 1, 5, 1 };

extern "C" {
	void L_EntityPickup_AppearFast(Entity_Pickup* pickup) {
		pickup->AppearFast();
	}

	bool L_EntityPickup_CanReroll(Entity_Pickup* pickup) {
		return pickup->CanReroll();
	}

	int L_EntityPickup_GetCoinValue(Entity_Pickup* pickup) {
		if (pickup->_subtype > 7)
			return 1;
		else
			return coinValues[pickup->_subtype];
	}

	bool L_EntityPickup_IsShopItem(Entity_Pickup* pickup) {
		return pickup->_price != 0;
	}

	void L_EntityPickup_Morph(Entity_Pickup* pickup, int type, int variant, int subtype, bool keepPrice, bool keepSeed, bool ignoreModifiers) {
		pickup->Morph(type, variant, subtype, keepPrice, keepSeed, ignoreModifiers);
	}

	void L_EntityPickup_PlayDropSound(Entity_Pickup* pickup) {
		pickup->PlayDropSound();
	}

	void L_EntityPickup_PlayPickupSound(Entity_Pickup* pickup) {
		pickup->PlayPickupSound();
	}

	bool L_EntityPickup_TryOpenChest(Entity_Pickup* pickup, Entity_Player* player) {
		return pickup->TryOpenChest(player);
	}

	bool L_EntityPickup_GetAutoUpdatePrice(Entity_Pickup* pickup) {
		return pickup->_autoUpdatePrice;
	}

	void L_EntityPickup_SetAutoUpdatePrice(Entity_Pickup* pickup, bool value) {
		pickup->_autoUpdatePrice = value;
	}

	int L_EntityPickup_GetCharge(Entity_Pickup* pickup) {
		return pickup->_charge;
	}

	void L_EntityPickup_SetCharge(Entity_Pickup* pickup, int value) {
		pickup->_charge = value;
	}

	int L_EntityPickup_GetOptionsPickupIndex(Entity_Pickup* pickup) {
		return pickup->_optionsPickupIndex;
	}

	void L_EntityPickup_SetOptionsPickupIndex(Entity_Pickup* pickup, int value) {
		pickup->_optionsPickupIndex = value;
	}

	int L_EntityPickup_GetPrice(Entity_Pickup* pickup) {
		return pickup->_price;
	}

	void L_EntityPickup_SetPrice(Entity_Pickup* pickup, int value) {
		pickup->_price = value;
	}

	int L_EntityPickup_GetShopItemId(Entity_Pickup* pickup) {
		return pickup->_shopItemId;
	}

	void L_EntityPickup_SetShopItemId(Entity_Pickup* pickup, int value) {
		pickup->_shopItemId = value;
	}

	int L_EntityPickup_GetState(Entity_Pickup* pickup) {
		return pickup->_state;
	}

	void L_EntityPickup_SetState(Entity_Pickup* pickup, int value) {
		pickup->_state = value;
	}

	int L_EntityPickup_GetTimeout(Entity_Pickup* pickup) {
		return pickup->_state;
	}

	void L_EntityPickup_SetTimeout(Entity_Pickup* pickup, int value) {
		pickup->_timeout = value;
	}

	bool L_EntityPickup_GetTouched(Entity_Pickup* pickup) {
		return pickup->_touched;
	}

	void L_EntityPickup_SetTouched(Entity_Pickup* pickup, bool value) {
		pickup->_touched = value;
	}

	int L_EntityPickup_GetWait(Entity_Pickup* pickup) {
		return pickup->_wait;
	}

	void L_EntityPickup_SetWait(Entity_Pickup* pickup, int value) {
		pickup->_wait = value;
	}
}