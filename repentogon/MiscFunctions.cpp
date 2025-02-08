#include "MiscFunctions.h"

namespace REPENTOGON
{
    GameStateSlot GetGameStateSlot(GameState* state)
    {
        if (g_Manager == nullptr || g_Game == nullptr)
        {
            return GameStateSlot::NULL_SLOT;
        }

        static const std::map<uintptr_t, GameStateSlot> slotMap = {
            { (uintptr_t)&g_Manager->_gamestate, GameStateSlot::SAVE_FILE },
            { (uintptr_t)&g_Game->_glowingHourglassStates[0]._gameState, GameStateSlot::GLOWING_HOURGLASS_1 },
            { (uintptr_t)&g_Game->_glowingHourglassStates[1]._gameState, GameStateSlot::GLOWING_HOURGLASS_2 }
        };

        uintptr_t slotPtr = reinterpret_cast<uintptr_t>(state);
        auto it = slotMap.find(slotPtr);

        if (it == slotMap.end())
        {
            return GameStateSlot::NULL_SLOT;
        }

        return it->second;
    }
}