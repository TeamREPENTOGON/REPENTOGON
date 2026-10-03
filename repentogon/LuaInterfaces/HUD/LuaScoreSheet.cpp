#include "IsaacRepentance.h"

MOD_EXPORT ScoreSheet* L_ScoreSheet_Get() {
	return &g_Game->_scoreSheet;
}

MOD_EXPORT void L_ScoreSheet_Calculate() {
	g_Game->_scoreSheet.Calculate();
}

MOD_EXPORT void L_ScoreSheet_AddFinishedStage(int stage, int stageType, unsigned int time) {
	g_Game->_scoreSheet.AddFinishedStage(stage, stageType, time, false);
}
