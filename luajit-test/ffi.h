#include "IsaacRepentance.h"

extern "C" {
    __declspec(dllexport) Entity* L_Spawn(int type, int variant, const Vector* pos, const Vector* vel, Entity* spawner, unsigned int subtype, unsigned int seed, unsigned int unk);
}