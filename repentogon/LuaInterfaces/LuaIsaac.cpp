#include "SigScan.h"
#include "IsaacRepentance.h"
#include "../ImGuiFeatures/ConsoleMega.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/XMLData.h"
#include "../Patches/ItemSpoofSystem.h"
#include "../LuaClasses.h"
#include "LuaEntityBridge.h"

#include "Windows.h"
#include <string>
#include "../Patches/ChallengesStuff.h"
#include <dwmapi.h>
#include <chrono>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#include "../MiscFunctions.h"

constexpr uint32_t CONSOLE_COLOR_WARN = 0xFFFCCA03;

static int timerFnTable = -1;

static bool s_modsLoaded = false;

// make it so that the MC_POST_MODS_LOADED callback runs before marking mods as loaded
HOOK_METHOD_PRIORITY(ModManager, LoadConfigs, -1, () -> void)
{
	s_modsLoaded = false;
	super();
	s_modsLoaded = true;
}

MOD_EXPORT int L_Isaac_FindByType(int type, int variant, int subtype, bool cache, bool ignoreFriendly)
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	EntityList_EL res(*list->GetUpdateEL());

	list->QueryType(&res, type, variant, subtype, cache, ignoreFriendly);

	unsigned int size = res._size;
	StoreEntityResults(res._data, size);
	if (size) {
		res.Destroy();
	}
	return (int)size;
}

MOD_EXPORT int L_Isaac_GetRoomEntities()
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList_EL* res = room->GetEntityList()->GetUpdateEL();
	StoreEntityResults(res->_data, res->_size);
	return (int)res->_size;
}

MOD_EXPORT int L_Isaac_FindInRadius(Vector* pos, float radius, unsigned int partition)
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();

	EntityList_EL res = list->QueryRadius(pos, radius, partition);

	unsigned int size = res._size;
	StoreEntityResults(res._data, size);
	if (size) {
		res.Destroy();
	}
	return (int)size;
}

MOD_EXPORT int L_Isaac_FindInCapsule(Capsule* capsule, unsigned int partition)
{
	Room* room = g_Game->GetCurrentRoom();
	EntityList* list = room->GetEntityList();
	EntityList_EL res(*list->GetUpdateEL());

	list->QueryCapsule(&res, capsule, partition);

	unsigned int size = res._size;
	StoreEntityResults(res._data, size);
	if (size) {
		res.Destroy();
	}
	return (int)size;
}

MOD_EXPORT void L_Isaac_Explode(Vector* pos, Entity* source, float damage)
{
	g_LuaEngine->Isaac_Explode(pos, source, damage);
}

MOD_EXPORT void L_Isaac_GetFreeNearPosition(Vector* out, Vector* pos, float step)
{
	g_LuaEngine->Isaac_GetFreeNearPosition(out, pos, step);
}

MOD_EXPORT void L_Isaac_GetRandomPosition(Vector* out)
{
	g_LuaEngine->Isaac_GetRandomPosition(out);
}

MOD_EXPORT GridEntity* L_Isaac_GridSpawn(int type, int variant, Vector* pos, bool forced)
{
	return g_LuaEngine->Isaac_GridSpawn(type, variant, pos, forced);
}

MOD_EXPORT void L_Isaac_ScreenToWorld(Vector* out, Vector* pos)
{
	g_LuaEngine->Isaac_ScreenToWorld(out, pos);
}

MOD_EXPORT void L_Isaac_ScreenToWorldDistance(Vector* out, Vector* pos)
{
	g_LuaEngine->Isaac_ScreenToWorldDistance(out, pos);
}

MOD_EXPORT Entity* L_Isaac_Spawn(int type, int variant, int subtype, Vector* pos, Vector* vel, Entity* spawner)
{
	return g_LuaEngine->Isaac_Spawn(type, variant, subtype, pos, vel, spawner);
}

MOD_EXPORT void L_Isaac_WorldToRenderPosition(Vector* out, Vector* pos)
{
	g_LuaEngine->Isaac_WorldToRenderPosition(out, pos);
}

MOD_EXPORT void L_Isaac_WorldToScreen(Vector* out, Vector* pos)
{
	g_LuaEngine->Isaac_WorldToScreen(out, pos);
}

MOD_EXPORT void L_Isaac_WorldToScreenDistance(Vector* out, Vector* pos)
{
	g_LuaEngine->Isaac_WorldToScreenDistance(out, pos);
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

// Timers call a Lua function later, so this one has to stay on the Lua stack.
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

MOD_EXPORT void L_Isaac_StartNewGame(int pltype, int challenge, unsigned int difficulty, Seeds* seeds, unsigned int seed, bool isCustomRun) {
	Seeds seedobj;
	if (seeds) {
		seedobj.construct_from_copy(seeds);
	} else {
		seedobj.constructor();
		seedobj.set_start_seed(seed);
		seedobj._isCustomRun = isCustomRun;
	}
	g_Manager->StartNewGame(pltype, challenge, seedobj, difficulty);
}

MOD_EXPORT void L_Isaac_RenderToWorld(Vector* out, Vector* pos) {
	const Vector WORLD_VIEWPORT_SIZE = Vector(338.0, 182.0);
	const Vector WORLD_RENDER_ORIGIN = Vector(60.0, 140.0);
	const float WORLD_TO_SCREEN_SCALE = 0.65f;

	Game& game = *g_Game;
	Room& room = *game._room;
	Vector screenSize = Vector(g_WIDTH, g_HEIGHT);

	Vector offset = game._screenShakeOffset + room._renderScrollOffset;
	Vector uiViewportTopLeft = (screenSize - WORLD_VIEWPORT_SIZE) * 0.5f;
	Vector worldLocalRenderPos = (*pos - offset) - uiViewportTopLeft;

	*out = worldLocalRenderPos / WORLD_TO_SCREEN_SCALE + WORLD_RENDER_ORIGIN;
}

MOD_EXPORT void L_Isaac_DrawLine(Vector* pos1, Vector* pos2, KColor* col1, KColor* col2, float thickness) {
	g_ShapeRenderer->RenderLine(pos1, pos2, col1, col2, thickness);
}

MOD_EXPORT void L_Isaac_DrawQuad(Vector* postl, Vector* postr, Vector* posbl, Vector* posbr, KColor* col, float thickness) {
	DestinationQuad quad; //TODO make a constructor for this
	quad._topLeft = *postl;
	quad._topRight = *postr;
	quad._bottomLeft = *posbl;
	quad._bottomRight = *posbr;

	g_ShapeRenderer->OutlineQuad(&quad, col, thickness);
}

std::wstring mb_to_wide(const char* input,size_t len) {
	std::wstring out;
	size_t size=MultiByteToWideChar(CP_UTF8, 0, input, len,nullptr,0);
	out.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, input, len, out.data(), size);
	return out;
};

std::string wide_to_mb(const wchar_t* input, size_t len) {
	std::string out;
	size_t size = WideCharToMultiByte(CP_UTF8, 0, input, len, nullptr, 0,NULL,NULL);
	out.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, input, len, out.data(), size, NULL, NULL);
	return out;
};

MOD_EXPORT bool L_Isaac_SetClipboard(const char* text) {
	if (!OpenClipboard(NULL)) {
		return false;
	}

	EmptyClipboard();
	size_t textLength = strlen(text);

	//allocate global memory to hold the text
	std::wstring converted_str=mb_to_wide(text,textLength);
	size_t allocsize = wcslen(converted_str.c_str()) + 1;
	allocsize *= sizeof(wchar_t);

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, allocsize);
	if (hData == NULL) {
		CloseClipboard();
		return false;
	}

	//lock the global memory to get a pointer to the data
	wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
	if (pszText == NULL) {
		CloseClipboard();
		GlobalFree(hData);
		return false;
	}
	wcscpy(pszText, converted_str.c_str()); //copy the text to the global memory
	GlobalUnlock(hData);//unlock the global memory
	SetClipboardData(CF_UNICODETEXT, hData);
	CloseClipboard();

	return true;
}

MOD_EXPORT const char* L_Isaac_GetClipboard() {
	static std::string clipboardText;

	if (!OpenClipboard(NULL)) {
		return nullptr;
	}

	HANDLE hData = GetClipboardData(CF_UNICODETEXT); //get the clipboard data handle
	if (hData == NULL) {
		CloseClipboard();
		return nullptr;
	}

	wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData)); 	//lock the handle to get a pointer to the data
	if (pszText == NULL) {
		CloseClipboard();
		return nullptr;
	}
	clipboardText = wide_to_mb(pszText,wcslen(pszText)+1);

	//unlock and close the clipboard
	GlobalUnlock(hData);
	CloseClipboard();

	return clipboardText.c_str();
}

MOD_EXPORT int L_Isaac_GetSubTypeByName(const char* name) {
	string text = string(name);
	if (XMLStuff.EntityData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.EntityData->GetNodeByName(text);
		if ((ent.count("subtype") > 0) && (ent["subtype"].length() > 0)) {
			return stoi(ent["subtype"]);
		}
	};
	return 0;
}

MOD_EXPORT void L_Isaac_PlayCutscene(unsigned int cutscene, bool shouldClear) {
	if (cutscene > 26) {
		string out;
		g_Game->GetConsole()->RunCommand("cutscene " + to_string(cutscene), &out, NULL);
		return;
	}
	g_Manager->ShowCutscene(cutscene, shouldClear, 0);
}

MOD_EXPORT unsigned int L_Isaac_GetRoomSpawnSeed() {
	return g_Game->GetCurrentRoomDesc()->SpawnSeed;
}

MOD_EXPORT Entity_NPC* L_Isaac_SpawnBoss(unsigned int type, unsigned int var, unsigned int sub, Vector* pos, Vector* vel, Entity* spawner, unsigned int seed) {
	Entity_NPC* ent = (Entity_NPC*)g_Game->Spawn(type, var, *pos, *vel, spawner, sub, seed, 0);
	ent->_isBoss = true;
	return ent;
}

MOD_EXPORT int L_Isaac_GetCutsceneByName(const char* name) {
	string text = string(name);
	if (XMLStuff.CutsceneData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.CutsceneData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			return stoi(ent["id"]);
		}
	};
	return -1;
}

MOD_EXPORT int L_Isaac_GetGiantBookByName(const char* name) {
	string text = string(name);
	if (XMLStuff.GiantBookData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.GiantBookData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			return stoi(ent["id"]);
		}
	};
	return -1;
}

MOD_EXPORT bool L_Isaac_CanStartTrueCoop() {
	return !PlayerManager::CoopBabiesOnly();
}

MOD_EXPORT int L_Isaac_GetNullItemIdByName(const char* nameString) {
	const string name = string(nameString);

	for (ItemConfig_Item* nullitem : *g_Manager->GetItemConfig()->GetNullItems()) {
		if (nullitem != nullptr && nullitem->name == name) {
			return nullitem->id;
		}
	}

	return -1;
}

MOD_EXPORT int L_Isaac_ShowErrorDialog(const char* title, const char* text, int icon, int buttons) {
	return MessageBoxA(NULL, text, title, icon | buttons);
}

MOD_EXPORT ANM2* L_Isaac_GetCursorSprite() {
	return &g_Manager->_cursorSprite;
}

bool apipause = false;
HOOK_STATIC(Manager, Update, (bool unk) -> void, __stdcall) {
	if (apipause) {
		g_Manager->_state = 2;
	}
	super(unk);

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

MOD_EXPORT void L_Isaac_GetRenderPosition(Vector* out, Vector* pos, bool scale) {
	*out = Isaac::GetRenderPosition(pos, scale);
}

MOD_EXPORT void L_Isaac_GetCollectibleSpawnPosition(Vector* out, Vector* pos) {
	*out = Isaac::GetCollectibleSpawnPosition(pos);
}

MOD_EXPORT void L_Isaac_TriggerWindowResize()
{
	g_Manager->ResizeWindow(g_WindowSizeX, g_WindowSizeY);
}

MOD_EXPORT void L_Isaac_CenterCursor()
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
};

MOD_EXPORT int L_Isaac_SetDwmWindowAttribute(int attribid, int attribval)
{
	HWND hwnd = GetActiveWindow();
	DWORD activeProcessId;
	GetWindowThreadProcessId(hwnd, &activeProcessId);
	DWORD currentProcessId = GetCurrentProcessId();

	switch (attribid) {
	case (DWMWINDOWATTRIBUTE)DWMWA_CLOAK:
		return 1;
	case (DWMWINDOWATTRIBUTE)DWMWA_CLOAKED:
		return 2;
	};
	if (activeProcessId == currentProcessId) {
		DwmSetWindowAttribute(hwnd, attribid, &attribval, sizeof(attribval));
	};

	return 0;
};

MOD_EXPORT int L_Isaac_GetDwmWindowAttribute(int attribid)
{
	HWND hwnd = GetActiveWindow();
	int32_t attribval;
	DwmGetWindowAttribute(hwnd, attribid,&attribval,sizeof(attribval));
	return attribval;
};

MOD_EXPORT void L_Isaac_SetWindowTitle(const char* text)
{
	if (!text) {
		REPENTOGON::SetStockWindowTitle();
		return;
	};
	char buffer[256];
	strncpy_s(REPENTOGON::moddedtitle, text, 255);
	strncpy_s(buffer, REPENTOGON::stocktitle, 255);
	strncat_s(buffer, REPENTOGON::moddedtitle, 255);
	SetWindowTextA(GetActiveWindow(), buffer);
};

MOD_EXPORT const char* L_Isaac_GetWindowTitle()
{
	return REPENTOGON::moddedtitle;
};

MOD_EXPORT bool L_Isaac_IsInGame() {
	return Isaac::IsInGame();
}

MOD_EXPORT bool L_Isaac_IsChallengeDone(int challengeid) {
	return IsChallengeCompleted(challengeid);
}

MOD_EXPORT void L_Isaac_ClearChallenge(int challengeid) {
	MarkChallengeCompleted(challengeid);
}

MOD_EXPORT void L_Isaac_UndoChallenge(int challengeid) {
	ResetChallengeCompletion(challengeid);
}

MOD_EXPORT int L_Isaac_GetModChallengeCompletion(const char* modidString, const char* challengenameString) {
	const string modid = modidString;
	const string challengename = challengenameString;

	const string key = challengename + modid;
	if (Challenges.count(key)) {
		return Challenges[key] > 0;
	}
	return -1;
}

MOD_EXPORT int L_Isaac_GetModChallengeClearCount(int challengeid) {
	XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeid);
	return Challenges[node["name"] + node["sourceid"]];
}

MOD_EXPORT int L_Isaac_GetBossColorIdxByName(const char* name) {
	string bosscolorname = name;
	auto iter = XMLStuff.BossColorData->childbyname.find(bosscolorname);
	if (iter == XMLStuff.BossColorData->childbyname.end()) {
		return -1;
	}
	return iter->second - 1;
}

MOD_EXPORT int L_Isaac_GetBackdropTypeByName(const char* name) {
	string text = string(name);
	if (XMLStuff.BackdropData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.BackdropData->GetNodeByName(text);
		if ((ent.end() != ent.begin()) && (ent.count("id") > 0) && (ent["id"].length() > 0)) {
			return stoi(ent["id"]);
		}
	};
	return -1;
}

// The returned string is only valid until the next call.
MOD_EXPORT const char* L_Isaac_GetChangelog() {
	static string text;
	text = "Changelog unavailable :(\n";
	ostringstream outtext;
	ifstream changelog;
	changelog.open("rgon_changelog.txt");
	if (changelog.is_open()) {
		outtext << changelog.rdbuf();
		text = outtext.str();
	};
	return text.c_str();
};

namespace {
	std::string currentIconPath = "";
	HANDLE currentIconSmall = NULL;
	HANDLE currentIconBig = NULL;
}

static void GetIconResolutions(bool ignorecap, int& smallIconResolution, int& bigIconResolution) {
	smallIconResolution = 16;
	bigIconResolution = std::min(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));

	if (ignorecap) {
		smallIconResolution = LR_DEFAULTSIZE;
		bigIconResolution = std::max(bigIconResolution, LR_DEFAULTSIZE);
	};
}

MOD_EXPORT void L_Isaac_SetIconID(int icontoset, bool ignorecap) {
	int smallIconResolution, bigIconResolution;
	GetIconResolutions(ignorecap, smallIconResolution, bigIconResolution);

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
	HANDLE icon = LoadImageA(GetModuleHandle(NULL), (LPCSTR)icontoset, IMAGE_ICON, smallIconResolution, smallIconResolution, 0);
	HANDLE icon_big = LoadImageA(GetModuleHandle(NULL), (LPCSTR)icontoset, IMAGE_ICON, bigIconResolution, bigIconResolution, 0);
	if (icon) {
		SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)icon);
		SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)icon_big);
	};
}

MOD_EXPORT bool L_Isaac_SetIconPath(const char* path, bool ignorecap) {
	int smallIconResolution, bigIconResolution;
	GetIconResolutions(ignorecap, smallIconResolution, bigIconResolution);

	std::string modpath = path;
	if (modpath.empty()) {
		return true;
	}
	std::string fullpath;
	g_Manager->GetModManager()->TryRedirectPath(&fullpath, &modpath);

	if (currentIconSmall == NULL || currentIconBig == NULL || currentIconPath.empty() || currentIconPath != fullpath) {
		HANDLE newIconSmall = LoadImageA(NULL, fullpath.c_str(), IMAGE_ICON, smallIconResolution, smallIconResolution, LR_LOADFROMFILE);
		if (newIconSmall == NULL) {
			return false;
		}
		HANDLE newIconBig = LoadImageA(NULL, fullpath.c_str(), IMAGE_ICON, bigIconResolution, bigIconResolution, LR_LOADFROMFILE);
		if (newIconBig == NULL) {
			return false;
		}
		if (currentIconSmall != NULL) {
			DestroyIcon((HICON)currentIconSmall);
		}
		if (currentIconBig != NULL) {
			DestroyIcon((HICON)currentIconBig);
		}
		currentIconSmall = newIconSmall;
		currentIconBig = newIconBig;
		currentIconPath = fullpath;
	}

	SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)currentIconSmall);
	SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)currentIconBig);

	return true;
};

MOD_EXPORT int L_Isaac_FindTargetPit(Vector* position, Vector* targetPosition, int pitIndex) {
	return Entity_NPC::FindTargetPit(position, targetPosition, pitIndex);
}

MOD_EXPORT void L_Isaac_GetAxisAlignedUnitVectorFromDir(Vector* out, int dir) {
	*out = Isaac::GetAxisAlignedUnitVectorFromDir(dir);
}

MOD_EXPORT void L_Isaac_StartDailyGame(unsigned int date) {
	// defer start to the manager
	// Note: At the moment we cannot properly free some of the memory allocated by the Seeds constructors by ourselves.
	// However, StartNewGame will handle it for us. Just be aware of this.
	Seeds seeds; seeds.constructor();
	g_Manager->StartNewGame(ePlayerType::PLAYER_ISAAC, eChallenge::CHALLENGE_NULL, seeds, 0);

	// setup daily challenge, so that the newly started game is treated as a daily challenge
	auto* dailyChallenge = g_Manager->GetDailyChallenge();
	dailyChallenge->Init(date);
	dailyChallenge->_isPractice = true;
}

static bool shuttingDown = false;
HOOK_STATIC(Isaac, Shutdown, () -> void, __cdecl) {
	shuttingDown = true;
	super();
}
MOD_EXPORT bool L_Isaac_IsShuttingDown() {
	return shuttingDown;
}

MOD_EXPORT ANM2* L_Isaac_GetButtonsSprite() {
	return &g_Manager->_buttonsSprite;
}

MOD_EXPORT int64_t L_Isaac_GetNanoTime()
{
	using clock = std::chrono::high_resolution_clock;
	auto now = clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
}

MOD_EXPORT bool L_Isaac_ReworkCollectible(int collectible)
{
	if (!(CollectibleType::COLLECTIBLE_NULL < collectible && collectible < CollectibleType::NUM_COLLECTIBLES))
	{
		return false;
	}

	if (ItemSpoofSystem::IsReworkedCollectible(collectible, -1)) {
		return true;
	}

	if (s_modsLoaded)
	{
		g_Game->GetConsole()->Print("[WARN] ReworkCollectible() ignored: Reworks can only be set during startup.\n", CONSOLE_COLOR_WARN, 0x96u);
		return true;
	}

	ItemSpoofSystem::ReworkCollectible(collectible);
	return true;
}

MOD_EXPORT bool L_Isaac_ReworkBirthright(int playerType)
{
	if (!(0 <= playerType && playerType < ePlayerType::NUM_PLAYER_TYPES))
	{
		return false;
	}

	if (ItemSpoofSystem::IsReworkedCollectible(COLLECTIBLE_BIRTHRIGHT, playerType)) {
		return true;
	}

	if (s_modsLoaded)
	{
		g_Game->GetConsole()->Print("[WARN] ReworkBirthright() ignored: Reworks can only be set during startup.\n", CONSOLE_COLOR_WARN, 0x96u);
		return true;
	}

	ItemSpoofSystem::ReworkBirthright(playerType);
	return true;
}

MOD_EXPORT bool L_Isaac_ReworkTrinket(int trinket)
{
	if (!(TrinketType::TRINKET_NULL < trinket && trinket < TrinketType::NUM_TRINKETS))
	{
		return false;
	}

	if (ItemSpoofSystem::IsReworkedTrinket(trinket)) {
		return true;
	}

	if (s_modsLoaded)
	{
		g_Game->GetConsole()->Print("[WARN] ReworkTrinket() ignored: Reworks can only be set during startup.\n", CONSOLE_COLOR_WARN, 0x96u);
		return true;
	}

	ItemSpoofSystem::ReworkTrinket(trinket);
	return true;
}

MOD_EXPORT bool L_Isaac_RenderCollectionItem(int itemID, Vector* pos, Vector* scale, ColorMod* color)
{
	if (!g_Manager->_itemConfig.GetCollectible(itemID)) {
		return false;
	}

	Vector posVec = *pos;
	Vector scaleVec = scale ? *scale : Vector(1, 1);
	ColorMod colorMod = color ? *color : ColorMod();
	BlendMode blendMode = BlendMode(1);

	g_Game->_gameOver.RenderItemSprite(itemID, &posVec, &colorMod, &scaleVec, &blendMode);

	return true;
}

// Returns true if the string can be used as a singular folder name (no sneaky things like "../folder" allowed).
bool IsSafeFolderName(std::string_view name) {
	// Reject "current directory" or "parent directory"
	if (name.empty() || name == "." || name == "..") {
		return false;
	}
	// Reject any string with slashes
	if (name.find('/') != std::string_view::npos || name.find('\\') != std::string_view::npos) {
		return false;
	}
	return true;
}

// The returned string is only valid until the next call. NULL if there is no data.
MOD_EXPORT const char* L_Isaac_LoadModDataFromFolder(const char* folderName) {
	static std::string data;

	int slot = g_Manager->_currentSaveSlot;
	if (slot < 0 || slot > 3 || !IsSafeFolderName(folderName)) {
		return nullptr;
	} else if (slot == 0) {
		// Isaac.LoadModData does this too
		slot = 1;
	}

	const std::string path = REPENTOGON::StringFormat("../data/%s/save%d.dat", folderName, slot);

	// Could have used a std::ifstream or something instead, but who knows maybe the consistency with Isaac.LoadModData means something
	KAGE_Filesys_File file;
	if (file.OpenRead(path.c_str()) && file.IsOpen()) {
		const long size = file.GetSize();
		data.assign(size + 1, '\0');
		file.Read(data.data(), 1, size);
		return data.c_str();
	}

	return nullptr;
}

MOD_EXPORT int L_Isaac_GetBabyIdByName(const char* nameString) {
	const string name = nameString;

	for (const EntityConfig_Baby& baby : *g_Manager->GetEntityConfig()->GetBabies()) {
		if (baby.name == name) {
			return baby.id;
		}
	}

	return -1;
}


//Deprecated methods

MOD_EXPORT void L_Isaac_ClearBossHazards(bool ignoreNPCs) {
	Entity_NPC* entity = nullptr;
	entity->ClearBossHazards(ignoreNPCs);

	g_Game->GetConsole()->Print("[WARN] Isaac.ClearBossHazards is deprecated. Use Room:ClearBossHazards instead", CONSOLE_COLOR_WARN, 0x96u);
}

MOD_EXPORT bool L_Isaac_CreateWeapon(int wepType, Entity* ent, Weapon** weapon) {
	if (!(WEAPON_NULL <= wepType && wepType < NUM_WEAPON_TYPES))
	{
		return false;
	}

	*weapon = Isaac::CreateWeapon((WeaponType)wepType, ent);
	return true;
}

MOD_EXPORT void L_Isaac_DestroyWeapon(Weapon* weapon) {
	Entity* owner = weapon->GetOwner();
	if (!owner) {
		return;
	}

	if (Entity_Player* player = owner->ToPlayer()) {
		for (int i = 0; i < 5; ++i) {
			if (*player->GetWeapon(i) == weapon) {
				Isaac::DestoryWeapon(player->GetWeapon(i));
				break;
			}
		}
	}
	else if (Entity_Familiar* familiar = owner->ToFamiliar()) {
		if (*familiar->GetWeapon() == weapon) {
			Isaac::DestoryWeapon(familiar->GetWeapon());
		}
	}
}

MOD_EXPORT void L_Isaac_DebugString(const char* text) {
	std::string str(text);
	LuaEngine::Isaac_DebugString(&str);
}

MOD_EXPORT Entity_Player* L_Isaac_GetPlayer(int index) {
	if (g_Game == nullptr || g_Game->_playerManager._playerList.size() == 0) {
		return nullptr;
	}
	return g_Game->GetPlayer((unsigned int)index);
}

MOD_EXPORT unsigned int L_Isaac_GetFrameCount() {
	return g_Manager->_framecount;
}

MOD_EXPORT unsigned int L_Isaac_GetChallenge() {
	return g_Game ? g_Game->_challenge : 0;
}

MOD_EXPORT Font* L_Isaac_GetTextFont() {
	return &g_Manager->_font5_terminus8;
}

MOD_EXPORT void L_Isaac_AddPillEffectToPool(int effect) {
	if (g_Game) {
		g_Game->_itemPool.ForceAddPillEffect(effect);
	}
}

MOD_EXPORT int L_Isaac_GetEntityTypeByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetEntityTypeByName(&str);
}

MOD_EXPORT int L_Isaac_GetEntityVariantByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetEntityVariantByName(&str);
}

MOD_EXPORT int L_Isaac_GetItemIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetItemIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetPlayerTypeByName(const char* name, bool isBSkin) {
	std::string str(name);
	return LuaEngine::Isaac_GetPlayerTypeByName(&str, isBSkin);
}

MOD_EXPORT int L_Isaac_GetCardIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetCardIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetPillEffectByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetPillEffectByName(&str);
}

MOD_EXPORT int L_Isaac_GetTrinketIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetTrinketIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetChallengeIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetChallengeIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetCostumeIdByPath(const char* path) {
	std::string str(path);
	return LuaEngine::Isaac_GetCostumeIdByPath(&str);
}

MOD_EXPORT int L_Isaac_GetCurseIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetCurseIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetSoundIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetSoundIdByName(&str);
}

MOD_EXPORT int L_Isaac_GetMusicIdByName(const char* name) {
	std::string str(name);
	return LuaEngine::Isaac_GetMusicIdByName(&str);
}

MOD_EXPORT unsigned int L_Isaac_GetTime() {
	return timeGetTime();
}

MOD_EXPORT const char* L_Isaac_ExecuteCommand(const char* command) {
	static std::string result;
	std::string cmd(command);

	alignas(std::string) char buffer[sizeof(std::string)];
	std::string* out = (std::string*)LuaEngine::Isaac_ExecuteCommand((std::string*)buffer, &cmd);
	result = *out;
	out->~basic_string();
	return result.c_str();
}

MOD_EXPORT void L_Isaac_ConsoleOutput(const char* text) {
	if (g_Game) {
		std::string str(text);
		g_Game->GetConsole()->Print(str, Console::Color::WHITE, 0x96u);
	}
}

MOD_EXPORT int L_Isaac_CountEntities(Entity* spawner, int type, int variant, int subtype) {
	if (g_Game == nullptr) {
		return 0;
	}
	return g_Game->_room->GetEntityList()->CountEntities(spawner, type, variant, subtype);
}

MOD_EXPORT float L_Isaac_GetScreenWidth() {
	return g_WIDTH;
}

MOD_EXPORT float L_Isaac_GetScreenHeight() {
	return g_HEIGHT;
}

MOD_EXPORT float L_Isaac_GetScreenPointScale() {
	return g_PointScale;
}

static LuaBridgeRef ModRef(int ref) {
	LuaBridgeRef modRef;
	modRef._state = g_LuaEngine->_state;
	modRef._ref = ref;
	return modRef;
}

MOD_EXPORT void L_Isaac_RegisterMod(int ref, const char* name, int apiVersion) {
	std::string str(name);
	LuaEngine::Isaac_RegisterMod(ModRef(ref), &str, apiVersion);
}

MOD_EXPORT void L_Isaac_SaveModData(int ref, const char* data) {
	std::string str(data);
	LuaEngine::Isaac_SaveModData(ModRef(ref), &str);
}

MOD_EXPORT const char* L_Isaac_LoadModData(int ref) {
	static std::string result;

	alignas(std::string) char buffer[sizeof(std::string)];
	std::string* out = (std::string*)LuaEngine::Isaac_LoadModData((std::string*)buffer, ModRef(ref));
	result = *out;
	out->~basic_string();
	return result.c_str();
}

MOD_EXPORT bool L_Isaac_HasModData(int ref) {
	return LuaEngine::Isaac_HasModData(ModRef(ref));
}

MOD_EXPORT void L_Isaac_RemoveModData(int ref) {
	LuaEngine::Isaac_RemoveModData(ModRef(ref));
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua_newtable(_state);
	timerFnTable = luaL_ref(_state, LUA_REGISTRYINDEX);

	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "CreateTimer", Lua_CreateTimer);
}
