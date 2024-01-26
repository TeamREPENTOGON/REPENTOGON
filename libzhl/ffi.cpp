#include "IsaacRepentance.h"

#include "ffi.h"

extern "C" {
    void L_Spawn(int type, int variant, unsigned int subtype, void* pos) {
        printf("%u\n", subtype);
        g_Game->Spawn(type, variant, *(const Vector*)pos, Vector(0.0, 0.0), nullptr, subtype, 0, 0);
    }

    void L_Console_Print(const char* str) {
        g_Game->GetConsole()->Print(std::string(str), Console::Color::WHITE, 0x96U);
    }

    void L_Game_ClearDonationModGreed() {
        g_Game->ClearDonationModGreed();
    }

    unsigned int L_GetTime() {
        return timeGetTime();
    };

    unsigned int L_Random(unsigned int max) {
        return Isaac::Random(max);
    }
}