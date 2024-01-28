#include "IsaacRepentance.h"

#include "ffi.h"

extern "C" {

    
    // CONSOLE
    void L_Console_Print(const char* str, unsigned int color) {
        g_Game->GetConsole()->Print(std::string(str), color, 0x96U);
    }

    void L_Console_PrintError(const char* str) {
        g_Game->GetConsole()->PrintError(str);
    }


    void L_Spawn(int type, int variant, unsigned int subtype, void* pos) {
        printf("%u\n", subtype);
        g_Game->Spawn(type, variant, *(const Vector*)pos, Vector(0.0, 0.0), nullptr, subtype, 0, 0);
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

    void L_KAGE_LogMessage(const char* str) {
        KAGE::LogMessage(0, str);
    }
}