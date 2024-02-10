#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {

	ANM2* L_NightmareScene_GetBackgroundSprite() {
		return g_Manager->GetNightmareScene()->GetBackgroundSprite();
	}

	ANM2* L_NightmareScene_GetBubbleSprite() {
		return g_Manager->GetNightmareScene()->GetBubbleSprite();
	}

	ANM2* L_NightmareScene_GetProgressBarSprite() {
		return g_Manager->GetNightmareScene()->GetProgressBarSprite();
	}

	int* L_NightmareScene_GetProgressBarMap() {
		return g_Manager->GetNightmareScene()->GetProgressBarMap();
	}

	bool L_NightmareScene_IsDogmaNightmare() {
		return g_Manager->GetNightmareScene()->IsDogmaNightmare();
	}
}