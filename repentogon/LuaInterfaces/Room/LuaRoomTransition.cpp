#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	ANM2* L_RoomTransition_GetVersusScreenSprite() {
		return g_Game->GetRoomTransition()->GetVersusScreenSprite();
	}

	void L_RoomTransition_StartBossIntro(int bossID1, int bossID2) {
		g_Game->GetRoomTransition()->StartBossIntro(bossID1, bossID2);
	}

	int L_RoomTransition_GetTransitionMode() {
		return g_Game->GetRoomTransition()->GetTransitionMode();
	}

	bool L_RoomTransition_IsRenderingBossIntro() {
		return g_Game->GetRoomTransition()->IsRenderingBossIntro();
	}
}