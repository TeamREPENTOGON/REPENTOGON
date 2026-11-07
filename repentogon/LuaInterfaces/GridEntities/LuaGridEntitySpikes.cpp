#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntitySpikes_Update(GridEntity_Spikes* spikes) {
		spikes->Update();
	}
}