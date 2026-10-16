#include "IsaacRepentance.h"

static const int s_VersusScreenPlayerExtraPortraitLayers[4][2] = {
	{5, 12},
	{18, 21},
	{19, 22},
	{20, 23},
};

// Previously, RoomTransition contained a single sprite for the first player's "extra portrait" (ie tainted eden's glitchy effect)
// In REP+, with online co-op showing all players in the versus screen, this was replaced with an std::map of layer (int) to ANM2
// For backward compatability this function returns a reference to player 1's extra portrait by default.
// Also note that normally the game does not play the co-op version of the VS screen for local co-op.
// I think it did for earlier versions of Rep+. It still loads all the (extra) portraits though, so mods could trigger the animaion.
MOD_EXPORT ANM2* L_RoomTransition_GetPlayerExtraPortraitSprite(int playerIndex) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	if (playerIndex < 0 || playerIndex > 3) {
		return nullptr;
	}

	// The "alt" layer is for the "no shake" version of the portrait.
	// Only one or the other is ever populated in the map, as it is recreated from scratch in StartBossIntro.
	int layer = s_VersusScreenPlayerExtraPortraitLayers[playerIndex][0];
	int altLayer = s_VersusScreenPlayerExtraPortraitLayers[playerIndex][1];

	auto& map = *roomTransition->GetExtraLayerANM2s();

	if (map.count(altLayer)) {
		return &map[altLayer];
	}
	else if (map.count(layer)) {
		return &map[layer];
	}
	else {
		return nullptr;
	}
}

MOD_EXPORT int L_RoomTransition_GetTransitionMode() {
	return g_Game->_roomTransition._mode;
}

MOD_EXPORT ANM2* L_RoomTransition_GetVersusScreenSprite() {
	return &g_Game->_roomTransition._versusScreenANM2;
}

MOD_EXPORT bool L_RoomTransition_IsRenderingBossIntro() {
	return g_Game->_roomTransition._mode == 2 && g_Game->_roomTransition._unkStartRoomTransitionCond != 0;
}

MOD_EXPORT void L_RoomTransition_StartBossIntro(unsigned int bossID1, unsigned int bossID2) {
	g_Game->_roomTransition._roomIndex = g_Game->_startingRoomIdx; // safety measure to prevent crashes if current transition's roomIndex is invalid
	g_Game->_roomTransition.StartBossIntro(bossID1, bossID2);
}