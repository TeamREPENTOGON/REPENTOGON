/* This is saved as UTF-8 with BOM(code page 65001) */
#ifndef I
#define I(ITEM, TRANSLATE)
#endif

//=========== Main Menu Bar ==========
// I(BAR_TOOLS, ICON_FA_SCREWDRIVER_WRENCH " Tools") // non-utf8 version is no problem here for en_us
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8" Tools")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8" Debug Console")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8" Log Viewer")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8" Game Options")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8" Performance")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8" Style Editor")

//=========== Game Options =============

I(OPTIONS, "Options")
I(SFX_VOLUME, "SFX Volume")
I(MUSIC_VOLUME, "Music Volume")
I(MAP_OPACITY, "Map Opacity")