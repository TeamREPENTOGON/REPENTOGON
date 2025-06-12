#pragma once

/* This file must be kept synced with the equivalent one on the launcher's
 * side. Together they define the shared interface both programs use to
 * communicate.
 */
namespace LauncherInterface {
    /* Launcher should prompt the user for immediate restart of the game, due
     * to mods having been (un)loaded / (un)installed.
     */
    static constexpr const int LAUNCHER_EXIT_MODS_CHANGED = 1 << 8;
}