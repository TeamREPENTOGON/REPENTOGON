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
        constexpr int REPENTOGON_INTERNAL_PRIORITY = 100;
        constexpr int LUA_ERROR_PRIORITY = 0;

        void RaiseError(const std::string& message, int priority);
    };
}