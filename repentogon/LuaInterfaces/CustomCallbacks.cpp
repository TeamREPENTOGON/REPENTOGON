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

//Callback tracking for optimizations
std::bitset<500> CallbackState; //I dont think we will add 500 callbacks but lets set it there for now
HOOK_STATIC(Isaac,SetBuiltInCallbackState, (const int callbackid, bool enable)-> void, __cdecl){
	if (callbackid > 1000) {
		CallbackState.set(callbackid - 1000, enable);
	}
	else {
		super(callbackid, enable);
	}
}
//Callback Tracking for optimizations

extern int additiveCallbackKey;
extern int preRenderCallbackKey;

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

//HUD_POST_RENDER (id: 1022)

HOOK_METHOD(HUD, Render, () -> void) {
	const int callbackid = 1022;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);

	}
	super();
}

//HUD_POST_RENDER callback end

//Character menu render Callback(id:1023)
HOOK_METHOD(MenuManager, RenderButtonLayout, () -> void) {
	super();
	const int callbackid = 1023;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);
	}
}

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
		return super(ID, Volume, FrameDelay, Loop, Pitch, Pan);
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
			}
		}
		else if (lua_isboolean(L, -1)) {
			if (!lua_toboolean(L, -1)) { // this particular if statement can't be combined with the one above it, or it fails to register false properly
				return;
			}
		}
		else {
			super(ID, Volume, FrameDelay, Loop, Pitch, Pan);
			ProcessPostSFXPlay(ID, Volume, FrameDelay, Loop, Pitch, Pan);
		}
	}
}
//SFX_PRE/POST_PLAY callbacks end

// PRE/POST_X_COLLISION callbacks START
struct CollisionInputs {
	Entity* entity;
	Entity* collider;
	bool low;

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

	if (CallbackState.test(callbackid - 1000)) {
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
#define HOOK_COLLISION_CALLBACKS(_type, _metatable, _precallback, _postcallback) \
HOOK_METHOD(_type, HandleCollision, (Entity* collider, bool low) -> bool) { \
	CollisionInputs inputs = {this, collider, low}; \
	inputs.SetMetatable(_metatable); \
	return HandleCollisionCallbacks(inputs, _precallback, _postcallback, _COLLISION_SUPER_LAMBDA()); \
}

HOOK_COLLISION_CALLBACKS(Entity_Player, lua::Metatables::ENTITY_PLAYER, 1230, 1231)
HOOK_COLLISION_CALLBACKS(Entity_Tear, lua::Metatables::ENTITY_TEAR, 1232, 1233)
HOOK_COLLISION_CALLBACKS(Entity_Familiar, lua::Metatables::ENTITY_FAMILIAR,	1234, 1235)
HOOK_COLLISION_CALLBACKS(Entity_Bomb, lua::Metatables::ENTITY_BOMB, 1236, 1237)
HOOK_COLLISION_CALLBACKS(Entity_Pickup, lua::Metatables::ENTITY_PICKUP, 1238, 1239)
HOOK_COLLISION_CALLBACKS(Entity_Slot, lua::metatables::EntitySlotMT, 1240, 1241)
HOOK_COLLISION_CALLBACKS(Entity_Knife, lua::Metatables::ENTITY_KNIFE, 1242, 1243)
HOOK_COLLISION_CALLBACKS(Entity_Projectile, lua::Metatables::ENTITY_PROJECTILE, 1244, 1245)
HOOK_COLLISION_CALLBACKS(Entity_NPC, lua::Metatables::ENTITY_NPC, 1246, 1247)
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

//PRE/POST_ENTITY_THROW (1040/1041)
void ProcessPostEntityThrow(Vector* Velocity, Entity_Player* player, Entity* ent) {
	const int callbackid = 1041;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			//.push(ent->GetType())
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
HOOK_METHOD(Level, Init, () -> void) {
	const int callbackid = 1060;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.call(1);
	}
	super();
}
//PRE_LEVEL_INIT Callback (id: 1060 enum pending)

//PRE_TRIGGER_PLAYER_DEATH (id: 1050)
HOOK_METHOD(Entity_Player, TriggerDeath, (bool checkOnly) -> bool) {
	const int callbackid = 1050;	
	if (!checkOnly && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					this->Revive();
					*this->GetVisible() = true;
					return false;
				}
			}
		}
		return super(checkOnly);
	}
	else {
		return super(checkOnly);
	}
}
//PRE_TRIGGER_PLAYER_DEATH end

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
			.push(paws->GetANM2(), lua::Metatables::SPRITE)
			.push(paws->GetStatsANM2(), lua::Metatables::SPRITE)
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
		.push(this->GetANM2(), lua::Metatables::SPRITE)
		.push(this->GetStatsANM2(), lua::Metatables::SPRITE)
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


//(PRE_)USE_PILL (id: 1065/1001)
HOOK_METHOD(Entity_Player, UsePill, (int pillEffect, int pillColor, unsigned int useFlag) -> void) {
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
	const int callbackid2 = 1001;
	if (CallbackState.test(callbackid2 - 1000)) {
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

//GET_SHOP_ITEM_PRICE (id: 1066)
HOOK_METHOD(Room, GetShopItemPrice, (unsigned int entVariant, unsigned int entSubtype, int shopItemID) -> int) {
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
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return super(entVariant, entSubtype, shopItemID);
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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

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

bool noInfLoop = false;

//PRE_GRID_ENTITY_SPAWN (id: 1100)
HOOK_METHOD(Room, SpawnGridEntity, (int idx, unsigned int type, unsigned int variant, unsigned int seed, int vardata) -> bool) {
	noInfLoop = false;
	const int callbackid = 1100;
	if (CallbackState.test(callbackid - 1000)) {
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
			if (lua_isuserdata(L, -1)) {
				GridEntityDesc* desc = lua::GetUserdata<GridEntityDesc*>(L, -1, lua::Metatables::GRID_ENTITY_DESC, "GridEntityDesc");
				noInfLoop = true;
				return g_Game->_room->SpawnGridEntityDesc(idx, desc);
			}
			else if (lua_istable(L, -1)) {
				type = (GridEntityType)lua::callbacks::ToInteger(L, 1);
				variant = (unsigned int)lua::callbacks::ToInteger(L, 2);
				vardata = lua::callbacks::ToInteger(L, 3);
				seed = (unsigned int)lua::callbacks::ToInteger(L, 4);
			}
			else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
			{
				return false;
			}
		}
	}
	return super(idx, type, variant, seed, vardata);
}

//also PRE_GRID_ENTITY_SPAWN (id: 1100), but for the override
HOOK_METHOD(Room, SpawnGridEntityDesc, (int idx, GridEntityDesc* desc) -> bool) {
	const int callbackid = 1100;
	if (!noInfLoop) {
		if (CallbackState.test(callbackid - 1000)) {
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
					desc->_type = (GridEntityType)lua::callbacks::ToInteger(L, 1);
					desc->_variant = (unsigned int)lua::callbacks::ToInteger(L, 2);
					desc->_varData = lua::callbacks::ToInteger(L, 3);
					desc->_spawnSeed = (unsigned int)lua::callbacks::ToInteger(L, 4);
				}
				else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
				{
					return false;
				}
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

//RenderHead Callback (id: 1038)
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
				super(lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector"));
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
}

//Renderbody Callback (id: 1039)
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
				super(lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector"));
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
}

//PLAYER_GET_ACTIVE_MAX_CHARGE (id: 1072)
HOOK_OVERLOADED_METHOD(Entity_Player, GetActiveMaxCharge, int, (int, int), (int item, int vardata) -> int) {
	const int callbackid = 1072;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(item)
			.push(item)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(vardata)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return super(item, vardata);
}

//PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE (id: 1073)
HOOK_METHOD(Entity_Player, GetActiveMinUsableCharge, (int slot) -> int) {
	const int callbackid = 1073;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetActiveItem(slot))
			.push(slot)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}
	return super(slot);
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

//POST_SLOT_UPDATE (1122)
HOOK_METHOD(Entity_Slot, Update, () -> void) {
	super();
	const int callbackid = 1122;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
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
			.push(this->GetOverlayID())
			//.push(this, lua::metatables::ItemOverlayMT)
			.push(this->GetOverlayID())
			.push(unk)
			.call(1);
	}
}

//PRE/POST_ITEM_OVERLAY_SHOW (id: 1076, 1134)
HOOK_METHOD(ItemOverlay, Show, (int overlayID, int delay, Entity_Player* player) -> void) {
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
//PRE_COMPLETION_EVENT (1049) 
HOOK_STATIC(Manager, RecordPlayerCompletion, (int unk) -> void, __stdcall) {
	const int callbackid = 1049;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(unk)
			.push(unk)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(unk);
}

//POST_PLAYERHUD_RENDER_ACTIVE_ITEM (1079)
HOOK_METHOD(PlayerHUD, RenderActiveItem, (unsigned int slot, const Vector &pos, float alpha, float unk) -> void) {
	super(slot,pos, alpha, unk);

	const int callbackid = 1079;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(this->GetPlayer(), lua::Metatables::ENTITY_PLAYER)
			.push(slot)
			.pushUserdataValue(pos, lua::Metatables::VECTOR)
			.push(alpha)
			.call(1);
	}
}

//POST_PLAYERHUD_RENDER_HEARTS (1091)
HOOK_METHOD(PlayerHUD, RenderHearts, (Vector* unk1, ANM2 *sprite, const Vector &pos, float unk2) -> void) {
	super(unk1, sprite, pos, unk2);

	const int callbackid = 1091;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(unk1, lua::Metatables::VECTOR)
			.push(sprite, lua::Metatables::SPRITE)
			.pushUserdataValue(pos, lua::Metatables::VECTOR)
			.push(unk2)
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
			.push(ent->GetType())
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

		lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
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
			.push(this->GetWeaponType())
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
			lua_rawgeti(L, -1, 1);
			int level = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, 2);
			int type = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);

			logViewer.AddLog("[REPENTOGON]", "MC_PRE_SELECT_LEVEL %d %d", level, type);
			super(level, type);
			return;
		}
	}
	else {
		super(levelType, stageType);
	}
}

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

HOOK_METHOD(Level, place_room, (LevelGenerator_Room* slot, RoomConfig* config, uint32_t seed, uint32_t unk) -> bool) {
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
		
		RoomConfig* other = nullptr;
		lua::LuaResults results = caller.push(config, lua::Metatables::ROOM_CONFIG_ROOM).push(seed).call(1);
		if (lua_isuserdata(L, -1)) {
			auto opt = lua::TestUserdata<RoomConfig*>(L, -1, lua::Metatables::ROOM_CONFIG_ROOM);

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

const int coinValues[8] = { 1, 1, 5, 10, 2, 1, 5, 1 };

int FixedGetCoinValue(int subtype) {
	if (subtype > 7)
		return 1;
	else
		return coinValues[subtype];
}

HOOK_METHOD(Entity_Pickup, GetCoinValue, () -> int) {
	if (*this->GetVariant() == 20) {
		const int callbackid = 1250;
		if (CallbackState.test(callbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
				.push(*this->GetSubType())
				.push(this, lua::Metatables::ENTITY_PICKUP)
				.call(1);

			if (!result) {
				if (lua_isinteger(L, -1)) {
					return ((int)lua_tointeger(L, -1));
				}
			}
		}
		const unsigned int subtype = *this->GetSubType();
		XMLAttributes xmlData = XMLStuff.EntityData->GetNodesByTypeVarSub(5, 20, subtype, true);
		const std::string coinValue = xmlData["coinvalue"];
		
		if (isdigit(coinValue[0])) {
			return stoi(coinValue);
		}
		return FixedGetCoinValue(subtype);
	}
	// Apparently trying to hook a func with a jump in its first 5 bytes is hellish. Just return what the normal func already would have
	return 0;
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
HOOK_METHOD(Entity_Player, FireBoneClub, (Entity* parent, uint32_t variant, bool unk) -> Entity_Knife*) {
	Entity_Knife* club = super(parent, variant, unk);
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

/* ////////////////////////
// Grid Callbacks
*/ ////////////////////////

bool DoGridPreUpdateCallback(GridEntity* grid, const char* mt, int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, mt)
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

bool DoGridPreUpdateCallback(GridEntity* grid, lua::Metatables mt, int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, mt)
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

void DoPostGridCallback(GridEntity * grid, const char* mt, const int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, mt)
			.call(1);
	}
}

void DoPostGridCallback(GridEntity* grid, lua::Metatables mt, const int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, lua::metatables::GridDecorationMT)
			.call(1);
	}
}

//PRE/POST_GRID_ENTITY_DECORATION_UPDATE (1400,1401)
HOOK_METHOD(GridEntity_Decoration, Update, () -> void) {
	const int preCallbackId = 1400;
	const int postCallbackId = preCallbackId+1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridDecorationMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridDecorationMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_DOOR_UPDATE (1402,1403)
HOOK_METHOD(GridEntity_Door, Update, () -> void) {
	const int preCallbackId = 1402;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_DOOR, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_DOOR, postCallbackId);
}

//PRE/POST_GRID_ENTITY_FIRE_UPDATE (1404,1405)
HOOK_METHOD(GridEntity_Fire, Update, () -> void) {
	const int preCallbackId = 1404;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridFireMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridFireMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_GRAVITY_UPDATE (1406,1407)
HOOK_METHOD(GridEntity_Gravity, Update, () -> void) {
	const int preCallbackId = 1406;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridGravityMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridGravityMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_LOCK_UPDATE (1408,1409)
HOOK_METHOD(GridEntity_Lock, Update, () -> void) {
	const int preCallbackId = 1408;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridLockMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridLockMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_PIT_UPDATE (1410,1411)
HOOK_METHOD(GridEntity_Pit, Update, () -> void) {
	const int preCallbackId = 1410;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_PIT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_PIT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_POOP_UPDATE (1412,1413)
HOOK_METHOD(GridEntity_Poop, Update, () -> void) {
	const int preCallbackId = 1412;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_POOP, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_POOP, postCallbackId);
}

//PRE/POST_GRID_ENTITY_PRESSUREPLATE_UPDATE (1414,1415)
HOOK_METHOD(GridEntity_PressurePlate, Update, () -> void) {
	const int preCallbackId = 1414;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, postCallbackId);
}

//PRE/POST_GRID_ENTITY_ROCK_UPDATE (1416,1417)
HOOK_METHOD(GridEntity_Rock, Update, () -> void) {
	const int preCallbackId = 1416;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_ROCK, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_ROCK, postCallbackId);
}

//PRE/POST_GRID_ENTITY_SPIKES_UPDATE (1418,1419)
HOOK_METHOD(GridEntity_Spikes, Update, () -> void) {
	const int preCallbackId = 1418;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::Metatables::GRID_ENTITY_SPIKES, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_SPIKES, postCallbackId);
}

//PRE/POST_GRID_ENTITY_STATUE_UPDATE (1420,1421)
HOOK_METHOD(GridEntity_Statue, Update, () -> void) {
	const int preCallbackId = 1420;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridStatueMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridStatueMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_TELEPORTER_UPDATE (1422,1423)
HOOK_METHOD(GridEntity_Teleporter, Update, () -> void) {
	const int preCallbackId = 1422;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridTeleporterMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridTeleporterMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_STATUE_UPDATE (1424,1425)
HOOK_METHOD(GridEntity_TNT, Update, () -> void) {
	const int preCallbackId = 1424;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridTNT_MT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridTNT_MT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_STATUE_UPDATE (1426,1427)
HOOK_METHOD(GridEntity_TrapDoor, Update, () -> void) {
	const int preCallbackId = 1426;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridTrapDoorMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridTrapDoorMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_STATUE_UPDATE (1428,1429)
HOOK_METHOD(GridEntity_Web, Update, () -> void) {
	const int preCallbackId = 1428;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreUpdateCallback(this, lua::metatables::GridWebMT, preCallbackId)) {
		return;
	}

	super();

	DoPostGridCallback(this, lua::metatables::GridWebMT, postCallbackId);
}

struct GridRenderCallback {
	int id;
	const char* cmt;
};

const GridRenderCallback gridRenderCallbacks[28] = {
	{NULL, NULL}, // GRID_NULL
	{NULL, NULL}, // GRID_DECORATION
	{NULL, NULL}, // GRID_ROCK
	{NULL, NULL}, // GRID_ROCKB
	{NULL, NULL}, // GRID_ROCKT
	{NULL, NULL}, // GRID_ROCK_BOMB
	{NULL, NULL}, // GRID_ROCK_ALT
	{NULL, NULL}, // GRID_PIT
	{1432, NULL}, // GRID_SPIKES
	{1432, NULL}, // GRID_SPIKES_ONOFF
	{1434, lua::metatables::GridWebMT}, // GRID_SPIDERWEB
	{NULL, NULL}, // GRID_LOCK
	{1436, lua::metatables::GridTNT_MT}, // GRID_TNT
	{NULL, NULL}, // GRID_FIREPLACE
	{NULL, NULL}, // GRID_POOP
	{NULL, NULL}, // GRID_WALL
	{NULL, NULL}, // GRID_DOOR
	{1438, lua::metatables::GridTrapDoorMT}, // GRID_TRAPDOOR
	{1440, lua::metatables::GridStairsMT}, // GRID_STAIRS
	{NULL, NULL}, // GRID_GRAVITY
	{NULL, NULL}, // GRID_PRESSURE_PLATE
	{NULL, NULL}, // GRID_STATUE
	{NULL, NULL}, // GRID_ROCK_SS
	{NULL, NULL}, // GRID_TELEPORTER
	{NULL, NULL}, // GRID_PILLAR
	{NULL, NULL}, // GRID_ROCK_SPIKED
	{NULL, NULL}, // GRID_ROCK_ALT2
	{NULL, NULL}, // GRID_ROCK_GOLD
};

bool DoGridPreRenderCallback(GridEntity* grid, Vector& offset, const char* mt, int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, mt)
			.pushUserdataValue(offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return false;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = offset + *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}

	return true;
}

bool DoGridPreRenderCallback(GridEntity* grid, Vector& offset, lua::Metatables mt, int id) {
	if (CallbackState.test(id - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, preRenderCallbackKey);

		lua::LuaResults result = lua::LuaCaller(L).push(id)
			.push(grid->GetDesc()->_type)
			.push(grid, mt)
			.pushUserdataValue(offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return false;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = offset + *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
			}
		}
	}

	return true;
}
//PRE/POST_GRID_ENTITY_[x]_RENDER(1432-1441)
HOOK_METHOD(GridEntity, Render, (Vector& offset) -> void) {
	int preCallbackId, postCallbackId;
	GridEntityType gridType = (GridEntityType)this->GetDesc()->_type;
	if (gridRenderCallbacks[gridType].id == NULL)
		return;

	preCallbackId = gridRenderCallbacks[gridType].id;
	postCallbackId = preCallbackId + 1;

	if (gridType == GRID_SPIKES || gridType == GRID_SPIKES_ONOFF) {

		if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_SPIKES, preCallbackId))
			return;

		super(offset);

		DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_SPIKES, postCallbackId);
	}
	else
	{
		if (!DoGridPreRenderCallback(this, offset, gridRenderCallbacks[gridType].cmt, preCallbackId))
			return;

		super(offset);

		DoPostGridCallback(this, gridRenderCallbacks[gridType].cmt, postCallbackId);
	}
}

//PRE/POST_GRID_ENTITY_DECORATION_RENDER (1444,1445)
HOOK_METHOD(GridEntity_Decoration, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1444;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::metatables::GridDecorationMT, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::metatables::GridDecorationMT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_DOOR_RENDER (1446,1447)
HOOK_METHOD(GridEntity_Door, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1446;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_DOOR, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_DOOR, postCallbackId);
}

//PRE/POST_GRID_ENTITY_FIRE_RENDER (1448,1449)
HOOK_METHOD(GridEntity_Fire, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1448;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::metatables::GridFireMT, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::metatables::GridFireMT, postCallbackId);
}

/* this is causing locks to turn into teleporters on reentering a room???????????????????????
//PRE/POST_GRID_ENTITY_LOCK_RENDER (1450,1451)
//PRE/POST_GRID_ENTITY_TELEPRTER_RENDER (1452,1453)
HOOK_METHOD(GridEntity_Lock, Render, (Vector& offset) -> void) {
	bool isTeleporter = this->GetDesc()->_type = 23;
	const char* mt = isTeleporter ? lua::metatables::GridTeleporterMT : lua::metatables::GridLockMT;
	int preCallbackId = isTeleporter ? 1450 : 1452;
	int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, mt, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, mt, postCallbackId);
}
*/

//PRE/POST_GRID_ENTITY_PIT_RENDER (1454,1455)
HOOK_METHOD(GridEntity_Pit, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1454;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_PIT, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_PIT, postCallbackId);
}

//PRE/POST_GRID_ENTITY_POOP_RENDER (1456,1457)
HOOK_METHOD(GridEntity_Poop, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1456;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_POOP, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_POOP, postCallbackId);
}

//PRE/POST_GRID_ENTITY_ROCK_RENDER (1458,1459)
HOOK_METHOD(GridEntity_Rock, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1458;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_ROCK, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_ROCK, postCallbackId);
}

//PRE/POST_GRID_ENTITY_PRESSUREPLATE_RENDER (1460,1461)
HOOK_METHOD(GridEntity_PressurePlate, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1460;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, postCallbackId);
}

//PRE/POST_GRID_ENTITY_WALL_RENDER (1462,1463)
HOOK_METHOD(GridEntity_Wall, Render, (Vector& offset) -> void) {
	const int preCallbackId = 1462;
	const int postCallbackId = preCallbackId + 1;

	if (!DoGridPreRenderCallback(this, offset, lua::metatables::GridWallMT, preCallbackId))
		return;

	super(offset);

	DoPostGridCallback(this, lua::metatables::GridWallMT, postCallbackId);
}

HOOK_METHOD(LuaCallbackCaller, CallInputAction, (LuaEngine* engine, Entity* entity, int hook, int action) -> LuaCallbackCallerResult) {
	int repentogonCallbackId = 1464;
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

HOOK_METHOD(PlayerHUD, RenderTrinket, (unsigned int slot, Vector* pos, float scale) -> void) {
	const int callbackid = 1264;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(slot)
			.push(slot)
			.pushUserdataValue(*pos, lua::Metatables::VECTOR)
			.push(scale)
			.push(_player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2) && lua_isuserdata(L, -1)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Position") {
							*pos = *lua::GetUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
						}
					}
					else if (lua_isstring(L, -2) && lua_isnumber(L, -1)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Scale") {
							scale = (float)lua_tonumber(L, -1);
						}
					}
					lua_pop(L, 1);
				}
			}
			else if (lua_isboolean(L, -1))
			{
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}
	super(slot, pos, scale);
}
