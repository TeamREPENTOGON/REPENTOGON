#include "SigScan.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/XMLData.h"

#include "Windows.h"
#include <string>

static int QueryRadiusRef = -1;
static int timerFnTable = -1;

LUA_FUNCTION(Lua_IsaacFindByTypeFix)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	int type = (int)luaL_checkinteger(L, 1);
	int variant = (int)luaL_optinteger(L, 2, -1);
	int subtype = (int)luaL_optinteger(L, 3, -1);
	bool cache = false;
	if lua_isboolean(L, 4)
		cache = lua_toboolean(L, 4);
	bool ignoreFriendly = false;
	if lua_isboolean(L, 5)
		ignoreFriendly = lua_toboolean(L, 5);
	lua_newtable(L);
	EntityList_EL res(*list->GetUpdateEL());

	list->QueryType(&res, type, variant, subtype, cache, ignoreFriendly);

	unsigned int size = res._size;

	if (size) {
		Entity** data = res._data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}

		res.Destroy();
	}

	return 1;
}


LUA_FUNCTION(Lua_IsaacGetRoomEntitiesFix)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList_EL* res = room->GetEntityList()->GetUpdateEL();
	lua_newtable(L);
	unsigned int size = res->_size;

	if (size) {
		Entity** data = res->_data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}
	}
	return 1;
}

static void DummyQueryRadius(EntityList_EL* el, void* pos, int partition) {
	el->_data = nullptr;
	el->_size = 0;
	el->_sublist = 1;
	el->_capacity = 0;
}

LUA_FUNCTION(Lua_IsaacFindInRadiusFix)
{
	Room* room = *g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float radius = (float)luaL_checknumber(L, 2);
	unsigned int partition = (unsigned int)luaL_optinteger(L, 3, -1);

	EntityList_EL res;
	EntityList_EL* resPtr = &res;
	lua_newtable(L);

	lua_rawgeti(g_LuaEngine->_state, LUA_REGISTRYINDEX, QueryRadiusRef);
	const void* queryRadius = lua_topointer(g_LuaEngine->_state, -1);
	lua_pop(g_LuaEngine->_state, 1);

	/* __asm {
		push ecx;
		mov ecx, list;
		push partition;
		push pos;
		push resPtr;
		movss xmm3, radius;
		call queryRadius;
		pop ecx;
	} */

	list->QueryRadius(&res, pos, radius, partition);

	unsigned int size = res._size;

	if (size) {
		Entity** data = res._data;
		unsigned int idx = 1;
		while (size) {
			Entity* ent = *data;
			lua_pushnumber(L, idx);
			lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
			lua_settable(L, -3);
			++data;
			idx++;
			--size;
		}

		res.Destroy();
	}

	return 1;
}

LUA_FUNCTION(Lua_GetLoadedModules) {
	lua_pushstring(L, "_LOADED");
	int t = lua_rawget(L, LUA_REGISTRYINDEX);
	if (t != LUA_TNIL) {
		return 1;
	}

	return 0;
}

static void __cdecl TimerFunction(Entity_Effect* effect) {
	lua_State* L = g_LuaEngine->_state;
	lua_rawgeti(g_LuaEngine->_state, LUA_REGISTRYINDEX, timerFnTable); // table
	lua_pushlightuserdata(L, effect); // table, ptr
	lua_rawget(L, -2); // table, fn
	lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT)); // table, fn, arg
	// lua_pushinteger(L, 10);
	lua_pcall(L, 1, 0, 0); // table
	lua_pop(L, 1); // restored
}

LUA_FUNCTION(Lua_CreateTimer) {
	if (!lua_isfunction(L, 1)) {
		return luaL_error(L, "Expected function, got %s", lua_typename(L, lua_type(L, 1)));
	}

	int delay = (int)luaL_checkinteger(L, 2);
	if (delay < 0) {
		delay = 1;
	}

	int times = (int)luaL_optinteger(L, 3, 0);
	if (times < 0)
		times = 1;

	bool persistent = true;
	if (lua_isboolean(L, 4))
		persistent = lua_toboolean(L, 4);

	Entity_Effect* effect = Entity_Effect::CreateTimer(&TimerFunction, delay, times, persistent);

	// Register function in the registry
	lua_rawgeti(L, LUA_REGISTRYINDEX, timerFnTable);
	lua_pushlightuserdata(L, effect);
	lua_pushvalue(L, 1);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	return 1;
}

LUA_FUNCTION(Lua_DrawLine) {
	Vector* pos1 = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	Vector* pos2 = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	KColor* col1 = lua::GetUserdata<KColor*>(L, 3, lua::Metatables::KCOLOR, "KColor");
	KColor* col2 = lua::GetUserdata<KColor*>(L, 4, lua::Metatables::KCOLOR, "KColor");
	float thickness = (float)luaL_optnumber(L, 5, 1); // mmmmMMMMMMMMMMMMMMmm

	g_ShapeRenderer->RenderLine(pos1, pos2, col1, col2, thickness);

	return 0;
}

LUA_FUNCTION(Lua_DrawQuad) {
	Vector* postl = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	Vector* postr = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* posbl = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Vector* posbr = lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	KColor* col = lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	float thickness = (float)luaL_optnumber(L, 6, 1); // mmmmMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmmmmmmmm

	DestinationQuad quad; //TODO make a constructor for this
	quad._topLeft = *postl;
	quad._topRight = *postr;
	quad._bottomLeft = *posbl;
	quad._bottomRight = *posbr;

	g_ShapeRenderer->OutlineQuad(&quad, col, thickness);

	return 0;

}

LUA_FUNCTION(Lua_SetClipboard) {
	const char* text = luaL_checkstring(L, 1);

	if (!OpenClipboard(NULL)) {
		lua_pushboolean(L, false);
		return 2;
	}

	EmptyClipboard();
	size_t textLength = strlen(text);

	//allocate global memory to hold the text
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, textLength + 1);
	if (hData == NULL) {
		CloseClipboard();
		lua_pushboolean(L, false);
		return 2;
	}

	//lock the global memory to get a pointer to the data
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == NULL) {
		CloseClipboard();
		GlobalFree(hData);
		lua_pushboolean(L, false);
		return 2;
	}

	strcpy_s(pszText, textLength + 1, text); //copy the text to the global memory
	GlobalUnlock(hData);//unlock the global memory
	SetClipboardData(CF_TEXT, hData);
	CloseClipboard();
	lua_pushboolean(L, true);

	return 1;
}

LUA_FUNCTION(Lua_GetClipboard) {
	if (!OpenClipboard(NULL)) {
		lua_pushnil(L);
		return 1;
	}

	HANDLE hData = GetClipboardData(CF_TEXT); //get the clipboard data handle
	if (hData == NULL) {
		CloseClipboard();
		lua_pushnil(L);
		return 1;
	}

	char* pszText = static_cast<char*>(GlobalLock(hData)); 	//lock the handle to get a pointer to the data
	if (pszText == NULL) {
		CloseClipboard();
		lua_pushnil(L);
		return 1;
	}
	std::string clipboardText(pszText);

	//unlock and close the clipboard
	GlobalUnlock(hData);
	CloseClipboard();

	lua_pushstring(L, clipboardText.c_str());

	return 1;
}

LUA_FUNCTION(Lua_GetSubTypwByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.EntityData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.EntityData->GetNodeByName(text);
		if ((ent.count("subtype") > 0) && (ent["subtype"].length() > 0)) {
			lua_pushnumber(L, stoi(ent["subtype"]));
			return 1;
		}
	};
	lua_pushnumber(L, 0);
	return 1;
}

LUA_FUNCTION(Lua_PlayCutscene) {
	int text = (int)luaL_checknumber(L, 1);
	string out;
	g_Game->GetConsole()->RunCommand("cutscene " + to_string(text), &out, NULL);
	return 1;
}

LUA_FUNCTION(Lua_GetCutsceneByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.CutsceneData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.CutsceneData->GetNodeByName(text);
		if ((ent.count("id") > 0) && (ent["id"].length() > 0)) {
			lua_pushnumber(L, stoi(ent["id"]));
			return 1;
		}
	};
	lua_pushnumber(L, 0);
	return 1;
}

LUA_FUNCTION(Lua_IsaacCanStartTrueCoop) {
	lua_pushboolean(L, !Isaac::CanStartTrueCoop());
	return 1;
}

LUA_FUNCTION(Lua_IsaacGetNullItemIdByName) {
	const string name = string(luaL_checkstring(L, 1));

	for (ItemConfig_Item* nullitem : *g_Manager->GetItemConfig()->GetNullItems()) {
		if (nullitem != nullptr && nullitem->name == name) {
			lua_pushinteger(L, nullitem->id);
			return 1;
		}
	}

	lua_pushinteger(L, -1);
	return 1;
}

LUA_FUNCTION(Lua_IsaacShowErrorDialog) {
	const char* title = luaL_checkstring(L, 1);
	const char* text = luaL_checkstring(L, 2);
	int icon = (int)luaL_optinteger(L, 3, MB_ICONERROR);
	int buttons = (int)luaL_optinteger(L, 4, MB_OK);

	int mbreturn = MessageBoxA(NULL, text, title, icon | buttons);
	lua_pushinteger(L, mbreturn);

	return 1;
}

LUA_FUNCTION(Lua_IsaacGetCursorSprite) {
	lua::luabridge::UserdataPtr::push(L, &g_Manager->_cursorSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

// this is a rewrite of the function, the ingame one returns the result in a wacky way that i can't get to not crash
Vector GetRenderPosition(Vector *worldPos, bool scale) {
	float x = (g_WIDTH - 338.0f) * 0.5f + (worldPos->x - 60.0f) * 0.65f;
	float y = (g_HEIGHT - 182.0f) * 0.5f + (worldPos->y - 140.0f) * 0.65f;

	Vector result(x, y);

	if (scale) {
		float scale = g_DisplayPixelsPerPoint * g_PointScale;
		result.x = floor(x * scale + 0.5f) / scale;
		result.y = floor(y * scale + 0.5f) / scale;
	}
	return result;
}

LUA_FUNCTION(Lua_IsaacGetRenderPosition) {
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	bool scale = true;
	if (lua_isboolean(L, 2)) {
		scale = lua_toboolean(L, 2);
	}

	Vector result = GetRenderPosition(pos, scale);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = result;

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua_newtable(_state);
	timerFnTable = luaL_ref(_state, LUA_REGISTRYINDEX);

	// Fix existing functions
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetRoomEntities", Lua_IsaacGetRoomEntitiesFix);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "FindByType", Lua_IsaacFindByTypeFix);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "FindInRadius", Lua_IsaacFindInRadiusFix);

	// new functions
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "CanStartTrueCoop", Lua_IsaacCanStartTrueCoop);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "CreateTimer", Lua_CreateTimer);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "DrawQuad", Lua_DrawQuad);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "DrawLine", Lua_DrawLine);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetClipboard", Lua_GetClipboard);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetClipboard", Lua_SetClipboard);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCursorSprite", Lua_IsaacGetCursorSprite);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetLoadedModules", Lua_GetLoadedModules);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetEntitySubTypeByName", Lua_GetSubTypwByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCutsceneIdByName", Lua_GetCutsceneByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetNullItemIdByName", Lua_IsaacGetNullItemIdByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetRenderPosition", Lua_IsaacGetRenderPosition);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "PlayCutscene", Lua_PlayCutscene);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "ShowErrorDialog", Lua_IsaacShowErrorDialog);

	SigScan scanner("558bec83e4f883ec14535657f3");
	bool result = scanner.Scan();
	if (!result) {
		lua_pushlightuserdata(_state, &DummyQueryRadius);
	}
	else {
		lua_pushlightuserdata(_state, scanner.GetAddress());
	}
	QueryRadiusRef = luaL_ref(_state, LUA_REGISTRYINDEX);
}