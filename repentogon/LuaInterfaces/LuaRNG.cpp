#include "IsaacRepentance.h"

extern "C" {
	MOD_EXPORT const uint32_t* L_RNG_GetShiftsTable() {
		return &s_Shifts;
	}

	MOD_EXPORT unsigned int L_Random() {
		return Isaac::genrand_int32();
	}
}