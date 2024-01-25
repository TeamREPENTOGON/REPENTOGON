#include "IsaacRepentance.h"

#include "ffi.h"

extern "C" {
    void L_Spawn(int type, int variant, unsigned int subtype, void* pos) {
        printf("%u\n", subtype);
        g_Game->Spawn(type, variant, *(const Vector*)pos, Vector(0.0, 0.0), nullptr, subtype, 0, 0);
    }

    void L_Console_Print(const char* str) {
        g_Game->GetConsole()->Print(std::string(str), 0xffd3d3d3, 0x96);
    }

    unsigned int L_Random(unsigned int max) {
        return Isaac::Random(max);
    }
}