#pragma once

#include <string>

namespace ExtraRenderSteps
{
    namespace detail
    {
        void ApplyPatches();
    }

    namespace ErrorDisplay
    {
        enum Priority
        {
            REPENTOGON_CRITICAL = 10000,
            REPENTOGON_HIGH = 1000,
            REPENTOGON_MEDIUM = 500,
            REPENTOGON_LOW = 100,
            LUA_ERROR = 0,
        };

        void RaiseError(const std::string& message, int priority);
    };
}