#include <unordered_set>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EffectCreateLight)
{
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float scale = (float)luaL_optnumber(L, 2, (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
	int lifespan = (int)luaL_optinteger(L, 3, -1);
	int state = (int)luaL_optinteger(L, 4, 6);
	ColorMod color;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		color = *lua::GetLuabridgeUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}

	if (lifespan < 1) {
		lifespan = -1;
	}
	if (state < 1) {
		state = 6;
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 121, *pos, Vector(0, 0), nullptr, 0, Isaac::genrand_int32(), 0);
	if (!effect) {
		lua_pushnil(L);
	}
	else {
		effect->_state = state;
		effect->_timeout = lifespan;
		effect->_lifespan = lifespan;
		effect->SetColor(&color, -1, 255, false, false);
		effect->_sprite._scale *= scale;

		lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}

	return 1;
}

LUA_FUNCTION(Lua_EffectCreateLootPreview) {
	LootList* loot = lua::GetRawUserdata<LootList*>(L, 1, lua::metatables::LootListMT);
	Vector* position = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Entity_Pickup* owner = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 3, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	Entity_Effect* eff = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 4, lua::Metatables::ENTITY_EFFECT, "EntityEffect");
	lua::luabridge::UserdataPtr::push(L, Entity_Effect::CreateLootPreview(loot, position, owner, eff), lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

LUA_FUNCTION(Lua_GetGridEntityDesc) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");

	if (entity->_variant == 136) {
		lua::luabridge::UserdataPtr::push(L, (GridEntityDesc*)&entity->_varData, lua::GetMetatableKey(lua::Metatables::GRID_ENTITY_DESC));
	} else {
		lua_pushnil(L);
	}
	
	return 1;
}

static const std::unordered_set<int> tearflagEffectVariants = {
	54,   // PLAYER_CREEP_HOLYWATER_TRAIL (Aquarius)
	113,  // BRIMSTONE_BALL
	126,  // TECH_DOT
	167,  // CHAIN_LIGHTNING
};
static bool AllowTearflagAccess(Entity_Effect* effect) {
	return tearflagEffectVariants.count(effect->_variant);
}

LUA_FUNCTION(Lua_GetTearFlags) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");

	if (AllowTearflagAccess(entity)) {
		lua::luabridge::UserdataValue<BitSet128>::push(L, lua::GetMetatableKey(lua::Metatables::BITSET_128), entity->_varData);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_SetTearFlags) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");

	if (AllowTearflagAccess(entity)) {
		entity->_varData = *lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128");
	}

	return 0;
}

LUA_FUNCTION(Lua_AddTearFlags) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");

	if (AllowTearflagAccess(entity)) {
		entity->_varData.AddFlags(*lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128"));
	}

	return 0;
}

LUA_FUNCTION(Lua_ClearTearFlags) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");

	if (AllowTearflagAccess(entity)) {
		entity->_varData.RemoveFlags(*lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128"));
	}

	return 0;
}

LUA_FUNCTION(Lua_HasTearFlags) {
	auto* entity = lua::GetLuabridgeUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");
	lua_pushboolean(L, AllowTearflagAccess(entity) && entity->_varData.HasAny(*lua::GetLuabridgeUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128")));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetGridEntityDesc", Lua_GetGridEntityDesc },
		{ "GetTearFlags", Lua_GetTearFlags },
		{ "SetTearFlags", Lua_SetTearFlags },
		{ "AddTearFlags", Lua_AddTearFlags },
		{ "ClearTearFlags", Lua_ClearTearFlags },
		{ "HasTearFlags", Lua_HasTearFlags },
		// Previous function names
		{ "GetAquariusTearFlags", Lua_GetTearFlags },
		{ "SetAquariusTearFlags", Lua_SetTearFlags },
		{ "AddAquariusTearFlags", Lua_AddTearFlags },
		{ "ClearAquariusTearFlags", Lua_ClearTearFlags },
		{ "HasAquariusTearFlags", Lua_HasTearFlags },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_EFFECT, functions);

	lua::RegisterGlobalClassFunction(_state, "EntityEffect", "CreateLight", Lua_EffectCreateLight);
	lua::RegisterGlobalClassFunction(_state, "EntityEffect", "CreateLootPreview", Lua_EffectCreateLootPreview);
}
