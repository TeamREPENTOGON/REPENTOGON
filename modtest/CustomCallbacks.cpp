#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//AddCollectible Callback (id: 1004 enum pending)
HOOK_METHOD(Entity_Player, AddCollectible, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	printf("item get\n");
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);
	lua::LuaCaller caller(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults res = caller.push(1004)
		.push(type)
		.push(type)
		.push(charge)
		.push(firsttime)
		.push(slot)
		.push(vardata)
		.call(1);

	if (!res) {
		printf("Additem callback run \n");
		if (lua_istable(L, -1)) { // 6 params 
			int* result = new int[5];
			printf("tablesize: %d\n", 5);
			for (int i = 1; i <= 5; i++) {
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				result[i - 1] = lua_tointeger(L, -1); //I only need ints here, otherwise I'd need to check the type
				printf("V: %d\n", result[i - 1]);
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

	super(type, charge, firsttime, slot, vardata);
}
//AddCollectible Callback (id: 1004 enum pending)

//1005 RESERVED - POST_ADD_COLLECTIBLE

//POST_TAKE_DMG callback (id: 1006 enum pending)
void ProcessPostDamageCallback(Entity* ent, float damage, unsigned __int64 damageFlags, EntityRef* source, int damageCountdown) {

	lua_State* L = g_LuaEngine->_state;

	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L)
		.push(1006)
		.pushnil()
		.push(ent, lua::Metatables::ENTITY)
		.push(damage)
		.push(damageFlags)
		.push(source, lua::Metatables::ENTITY_REF)
		.push(damageCountdown)
		.call(1); // Sylmir, note: the original code asked for a single return value but never used it

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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1010)
		.push(gridRock, lua::Metatables::GRID_ENTITY_ROCK)
		.pushnil()
		.push(variant)
		.call(1); // Sylmir note: original version said there was 1 result on the stack

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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1020).call(1);

	super();
}

//HUD_UPDATE callback end

//HUD_POST_UPDATE (id: 1021)

HOOK_METHOD(HUD, PostUpdate, () -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1021).call(1);

	super();
}

//HUD_POST_UPDATE callback end

//HUD_POST_RENDER (id: 1022)

HOOK_METHOD(HUD, Render, () -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1022).call(1);

	/*FILE* f = fopen("repentogon.log", "a");
	fprintf(f, "After call to HUD::Render, there are %d elements on the stack\n", lua_gettop(L));
	fclose(f);*/

	super();
}

//HUD_POST_RENDER callback end

//Character menu render Callback(id:1023)
HOOK_METHOD(MenuManager, Render, () -> void) {
	super();
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1023).call(1);
}

//Character menu render Callback end

//SFX_PRE/POST_PLAY (id: 1030/1031)
void ProcessPostSFXPlay(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
{
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1031)
		.push(ID)
		.push(ID)
		.push(Volume)
		.push(FrameDelay)
		.push(Loop)
		.push(Pitch)
		.push(Pan)
		.call(1);
}

HOOK_METHOD(SFXManager, Play, (int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1030)
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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaCaller(L).push(1041)
		//.push(ent->GetType())
		.push(player, lua::Metatables::ENTITY_PLAYER)
		.push(ent, lua::Metatables::ENTITY)
		.pushUserdataValue(*Velocity, lua::Metatables::VECTOR)
		.call(1);
}

HOOK_METHOD(Entity_Player, ThrowHeldEntity, (Vector* Velocity) -> Entity*) {
	lua_State* L = g_LuaEngine->_state;
	Entity** heldEntity = this->GetHeldEntity();
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	//printf("Entity type: %d\n", (*heldEntity)->GetVariant());

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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	Entity_Player* ent = (Entity_Player*)this;

	lua::LuaCaller(L).push(1042)
		.push(this->GetPlayerType())
		.push(ent, lua::Metatables::ENTITY_PLAYER)
		.call(1);

	super();
}

//PRE_ROOM_EXIT (1043) (currently using Entity_Player::TriggerRoomExit as the base)
HOOK_METHOD(Entity_Player, TriggerRoomExit, (bool unk) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	Entity_Player* ent = (Entity_Player*)this;

	lua::LuaCaller(L).push(1043)
		.pushnil()
		.push(ent, lua::Metatables::ENTITY_PLAYER)
		.push(unk)
		.call(1);
	super(unk);
}

//PRE_MUSIC_PLAY Callback (id: 1034 enum pending)
HOOK_METHOD(Music, Play, (int musicid, float volume) -> void) {
	printf("music plays\n");
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1034)
		.push(musicid)
		.push(musicid)
		.push(volume)
		.push(false)
		.call(1);

	if (!result) {
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
			super(lua_tointeger(L, -1), volume);
			return;
		}
		else if (lua_isboolean(L, -1)) {
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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1034)
		.push(musicid)
		.push(musicid)
		.push(faderate)
		.push(true)
		.call(1);

	if (!result) {
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
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1060)
		.call(1);
	super();
}
//PRE_LEVEL_INIT Callback (id: 1060 enum pending)

//PRE_TRIGGER_PLAYER_DEATH (id: 1050)
HOOK_METHOD(Entity_Player, TriggerDeath, (bool checkOnly) -> bool) {
	lua_State* L = g_LuaEngine->_state;

	if (!checkOnly) {
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(1050)
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
	lua_State* L = g_LuaEngine->_state;

	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1070)
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
	return true;
}

void ProcessPostRestockCallback(bool Partial) {
	lua_State* L = g_LuaEngine->_state;

	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1071)
		.pushnil()
		.push(Partial)
		.call(1);
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

	lua::LuaStackProtector protector(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1061)
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


//Pre_Morph Callbacks (id:1080)
HOOK_METHOD(Entity_NPC, Morph, (int EntityType, int Variant, int SubType, int Championid) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/// INCONSISTENT CALLBACK ID IN COMMENT AND REALITY
	/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	lua::LuaResults result = lua::LuaCaller(L).push(2012)
		.push(this, lua::Metatables::ENTITY_NPC)
		.pushnil()
		.push(EntityType)
		.push(Variant)
		.push(SubType)
		.push(Championid)
		.call(1);

	if (!result) {
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
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(2013)
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
			printf("Pickup Morph callback run \n");
			int tablesize = lua_rawlen(L, -1);
			if (tablesize == 6) {
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), lua::callbacks::ToBoolean(L, 4), lua::callbacks::ToBoolean(L, 5), lua::callbacks::ToBoolean(L, 6));
				return;
			}
			else if (tablesize == 3) {
				super(lua::callbacks::ToNumber(L, 1), lua::callbacks::ToNumber(L, 2), lua::callbacks::ToNumber(L, 3), KeepPrice, KeepSeed, IgnoreModifiers);
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
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1062)
		.push(*pickup->GetVariant())
		.push(pickup, lua::Metatables::ENTITY_PICKUP)
		.push(player, lua::Metatables::ENTITY_PLAYER)
		.push(moneySpent)
		.call(1);
}

HOOK_METHOD(Entity_Pickup, TriggerShopPurchase, (Entity_Player* player, int moneySpent) -> void) {

	Entity_Pickup* pickup = (Entity_Pickup*)this;
	super(player, moneySpent);
	ProcessPostPickupShopPurchase(pickup, player, moneySpent);
}


//GET_FOLLOWER_PRIORITY (id: 1063)
HOOK_METHOD(Entity_Familiar, GetFollowerPriority, () -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
	Entity_Familiar* fam = (Entity_Familiar*)this;


	lua::LuaResults result = lua::LuaCaller(L).push(1063)
		.push(*fam->GetVariant())
		.push(fam, lua::Metatables::ENTITY_FAMILIAR)
		.call(1);

	if (!result) {
		if (lua_isinteger(L, -1)) {
			return lua_tointeger(L, -1);
		}

	}

	return super();
}

//PRE_USE_CARD (id: 1064)
HOOK_METHOD(Entity_Player, UseCard, (int cardType, unsigned int useFlag) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	Entity_Player* plr = (Entity_Player*)this;
	lua::LuaResults result = lua::LuaCaller(L).push(1064)
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


//PRE_USE_PILL (id: 1065)
HOOK_METHOD(Entity_Player, UsePill, (int pillEffect, int pillColor, unsigned int useFlag) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1065)
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

	super(pillEffect, pillColor, useFlag);
}

//GET_SHOP_ITEM_PRICE (id: 1066)
HOOK_METHOD(Room, GetShopItemPrice, (unsigned int entVariant, unsigned int entSubtype, int shopItemID) -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1066)
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

	return super(entVariant, entSubtype, shopItemID);
}

//PLAYER_GET_HEALTH_TYPE (id: 1067)
HOOK_METHOD(Entity_Player, GetHealthType, () -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1067)
		.push(this->GetPlayerType())
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.call(1);

	if (!result) {
		if (lua_isinteger(L, -1)) {
			return lua_tointeger(L, -1);
		}
	}

	return super();
}
//PRE_FAMILIAR_RENDER (id: 1080)
HOOK_METHOD(Entity_Familiar, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1080)
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

	super(offset);
}

//PRE_NPC_RENDER (id: 1081)
HOOK_METHOD(Entity_NPC, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1081)
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

	super(offset);
}

//PRE_PLAYER_RENDER (id: 1082)
HOOK_METHOD(Entity_Player, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1082)
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

	super(offset);
}

//PRE_PICKUP_RENDER (id: 1083)
HOOK_METHOD(Entity_Pickup, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1083)
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

	super(offset);
}

//PRE_TEAR_RENDER (id: 1084)
HOOK_METHOD(Entity_Tear, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1083)
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

	super(offset);
}

//PRE_PROJECTILE_RENDER (id: 1085)
HOOK_METHOD(Entity_Projectile, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1085)
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

	super(offset);
}

//PRE_KNIFE_RENDER (id: 1086)
HOOK_METHOD(Entity_Knife, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1086)
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

	super(offset);
}

//PRE_EFFECT_RENDER (id: 1087)
HOOK_METHOD(Entity_Effect, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1087)
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

	super(offset);
}

//PRE_BOMB_RENDER (id: 1088)
HOOK_METHOD(Entity_Bomb, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1088)
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

	super(offset);
}

//PRE/POST_SLOT_RENDER (id: 1089/1090)
HOOK_METHOD(Entity_Slot, Render, (Vector* offset) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1089)
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

	super(offset);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults postResult = lua::LuaCaller(L).push(1090)
		.push(*this->GetVariant())
		.push(this, lua::metatables::EntitySlotMT)
		.pushUserdataValue(*offset, lua::Metatables::VECTOR)
		.call(1);
}

//RenderHead Callback (id: 1038)
HOOK_METHOD(Entity_Player, RenderHead, (Vector* x) -> void) {
	//printf("Head Rendering \n");
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1038)
		.push(this->GetPlayerType())
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.push(x, lua::Metatables::VECTOR)
		.call(1);

	if (!result) {
		if (lua_isuserdata(L, -1)) {
			printf("Head Render callback run \n");
			Vector* newpos = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			super(newpos);
			return;
		}
		else if (lua_isboolean(L, -1)) {
			printf("Head Render callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(x);
}

//Renderbody Callback (id: 1039)
HOOK_METHOD(Entity_Player, RenderBody, (Vector* x) -> void) {
	//printf("Body Rendering \n");
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1039)
		.push(this->GetPlayerType())
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.push(x, lua::Metatables::VECTOR)
		.call(1);

	if (!result) {
		if (lua_isuserdata(L, -1)) {
			printf("Body Render callback run \n");
			Vector* newpos = *(Vector**)((char*)lua::CheckUserdata(L, -1, lua::Metatables::VECTOR, "Vector") + 4);
			super(newpos);
			return;
		}
		else if (lua_isboolean(L, -1)) {
			printf("Body Render callback run \n");
			if (!lua_toboolean(L, -1)) {
				return;
			}
		}
	}
	super(x);
}

//PRE_ROOM_TRIGGER_CLEAR (id: 1068)
HOOK_METHOD(Room, TriggerClear, (bool playSound) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1068)
		.pushnil()
		.push(playSound)
		.call(1);

	super(playSound);
}

//PRE_PLAYER_TRIGGER_ROOM_CLEAR (id: 1069)
HOOK_METHOD(Entity_Player, TriggerRoomClear, () -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1069)
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
	super();
}

//PLAYER_GET_ACTIVE_MAX_CHARGE (id: 1072)
HOOK_METHOD(Entity_Player, GetActiveMaxCharge, (int item, int vardata) -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1072)
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
	return super(item, vardata);
}

//PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE (id: 1073)
HOOK_METHOD(Entity_Player, GetActiveMinUsableCharge, (int slot) -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1073)
		.push(this->GetActiveItem(slot))
		.push(slot)
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.call(1);

	if (!result) {
		if (lua_isinteger(L, -1)) {
			return lua_tointeger(L, -1);
		}
	}
	return super(slot);
}

//MC_PRE_REPLACE_SPRITESHEET (id: 1100)
HOOK_METHOD(ANM2, ReplaceSpritesheet, (int LayerID, IsaacString& PngFilename) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1100)
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

	super(LayerID, PngFilename);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults postResult = lua::LuaCaller(L).push(1101)
		.push(_filename.Get())
		.push(LayerID)
		.push(PngFilename.Get())
		.call(1);
}

//PLAYER_GET_HEART_LIMIT (id: 1074)
HOOK_METHOD(Entity_Player, GetHealthLimit, (bool keeper) -> int) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1074)
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
	return super(keeper);
}

//PRE_SLOT_COLLIDE (1120)
HOOK_METHOD(Entity_Slot, HandleCollision, (Entity* collider, bool Low) -> bool) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1120)
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

	return super(collider, Low);
}

//POST_SLOT_INIT (1121)
HOOK_METHOD(Entity_Slot, Init, (int Type, int Variant, int SubType, int Seed) -> void) {
	super(Type, Variant, SubType, Seed);

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1121)
		.push(Variant)
		.push(this, lua::metatables::EntitySlotMT)
		.call(1);
}

//POST_SLOT_UPDATE (1122)
HOOK_METHOD(Entity_Slot, Update, () -> void) {
	super();

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1122)
		.push(*this->GetVariant())
		.push(this, lua::metatables::EntitySlotMT)
		.call(1);
}

//PRE/POST_SLOT_CREATE_EXPLOSION_DROPS (1123/1124)
HOOK_METHOD(Entity_Slot, CreateDropsFromExplosion, () -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1123)
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

	super();

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults postResult = lua::LuaCaller(L).push(1124)
		.push(*this->GetVariant())
		.push(this, lua::metatables::EntitySlotMT)
		.call(1);
}

//PRE/POST_SLOT_SET_PRIZE_COLLECTIBLE (1125/1126)
HOOK_METHOD(Entity_Slot, SetPrizeCollectible, (int id) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1125)
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

	super(id);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults postResult = lua::LuaCaller(L).push(1126)
		.push(*this->GetVariant())
		.push(this, lua::metatables::EntitySlotMT)
		.push(id)
		.call(1);
}

//POST_ITEM_OVERLAY_UPDATE (id: 1075)
HOOK_METHOD(ItemOverlay, Update, (bool unk) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	super(unk);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1075)
		.pushnil()
		//.push(this->GetSprite(), lua::Metatables::SPRITE)
		.call(1);
}

//PRE_ITEM_OVERLAY_SHOW (id: 1076)
HOOK_METHOD(ItemOverlay, Show, (int overlayID, int unk, Entity_Player* player) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1076)
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

	super(overlayID, unk, player);
}

//POST_PLAYER_NEW_ROOM_TEMP_EFFECTS (id: 1077)
HOOK_METHOD(Entity_Player, TriggerNewRoom_TemporaryEffects, () -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	super();

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1077)
		.push(this->GetPlayerType())
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.call(1);
}

//POST_PLAYER_NEW_LEVEL (id: 1078)
HOOK_METHOD(Entity_Player, TriggerNewStage, (bool unk) -> void) {
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	super(unk);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(1078)
		.push(this->GetPlayerType())
		.push(this, lua::Metatables::ENTITY_PLAYER)
		.push(unk)
		.call(1);
}