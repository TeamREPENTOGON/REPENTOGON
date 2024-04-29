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
	if (this->_isFollower && this->_parent == nullptr && this->_child != nullptr && this->_child->_type == 3 && ((Entity_Familiar*)(this->_child))->_isFollower) {
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
	if (this->_isDelayed && this->_parent == nullptr && this->_child != nullptr && this->_child->_type == 3 && ((Entity_Familiar*)(this->_child))->_isDelayed) {
		// Mimic the message the game would usually print to the log in this case.
		KAGE::_LogMessage(0, "[warn] Tried to add familiar %d to delayed again!\n", this->_variant);
		return;
	}
	super();
}
bool exception_message_shown = false;
HOOK_GLOBAL_PRIORITY(RepCaughtException, 9999, (int x)->void,__cdecl) {
	char message[1024]="Something went wrong in the vanilla code! Lua traceback unavailable!";
	if (!exception_message_shown) {
		if (g_LuaEngine != nullptr) {
			lua_Debug debuginfo;
			lua_getstack(g_LuaEngine->_state, 0, &debuginfo);
			lua_getinfo(g_LuaEngine->_state, "Sln", &debuginfo);
			snprintf(message, 1024, "Something went wrong in the vanilla code!\nHere's some Lua traceback!\n%s\n%s\n", debuginfo.short_src, debuginfo.namewhat);
		}
		printf("hi! i live in your exception handler! ;3\n");
		MessageBoxA(nullptr, message, "A super-basic crash handler!", MB_OK);
		exception_message_shown = true;
	};
	super(x);
};
