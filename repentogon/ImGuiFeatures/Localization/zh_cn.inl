/* This is saved as UTF-8 with BOM(code page 65001) */
#ifndef I
#define I(ITEM, TRANSLATE)
#endif

#ifndef V
#define V(TYPE, VAR, VALUE)
#endif

// V(float, UNIFONT_DEFAULT_SIZE, 12)

//=========== Main Menu Bar ==========
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8"工具")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8"控制台")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8"日志查看器")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8"游戏选项")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8"性能")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8"样式编辑器")

I(DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, u8"ImGui样式编辑器")

//=========== Console Mega =============

I(CONSOLE_WINDOW_NAME, u8"控制台")

I(CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, u8"[错误] %s 仅能在游戏内使用！")
I(CONSOLE_HELP_OUTSIDE_GAME_HINT, u8"（仅有启用了游戏外显示的命令才会在此处显示。）\n")
I(CONSOLE_NO_MACRO_HINT, u8"找不到该名称的宏命令\n")

I(CONSOLE_CMD_HINT, u8"在此处输入您的命令（输入“help”以获取帮助）")

I(CONSOLE_DEBUG_HINT_1, u8"实体位置")
I(CONSOLE_DEBUG_HINT_2, u8"单元格")
I(CONSOLE_DEBUG_HINT_3, u8"无限血量")
I(CONSOLE_DEBUG_HINT_4, u8"高伤害")
I(CONSOLE_DEBUG_HINT_5, u8"显示房间信息")
I(CONSOLE_DEBUG_HINT_6, u8"显示碰撞箱")
I(CONSOLE_DEBUG_HINT_7, u8"显示伤害")
I(CONSOLE_DEBUG_HINT_8, u8"无限充能")
I(CONSOLE_DEBUG_HINT_9, u8"高幸运")
I(CONSOLE_DEBUG_HINT_10, u8"快速消灭")
I(CONSOLE_DEBUG_HINT_11, u8"单元格位置")
I(CONSOLE_DEBUG_HINT_12, u8"角色道具信息")
I(CONSOLE_DEBUG_HINT_13, u8"显示单元格碰撞点")
I(CONSOLE_DEBUG_HINT_14, u8"显示Lua内存使用情况")

I(CONSOLE_COMBO_HINT_0, u8"宝箱房")
I(CONSOLE_COMBO_HINT_1, u8"商店")
I(CONSOLE_COMBO_HINT_2, u8"头目房")
I(CONSOLE_COMBO_HINT_3, u8"恶魔房")
I(CONSOLE_COMBO_HINT_4, u8"天使房")
I(CONSOLE_COMBO_HINT_5, u8"隐藏房")
I(CONSOLE_COMBO_HINT_6, u8"图书馆")
I(CONSOLE_COMBO_HINT_7, u8"挑战房")
I(CONSOLE_COMBO_HINT_8, u8"金箱子")
I(CONSOLE_COMBO_HINT_9, u8"红箱子")
I(CONSOLE_COMBO_HINT_10, u8"乞丐")
I(CONSOLE_COMBO_HINT_11, u8"恶魔乞丐")
I(CONSOLE_COMBO_HINT_12, u8"诅咒房")
I(CONSOLE_COMBO_HINT_13, u8"钥匙大师")
I(CONSOLE_COMBO_HINT_14, u8"头目车轮战")
I(CONSOLE_COMBO_HINT_15, u8"夹层")

//=========== Console Mega Commands ==========
I(CONSOLE_ACHIEVEMENT_DESC, u8"解锁成就")
I(CONSOLE_ACHIEVEMENT_HELP, u8"解锁成就")

I(CONSOLE_ADD_PLAYER_DESC, u8"生成一个玩家实体")
I(CONSOLE_ADD_PLAYER_HELP, u8"生成一个玩家实体。默认参数会生成一个由ID为0的控制器操控的以撒。\ntype设为-1将会操控房间中随机敌人。\n例如：\n（addplayer 7 1）会生成一个阿撒泻勒，其由第二输入设备操控（大多数情况下控制器1即为第二输入设备）")

I(CONSOLE_CHALLENGE_DESC, u8"开始一局新的挑战")
I(CONSOLE_CHALLENGE_HELP, u8"结束当前游戏并以随机种子进行给定ID的挑战。\n例如：\n（challenge 20）\n开始一论新的返璞归真挑战。\n")

I(CONSOLE_CLEAR_DESC, u8"清空控制台")
I(CONSOLE_CLEAR_HELP, u8"清除控制台显示的所有文字。仍会保留“Repentance Console”。")

I(CONSOLE_CLEARCACHE_DESC, u8"清除精灵图缓存")
I(CONSOLE_CLEARCACHE_HELP, u8"清除游戏的精灵图缓存。可以用于尝试解决内存问题。\n副作用为无需完全重启游戏即可重新加载修改后的精灵图。")

I(CONSOLE_CLEARSEEDS_DESC, u8"清除当前游戏的彩蛋种子")
I(CONSOLE_CLEARSEEDS_HELP, u8"清除当前游戏的“特殊”种子效果。\n例如：\n使用种子GFVE LLLL开始一轮游戏。使用命令clearseeds会移除种子效果。")

I(CONSOLE_COMBO_DESC, u8"获得一个道具池中多个随机道具")
I(CONSOLE_COMBO_HELP, u8"获得选定道具池中的选定数量个随机道具。\n例如：\n（combo 4.6）会给予天使房道具池的六个随机道具。\n老天，我不知道他们为什么要为这一（1）个命令制作定制的ID系统。")

I(CONSOLE_COPY_DESC, u8"复制之前输入的指定数量的命令至剪贴板")
I(CONSOLE_COPY_HELP, u8"复制之前输入的指定数量的命令至系统剪贴板。\n例如：\n（copy 3）会复制前三次输入。")

I(CONSOLE_COSTUMETEST_DESC, u8"给予角色随机角色形象")
I(CONSOLE_COSTUMETEST_HELP, u8"角色随机获得选定数量个道具的角色形象。\n例如：\n（costumetest 34）会给与角色34个随机服饰。")

I(CONSOLE_CURSE_DESC, u8"为游戏添加诅咒效果")
I(CONSOLE_CURSE_HELP, u8"为游戏永久添加诅咒（直至被覆写）。此命令使用位掩码——ID 为1的诅咒是1,2是2,3是4,4是8，依此类推。通过这种方式，所需的诅咒 ID 得以计算，并且可以同时启用多个。\n例如：\n（curse 96）会同时启用致盲诅咒和混乱诅咒。")

I(CONSOLE_CUTSCENE_DESC, u8"播放过场动画")
I(CONSOLE_CUTSCENE_HELP, u8"立即播放选定的过场动画。\n例如：\n（cutscene 1）会立刻播放开场动画。")

I(CONSOLE_DEBUG_DESC, u8"启用调试标记")
I(CONSOLE_DEBUG_HELP, u8"启用选定的调试标记。\n例如：\n（debug 3）会给予玩家无限的血量。")

I(CONSOLE_DELIRIOUS_DESC, u8"强制设定精神错乱为某个头目")
I(CONSOLE_DELIRIOUS_HELP, u8"规定精神错乱（主动道具）所生成的下个跟班为所选值。\n例如：\n（delirious 3）会强制将精神错乱变形为胖蛆。")

I(CONSOLE_EGGS_DESC, u8"解锁所有彩蛋种子")
I(CONSOLE_EGGS_HELP, u8" 永久 解锁此存档文件的所有彩蛋种子。")

I(CONSOLE_FORCEROOM_DESC, u8"强制在楼层生成时使用房间布局")
I(CONSOLE_FORCEROOM_HELP, u8"设置参数指定的房间为“forced room”。 即将该房间权重设为1000，使得该房间在reseed命令下的楼层中更容易出现。")

I(CONSOLE_FULLRESTART_DESC, u8"关闭并重新打开游戏")
I(CONSOLE_FULLRESTART_HELP, u8"关闭并重新打开游戏")

I(CONSOLE_GIVEITEM_DESC, u8"给予角色道具、饰品、卡牌或胶囊")
I(CONSOLE_GIVEITEM_HELP, u8"给予主角色道具、饰品、卡牌或胶囊。变量可以输入名称或类型。类型（c）代表道具，（t）代表饰品，（k）代表卡牌，（p）代表胶囊。大多数消耗品均被视为卡牌。\n这个命令的缩写是（g）。\n例如\n（giveitem c1）会给予角色悲伤洋葱。\n（giveitem t1）会给予角色被吞下的硬币。\n（giveitem p1）会给予角色过激幻觉胶囊。\n（giveitem k1）会给予角色0-愚者。")

I(CONSOLE_GIVEITEM2_DESC, u8"给予第二角色道具、饰品、卡牌或胶囊")
I(CONSOLE_GIVEITEM2_HELP, u8"给予第二角色道具、饰品、卡牌或胶囊。类型（c）代表道具，（t）代表饰品，（k）代表卡牌，（p）代表胶囊。大多数消耗品均被视为卡牌。\n这个命令的缩写是（g）。\n例如\n（giveitem2 c1）会给予角色悲伤洋葱。\n（giveitem2 t1）会给予角色被吞下的硬币。\n（giveitem2 p1）会给予角色过激幻觉胶囊。\n（giveitem2 k1）会给予角色0-愚者。")

I(CONSOLE_GOTO_DESC, u8"传送到新房间")
I(CONSOLE_GOTO_HELP, u8"将角色传送到当前层特定地形的房间中。（d）代表普通房间，（s）代表特殊房间，（x）代表特定楼层的某些特殊房间布局\n例如：\n（goto s.boss.1010）将传送至萌死戳头目战房间。")

I(CONSOLE_GRIDSPAWN_DESC, u8"生成障碍物")
I(CONSOLE_GRIDSPAWN_HELP, u8"以选定ID生成障碍物，其会出现在房间内随机位置。")

I(CONSOLE_HELP_DESC, u8"获取命令帮助")
I(CONSOLE_HELP_HELP, u8"获取有关命令及其语法的信息。")

I(CONSOLE_LISTCOLLECTIBLES_DESC, u8"输出已获得的道具")
I(CONSOLE_LISTCOLLECTIBLES_HELP, u8"输出本局角色目前持有的道具。")

I(CONSOLE_LOCKACHIEVEMENT_DESC, u8"锁定成就")
I(CONSOLE_LOCKACHIEVEMENT_HELP, u8"锁定成就")

I(CONSOLE_LUA_DESC, u8"运行Lua代码")
I(CONSOLE_LUA_HELP, u8"立即运行输入的Lua代码。任何可以在Lua文件中运行的代码都可以在此处运行。\n这个命令的缩写是（l）")

I(CONSOLE_LUAMEM_DESC, u8"显示Lua内存用量")
I(CONSOLE_LUAMEM_HELP, u8"显示出当前Lua使用的内存空间用量")

I(CONSOLE_LUAMOD_DESC, u8"重新加载Lua模组")
I(CONSOLE_LUAMOD_HELP, u8"重新加载选定mod文件夹中的Lua代码。\n例如：\n（luamod testmod）会重新加载文件夹\"testmod\"内的Lua代码。")

I(CONSOLE_LUARESET_DESC, u8"[实验性]重置Lua上下文")
I(CONSOLE_LUARESET_HELP, u8"销毁当前的Lua上下文并重新创建。这主要是一个后端命令，以帮助同步在线游戏。\n如果在游戏中这样做，将产生不可预见的后果，请只在主菜单上使用此命令，除非你知道自己在做什么。拜托？")

I(CONSOLE_LUARUN_DESC, u8"运行Lua文件")
I(CONSOLE_LUARUN_HELP, u8"立即运行选定的Lua文件\n例如：\n（luarun mods/test/test.lua）将运行“test”模组文件夹中的“test.lua”。")

I(CONSOLE_MACRO_DESC, u8"运行一组命令")
I(CONSOLE_MACRO_HELP, u8"按指定的顺序执行命令。请查看自动补全获取宏命令表，这些命令可以提高效率。")

I(CONSOLE_METRO_DESC, u8"强制设定节拍器所生成的下个道具")
I(CONSOLE_METRO_HELP, u8"规定节拍器所生成的下个道具。\n例如：\n（metro c1）将强制节拍器给予悲伤洋葱。")

I(CONSOLE_NETDELAY_DESC, u8"更改网络延迟")
I(CONSOLE_NETDELAY_HELP, u8"将网络延迟设置为选定值。如果在联机游戏中遭遇闪回可以使用。")

I(CONSOLE_NETSTART_DESC, u8"初始化联机合作")
I(CONSOLE_NETSTART_HELP, u8"与选定Steam ID的玩家进行链接（在线多人）。最多四名玩家。\n例如：\nnetstart <steam_user_id1> <steam_user_id2>")

I(CONSOLE_PLAYSFX_DESC, u8"播放音效")
I(CONSOLE_PLAYSFX_HELP, u8"立即播放游戏内的音效。\n例如：\n（playsfx 187）会播放错误提示。")

I(CONSOLE_PROF_DESC, u8"[损坏]启动采集")
I(CONSOLE_PROF_HELP, u8"本应收集数据并写入csv文件的，但无法使用。都怪Nicalis！")

I(CONSOLE_PROFSTOP_DESC, u8"[损坏]停止采集")
I(CONSOLE_PROFSTOP_HELP, u8"本应停止收集数据的，但采集功能是坏的，全都是因为Nicalis太粗心了！")

I(CONSOLE_REMOVE_DESC, u8"移除一个物品")
I(CONSOLE_REMOVE_HELP, u8"立即移除角色的物品。输入规则与give相同，查看give命令的帮助以获取更多信息。")

I(CONSOLE_REMOVE2_DESC, u8"移除一个物品")
I(CONSOLE_REMOVE2_HELP, u8"立即移除第二角色的物品。输入规则与give相同，查看give命令的帮助以获取更多信息。")

I(CONSOLE_RELOADFX_DESC, u8"重载楼层的图层")
I(CONSOLE_RELOADFX_HELP, u8"重载当前楼层的特效渲染图层。")

I(CONSOLE_RELOADSHADERS_DESC, u8"重载游戏内光影")
I(CONSOLE_RELOADSHADERS_HELP, u8"重载全部已加载的光影。")

I(CONSOLE_RELOADWISPS_DESC, u8"重载魂火与蝗虫")
I(CONSOLE_RELOADWISPS_HELP, u8"重新加载美德之书生成的魂火跟班和无底坑生成的蝗虫。")

I(CONSOLE_REPEAT_DESC, u8"重复上一条命令")
I(CONSOLE_REPEAT_HELP, u8"将上一条输入的命令重复X次\n例如：\n（giveitem 1）会给予角色悲伤洋葱。（repeat 5）将再重复给予角色五次悲伤洋葱。")

I(CONSOLE_RESEED_DESC, u8"使用随机种子替换当前楼层")
I(CONSOLE_RESEED_HELP, u8"使用随机种子替换当前层的房间分布，会生成新的房间布局。")

I(CONSOLE_RESTART_DESC, u8"重新开始一局新的游戏")
I(CONSOLE_RESTART_HELP, u8"重新开始一局新的游戏。接受角色ID作为可选参数。\n例如：\n（restart 3）会以犹大开始新的一局游戏。")

I(CONSOLE_RESTOCK_DESC, u8"补充所有商店")
I(CONSOLE_RESTOCK_HELP, u8"补充所有商店。")

I(CONSOLE_REWIND_DESC, u8"回溯游戏状态至上一个房间")
I(CONSOLE_REWIND_HELP, u8"取消当前房间中产生的所有变化传送回上一个房间。在出现同步失效的房间使用可修复同步失效。（类似发光沙漏）")

I(CONSOLE_SEED_DESC, u8"使用选定种子重新开始一局新的游戏")
I(CONSOLE_SEED_HELP, u8"使用选定种子重新开始一局新的游戏。\n例如：\n（seed N1CA L1SY）将以种子N1CA L1SY开始一局新的游戏。")

I(CONSOLE_SPAWN_DESC, u8"生成实体")
I(CONSOLE_SPAWN_HELP, u8"生成一个新的实体。参数为(type).(variant).(subtype).(champion)。\n例如：\n（spawn 5.40.1）会生成一个炸弹。")

I(CONSOLE_STAGE_DESC, u8"传送至楼层")
I(CONSOLE_STAGE_HELP, u8"立即传送至特定章节与楼层。使用（a-d）作为修饰符，（a）对应羔羊的愤怒变种，（b）对应胎衣变种，（c）对应抗生变种，（d）对应忏悔变种。\n例如：\n（stage 4d）将角色传送至淹水洞穴II。")

I(CONSOLE_TIME_DESC, u8"输出游戏时间")
I(CONSOLE_TIME_HELP, u8"输出本局游戏经过的时间。")

I(CONSOLE_TESTBOSSPOOL_DESC, u8"输出当前层的头目列表")
I(CONSOLE_TESTBOSSPOOL_HELP, u8"输出当前层头目的名称与概率，概率以百分数形式表示（100%=10000）。")

I(CONSOLE_STAGE_BLUE_WOMB, u8"???/蓝子宫")
I(CONSOLE_STAGE_HOME_DAY, u8"家（白天）")
I(CONSOLE_STAGE_HOME_NIGHT, u8"家（夜晚）")

//=========== Game Options =============

I(OPT_RESET_BTN_HINT, u8"重置为默认值")

I(OPT_OPTIONS, u8"选项")
I(OPT_SFX_VOLUME, u8"音效")
I(OPT_MUSIC_VOLUME, u8"音乐")
I(OPT_MAP_OPACITY, u8"地图可见度")
I(OPT_FILTER, u8"滤镜")
I(OPT_FILTER_MARK, u8"使用着色器，减少游戏像素化程度。可能会导致文本渲染问题!")
I(OPT_POP_UPS, u8"弹窗")
I(OPT_ANNOUNCER_VOICE_MODE, u8"播报员语音模式")

I(OPT_HUD_HUDS, u8"界面")
I(OPT_HUD_HUD_OFFSET, u8"界面位置")
I(OPT_HUD_FOUND_HUD, u8"属性界面")
I(OPT_HUD_FOUND_HUD_MARKER, u8"在屏幕左侧显示角色属性。")
I(OPT_HUD_CHARGE_BARS, u8"蓄力条")
I(OPT_HUD_BOSS_HEALTH_BAR_ON_BOTTOM, u8"在底部显示头目生命值")
I(OPT_HUD_EXTRA_HUD, u8"道具表界面")
I(OPT_HUD_EXTRA_HUD_MARKER, u8"在屏幕右侧显示角色持有的道具。")

I(OPT_CONSOLE, u8"控制台")
I(OPT_CONSOLE_ENABLE_DEBUG_CONSOLE, u8"启用控制台")
I(OPT_CONSOLE_FADED_CONSOLE_DISPLAY, u8"透明控制台")
I(OPT_CONSOLE_SAVE_COMMAND_HISTORY, u8"保存命令历史")
I(OPT_CONSOLE_CONSOLE_FONT, u8"控制台字体")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT, u8"使用unicode字体")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT_MARK, u8"改进非拉丁语言支持。会在一定程度上增加内存使用。")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE, u8"Unifont渲染模式")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE_MARK, u8"更改字体大小以达到最佳效果。需要重新启动。")

I(OPT_VIS, u8"视觉效果")
I(OPT_VIS_GAMMA, u8"伽马")
I(OPT_VIS_EXPOSURE, u8"曝光")
I(OPT_VIS_BRIGHTNESS, u8"亮度")
I(OPT_VIS_CONTRAST, u8"对比度")
I(OPT_VIS_MAX_SCALE, u8"最大缩放")
I(OPT_VIS_MAX_RENDER_SCALE, u8"最大渲染缩放")
I(OPT_VIS_ACTIVE_CAMERA, u8"动态视野")
I(OPT_VIS_VSYNC, u8"垂直同步")
I(OPT_VIS_BORDERLESS_FULLSCREEN, u8"无边框窗口")
I(OPT_VIS_EFFECTS, u8"效果")
I(OPT_VIS_ANIM_INTERP, u8"动画插帧")
I(OPT_VIS_COLOR_CORRECTION, u8"色彩校正")
I(OPT_VIS_COLOR_MODIFIER, u8"色彩调整")
I(OPT_VIS_LIGHTING, u8"照明")
I(OPT_VIS_SHOCKWAVES, u8"冲击波")
I(OPT_VIS_CAUSTICS, u8"焦散曲线")
I(OPT_VIS_PIXELATION, u8"像素化")
I(OPT_VIS_PIXELATION_MARK, u8"例如胶囊复古视野")
I(OPT_VIS_BLOOM, u8"泛光")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING, u8"启用水面渲染")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING_MARK, u8"更改水面设置时需要启用")
I(OPT_VIS_WATER_SURFACE, u8"水面")

I(OPT_ACC, u8"易用性")
I(OPT_ACC_AIM_LOCK, u8"锁定瞄准")
I(OPT_ACC_RUMBLE, u8"震动")
I(OPT_ACC_MOUSE_CONTROL, u8"鼠标控制")
I(OPT_ACC_BULLET_VISIBILITY, u8"子弹可见度")
I(OPT_ACC_TOUCH_MODE, u8"触屏模式")

I(OPT_MISC, u8"杂项")
I(OPT_MISC_ENABLE_MODS, u8"启用模组")
I(OPT_MISC_ENABLE_CTRL_HOTPLUG, u8"启用手柄热插拔")
I(OPT_MISC_USE_STEAM_CLOUD, u8"使用Steam云存档")
I(OPT_MISC_PAUSE_ON_FOCUS_LOST, u8"失去焦点时暂停")

I(OPT_REPENTOGON, u8"REPENTOGON")
I(OPT_REPENTOGON_BETTER_VOID_GEN, u8"更好的虚空生成")
I(OPT_REPENTOGON_BETTER_VOID_GEN_MARK, u8"虚空章节将从包括变种章节在内的所有解锁的章节中获取房间。")
I(OPT_REPENTOGON_HUSH_PANIC_FIX, u8"死寂漏洞修复")
I(OPT_REPENTOGON_HUSH_PANIC_FIX_MARK, u8"修复原版死寂生命值低于50%后没有攻击间隔的bug。")

I(OPT_REPENTOGON_KEY_MASTERS_AFFECTDEAL_CHANCES, u8"钥匙大师影响恶魔房/天使房概率")
I(OPT_REPENTOGON_KEY_MASTERS_AFFECTDEAL_CHANCES_MARK, u8"炸毁钥匙大师将提高开启恶魔房/天使房概率，与炸毁其他乞丐一致。")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR, u8"快速房间清理")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR_MARK, u8"房间开门和生成掉落物的速度更快。头目房仍需等待头目死亡动画播放完毕。")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES, u8"阻止模组更新")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES_MARK, u8"游戏不再保持模组文件夹与创意工坊文件夹同步。用于在模组开发期间暂时清空mods文件夹。")

I(OPT_EXTRA_HUD_MODES_OFF, u8"关")
I(OPT_EXTRA_HUD_MODES_NORMAL, u8"普通")
I(OPT_EXTRA_HUD_MODES_MINI, u8"小号")

I(OPT_ANNOUNCER_MODES_RANDOM, u8"随机")
I(OPT_ANNOUNCER_MODES_OFF, u8"关")
I(OPT_ANNOUNCER_MODES_ALWAYS_ON, u8"总是")

I(OPT_CONSOLE_FOND_MODES_DEFAULT, u8"默认")
I(OPT_CONSOLE_FOND_MODES_SMALL, u8"小")
I(OPT_CONSOLE_FOND_MODES_TINY, u8"非常小")

I(OPT_OFF_ON_MODES_OFF, u8"关闭")
I(OPT_OFF_ON_MODES_ON, u8"打开")

I(OPT_UNIFONT_RENDER_MODE_NORMAL, u8"正常：13px，仅非拉丁字符")
I(OPT_UNIFONT_RENDER_MODE_LARGE, u8"大（像素完美）: 16px")
I(OPT_UNIFONT_RENDER_MODE_MEDIUM, u8"中: 14px")
I(OPT_UNIFONT_RENDER_MODE_TINY, u8"小（像素完美）: 16px，0.5倍缩放")
I(OPT_UNIFONT_RENDER_MODE_TINY_LOW, u8"小（低画质）: 8px")

//============== Help Menu ===============

// Egg Game
I(HELP_EGG_PRESENT, u8"礼物！")
I(HELP_EGG_DELI_COOKIE, u8"美味的饼干。你尝了一点。")
I(HELP_EGG_GREAT_COOKIE, u8"饼干尝起来很棒！")
I(HELP_EGG_EATEN_COOKIE, u8"你把饼干吃完了。")

I(HELP_EGG_ANOTHER_GIFT, u8"还有一个礼物……")
I(HELP_EGG_WEIRED, u8"你又拿了一件礼物!闻起来怪怪的……")
I(HELP_EGG_PIECE_OF_SHIT, u8"一坨普通的屎……\n尝起来不太有营养……")

I(HELP_EGG_TOILET, u8"你把屎从马桶冲走！")
I(HELP_EGG_PRESENT_BEHIND_TOILET, u8"你看了看厕所后面，发现了一件漂亮的礼物。")
I(HELP_EGG_ANCIENT_EGG, u8"一枚看起来很古老的蛋。感觉暖暖的……")
I(HELP_EGG_DRAGON_FROM_EGG, u8"蛋里孵出来一只龙!")
I(HELP_EGG_HUNGRY_DRAGON, u8"你的龙饥饿、寂寞。")
I(HELP_EGG_FEED_DRAGON, u8"你试着喂这条龙……")
I(HELP_EGG_LEMON, u8"你喂给这条龙一个柠檬。\n它疑惑地看着你……")
I(HELP_EGG_CARROT, u8"你喂给这条龙一根胡萝卜。\n它似乎不喜欢这个……")
I(HELP_EGG_BACON, u8"你喂给这条龙一些培根。\n它吃得很开心！")
I(HELP_EGG_ATTENTION, u8"你的龙希望得到关注！")
I(HELP_EGG_PET_AND_LOVE, u8"你经常抚摸这只龙，并把你的爱给他。")
I(HELP_EGG_DRAGON_HAPPY, u8"你的龙看起来很快乐！")

I(HELP_EGG_RIDEABLE, u8"你开始思考龙能不能骑。")
I(HELP_EGG_TAKE_OFF, u8"你和你的龙飞上了天空！")
I(HELP_EGG_INSEPARABLE, u8"你感觉好像你和你的龙不可分离。")
I(HELP_EGG_REACH_THE_HEAVEN, u8"你继续上升!\n你已经到达了天堂。")
I(HELP_EGG_GOD_SMITES_DRAGON, u8"不幸的是，上帝并不在乎你滑稽的行为。\n他当场打了你的龙，而你也坠入未知的深渊。")
I(HELP_EGG_YOU_SURVIVE, u8"你幸存下来，但你的龙死于撞击……")

I(HELP_EGG_REVENGE, u8"你会报仇的。\n但是如何报仇？")
I(HELP_EGG_REBUILD, u8"我们可以复活他。")
I(HELP_EGG_STRONGER, u8"我们可以让他更加强大。")
I(HELP_EGG_REBORN_AS_GUN, u8"你的龙重生为一把枪！")

I(HELP_EGG_RIDEABLE_AGAIN, u8"你开始思考你合法的、独一无二的枪龙能不能骑。")
I(HELP_EGG_TAKE_OFF_AGAIN, u8"你和你显眼的龙飞上了天空。")
I(HELP_EGG_INSEPARABLE_AGAIN, u8"你感觉好像你和你显眼的龙与这个游戏密不可分。")
I(HELP_EGG_ASCENDING_1, u8"你继续上升！")
I(HELP_EGG_ASCENDING_2, u8"你持续上升！")
I(HELP_EGG_ASCENDING_3, u8"你仍在上升！")
I(HELP_EGG_ASCENDING, u8"你还在上升……")

I(HELP_EGG_SINS_CRAWLING, u8"你感到罪恶爬上了你的脊背。")
I(HELP_EGG_NO_HEAVEN, u8"愚蠢的你，因为天堂不存在。")
I(HELP_EGG_NO_DRAGON, u8"这里没有龙。")
I(HELP_EGG_NOTHING, u8"这里什么都没有。")

I(HELP_EGG_HOWDY, u8"你好！我是花朵！花朵·花朵！")
I(HELP_EGG_I_OWE_YOU, u8"我非常感谢你！你真的把那个老傻瓜整得很惨！\n没有你，我永远也过不了他那一关！")
I(HELP_EGG_WITH_YOUR_HELP, u8"但是现在，有了 你的 帮助……")
I(HELP_EGG_HE_DEAD, u8"他死了。")
I(HELP_EGG_GOT_HUMAN_SOUL, u8"我已经得到了人类的灵魂！")
I(HELP_EGG_SOUL_INSIDE_ME_AGAIN, u8"我已经空虚了太久… 再次拥有灵魂在体内的感觉太棒了。")
I(HELP_EGG_FELL_WRIGGLING, u8"嗯，我可以感受到他们正在扭动……")
I(HELP_EGG_FEELING_LEFT_OUT, u8"啊，你是不是觉得被冷落了，是吗？")
I(HELP_EGG_NEED_ONE_MORE_SOUL, u8"这真是太完美了。毕竟，我只有六个灵魂。 我还需要一个……")
I(HELP_EGG_BEFORE_BECOME_GOD, u8"在我成为神之前……")
I(HELP_EGG_POWER, u8"然后，用我新获的力量……")
I(HELP_EGG_MONSTER, u8"怪物……")
I(HELP_EGG_HUMAN, u8"人类……")
I(HELP_EGG_EVERYONE, u8"每个人……")
I(HELP_EGG_SHOW_MEANING, u8"我会让他们都知道这个世界的真正含义！")
I(HELP_EGG_STILL_GUN, u8"世界上可能没有龙，但是还有枪。\n你拔出你的枪，一枪打死了Flowey。")
I(HELP_EGG_POWER_OF_REPENTOGON, u8"你拯救了今天！这就是REPENTOGON的力量！")

// Controls

I(HELP_CTRL_PS_DPAD, u8"十字方向键")
I(HELP_CTRL_PS_DPAD_DESC, u8"移动、调整速度（输入时激活）")
I(HELP_CTRL_PS_TOUCH, u8"触控板")
I(HELP_CTRL_PS_TOUCH_DESC, u8"鼠标")
I(HELP_CTRL_PS_LSTICK, u8"左摇杆")
I(HELP_CTRL_PS_LSTICK_DESC, u8"滚屏")
I(HELP_CTRL_PS_SQUARE, u8"方形")
I(HELP_CTRL_PS_SQUARE_DESC, u8"点按：切换菜单\n按住+L1/R1：更改窗口焦点\n按住+十字键：调整窗口大小\n按住+L-Stick：移动窗口")
I(HELP_CTRL_PS_TRIANGLE, u8"三角")
I(HELP_CTRL_PS_TRIANGLE_DESC, u8"编辑文字、屏幕键盘")
I(HELP_CTRL_PS_CIRCLE, u8"圆形")
I(HELP_CTRL_PS_CIRCLE_DESC, u8"取消、关闭、退出")
I(HELP_CTRL_PS_CROSS, u8"叉键")
I(HELP_CTRL_PS_CROSS_DESC, u8"激活、打开、切换\n同时使用十字键增强输入\n（+L1/R1以调快/慢）")

I(HELP_CTRL_XB_DPAD, u8"十字方向键")
I(HELP_CTRL_XB_DPAD_DESC, u8"移动、调整速度（输入时激活）")
I(HELP_CTRL_XB_STICK, u8"左摇杆")
I(HELP_CTRL_XB_STICK_DESC, u8"滚屏")
I(HELP_CTRL_XB_X, u8"X")
I(HELP_CTRL_XB_X_DESC, u8"点按：切换菜单\n按住+L1/R1：更改窗口焦点\n按住+十字键：调整窗口大小\n按住+L-Stick：移动窗口")
I(HELP_CTRL_XB_Y, u8"Y")
I(HELP_CTRL_XB_Y_DESC, u8"编辑文字、屏幕键盘")
I(HELP_CTRL_XB_B, u8"B")
I(HELP_CTRL_XB_B_DESC, u8"取消、关闭、退出")
I(HELP_CTRL_XB_A, u8"A")
I(HELP_CTRL_XB_A_DESC, u8"激活、打开、切换\n同时使用十字键增强输入\n（+L1/R1以调快/慢）")

I(HELP_CTRL_SWITCH_DPAD, u8"十字方向键")
I(HELP_CTRL_SWITCH_DPAD_DESC, u8"移动、调整速度（输入时激活）")
I(HELP_CTRL_SWITCH_LSTICK, u8"左摇杆")
I(HELP_CTRL_SWITCH_LSTICK_DESC, u8"滚屏")
I(HELP_CTRL_SWITCH_Y, u8"Y")
I(HELP_CTRL_SWITCH_Y_DESC, u8"点按：切换菜单\n按住+L1/R1：更改窗口焦点\n按住+十字键：调整窗口大小\n按住+L-Stick：移动窗口")
I(HELP_CTRL_SWITCH_X, u8"X")
I(HELP_CTRL_SWITCH_X_DESC, u8"编辑文字、屏幕键盘")
I(HELP_CTRL_SWITCH_B, u8"B")
I(HELP_CTRL_SWITCH_B_DESC, u8"取消、关闭、退出")
I(HELP_CTRL_SWITCH_A, u8"A")
I(HELP_CTRL_SWITCH_A_DESC, u8"激活、打开、切换\n同时使用十字键增强输入\n（+L1/R1以调快/慢）")

// Help Menu

I(HELP_MENU, ICON_U8_FA_CIRCLE_QUESTION u8" 帮助")
I(HELP_MENU_CTRLS, ICON_U8_FA_GAMEPAD u8" 控制")
I(HELP_MENU_CTRL_MOUSE, u8"鼠标")
I(HELP_MENU_CTRL_MOUSE_HINT, u8"缩放文本: 将鼠标悬停在文本/窗口上，使用 CTRL+滚轮 进行缩放")

I(HELP_MENU_CTRL_KEYBOARD, u8"键盘")
I(HELP_MENU_CTRL_CONTROLLER, u8"手柄")

I(HELP_MENU_CTRL_PLAYSTATION, u8"Playstation")
I(HELP_MENU_CTRL_PLAYSTATION_TEXT, u8"DualShock4手柄说明")

I(HELP_MENU_CTRL_XBOX, u8"XBox")
I(HELP_MENU_CTRL_XBOX_TEXT, u8"XBox手柄说明")

I(HELP_MENU_CTRL_SWITCH, u8"Switch")
I(HELP_MENU_CTRL_SWITCH_TEXT, u8"Joy-Con手柄说明")

I(HELP_MENU_DOC, ICON_U8_FA_BOOK u8" 文档")
I(HELP_MENU_DOC_TEXT, u8"在此查阅文档：")

I(HELP_MENU_TRANSLATE, ICON_U8_FA_GLOBE u8" 翻译信息")

// ============ ImGui ===============

I(IMGUI_WIN_CTX_MENU_PIN_WINDOW, u8"固定窗口")
I(IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC, u8"被固定的窗口在关闭开发工具后依旧可见。")

I(IMGUI_CHANGE_KEY_BTN_NAME, u8"更改")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_CTRL, u8"按下手柄上的按键。")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_KEYBOARD, u8"按下键盘上的按键。")
I(IMGUI_CHANGE_KEY_BTN_PRESS_ESC, u8"按下ESC取消输入")

// ============= Log Viewer ==============

I(LOGV_WIN_NAME, u8"日志查看器")
I(LOGV_FILTER, u8"过滤器")
I(LOGV_PIN_WINDOW, u8"固定窗口")
I(LOGV_PIN_WINDOW_HINT, u8"被固定的窗口在关闭开发工具后依旧可见。")
I(LOGV_AUTOSCROLL, u8"自动滚动")
I(LOGV_SHOW_CATEGORIES, u8"显示类别：")

// ============ Performance Window ==========

I(PERF_WIN_NAME, u8"性能")
I(PERF_TIMEFRAME_NAME, u8"时间范围")
I(PERF_TIMEFRAME_FORMAT, u8"%.1f秒")
I(PERF_LUA_MEM_USE, u8"Lua内存占用")

// =========== Translate Report ============

I(TRANS_LANG_JAPANESE, u8"日语")
I(TRANS_LANG_KOREAN, u8"韩语")
I(TRANS_LANG_CHINESE_SIMPLE, u8"中文（简体）")
I(TRANS_LANG_RUSSIAN, u8"俄语")
I(TRANS_LANG_GERMAN, u8"德语")
I(TRANS_LANG_SPANISH, u8"西班牙语")
I(TRANS_LANG_FRENCH, u8"法语")

I(TRANS_REPORT_WIN_NAME, u8"翻译报告")
I(TRANS_LANGUAGE_HINT, u8"%s语言的翻译报告")
I(TRANS_UNTRANS_ITEMS, u8"未翻译的词条：")
I(TRANS_COUNT_HINT, u8"总计：%d 已翻译：%d")
