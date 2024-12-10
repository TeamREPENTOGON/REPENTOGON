#include <lua.hpp>
#include <optional>
#include <cstring>

#include "IsaacRepentance.h"
#include "../ImGuiFeatures/LogViewer.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "LuaWeapon.h"
#include "LuaLevelGenerator.h"
#include "Log.h"
#include "../Patches/XMLData.h"
#include "Level.h"
#include "../LuaInit.h"
#include "../Patches/MainMenuBlock.h"

//Callback tracking for optimizations
std::bitset<500> CallbackState;  // For new REPENTOGON callbacks. I dont think we will add 500 callbacks but lets set it there for now
std::bitset<75> VanillaCallbackState;  // For vanilla callbacks & reimplementations of them.
HOOK_STATIC(Isaac,SetBuiltInCallbackState, (const int callbackid, bool enable)-> void, __cdecl){
	if (callbackid > 1000) {
		CallbackState.set(callbackid - 1000, enable);
	}
	else {
		VanillaCallbackState.set(callbackid, enable);
		super(callbackid, enable);
	}
}

//PRE/POST_ADD_COLLECTIBLE (1004/1005)
void ProcessPostAddCollectible(int type, int charge, bool firsttime, int slot, int vardata, Entity_Player* player) {
	const int callbackid = 1005;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(type)
			.push(type)
			.push(charge)
			.push(firsttime)
			.push(slot)
			.push(vardata)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
}

HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	const int callbackid = 1004; 
	if (CallbackState.test(callbackid - 1000)) {

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua::LuaCaller caller(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults res = caller.push(callbackid)
			.push(type)
			.push(type)
			.push(charge)
			.push(firsttime)
			.push(slot)
			.push(vardata)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!res) {
			if (lua_istable(L, -1)) { // 6 params 
				int* result = new int[5];
				for (int i = 1; i <= 5; i++) {
					lua_pushinteger(L, i);
					lua_gettable(L, -2);
					result[i - 1] = (int)lua_tointeger(L, -1); //I only need ints here, otherwise I'd need to check the type
					lua_pop(L, 1);
				}
				super(result[0], result[1], result[2], result[3], result[4]);
				ProcessPostAddCollectible(result[0], result[1], result[2], result[3], result[4], this);
				return;
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isinteger(L, -1))
			{
				super((int)lua_tointeger(L, -1), charge, firsttime, slot, vardata);
				ProcessPostAddCollectible((int)lua_tointeger(L, -1), charge, firsttime, slot, vardata, this);
				return;
			}
		}
	}

	super(type, charge, firsttime, slot, vardata);
	ProcessPostAddCollectible(type, charge, firsttime, slot, vardata, this);
}

// 1010 used to have GRID_ROCK_UPDATE, now free

//HUD_UPDATE (id: 1020)

HOOK_METHOD(HUD, Update, () -> void) {
	const int callbackid = 1020;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);
	}
	super();
}

//HUD_UPDATE callback end

//HUD_POST_UPDATE (id: 1021)

HOOK_METHOD(HUD, PostUpdate, () -> void) {
	const int callbackid = 1021;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);
	}
	super();
}

//HUD_POST_UPDATE callback end

//HUD_RENDER (1022) and POST_HUD_RENDER (1024)

HOOK_METHOD(HUD, Render, () -> void) {
	const int precallbackid = 1022;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(precallbackid).call(1);
	}

	super();

	const int postcallbackid = 1024;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(postcallbackid).call(1);
	}
}

//(POST_)HUD_RENDER callbacks end


//Character menu render Callback(id:1023
//TODO Broken in Rep+ as RenderButtonLayout was inlined.
/*HOOK_METHOD(MenuManager, RenderButtonLayout, () -> void) {
	super();
	const int callbackid = 1023;
	MainMenuInputBlock::_enabled = false;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);
	}
	MainMenuInputBlock::_enabled = true;
	if ((this->_state | (1 << 8)) == this->_state) {
		MainMenuInputBlock::_enabled = false;	//kill off if we are in the game transition
	};
}*/

//Character menu render Callback end

//SFX_PRE/POST_PLAY (id: 1030/1031)
void ProcessPostSFXPlay(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
{
	const int callbackid = 1031;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(ID)
			.push(ID)
			.push(Volume)
			.push(FrameDelay)
			.push(Loop)
			.push(Pitch)
			.push(Pan)
			.call(1);
	}
}

HOOK_METHOD(SFXManager, Play, (int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan) -> void) {
	const int callbackid = 1030;
	if (!CallbackState.test(callbackid - 1000)) {
		super(ID, Volume, FrameDelay, Loop, Pitch, Pan);
		ProcessPostSFXPlay(ID, Volume, FrameDelay, Loop, Pitch, Pan);
		return;
	}
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(ID)
			.push(ID)
			.push(Volume)
			.push(FrameDelay)
			.push(Loop)
			.push(Pitch)
			.push(Pan)
			.call(1);
	if (!result) {
		if (lua_istable(L, -1)) {
			if (lua_rawlen(L, -1) == 6) {
				ID = lua::callbacks::ToInteger(L, 1);
				Volume = (float) lua::callbacks::ToNumber(L, 2);
				FrameDelay = lua::callbacks::ToInteger(L, 3);
				Loop = lua::callbacks::ToBoolean(L, 4);
				Pitch = (float) lua::callbacks::ToNumber(L, 5);
				Pan = (float) lua::callbacks::ToNumber(L, 6);
				super(ID, Volume, FrameDelay, Loop, Pitch, Pan);
				ProcessPostSFXPlay(ID, Volume, FrameDelay, Loop, Pitch, Pan);
				return;
			}
		}
		else if (lua_isboolean(L, -1)) {
			if (!lua_toboolean(L, -1)) { // this particular if statement can't be combined with the one above it, or it fails to register false properly
				return;
			}
		}
	}
	super(ID, Volume, FrameDelay, Loop, Pitch, Pan);
	ProcessPostSFXPlay(ID, Volume, FrameDelay, Loop, Pitch, Pan);
}
//SFX_PRE/POST_PLAY callbacks end

// PRE/POST_X_COLLISION callbacks START
struct CollisionInputs {
	Entity* entity;
	Entity* collider;
	bool low;
	bool override_vanilla_pre_collision;

	lua::Metatables vanilla_metatable = lua::Metatables::ENTITY;
	const char* luabridge_metatable = nullptr;

	void SetMetatable(const lua::Metatables metatable) {
		vanilla_metatable = metatable;
	}
	void SetMetatable(const char* metatable) {
		luabridge_metatable = metatable;
	}
};

struct PreCollisionResult {
	bool skip_internal_code = false;
	std::optional<bool> override_result = std::nullopt;
};

// Runs either a PRE_X_COLLISION or POST_X_COLLISION callback (they push the same parameters).
lua::LuaResults RunCollisionCallback(const CollisionInputs& inputs, const int callbackid, lua_State* L) {
	int discriminator = *inputs.entity->GetVariant();
	if (inputs.vanilla_metatable == lua::Metatables::ENTITY_NPC) {
		discriminator = *inputs.entity->GetType();
	}
	else if (inputs.vanilla_metatable == lua::Metatables::ENTITY_KNIFE) {
		discriminator = *inputs.entity->GetSubType();
	}

	lua::LuaCaller& lua_caller = lua::LuaCaller(L).push(callbackid).push(discriminator);
	if (inputs.luabridge_metatable != nullptr) {
		lua_caller.pushLuabridge(inputs.entity, inputs.luabridge_metatable);
	}
	else {
		lua_caller.push(inputs.entity, inputs.vanilla_metatable);
	}
	return lua_caller.push(inputs.collider, lua::Metatables::ENTITY)
		.push(inputs.low)
		.call(1);
}

// New PRE_X_COLLISION code with table return functionality.
PreCollisionResult ProcessPreCollisionCallback(const CollisionInputs& inputs, const int callbackid) {
	PreCollisionResult result;

	if (inputs.override_vanilla_pre_collision ? VanillaCallbackState.test(callbackid) : CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults lua_result = RunCollisionCallback(inputs, callbackid, L);

		if (!lua_result) {
			if (lua_isboolean(L, -1)) {
				// Vanilla boolean returns always skip internal code.
				result.skip_internal_code = true;
				result.override_result = lua_toboolean(L, -1);
			}
			else if (lua_istable(L, -1)) {
				// New table return format.
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2) && lua_isboolean(L, -1)) {
						const std::string key = lua_tostring(L, -2);
						const bool value = lua_toboolean(L, -1);
						if (key == "SkipCollisionEffects") {
							result.skip_internal_code = value;
						}
						else if (key == "Collide") {
							result.override_result = !value;
						}
					}
					lua_pop(L, 1);
				}
			}
		}
	}

	return result;
}

// POST_X_COLLISION
void ProcessPostCollisionCallback(const CollisionInputs& inputs, const int callbackid) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		RunCollisionCallback(inputs, callbackid, L);
	}
}

// Handles both PRE_X_COLLISION and POST_X_COLLISION and decides whether or not to collide/execute the internal collision code.
bool HandleCollisionCallbacks(const CollisionInputs& inputs, const int precallbackid, const int postcallbackid, std::function<bool(Entity*, bool)> super_lambda) {
	const PreCollisionResult pre_collision_result = ProcessPreCollisionCallback(inputs, precallbackid);

	bool result = false;

	if (!pre_collision_result.skip_internal_code) {
		// Call the internal collision code.
		result = super_lambda(inputs.collider, inputs.low);
		// Only run POST_X_COLLISION if the internal collision code ran.
		ProcessPostCollisionCallback(inputs, postcallbackid);
	}
	if (pre_collision_result.override_result.has_value()) {
		result = *pre_collision_result.override_result;
	}
	return result;
}

// "super" is wrapped into a lambda so that I can share the logic for all the collision callbacks without shoving it all into the macro.
#define _COLLISION_SUPER_LAMBDA() [this](Entity* collider, bool low) { return super(collider, low); }
#define HOOK_COLLISION_CALLBACKS(_type, _metatable, _overridevanillaprecollision, _precallback, _postcallback) \
HOOK_METHOD(_type, HandleCollision, (Entity* collider, bool low) -> bool) { \
	CollisionInputs inputs = { this, collider, low, _overridevanillaprecollision }; \
	inputs.SetMetatable(_metatable); \
	return HandleCollisionCallbacks(inputs, _precallback, _postcallback, _COLLISION_SUPER_LAMBDA()); \
}

HOOK_COLLISION_CALLBACKS(Entity_Player, lua::Metatables::ENTITY_PLAYER, true, 33, 1231)
HOOK_COLLISION_CALLBACKS(Entity_Tear, lua::Metatables::ENTITY_TEAR, true, 42, 1233)
HOOK_COLLISION_CALLBACKS(Entity_Familiar, lua::Metatables::ENTITY_FAMILIAR, true, 26, 1235)
HOOK_COLLISION_CALLBACKS(Entity_Bomb, lua::Metatables::ENTITY_BOMB, true, 60, 1237)
HOOK_COLLISION_CALLBACKS(Entity_Pickup, lua::Metatables::ENTITY_PICKUP, true, 38, 1239)
HOOK_COLLISION_CALLBACKS(Entity_Knife, lua::Metatables::ENTITY_KNIFE, true, 53, 1243)
HOOK_COLLISION_CALLBACKS(Entity_Projectile, lua::Metatables::ENTITY_PROJECTILE, true, 46, 1245)
HOOK_COLLISION_CALLBACKS(Entity_NPC, lua::Metatables::ENTITY_NPC, true, 30, 1247)
HOOK_COLLISION_CALLBACKS(Entity_Slot, lua::metatables::EntitySlotMT, false, 1240, 1241)

// Nullify the vanilla pre-collision callbacks.
HOOK_METHOD(LuaEngine, PrePlayerCollide, (Entity_Player* player, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreTearCollision, (Entity_Tear* tear, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreFamiliarCollision, (Entity_Familiar* fam, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreBombCollision, (Entity_Bomb* bomb, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PrePickupCollision, (Entity_Pickup* pickup, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreKnifeCollision, (Entity_Knife* knife, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreProjectileCollision, (Entity_Projectile* proj, Entity* collider, bool low) -> int) { return 0; }
HOOK_METHOD(LuaEngine, PreNPCCollision, (Entity_NPC* npc, Entity* collider, bool low) -> int) { return 0; }
// PRE/POST_X_COLLISION callbacks END

// POST_LASER_COLLISION (1249) (PRE_LASER_COLLISION lives in ASMPatches land)
HOOK_METHOD(Entity_Laser, DoDamage, (Entity* entity, float damage) -> void) {
	super(entity, damage);

	const int callbackid = 1249;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_LASER)
			.push(entity, lua::Metatables::ENTITY)
			.call(1);
	}
}

// Returns true if the Update logic should be skipped.
bool RunPreUpdateCallback(Entity* entity, const lua::Metatables metatable, const int callbackid) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller& lua_caller = lua::LuaCaller(L).push(callbackid);
		if (metatable == lua::Metatables::ENTITY_KNIFE) {
			lua_caller.push(*entity->GetSubType());
		}
		else {
			lua_caller.push(*entity->GetVariant());
		}
		lua::LuaResults result = lua_caller.push(entity, metatable).call(1);

		if (!result && lua_isboolean(L, -1)) {
			return lua_toboolean(L, -1);
		}
	}
	return false;
}

// Still call Entity::Update() if we skip the subclass' update,
// since that aligns with how MC_PRE_NPC_UPDATE works.
#define HOOK_PRE_UPDATE_CALLBACK(_type, _metatable, _callbackid) \
HOOK_METHOD(_type, Update, () -> void) { \
	if (!RunPreUpdateCallback(this, _metatable, _callbackid)) { \
		super(); \
	} else { \
		((Entity*)this)->Original_Update(); \
	} \
}

// PRE_X_UPDATE (1160 ~ 1168) (PRE_SLOT_UPDATE is 1169)
HOOK_PRE_UPDATE_CALLBACK(Entity_Player, lua::Metatables::ENTITY_PLAYER, 1160)
HOOK_PRE_UPDATE_CALLBACK(Entity_Tear, lua::Metatables::ENTITY_TEAR, 1161)
HOOK_PRE_UPDATE_CALLBACK(Entity_Familiar, lua::Metatables::ENTITY_FAMILIAR, 1162)
HOOK_PRE_UPDATE_CALLBACK(Entity_Bomb, lua::Metatables::ENTITY_BOMB, 1163)
HOOK_PRE_UPDATE_CALLBACK(Entity_Pickup, lua::Metatables::ENTITY_PICKUP, 1164)
HOOK_PRE_UPDATE_CALLBACK(Entity_Knife, lua::Metatables::ENTITY_KNIFE, 1165)
HOOK_PRE_UPDATE_CALLBACK(Entity_Projectile, lua::Metatables::ENTITY_PROJECTILE, 1166)
HOOK_PRE_UPDATE_CALLBACK(Entity_Laser, lua::Metatables::ENTITY_LASER, 1167)
HOOK_PRE_UPDATE_CALLBACK(Entity_Effect, lua::Metatables::ENTITY_EFFECT, 1168)

// NPC_PICK_TARGET
HOOK_METHOD(Entity_NPC, GetPlayerTarget, () -> Entity*) {
	Entity* unmodifiedTarget = super();

	const int callbackid = 1222;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults lua_result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetType())
			.push(this, lua::Metatables::ENTITY_NPC)
			.push(unmodifiedTarget, lua::Metatables::ENTITY)
			.call(1);

		if (!lua_result && lua_isuserdata(L, -1)) {
			return lua::GetUserdata<Entity*>(L, -1, lua::Metatables::ENTITY, "Entity");
		}
	}

	return unmodifiedTarget;
}


// PRE_PLAYER_TAKE_DMG
// (Runs before holy mantle, etc)
HOOK_METHOD(Entity_Player, TakeDamage, (float damage, unsigned long long damageFlags, EntityRef* source, int damageCountdown) -> bool) {
	const int callbackid = 1008;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults lua_result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(damage)
			.push(damageFlags)
			.push(source, lua::Metatables::ENTITY_REF)
			.push(damageCountdown)
			.call(1);

		if (!lua_result && lua_isboolean(L, -1) && lua_toboolean(L, -1) == false) {
			return false;
		}
	}

	return super(damage, damageFlags, source, damageCountdown);
}

// PRE/POST_PLAYER_ADD_HEARTS
std::optional<int> PreAddHeartsCallbacks(Entity_Player* player,int hearts, int heartcallbackid, std::optional<bool> boolval) {
	const int callbackid = 1009;

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller caller = lua::LuaCaller(L).push(callbackid)
		.push(heartcallbackid)
		.push(player, lua::Metatables::ENTITY_PLAYER)
		.push(hearts)
		.push(heartcallbackid);

	if (boolval == std::nullopt) {
		caller = caller.pushnil();
	}
	else {
		caller = caller.push(boolval.value());
	}

	lua::LuaResults result=caller.call(1);

	if (!result) {
		if (lua_isinteger(L, -1)) {
			 return (int)lua_tointeger(L, -1);
		}
	}
	return std::nullopt;
}

void PostAddHeartsCallbacks(Entity_Player* player, int hearts, int heartcallbackid, std::optional<bool> boolval) {
	const int callbackid = 1010;

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller caller = lua::LuaCaller(L).push(callbackid)
		.push(heartcallbackid)
		.push(player, lua::Metatables::ENTITY_PLAYER)
		.push(hearts)
		.push(heartcallbackid);

	if (boolval == std::nullopt) {
		caller = caller.pushnil();
	}
	else {
		caller = caller.push(boolval.value());
	}

	lua::LuaResults result = caller.call(1);

	return;
}

HOOK_METHOD(Entity_Player, AddHearts, (int hearts, bool unk) -> void) {	//red hp
	if (!CallbackState.test(1009 - 1000)) {
		super(hearts, unk);
	}
	else{
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, hearts, 1<<0, std::nullopt);	//do not pass unk
		hearts = heartcount.value_or(hearts);
		super(hearts, unk);
	}

	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, hearts, 1 << 0, std::nullopt);
	}
}

HOOK_METHOD(Entity_Player, AddMaxHearts, (int amount, bool ignoreKeeper) -> void) {	//max hearts
	if (!CallbackState.test(1009 - 1000)) {
		super(amount, ignoreKeeper);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 1, ignoreKeeper);
		amount = heartcount.value_or(amount);
		super(amount, ignoreKeeper);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 1, ignoreKeeper);
	}
}

HOOK_METHOD(Entity_Player, AddSoulHearts, (int amount) -> Entity_Player*) {	//soul hp
	if (!CallbackState.test(1009 - 1000)) {
		super(amount);
	}

	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1<<2 ,std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}

	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 2, std::nullopt);
	}
	return this;
}


HOOK_METHOD(Entity_Player, AddBlackHearts, (int amount) -> void) {	//black
	if (!CallbackState.test(1009 - 1000)) {
		super(amount);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1<<3, std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 3, std::nullopt);
	}
}

HOOK_METHOD(Entity_Player, AddEternalHearts, (int amount) -> void) {	//eternal
	if (!CallbackState.test(1009 - 1000) && !CallbackState.test(1010 - 1000)) {
		super(amount);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 4, std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 4, std::nullopt);
	}
}

HOOK_METHOD(Entity_Player, AddGoldenHearts, (int amount) -> void) {	//golden
	if (!CallbackState.test(1009 - 1000)) {
		super(amount);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 5, std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 5, std::nullopt);
	}
}

HOOK_METHOD(Entity_Player, AddBoneHearts, (int amount) -> Entity_Player*) {	//bone
	if (!CallbackState.test(1009 - 1000)) {
		super(amount);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 6, std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 6, std::nullopt);
	}
	return this;
}

HOOK_METHOD(Entity_Player, AddRottenHearts, (int amount, bool unk) -> void) {	//rotten
	if (!CallbackState.test(1009 - 1000)) {
		super(amount, unk);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 7, std::nullopt);	//do not pass unk
		amount = heartcount.value_or(amount);
		super(amount, unk);
	}

	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 7, std::nullopt);
	}
}

HOOK_METHOD(Entity_Player, AddBrokenHearts, (int amount) -> void) {	//broken
	if (!CallbackState.test(1009 - 1000)) {
		super(amount);
	}
	else {
		std::optional<int> heartcount = PreAddHeartsCallbacks(this, amount, 1 << 8, std::nullopt);
		amount = heartcount.value_or(amount);
		super(amount);
	}
	if (CallbackState.test(1010 - 1000)) {
		PostAddHeartsCallbacks(this, amount, 1 << 8, std::nullopt);
	}
}


//PRE/POST_ENTITY_THROW (1040/1041)
void ProcessPostEntityThrow(Vector* Velocity, Entity_Player* player, Entity* ent) {
	const int callbackid = 1041;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			//.push(ent->GetType())
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(ent, lua::Metatables::ENTITY)
			.pushUserdataValue(*Velocity, lua::Metatables::VECTOR)
			.call(1);
	}
}

HOOK_METHOD(Entity_Player, ThrowHeldEntity, (Vector* Velocity) -> Entity*) {
	if (!CallbackState.test(1040 - 1000) && !CallbackState.test(1041 - 1000)) { 
		return super(Velocity);
	}
	lua_State* L = g_LuaEngine->_state;
	Entity** heldEntity = this->GetHeldEntity();
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults results = lua::LuaCaller(L).push(1040)
		//.push((*heldEntity)->GetType())
		.pushnil()
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.push(*heldEntity, lua::Metatables::ENTITY)
		.pushUserdataValue(*Velocity, lua::Metatables::VECTOR)
		.call(1);

	if (!results) {
		if (lua_isuserdata(L, -1)) {
			Velocity = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			Entity* res = super(Velocity);
			ProcessPostEntityThrow(Velocity, this, res);
			return res;
		}
		else {
			Entity* res = super(Velocity);
			ProcessPostEntityThrow(Velocity, this, res);
			return res;
		}
	}
	else
	{
		return super(Velocity);
	}
}
//PRE/POST_ENTITY_THROW end

//PLAYER_INIT_POST_LEVEL_INIT_STATS (1042)
HOOK_METHOD(Entity_Player, InitPostLevelInitStats, () -> void) {
	const int callbackid = 1042;
	super();
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		Entity_Player* ent = (Entity_Player*)this;

		lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(ent, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
}

//PRE_ROOM_EXIT (1043) (currently using Entity_Player::TriggerRoomExit as the base)
HOOK_METHOD(Entity_Player, TriggerRoomExit, (bool unk) -> void) {
	const int callbackid = 1043;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		Entity_Player* ent = (Entity_Player*)this;

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(ent, lua::Metatables::ENTITY_PLAYER)
			.push(unk)
			.call(1);
	}
	super(unk);
}

//PRE_MUSIC_PLAY Callback (id: 1034 enum pending)
HOOK_METHOD(Music, Play, (int musicid, float volume) -> void) {
	const int callbackid = 1034;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(musicid)
			.push(musicid)
			.push(volume)
			.push(false)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				int tablesize = (int)lua_rawlen(L, -1);
				if (tablesize == 2) {
					super(lua::callbacks::ToInteger(L, 1), (float) lua::callbacks::ToNumber(L, 2));
					return;
				}
			}
			else if (lua_isinteger(L, -1)) {
				super((int)lua_tointeger(L, -1), volume);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(musicid, volume);
}

HOOK_METHOD(Music, Crossfade, (int musicid, float faderate) -> void) {
	const int callbackid = 1034;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(musicid)
			.push(musicid)
			.push(faderate)
			.push(true)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				int tablesize = (int)lua_rawlen(L, -1);
				if (tablesize == 2) {
					super(lua::callbacks::ToInteger(L, 1), (float) lua::callbacks::ToNumber(L, 2));
					return;
				}
			}
			else if (lua_isinteger(L, -1)) {
				super((int)lua_tointeger(L, -1), faderate);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(musicid, faderate);
}
//PRE_MUSIC_PLAY Callback (id: 1034 enum pending)

//PRE_MUSIC_LAYER_TOGGLE (1035)
HOOK_METHOD(Music, EnableLayer, (int id, bool instant) -> void) {
	const int callbackid = 1035;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(id)
			.push(id)
			.push(true)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				super((int)lua_tointeger(L, -1), instant);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return; // False, meaning "layer should stay disabled"
				}
			}
		}
	}
	super(id, instant);
}

HOOK_METHOD(Music, DisableLayer, (int id) -> void) {
	const int callbackid = 1035;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(id)
			.push(id)
			.push(false)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				super((int)lua_tointeger(L, -1));
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return; // True, meaning "layer should stay enabled"
				}
			}
		}
	}
	super(id);
}

//PRE_LEVEL_INIT Callback (id: 1060 enum pending)
HOOK_METHOD(Level, Init, (bool unk) -> void) {
	levelASM.ForceSpecialQuest = 0;

	const int callbackid = 1060;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.call(1);
	}
	super(unk);
}
//PRE_LEVEL_INIT Callback (id: 1060 enum pending)

//PRE/POST_RESTOCK_SHOP (id: 1070/1071)
bool ProcessPreRestockCallback(bool Partial) {
	const int callbackid = 1070;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;

		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(Partial)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return false;
				}
			}
		}
	}
	return true;
}

void ProcessPostRestockCallback(bool Partial) {
	const int callbackid = 1071;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;

		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(Partial)
			.call(1);
	}
}

HOOK_METHOD(Room, ShopRestockFull, () -> void) {
	if (ProcessPreRestockCallback(false)) {
		super();
		ProcessPostRestockCallback(false);
	}
}

HOOK_METHOD(Room, ShopRestockPartial, () -> void) {
	if (ProcessPreRestockCallback(true)) {
		super();
		ProcessPostRestockCallback(true);
	}
}

//PRE_LEVEL_CHANGE_ROOM (id :1061)
HOOK_METHOD(Level, ChangeRoom, (int roomId, int dimension) -> void) {
	const int callbackid = 1061;
	if (!CallbackState.test(callbackid - 1000)) { return super(roomId, dimension); }
	lua_State* L = g_LuaEngine->_state;

	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
		.pushnil()
		.push(roomId)
		.push(dimension)
		.call(1);

	if (!result) {
		if (lua_istable(L, -1)) {
			if (lua_rawlen(L, -1) == 2) {
				super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToInteger(L, 2));
			}
		}
		else {
			super(roomId, dimension);
		}
	}
}


//Morph Callbacks (id:1215-1212)
void PostNPCMorph(Entity_NPC* npc, int EntityType, int Variant, int SubType) {
	const int callbackid = 1214;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(npc, lua::Metatables::ENTITY_NPC)
			.push(EntityType)
			.push(Variant)
			.push(SubType)
			.call(1);
	}
}

HOOK_METHOD(Entity_NPC, Morph, (int EntityType, int Variant, int SubType, int Championid) -> void) {
	const int callbackid = 1212;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this, lua::Metatables::ENTITY_NPC)
			.push(EntityType)
			.push(Variant)
			.push(SubType)
			.push(Championid)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				int tablesize = (int)lua_rawlen(L, -1);
				if (tablesize == 4) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToInteger(L, 2), lua::callbacks::ToInteger(L, 3), lua::callbacks::ToInteger(L, 4));
					PostNPCMorph(this, pretype, prevar, presub);
					return;
				}
				else if (tablesize == 3) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToInteger(L, 2), lua::callbacks::ToInteger(L, 3), Championid);
					PostNPCMorph(this, pretype, prevar, presub);
					return;
				}
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	int pretype = *(this->GetType());
	int prevar = *(this->GetVariant());
	int presub = *(this->GetSubType());
	super(EntityType, Variant, SubType, Championid);
	PostNPCMorph(this, pretype, prevar, presub);
}


void PostPickupMorph(Entity_Pickup* itm, int EntityType, int Variant, int SubType, bool KeepPrice, bool KeepSeed, bool IgnoreModifiers) {
	const int callbackid = 1215;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(itm, lua::Metatables::ENTITY_PICKUP)
			.push(EntityType)
			.push(Variant)
			.push(SubType)
			.push(KeepPrice)
			.push(KeepSeed)
			.push(IgnoreModifiers)
			.call(1);
	}
}


HOOK_METHOD(Entity_Pickup, Morph, (int EntityType, int Variant, int SubType, bool KeepPrice, bool KeepSeed, bool IgnoreModifiers) -> void) {
	const int callbackid = 1213;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this, lua::Metatables::ENTITY_PICKUP)
			.push(EntityType)
			.push(Variant)
			.push(SubType)
			.push(KeepPrice)
			.push(KeepSeed)
			.push(IgnoreModifiers)
			.call(1);


		if (!result) {
			if (lua_istable(L, -1)) {
				int tablesize = (int)lua_rawlen(L, -1);
				if (tablesize == 6) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToInteger(L, 2), lua::callbacks::ToInteger(L, 3), lua::callbacks::ToBoolean(L, 4), lua::callbacks::ToBoolean(L, 5), lua::callbacks::ToBoolean(L, 6));
					PostPickupMorph(this, pretype, prevar, presub, KeepPrice, KeepSeed, IgnoreModifiers);
					return;
				}
				else if (tablesize == 3) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToInteger(L, 2), lua::callbacks::ToInteger(L, 3), KeepPrice, KeepSeed, IgnoreModifiers);
					PostPickupMorph(this, pretype, prevar, presub, KeepPrice, KeepSeed, IgnoreModifiers);
					return;
				}
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	int pretype = *(this->GetType());
	int prevar = *(this->GetVariant());
	int presub = *(this->GetSubType());
	super(EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers);
	PostPickupMorph(this, pretype, prevar, presub, KeepPrice, KeepSeed, IgnoreModifiers);
}
//end of Pre_Morph callvacks



//PRE/POST_COMPLETION_MARKS_RENDER (id: 1216/1217)
void PostMarksRender(CompletionWidget* cmp, Vector* pos, Vector* scale) {
	const int callbackid = 1217;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(cmp->GetANM2(), lua::Metatables::SPRITE)
			.push(pos, lua::Metatables::VECTOR)
			.push(scale, lua::Metatables::VECTOR)
			.push(cmp->CharacterId)
			.call(1);
	}

}
HOOK_METHOD(CompletionWidget, Render, (Vector* pos, Vector* scale) -> void) {
	const int callbackid = 1216;
	if (!(CallbackState.test(callbackid - 1000)|| CallbackState.test((callbackid + 1) - 1000))) { super(pos, scale); return; }
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
		.pushnil()
		.push(this->GetANM2(), lua::Metatables::SPRITE)
		.push(pos, lua::Metatables::VECTOR)
		.push(scale, lua::Metatables::VECTOR)
		.push(this->CharacterId)
		.call(1);

	if (!result) {
		if (lua_isboolean(L, -1)) {
			if (lua_toboolean(L, -1)) {
				return;
			}
		}
		else {
			super(pos, scale);
			PostMarksRender(this, pos, scale);
		}
	}
}

//PRE/POST_PAUSE_SCREEN_RENDER (id: 1218/1219)
void PostPauseScreenRender(PauseScreen* paws) {
	const int callbackid = 1219;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(&paws->mainsprite, lua::Metatables::SPRITE)
			.push(&paws->statssprite, lua::Metatables::SPRITE)
			.call(1);
	}


}
HOOK_METHOD(PauseScreen, Render, () -> void) {
	const int callbackid = 1218;
	if (!(CallbackState.test(callbackid - 1000) || CallbackState.test((callbackid + 1) - 1000))) { super(); return; }
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
		.pushnil()
		.push(&this->mainsprite, lua::Metatables::SPRITE)
		.push(&this->statssprite, lua::Metatables::SPRITE)
		.call(1);

	if (!result) {
		if (lua_isboolean(L, -1)) {
			if (lua_toboolean(L, -1)) {
				return;
			}
		}
		else {
			super();
			PostPauseScreenRender(this);
		}
	}
}


//POST_PICKUP_SHOP_PURCHASE (id: 1062)
void ProcessPostPickupShopPurchase(Entity_Pickup* pickup, Entity_Player* player, int moneySpent)
{
	const int callbackid = 1062;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*pickup->GetVariant())
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(moneySpent)
			.call(1);
	}
}

HOOK_METHOD(Entity_Pickup, TriggerShopPurchase, (Entity_Player* player, int moneySpent) -> void) {

	Entity_Pickup* pickup = (Entity_Pickup*)this;
	super(player, moneySpent);
	ProcessPostPickupShopPurchase(pickup, player, moneySpent);
}


//GET_FOLLOWER_PRIORITY (id: 1063)
HOOK_METHOD(Entity_Familiar, GetFollowerPriority, () -> int) {
	const int callbackid = 1063;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		Entity_Familiar* fam = (Entity_Familiar*)this;


		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*fam->GetVariant())
			.push(fam, lua::Metatables::ENTITY_FAMILIAR)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}

		}
	}
	return super();
}

//PRE_USE_CARD (id: 1064)
HOOK_METHOD(Entity_Player, UseCard, (int cardType, unsigned int useFlag) -> void) {
	const int callbackid = 1064;
	if (!CallbackState.test(callbackid - 1000)) { return super(cardType, useFlag); }
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	Entity_Player* plr = (Entity_Player*)this;
	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
		.push(cardType)
		.push(cardType)
		.push(plr, lua::Metatables::ENTITY_PLAYER)
		.push(useFlag)
		.call(1);

	if (!result) {
		if (lua_isboolean(L, -1)) {
			if (lua_toboolean(L, -1)) {
				return;
			}
		}
		else {
			super(cardType, useFlag);
		}
	}
}


//(PRE_)USE_PILL (id: 1065/10)
HOOK_METHOD(Entity_Player, UsePill, (int pillEffect, int pillColor, unsigned int useFlag) -> void) {
	// MC_PRE_USE_PILL
	const int callbackid1 = 1065;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(pillEffect)
			.push(pillEffect)
			.push(pillColor)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(useFlag)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(pillEffect, pillColor, useFlag);

	// MC_USE_PILL re-implementation
	const int callbackid2 = 10;
	if (VanillaCallbackState.test(10)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(pillEffect)
			.push(pillEffect)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(useFlag)
			.push(pillColor)
			.call(1);
	}
}

// Kill the original MC_USE_PILL callback.
HOOK_METHOD(LuaEngine, UsePill, (int pillEffect, Entity_Player* player, unsigned int useFlags) -> void) {}

//GET_SHOP_ITEM_PRICE (id: 1066)
HOOK_METHOD(Room, GetShopItemPrice, (unsigned int entVariant, unsigned int entSubtype, int shopItemID) -> int) {
	int price = super(entVariant, entSubtype, shopItemID);
	const int callbackid = 1066;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(entVariant)
			.push(entVariant)
			.push(entSubtype)
			.push(shopItemID)
			.push(price)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				price = (int)lua_tointeger(L, -1);
			}
		}
	}
	return price;
}

//PLAYER_GET_HEALTH_TYPE (id: 1067)
HOOK_METHOD(Entity_Player, GetHealthType, () -> int) {
	const int callbackid = 1067;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return super();
}

//PRE_FAMILIAR_RENDER (id: 1080)
HOOK_METHOD(Entity_Familiar, Render, (Vector* offset) -> void) {
	const int callbackid = 1080;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_FAMILIAR)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_NPC_RENDER (id: 1081)
HOOK_METHOD(Entity_NPC, Render, (Vector* offset) -> void) {
	const int callbackid = 1081;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetType())
			.push(this, lua::Metatables::ENTITY_NPC)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR) 
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_PLAYER_RENDER (id: 1082)
HOOK_METHOD(Entity_Player, Render, (Vector* offset) -> void) {
	const int callbackid = 1082;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_PICKUP_RENDER (id: 1083)
HOOK_METHOD(Entity_Pickup, Render, (Vector* offset) -> void) {
	const int callbackid = 1083;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PICKUP)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_TEAR_RENDER (id: 1084)
HOOK_METHOD(Entity_Tear, Render, (Vector* offset) -> void) {
	const int callbackid = 1084;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_TEAR)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_PROJECTILE_RENDER (id: 1085)
HOOK_METHOD(Entity_Projectile, Render, (Vector* offset) -> void) {
	const int callbackid = 1085;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PROJECTILE)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_KNIFE_RENDER (id: 1086)
HOOK_METHOD(Entity_Knife, Render, (Vector* offset) -> void) {
	const int callbackid = 1086;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_KNIFE)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_EFFECT_RENDER (id: 1087)
HOOK_METHOD(Entity_Effect, Render, (Vector* offset) -> void) {
	const int callbackid = 1087;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_EFFECT)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE_BOMB_RENDER (id: 1088)
HOOK_METHOD(Entity_Bomb, Render, (Vector* offset) -> void) {
	const int callbackid = 1088;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_BOMB)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
}

//PRE/POST_SLOT_RENDER (id: 1089/1090)
HOOK_METHOD(Entity_Slot, Render, (Vector* offset) -> void) {
	const int callbackid1 = 1089;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(offset);
	const int callbackid2 = 1090;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);
	}
}

int ProtectedCallbackIntAssign(lua_State* L, int var, int pos) {
	if (lua::callbacks::CheckInteger(L, pos))
		var = lua::callbacks::ToInteger(L, pos);
	return var;
}

bool noInfLoop = false;
//PRE_GRID_ENTITY_SPAWN (id: 1100)
HOOK_METHOD(Room, SpawnGridEntity, (int idx, unsigned int type, unsigned int variant, unsigned int seed, int vardata) -> bool) {
	const int callbackid = 1100;
	if (!noInfLoop && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(type)
			.push(type)
			.push(variant)
			.push(vardata)
			.push(idx)
			.push(seed)
			.pushnil() // no GridEntityDesc
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				type = (GridEntityType)ProtectedCallbackIntAssign(L, type, 1);
				variant = ProtectedCallbackIntAssign(L, variant, 2);
				vardata = ProtectedCallbackIntAssign(L, vardata, 3);
				seed = ProtectedCallbackIntAssign(L, seed, 4);
			}
			else if (lua_isuserdata(L, -1)) {
				GridEntityDesc* desc = lua::GetUserdata<GridEntityDesc*>(L, -1, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
				noInfLoop = true;
				return g_Game->_room->SpawnGridEntityDesc(idx, desc);
			}
			else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
			{
				return false;
			}
		}
	}
	noInfLoop = false;
	return super(idx, type, variant, seed, vardata);
}

//also PRE_GRID_ENTITY_SPAWN (id: 1100), but for the override
HOOK_METHOD(Room, SpawnGridEntityDesc, (int idx, GridEntityDesc* desc) -> bool) {
	const int callbackid = 1100;
	if (!noInfLoop && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(desc->_type)
			.push(desc->_type)
			.push(desc->_variant)
			.push(desc->_varData)
			.push(idx)
			.push(desc->_spawnSeed)
			.push(desc, lua::Metatables::GRID_ENTITY_DESC) // yes GridEntityDesc
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				desc = lua::GetUserdata<GridEntityDesc*>(L, -1, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
			}
			else if (lua_istable(L, -1)) {
				int type = (GridEntityType)ProtectedCallbackIntAssign(L, desc->_type, 1);
				int variant = ProtectedCallbackIntAssign(L, desc->_variant, 2);
				int varData = ProtectedCallbackIntAssign(L, desc->_varData, 3);
				int spawnSeed = ProtectedCallbackIntAssign(L, desc->_spawnSeed, 4);

				noInfLoop = true;
				return g_Game->_room->SpawnGridEntity(idx, type, variant, spawnSeed, varData);
			}
			else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
			{
				return false;
			}
		}
	}
	noInfLoop = false;
	return super(idx, desc);
}

//POST_GRID_ENTITY_SPAWN (id: 1101)
//there's another call in ASMPatches
HOOK_METHOD(GridEntity, Init, (unsigned int Seed) -> void) {
	super(Seed);

	const int callbackid = 1101;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
			.push(this->GetDesc()->_type)
			.push(this, lua::Metatables::GRID_ENTITY)
			.call(1);
	}
};

void postrenderbodyhead(const int callbackid,Entity_Player* playa, Vector* x) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(playa->GetPlayerType())
			.push(playa, lua::Metatables::ENTITY_PLAYER)
			.pushUserdataValue(*x, lua::Metatables::VECTOR)
			.call(1);
	}
}

//RenderHead Callback (id: 1038/1045)
HOOK_METHOD(Entity_Player, RenderHead, (Vector* x) -> void) {
	const int callbackid = 1038;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.pushUserdataValue(*x, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				Vector* p = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
				super(p);
				postrenderbodyhead(1045, this, p);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(x);
	postrenderbodyhead(1045, this, x);
}

//PRE_POST Renderbody Callback (id: 1039/1046)
HOOK_METHOD(Entity_Player, RenderBody, (Vector* x) -> void) {
	const int callbackid = 1039;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.pushUserdataValue(*x, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				Vector* p = lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
				super(p);
				postrenderbodyhead(1046, this, p);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(x);
	postrenderbodyhead(1046, this, x);
}

//PRE_ROOM_TRIGGER_CLEAR (id: 1068)
HOOK_METHOD(Room, TriggerClear, (bool playSound) -> void) {
	const int callbackid = 1068;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(playSound)
			.call(1);
	}
	super(playSound);
}

//PRE_PLAYER_TRIGGER_ROOM_CLEAR (id: 1069)
HOOK_METHOD(Entity_Player, TriggerRoomClear, () -> void) {
	const int callbackid = 1069;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);


		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}

	}

	super();

	const int callbackid2 = 1138;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(*this->GetVariant())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
}

// MC_PLAYER_GET_ACTIVE_MAX_CHARGE (below) can get triggered multiple times in a single call to some functions.
// Some result caching is done here specifically for those functions, to reduce how many times the callback runs.
// Unfortunately these callbacks can still run quite a few times per frame in some situations.
bool cacheMaxChargeCallback = false;
std::unordered_map<int, int> cachedMaxCharge;

HOOK_METHOD(Entity_Player, ControlActiveItem, (int slot) -> void) {
	cacheMaxChargeCallback = true;
	cachedMaxCharge.clear();

	super(slot);

	cacheMaxChargeCallback = false;
	cachedMaxCharge.clear();
}

HOOK_METHOD(Entity_Player, AddActiveCharge, (int charge, int slot, bool flashHUD, bool overcharge, bool force) -> int) {
	cacheMaxChargeCallback = true;
	cachedMaxCharge.clear();

	const int result = super(charge, slot, flashHUD, overcharge, force);

	cacheMaxChargeCallback = false;
	cachedMaxCharge.clear();

	return result;
}

HOOK_METHOD(PlayerHUD, RenderActiveItem, (unsigned int activeSlot, const Vector& pos, int hudSlot, float size, float alpha, bool unused) -> void) {
	cacheMaxChargeCallback = true;
	cachedMaxCharge.clear();

	super(activeSlot, pos, hudSlot, size, alpha, unused);

	cacheMaxChargeCallback = false;
	cachedMaxCharge.clear();
}

//PLAYER_GET_ACTIVE_MAX_CHARGE (id: 1072)
HOOK_OVERLOADED_METHOD(Entity_Player, GetActiveMaxCharge, int, (int, int), (int item, int vardata) -> int) {
	const int normalMaxCharge = super(item, vardata);

	const int callbackid = 1072;
	if (item != 0 && CallbackState.test(callbackid - 1000)) {
		if (cacheMaxChargeCallback && cachedMaxCharge.find(item) != cachedMaxCharge.end()) {
			return cachedMaxCharge[item];
		} else {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
				.push(item)
				.push(item)
				.push(this, lua::Metatables::ENTITY_PLAYER)
				.push(vardata)
				.push(normalMaxCharge)
				.call(1);

			if (!result && lua_isinteger(L, -1)) {
				const int newMaxCharge = std::max((int)lua_tointeger(L, -1), 0);
				if (cacheMaxChargeCallback) {
					cachedMaxCharge[item] = newMaxCharge;
				}
				return newMaxCharge;
			}
		}
	}

	return normalMaxCharge;
}

//PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE (id: 1073)
HOOK_METHOD(Entity_Player, GetActiveMinUsableCharge, (int slot) -> int) {
	const int normalMinCharge = super(slot);

	const int callbackid = 1073;
	if (this->GetActiveItem(slot) != 0 && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetActiveItem(slot))
			.push(slot)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(normalMinCharge)
			.call(1);

		if (!result && lua_isinteger(L, -1)) {
			return std::max((int)lua_tointeger(L, -1), 0);
		}
	}

	return normalMinCharge;
}

//MC_PRE_REPLACE_SPRITESHEET (id: 1116)
HOOK_METHOD(ANM2, ReplaceSpritesheet, (int LayerID, std::string& PngFilename) -> void) {
	const int callbackid1 = 1116;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(_filename.c_str())
			.push(LayerID)
			.push(PngFilename.c_str())
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				LayerID = lua::callbacks::ToInteger(L, 1);

				PngFilename = lua::callbacks::ToString(L, 2);
			}
		}
	}
	super(LayerID, PngFilename);
	const int callbackid2 = 1117;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(_filename.c_str())
			.push(LayerID)
			.push(PngFilename.c_str())
			.call(1);
	}
}

//PLAYER_GET_HEART_LIMIT (id: 1074)
HOOK_METHOD(Entity_Player, GetHealthLimit, (bool keeper) -> int) {
	const int callbackid = 1074;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(super(keeper))
			.push(keeper)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return super(keeper);
}

//POST_SLOT_INIT (1121)
HOOK_METHOD(Entity_Slot, Init, (unsigned int Type, unsigned int Variant, unsigned int SubType, unsigned int Seed) -> void) {
	super(Type, Variant, SubType, Seed);
	const int callbackid = 1121;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(Variant)
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

// PRE/POST_SLOT_UPDATE (1169 / 1122)
HOOK_METHOD(Entity_Slot, Update, () -> void) {
	const int precallbackid = 1169;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(precallbackid)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.call(1);

		if (!result && lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == true) {
			((Entity*)this)->Original_Update();
			return;
		}
	}

	super();

	const int postcallbackid = 1122;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postcallbackid)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

//PRE/POST_SLOT_CREATE_EXPLOSION_DROPS (1123/1124)
HOOK_METHOD(Entity_Slot, CreateDropsFromExplosion, () -> void) {
	const int callbackid1 = 1123;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super();
	const int callbackid2 = 1124;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

//PRE/POST_SLOT_SET_PRIZE_COLLECTIBLE (1125/1126)
HOOK_METHOD(Entity_Slot, SetPrizeCollectible, (int id) -> void) {
	const int callbackid1 = 1125;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.push(id)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isnumber(L, -1)) {
				id = (int)lua_tonumber(L, -1);
			}
		}
	}
	super(id);
	const int callbackid2 = 1126;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(*this->GetVariant())
			.pushLuabridge(this, lua::metatables::EntitySlotMT)
			.push(id)
			.call(1);
	}
}

//POST_ITEM_OVERLAY_UPDATE (id: 1075)
HOOK_METHOD(ItemOverlay, Update, (bool unk) -> void) {
	super(unk);
	const int callbackid = 1075;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(_overlayID)
			//.push(this, lua::metatables::ItemOverlayMT)
			.push(_overlayID)
			.push(unk)
			.call(1);
	}
}

//PRE/POST_ITEM_OVERLAY_SHOW (id: 1076, 1134)
HOOK_METHOD_PRIORITY(ItemOverlay, Show, -100, (int overlayID, int delay, Entity_Player* player) -> void) {
	lua_State* L = g_LuaEngine->_state;
	const int callbackid1 = 1076;
	if (CallbackState.test(callbackid1 - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(overlayID)
			//.push(this, lua::metatables::ItemOverlayMT)
			.push(overlayID)
			.push(delay)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isnumber(L, -1)) {
				overlayID = (int)lua_tonumber(L, -1);
			}
		}
	}
	super(overlayID, delay, player);
	const int callbackid2 = 1134;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid2)
			.push(overlayID)
			//.push(this, lua::metatables::ItemOverlayMT)
			.push(overlayID)
			.push(delay)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}

}

//POST_PLAYER_NEW_ROOM_TEMP_EFFECTS (id: 1077)
HOOK_METHOD(Entity_Player, TriggerNewRoom_TemporaryEffects, () -> void) {
	super();

	const int callbackid = 1077;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
}

//POST_PLAYER_NEW_LEVEL (id: 1078)
HOOK_METHOD(Entity_Player, TriggerNewStage, (bool unk) -> void) {
	
	super(unk);
	const int callbackid = 1078;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(unk)
			.call(1);
	}
}

HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	super(CharacterId, RenderPos, DefaultSprite);
	const int callbackid = 1333;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(CharacterId)
			.push(CharacterId)
			.pushUserdataValue(*RenderPos, lua::Metatables::VECTOR)
			.push(DefaultSprite, lua::Metatables::SPRITE)
			.call(1);
	}
}

//COMPLETION_MARK_GET 1047
//POST_COMPLETION_MARK_GET 1048 // There are in CompletionTracker.cpp for convenience
//(PRE/POST)_COMPLETION_EVENT (1049/1052) 
HOOK_STATIC(Manager, RecordPlayerCompletion, (int completion) -> void, __stdcall) {
	const int callbackid1 = 1049;
	const int callbackid2 = 1052;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {

		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(completion)
			.push(completion)
			.call(1);

		if (!result)
		{
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1) == false)
					return;
			}
			else if (lua_isinteger(L, -1)) {
				int retCompletion = (int)lua_tointeger(L, -1);
				if (retCompletion >= 0 && retCompletion <= 17)
					completion = retCompletion;
				else
					return;
			}
		}
	}

	super(completion);

	if (CallbackState.test(callbackid2 - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid2)
			.push(completion)
			.push(completion)
			.call(1);
	}
}

// PRE/POST_PLAYERHUD_RENDER_ACTIVE_ITEM (1119/1079)
// TODO(connor): REP+ refactors may have made some of the offset corrections here obselete, also hudSlot might be relevant, investigate more later
HOOK_METHOD(PlayerHUD, RenderActiveItem, (unsigned int activeSlot, const Vector &pos, int hudSlot, float size, float alpha, bool unused) -> void) {
	const bool isSchoolbagSlot = (activeSlot == 1);

	// If the slot is ActiveSlot.SLOT_SECONDARY (schoolbag), halve the size/scale.
	// The game does this inside RenderActiveItem.
	// Size adjustments for pocket slots are already accounted for.
	float actualSize = size;
	if (isSchoolbagSlot) {
		actualSize *= 0.5;
	}

	// The positions we send through the callback may be modified slightly to be more accurate to where the game actually rendered stuff.
	Vector itemPos = pos;
	Vector chargeBarPos = pos;

	// Index #4 is for player 1's Esau, who has different offsets for the charge bar.
	if (this->_playerHudIndex < 4) {
		chargeBarPos.x += (isSchoolbagSlot ? -2 : 34) * actualSize;
	}
	else if (isSchoolbagSlot) {
		chargeBarPos.x += 38 * actualSize;
	}
	chargeBarPos.y += 17 * actualSize;

	if (this->_activeItem[activeSlot].bookImage != nullptr) {
		// A book sprite was rendered under the item (Book of Virtues or Judas' Birthright).
		// Update the offsets we're sending through the callbacks to match where the corresponding sprites were actually rendered.
		itemPos.y -= 4;
		chargeBarPos.y += 3;
	}
	
	const int precallbackid = 1119;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults result = lua::LuaCaller(L).push(precallbackid)
			.pushnil()
			.push(this->GetPlayer(), lua::Metatables::ENTITY_PLAYER)
			.push(activeSlot)
			.pushUserdataValue(itemPos, lua::Metatables::VECTOR)
			.push(alpha)
			.push(actualSize)
			.pushUserdataValue(chargeBarPos, lua::Metatables::VECTOR)
			.call(1);

		if (!result && lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == true) {
			return;
		}
	}

	super(activeSlot, pos, hudSlot, size, alpha, unused);

	const int postcallbackid = 1079;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(postcallbackid)
			.pushnil()
			.push(this->GetPlayer(), lua::Metatables::ENTITY_PLAYER)
			.push(activeSlot)
			.pushUserdataValue(itemPos, lua::Metatables::VECTOR)
			.push(alpha)
			.push(actualSize)
			.pushUserdataValue(chargeBarPos, lua::Metatables::VECTOR)
			.call(1);
	}
}

//PRE/POST_PLAYERHUD_RENDER_HEARTS (1118/1091)
HOOK_METHOD(PlayerHUD, RenderHearts, (Vector* unk, ANM2* sprite, int hudSlot, float scale, Vector pos) -> void) {
	lua_State* L = g_LuaEngine->_state;

	const int callbackid1 = 1118;
	if (CallbackState.test(callbackid1 - 1000)) {

		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.pushnil()
			.push(unk, lua::Metatables::VECTOR)
			.push(sprite, lua::Metatables::SPRITE)
			.pushUserdataValue(pos, lua::Metatables::VECTOR)
			.push(scale)
			.push(_player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(unk, sprite, hudSlot, scale, pos);

	const int callbackid2 = 1091;
	if (CallbackState.test(callbackid2 - 1000)) {
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid2)
			.pushnil()
			.push(unk, lua::Metatables::VECTOR)
			.push(sprite, lua::Metatables::SPRITE)
			.pushUserdataValue(pos, lua::Metatables::VECTOR)
			.push(scale)
			.push(_player, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
}

//MC_PRE_GET_LIGHTING_ALPHA
HOOK_METHOD(Room, GetLightingAlpha, () -> float) {
	const int callbackid = 1150;
	float originalAlpha = super();
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults result =  lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(originalAlpha)
			.call(1);

		if (!result) {
			return (float)lua_tonumber(L, -1);
		}
	}
	return originalAlpha;
}

//MC_PRE_GRID_LIGHTING_RENDER
HOOK_METHOD(Room, RenderGridLight, (GridEntity* grid, Vector& offset) -> void) {
	const int callbackid = 1151;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(grid->GetType())
			.push(grid, lua::Metatables::GRID_ENTITY)
			.pushUserdataValue(offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = offset + *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(grid, offset);
}

//MC_PRE_ENTITY_LIGHTING_RENDER
HOOK_METHOD(Room, RenderEntityLight, (Entity* ent, Vector& offset) -> void) {
	const int callbackid = 1152;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*ent->GetType())
			.push(ent, lua::Metatables::ENTITY)
			.pushUserdataValue(offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = offset + *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}
	super(ent, offset);
}

//PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM (1092)
HOOK_METHOD(Entity_Player, GetCollectibleNum, (int CollectibleID, bool OnlyCountTrueItems) -> int) {
	const int callbackid = 1092;
	int originalCount = super(CollectibleID, OnlyCountTrueItems);
	int modCount = 0;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(modCount)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(CollectibleID)
			.push(OnlyCountTrueItems)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				modCount = (int)lua_tointeger(L, -1);
			}
		}
	}
	return originalCount + modCount;
}

//PRE_PLAYER_HAS_COLLECTIBLE (1093) [currently lags with enabled debug 12, unused]
/*HOOK_METHOD(Entity_Player, HasCollectible, (int CollectibleID, bool OnlyCountTrueItems) -> bool) {
	const int callbackid = 1093;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(CollectibleID)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(CollectibleID)
			.push(OnlyCountTrueItems)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return lua_toboolean(L, -1);
			}
		}
	}
	return super(CollectibleID, OnlyCountTrueItems);
}
*/

//PRE_MUSIC_PLAY_JINGLE (1094)
HOOK_METHOD(Music, PlayJingle, (int musicId, int unusedInt, bool unusedBool) -> void) {
	const int callbackid = 1094;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(musicId)
			.push(musicId)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				super((int)lua_tointeger(L, -1), unusedInt, unusedBool);
				return;
			}
			else if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(musicId, unusedInt, unusedBool);
}

//POST_TRIGGER_COLLECTIBLE_REMOVED (1095) 
HOOK_METHOD(Entity_Player, TriggerCollectibleRemoved, (unsigned int collectibleID) -> void) {
	super(collectibleID);
	const int callbackid = 1095;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(collectibleID)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(collectibleID)
			.call(1);
	}
}

//POST_TRIGGER_TRINKET_ADDED (1096) 
HOOK_METHOD(Entity_Player, TriggerTrinketAdded, (unsigned int trinketID, bool firstTimePickingUp) -> void) {
	super(trinketID, firstTimePickingUp);
	const int callbackid = 1096;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(trinketID)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(trinketID)
			.push(firstTimePickingUp)
			.call(1);
	}
}

//POST_TRIGGER_TRINKET_REMOVED (1097) 
HOOK_METHOD(Entity_Player, TriggerTrinketRemoved, (unsigned int trinketID) -> void) {
	super(trinketID);
	const int callbackid = 1097;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(trinketID)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(trinketID)
			.call(1);
	}
}

//POST_TRIGGER_WEAPON_FIRED (1098)
HOOK_METHOD(Weapon, TriggerTearFired, (const Vector& dir, int FireAmount) -> void) {
	super(dir, FireAmount);
	const int callbackid = 1098;
	Entity* ent = this->GetOwner();
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(GetWeaponType())
			.pushUserdataValue(dir, lua::Metatables::VECTOR)
			.push(FireAmount)
			.push(ent, lua::Metatables::ENTITY);
		
		WeaponData* data = new (caller.pushUd(sizeof(WeaponData), lua::metatables::WeaponMT)) WeaponData;
		data->weapon = this;
		if (Entity_Familiar* familiar = ent->ToFamiliar()) {
			data->owner = familiar;
		}
		else if (Entity_Player* player = ent->ToPlayer()) {
			data->owner = player;
			for (int i = 0; i < 4; ++i) {
				if (*(player->GetWeapon(i)) == this) {
					data->slot = i;
					break;
				}
			}
		}

		lua::LuaResults result = caller.call(1);
	}
}

//POST_WEAPON_FIRE (1105)
HOOK_METHOD(Weapon, Fire, (const Vector& dir, bool isShooting, bool isInterpolated)-> void) {
	super(dir, isShooting, isInterpolated);
	const int callbackid = 1105;
	Entity* ent = this->GetOwner();
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(this->GetWeaponType());
		WeaponData* data = new (caller.pushUd(sizeof(WeaponData), lua::metatables::WeaponMT)) WeaponData;
		data->weapon = this;
		if (Entity_Familiar* familiar = ent->ToFamiliar()) {
			data->owner = familiar;
		}
		else if (Entity_Player* player = ent->ToPlayer()) {
			data->owner = player;
			for (int i = 0; i < 4; ++i) {
				if (*(player->GetWeapon(i)) == this) {
					data->slot = i;
					break;
				}
			}
		}
		caller.pushUserdataValue(dir, lua::Metatables::VECTOR)
			.push(isShooting)
			.push(isInterpolated)
			.call(1);
	}
}

//POST_GRID_ROCK_DESTROY (1011)
void ProcessGridRockDestroy(GridEntity_Rock* gridRock, bool Immediate, int type) {
	const int callbackid = 1011;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(type)
			.push(gridRock, lua::Metatables::GRID_ENTITY_ROCK)
			.push(type)
			.push(Immediate)
			.call(1);
	}
}


HOOK_METHOD(GridEntity_Rock, Destroy, (bool Immediate) -> bool) {
	bool result = super(Immediate);
	GridEntity_Rock* gridRock = (GridEntity_Rock*)this;
	int gridType = gridRock->GetDesc()->_type;
	if (result) ProcessGridRockDestroy(gridRock,Immediate, gridType);
	return result;
	
}

//(POST_)GRID_HURT_DAMAGE (1012/1013)
void ProcessPostGridHurtDamage(GridEntity* gridEnt, int type, Entity* ent, int Damage, int DamageFlags, float unk3, bool unk4) {
	const int callbackid = 1013;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(type)
			.push(gridEnt, lua::Metatables::GRID_ENTITY)
			.push(ent, lua::Metatables::ENTITY)
			.push(Damage)
			.push(DamageFlags)
			.push(unk3)
			.push(unk4)
			.call(1);
	}
}

HOOK_METHOD(GridEntity, hurt_func, (Entity* ent, int Damage, int DamageFlags, float unk3, bool unk4) -> void) {
	const int callbackid = 1012;
	if (!(CallbackState.test(callbackid - 1000) || CallbackState.test((callbackid + 1) - 1000))) { super(ent, Damage, DamageFlags, unk3, unk4); return; }
	int gridType = this->GetDesc()->_type;

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
		.push(gridType)
		.push(this, lua::Metatables::GRID_ENTITY)
		.push(ent, lua::Metatables::ENTITY)
		.push(Damage)
		.push(DamageFlags)
		.push(unk3)
		.push(unk4)
		.call(1);

	if (!result) {
		if (lua_isboolean(L, -1)) {
			if (lua_toboolean(L, -1)) {
				return;
			}
		}
		else {
			super(ent, Damage, DamageFlags, unk3, unk4);
			ProcessPostGridHurtDamage(this, gridType, ent, Damage, DamageFlags, unk3, unk4);
		}
	}
}

// MC_POST_LEVEL_LAYOUT_GENERATED
HOOK_METHOD(LevelGenerator, Generate, (int unk, bool unk2, bool unk3, bool unk4, unsigned int const& allowedShapes, unsigned int numDeadEnds, LevelGenerator_Room* startRoom) -> void) {
	super(unk, unk2, unk3, unk4, allowedShapes, numDeadEnds, startRoom);

	const int callbackId = 1099;
	if (!CallbackState.test(callbackId - 1000)) {
		return;
	}

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
	lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
		.push(12)
		.push(this, lua::metatables::LevelGeneratorMT)
		.call(0);

	/* std::ofstream stream("repentogon.log", std::ios::app);
	stream << "After the call to Generate: " << std::endl;
	stream << " - Dead ends (" << GetDeadEnds()->size() << "): ";
	for (int idx : *GetDeadEnds()) {
		stream << idx << " ";
	}
	stream << std::endl;
	stream << " - Non dead ends (" << GetNonDeadEnds()->size() << "): ";
	for (int idx : *GetNonDeadEnds()) {
		stream << idx << " ";
	}
	stream << std::endl;
	stream << " - Rooms are as follows : " << std::endl;
	for (LevelGenerator_Room const& room : *allRooms) {
		stream << "\tRoom " << room._generationIndex << " at (" << room._gridColIdx << ", " << room._gridLineIdx << ") of shape " << room._shape << " with allowed door slots " << room._doors << " connects to ";
		for (auto const& neighbor : room._neighbors) {
			stream << neighbor << " ";
		}
		stream << std::endl;
	}

	stream.flush(); */
}

//POST_NIGHTMARE_SCENE_RENDER (1102)
/*HOOK_METHOD(NightmareScene, Render, () -> void) {
	super();
	const int callbackid = 1102;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			//.push(this, lua::metatables::NightmareSceneMT)
			.call(1);
	}
}
*/

//POST_NIGHTMARE_SCENE_SHOW (1103)
HOOK_METHOD(NightmareScene, Show, (bool unk) -> void) {
	super(unk);
	const int callbackid = 1103;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			//.push(this, lua::metatables::NightmareSceneMT)
			.push(unk)
			.call(1);
	}
}

// PRE_LEVEL_SELECT (1104)
HOOK_METHOD(Level, SetStage, (int levelType, int stageType) -> void) {
	// logViewer.AddLog("[REPENTOGON]", "Level::SetStage: stageid: %d, alt: %d\n", levelType, stageType);
	const int callbackId = 1104;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		int startTop = lua_gettop(L);
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(levelType)
			.push(stageType)
			.call(LUA_MULTRET);

		int resTop = lua_gettop(L);
		if (startTop == resTop) {
			super(levelType, stageType);
			return;
		}
		else {
			if (resTop == startTop + 1) {
				if (lua_istable(L, -1)) {
					lua_len(L, -1);
					int len = (int)lua_tointeger(L, -1);
					lua_pop(L, 1);

					if (len != 2) {
						logViewer.AddLog(LogViewer::Game, "MC_PRE_LEVEL_SELECT: Invalid return value, table contains %d elements, expected 2\n", len);
						goto error;
					}

					lua_rawgeti(L, -1, 1);
					int level = (int)lua_tointeger(L, -1);
					lua_pop(L, 1);
					if (g_Game->IsGreedMode()) {
						if (level < 1 || level > 7) {
							logViewer.AddLog(LogViewer::Game, "MC_PRE_LEVEL_SELECT: Invalid level stage, received %d, should be in range [1; 7] (detected greed(ier) mode)\n", level);
							goto error;
						}
					}
					else {
						if (level < 1 || level > 14) {
							logViewer.AddLog(LogViewer::Game, "MC_PRE_LEVEL_SELECT: Invalid level stage, received %d, should be in range [1; 14] (detected normal/hard mode)\n", level);
							goto error;
						}
					}

					lua_rawgeti(L, -1, 2);
					int type = (int)lua_tointeger(L, -1);
					lua_pop(L, 1);
					if (type < 0 || type == 3 || type > 5) {
						logViewer.AddLog(LogViewer::Game, "MC_PRE_LEVEL_SELECT: Invalid stage type, received value %d, expected 0, 1, 2, 4 or 5\n", type);
						goto error;
					}

					super(level, type);
					return;
				}
			}
			else {
				logViewer.AddLog(LogViewer::Game, "MC_PRE_LEVEL_SELECT: Invalid number of return values, got %d, expected 1\n", resTop - startTop);
				goto error;
			}
		}

	error:
		super(levelType, stageType);
	}
	else {
		super(levelType, stageType);
	}
}

//MC_PRE_BACKDROP_CHANGE/INIT (1141-1142) --> it's chilling on xmldata.cpp for now, may be moved here later

HOOK_METHOD(Backdrop, RenderWalls, (Vector const& renderOffset, ColorMod mod) -> void) {
	const int callbackId = 1106;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(&mod, lua::Metatables::COLOR)
			.call(0);
	}

	super(renderOffset, mod);
}

HOOK_METHOD(Backdrop, RenderFloor, (Vector const& renderOffset, ColorMod mod) -> void) {
	const int callbackId = 1107;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(&mod, lua::Metatables::COLOR)
			.call(0);
	}

	super(renderOffset, mod);
}

HOOK_METHOD(Backdrop, RenderWater, (Vector const& renderOffset) -> void) {
	const int callbackId = 1108;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.call(0);
	}

	super(renderOffset);
}

HOOK_METHOD(Backdrop, pre_render_walls, () -> void) {
	super();

	const int callbackId = 1109;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.call(0);
	}
}

//PLAYER_INIT_PRE_LEVEL_INIT_STATS (1127)
HOOK_METHOD(Entity_Player, InitPreLevelInitStats, () -> void) {
	const int callbackid = 1127;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		Entity_Player* ent = (Entity_Player*)this;

		lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(ent, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
	super();
}

//PRE_NEW_ROOM (1200)
HOOK_METHOD(Room, Init, (int param_1, RoomDescriptor* descriptor) -> void) {
	const int callbackid = 1200;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this, lua::Metatables::ROOM)
			.push(descriptor, lua::Metatables::ROOM_DESCRIPTOR)
			.call(1);
	}

	super(param_1, descriptor);
}

HOOK_METHOD(ModManager, LoadConfigs, () -> void) {
	super();
	const int callbackid = 1210;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.call(1);
	}
}

// PRE_RENDER
HOOK_METHOD(Manager, Render, () -> void) {
	const int callbackid = 1135;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.call(1);
	}

	super();
}

HOOK_METHOD(Level, place_room, (LevelGenerator_Room* slot, RoomConfig_Room* config, uint32_t seed, uint32_t unk) -> bool) {
	const int callbackid = 1137;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller caller(L);
		LuaLevelGeneratorRoom* room = caller.push(callbackid).pushnil().pushUd<LuaLevelGeneratorRoom>(lua::metatables::LevelGeneratorRoomMT);
		room->cleanup = false;
		room->context = nullptr;
		room->room = slot;
		
		RoomConfig_Room* other = nullptr;
		lua::LuaResults results = caller.push(config, lua::Metatables::ROOM_CONFIG_ROOM).push(seed).call(1);
		if (lua_isuserdata(L, -1)) {
			auto opt = lua::TestUserdata<RoomConfig_Room*>(L, -1, lua::Metatables::ROOM_CONFIG_ROOM);

			if (!opt) {
				KAGE::LogMessage(2, "Invalid userdata returned in MC_PRE_LEVEL_PLACE_ROOM");
			}
			else {
				other = *opt;
			}
		}

		if (other) {
			bool ok = true;
			if (other->Shape != config->Shape) {
				KAGE::_LogMessage(2, "MC_PRE_LEVEL_PLACE_ROOM: Shape mismatch. Original = %d, override = %d\n", config->Shape, other->Shape);
				ok = false;
			}

			for (int i = 0; i < MAX_DOOR_SLOTS; ++i) {
				if (slot->_doors & (1 << i) && !(other->Doors & (1 << i))) {
					KAGE::_LogMessage(2, "MC_PRE_LEVEL_PLACE_ROOM: Required doors mismatch. Slot %d (mask = %d) required\n", i, 1 << i);
					ok = false;
				}
			}
			
			if (ok) {
				return super(slot, other, seed, unk);
			}
		}
	}
	return super(slot, config, seed, unk);
}

//MC_POST_PLAYER_GET_MULTI_SHOT_PARAMS (1251)
static bool isMultiShotParamsEvaluating = false;
HOOK_METHOD(Entity_Player, GetMultiShotParams, (Weapon_MultiShotParams* params, int weaponType) -> Weapon_MultiShotParams*) {
	if (isMultiShotParamsEvaluating) // Prevent infinite loop when number of shots was altered
	{
		return super(params, weaponType);
	}

	params = super(params, weaponType);

	isMultiShotParamsEvaluating = true;
	const int callbackid = 1251;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		Entity_Player* ent = (Entity_Player*)this;

		lua::LuaResults results = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(ent, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (lua_isuserdata(L, -1)) {
			auto opt = lua::GetUserdata<Weapon_MultiShotParams*>(L, -1, lua::metatables::MultiShotParamsMT);

			if (!opt) {
				KAGE::LogMessage(2, "Invalid userdata returned in MC_POST_GET_MULTI_SHOT_PARAMS");
			}
			else {
				*params = *opt;
			}
		}
	}
	isMultiShotParamsEvaluating = false;
	return params;
}

//POST_FAMILIAR_FIRE_PROJECTILE (id: 1252)
HOOK_METHOD(Entity_Familiar, FireProjectile, (const Vector& AimDirection, bool unk) -> Entity_Tear*) {
	Entity_Tear* tear = super(AimDirection, unk);
	const int callbackid = 1252;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		Entity_Familiar* fam = (Entity_Familiar*)this;


		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*fam->GetVariant())
			.push(tear, lua::Metatables::ENTITY_TEAR)
			.call(1);
	}

	return tear;
}

//POST_FIRE_BOMB (id: 1253)
HOOK_METHOD(Entity_Player, FireBomb, (Vector* pos, Vector* vel, Entity* source) -> Entity_Bomb*) {
	Entity_Bomb* bomb = super(pos, vel, source);
	const int callbackid = 1253;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(bomb, lua::Metatables::ENTITY_BOMB)
			.call(1);
	}

	return bomb;
}

//POST_FIRE_BONE_CLUB (id: 1254)
HOOK_METHOD(Entity_Player, FireBoneClub, (Entity* parent, uint32_t variant, bool unk, Weapon* weapon, float unk2) -> Entity_Knife*) {
	Entity_Knife* club = super(parent, variant, unk, weapon, unk2);
	const int callbackid = 1254;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(club, lua::Metatables::ENTITY_KNIFE)
			.call(1);
	}

	return club;
}

//POST_FIRE_BRIMSTONE (id: 1255)
HOOK_STATIC(Weapon, FireBrimstone, (Entity* entity,  const Vector& position, const Vector& direction, float damageMult, float unk1, uint32_t unk2)-> Entity_Laser*, __cdecl) {
	Entity_Laser* brimstone = super(entity, position, direction, damageMult, unk1, unk2);
	const int callbackid = 1255;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(brimstone, lua::Metatables::ENTITY_LASER)
			.call(1);
	}

	return brimstone;
}

//POST_FIRE_BRIMSTONE_BALL (id: 1256)
HOOK_METHOD(Entity_Player, FireBrimstoneBall, (const Vector& pos, const Vector& vel, const Vector& offset, uint32_t unk1, uint32_t unk2, Entity* unk3) -> Entity_Effect*) {
	Entity_Effect* ball = super(pos, vel, offset, unk1, unk2, unk3);
	const int callbackid = 1256;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(ball, lua::Metatables::ENTITY_EFFECT)
			.call(1);
	}

	return ball;
}

//POST_FIRE_KNIFE (id: 1257)
HOOK_METHOD(Entity_Player, FireKnife, (Entity* parent, uint32_t variant, float rotationOffset, bool cantOverwrite, uint32_t subType) -> Entity_Knife*) {
	Entity_Knife* knife = super(parent, variant, rotationOffset, cantOverwrite, subType);
	const int callbackid = 1257;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(knife, lua::Metatables::ENTITY_KNIFE)
			.call(1);
	}

	return knife;
}

//POST_FIRE_SWORD (id: 1258)
HOOK_METHOD(Weapon, FireSword, (Entity* param_1, uint32_t param_2, float param_3, bool param_4, bool param_5, uint32_t param_6, Entity* param_7) -> Entity_Knife*) {
	Entity_Knife* sword = super(param_1, param_2, param_3, param_4, param_5, param_6, param_7);
	const int callbackid = 1258;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(sword, lua::Metatables::ENTITY_KNIFE)
			.call(1);
	}

	return sword;
}

//POST_FIRE_TECH_LASER (id: 1259)
HOOK_METHOD(Entity_Player, FireTechLaser, (const Vector& pos, uint32_t offsetID, const Vector& dir, bool leftEye, bool oneHit, Entity * source, float damageScale) -> Entity_Laser*) {
	Entity_Laser* laser = super(pos, offsetID, dir, leftEye, oneHit, source, damageScale);
	const int callbackid = 1259;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(laser, lua::Metatables::ENTITY_LASER)
			.call(1);
	}

	return laser;
}

//POST_FIRE_TECH_X_LASER (id: 1260)
HOOK_METHOD(Entity_Player, FireTechXLaser, (const Vector& pos, const Vector& dir, float radius, Entity* source, float damageMultiplier) -> Entity_Laser*) {
	Entity_Laser* laser = super(pos, dir, radius, source, damageMultiplier);
	const int callbackid = 1260;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(laser, lua::Metatables::ENTITY_LASER)
			.call(1);
	}

	return laser;
}

//POST_FAMILIAR_FIRE_BRIMSTONE (id: 1261)
HOOK_METHOD(Entity_Familiar, FireBrimstone, (const Vector& AimDirection, bool unk) -> Entity_Laser*) {
	Entity_Laser* brimstone = super(AimDirection, unk);
	const int callbackid = 1261;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		Entity_Familiar* fam = (Entity_Familiar*)this;


		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*fam->GetVariant())
			.push(brimstone, lua::Metatables::ENTITY_LASER)
			.call(1);
	}

	return brimstone;
}

//POST_FAMILIAR_FIRE_TECH_LASER (id: 1262)
HOOK_METHOD(Entity_Familiar, FireTechLaser, (const Vector& AimDirection) -> Entity_Laser*) {
	Entity_Laser* laser = super(AimDirection);
	const int callbackid = 1262;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		Entity_Familiar* fam = (Entity_Familiar*)this;


		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*fam->GetVariant())
			.push(laser, lua::Metatables::ENTITY_LASER)
			.call(1);
	}

	return laser;
}

/////////////////////////////////////////////////

struct GridEntityRenderInputs {
	GridEntity* grid;
	const Vector& renderOffset;

	lua::Metatables vanilla_metatable = lua::Metatables::GRID_ENTITY;
	const char* luabridge_metatable = nullptr;

	void SetMetatable(const lua::Metatables metatable) {
		vanilla_metatable = metatable;
	}
	void SetMetatable(const char* metatable) {
		luabridge_metatable = metatable;
	}
};

struct PreGridEntityResult {
	bool skip_internal_code = false;
	Vector renderOffset;
};

lua::LuaResults RunGridRenderCallback(const GridEntityRenderInputs& inputs, const int callbackid, bool pushOffset, lua_State* L) {
	int discriminator = inputs.grid->GetDesc()->_type;
	lua::LuaCaller& lua_caller = lua::LuaCaller(L).push(callbackid).push(discriminator);
	if (inputs.luabridge_metatable != nullptr) {
		lua_caller.pushLuabridge(inputs.grid, inputs.luabridge_metatable);
	}
	else {
		lua_caller.push(inputs.grid, inputs.vanilla_metatable);
	}
	if (pushOffset)
	{
		lua_caller.pushUserdataValue(inputs.renderOffset, lua::Metatables::VECTOR);
	}
	return lua_caller.call(1);
}

PreGridEntityResult ProcessPreGridRenderCallback(const GridEntityRenderInputs& inputs, const int callbackid) {
	PreGridEntityResult result{ false, inputs.renderOffset };

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults lua_result = RunGridRenderCallback(inputs, callbackid, true, L);

		if (!lua_result) {
			if (lua_isboolean(L, -1)) {
				// Vanilla boolean returns always skip internal code.
				result.skip_internal_code = true;
			}
			else if (lua_isuserdata(L, -1)) {
				result.renderOffset = *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}

	return result;
}

void ProcessPostGridRenderCallback(const GridEntityRenderInputs& inputs, const int callbackid) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		RunGridRenderCallback(inputs, callbackid, false, L);
	}
}

void HandleGridRenderCallbacks(const GridEntityRenderInputs& inputs, const int precallbackid, const int postcallbackid, std::function<void(Vector&)> super_lambda) {
	const PreGridEntityResult pre_render_result = ProcessPreGridRenderCallback(inputs, precallbackid);

	if (!pre_render_result.skip_internal_code) {
		Vector offset = pre_render_result.renderOffset;
		super_lambda(offset);
		ProcessPostGridRenderCallback(inputs, postcallbackid);
	}
}

#define _GRIDRENDER_SUPER_LAMBDA() [this](Vector& offset) { super(offset); }
#define HOOK_GRIDRENDER_CALLBACKS(_type, _metatable, _precallback, _postcallback) \
HOOK_METHOD(_type, Render, (Vector& offset) -> void) { \
	GridEntityRenderInputs inputs = {this, offset}; \
	inputs.SetMetatable(_metatable); \
	HandleGridRenderCallbacks(inputs, _precallback, _postcallback, _GRIDRENDER_SUPER_LAMBDA()); \
}

HOOK_GRIDRENDER_CALLBACKS(GridEntity_Decoration, lua::metatables::GridDecorationMT, 1444, 1445);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Door, lua::Metatables::GRID_ENTITY_DOOR, 1446, 1447);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Fire, lua::metatables::GridFireMT, 1448, 1449);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Pit, lua::Metatables::GRID_ENTITY_PIT, 1454, 1455);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Poop, lua::Metatables::GRID_ENTITY_POOP, 1456, 1457);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Rock, lua::Metatables::GRID_ENTITY_ROCK, 1458, 1459);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_PressurePlate, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, 1460, 1461);
HOOK_GRIDRENDER_CALLBACKS(GridEntity_Wall, lua::metatables::GridWallMT, 1462, 1463);

struct GridRenderCallback {
	GridEntityType type;
	lua::Metatables vanilla_metatable;
	const char* luabridge_metatable;
	int precallback, postcallback;
};

// Statue vtable will need to be patched, it has no Render function at all
struct GridRenderCallback callbacks[8] = {
	{GRID_SPIKES, lua::Metatables::GRID_ENTITY_SPIKES, nullptr, 1432, 1433},
	{GRID_SPIKES_ONOFF, lua::Metatables::GRID_ENTITY_SPIKES, nullptr, 1432, 1433},
	{GRID_SPIDERWEB, lua::Metatables::GRID_ENTITY, lua::metatables::GridWebMT, 1434, 1435},
	{GRID_TNT, lua::Metatables::GRID_ENTITY, lua::metatables::GridTNT_MT, 1436, 1437},
	{GRID_TRAPDOOR, lua::Metatables::GRID_ENTITY, lua::metatables::GridTrapDoorMT, 1438, 1439},
	{GRID_STAIRS, lua::Metatables::GRID_ENTITY, lua::metatables::GridStairsMT, 1440, 1441},
	{GRID_LOCK, lua::Metatables::GRID_ENTITY, lua::metatables::GridLockMT, 1450, 1451},
	{GRID_TELEPORTER, lua::Metatables::GRID_ENTITY, lua::metatables::GridTeleporterMT, 1452, 1453},
};

HOOK_METHOD(GridEntity_Lock, Render, (Vector& offset) -> void) {
	GridEntityType gridType = (GridEntityType)this->GetDesc()->_type;
	for (int i = 5; i < 7; i++) {
		if (gridType == callbacks[i].type)
		{
			GridEntityRenderInputs inputs = { this, offset };
			if (callbacks[i].luabridge_metatable != nullptr) {
				inputs.SetMetatable(callbacks[i].luabridge_metatable);
			}
			else
			{
				inputs.SetMetatable(callbacks[i].vanilla_metatable);
			}

			HandleGridRenderCallbacks(inputs, callbacks[i].precallback, callbacks[i].postcallback, _GRIDRENDER_SUPER_LAMBDA());
			return;
		}
	}

	super(offset);

}

HOOK_METHOD(GridEntity, Render, (Vector& offset) -> void) {
	GridEntityType gridType = (GridEntityType)this->GetDesc()->_type;
	for (int i = 0; i < 6; i++) {
		if (gridType == callbacks[i].type)
		{
			GridEntityRenderInputs inputs = { this, offset };
			if (callbacks[i].luabridge_metatable != nullptr) {
				inputs.SetMetatable(callbacks[i].luabridge_metatable);
			}
			else
			{
				inputs.SetMetatable(callbacks[i].vanilla_metatable);
			}
			
			HandleGridRenderCallbacks(inputs, callbacks[i].precallback, callbacks[i].postcallback, _GRIDRENDER_SUPER_LAMBDA());
			return;
		}
	}

	super(offset);

}

/* ////////////////////////
// Grid Callbacks START!!
*/ ////////////////////////

struct GridEntityUpdateInputs {
	GridEntity* grid;

	lua::Metatables vanilla_metatable = lua::Metatables::GRID_ENTITY;
	const char* luabridge_metatable = nullptr;

	void SetMetatable(const lua::Metatables metatable) {
		vanilla_metatable = metatable;
	}
	void SetMetatable(const char* metatable) {
		luabridge_metatable = metatable;
	}
};

lua::LuaResults RunGridUpdateCallback(const GridEntityUpdateInputs& inputs, const int callbackid, lua_State* L) {
	int discriminator = inputs.grid->GetDesc()->_type;
	lua::LuaCaller& lua_caller = lua::LuaCaller(L).push(callbackid).push(discriminator);
	if (inputs.luabridge_metatable != nullptr) {
		lua_caller.pushLuabridge(inputs.grid, inputs.luabridge_metatable);
	}
	else {
		lua_caller.push(inputs.grid, inputs.vanilla_metatable);
	}
	return lua_caller.call(1);
}

void ProcessPostGridUpdateCallback(const GridEntityUpdateInputs& inputs, const int callbackid) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		RunGridUpdateCallback(inputs, callbackid, L);
	}
}

bool ProcessPreGridUpdateCallback(const GridEntityUpdateInputs& inputs, const int callbackid) {
	bool skipUpdate = false;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults lua_result = RunGridUpdateCallback(inputs, callbackid, L);

		if (!lua_result) {
			if (lua_isboolean(L, -1)) {
				// Vanilla boolean returns always skip internal code.
				skipUpdate = true;
			}
		}
	}

	return skipUpdate;
}

void HandleGridUpdateCallbacks(const GridEntityUpdateInputs& inputs, const int precallbackid, const int postcallbackid, std::function<void()> super_lambda) {
	const bool pre_update_result = ProcessPreGridUpdateCallback(inputs, precallbackid);

	if (!pre_update_result) {
		super_lambda();
		ProcessPostGridUpdateCallback(inputs, postcallbackid);
	}
}

#define _GRIDUPDATE_SUPER_LAMBDA() [this] () { super(); }
#define HOOK_GRIDUPDATE_CALLBACKS(_type, _metatable, _precallback, _postcallback) \
HOOK_METHOD(_type, Update, () -> void) { \
	GridEntityUpdateInputs inputs = {this}; \
	inputs.SetMetatable(_metatable); \
	HandleGridUpdateCallbacks(inputs, _precallback, _postcallback, _GRIDUPDATE_SUPER_LAMBDA()); \
}

HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Decoration, lua::metatables::GridDecorationMT, 1400, 1401);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Door, lua::Metatables::GRID_ENTITY_DOOR, 1402, 1403);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Fire, lua::metatables::GridFireMT, 1404, 1405);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Gravity, lua::metatables::GridGravityMT, 1406, 1407);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Lock, lua::metatables::GridLockMT, 1408, 1409);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Pit, lua::Metatables::GRID_ENTITY_PIT, 1410, 1411);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Poop, lua::Metatables::GRID_ENTITY_POOP, 1412, 1413);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_PressurePlate, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, 1414, 1415);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Rock, lua::Metatables::GRID_ENTITY_ROCK, 1416, 1417);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Spikes, lua::Metatables::GRID_ENTITY_SPIKES, 1418, 1419);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Stairs, lua::metatables::GridStairsMT, 1420, 1421);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Statue, lua::metatables::GridStatueMT, 1422, 1423);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Teleporter, lua::metatables::GridTeleporterMT, 1424, 1425);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_TrapDoor, lua::metatables::GridTrapDoorMT, 1426, 1427);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_Web, lua::metatables::GridWebMT, 1428, 1429);
HOOK_GRIDUPDATE_CALLBACKS(GridEntity_TNT, lua::metatables::GridTNT_MT, 1430, 1431);

/* ////////////////////////
// Grid Callbacks END!!
*/ ////////////////////////

//IS_PERSISTENT_ROOM_ENTITY (id: 1263)
HOOK_METHOD(Room, IsPersistentRoomEntity, (int type, int variant, int subtype) -> bool) {
	const int callbackid = 1263;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);


		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(type)
			.push(variant)
			.call(1);

		if (!result && lua_isboolean(L, -1)) {
			return lua_toboolean(L, -1);
		}
	}
	return super(type, variant, subtype);
}


HOOK_METHOD(LuaCallbackCaller, CallInputAction, (LuaEngine* engine, Entity* entity, int hook, int action) -> LuaCallbackCallerResult) {
	const int repentogonCallbackId = 1464;
	if (!Isaac::IsInGame()) {
		callbackId = repentogonCallbackId;
	}

	return super(engine, entity, hook, action);
}

//MC_POST_SAVESLOT_LOAD (1470)
bool saveslotselected = false;
bool waszero = false;
HOOK_METHOD_PRIORITY(Manager, SetSaveSlot,-9999, (unsigned int slot) -> void) {
	super(slot);
	unsigned int saveslot = 1;
	if (slot > 0) { 
		saveslot = slot; 
		if (waszero) { saveslotselected = true; } 
	}else { waszero = true; }

	const int callbackid1 = 1470;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid1 - 1000)) {

		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
			.push(saveslot)
			.push(saveslot)
			.push(saveslotselected)
			.push(slot)
			.call(1);

	}


}

//MC_POST_PRE_CHALLENGE_DONE (1471-1472)
HOOK_METHOD_PRIORITY(PersistentGameData, AddChallenge, -9999, (int challengeid) -> void) {
	const int callbackid1 = 1471;
	if (CallbackState.test(callbackid1 - 1000)) {

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua::LuaCaller caller(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = caller.push(callbackid1)
			.push(challengeid)
			.push(challengeid)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}

	}


	super(challengeid);

	const int callbackid2 = 1472;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid2 - 1000)) {

		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid2)
			.push(challengeid)
			.push(challengeid)
			.call(1);

	}


}

//ModCallbacks.MC_PRE_FAMILIAR_CAN_CHARM = 1473

//MC_POST_ACHIEVEMENT UNLOCK (1476)
HOOK_METHOD_PRIORITY(PersistentGameData, TryUnlock, -9999, (int achievid) -> bool) {
	bool deed = super(achievid);
	if (deed) {
		const int callbackid1 = 1476;
		lua_State* L = g_LuaEngine->_state;
		if (CallbackState.test(callbackid1 - 1000)) {

			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(callbackid1)
				.push(achievid)
				.push(achievid)
				.call(1);

		}
	}
	return deed;
}

//HOOK_METHOD(PlayerHUD, RenderTrinket, (unsigned int slot, Vector* pos, float scale) -> void) {
//	const int callbackid = 1264;
//	if (CallbackState.test(callbackid - 1000)) {
//		lua_State* L = g_LuaEngine->_state;
//		lua::LuaStackProtector protector(L);
//
//		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
//
//		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
//			.push(slot)
//			.push(slot)
//			.pushUserdataValue(*pos, lua::Metatables::VECTOR)
//			.push(scale)
//			.push(_player, lua::Metatables::ENTITY_PLAYER)
//			.call(1);
//
//		if (!result) {
//			if (lua_istable(L, -1)) {
//				lua_pushnil(L);
//				while (lua_next(L, -2) != 0) {
//					if (lua_isstring(L, -2) && lua_isuserdata(L, -1)) {
//						const std::string key = lua_tostring(L, -2);
//						if (key == "Position") {
//							*pos = *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
//						}
//					}
//					else if (lua_isstring(L, -2) && lua_isnumber(L, -1)) {
//						const std::string key = lua_tostring(L, -2);
//						if (key == "Scale") {
//							scale = (float)lua_tonumber(L, -1);
//						}
//					}
//					lua_pop(L, 1);
//				}
//			}
//			else if (lua_isboolean(L, -1))
//			{
//				if (lua_toboolean(L, -1)) {
//					return;
//				}
//			}
//		}
//	}
//	super(slot, pos, scale);
//}

//MC_PRE/POST_MINIMAP_UPDATE (1477/1478)
HOOK_METHOD(Minimap, Update, () -> void) {
	const int precallbackid = 1477;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(precallbackid).call(1);
	}

	super();

	const int postcallbackid = 1478;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(postcallbackid).call(1);
	}
}

//MC_PRE/POST_MINIMAP_RENDER (1479/1480)
HOOK_METHOD(Minimap, Render, () -> void) {
	const int precallbackid = 1479;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(precallbackid).call(1);
	}

	super();

	const int postcallbackid = 1480;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(postcallbackid).call(1);
	}
}

//MC_PRE_PICKUP_GET_LOOT_LIST (1334)
HOOK_METHOD(Entity_Pickup, GetLootList, (bool shouldAdvance, Entity_Player* player) -> LootList) {
	LootList list = super(shouldAdvance, player);

	const int callbackid = 1334;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			//.push(&list, lua::metatables::LootListMT) //can't work with loot directly for now
			.push(this, lua::Metatables::ENTITY_PICKUP)
			.push(shouldAdvance)
			.call(1);

		if (!result && lua_isuserdata(L, -1)) {
			return *lua::GetUserdata<LootList*>(L, -1, lua::metatables::LootListMT);
		}
	}
	return list;
}

//MC_POST_PLAYER_TRIGGER_EFFECT_REMOVED (1268)
HOOK_METHOD(Entity_Player, TriggerEffectRemoved, (ItemConfig_Item* item, int unused) -> void) {
	super(item, unused);
	const int callbackid = 1268;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(item, lua::Metatables::ITEM)
			.call(1);
	}
}

//MC_POST_ROOM_TRIGGER_EFFECT_REMOVED (1269)
HOOK_METHOD(Room, TriggerEffectRemoved, (ItemConfig_Item* item, int unused) -> void) {
	super(item, unused);
	const int callbackid = 1269;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(item, lua::Metatables::ITEM)
			.call(1);
	}
}

//MC_PRE/POST_PLAYER_REVIVE (1482)
HOOK_METHOD(Entity_Player, Revive, () -> void) {
		const int precallbackid = 1481;
		if (CallbackState.test(precallbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			Entity_Player* ent = (Entity_Player*)this;

			lua::LuaResults lua_result = lua::LuaCaller(L).push(precallbackid)
				.push(this->GetPlayerType())
				.push(ent, lua::Metatables::ENTITY_PLAYER)
				.call(1);

			if (!lua_result && lua_isboolean(L, -1) && lua_toboolean(L, -1) == false) {
				return;
			}
		}

	super();

	const int postcallbackid = 1482;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		Entity_Player* ent = (Entity_Player*)this;

		lua::LuaCaller(L).push(postcallbackid)
			.push(this->GetPlayerType())
			.push(ent, lua::Metatables::ENTITY_PLAYER)
			.call(1);
	}
} 

// MC_PRE_FORTUNE_DISPLAY (1483)
HOOK_METHOD(HUD, ShowFortuneText, (int** param_1) -> void) {
	const int callbackId = 1483;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(param_1);
}

// MC_PRE_ITEM_TEXT_DISPLAY (1484) //prev: ShowItemTextCustom buw ShowItemTextCustom should work?
/*HOOK_METHOD(HUD, ShowItemText, (wchar_t* title, wchar_t* subtitle, bool isSticky, bool isCurseDisplay) -> void) {
	const int callbackId = 1484;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		int sizeNeededTitle = WideCharToMultiByte(CP_UTF8, 0, &title[0], wcslen(title), NULL, 0, NULL, NULL);
		std::string strTitle(sizeNeededTitle, 0);
		WideCharToMultiByte(CP_UTF8, 0, title, wcslen(title), &strTitle[0], sizeNeededTitle, NULL, NULL);
		 
		int sizeNeededSubtitle = WideCharToMultiByte(CP_UTF8, 0, &subtitle[0], wcslen(subtitle), NULL, 0, NULL, NULL);
		std::string strSubtitle(sizeNeededSubtitle, 0);
		WideCharToMultiByte(CP_UTF8, 0, subtitle, wcslen(subtitle), &strSubtitle[0], sizeNeededSubtitle, NULL, NULL);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(strTitle.c_str())
			.push(strSubtitle.c_str())
			.push(isSticky)
			.push(isCurseDisplay)
			.call(1);
		 
		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			} 
		} 
	}

	super(title, subtitle, isSticky, isCurseDisplay);
}
*/


//MC_POST_BOSS_INTRO_SHOW (1270)
HOOK_METHOD(RoomTransition, StartBossIntro, (unsigned int bossID1, unsigned int bossID2) -> void) {
	super(bossID1, bossID2);

	const int callbackid = 1270;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(bossID1)
			.push(bossID2)
			.call(1);
	}
}

//MC_POST_ROOM_TRANSITION_UPDATE (1271)
HOOK_METHOD(RoomTransition, Update, () -> void) {
	super();

	const int callbackid = 1271;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(_mode)
			.call(1);
	}
}

//MC_POST_ROOM_TRANSITION_RENDER (1272)
HOOK_METHOD(RoomTransition, Render, () -> void) {
	super();

	const int callbackid = 1272;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(_mode)
			.call(1);
	}
}

//MC_PRE_BOSS_SELECT (1280)
HOOK_METHOD(BossPool, GetBossId, (int leveltype, int levelvariant, RNG* unusedRNG) -> int) {
	int bossId = super(leveltype, levelvariant, unusedRNG);

	const int callbackid = 1280;
	const auto stageId = RoomConfig::GetStageID(leveltype, levelvariant, -1);

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(bossId)
			.push(bossId)
			.push(&_pool[stageId], lua::metatables::BossPoolMT)
			.push(leveltype)
			.push(levelvariant)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				bossId = (int)lua_tointeger(L, -1);
			}
		}
	}
	return bossId;
}

//PRE_GET_RANDOM_ROOM_INDEX (1290)
HOOK_METHOD(Level, GetRandomRoomIndex, (bool IAmErrorRoom, unsigned int Seed) -> int) {
	int ret = super(IAmErrorRoom, Seed);

	const int callbackid = 1290;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(ret)
			.push(IAmErrorRoom)
			.push(Seed)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return ret;
}

//PRE_FORCE_ADD_PILL_EFFECT (1128), temporarily disabled
HOOK_METHOD(ItemPool, ForceAddPillEffect, (int32_t ID)->int) {
	int ret = -1;
	int callbackid = 1128;
	// if (CallbackState.test(callbackid - 1000)) {
	// 	lua_State* L = g_LuaEngine->_state;
	// 	lua::LuaStackProtector protector(L);

	// 	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	// 	lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
	// 		.pushnil()
	// 		.push(ID)
	// 		.call(1);

	// 	if (!result) {
	// 		if (lua_isinteger(L, -1)) {
	// 			ret = (int)lua_tointeger(L, -1);
	// 		};
	// 	}
	// };

	if (ret == -1) {
		ret = super(ID);
	};

	//POST_FORCE_PILL_EFFECT (1129)
	callbackid = 1129;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(ID)
			.push(ret)
			.call(1);
	};

	return ret;
};

inline int GetGlowingHourglassSlot(GameState* gameState) { //g_Game->_currentGlowingHourglassSlot should always contain the slot that is currently being saved/restored, but I'm doing this just to be safe.
	if (gameState == &g_Game->_glowingHourglassStates[0]._gameState) {
		return 0;
	}

	if (gameState == &g_Game->_glowingHourglassStates[1]._gameState) {
		return 1;
	}

	return -1;
}

//PRE/POST_GLOWING_HOURGLASS_SAVE (1300 - 1302)
HOOK_METHOD(Game, SaveState, (GameState* gameState) -> void) {
	int currentSlot = GetGlowingHourglassSlot(gameState);

	const int preCallbackId = 1302;
	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(preCallbackId)
			.pushnil()
			.push(currentSlot)
			.call(1);
	}

	super(gameState);

	if (currentSlot == -1) {
		return;
	}

	const int postCallbackId = 1300;
	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.pushnil()
			.push(currentSlot)
			.call(1);
	}
	return;
}

//PRE/POST_GLOWING_HOURGLASS_LOAD (1301 - 1303)
HOOK_METHOD(Game, RestoreState, (GameState* gameState, bool startGame) -> void) {
	int currentSlot = GetGlowingHourglassSlot(gameState);

	if (currentSlot != -1)
	{
		const int preCallbackId = 1303;
		if (CallbackState.test(preCallbackId - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(preCallbackId)
				.pushnil()
				.push(currentSlot)
				.call(1);
		}
	}

	super(gameState, startGame);

	if (currentSlot == -1) {
		return;
	}

	const int postCallbackId = 1301;
	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.pushnil()
			.push(currentSlot)
			.call(1);
	}
	return;
}

//PRE/POST_ENTITY_SET_COLOR (1486/1487)
HOOK_METHOD(Entity, SetColor, (ColorMod* color, int duration, int priority, bool fadeOut, bool share) -> void) {
	const int preCallbackId = 1485;
	ColorMod colorCopy;

	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(preCallbackId)
			.push(this->_type)
			.push(this, lua::Metatables::ENTITY)
			.push(color, lua::Metatables::COLOR)
			.push(duration)
			.push(priority)
			.push(fadeOut)
			.push(share)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1) && !lua_toboolean(L, -1)) {
				return;
			}
			else if (lua_isuserdata(L, -1)) {
				// We need to copy the returned color in case the Lua stack is cleaned up in order to avoid unexpected behavior.
				ColorMod* returnedColor = lua::GetUserdata<ColorMod*>(L, -1, lua::Metatables::COLOR, "Color");
				colorCopy = *returnedColor;
				color = &colorCopy;
			}
		}
	}

	super(color, duration, priority, fadeOut, share);

	const int postCallbackId = 1486;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.push(this->_type)
			.push(this, lua::Metatables::ENTITY)
			.push(color)
			.push(duration)
			.push(priority)
			.push(fadeOut)
			.push(share)
			.call(1);
	}
}

inline bool IsCoopBaby(Entity_Player* player) {
	return player->_variant == 1 || player->_isCoopGhost;
}

//PRE/POST_PLAYER_ADD_COSTUME (1281/1283)
HOOK_METHOD(Entity_Player, AddCostume, (ItemConfig_Item* item, bool itemStateOnly) -> void) {
	if (IsCoopBaby(this)) {
		return;
	}

	const int preCallbackid = 1281;
	if (CallbackState.test(preCallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(preCallbackid)
			.pushnil()
			.push(item, lua::Metatables::ITEM)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(itemStateOnly)
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				auto* retItem = lua::GetUserdata<ItemConfig_Item*>(L, -1, lua::Metatables::ITEM, "Item");
				if (retItem) {
					item = retItem;
				}
			}
			else if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(item, itemStateOnly);

	const int postCallbackid = 1283;
	if (CallbackState.test(postCallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackid)
			.pushnil()
			.push(item, lua::Metatables::ITEM)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(itemStateOnly)
			.call(1);
	}
}

//PRE/POST_PLAYER_REMOVE_COSTUME (1282/1284)
HOOK_METHOD(Entity_Player, RemoveCostume, (ItemConfig_Item* item) -> void) {
	if (!item || IsCoopBaby(this)) {
		return;
	}

	const int preCallbackid = 1282;
	if (CallbackState.test(preCallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(preCallbackid)
			.pushnil()
			.push(item, lua::Metatables::ITEM)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(item);

	const int postCallbackid = 1284;
	if (CallbackState.test(postCallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackid)
			.pushnil()
			.push(item, lua::Metatables::ITEM)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
}

//MC_POST_PICKUP_SELECTION_REWORK(37)
int requestedPickupVariant = 0;
int requestedPickupSubType = 0;
uint32_t pickupInitSeed = 0;

HOOK_METHOD(Entity_Pickup, Init, (uint32_t type, int variant, int subType, uint32_t seed) -> void)
{
	requestedPickupVariant = variant;
	requestedPickupSubType = subType;
	pickupInitSeed = seed;

	super(type, variant, subType, seed);
}

HOOK_STATIC(LuaEngine, PostPickupSelection, (Entity_Pickup* pickup, int* variant, int* subType) -> void, __stdcall)
{
	const int callbackId = 37;
	lua_State* L = g_LuaEngine->_state;

	if (VanillaCallbackState.test(callbackId)) {
		RNG rng = RNG();
		rng.SetSeed(pickupInitSeed, 3); // not using 35 as it's what's used by the selection RNG

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults failure = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.push(*variant)
			.push(*subType)
			.push(requestedPickupVariant)
			.push(requestedPickupSubType)
			.push(&rng, lua::Metatables::RNG)
			.call(1);

		if (failure || !lua_istable(L, -1))
		{
			return;
		}

		lua_rawgeti(L, -1, 1);
		*variant = (int)luaL_optinteger(L, -1, *variant);
		lua_pop(L, 1);

		lua_rawgeti(L, -1, 2);
		*subType = (int)luaL_optinteger(L, -1, *subType);
		lua_pop(L, 1);
	}
}

// GET_STATUS_EFFECT_TARGET (1485)
HOOK_METHOD(Entity, GetStatusEffectTarget, () -> Entity*) {
	const int callbackid = 1485;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackid)
			.push(this->_type)
			.push(this, lua::Metatables::ENTITY)
			.call(1);

		if (!results) {
			if (lua_isuserdata(L, -1)) {
				return lua::GetUserdata<Entity*>(L, -1, lua::Metatables::ENTITY, "Entity");
			}
		}
	}
	return super();
}

// PRE/POST_STATUS_EFFECT_APPLY (1465/1466)
struct TimedOnlyStatusEffectApplyInputs {
	Entity* entity;
	EntityRef source;
	int duration;
};

void HandleTimedOnlyStatusApplyCallback(int statusId, TimedOnlyStatusEffectApplyInputs& inputs, std::function<void(const EntityRef&, int)> super)
{
	const int preCallbackId = 1465;
	
	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(preCallbackId)
			.push(statusId)
			.push(statusId)
			.push(inputs.entity, lua::Metatables::ENTITY)
			.push(&(inputs.source), lua::Metatables::ENTITY_REF)
			.push(inputs.duration)
			.call(1);

		if (!results) {
			if (lua_isinteger(L, -1)) {
				inputs.duration = (int)lua_tointeger(L, -1);
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(inputs.source, inputs.duration);

	const int postCallbackId = 1466;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.push(statusId)
			.push(statusId)
			.push(inputs.entity, lua::Metatables::ENTITY)
			.push(&(inputs.source), lua::Metatables::ENTITY_REF)
			.push(inputs.duration)
			.call(1);
	}
}

#define _APPLY_TIMED_ONLY_STATUS_EFFECT_LAMBDA() [this](const EntityRef& ref, int duration) { return super(ref, duration); }
#define HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(_method, statusId) \
HOOK_METHOD(Entity, _method, (const EntityRef& ref, int duration) -> void) { \
	TimedOnlyStatusEffectApplyInputs inputs = {this, ref, duration}; \
	HandleTimedOnlyStatusApplyCallback(statusId, inputs, _APPLY_TIMED_ONLY_STATUS_EFFECT_LAMBDA()); \
}

struct DamageStatusEffectApplyInputs {
	Entity* entity;
	EntityRef source;
	int duration;
	float damage;
};

void HandleDamageStatusApplyCallback(int statusId, DamageStatusEffectApplyInputs& inputs, std::function<void(const EntityRef&, int, float)> super)
{
	const int preCallbackId = 1465;

	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(preCallbackId)
			.push(statusId)
			.push(statusId)
			.push(inputs.entity, lua::Metatables::ENTITY)
			.push(&(inputs.source), lua::Metatables::ENTITY_REF)
			.push(inputs.duration)
			.push(inputs.damage)
			.call(1);

		if (!results) {
			if (lua_istable(L, -1)) {
				inputs.duration = lua::callbacks::ToInteger(L, 1);
				inputs.damage = (float)lua::callbacks::ToNumber(L, 2);
			}
			else if (lua_isinteger(L, -1)) {
				inputs.duration = (int)lua_tointeger(L, -1);
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(inputs.source, inputs.duration, inputs.damage);

	const int postCallbackId = 1466;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.push(statusId)
			.push(statusId)
			.push(inputs.entity, lua::Metatables::ENTITY)
			.push(&(inputs.source), lua::Metatables::ENTITY_REF)
			.push(inputs.duration)
			.push(inputs.damage)
			.call(1);
	}
}

#define _APPLY_DAMAGE_STATUS_EFFECT_LAMBDA() [this](const EntityRef& ref, int duration, float damage) { return super(ref, duration, damage); }
#define HOOK_DAMAGE_STATUS_APPLY_CALLBACKS(_method, statusId) \
HOOK_METHOD(Entity, _method, (const EntityRef& ref, int duration, float damage) -> void) { \
	DamageStatusEffectApplyInputs inputs = {this, ref, duration, damage}; \
	HandleDamageStatusApplyCallback(statusId, inputs, _APPLY_DAMAGE_STATUS_EFFECT_LAMBDA()); \
}

HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddBaited, 0);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddBleeding, 1);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddBrimstoneMark, 2);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddCharmed, 4);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddFear, 6);
//HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddFreeze, 7); requires ignoreBossStatusEffectCooldown param
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddIce, 8);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddMagnetized, 10);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddMidasFreeze, 11);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddShrink, 13);
HOOK_TIMED_ONLY_STATUS_APPLY_CALLBACKS(AddWeakness, 15);

HOOK_DAMAGE_STATUS_APPLY_CALLBACKS(AddBurn, 3);
HOOK_DAMAGE_STATUS_APPLY_CALLBACKS(AddPoison, 12);

HOOK_METHOD(Entity, AddConfusion, (const EntityRef& ref, int duration, bool ignoreBosses) -> void) {
	const int preCallbackId = 1465;
	const int statusId = 5;

	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(preCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(ignoreBosses)
			.call(1);

		if (!results) {
			if (lua_istable(L, -1)) {
				duration = lua::callbacks::ToInteger(L, 1);
				ignoreBosses = lua::callbacks::ToBoolean(L, 2);
			}
			else if (lua_isinteger(L, -1)) {
				duration = (int)lua_tointeger(L, -1);
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(ref, duration, ignoreBosses);

	const int postCallbackId = 1466;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result =  lua::LuaCaller(L).push(postCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(ignoreBosses)
			.call(1);
	}
}

HOOK_METHOD(Entity, AddKnockback, (const EntityRef& ref, const Vector& pushDirection, int duration, bool takeImpactDamage) -> void) {
	Vector pushVector(pushDirection);

	const int preCallbackId = 1465;
	const int statusId = 9;

	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(preCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(&pushVector, lua::Metatables::VECTOR)
			.push(takeImpactDamage)
			.call(1);

		if (!results) {
			if (lua_istable(L, -1)) {
				duration = lua::callbacks::ToInteger(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				pushVector = *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
				lua_pop(L, 1);

				takeImpactDamage = lua::callbacks::ToBoolean(L, 3);
			}
			else if (lua_isinteger(L, -1)) {
				duration = (int)lua_tointeger(L, -1);
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(ref, pushVector, duration, takeImpactDamage);

	const int postCallbackId = 1466;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(postCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(&pushVector, lua::Metatables::VECTOR)
			.push(takeImpactDamage)
			.call(1);
	}
}

HOOK_METHOD(Entity, AddSlowing, (const EntityRef& ref, int duration, float amount, ColorMod color) -> void) {
	const int preCallbackId = 1465;
	const int statusId = 14;

	if (CallbackState.test(preCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(preCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(amount)
			.push(&color, lua::Metatables::COLOR)
			.call(1);

		if (!results) {
			if (lua_istable(L, -1)) {
				duration = lua::callbacks::ToInteger(L, 1);
				amount = (float)lua::callbacks::ToNumber(L, 1);

				lua_pushinteger(L, 3);
				lua_gettable(L, -2);
				color = *lua::GetUserdata<ColorMod*>(L, -1, lua::Metatables::COLOR, "Color");
				lua_pop(L, 1);

			}
			else if (lua_isinteger(L, -1)) {
				duration = (int)lua_tointeger(L, -1);
			}
			else if (lua_isboolean(L, -1))
			{
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	super(ref, duration, amount, color);

	const int postCallbackId = 1466;

	if (CallbackState.test(postCallbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(postCallbackId)
			.push(statusId)
			.push(statusId)
			.push(this, lua::Metatables::ENTITY)
			.push((EntityRef*)(&ref), lua::Metatables::ENTITY_REF)
			.push(duration)
			.push(amount)
			.push(&color, lua::Metatables::COLOR)
			.call(1);
	}
}