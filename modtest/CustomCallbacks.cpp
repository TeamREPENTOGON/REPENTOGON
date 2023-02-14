#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//AddCollectible Callback (id: 1004 enum pending)
HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	printf("item get\n");
	lua_State *L = g_LuaEngine->_state;
	
	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1004); //parameters (1004 is the callback id)
	lua_pushinteger(L, type); 
	lua_pushinteger(L, charge);
	lua_pushboolean(L, firsttime);
	lua_pushinteger(L, slot);
	lua_pushinteger(L, vardata);

	if ((!lua_pcall(L, 6, 1, 0)) && (lua_istable(L, -1))) { // 6 params 
		printf("Additem callback run \n");
		int tablesize = lua_rawlen(L, -1);
		if (tablesize == 5){ 
		int* result = new int[tablesize];
		printf("tablesize: %d\n",tablesize );
		for (int i = 1; i <= tablesize; i++) {
			lua_pushinteger(L, i); 
			lua_gettable(L, -2);
			result[i-1] = lua_tointeger(L, -1); //I only need ints here, otherwise I'd need to check the type
			printf("V: %d\n", result[i-1]);
			lua_pop(L, 1);
		}
		super(result[0], result[1], result[2], result[3], result[4]);
		return;
		}
	}
	super(type, charge, firsttime, slot, vardata);
}
//AddCollectible Callback (id: 1004 enum pending)

//1005 RESERVED - POST_ADD_COLLECTIBLE

//POST_TAKE_DMG callback (id: 1006 enum pending)
void ProcessPostDamageCallback(Entity* ent, float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) {

	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1006);
	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
	lua_pushnumber(L, damage);
	lua_pushinteger(L, damageFlags);
	lua::luabridge::UserdataPtr::push(L, source, lua::GetMetatableKey(lua::Metatables::ENTITY_REF));
	lua_pushinteger(L, damageCountdown);

	lua_pcall(L, 6, 1, 0);

};

HOOK_METHOD(Entity, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if (result) ProcessPostDamageCallback(ent, damage, damageFlags, source, damageCountdown);
	return result;
}

HOOK_METHOD(Entity_Player, TakeDamage, (float damage, unsigned __int64 damageFlags, EntityRef *source, int damageCountdown) -> bool) {
	bool result = super(damage, damageFlags, source, damageCountdown);
	Entity* ent = (Entity*)this;

	if(result) ProcessPostDamageCallback(ent, damage, damageFlags, source, damageCountdown);
	return result;
}
//POST_TAKE_DMG callback end

//GRID_ROCK_UPDATE (id: 1010)

void ProcessGridRockUpdate(GridEntity_Rock* gridRock, int variant) {
	lua_State *L = g_LuaEngine ->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1010);

	lua::luabridge::UserdataPtr::push(L, gridRock, lua::GetMetatableKey(lua::Metatables::GRID_ENTITY_ROCK));

	lua_pushinteger(L, variant);

	lua_pcall(L, 3, 1, 0);

}

HOOK_METHOD(GridEntity_Rock, Update, () -> void) {
	GridEntity_Rock* rockGridEnt = (GridEntity_Rock*)this;

	int variant = this->_variant;
	ProcessGridRockUpdate(this, variant);
	super();
}

//GRID_ROCK_UPDATE callback end

//HUD_UPDATE (id: 1020)

HOOK_METHOD(HUD, Update, () -> void) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1020);

	lua_pcall(L, 1, 1, 0);

	super();
}

//HUD_UPDATE callback end

//HUD_POST_UPDATE (id: 1021)

HOOK_METHOD(HUD, PostUpdate, () -> void) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1021);

	lua_pcall(L, 1, 1, 0);

	super();
}

//HUD_POST_UPDATE callback end

//HUD_POST_UPDATE (id: 1022)

HOOK_METHOD(HUD, Render, () -> void) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1022);

	lua_pcall(L, 1, 1, 0);

	super();
}

//HUD_POST_UPDATE callback end

//Character menu render Callback(id:1023)
HOOK_METHOD(Menu_Character, Render, () -> void) {
	super();
	lua_State* L = g_LuaEngine->_state;
	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1023);
	lua_pcall(L, 1, 1, 0);
}

//Character menu render Callback end

//SFX_PRE/POST_PLAY (id: 1030/1031)
void ProcessPostSFXPlay(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
{
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1031);
	lua_pushinteger(L, ID);
	lua_pushnumber(L, Volume);
	lua_pushinteger(L, FrameDelay);
	lua_pushboolean(L, Loop);
	lua_pushnumber(L, Pitch);
	lua_pushnumber(L, Pan);

	lua_pcall(L, 7, 1, 0);

}

HOOK_METHOD(SFXManager, Play, (int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan) -> void) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1030);
	lua_pushinteger(L, ID);
	lua_pushnumber (L, Volume);
	lua_pushinteger(L, FrameDelay);
	lua_pushboolean(L, Loop);
	lua_pushnumber (L, Pitch);
	lua_pushnumber (L, Pan);

	if (!lua_pcall(L, 7, 1, 0)) { // is this if statement even necessary? seems to run this code regardless
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
void ProcessPostEntityThrow(Vector* Velocity, Entity* ent) {
	lua_State* L = g_LuaEngine->_state;
	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1041);
	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), *Velocity);

	lua_pcall(L, 3, 1, 0);
}

HOOK_METHOD(Entity_Player, ThrowHeldEntity, (Vector* Velocity) -> Entity*) {

	std::string err = "test"; 
	g_Game->_console.PrintError(err);  // crashes as well :(

	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1040);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), *Velocity);
	
	int pcallRes = lua_pcall(L, 2, 1, 0);
	if (!pcallRes) {
		if (lua_isuserdata(L, -1)) {
			Velocity = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			Entity* res = super(Velocity);
			ProcessPostEntityThrow(Velocity, res);
			return res;
		}
		else {
			Entity* res = super(Velocity);
			ProcessPostEntityThrow(Velocity, res);
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
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");
	lua_pushinteger(L, 1042);

	Entity_Player* ent = (Entity_Player*)this;
	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	lua_pcall(L, 2, 1, 0);
	super();
}

//PRE_ROOM_EXIT (1043) (currently using Entity_Player::TriggerRoomExit as the base)
HOOK_METHOD(Entity_Player, TriggerRoomExit, (bool unk) -> void) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");
	lua_pushinteger(L, 1043);

	Entity_Player* ent = (Entity_Player*)this;
	lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	lua_pushboolean(L, unk);
	lua_pcall(L, 3, 1, 0);
	super(unk);
}

//PRE_MUSIC_PLAY Callback (id: 1034 enum pending)
HOOK_METHOD(Music, Play, (int musicid, float volume) -> void) {
	printf("music plays\n");
	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1034); //parameters (1004 is the callback id)
	lua_pushinteger(L, musicid);
	lua_pushnumber(L, volume);
	lua_pushboolean(L, false); //isfade

	if (!lua_pcall(L, 4, 1, 0)) { // 6 params 
		if (lua_istable(L, -1)) {
			printf("Music callback run \n");
			int tablesize = lua_rawlen(L, -1);
			if (tablesize == 2) {
				super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToNumber(L, 2));
				return;
			}
		}else if (lua_isinteger(L, -1)) {
			printf("Music callback run \n");
			super(lua_tointeger(L, -1), volume);
			return;
		}else if(lua_isboolean(L, -1)){
			printf("Music callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(musicid, volume);
}
HOOK_METHOD(Music, Crossfade, (int musicid, float faderate) -> void) {
	printf("music fades\n");
	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1034); //parameters (1004 is the callback id)
	lua_pushinteger(L, musicid);
	lua_pushnumber(L, faderate);
	lua_pushboolean(L, true); //isfade

	if (!lua_pcall(L, 4, 1, 0)) { // 6 params 
		if (lua_istable(L, -1)) {
			printf("Music callback run \n");
			int tablesize = lua_rawlen(L, -1);
			if (tablesize == 2) {
				super(lua::callbacks::ToInteger(L, 1), lua::callbacks::ToNumber(L, 2));
				return;
			}
		}
		else if (lua_isinteger(L, -1)) {
			printf("Music callback run \n");
			super(lua_tointeger(L, -1), faderate);
			return;
		}
		else if (lua_isboolean(L, -1)) {
			printf("Music callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(musicid, faderate);
}
//PRE_MUSIC_PLAY Callback (id: 1034 enum pending)

//PRE_LEVEL_INIT Callback (id: 1060 enum pending)
HOOK_METHOD(Level, Init, () -> void) {
	printf("Stage Init \n");
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");
	lua_pushinteger(L, 1060);
	lua_pcall(L, 1, 1, 0);
	super();
}
//PRE_LEVEL_INIT Callback (id: 1060 enum pending)

//PRE_TRIGGER_PLAYER_DEATH (id: 1050)
HOOK_METHOD(Entity_Player, TriggerDeath, (bool checkOnly) -> bool) {
	lua_State* L = g_LuaEngine->_state;

	if (!checkOnly) {
		lua_getglobal(L, "Isaac");
		lua_getfield(L, -1, "RunCallback");

		lua_pushinteger(L, 1050);
		lua::luabridge::UserdataPtr::push(L, this, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

		if (!lua_pcall(L, 2, 1, 0)) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					this->Revive();
					this->_visible = true;
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
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1070);
	lua_pushboolean(L, Partial);

	if (!lua_pcall(L, 2, 1, 0)) {
		if (lua_isboolean(L, -1)) {
			if (!lua_toboolean(L, -1)) {
				return false;
			}
		}
	}
	return true;
}

void ProcessPostRestockCallback(bool Partial) {
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1071);
	lua_pushboolean(L, Partial);

	lua_pcall(L, 2, 1, 0);
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
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");
	lua_pushinteger(L, 1061);

	lua_pushinteger(L, roomId);
	lua_pushinteger(L, dimension);

	if (!lua_pcall(L, 3, 1, 0)) {
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


//Pre_Morph Callbacks (id:1080)
HOOK_METHOD(Entity_NPC, Morph, (int EntityType, int Variant, int SubType, int Championid) -> void) {
	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 2012);
	lua::luabridge::UserdataPtr::push(L, this, lua::GetMetatableKey(lua::Metatables::ENTITY_NPC));
	lua_pushinteger(L, EntityType);
	lua_pushinteger(L, Variant);
	lua_pushinteger(L, SubType);
	lua_pushinteger(L, Championid);

	if (!lua_pcall(L, 6, 1, 0)) {
		if (lua_istable(L, -1)) {
			printf("NPC Morph callback run \n");
			int tablesize = lua_rawlen(L, -1);
			if (tablesize == 4) {				
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), lua::callbacks::ToNumber(L, 4));
				return;
			}
			else if (tablesize == 3) {
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), Championid);
				return;
			}
		}
		else if (lua_isboolean(L, -1)) {
			printf("NPC Morph callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(EntityType, Variant, SubType, Championid);
}


HOOK_METHOD(Entity_Pickup, Morph, (int EntityType, int Variant, int SubType, bool KeepPrice, bool KeepSeed, bool IgnoreModifiers) -> void) {
	printf("Pickup Morphed \n");
	lua_State *L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 2013);
	lua::luabridge::UserdataPtr::push(L, this, lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));
	lua_pushinteger(L, EntityType);
	lua_pushinteger(L, Variant);
	lua_pushinteger(L, SubType);
	lua_pushboolean(L, KeepPrice);
	lua_pushboolean(L, KeepSeed);
	lua_pushboolean(L, IgnoreModifiers);
	

	if (!lua_pcall(L, 8, 1, 0)) {
		if (lua_istable(L, -1)) {
			printf("Pickup Morph callback run \n");
			int tablesize = lua_rawlen(L, -1);
			if (tablesize == 6) {
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), lua::callbacks::ToBoolean(L, 4), lua::callbacks::ToBoolean(L, 5), lua::callbacks::ToBoolean(L, 6));
				return;
			}
			else if (tablesize == 3) {
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), KeepPrice, KeepSeed,IgnoreModifiers);
				return;
			}
		}
		else if (lua_isboolean(L, -1)) {
			printf("Pickup Morph callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers);
}
//end of Pre_Morph callvacks

//POST_PICKUP_SHOP_PURCHASE (id: 1062)
void ProcessPostPickupShopPurchase(Entity_Pickup* pickup, Entity_Player* player, int moneySpent)
{
	lua_State* L = g_LuaEngine->_state;

	lua_getglobal(L, "Isaac");
	lua_getfield(L, -1, "RunCallback");

	lua_pushinteger(L, 1062);

	lua::luabridge::UserdataPtr::push(L, pickup, lua::GetMetatableKey(lua::Metatables::ENTITY_PICKUP));
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	lua_pushinteger(L, moneySpent);

	lua_pcall(L, 4, 1, 0);
}

HOOK_METHOD(Entity_Pickup, TriggerShopPurchase, (Entity_Player* player, int moneySpent) -> void) {

	Entity_Pickup* pickup = (Entity_Pickup*)this;
	super(player, moneySpent);
	ProcessPostPickupShopPurchase(pickup, player, moneySpent);
}

