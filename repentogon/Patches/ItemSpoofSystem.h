#pragma once

namespace ItemSpoofSystem
{
    void StartLuaRequest() noexcept;
    void EndLuaRequest() noexcept;
    void ReworkCollectible(int collectible) noexcept;
    void ReworkBirthright(int playerType) noexcept;
    void ReworkTrinket(int trinket) noexcept;
}