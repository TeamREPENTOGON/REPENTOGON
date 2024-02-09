#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_StageTransition_SetSame(bool sameStage) {
		g_Game->GetStageTransition()->_sameStage = sameStage;
	}
	
	bool L_StageTransition_GetSame() {
		return g_Game->GetStageTransition()->_sameStage;
	}
}