#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "Log.h"
#include <filesystem>
#include <algorithm>
#include <chrono>

#undef max

// The GameOver class stores an EntityConfig_Entity reference for the modded entity that the player dies to,
// in order to access the mod's death portraits. However, it does not clear this reference when the
// player then dies to a vanilla entity, and still renders it, causing overlapping sprites.
// Fix this by clearing the EntityConfig_Entity pointer before the death screen is initialized.
HOOK_METHOD(GameOver, Show, () -> void) {
	this->modEntityConfig = nullptr;
	super();
}

// GetNullFrame will print an error to the console if AnimationData is null, but will continue on trying to use it anyway, sometimes crashing.
// The game actually does check for null AnimationData and return nullptr if so, but only in the ANM2 class's wrappers for these functions.
// Since we needed to use the AnimationState functions directly for access to the NullFrames of overlay animations, these hooks prevent the
// functions from running if AnimationData is null, preventing possible crashes.
HOOK_METHOD(AnimationState, GetNullFrame, (const char* nullLayerName) -> NullFrame*) {
	if (this->_animData == nullptr) return nullptr;
	return super(nullLayerName);
}
HOOK_METHOD(AnimationState, GetNullFrameByID, (int nullLayerID) -> NullFrame*) {
	if (this->_animData == nullptr) return nullptr;
	return super(nullLayerID);
}

// If AddToFollowers is called on the familiar currently at the front of the chain, it will try to add the familiar to the chain again,
// as if it were not already a part of it. If the game decides to place the familiar second-from-the-front, it will Parent to itself.
// This causes the chain to stop moving, and can also lead to infinite loops when the game tries to iterate over the chain.
// The game does have a check to try to prevent this, but it checks if the familiar already has a Parent that is either a familiar
// or the player. The problem is that the familiar at the front of the chain does not parent to the player, its parent is always null.
// This hook adds a safety check to skip the call if the familiar is already at the front of the chain.
HOOK_METHOD(Entity_Familiar, AddToFollowers, () -> void) {
	// Skip call if this familiar is a follower with a null Parent & a Child that is a follower familiar.
	// This sufficiently indicates that the familiar is at the front of the chain already.
	if (this->_isFollower && this->GetParent() == nullptr && this->GetChild() != nullptr && this->GetChild()->_type == ENTITY_FAMILIAR && ((Entity_Familiar*)(this->GetChild()))->_isFollower) {
		// Mimic the message the game would usually print to the log in this case.
		KAGE::_LogMessage(0, "[warn] Tried to add familiar %d to followers again!\n", this->_variant);
		return;
	}
	super();
}
// Hook a similar safety check into AddToDelayed, though this one was much less likely to trigger a bug (but still possible).
HOOK_METHOD(Entity_Familiar, AddToDelayed, () -> void) {
	// Skip call if this a "delayed" familiar with a null Parent & a Child that is a "delayed" familiar.
	// This sufficiently indicates that the familiar is at the front of the chain already.
	if (this->_isDelayed && this->GetParent() == nullptr && this->GetChild() != nullptr && this->GetChild()->_type == ENTITY_FAMILIAR && ((Entity_Familiar*)(this->GetChild()))->_isDelayed) {
		// Mimic the message the game would usually print to the log in this case.
		KAGE::_LogMessage(0, "[warn] Tried to add familiar %d to delayed again!\n", this->_variant);
		return;
	}
	super();
};

// Fix mods folder redir for fonts
HOOK_METHOD(Font, Load, (char const* path, bool unusedIsLoading) -> void) {
	std::string newPath = path;
	std::string lower = newPath;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	if (lower._Starts_with("mods/") || lower._Starts_with("mods\\"))
	{
		char buffer[65535];
		DWORD len = GetModuleFileNameA(NULL, buffer, 65535);
		std::filesystem::path newerPath = std::filesystem::path(std::string(buffer, len));
		newPath = newerPath.parent_path().parent_path().string() + "/" + newPath;
		return super(newPath.c_str(), unusedIsLoading);
	}
	super(path, unusedIsLoading);
}


// Fix crash if nil is passed as the string from luaside.
HOOK_METHOD(Font, DrawString, (const char* str, Vector pos, Vector scale, KColor* color, FontSettings* settings) -> void) {
	if (str) {
		super(str, pos, scale, color, settings);
	}
}

//Fix nasty behaviour when calling getgrident with a negative value
HOOK_METHOD(Room, GetGridEntity, (unsigned int idx) -> GridEntity*) {
	if (idx < 0) {
		return 0;
	}
	return super(idx);
}


// Do nothing for invalid PillColor values.
HOOK_METHOD(ItemPool, IdentifyPill, (uint32_t pillColor) -> void) {
	if ((pillColor & PILL_COLOR_MASK) < NUM_PILLS) {
		super(pillColor);
	}
}
HOOK_METHOD(ItemPool, IsPillIdentified, (uint32_t pillColor) -> bool) {
	if ((pillColor & PILL_COLOR_MASK) < NUM_PILLS) {
		return super(pillColor);
	}
	return false;
}

// Fixes rendering bugs caused by starting a new render operation before presenting the previous one.
HOOK_STATIC(Rendering, PushCurrentRenderTarget, () -> void, __stdcall)
{
	KAGE_Graphics_ImageManager& imageManager = g_KAGE_Graphics_ImageManager;
	if (!(imageManager._frameImages.empty() && imageManager._transparentBatches.empty()))
	{
		g_KAGE_Graphics_Manager.Present();
	}

	super();
}

// Set patched out deselectable buttons on the online and daily menus to render at 0.5 alpha.
HOOK_METHOD(Menu_Online, Render, () -> void) {
	int layers[4] = {1, 2, 3, 14};
	for (int layer : layers) {
		this->_anm2.GetLayer(layer)->_color._tint[3] = 0.5;
	}

	KColor fontColor(0.21f, 0.18f, 0.18f, 1.f);
	FontSettings settings; 
	settings._align = 1; 

	Vector pos = Vector(g_MenuManager->_ViewPosition.x - g_MenuManager->_viewPositionSet[19].x + 330, g_MenuManager->_ViewPosition.y - g_MenuManager->_viewPositionSet[19].y + 220);

	super();
	g_Manager->_font1_TeamMeatEx10.DrawString("Change the Launch Mode\n to Vanilla, in the launcher options\n to play Online.", pos, Vector(1, 1), &fontColor, &settings);
};

// This one is easier, since the ANM2 already has a frame with the right alpha set.
HOOK_METHOD(Menu_DailyChallenge, Render, () -> void) {
	this->_DailyRunSprite.SetLayerFrame(3, 2);
	super();
}

// [ONLINE-EXPERIMENT] Instrumentation only, no behavior change.
// Logs each time the Online menu's internal state changes (eg. first reached, sub-screen changes),
// using the _prevState/_state fields already exposed in the Menu_Online ZHL struct, to avoid
// spamming the log every single frame this menu is open.
HOOK_METHOD(Menu_Online, Update, () -> void) {
	int stateBefore = this->_state;
	super();
	if (this->_state != stateBefore) {
		ZHL::Log("[ONLINE] -> menu reached: Menu_Online state changed %d -> %d (prevState=%d)\n", stateBefore, this->_state, this->_prevState);
	}
}

// [ONLINE-TRACE] Shared state for correlating online_mods_check()==false with what Menu_Game and
// Menu_Title do immediately afterward. onlineCheckFalseWindowActive/Start are set by the
// online_mods_check hook further below; lastMenuGameUpdateTime is set by Menu_Game::Update below
// and read by Menu_Title::Update further below to compute gapFromMenuGame.
static std::chrono::steady_clock::time_point onlineCheckFalseWindowStart{};
static bool onlineCheckFalseWindowActive = false;
static std::chrono::steady_clock::time_point lastMenuGameUpdateTime{};

// [HOOK-ORDER experiment] TEMPORARY instrumentation only, no behavior change to any hook.
// Shared (non-static, external linkage) so CutsceneSkip.cpp, ModsMenuStuff.cpp and XMLData.cpp
// can log into the same call-grouping counter/time window from their own existing
// HOOK_METHOD(MenuManager, Update, ...) hooks, via extern declarations added at each of those
// call sites. Goal: determine the real runtime nesting order of the 4 hooks currently on
// MenuManager::Update, and see exactly which one (if any) turns _selectedMenuID from 19 into 1.
//
// g_MMUpdateDepth counts how many of the 4 hooks are currently nested inside each other for the
// SAME logical MenuManager::Update() invocation (super() chains them); a transition from depth 0
// to 1 marks the start of a brand new invocation, so g_MMUpdateCallID is only incremented then -
// whichever hook happens to run outermost gets it "for free", without needing to know in advance
// which one that is.
int g_MMUpdateDepth = 0;
int g_MMUpdateCallID = 0;
std::chrono::steady_clock::time_point g_HookOrderWindowStart{};
bool g_HookOrderWindowActive = false;

int HookOrder_Enter(const char* fileTag, int selectedMenuID) {
	if (g_MMUpdateDepth == 0) {
		g_MMUpdateCallID++;
	}
	g_MMUpdateDepth++;
	if (g_HookOrderWindowActive) {
		long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_HookOrderWindowStart).count();
		if (ms <= 1000) {
			ZHL::Log("[HOOK-ORDER] UPDATE#%d %s ENTER selected=%d\n", g_MMUpdateCallID, fileTag, selectedMenuID);
		} else {
			g_HookOrderWindowActive = false;
		}
	}
	return g_MMUpdateCallID;
}
void HookOrder_BeforeSuper(const char* fileTag, int callID, int selectedMenuID) {
	if (g_HookOrderWindowActive) {
		ZHL::Log("[HOOK-ORDER] UPDATE#%d %s BEFORE_SUPER selected=%d\n", callID, fileTag, selectedMenuID);
	}
}
void HookOrder_AfterSuper(const char* fileTag, int callID, int selectedMenuID) {
	if (g_HookOrderWindowActive) {
		ZHL::Log("[HOOK-ORDER] UPDATE#%d %s AFTER_SUPER selected=%d\n", callID, fileTag, selectedMenuID);
	}
	g_MMUpdateDepth--;
}

// [ONLINE-TRACE] Instrumentation only, no behavior change.
// Logs Menu_Game's main list cursor and SelectedElement only when either changes across an
// Update() call (unchanged from before), plus a short windowed trace: for up to 2 seconds after
// online_mods_check() returns false, every Menu_Game::Update() call is logged with its `this`
// pointer, to determine whether the Menu_Game object keeps updating, stops, or is a different
// instance. The offset 0x22C field is not a named field in MenuGame.zhl, but its address is
// directly evidenced by ASMPatchOnlineSelection (ASMMenu.cpp); we reuse that known offset as-is,
// we do not re-assume what it represents.
HOOK_METHOD(Menu_Game, Update, () -> void) {
	int stateBefore = *(int*)((char*)this + 0x22C);
	int selectedBefore = this->SelectedElement;

	super();

	int stateAfter = *(int*)((char*)this + 0x22C);
	int selectedAfter = this->SelectedElement;

	auto now = std::chrono::steady_clock::now();
	lastMenuGameUpdateTime = now;

	if (onlineCheckFalseWindowActive) {
		long long elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - onlineCheckFalseWindowStart).count();
		if (elapsedMs <= 2000) {
			ZHL::Log("[ONLINE-TRACE] Menu_Game::Update window\nselected=%d\nstate=%d\nthis=%p\n", selectedAfter, stateAfter, (void*)this);
		} else {
			onlineCheckFalseWindowActive = false;
		}
	}

	if (stateAfter != stateBefore || selectedAfter != selectedBefore) {
		ZHL::Log("[ONLINE-TRACE] Menu_Game::Update state %d->%d selected %d->%d\n", stateBefore, stateAfter, selectedBefore, selectedAfter);
	}
}

// [ONLINE-TRACE] Helper: human-readable name for MenuManager::_selectedMenuID values, per the
// real, documented eMainMenuType enum (libzhl/functions/Global.zhl). Used only for log clarity.
static const char* GetMainMenuTypeName(int id) {
	switch (id) {
		case 0: return "NONE(0, unusable per Global.zhl comment)";
		case 1: return "TITLE";
		case 2: return "SAVES";
		case 3: return "GAME";
		case 4: return "DAILYRUN";
		case 5: return "CHARACTER";
		case 6: return "SPECIALSEEDS";
		case 7: return "CHALLENGE";
		case 8: return "COLLECTION";
		case 9: return "STATS";
		case 10: return "OPTIONS";
		case 11: return "CONTROLS";
		case 12: return "KEYCONFIG";
		case 13: return "ENDINGS";
		case 14: return "BESTIARY";
		case 15: return "MODCHALLENGES";
		case 16: return "MODS";
		case 17: return "ONLINELOBBY";
		case 18: return "FRIENDLOBBIES";
		case 19: return "MULTIPLAYER";
		case 20: return "OPTIONSONLINE";
		case 21: return "CREATELOBBY";
		case 22: return "ONLINEAWARDS";
		default: return "UNKNOWN";
	}
}

// [ONLINE-TRACE] Instrumentation only, no behavior change.
// MenuManager::_selectedMenuID (0x40, MenuManager.zhl) is the real, documented field for the
// currently active top-level menu screen (per eMainMenuType in Global.zhl, and per its existing
// use throughout the codebase, eg. ConsoleHooks.cpp's "inTitleOrFileSelect" check). We log only
// changes to this field, instead of _state/_returnState (already confirmed to never change).
// MenuManager::Update is already hooked elsewhere (CutsceneSkip.cpp, ModsMenuStuff.cpp,
// XMLData.cpp) for unrelated purposes - notably XMLData.cpp's hook unconditionally resets
// _selectedMenuID back to 1 (TITLE) whenever it is 4/17/18/19/21 (DAILYRUN/ONLINELOBBY/
// FRIENDLOBBIES/MULTIPLAYER/CREATELOBBY), labeled "Menu Bug Crash fix and backwards compat". ZHL
// chains multiple hooks via super(), so adding this one does not remove or duplicate that hook -
// but it is the strongest known candidate for explaining the return to title, so this log is
// intended to directly confirm or refute it.
HOOK_METHOD(MenuManager, Update, () -> void) {
	int stateBefore = this->_state;
	int returnStateBefore = this->_returnState;
	int selectedMenuIDBefore = this->_selectedMenuID;

	// [HOOK-ORDER experiment] TEMPORARY, no behavior change.
	int hookOrderCallID = HookOrder_Enter("MiscFixes.cpp", selectedMenuIDBefore);
	HookOrder_BeforeSuper("MiscFixes.cpp", hookOrderCallID, this->_selectedMenuID);
	super();
	HookOrder_AfterSuper("MiscFixes.cpp", hookOrderCallID, this->_selectedMenuID);

	int stateAfter = this->_state;
	int returnStateAfter = this->_returnState;
	int selectedMenuIDAfter = this->_selectedMenuID;

	if (stateAfter != stateBefore) {
		ZHL::Log("[ONLINE-TRACE] MenuManager::_state old=%d new=%d\n", stateBefore, stateAfter);
	}
	if (returnStateAfter != returnStateBefore) {
		ZHL::Log("[ONLINE-TRACE] MenuManager::_returnState old=%d new=%d\n", returnStateBefore, returnStateAfter);
	}
	if (selectedMenuIDAfter != selectedMenuIDBefore) {
		ZHL::Log("[ONLINE-TRACE] MenuManager::_selectedMenuID old=%d(%s) new=%d(%s)\n",
			selectedMenuIDBefore, GetMainMenuTypeName(selectedMenuIDBefore),
			selectedMenuIDAfter, GetMainMenuTypeName(selectedMenuIDAfter));
	}
}

// [ONLINE-TRACE] Instrumentation only, no behavior change.
// MenuManager::Init() is a real, confirmed signature (MenuManager.zhl) and is not currently
// hooked anywhere else in the codebase. Logging its entry tells us whether MenuManager is being
// reconstructed/reinitialized (as opposed to just changing _state) when returning to the title
// screen. Init() takes no parameters, so there is nothing else to log.
HOOK_METHOD(MenuManager, Init, () -> void) {
	ZHL::Log("[ONLINE-TRACE] MenuManager::Init ENTER\n");
	super();
}

// [ONLINE-TRACE] Instrumentation only, no behavior change.
// Menu_Title::Update() runs every frame while the title screen is active/rendering, so we cannot
// log every call. We log only: (a) the very first call this session, and (b) any call that
// follows a gap of more than 500ms since the previous call - ie. the title screen was NOT being
// updated for a while and has just become active again. Also logs `this` and the time elapsed
// since Menu_Game's own last Update() call (gapFromMenuGame), to correlate ownership handoff
// between the two menus. No Menu_Title::Init/constructor/destructor signature exists in
// MenuTitle.zhl (only Update and Render are documented), so we do not hook anything there - only
// Update is a real, confirmed signature.
static std::chrono::steady_clock::time_point lastMenuTitleUpdateTime{};
static bool hasLoggedFirstMenuTitleUpdate = false;

HOOK_METHOD(Menu_Title, Update, () -> void) {
	auto now = std::chrono::steady_clock::now();

	if (!hasLoggedFirstMenuTitleUpdate) {
		ZHL::Log("[ONLINE-TRACE] Menu_Title ACTIVE\nthis=%p\n(first Update() call this session)\n", (void*)this);
		hasLoggedFirstMenuTitleUpdate = true;
	} else {
		long long gapMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMenuTitleUpdateTime).count();
		if (gapMs > 500) {
			long long gapFromMenuGameMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMenuGameUpdateTime).count();
			ZHL::Log("[ONLINE-TRACE] Menu_Title ACTIVE\nthis=%p\ngapFromMenuGame=%lldms\n(gap %lldms since its own last Update() call)\n", (void*)this, gapFromMenuGameMs, gapMs);
		}
	}
	lastMenuTitleUpdateTime = now;

	super();
}

// [ONLINE-TRACE] Instrumentation only, no behavior change.
// Isaac::Shutdown is a real, confirmed static signature (Isaac.zhl) already hooked elsewhere
// (ImGui.cpp, LuaIsaac.cpp) for unrelated purposes; ZHL chains multiple hooks via super(), so
// this one does not remove or duplicate those. Logging only its entry lets us distinguish a real
// process shutdown (this line present) from a menu transition back to the title screen (this
// line absent) in the log, per the task's request.
HOOK_STATIC(Isaac, Shutdown, () -> void, __cdecl) {
	ZHL::Log("[ONLINE-TRACE] GAME SHUTDOWN\n");
	super();
}

// [ONLINE-EXPERIMENT] Instrumentation only, no behavior change.
// Menu_Game::online_mods_check is a native Repentance+ function (signature already known via
// MenuGame.zhl) that REPENTOGON does not otherwise hook or alter. Logging its entry/return value
// tells us whether the game's own mod-gating check is what actually rejects REPENTOGON for online,
// independently of REPENTOGON's own menu/lobby blocks. When it returns false, we also open the
// 2-second Menu_Game::Update observation window (see onlineCheckFalseWindowActive/Start above).
//
// On identifying the caller: we deliberately do NOT attempt to log a return address here. ZHL's
// HOOK_METHOD mechanism inserts this hook via a detour/trampoline between the real call site and
// this function body, so a caller-address intrinsic (eg. _ReturnAddress()) taken from inside this
// hook would report an address inside ZHL's own hook plumbing, not the real game code that
// logically called online_mods_check() - that would be misleading, not reliable, so we skip it.
HOOK_METHOD(Menu_Game, online_mods_check, () -> bool) {
	ZHL::Log("[ONLINE] -> online_mods_check: ENTER\n");
	bool result = super();
	ZHL::Log("[ONLINE] -> online_mods_check: EXIT result=%d\n", result ? 1 : 0);
	if (!result) {
		onlineCheckFalseWindowStart = std::chrono::steady_clock::now();
		onlineCheckFalseWindowActive = true;
		// [HOOK-ORDER experiment] TEMPORARY, no behavior change - opens the 1s hook-order window.
		g_HookOrderWindowStart = onlineCheckFalseWindowStart;
		g_HookOrderWindowActive = true;
	}
	return result;
}

//prevents joining lobbies
// [ONLINE-EXPERIMENT] Instrumented only. Behavior unchanged: the hook body is still empty,
// so the game's native lobby-join handling is still skipped entirely. This logging exists
// solely to confirm whether the game even reaches this call during an online attempt.
HOOK_METHOD(Menu_Game, UnknownJoinLobby, (int unk1, int unk2, int unk3) -> void) {
	ZHL::Log("[ONLINE] -> lobby/join attempt: Menu_Game::UnknownJoinLobby ENTER (unk1=%d, unk2=%d, unk3=%d)\n", unk1, unk2, unk3);
	ZHL::Log("[ONLINE] -> lobby/join attempt: Menu_Game::UnknownJoinLobby EXIT (no-op, original body intentionally left empty by REPENTOGON)\n");
}

//Prints log message about redirected configs
HOOK_METHOD(ModManager, TryRedirectPath, (std_string* result, std_string* filePath) -> void) {
	super(result, filePath);

	auto suffixRes = [](std::string s) {
		if (s.rfind(".xml") != std::string::npos)
			return true;
		else
			return false;
	};

	if (!result->empty() && result->compare(*filePath) != 0 && suffixRes(*result)) {
		KAGE::_LogMessage(0, "[warn] Redirected .xml config %s\n", result->c_str());
	}
}

//prevents playing online modes
// [ONLINE-EXPERIMENT] Causality test: instead of forcing _modBanStatus = 3, preserve whatever
// value the game's own ListMods logic computed (postCallBanStatus), to measure whether
// online_mods_check() still rejects online with the native value. Logging unchanged.
HOOK_METHOD(ModManager, ListMods, () -> void) {
	int preCallBanStatus = _modBanStatus;
	ZHL::Log("[ONLINE] -> mod check: ModManager::ListMods ENTER, _modBanStatus (original, pre-super) = %d\n", preCallBanStatus);

	super();

	int postCallBanStatus = _modBanStatus;
	ZHL::Log("[ONLINE] -> mod check: ModManager::ListMods after game logic (post-super), _modBanStatus = %d\n", postCallBanStatus);

	_modBanStatus = postCallBanStatus;
	ZHL::Log("[ONLINE] -> mod check: ModManager::ListMods REPENTOGON forces _modBanStatus = %d (unchanged behavior)\n", _modBanStatus);
}

// Fixes game crashing when spawning an entity with a seed of 0.
// Since Game::Spawn is inlined in some places, and vanilla spawns can still end up with a seed of 0,
// we enforce a valid seed in the callback function, as all spawns ultimately pass through it.
HOOK_STATIC(LuaEngine, Callback_PreEntitySpawn, (int* type, int* variant, Vector* position, Vector* velocity, Entity* spawner, int* subType, uint32_t* seed) -> void, __stdcall)
{
	*seed = std::max(*seed, 1U); // avoid mods getting a seed of 0
	super(type, variant, position, velocity, spawner, subType, seed);
	*seed = std::max(*seed, 1U); // in case some mod changed the seed.
}

// While rare, crawlspaces in the mineshaft can cause softlocks if the mineshaft room involves keys.
// Versions of Rep+ after v1.9.7.12 don't allow them to spawn.
HOOK_METHOD(GridEntity_Rock, TrySpawnLadder, () -> bool) {
	if (g_Game->_room->HasCurseMist()) {
		return false;
	}
	return super();
}
HOOK_METHOD(Room, SpawnGridEntity, (int gridIndex, unsigned int type, unsigned int variant, unsigned int seed, int vardata) -> bool) {
	if (type == GRID_STAIRS && g_Game->_room->HasCurseMist()) {
		return false;
	}
	return super(gridIndex, type, variant, seed, vardata);
}
HOOK_METHOD(Room, SpawnGridEntityDesc, (int gridIndex, GridEntityDesc* desc) -> bool) {
	if (desc && desc->_type == GRID_STAIRS && g_Game->_room->HasCurseMist()) {
		return false;
	}
	return super(gridIndex, desc);
}

// Fixes a bug in Rep+ v1.9.7.12 where Bethany in the Blood Mary challenge has their innate Book Of Virtues, which was unintended and fixed in a later patch.
// I think this happened because this function was newly created in Rep+ due to some refactoring.
HOOK_METHOD(Entity_Player, HasInnateCollectible, (int collectibleType, int unused) -> bool) {
	if (this->GetPlayerType() == PLAYER_BETHANY && collectibleType == COLLECTIBLE_BOOK_OF_VIRTUES && g_Game->GetChallenge() == CHALLENGE_BLOODY_MARY) {
		return false;
	}
	return super(collectibleType, unused);
}

// The game will attempt to update a mod's metadata.xml if it is missing a name/directory/version, or if it is missing entirely.
// However, for workshop mods, because WriteMetadata uses the "_directory" value, which at this point in time has not been appended
// with the workshop ID, the game will create a new folder for "modname" instead of "modname_123456" and write the metadata there,
// creating an "empty" duplicate of the mod containing nothing but this "fixed" metadata.xml. In practice this only happens if a mod
// is uploaded without a version, which is rare but can happen! Shoutout to "Minecraft Explosions"! (And Sacrilege, initially!!)
//
// Anyway there is no reason for this function to ever create a new folder. The practical purpose for this function is probably to
// auto-generate a default metadata xml for local mods that are lacking one, or to populate a version for devs who've neglected to
// add one to their XML (though ig some people are working around this anyway lmao).
//
// So, skip any call to this function that would write to a folder that doesn't already exist, since it is not going to do anything useful.
// Actually fixing the attempted write would not be worth the effort or potential bugs.
HOOK_METHOD(ModEntry, WriteMetadata, () -> void) {
	std::filesystem::path basePath(&g_ModdingDataPath);
	if (!std::filesystem::exists(basePath / this->_directory)) {
		return;
	}
	super();
}
