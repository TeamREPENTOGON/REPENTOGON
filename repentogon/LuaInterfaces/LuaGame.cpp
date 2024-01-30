#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_Game_ClearDonationModGreed() {
		g_Game->ClearDonationModGreed();
	}
}