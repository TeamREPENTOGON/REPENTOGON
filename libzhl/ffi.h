#pragma once

#include <stdio.h>
#include "IsaacRepentance.h"

extern "C" {
    LIBZHL_API void L_Console_Print(const char* str);
    LIBZHL_API unsigned int L_Random(unsigned int max);
    LIBZHL_API void L_Spawn(int type, int variant, unsigned int subtype, void* pos);
}