#include "IsaacRepentance.h"

// No point in using a separate getter for DailyChallenge, we only export ChallengeParams.
MOD_EXPORT ChallengeParam* L_DailyChallenge_GetChallengeParams() {
	return &g_Manager->_dailyChallenge._params;
}