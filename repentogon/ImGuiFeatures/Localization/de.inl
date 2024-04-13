/* This is saved as UTF-8 with BOM(code page 65001) */
#ifndef I
#define I(ITEM, TRANSLATE)
#endif

#ifndef V
#define V(TYPE, VAR, VALUE)
#endif

// V(float, UNIFONT_DEFAULT_SIZE, 13)

//=========== Main Menu Bar ==========
// I(BAR_TOOLS, ICON_FA_SCREWDRIVER_WRENCH " Tools") // non-utf8 version is no problem here for en_us
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8" Werkzeuge")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8" Entwicklerkonsole")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8" Protokoll")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8" Spiele Optionen")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8" Leistung")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8" Aussehen anpassen")

I(DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, u8"ImGui Aussehen anpassen")

//=========== Console Mega =============

I(CONSOLE_WINDOW_NAME, u8"Entwicklerkonsole")

I(CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, u8"[ERROR] %s kann nicht außerhalb des Spiels benutzt werden!")
I(CONSOLE_HELP_OUTSIDE_GAME_HINT, u8"(Nur Befehle die außerhalb des Spiels benutzt werden können, werden angezeigt.)\n")
I(CONSOLE_NO_MACRO_HINT, u8"Kein Makro mit diesen Namen gefunden.\n")

I(CONSOLE_CMD_HINT, u8"Befehl hier eingeben (\"help\" für Hilfe)")

I(CONSOLE_DEBUG_HINT_1, u8"Entity Positionen")
I(CONSOLE_DEBUG_HINT_2, u8"Raster")
I(CONSOLE_DEBUG_HINT_3, u8"Unendliches Leben")
I(CONSOLE_DEBUG_HINT_4, u8"Hoher Schaden")
I(CONSOLE_DEBUG_HINT_5, u8"Zeige Raum Informationen")
I(CONSOLE_DEBUG_HINT_6, u8"Zeige Treffersphären")
I(CONSOLE_DEBUG_HINT_7, u8"Zeige Schadenswerte")
I(CONSOLE_DEBUG_HINT_8, u8"Unendliche Gegenstandsladung")
I(CONSOLE_DEBUG_HINT_9, u8"Großes Glück")
I(CONSOLE_DEBUG_HINT_10, u8"Schnelles töten")
I(CONSOLE_DEBUG_HINT_11, u8"Raster-Informationen")
I(CONSOLE_DEBUG_HINT_12, u8"Spieler Gegenstands-Informationen")
I(CONSOLE_DEBUG_HINT_13, u8"Zeige Raster Kollisionspunkte")
I(CONSOLE_DEBUG_HINT_14, u8"Zeige Lua Arbeitsspeicher Nutzung")

I(CONSOLE_COMBO_HINT_0, u8"Schatz")
I(CONSOLE_COMBO_HINT_1, u8"Shop")
I(CONSOLE_COMBO_HINT_2, u8"Boss")
I(CONSOLE_COMBO_HINT_3, u8"Teufel")
I(CONSOLE_COMBO_HINT_4, u8"Engel")
I(CONSOLE_COMBO_HINT_5, u8"Geheimnis")
I(CONSOLE_COMBO_HINT_6, u8"Bücherei")
I(CONSOLE_COMBO_HINT_7, u8"Herausvorderung")
I(CONSOLE_COMBO_HINT_8, u8"Goldene Truhe")
I(CONSOLE_COMBO_HINT_9, u8"Rote Truhe")
I(CONSOLE_COMBO_HINT_10, u8"Bettler")
I(CONSOLE_COMBO_HINT_11, u8"Dämonen Bettler")
I(CONSOLE_COMBO_HINT_12, u8"Fluch")
I(CONSOLE_COMBO_HINT_13, u8"Schlüsselmeister")
I(CONSOLE_COMBO_HINT_14, u8"Boss Ansturm")
I(CONSOLE_COMBO_HINT_15, u8"Verlies")

//=========== Console Mega Commands ==========
I(CONSOLE_ACHIEVEMENT_DESC, u8"Schaltet Erfolge frei")
I(CONSOLE_ACHIEVEMENT_HELP, u8"Schaltet Erfolge frei")

I(CONSOLE_ADD_PLAYER_DESC, u8"Erzeugt einen neuen Spieler")
I(CONSOLE_ADD_PLAYER_HELP, u8"Erzeugt einen neuen Spieler. Standardmäßig spawnt Isaac mit der Controller-ID 0.\nMit der Spieler-ID -1 können Sie die Kontrolle über einen zufälligen Feind im Raum übernehmen.\nBeispiel:\n(addplayer 7 1) spawnt Azazel und kann mit dem zweiten Eingabegerät gesteuert werden ( in den meisten Fällen Controller 1)")

I(CONSOLE_CHALLENGE_DESC, u8"Starten Sie einen Herausforderungs-Spiel")
I(CONSOLE_CHALLENGE_HELP, u8"Stoppt das aktuellen Lauf und startet einen Neuen mit einem zufälligen Seed und der angegebenen Herausforderungs-ID.\nBeispiel:\n(challenge 20) startet die Puristen-Herausforderung.\n")

I(CONSOLE_CLEAR_DESC, u8"Leere die Entwicklerkonsole")
I(CONSOLE_CLEAR_HELP, u8"Entfernt alle Texte die aktuell in der Entwicklerkonsole dargestellt werden. Nur die Zeile \"Repentance Console\" bleibt bestehen.")

I(CONSOLE_CLEARCACHE_DESC, u8"Leere den Grafik-Zwischenspeicher")
I(CONSOLE_CLEARCACHE_HELP, u8"Leere den Grafik-Zwischenspeicher des Spiels. Dies kann bei Arbeitsspeicher Problemen helfen.\nDies hat zudem den Effekt das Grafiken von Mods neu geladen werden, ohne das Spiel neu starten zu müssen.")
