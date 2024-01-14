/* This is saved as UTF-8 with BOM(code page 65001) */
#ifndef I
#define I(ITEM, TRANSLATE)
#endif

#ifndef V
#define V(TYPE, VAR, VALUE)
#endif

/*
 * note: Remove items inside "en_us.inl" may cause compile error, and you 
 * need also remove all of them in other language files.
 */

//V(float, UNIFONT_DEFAULT_SIZE, 13)

//=========== Main Menu Bar ==========
// I(BAR_TOOLS, ICON_FA_SCREWDRIVER_WRENCH " Tools") // non-utf8 version is no problem here for en_us
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8" Tools")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8" Debug Console")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8" Log Viewer")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8" Game Options")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8" Performance")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8" Style Editor")

I(DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, u8"Dear ImGui Style Editor")

//=========== Console Mega =============

I(CONSOLE_WINDOW_NAME, u8"Console")

I(CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, u8"[ERROR] %s can't be used if not in-game!")
I(CONSOLE_HELP_OUTSIDE_GAME_HINT, u8"(Only commands enabled to show outside of the game will appear right now.)\n")
I(CONSOLE_NO_MACRO_HINT, u8"No macro with that name.\n")

I(CONSOLE_CMD_HINT, u8"Type your command here (\"help\" for help)")

I(CONSOLE_DEBUG_HINT_1, u8"Entity Positions")
I(CONSOLE_DEBUG_HINT_2, u8"Grid")
I(CONSOLE_DEBUG_HINT_3, u8"Infinite HP")
I(CONSOLE_DEBUG_HINT_4, u8"High Damage")
I(CONSOLE_DEBUG_HINT_5, u8"Show Room Info")
I(CONSOLE_DEBUG_HINT_6, u8"Show Hitspheres")
I(CONSOLE_DEBUG_HINT_7, u8"Show Damage Values")
I(CONSOLE_DEBUG_HINT_8, u8"Infinite Item Charges")
I(CONSOLE_DEBUG_HINT_9, u8"High Luck")
I(CONSOLE_DEBUG_HINT_10, u8"Quick Kill")
I(CONSOLE_DEBUG_HINT_11, u8"Grid Info")
I(CONSOLE_DEBUG_HINT_12, u8"Player Item Info")
I(CONSOLE_DEBUG_HINT_13, u8"Show Grid Collision Points")
I(CONSOLE_DEBUG_HINT_14, u8"Show Lua Memory Usage")

I(CONSOLE_COMBO_HINT_0, u8"Treasure")
I(CONSOLE_COMBO_HINT_1, u8"Shop")
I(CONSOLE_COMBO_HINT_2, u8"Boss")
I(CONSOLE_COMBO_HINT_3, u8"Devil")
I(CONSOLE_COMBO_HINT_4, u8"Angel")
I(CONSOLE_COMBO_HINT_5, u8"Secret")
I(CONSOLE_COMBO_HINT_6, u8"Library")
I(CONSOLE_COMBO_HINT_7, u8"Challenge")
I(CONSOLE_COMBO_HINT_8, u8"Golden Chest")
I(CONSOLE_COMBO_HINT_9, u8"Red Chest")
I(CONSOLE_COMBO_HINT_10, u8"Beggar")
I(CONSOLE_COMBO_HINT_11, u8"Demon Beggar")
I(CONSOLE_COMBO_HINT_12, u8"Curse")
I(CONSOLE_COMBO_HINT_13, u8"Key Master")
I(CONSOLE_COMBO_HINT_14, u8"Boss Rush")
I(CONSOLE_COMBO_HINT_15, u8"Dungeon")

//=========== Console Mega Commands ==========
I(CONSOLE_ACHIEVEMENT_DESC, u8"Unlocks achievements")
I(CONSOLE_ACHIEVEMENT_HELP, u8"Unlocks achievements")

I(CONSOLE_ADD_PLAYER_DESC, u8"Spawns a new player")
I(CONSOLE_ADD_PLAYER_HELP, u8"Spawns a new player entity. On default, it spawns Isaac with controller ID 0.\nPlayer ID -1 lets you take control of a random enemy in the room.\nExample:\n(addplayer 7 1) Spawns Azazel and can be controlled with the second input device (controller 1 in most cases)")

I(CONSOLE_CHALLENGE_DESC, u8"Start a challenge run")
I(CONSOLE_CHALLENGE_HELP, u8"Stops the current run and starts a new run on a random seed with the given challenge ID.\nExample:\n(challenge 20) will start a new Purist challenge run.\n")

I(CONSOLE_CLEAR_DESC, u8"Clear the debug console")
I(CONSOLE_CLEAR_HELP, u8"Clears all text currently displayed in the debug console. Only the line \"Repentance Console\" will remain.")

I(CONSOLE_CLEARCACHE_DESC, u8"Clear the sprite cache")
I(CONSOLE_CLEARCACHE_HELP, u8"Clears the game's sprite cache. This can be useful for trying to deal with memory issues.\nThis also has the side effect of reloading modded sprites without needing a full game restart.")

I(CONSOLE_CLEARSEEDS_DESC, u8"Clear easter egg seeds in the current run")
I(CONSOLE_CLEARSEEDS_HELP, u8"Clears any \"special\" seed effects in the current run.\nExample:\nThe seed effect GFVE LLLL is applied in a run. Running clearseeds will remove this effect.")

I(CONSOLE_COMBO_DESC, u8"Give items from a specified pool")
I(CONSOLE_COMBO_HELP, u8"Gives a specified number of items from a specified item pool.\nExample:\n(combo 4.6) will give six random items from the Angel item pool.\nNo, I don't know why they made a bespoke ID system for this one (1) command.")

I(CONSOLE_COPY_DESC, u8"Copy previous commands to clipboard")
I(CONSOLE_COPY_HELP, u8"Copies a specified amount of previous console commands to the system clipboard.\nExample:\n(copy 3) will copy the previous three commands.")

I(CONSOLE_COSTUMETEST_DESC, u8"Give the player random costumes")
I(CONSOLE_COSTUMETEST_HELP, u8"Gives the player a specified amount of random costumes.\nExample:\n(costumetest 34) will give the player 34 random costumes.")

I(CONSOLE_CURSE_DESC, u8"Add curses to the current run")
I(CONSOLE_CURSE_HELP, u8"Permanently (until overridden) adds curses to the run. This command uses a bitmask- the curse with an ID of 1 is 1, 2 is 2, 3 is 4, 4 is 8, and so on. In this manner, desired curse ID's are tallied up and multiple can be enabled simultaneously.\nExample:\n(curse 96) will enable Curse of the Blind and Curse of the Maze simultaneously.")

I(CONSOLE_CUTSCENE_DESC, u8"Play a cutscene")
I(CONSOLE_CUTSCENE_HELP, u8"Immediately plays the specified cutscenne.\nExample:\n(cutscene 1) will immediately play the game's intro.")

I(CONSOLE_DEBUG_DESC, u8"Enable a debug flag")
I(CONSOLE_DEBUG_HELP, u8"Enables the specified debug flag.\nExample:\n(debug 3) will grant the player infinite HP.")

I(CONSOLE_DELIRIOUS_DESC, u8"Force Delirious to be a certain boss")
I(CONSOLE_DELIRIOUS_HELP, u8"Overrides the next boss the Delirious item will become.\nExample:\n(delirious 3) will force Delirious to be a Chub.")

I(CONSOLE_EGGS_DESC, u8"Unlock all easter egg seeds")
I(CONSOLE_EGGS_HELP, u8"PERMANENTLY unlocks all easter eggs in this save file.")

I(CONSOLE_FORCEROOM_DESC, u8"Force a room to be used in level generator")
I(CONSOLE_FORCEROOM_HELP, u8"Allows to set any room as \"forced room\". Said room gets weight of 1000, making it more likely to appear on floor with reseed command.")

I(CONSOLE_FULLRESTART_DESC, u8"Closes and reopens the game")
I(CONSOLE_FULLRESTART_HELP, u8"Closes and reopens the game")

I(CONSOLE_GIVEITEM_DESC, u8"Give the character items, trinkets, cards, and pills")
I(CONSOLE_GIVEITEM_HELP, u8"Gives the main player items, trinkets, cards and pills. These can either be by name or by prefix. Prefixes are (c) for items, (t) for trinkets, (p) for pills, and (k) for cards. Most pocket items count as cards.\nThis command also has shorthand which is just (g).\nExamples:\n(giveitem c1) will give the player The Sad Onion.\n(giveitem t1) will give the player Gulp!\n(giveitem p1) will give the player a Bad Trip pill.\n(giveitem k1) will give the player 0 - The Fool.")

I(CONSOLE_GIVEITEM2_DESC, u8"Give player 2 items, trinkets, cards, and pills")
I(CONSOLE_GIVEITEM2_HELP, u8"Gives the second player items, trinkets, cards and pills. These can either be by name or by prefix. Prefixes are (c) for items, (t) for trinkets, (p) for pills, and (k) for cards. Most pocket items count as cards.\nThis command also has shorthand which is just (g).\nExamples:\n(giveitem2 c1) will give the player The Sad Onion.\n(giveitem2 t1) will give the player Gulp!\n(giveitem2 p1) will give the player a Bad Trip pill.\n(giveitem2 k1) will give the player 0 - The Fool.")

I(CONSOLE_GOTO_DESC, u8"Teleport to a new room")
I(CONSOLE_GOTO_HELP, u8"Teleports the character to a new room. Use (d) for a standard room, (s) for a special room, or three numbers to teleport to an existing room on the floor.\nExample:\n(goto s.boss.1010) will go to a Monstro fight.")

I(CONSOLE_GRIDSPAWN_DESC, u8"Spawn a grid entity")
I(CONSOLE_GRIDSPAWN_HELP, u8"Spawns a new grid entity of the given ID at a random place in the room.")

I(CONSOLE_HELP_DESC, u8"Get info about commands")
I(CONSOLE_HELP_HELP, u8"Retrieve further info about a command and its syntax.")

I(CONSOLE_LISTCOLLECTIBLES_DESC, u8"List current items")
I(CONSOLE_LISTCOLLECTIBLES_HELP, u8"Lists the items the player currently has.")

I(CONSOLE_LOCKACHIEVEMENT_DESC, u8"Locks achievements")
I(CONSOLE_LOCKACHIEVEMENT_HELP, u8"Locks achievements")

I(CONSOLE_LUA_DESC, u8"Run Lua code")
I(CONSOLE_LUA_HELP, u8"Runs the given Lua code immediately. Anything which would work in a standard file will work here.\nThis command also has shorthand which is just (l).")

I(CONSOLE_LUAMEM_DESC, u8"Display lua memory usage")
I(CONSOLE_LUAMEM_HELP, u8"Displays the currently used RAM of LUA.")

I(CONSOLE_LUAMOD_DESC, u8"Reload a Lua mod")
I(CONSOLE_LUAMOD_HELP, u8"Reloads Lua code for the given mod folder.\nExample:\n(luamod testmod) will reload Lua code for the mod in the folder \"testmod\".")

I(CONSOLE_LUARESET_DESC, u8"[EXPERIMENTAL] Reset the Lua context")
I(CONSOLE_LUARESET_HELP, u8"Destroys the current Lua context and recreates it from scratch. This is mostly a backend command meant to help sync up networked play.\nThis has Unforeseen Consequences if done in-game, please only do this on the menu unless you know what you're doing. Please?")

I(CONSOLE_LUARUN_DESC, u8"Run a Lua file")
I(CONSOLE_LUARUN_HELP, u8"Runs a given Lua file immediately.\nExample:\n(luarun mods/test/test.lua) would run \"test.lua\" inside the \"test\" mod folder.")

I(CONSOLE_MACRO_DESC, u8"Trigger a set of commands")
I(CONSOLE_MACRO_HELP, u8"Run a set of commands in a specified order. These are effectively shortcuts. Refer to autocomplete for a list of macro commands.")

I(CONSOLE_METRO_DESC, u8"Force Metronome to be a certain item")
I(CONSOLE_METRO_HELP, u8"Overrides the next item Metronome will become.\nExample:\n(metro c1) will force Metronome to become The Sad Onion.")

I(CONSOLE_NETDELAY_DESC, u8"Change network delay")
I(CONSOLE_NETDELAY_HELP, u8"Changes network delay to a specified value. Can be useful if you see stutters during online gameplay.")

I(CONSOLE_NETSTART_DESC, u8"Initialize online coop")
I(CONSOLE_NETSTART_HELP, u8"Connects player(s) with specified Steam ID to your game (online multiplayer). Allows up to 4 players.\nExample:\nnetstart <steam_user_id1> <steam_user_id2>")

I(CONSOLE_PLAYSFX_DESC, u8"Play a sound effect")
I(CONSOLE_PLAYSFX_HELP, u8"Plays a sound effect immediately.\nExample:\n(playsfx 187) will play an incorrect buzzer.")

I(CONSOLE_PROF_DESC, u8"[BROKEN] Start profiling")
I(CONSOLE_PROF_HELP, u8"Supposed to log information to a CSV. Blame Nicalis!")

I(CONSOLE_PROFSTOP_DESC, u8"[BROKEN] Stop profiling")
I(CONSOLE_PROFSTOP_HELP, u8"Supposed to stop profiling but profiling is broken because we can't have nice things.")

I(CONSOLE_REMOVE_DESC, u8"Remove an item")
I(CONSOLE_REMOVE_HELP, u8"Removes an item from the player immediately. Accepts the same syntax as give, look at that command's help for more info.")

I(CONSOLE_REMOVE2_DESC, u8"Remove an item")
I(CONSOLE_REMOVE2_HELP, u8"Removes an item from the second player immediately. Accepts the same syntax as give, look at that command's help for more info.")

I(CONSOLE_RELOADFX_DESC, u8"Reload floor overlays")
I(CONSOLE_RELOADFX_HELP, u8"Reloads the current floor's effects.")

I(CONSOLE_RELOADSHADERS_DESC, u8"Reload in-game shaders")
I(CONSOLE_RELOADSHADERS_HELP, u8"Reloads any currently loaded shaders.")

I(CONSOLE_RELOADWISPS_DESC, u8"Reload wisps")
I(CONSOLE_RELOADWISPS_HELP, u8"Reloads wisps spawned by Book of Virtues and locusts spawned by Abyss.")

I(CONSOLE_REPEAT_DESC, u8"Repeat prior commands")
I(CONSOLE_REPEAT_HELP, u8"Repeats the previously entered command X amount of times.\nExample:\n(giveitem 1) is used to give the player The Sad Onion. (repeat 5) is then used to give the player The Sad Onion five more times.")

I(CONSOLE_RESEED_DESC, u8"Reseed the current floor")
I(CONSOLE_RESEED_HELP, u8"Reseeds the current floor, generating a brand new layout for it.")

I(CONSOLE_RESTART_DESC, u8"Restart on a new run")
I(CONSOLE_RESTART_HELP, u8"Restarts the game on a new run. Accepts an optional argument which is the character ID.\nExample:\n(restart 3) starts a new run as Judas.")

I(CONSOLE_RESTOCK_DESC, u8"Restocks all shops")
I(CONSOLE_RESTOCK_HELP, u8"Restocks all shops.")

I(CONSOLE_REWIND_DESC, u8"Reset game to last room state")
I(CONSOLE_REWIND_HELP, u8"Makes the game forget about the changes in current room and teleports Isaac back to previous room. Can be used to fix desynchronization issues if you use this command in a room where it happened. (Glowing Hourglass-like effect)")

I(CONSOLE_SEED_DESC, u8"Start a new run with the given seed")
I(CONSOLE_SEED_HELP, u8"Starts a new run with the given seed.\nExample:\n(seed N1CA L1SY) will start a new run with the seed N1CA L1SY.")

I(CONSOLE_SPAWN_DESC, u8"Spawn an entity")
I(CONSOLE_SPAWN_HELP, u8"Spawns a new entity. Syntax is (type).(variant).(subtype).(champion).\nExample:\n(spawn 5.40.1) will spawn a bomb.")

I(CONSOLE_STAGE_DESC, u8"Go to a stage")
I(CONSOLE_STAGE_HELP, u8"Immediately goes to the specified stage. Accepts (a-d) as modifiers, with (a) corresponding to WOTL alts, (b) corresponding to Afterbirth alts, (c) corresponding to Antibirth alts, and (d) corresponding to Repentance alts.\nExample:\n(stage 4d) will take the player to Ashpit II.")

I(CONSOLE_TIME_DESC, u8"Print game time")
I(CONSOLE_TIME_HELP, u8"Prints the total amount of time passed on the run.")

I(CONSOLE_TESTBOSSPOOL_DESC, u8"Print list of bosses for current floor")
I(CONSOLE_TESTBOSSPOOL_HELP, u8"Prints a list of boss names and percentage chance (100%=10000) for current floor.")

I(CONSOLE_STAGE_BLUE_WOMB, u8"??? / Blue Womb")
I(CONSOLE_STAGE_HOME_DAY, u8"Home (day)")
I(CONSOLE_STAGE_HOME_NIGHT, u8"Home (night)")

//=========== Game Options =============

I(OPT_RESET_BTN_HINT, u8"Reset to default value")

I(OPT_OPTIONS, u8"Options")
I(OPT_SFX_VOLUME, u8"SFX Volume")
I(OPT_MUSIC_VOLUME, u8"Music Volume")
I(OPT_MAP_OPACITY, u8"Map Opacity")
I(OPT_FILTER, u8"Filter")
I(OPT_FILTER_MARK, u8"Applies a shader that makes the game look no longer pixelated. Can cause issues with text rendering on screen!")
I(OPT_POP_UPS, u8"Pop-Ups")
I(OPT_ANNOUNCER_VOICE_MODE, u8"Announcer Voice Mode")

I(OPT_HUD_HUDS, u8"HUDs")
I(OPT_HUD_HUD_OFFSET, u8"HUD Offset")
I(OPT_HUD_FOUND_HUD, u8"Found HUD")
I(OPT_HUD_FOUND_HUD_MARKER, u8"Displays the stats of the player on the left side of the screen.")
I(OPT_HUD_CHARGE_BARS, u8"Chargebars")
I(OPT_HUD_BOSS_HEALTH_BAR_ON_BOTTOM, u8"Boss Healthbar on bottom")
I(OPT_HUD_EXTRA_HUD, u8"Extra Hud")
I(OPT_HUD_EXTRA_HUD_MARKER, u8"Displays collected items of the player on the right side of the screen.")

I(OPT_CONSOLE, u8"Console")
I(OPT_CONSOLE_ENABLE_DEBUG_CONSOLE, u8"Enable Debug Console")
I(OPT_CONSOLE_FADED_CONSOLE_DISPLAY,"Faded Console Display")
I(OPT_CONSOLE_SAVE_COMMAND_HISTORY, u8"Save Command History")
I(OPT_CONSOLE_CONSOLE_FONT, u8"Console Font")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT, u8"Enable unicode font")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT_MARK, u8"For improved non-latin language support. Will somewhat increase memory usage.")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE, u8"Unifont Render Mode")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE_MARK, u8"Change font size to make the font display more perfect. Restart needed.")

I(OPT_VIS, u8"Visual Effects")
I(OPT_VIS_GAMMA, u8"Gamma")
I(OPT_VIS_EXPOSURE, u8"Exposure")
I(OPT_VIS_BRIGHTNESS, u8"Brightness")
I(OPT_VIS_CONTRAST, u8"Contrast")
I(OPT_VIS_MAX_SCALE, u8"Max Scale")
I(OPT_VIS_MAX_RENDER_SCALE, u8"Max Render Scale")
I(OPT_VIS_ACTIVE_CAMERA, u8"Active Camera")
I(OPT_VIS_VSYNC, u8"VSync")
I(OPT_VIS_BORDERLESS_FULLSCREEN, u8"Borderless Fullscreen")
I(OPT_VIS_EFFECTS, u8"Effects")
I(OPT_VIS_ANIM_INTERP, u8"Animation Interpolation")
I(OPT_VIS_COLOR_CORRECTION, u8"Color Correction")
I(OPT_VIS_COLOR_MODIFIER, u8"Color Modifier")
I(OPT_VIS_LIGHTING, u8"Lighting")
I(OPT_VIS_SHOCKWAVES, u8"Shockwaves")
I(OPT_VIS_CAUSTICS, u8"Caustics")
I(OPT_VIS_PIXELATION, u8"Pixelation")
I(OPT_VIS_PIXELATION_MARK, u8"e.g. Retro vision pill")
I(OPT_VIS_BLOOM, u8"Bloom")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING, u8"Enable Water Surface rendering")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING_MARK, u8"Needs to be set when changing Water Surface setting")
I(OPT_VIS_WATER_SURFACE, u8"Water Surface")

I(OPT_ACC, u8"Accessibility")
I(OPT_ACC_AIM_LOCK, u8"Aim Lock")
I(OPT_ACC_RUMBLE, u8"Rumble")
I(OPT_ACC_MOUSE_CONTROL, u8"Mouse Control")
I(OPT_ACC_BULLET_VISIBILITY, u8"Bullet Visibility")
I(OPT_ACC_TOUCH_MODE, u8"Touch Mode")

I(OPT_MISC, u8"Misc")
I(OPT_MISC_ENABLE_MODS, u8"Enable Mods")
I(OPT_MISC_ENABLE_CTRL_HOTPLUG,"Enable Control Hotplug")
I(OPT_MISC_USE_STEAM_CLOUD, u8"Use Steam Cloud")
I(OPT_MISC_PAUSE_ON_FOCUS_LOST, u8"Pause on focus lost")

I(OPT_REPENTOGON, u8"REPENTOGON")
I(OPT_REPENTOGON_BETTER_VOID_GEN, u8"Better Void Generation")
I(OPT_REPENTOGON_BETTER_VOID_GEN_MARK, u8"The Void now pulls rooms from all unlocked floors, including alt path.")
I(OPT_REPENTOGON_HUSH_PANIC_FIX, u8"Hush Panic State Fix")
I(OPT_REPENTOGON_HUSH_PANIC_FIX_MARK, u8"Fixes the vanilla bug that causes Hush to no longer have any pause between attacks below 50% health.")
I(OPT_REPENTOGON_KEY_MASTERS_AFFECTDEAL_CHANCES, u8"Key Masters affect deal chances")
I(OPT_REPENTOGON_KEY_MASTERS_AFFECTDEAL_CHANCES_MARK, u8"Killing Key Masters will raise the chance to spawn a deal, now consistent with killing other bums.")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR, u8"Quicker Room Clear")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR_MARK, u8"Rooms will open their doors and spawn rewards quicker. Bosses still require their death animation to play out.")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES, u8"Prevent mod updates")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES_MARK, u8"The game will skip updating your mods folder to match your current workshop folder. Useful for temporarily emptying your mods folder during mod development.")


I(OPT_EXTRA_HUD_MODES_OFF, u8"Off")
I(OPT_EXTRA_HUD_MODES_NORMAL, u8"Normal")
I(OPT_EXTRA_HUD_MODES_MINI, u8"Mini")

I(OPT_ANNOUNCER_MODES_RANDOM, u8"Random")
I(OPT_ANNOUNCER_MODES_OFF, u8"Off")
I(OPT_ANNOUNCER_MODES_ALWAYS_ON, u8"Always on")

I(OPT_CONSOLE_FOND_MODES_DEFAULT, u8"Default")
I(OPT_CONSOLE_FOND_MODES_SMALL, u8"Small")
I(OPT_CONSOLE_FOND_MODES_TINY, u8"Tiny")

I(OPT_OFF_ON_MODES_OFF, u8"Off")
I(OPT_OFF_ON_MODES_ON, u8"On")

I(OPT_UNIFONT_RENDER_MODE_NORMAL, u8"Normal: 13px, only non-latin chars")
I(OPT_UNIFONT_RENDER_MODE_LARGE, u8"LargePerfect: 16px")
I(OPT_UNIFONT_RENDER_MODE_MEDIUM, u8"Medium: 14px")
I(OPT_UNIFONT_RENDER_MODE_TINY, u8"TinyPerfect: 16px and 0.5 scale")
I(OPT_UNIFONT_RENDER_MODE_TINY_LOW, u8"TinyLow: 8px")

//============== Help Menu ===============

// Egg Game
I(HELP_EGG_PRESENT, u8"A present!")
I(HELP_EGG_DELI_COOKIE, u8"A delicious cookie. You take a bite.")
I(HELP_EGG_GREAT_COOKIE, u8"The cookie tastes great!")
I(HELP_EGG_EATEN_COOKIE, u8"You have eaten the cookie.")

I(HELP_EGG_ANOTHER_GIFT, u8"There is another gift...")
I(HELP_EGG_WEIRED, u8"You took another present! It smells weird...")
I(HELP_EGG_PIECE_OF_SHIT, u8"An ordinary piece of shit...\nDoesn't taste very nutricious...")

I(HELP_EGG_TOILET, u8"You flushed the turd down the toilet!")
I(HELP_EGG_PRESENT_BEHIND_TOILET, u8"You look behind the toilet and find a fancy looking present.")
I(HELP_EGG_ANCIENT_EGG, u8"An ancient looking egg. Feels kinda warm...")
I(HELP_EGG_DRAGON_FROM_EGG, u8"A dragon hatched from the egg!")
I(HELP_EGG_HUNGRY_DRAGON, u8"Your dragon looks hungry and neglected.")
I(HELP_EGG_FEED_DRAGON, u8"You try to feed the dragon...")
I(HELP_EGG_LEMON, u8"You gave the dragon a lemon.\nIt looks at you confused...")
I(HELP_EGG_CARROT, u8"You feed the dragon a carrot.\nIt doesn't seem to like that...")
I(HELP_EGG_BACON, u8"You feed the dragon some bacon.\nIt eats it with delight!")
I(HELP_EGG_ATTENTION, u8"Your dragon wants attention!")
I(HELP_EGG_PET_AND_LOVE, u8"You pet the dragon a lot and give him lots of love.")
I(HELP_EGG_DRAGON_HAPPY, u8"Your dragon looks happy!")

I(HELP_EGG_RIDEABLE, u8"You begin to wonder if the dragon is rideable.")
I(HELP_EGG_TAKE_OFF, u8"You and your dragon take off to the sky!")
I(HELP_EGG_INSEPARABLE, u8"You feel as if you and your dragon companion are inseparable.")
I(HELP_EGG_REACH_THE_HEAVEN, u8"You keep ascending!\nYou have reached the heavens.")
I(HELP_EGG_GOD_SMITES_DRAGON, u8"Unfortunately, God cares not for your antics.\nHe smites your dragon on the spot, as you descend into the unknown depths below.")
I(HELP_EGG_YOU_SURVIVE, u8"You survive the fall, but your dragon dies on impact..")

I(HELP_EGG_REVENGE, u8"You will have your revenge.\nBut how?")
I(HELP_EGG_REBUILD, u8"We can rebuild him.")
I(HELP_EGG_STRONGER, u8"We can make him stronger.")
I(HELP_EGG_REBORN_AS_GUN, u8"Your dragon has been reborn as a gun!")

I(HELP_EGG_RIDEABLE_AGAIN, u8"You begin to wonder if your legally distinct gun dragon is rideable.")
I(HELP_EGG_TAKE_OFF_AGAIN, u8"You and your blatant reference take off to the sky!")
I(HELP_EGG_INSEPARABLE_AGAIN, u8"You feel as if you and your reference to the reference game are inseparable.")
I(HELP_EGG_ASCENDING_1, u8"You keep ascending!")
I(HELP_EGG_ASCENDING_2, u8"You keep ascending!")
I(HELP_EGG_ASCENDING_3, u8"You keep ascending!")
I(HELP_EGG_ASCENDING, u8"You keep ascending...")

I(HELP_EGG_SINS_CRAWLING, u8"You feel your sins crawling on your back.")
I(HELP_EGG_NO_HEAVEN, u8"Fool that you are, for there is no heaven.")
I(HELP_EGG_NO_DRAGON, u8"There is no dragon.")
I(HELP_EGG_NOTHING, u8"There is nothing.")

I(HELP_EGG_HOWDY, u8"Howdy! It's me, FLOWEY! FLOWEY the FLOWER!")
I(HELP_EGG_I_OWE_YOU, u8"I owe you a HUGE thanks! You really did a number on that old fool!\nWithout you, I NEVER could have gotten past him!")
I(HELP_EGG_WITH_YOUR_HELP, u8"But now, with YOUR help...")
I(HELP_EGG_HE_DEAD, u8"He's DEAD.")
I(HELP_EGG_GOT_HUMAN_SOUL, u8"And I'VE got the human SOULS!")
I(HELP_EGG_SOUL_INSIDE_ME_AGAIN, u8"Boy! I've been empty for so long...\nIt feels great to have a SOUL inside me again.")
I(HELP_EGG_FELL_WRIGGLING, u8"Mmmm, I can feel them wriggling...")
I(HELP_EGG_FEELING_LEFT_OUT, u8"Awww, you're feeling left out, aren't you?")
I(HELP_EGG_NEED_ONE_MORE_SOUL, u8"Well, that's just perfect. After all, I only have six souls.\nI still need one more...")
I(HELP_EGG_BEFORE_BECOME_GOD, u8"Before I become GOD.")
I(HELP_EGG_POWER, u8"And then, with my newfound powers...")
I(HELP_EGG_MONSTER, u8"Monsters...")
I(HELP_EGG_HUMAN, u8"Humans...")
I(HELP_EGG_EVERYONE, u8"Everyone...")
I(HELP_EGG_SHOW_MEANING, u8"I'll show them all the REAL meaning of this world!")
I(HELP_EGG_STILL_GUN, u8"There may be no dragon, but there still is a gun.\nYou pull out your gun and shoot Flowey, killing him instantly.")
I(HELP_EGG_POWER_OF_REPENTOGON, u8"The day is saved! That's the power of REPENTOGON.")

// Controls

I(HELP_CTRL_PS_DPAD, u8"D-PAD")
I(HELP_CTRL_PS_DPAD_DESC, u8"Move, Tweak values (when input active)")
I(HELP_CTRL_PS_TOUCH, u8"Touch Panel")
I(HELP_CTRL_PS_TOUCH_DESC, u8"Mouse")
I(HELP_CTRL_PS_LSTICK, u8"L Stick")
I(HELP_CTRL_PS_LSTICK_DESC, u8"Scroll")
I(HELP_CTRL_PS_SQUARE, u8"Square")
I(HELP_CTRL_PS_SQUARE_DESC, u8"Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window")
I(HELP_CTRL_PS_TRIANGLE, u8"Triangle")
I(HELP_CTRL_PS_TRIANGLE_DESC, u8"Edit Text / on-screen keyboard")
I(HELP_CTRL_PS_CIRCLE, u8"Circle")
I(HELP_CTRL_PS_CIRCLE_DESC, u8"Cancel / Close / Exit")
I(HELP_CTRL_PS_CROSS, u8"Cross")
I(HELP_CTRL_PS_CROSS_DESC, u8"Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)")


I(HELP_CTRL_XB_DPAD, u8"D-PAD")
I(HELP_CTRL_XB_DPAD_DESC, u8"Move, Tweak values (when input active)")
I(HELP_CTRL_XB_STICK, u8"L Stick")
I(HELP_CTRL_XB_STICK_DESC, u8"Scroll")
I(HELP_CTRL_XB_X, u8"X")
I(HELP_CTRL_XB_X_DESC, u8"Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window")
I(HELP_CTRL_XB_Y, u8"Y")
I(HELP_CTRL_XB_Y_DESC, u8"Edit Text / on-screen keyboard")
I(HELP_CTRL_XB_B, u8"B")
I(HELP_CTRL_XB_B_DESC, u8"Cancel / Close / Exit")
I(HELP_CTRL_XB_A, u8"A")
I(HELP_CTRL_XB_A_DESC, u8"Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)")

I(HELP_CTRL_SWITCH_DPAD, u8"D-PAD")
I(HELP_CTRL_SWITCH_DPAD_DESC, u8"Move, Tweak values (when input active)")
I(HELP_CTRL_SWITCH_LSTICK, u8"L Stick")
I(HELP_CTRL_SWITCH_LSTICK_DESC, u8"Scroll")
I(HELP_CTRL_SWITCH_Y, u8"Y")
I(HELP_CTRL_SWITCH_Y_DESC, u8"Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window")
I(HELP_CTRL_SWITCH_X, u8"X")
I(HELP_CTRL_SWITCH_X_DESC, u8"Edit Text / on-screen keyboard")
I(HELP_CTRL_SWITCH_B, u8"B")
I(HELP_CTRL_SWITCH_B_DESC, u8"Cancel / Close / Exit")
I(HELP_CTRL_SWITCH_A, u8"A")
I(HELP_CTRL_SWITCH_A_DESC, u8"Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)")

// Help Menu
I(HELP_MENU, ICON_U8_FA_CIRCLE_QUESTION u8" Help")
I(HELP_MENU_CTRLS, ICON_U8_FA_GAMEPAD u8" Controls")
I(HELP_MENU_CTRL_MOUSE, u8"Mouse")
I(HELP_MENU_CTRL_MOUSE_HINT, u8"Scale Text: Mousewheel + CTRL and hover over the text/window you want to scale")

I(HELP_MENU_CTRL_KEYBOARD, u8"Keyboard")
I(HELP_MENU_CTRL_CONTROLLER, u8"Controller")

I(HELP_MENU_CTRL_PLAYSTATION, u8"Playstation")
I(HELP_MENU_CTRL_PLAYSTATION_TEXT, u8"Controls for DualShock4 controllers")

I(HELP_MENU_CTRL_XBOX, u8"XBox")
I(HELP_MENU_CTRL_XBOX_TEXT, u8"Controls for XBox controllers")

I(HELP_MENU_CTRL_SWITCH, u8"Switch")
I(HELP_MENU_CTRL_SWITCH_TEXT, u8"Controls for Joy-Con controllers")

I(HELP_MENU_DOC, ICON_U8_FA_BOOK u8" Documentation")
I(HELP_MENU_DOC_TEXT, u8"The documentation can be found here: ")

I(HELP_MENU_TRANSLATE, ICON_U8_FA_GLOBE u8" Translate Information")

// ============ ImGui ===============

I(IMGUI_WIN_CTX_MENU_PIN_WINDOW, u8"Pin Window")
I(IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC, u8"Pinning a window will keep it visible even after closing Dev Tools.")

I(IMGUI_CHANGE_KEY_BTN_NAME, u8"Change")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_CTRL, u8"Press a button on your controller.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_KEYBOARD, u8"Press a key on your keyboard.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_ESC, u8"Press ESC to cancel input")

// ============= Log Viewer ==============

I(LOGV_WIN_NAME, u8"Log Viewer")
I(LOGV_FILTER, u8"Filter")
I(LOGV_PIN_WINDOW, u8"Pin Window")
I(LOGV_PIN_WINDOW_HINT, u8"Pinning a window will keep it visible even after closing Dev Tools.")
I(LOGV_AUTOSCROLL, u8"Autoscroll")
I(LOGV_SHOW_CATEGORIES, u8"Show categories:")

// ============ Performance Window ==========

I(PERF_WIN_NAME, u8"Performance")
I(PERF_TIMEFRAME_NAME, u8"Timeframe")
I(PERF_TIMEFRAME_FORMAT, u8"%.1f Seconds")
I(PERF_LUA_MEM_USE, u8"Lua Mem Usage")

// =========== Translate Report ============

I(TRANS_LANG_JAPANESE, u8"Japanese")
I(TRANS_LANG_KOREAN, u8"Korean")
I(TRANS_LANG_CHINESE_SIMPLE, u8"Chinese(Simple)")
I(TRANS_LANG_RUSSIAN, u8"Russian")
I(TRANS_LANG_GERMAN, u8"German")
I(TRANS_LANG_SPANISH, u8"Spanish")
I(TRANS_LANG_FRENCH, u8"French")

I(TRANS_REPORT_WIN_NAME, u8"Translate Report")
I(TRANS_LANGUAGE_HINT, u8"Translate info for language: %s")
I(TRANS_UNTRANS_ITEMS, u8"Untranslated items:")
I(TRANS_COUNT_HINT, u8"total count: %d translated: %d")
