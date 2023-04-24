#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//Callback tracking for optimizations
std::bitset<500> CallbackState; //I dont think we will add 500 callbacks but lets set it there for now
HOOK_STATIC(Isaac,SetBuiltInCallbackState, (int callbackid, bool enable)-> void, __cdecl){
	if (callbackid > 1000) {
		CallbackState.set(callbackid - 1000, enable);
	}
	else {
		super(callbackid, enable);
	}
}
//Callback Tracking for optimizations


//AddCollectible Callback (id: 1004 enum pending)
HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	int callbackid = 1004; 
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
					result[i - 1] = lua_tointeger(L, -1); //I only need ints here, otherwise I'd need to check the type
					lua_pop(L, 1);
				}
				super(result[0], result[1], result[2], result[3], result[4]);
				return;
			}
			else if (lua_isinteger(L, -1))
			{
				super(lua_tointeger(L, -1), charge, firsttime, slot, vardata);
				return;
			}
		}
	}
	super(type, charge, firsttime, slot, vardata);
}
//AddCollectible Callback (id: 1004 enum pending)

//1005 RESERVED - POST_ADD_COLLECTIBLE

//POST_TAKE_DMG callback (id: 1006 enum pending)
void ProcessPostDamageCallback(Entity* ent, float damage, unsigned __int64 damageFlags, EntityRef* source, int damageCountdown) {
	int callbackid = 1006;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;

		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L)
			.push(callbackid)
			.push(*ent->GetType())
			.push(ent, lua::Metatables::ENTITY)
			.push(damage)
			.push(damageFlags)
			.push(source, lua::Metatables::ENTITY_REF)
			.push(damageCountdown)
			.call(1); // Sylmir, note: the original code asked for a single return value but never used it
	}
};

HOOK_METHOD(Entity, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef* source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if (result) ProcessPostDamageCallback(ent, damage, damageFlags, source, damageCountdown);
	return result;
}

HOOK_METHOD(Entity_Player, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef* source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if (result) ProcessPostDamageCallback(ent, damage, damageFlags, source, damageCountdown);
	return result;
}
//POST_TAKE_DMG callback end

//GRID_ROCK_UPDATE (id: 1010)

void ProcessGridRockUpdate(GridEntity_Rock* gridRock, int variant) {
	int callbackid = 1010;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(gridRock, lua::Metatables::GRID_ENTITY_ROCK)
			.pushnil()
			.push(variant)
			.call(1); // Sylmir note: original version said there was 1 result on the stack
	}
}

HOOK_METHOD(GridEntity_Rock, Update, () -> void) {
	GridEntity_Rock* rockGridEnt = (GridEntity_Rock*)this;

	int variant = this->_variant;
	ProcessGridRockUpdate(this, variant);
	super();
}

//GRID_ROCK_UPDATE callback end

//_UPDATE (id: 1020)

HOOK_METHOD(HUD, Update, () -> void) {
	int callbackid = 1020;
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
	int callbackid = 1021;
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
	int callbackid = 1022;
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
HOOK_METHOD(MenuManager, Render, () -> void) {
	super();
	int callbackid = 1023;
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
	int callbackid = 1031;
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
	int callbackid = 1030;
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
				Volume = lua::callbacks::ToNumber(L, 2);
				FrameDelay = lua::callbacks::ToInteger(L, 3);
				Loop = lua::callbacks::ToBoolean(L, 4);
				Pitch = lua::callbacks::ToNumber(L, 5);
				Pan = lua::callbacks::ToNumber(L, 6);
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

//PRE/POST_ENTITY_THROW (1040/1041)
void ProcessPostEntityThrow(Vector* Velocity, Entity_Player* player, Entity* ent) {
	int callbackid = 1041;
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
			Velocity = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
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
	int callbackid = 1042;
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

//PRE_ROOM_EXIT (1043) (currently using Entity_Player::TriggerRoomExit as the base)
HOOK_METHOD(Entity_Player, TriggerRoomExit, (bool unk) -> void) {
	int callbackid = 1043;
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
	int callbackid = 1034;
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
				int tablesize = lua_rawlen(L, -1);
				if (tablesize == 2) {
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToNumber(L, 2));
					return;
				}
			}
			else if (lua_isinteger(L, -1)) {
				super(lua_tointeger(L, -1), volume);
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
	int callbackid = 1034;
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
				int tablesize = lua_rawlen(L, -1);
				if (tablesize == 2) {
					super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToNumber(L, 2));
					return;
				}
			}
			else if (lua_isinteger(L, -1)) {
				super(lua_tointeger(L, -1), faderate);
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

//PRE_LEVEL_INIT Callback (id: 1060 enum pending)
HOOK_METHOD(Level, Init, () -> void) {
	int callbackid = 1060;
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
	int callbackid = 1050;	
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
	int callbackid = 1070;
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
	int callbackid = 1071;
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
	int callbackid = 1061;
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
	int callbackid = 1214;
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
	int callbackid = 1212;
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
				int tablesize = lua_rawlen(L, -1);
				if (tablesize == 4) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), lua::callbacks::ToNumber(L, 4));
					PostNPCMorph(this, pretype, prevar, presub);
					return;
				}
				else if (tablesize == 3) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), Championid);
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
	int callbackid = 1215;
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
	int callbackid = 1213;
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
				int tablesize = lua_rawlen(L, -1);
				if (tablesize == 6) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), lua::callbacks::ToBoolean(L, 4), lua::callbacks::ToBoolean(L, 5), lua::callbacks::ToBoolean(L, 6));
					PostPickupMorph(this, pretype, prevar, presub, KeepPrice, KeepSeed, IgnoreModifiers);
					return;
				}
				else if (tablesize == 3) {
					int pretype = *(this->GetType());
					int prevar = *(this->GetVariant());
					int presub = *(this->GetSubType());
					super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), KeepPrice, KeepSeed, IgnoreModifiers);
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
	int callbackid = 1217;
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
	int callbackid = 1216;
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
	int callbackid = 1219;
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
	int callbackid = 1218;
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
	int callbackid = 1062;
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
	int callbackid = 1063;
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
				return lua_tointeger(L, -1);
			}

		}
	}
	return super();
}

//PRE_USE_CARD (id: 1064)
HOOK_METHOD(Entity_Player, UseCard, (int cardType, unsigned int useFlag) -> void) {
	int callbackid = 1064;
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
	int callbackid = 1065;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
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
	callbackid = 1001;
	if (CallbackState.test(callbackid - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
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
	int callbackid = 1066;
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
				return lua_tointeger(L, -1);
			}
		}
	}
	return super(entVariant, entSubtype, shopItemID);
}

//PLAYER_GET_HEALTH_TYPE (id: 1067)
HOOK_METHOD(Entity_Player, GetHealthType, () -> int) {
	int callbackid = 1067;
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
				return lua_tointeger(L, -1);
			}
		}
	}
	return super();
}
//PRE_FAMILIAR_RENDER (id: 1080)
HOOK_METHOD(Entity_Familiar, Render, (Vector* offset) -> void) {
	int callbackid = 1080;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_NPC_RENDER (id: 1081)
HOOK_METHOD(Entity_NPC, Render, (Vector* offset) -> void) {
	int callbackid = 1081;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_PLAYER_RENDER (id: 1082)
HOOK_METHOD(Entity_Player, Render, (Vector* offset) -> void) {
	int callbackid = 1082;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_PICKUP_RENDER (id: 1083)
HOOK_METHOD(Entity_Pickup, Render, (Vector* offset) -> void) {
	int callbackid = 1083;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_TEAR_RENDER (id: 1084)
HOOK_METHOD(Entity_Tear, Render, (Vector* offset) -> void) {
	int callbackid = 1084;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_PROJECTILE_RENDER (id: 1085)
HOOK_METHOD(Entity_Projectile, Render, (Vector* offset) -> void) {
	int callbackid = 1085;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_KNIFE_RENDER (id: 1086)
HOOK_METHOD(Entity_Knife, Render, (Vector* offset) -> void) {
	int callbackid = 1086;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_EFFECT_RENDER (id: 1087)
HOOK_METHOD(Entity_Effect, Render, (Vector* offset) -> void) {
	int callbackid = 1087;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE_BOMB_RENDER (id: 1088)
HOOK_METHOD(Entity_Bomb, Render, (Vector* offset) -> void) {
	int callbackid = 1088;
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
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
}

//PRE/POST_SLOT_RENDER (id: 1089/1090)
HOOK_METHOD(Entity_Slot, Render, (Vector* offset) -> void) {
	int callbackid = 1089;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isuserdata(L, -1)) {
				offset = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(offset);
	callbackid = 1090;
	if (CallbackState.test(callbackid - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.pushUserdataValue(*offset, lua::Metatables::VECTOR)
			.call(1);
	}
}

//RenderHead Callback (id: 1038)
HOOK_METHOD(Entity_Player, RenderHead, (Vector* x) -> void) {
	int callbackid = 1038;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(x, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				Vector* newpos = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
				super(newpos);
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
	int callbackid = 1039;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(this->GetPlayerType())
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(x, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isuserdata(L, -1)) {
				Vector* newpos = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
				super(newpos);
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
	int callbackid = 1068;
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
	int callbackid = 1069;
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
HOOK_METHOD(Entity_Player, GetActiveMaxCharge, (int item, int vardata) -> int) {
	int callbackid = 1072;
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
				return lua_tointeger(L, -1);
			}
		}
	}
	return super(item, vardata);
}

//PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE (id: 1073)
HOOK_METHOD(Entity_Player, GetActiveMinUsableCharge, (int slot) -> int) {
	int callbackid = 1073;
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
				return lua_tointeger(L, -1);
			}
		}
	}
	return super(slot);
}

//MC_PRE_REPLACE_SPRITESHEET (id: 1100)
HOOK_METHOD(ANM2, ReplaceSpritesheet, (int LayerID, IsaacString& PngFilename) -> void) {
	int callbackid = 1100;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(_filename.Get())
			.push(LayerID)
			.push(PngFilename.Get())
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				LayerID = lua::callbacks::ToInteger(L, 1);

				const char* filename = lua::callbacks::ToString(L, 2);
				if (strlen(filename) < 16) {
					strcpy(PngFilename.text, filename);
				}
				else {
					*(char**)PngFilename.text = (char*)filename;
				}
				PngFilename.unk = PngFilename.size = strlen(filename);
			}
		}
	}
	super(LayerID, PngFilename);
	callbackid = 1101;
	if (CallbackState.test(callbackid - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
			.push(_filename.Get())
			.push(LayerID)
			.push(PngFilename.Get())
			.call(1);
	}
}

//PLAYER_GET_HEART_LIMIT (id: 1074)
HOOK_METHOD(Entity_Player, GetHealthLimit, (bool keeper) -> int) {
	int callbackid = 1074;
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
				return lua_tointeger(L, -1);
			}
		}
	}
	return super(keeper);
}

//PRE_SLOT_COLLIDE (1120)
HOOK_METHOD(Entity_Slot, HandleCollision, (Entity* collider, bool Low) -> bool) {
	int callbackid = 1120;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.push(collider, lua::Metatables::ENTITY)
			.push(Low)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return lua_toboolean(L, -1);
			}
		}
	}
	return super(collider, Low);
}

//POST_SLOT_INIT (1121)
HOOK_METHOD(Entity_Slot, Init, (int Type, int Variant, int SubType, int Seed) -> void) {
	super(Type, Variant, SubType, Seed);
	int callbackid = 1121;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(Variant)
			.push(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

//POST_SLOT_UPDATE (1122)
HOOK_METHOD(Entity_Slot, Update, () -> void) {
	super();
	int callbackid = 1122;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

//PRE/POST_SLOT_CREATE_EXPLOSION_DROPS (1123/1124)
HOOK_METHOD(Entity_Slot, CreateDropsFromExplosion, () -> void) {
	int callbackid = 1123;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
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
	callbackid = 1124;
	if (CallbackState.test(callbackid - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.call(1);
	}
}

//PRE/POST_SLOT_SET_PRIZE_COLLECTIBLE (1125/1126)
HOOK_METHOD(Entity_Slot, SetPrizeCollectible, (int id) -> void) {
	int callbackid = 1125;
	lua_State* L = g_LuaEngine->_state;
	if (CallbackState.test(callbackid - 1000)) {
		
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.push(id)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isnumber(L, -1)) {
				id = lua_tonumber(L, -1);
			}
		}
	}
	super(id);
	callbackid = 1126;
	if (CallbackState.test(callbackid - 1000)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults postResult = lua::LuaCaller(L).push(callbackid)
			.push(*this->GetVariant())
			.push(this, lua::metatables::EntitySlotMT)
			.push(id)
			.call(1);
	}
}

//POST_ITEM_OVERLAY_UPDATE (id: 1075)
HOOK_METHOD(ItemOverlay, Update, (bool unk) -> void) {
	super(unk);
	int callbackid = 1075;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			//.push(this->GetSprite(), lua::Metatables::SPRITE)
			.call(1);
	}
}

//PRE_ITEM_OVERLAY_SHOW (id: 1076)
HOOK_METHOD(ItemOverlay, Show, (int overlayID, int unk, Entity_Player* player) -> void) {
	int callbackid = 1076;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(overlayID)
			.push(overlayID)
			.push(unk)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					return;
				}
			}
			else if (lua_isnumber(L, -1)) {
				overlayID = lua_tonumber(L, -1);
			}
		}
	}
	super(overlayID, unk, player);
}

//POST_PLAYER_NEW_ROOM_TEMP_EFFECTS (id: 1077)
HOOK_METHOD(Entity_Player, TriggerNewRoom_TemporaryEffects, () -> void) {
	super();

	int callbackid = 1077;
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
	int callbackid = 1078;
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
	int callbackid = 1333;
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

//PRE_COMPLETION_MARK_GET (1047) 
HOOK_METHOD(Manager, RecordPlayerCompletion, (int unk) -> void) {
	int callbackid = 1047;
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

	int callbackid = 1079;
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

	int callbackid = 1091;
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
	int callbackid = 1150;
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
			return lua_tonumber(L, -1);
		}
	}
	else return originalAlpha;
}

//MC_PRE_GRID_LIGHTING_RENDER
HOOK_METHOD(Room, RenderGridLight, (GridEntity* grid, Vector& offset) -> void) {
	int callbackid = 1151;
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
				offset = **(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(grid, offset);
}

//MC_PRE_ENTITY_LIGHTING_RENDER
HOOK_METHOD(Room, RenderEntityLight, (Entity* ent, Vector& offset) -> void) {
	int callbackid = 1152;
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
				offset = **(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			}
		}
	}
	super(ent, offset);
}

//PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM (1092)
HOOK_METHOD(Entity_Player, GetCollectibleNum, (int CollectibleID, bool OnlyCountTrueItems) -> int) {
	int callbackid = 1092;
	int originalCount = super(CollectibleID, OnlyCountTrueItems);
	int modCount = 0;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_getglobal(L, "Isaac");
		lua_getfield(L, -1, "RunAdditiveCallback");
		lua_remove(L, lua_absindex(L, -2));
		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(modCount)
			.push(this, lua::Metatables::ENTITY_PLAYER)
			.push(CollectibleID)
			.push(OnlyCountTrueItems)
			.call(1);

		if (!result) {
			if (lua_isinteger(L, -1)) {
				modCount = lua_tointeger(L, -1);
			}
		}
	}
	return originalCount + modCount;
}

//PRE_PLAYER_HAS_COLLECTIBLE (1093) [currently lags with enabled debug 12]
HOOK_METHOD(Entity_Player, HasCollectible, (int CollectibleID, bool OnlyCountTrueItems) -> bool) {
	int callbackid = 1093;

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

//PRE_MUSIC_PLAY_JINGLE (1094)
HOOK_METHOD(Music, PlayJingle, (int musicId, int unusedInt, bool unusedBool) -> void) {
	int callbackid = 1094;

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
				super(lua_tointeger(L, -1), unusedInt, unusedBool);
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
	int callbackid = 1095;
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
	int callbackid = 1096;
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
	int callbackid = 1097;
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