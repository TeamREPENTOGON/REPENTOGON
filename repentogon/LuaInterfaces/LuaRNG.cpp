#include "IsaacRepentance.h"

extern "C" {
	MOD_EXPORT const uint32_t* L_RNG_GetShiftsTable() {
		return &s_Shifts;
	}
}