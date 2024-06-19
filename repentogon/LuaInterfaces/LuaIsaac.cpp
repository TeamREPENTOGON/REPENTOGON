#include "SigScan.h"
#include "IsaacRepentance.h"
#include "../ImGuiFeatures/ConsoleMega.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/XMLData.h"

#include "Windows.h"
#include <string>
#include "../Patches/ChallengesStuff.h"
#include <dwmapi.h>

#include "../MiscFunctions.h"

static int QueryRadiusRef = -1;
static int timerFnTable = -1;

LUA_FUNCTION(Lua_IsaacFindByTypeFix)
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	int type = (int)luaL_checkinteger(L, 1);
	int variant = (int)luaL_optinteger(L, 2, -1);
	int subtype = (int)luaL_optinteger(L, 3, -1);
	bool cache = lua::luaL_optboolean(L, 4, false);
	bool ignoreFriendly = lua::luaL_optboolean(L, 5, false);
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
	Room* room = g_Game->GetCurrentRoom();
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
	Room* room = g_Game->GetCurrentRoom();
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
	if (delay <= 0) {
		delay = 1;
	}

	int times = (int)luaL_optinteger(L, 3, 0);
	if (times < 0)
		times = 1;

	bool persistent = lua::luaL_optboolean(L, 4, true);

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

LUA_FUNCTION(Lua_StartNewGame) {
	int pltype = (int)luaL_optinteger(L, 1, 0);
	int challenge = (int)luaL_optinteger(L, 2, 0);
	unsigned int difficulty = (int)luaL_optinteger(L, 3, 0);
	unsigned int seed = (int)luaL_optinteger(L, 4, 0);
	Seeds seedobj;	//am avoiding heap corruption this way
	seedobj.constructor();	
	seedobj.set_start_seed(seed);
	g_Manager->StartNewGame(pltype, challenge, seedobj, difficulty);
	return 0;
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

LUA_FUNCTION(Lua_GetSubTypeByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.EntityData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.EntityData->GetNodeByName(text);
		if ((ent.count("subtype") > 0) && (ent["subtype"].length() > 0)) {
			lua_pushinteger(L, stoi(ent["subtype"]));
			return 1;
		}
	};
	lua_pushinteger(L, 0);
	return 1;
}

LUA_FUNCTION(Lua_PlayCutscene) {
	const unsigned int cutscene = (unsigned int)luaL_checkinteger(L, 1);
	const bool shouldClear = lua::luaL_optboolean(L, 2, false);
	g_Manager->ShowCutscene(cutscene, shouldClear);
	return 0;
}

LUA_FUNCTION(Lua_GetCutsceneByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.CutsceneData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.CutsceneData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			lua_pushinteger(L, stoi(ent["id"]));
			return 1;
		}
	};
	lua_pushinteger(L, -1);
	return 1;
}

LUA_FUNCTION(Lua_GetGiantBookByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.GiantBookData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.GiantBookData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			lua_pushinteger(L, stoi(ent["id"]));
			return 1;
		}
	};
	lua_pushinteger(L, -1);
	return 1;
}

LUA_FUNCTION(Lua_IsaacCanStartTrueCoop) {
	lua_pushboolean(L, !PlayerManager::CoopBabiesOnly());
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

bool apipause = false;
HOOK_STATIC(Manager, Update, () -> void, __stdcall) {
	if (apipause) {
		g_Manager->_state = 2;
	}
	super();
	
}
LUA_FUNCTION(Lua_IsaacPause) {
	apipause = true;
	return 0;
}
LUA_FUNCTION(Lua_IsaacResume) {
	if (apipause && !console.enabled) {
		g_Game->GetConsole()->_state = 0;
	}
	apipause = false;
	return 0;
}

LUA_FUNCTION(Lua_IsaacGetRenderPosition) {
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	bool scale = lua::luaL_optboolean(L, 2, true);
	
	Vector result = Isaac::GetRenderPosition(pos, scale);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = result;

	return 1;
}

LUA_FUNCTION(Lua_IsaacGetCollectibleSpawnPosition) {
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");

	Vector result = Isaac::GetCollectibleSpawnPosition(pos);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = result;

	return 1;
}

LUA_FUNCTION(Lua_IsaacClearBossHazards) {
	if (g_Game == nullptr || g_Game->_room == nullptr) {
		return luaL_error(L, "Must be in a room to use this!");
	}
	bool ignoreNPCs = lua::luaL_optboolean(L, 1, false);
	Isaac::ClearBossHazards(ignoreNPCs);
	return 0;
}

LUA_FUNCTION(Lua_IsaacFindInCapsule)
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	unsigned int partition = (unsigned int)luaL_optinteger(L, 2, -1);

	lua_newtable(L);
	EntityList_EL res(*list->GetUpdateEL());

	list->QueryCapsule(&res, capsule, partition);

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

LUA_FUNCTION(Lua_TriggerWindowResize)
{
	g_Manager->ResizeWindow(g_WindowSizeX, g_WindowSizeY);
	return 0;
}

LUA_FUNCTION(Lua_CenterCursor)
{
	HWND hwnd = GetActiveWindow();
	DWORD activeProcessId;
	GetWindowThreadProcessId(hwnd, &activeProcessId);
	DWORD currentProcessId = GetCurrentProcessId();

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	POINT clientCenter;
	clientCenter.x = (clientRect.right - clientRect.left) / 2;
	clientCenter.y = (clientRect.bottom - clientRect.top) / 2;

	ClientToScreen(hwnd, &clientCenter);
	if (activeProcessId == currentProcessId) { //so it doesnt do it if Isaac is not the active win
		SetCursorPos(clientCenter.x, clientCenter.y);
	}
	return 0;
};

LUA_FUNCTION(Lua_SetDWMAttrib)
{
	HWND hwnd = GetActiveWindow();
	DWORD activeProcessId;
	GetWindowThreadProcessId(hwnd, &activeProcessId);
	DWORD currentProcessId = GetCurrentProcessId();
	int32_t attribid = (int32_t)luaL_optinteger(L, 1, 0);
	int32_t attribval = (int32_t)luaL_optinteger(L, 2, 0);

	switch (attribid) {
	case (DWMWINDOWATTRIBUTE)DWMWA_CLOAK:
		return luaL_error(L, "Usage of DWMWA_CLOAK attribute is prohibited!");
		break;
	case (DWMWINDOWATTRIBUTE)DWMWA_CLOAKED:
		return luaL_error(L, "Usage of DWMWA_CLOAKED attribute is prohibited!");
		break;
	};
	if (activeProcessId == currentProcessId) {
		DwmSetWindowAttribute(hwnd, attribid, &attribval, sizeof(attribval));
	};

	return 0;
};

LUA_FUNCTION(Lua_GetDWMAttrib)
{
	HWND hwnd = GetActiveWindow();
	int32_t attribid = (int32_t)luaL_optinteger(L, 1, 0);
	int32_t attribval;
	DwmGetWindowAttribute(hwnd, attribid,&attribval,sizeof(attribval));
	lua_pushinteger(L, attribval);
	return 1;
};

LUA_FUNCTION(Lua_SetWindowTitle)
{
	const char* text=nullptr;
	if (!lua_isstring(L,1)) {
		REPENTOGON::SetStockWindowTitle();
		return 0;
	};
	text = luaL_checkstring(L, 1);
	char buffer[256];
	strncpy_s(REPENTOGON::moddedtitle, text, 255);
	strncpy_s(buffer, REPENTOGON::stocktitle, 255);
	strncat_s(buffer, REPENTOGON::moddedtitle, 255);
	SetWindowTextA(GetActiveWindow(), buffer);
	return 0;
};

LUA_FUNCTION(Lua_GetWindowTitle)
{
	lua_pushstring(L, REPENTOGON::moddedtitle);
	return 1;
};

LUA_FUNCTION(Lua_IsInGame) {
	lua_pushboolean(L, Isaac::IsInGame());

	return 1;
}

LUA_FUNCTION(Lua_IsChallengeDone) {
	int challengeid = (int)luaL_checkinteger(L, 1);
	if (challengeid < 1) {
		return luaL_error(L, "Invalid Challenge ID (expected > 0, got %d)", challengeid);
	}
	if (challengeid <= 45) {
		lua_pushboolean(L, g_Manager->GetPersistentGameData()->challenges[challengeid]);
	}
	else {
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeid);
		lua_pushboolean(L, Challenges[node["name"] + node["sourceid"]] > 0);
	}
	return 1;
}


LUA_FUNCTION(Lua_UnDoChallenge) {
	int challengeid = (int)luaL_checkinteger(L, 1);
	UndoChallenge(challengeid);
	return 1;
}

LUA_FUNCTION(Lua_ClearChallenge) {
	int challengeid = (int)luaL_checkinteger(L, 1);
	if (challengeid < 1) {
		return luaL_error(L, "Invalid Challenge ID (expected > 0, got %d)", challengeid);
	}
	g_Manager->GetPersistentGameData()->AddChallenge(challengeid);
	if (challengeid <= 45) {
		g_Manager->GetPersistentGameData()->Save(); //if the challenges are already unlocked for the challenge then it wont fucking save otherwise!
	}
	return 1;
}

LUA_FUNCTION(Lua_GetModChallengeClearCount) {
	int challengeid = (int)luaL_checkinteger(L, 1);
	XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeid);
	lua_pushinteger(L, Challenges[node["name"] + node["sourceid"]]);

	return 1;
}

LUA_FUNCTION(Lua_GetBossColorIdxByName) {
	string bosscolorname = luaL_checkstring(L, 1);
	auto iter = XMLStuff.BossColorData->childbyname.find(bosscolorname);
	if (iter == XMLStuff.BossColorData->childbyname.end()) { lua_pushinteger(L, -1); }
	else {
		lua_pushinteger(L, (iter->second - 1));
	}

	return 1;
}

LUA_FUNCTION(Lua_GetBackdropTypeByName) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.BackdropData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.BackdropData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			lua_pushinteger(L, stoi(ent["id"]));
			return 1;
		}
	};
	lua_pushinteger(L, -1);
	return 1;
}

LUA_FUNCTION(Lua_GetRGON_Changelog) {
	string text = "Changelog unavailable :(\n";
	ostringstream outtext;
	ifstream changelog;
	changelog.open("rgon_changelog.txt");
	if (changelog.is_open()) {
		outtext << changelog.rdbuf();
		text = outtext.str();
	};
	lua_pushstring(L, text.c_str());
	return 1;
};

LUA_FUNCTION(Lua_SetIcon) {
//	int iconsize = luaL_optinteger(L, 2, ICON_SMALL);
	int resolution=16;
	bool ignorecap=lua::luaL_optboolean(L,2,false);
	if(ignorecap){
		resolution=LR_DEFAULTSIZE;
	};
	// switch (iconsize) {
	// case 0:
	// 	iconsize = ICON_SMALL;
	// 	break;
	// case 1:
	// 	iconsize = ICON_BIG;
	// 	break;
	// };
	if (lua_isinteger(L, 1)) {
		int icontoset = (int)luaL_checkinteger(L, 1);
		switch (icontoset) {
		case 0:
			icontoset = 0x65;
			break;
		case 1:
			icontoset = 0x68;
			break;
		default:
			icontoset = 0x65;
		};
		HANDLE icon = LoadImageA(GetModuleHandle(nullptr), (LPCSTR)icontoset, IMAGE_ICON, resolution, resolution, 0);
		HANDLE icon_big = LoadImageA(GetModuleHandle(nullptr), (LPCSTR)icontoset, IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, 0);
		if (icon) {
			SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)icon);
			SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)icon_big);
		};
		return 0;
	};
	const char* str=nullptr;
	str=luaL_optstring(L, 1, str);
	if (!str) {
		return 0;
	};
	std::string modpath = str;
	std::string fullpath;
	g_Manager->GetModManager()->TryRedirectPath(&fullpath,&modpath);
	HANDLE icon=LoadImageA(nullptr, fullpath.c_str(), IMAGE_ICON, resolution, resolution, LR_LOADFROMFILE | LR_SHARED);
	if (!icon) {
		return luaL_error(L, "Icon has failed to load!");
	};
	SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)icon);
	SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)icon);
	return 0;
};

LUA_FUNCTION(Lua_FindTargetPit) {
	Vector* position = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	Vector* targetPosition = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const int pitIndex = (int)luaL_optinteger(L, 3, -1);

	lua_pushinteger(L, Entity_NPC::FindTargetPit(position, targetPosition, pitIndex));
	return 1;
}

LUA_FUNCTION(Lua_GetAxisAlignedUnitVectorFromDir) {
	const int dir = (int)luaL_optinteger(L, 1, -1);

	Vector result = Isaac::GetAxisAlignedUnitVectorFromDir(dir);
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
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "ClearBossHazards", Lua_IsaacClearBossHazards);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "CreateTimer", Lua_CreateTimer);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "DrawQuad", Lua_DrawQuad);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "DrawLine", Lua_DrawLine);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetClipboard", Lua_GetClipboard);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetClipboard", Lua_SetClipboard);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCursorSprite", Lua_IsaacGetCursorSprite);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetLoadedModules", Lua_GetLoadedModules);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetEntitySubTypeByName", Lua_GetSubTypeByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCutsceneIdByName", Lua_GetCutsceneByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetGiantBookIdByName", Lua_GetGiantBookByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetNullItemIdByName", Lua_IsaacGetNullItemIdByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetRenderPosition", Lua_IsaacGetRenderPosition);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCollectibleSpawnPosition", Lua_IsaacGetCollectibleSpawnPosition);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "PlayCutscene", Lua_PlayCutscene);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "ShowErrorDialog", Lua_IsaacShowErrorDialog);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "FindInCapsule", Lua_IsaacFindInCapsule);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "TriggerWindowResize", Lua_TriggerWindowResize);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "CenterCursor", Lua_CenterCursor);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetDwmWindowAttribute", Lua_SetDWMAttrib);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetDwmWindowAttribute", Lua_GetDWMAttrib);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetWindowTitle", Lua_SetWindowTitle);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetWindowTitle", Lua_GetWindowTitle);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetIcon", Lua_SetIcon);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "IsInGame", Lua_IsInGame);
	//lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "Pause", Lua_IsaacPause); 
	//lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "Resume", Lua_IsaacResume); //not done, feel free to pick these up they suck
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "IsChallengeDone", Lua_IsChallengeDone);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "ClearChallenge", Lua_ClearChallenge);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "MarkChallengeAsNotDone", Lua_UnDoChallenge);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetModChallengeClearCount", Lua_GetModChallengeClearCount);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetBossColorIdxByName", Lua_GetBossColorIdxByName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetBossColorIdByName", Lua_GetBossColorIdxByName); //alias for musclememory
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetBackdropIdByName", Lua_GetBackdropTypeByName); //changed to Id to fit the rest didnt release yet so it foine
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "StartNewGame", Lua_StartNewGame);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "RGON_GetChangelog", Lua_GetRGON_Changelog);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "FindTargetPit", Lua_FindTargetPit);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetAxisAlignedUnitVectorFromDir", Lua_GetAxisAlignedUnitVectorFromDir);

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
