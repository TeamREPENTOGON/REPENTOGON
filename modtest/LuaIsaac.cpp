#include <lua.hpp>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "Windows.h"
#include <string>

static int QueryRadiusRef = -1;
static int timerFnTable = -1;

int Lua_IsaacFindByTypeFix(lua_State* L)
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


int Lua_IsaacGetRoomEntitiesFix(lua_State* L)
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

int Lua_IsaacFindInRadiusFix(lua_State* L)
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

static void RegisterFindByTypeFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "FindByType");
	lua_pushcfunction(L, Lua_IsaacFindByTypeFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetRoomEntitiesFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetRoomEntities");
	lua_pushcfunction(L, Lua_IsaacGetRoomEntitiesFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterFindInRadiusFix(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "FindInRadius");
	lua_pushcfunction(L, Lua_IsaacFindInRadiusFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_GetLoadedModules(lua_State* L) {
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

static int Lua_CreateTimer(lua_State* L) {
	if (!lua_isfunction(L, 1)) {
		return luaL_error(L, "Expected function, got %s", lua_typename(L, lua_type(L, 1)));
	}

	int delay = (int)luaL_checkinteger(L, 2);
	if (delay < 0) {
		delay = 1;
	}

	int times = (int)luaL_optinteger(L, 3, 0);
	if(times < 0)
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

static int Lua_DrawLine(lua_State* L) {
	Vector* pos1 = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	Vector* pos2 = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	KColor* col1 = lua::GetUserdata<KColor*>(L, 3, lua::Metatables::KCOLOR, "KColor");
	KColor* col2 = lua::GetUserdata<KColor*>(L, 4, lua::Metatables::KCOLOR, "KColor");
	float thickness = (float)luaL_optnumber(L, 5, 1); // mmmmMMMMMMMMMMMMMMmm

	g_ShapeRenderer->RenderLine(pos1, pos2, col1, col2, thickness);

	return 0;
}

static int Lua_DrawQuad(lua_State* L) {
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

static void RegisterGetLoadedModules(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetLoadedModules");
	lua_pushcfunction(L, Lua_GetLoadedModules);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterCreateTimer(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "CreateTimer");
	lua_pushcfunction(L, Lua_CreateTimer);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterDrawLine(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "DrawLine");
	lua_pushcfunction(L, Lua_DrawLine);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterDrawQuad(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "DrawQuad");
	lua_pushcfunction(L, Lua_DrawQuad);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}


int Lua_SetClipboard(lua_State* L) {
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

int Lua_GetClipboard(lua_State* L) {	
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

static void RegisterClipboardStuff(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "SetClipboard");
	lua_pushcfunction(L, Lua_SetClipboard);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetClipboard");
	lua_pushcfunction(L, Lua_GetClipboard);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

#include "XMLData.h"
static int Lua_GetSubTypwByName(lua_State* L) {
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
static void RegisterGetSubByName(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetEntitySubTypeByName");
	lua_pushcfunction(L, Lua_GetSubTypwByName);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_GetCutsceneByName(lua_State* L) {
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
static void RegisterGetCutsceneName(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetCutsceneIdByName");
	lua_pushcfunction(L, Lua_GetCutsceneByName);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_IsaacCanStartTrueCoop(lua_State* L) {
	lua_pushboolean(L, !Isaac::CanStartTrueCoop());
	return 1;
}

static void RegisterIsaacCanStartTrueCoop(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "CanStartTrueCoop");
	lua_pushcfunction(L, Lua_IsaacCanStartTrueCoop);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);

	lua_newtable(state);
	timerFnTable = luaL_ref(state, LUA_REGISTRYINDEX);

	RegisterFindByTypeFix(state);
	RegisterGetRoomEntitiesFix(state);
	RegisterFindInRadiusFix(state);
	RegisterGetLoadedModules(state);
	RegisterCreateTimer(state);
	RegisterDrawLine(state);
	RegisterDrawQuad(state);
	RegisterClipboardStuff(state);
	RegisterGetSubByName(state);
	RegisterIsaacCanStartTrueCoop(state);
	RegisterGetCutsceneName(state);

	SigScan scanner("558bec83e4f883ec14535657f3");
	bool result = scanner.Scan();
	if (!result) {
		lua_pushlightuserdata(state, &DummyQueryRadius);
	}
	else {
		lua_pushlightuserdata(state, scanner.GetAddress());
	}
	QueryRadiusRef = luaL_ref(state, LUA_REGISTRYINDEX);
}