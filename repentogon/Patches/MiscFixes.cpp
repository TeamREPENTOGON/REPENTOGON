#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"

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

// Fix crash if nil is passed as the string from luaside.
HOOK_METHOD(Font, DrawString, (const char* str, Vector pos, Vector scale, KColor* color, FontSettings* settings) -> void) {
	if (str) {
		super(str, pos, scale, color, settings);
	}
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