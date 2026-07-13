#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "Log.h"
#include <filesystem>
#include <algorithm>

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

//prevents joining lobbies
HOOK_METHOD(Menu_Game, UnknownJoinLobby, (int unk1, int unk2, int unk3) -> void) {

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
HOOK_METHOD(ModManager, ListMods, () -> void) {
	super();
	_modBanStatus = 3;
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
