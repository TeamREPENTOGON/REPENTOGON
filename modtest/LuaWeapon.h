#pragma once

#include "IsaacRepentance.h"
#include <variant>

struct WeaponData {
	Weapon* weapon;
	// Arbitrarily default to EntityPlayer here. This thing is super unsafe and the API
	// should be tweaked to prevent crashes.
	std::variant<Entity_Player*, Entity_Familiar*> owner = (Entity_Player*)nullptr;
	int8_t slot = -1;

	static Weapon* GetWeaponFromLua(lua_State* L, int idx);
};