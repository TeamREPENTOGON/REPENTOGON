#pragma once

// Returns false if the character is locked behind an XML-defined "achievement". Returns true otherwise.
bool IsCharacterUnlockedRgon(const int playerType);

// Return true if the character is hidden from the menu via rgon's "hideachievement" xml attribute.
// It is possible for a character to be hidden by a different achievement than their "unlock", or the same one.
bool IsCharacterHiddenByAchievementRgon(const int playerType);

// Patch that enables modded character to be "hidden" in the character menu until some achievement is unlocked, via the "hideachievement" attribute.
void PatchModdedCharacterHiddenByAchievementInMenu();
