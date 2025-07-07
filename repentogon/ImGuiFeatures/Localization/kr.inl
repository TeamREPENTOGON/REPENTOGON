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

// V(float, UNIFONT_DEFAULT_SIZE, 13)

//=========== Main Menu Bar ==========
// I(BAR_TOOLS, ICON_FA_SCREWDRIVER_WRENCH " Tools") // non-utf8 version is no problem here for en_us
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8" 도구")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8" 디버그 콘솔")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8" 로그 뷰어")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8" 게임 설정")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8" 성능")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8" 스타일 편집기")

I(DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, u8"ImGui 스타일 편집기")

//=========== Console Mega =============

I(CONSOLE_WINDOW_NAME, u8"콘솔")

I(CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, u8"[ERROR] %s 인게임이 아니면 사용할 수 없습니다!")
I(CONSOLE_HELP_OUTSIDE_GAME_HINT, u8"(지금은 게임 외부에서 사용 가능한 명령어만 표시됩니다.)\n")
I(CONSOLE_NO_MACRO_HINT, u8"해당 이름의 매크로가 없습니다.\n")

I(CONSOLE_CMD_HINT, u8"여기에 명령어를 입력하세요 (\"help\" 입력 시 도움말 표시)")

I(CONSOLE_DEBUG_HINT_1, u8"엔티티 위치 표시")
I(CONSOLE_DEBUG_HINT_2, u8"격자 표시")
I(CONSOLE_DEBUG_HINT_3, u8"무한 체력")
I(CONSOLE_DEBUG_HINT_4, u8"공격력 상승")
I(CONSOLE_DEBUG_HINT_5, u8"방 정보 표시")
I(CONSOLE_DEBUG_HINT_6, u8"피격 범위 표시")
I(CONSOLE_DEBUG_HINT_7, u8"피해량 표시")
I(CONSOLE_DEBUG_HINT_8, u8"아이템 상시 충전")
I(CONSOLE_DEBUG_HINT_9, u8"행운 상승")
I(CONSOLE_DEBUG_HINT_10, u8"빠른 처치")
I(CONSOLE_DEBUG_HINT_11, u8"격자 정보")
I(CONSOLE_DEBUG_HINT_12, u8"플레이어 아이템 정보")
I(CONSOLE_DEBUG_HINT_13, u8"Show Grid Collision Points")
I(CONSOLE_DEBUG_HINT_14, u8"Lua 메모리 사용량 표시")

I(CONSOLE_COMBO_HINT_0, u8"보물 방")
I(CONSOLE_COMBO_HINT_1, u8"상점")
I(CONSOLE_COMBO_HINT_2, u8"보스 방")
I(CONSOLE_COMBO_HINT_3, u8"악마 방")
I(CONSOLE_COMBO_HINT_4, u8"천사 방")
I(CONSOLE_COMBO_HINT_5, u8"비밀 방")
I(CONSOLE_COMBO_HINT_6, u8"책방")
I(CONSOLE_COMBO_HINT_7, u8"도전 방")
I(CONSOLE_COMBO_HINT_8, u8"황금 상자")
I(CONSOLE_COMBO_HINT_9, u8"빨간 상자")
I(CONSOLE_COMBO_HINT_10, u8"거지")
I(CONSOLE_COMBO_HINT_11, u8"익마 거지")
I(CONSOLE_COMBO_HINT_12, u8"저주 방")
I(CONSOLE_COMBO_HINT_13, u8"열쇠 거지")
I(CONSOLE_COMBO_HINT_14, u8"보스 러시")
I(CONSOLE_COMBO_HINT_15, u8"던전")

//=========== Console Mega Commands ==========
I(CONSOLE_ACHIEVEMENT_DESC, u8"도전 과제 해금")
I(CONSOLE_ACHIEVEMENT_HELP, u8"도전 과제 해금")

I(CONSOLE_ADD_PLAYER_DESC, u8"새 플레이어 생성")
I(CONSOLE_ADD_PLAYER_HELP, u8"새 플레이어 엔티티를 생성합니다. 기본적으로 컨트롤러 ID가 0인 아이작을 생성합니다.\n플레이어 ID를 -1로 입력하면 방 안에 있는 무작위 적을 제어할 수 있습니다.\n예시:\n(addplayer 7 1)을 입력하면 아자젤을 생성하며, 2번째 입력 장치(대부분의 경우 컨트롤러 1)로 제어할 수 있습니다.")

I(CONSOLE_CHALLENGE_DESC, u8"챌린지 실행")
I(CONSOLE_CHALLENGE_HELP, u8"현재 게임을 중단하고 주어진 챌린지 ID로 무작위 시드에서 새 게임을 시작합니다.\n예시:\n(challenge 20)을 입력하면 '순수주의자' 챌린지를 실행합니다.\n")

I(CONSOLE_CLEAR_DESC, u8"디버그 콘솔 청소")
I(CONSOLE_CLEAR_HELP, u8"디버그 콘솔에 표시된 모든 문자를 제거합니다. \"Repentance Console\"이라는 줄만 남습니다.")

I(CONSOLE_CLEARCACHE_DESC, u8"스프라이트 캐시 삭제")
I(CONSOLE_CLEARCACHE_HELP, u8"게임의 스프라이트 캐시를 제거합니다. 이 기능은 메모리 문제를 해결하는 데 유용할 수 있습니다.\n추가로, 게임을 재시작하지 않고 수정된 스프라이트를 다시 불러올 수 있습니다.")

I(CONSOLE_CLEARSEEDS_DESC, u8"현재 게임의 이스터 에그 시드 제거")
I(CONSOLE_CLEARSEEDS_HELP, u8"현재 게임에서 \"특별\" 시드의 효과를 제거합니다.\n예시:\nGFVE LLLL 시드의 효과가 적용된 게임에서 clearseeds를 입력하면 그 효과를 제거합니다.")

I(CONSOLE_COMBO_DESC, u8"지정한 배열의 아이템 지급")
I(CONSOLE_COMBO_HELP, u8"지정한 아이템 배열에서 정해진 개수만큼 아이템을 무작위로 지급합니다.\n예시:\n(combo 4.6)을 입력하면 천사 방 배열에서 무작위로 6개의 아이템을 지급합니다.\n아, 왜 이 명령어 하나 때문에 따로 ID 체계를 만들었는지 모르겠습니다.")

I(CONSOLE_COPY_DESC, u8"이전 명령어를 클립보드에 복사")
I(CONSOLE_COPY_HELP, u8"지정한 개수만큼 이전에 입력한 콘솔 명령어를 클립보드에 복사합니다.\n예시:\n(copy 3)을 입력하면 최근 3개의 명령어가 복사됩니다.")

I(CONSOLE_COSTUMETEST_DESC, u8"캐릭터에게 무작위 코스튬을 지급")
I(CONSOLE_COSTUMETEST_HELP, u8"지정한 개수만큼 캐릭터에게 무작위 코스튬을 지급합니다.\n예시:\n(costumetest 34)를 입력하면 캐릭터에게 34개의 무작위 코스튬을 지급합니다.")

I(CONSOLE_CURSE_DESC, u8"현재 게임에 저주 추가")
I(CONSOLE_CURSE_HELP, u8"게임에 영구적으로(덮어쓰기 전까지) 저주를 추가합니다. 이 명령어는 비트마스크를 사용하며, ID가 1인 저주는 1, 2는 2, 3은 4, 4는 8...과 같은 방식입니다. 이 방식으로 원하는 저주 ID를 계산하여 여러 저주를 동시에 활성화 할 수 있습니다.\n예시:\n(curse 96)을 입력하면 눈먼 자의 저주와 미로의 저주를 동시에 활성화합니다.")

I(CONSOLE_CUTSCENE_DESC, u8"컷신 재생")
I(CONSOLE_CUTSCENE_HELP, u8"지정한 컷신을 즉시 재생합니다.\n예시:\n(cutscene 1)을 입력하면 게임의 인트로를 즉시 재생합니다.")

I(CONSOLE_DEBUG_DESC, u8"디버그 플래그 활성화")
I(CONSOLE_DEBUG_HELP, u8"지정한 디버그 플래그를 활성화합니다.\n예시:\n(debug 3)을 입력하면 플레이어에게 무한 체력을 부여합니다.")

I(CONSOLE_DELIRIOUS_DESC, u8"정신착란이 생성하는 보스를 강제로 설정")
I(CONSOLE_DELIRIOUS_HELP, u8"정신착란(액티브 아이템)이 생성하는 다음 보스를 설정합니다.\n예시:\n(delirious 3)을 입력하면 정신착란이 처브를 생성합니다.")

I(CONSOLE_EGGS_DESC, u8"모든 이스터 에그 시드 해금")
I(CONSOLE_EGGS_HELP, u8"해당 저장파일에서 영구적으로 모든 이스터 에그가 해금됩니다.")

I(CONSOLE_FORCEROOM_DESC, u8"지정한 방을 레벨 생성기가 강제로 사용하도록 설정")
I(CONSOLE_FORCEROOM_HELP, u8"어떤 방이든 \"forced room\"으로 설정합니다. 해당 방의 가중치를 1000으로 설정하여, reseed 명령어를 사용할 때 해당 방이 층에 등장할 가능성이 매우 높아집니다.")

I(CONSOLE_FULLRESTART_DESC, u8"게임을 완전히 재시작")
I(CONSOLE_FULLRESTART_HELP, u8"게임을 완전히 재시작")

I(CONSOLE_GIVEITEM_DESC, u8"캐릭터에게 아이템, 장신구, 카드 또는 알약을 지급")
I(CONSOLE_GIVEITEM_HELP, u8"메인 캐릭터에게 아이템, 장신구, 카드 또는 알약을 지급합니다. 매개변수에는 이름이나 유형을 입력할 수 있습니다. (c)는 아이템을, (t)는 장신구를, (T)는 황금 장신구를, (p)는 알약을, (P)는 말약을, (k)는 카드를 의미합니다. 대부분의 포켓 아이템은 카드로 간주됩니다.\n이 명령어의 약자는 (g)입니다.\n예시:\n(giveitem c1)을 입력하면 캐릭터에게 눈물나는 양파를 지급합니다.\n(giveitem t1)은 캐릭터에게 삼킨 동전을 지급합니다.\n(giveitem p1)은 캐릭터에게 마약 환각 알약을 지급합니다.\n(giveitem k1)을 입력하면 0 - 바보를 지급합니다.")

I(CONSOLE_GIVEITEM2_DESC, u8"2번째 캐릭터에게 아이템, 장신구, 카드 또는 알약을 지급")
I(CONSOLE_GIVEITEM2_HELP, u8"2번째 캐릭터에게 아이템, 장신구, 카드 또는 알약을 지급합니다. 매개변수에는 이름이나 유형을 입력할 수 있습니다. (c)는 아이템을, (t)는 장신구를, (T)는 황금 장신구를, (p)는 알약을, (P)는 말약을, (k)는 카드를 의미합니다. 대부분의 포켓 아이템은 카드로 간주됩니다.\n이 명령어의 약자는 (g)입니다.\n예시:\n(giveitem c1)을 입력하면 캐릭터에게 눈물나는 양파를 지급합니다.\n(giveitem t1)은 캐릭터에게 삼킨 동전을 지급합니다.\n(giveitem p1)은 캐릭터에게 마약 환각 알약을 지급합니다.\n(giveitem k1)을 입력하면 0 - 바보를 지급합니다.")

I(CONSOLE_GOTO_DESC, u8"새 방으로 순간이동")
I(CONSOLE_GOTO_HELP, u8"캐릭터를 새 방으로 순간이동시킵니다. (d)는 일반 방을, (s)는 특별 방을, 숫자 세 개를 사용하면 현재 층에 존재하는 방으로 이동할 수 있습니다.\n예시:\n(goto s.boss.1010)을 입력하면 몬스트로 보스 방으로 이동합니다.")

I(CONSOLE_GRIDSPAWN_DESC, u8"구조물 생성")
I(CONSOLE_GRIDSPAWN_HELP, u8"지정한 ID의 구조물을 임의의 위치에 생성합니다.")

I(CONSOLE_HELP_DESC, u8"명령어 정보")
I(CONSOLE_HELP_HELP, u8"명령어의 상세 정보와 사용법을 확인합니다.")

I(CONSOLE_LISTCOLLECTIBLES_DESC, u8"현재 아이템 목록")
I(CONSOLE_LISTCOLLECTIBLES_HELP, u8"현재 캐릭터가 가진 아이템 목록을 출력합니다.")

I(CONSOLE_LOCKACHIEVEMENT_DESC, u8"도전 과제 잠금")
I(CONSOLE_LOCKACHIEVEMENT_HELP, u8"도전 과제 잠금")

I(CONSOLE_LUA_DESC, u8"Lua 코드 실행")
I(CONSOLE_LUA_HELP, u8"작성한 Lua 코드를 즉시 실행합니다. 일반 Lua 파일에서 작동하는 코드는 모두 여기에서도 사용할 수 있습니다.\n이 명령어의 약자는 (l)입니다.")

I(CONSOLE_LUAMEM_DESC, u8"Lua 메모리 사용량 표시")
I(CONSOLE_LUAMEM_HELP, u8"Lua에서 현재 사용 중인 RAM을 표시합니다.")

I(CONSOLE_LUAMOD_DESC, u8"Lua 모드 새로고침")
I(CONSOLE_LUAMOD_HELP, u8"지정한 모드 폴더의 Lua 코드를 다시 불러옵니다.\n예시:\n(luamod testmod)를 입력하면 \"testmod\" 폴더에 있는 Lua 코드를 다시 불러옵니다.")

I(CONSOLE_LUARESET_DESC, u8"[실험적] Lua 컨텍스트 초기화")
I(CONSOLE_LUARESET_HELP, u8"현재 Lua 컨텍스트를 제거하고 새로 생성합니다. 이 명령어는 주로 네트워크 플레이 동기화를 위한 백엔드용 명령어입니다.\n인게임에서 실행 시 예기치 못한 문제가 발생할 수 있으니, 메뉴 화면에서만 사용하세요. 특히 이 명령어의 영향을 완전히 이해하지 못했다면 더욱이요. 부탁드립니다.")

I(CONSOLE_LUARUN_DESC, u8"Lua 파일 실행")
I(CONSOLE_LUARUN_HELP, u8"지정한 Lua 파일을 즉시 실행합니다.\n예시:\n(luarun mods/test/test.lua)을 입력하면 \"test\" 모드 폴더의 \"test.lua\"를 실행합니다.")

I(CONSOLE_MACRO_DESC, u8"명령어 집합 실행")
I(CONSOLE_MACRO_HELP, u8"여러 명령어를 순서대로 실행합니다. 일종의 단축키입니다. 매크로는 자동 완성 기능에서 확인할 수 있습니다.")

I(CONSOLE_METRO_DESC, u8"메트로놈이 발동할 아이템을 설정")
I(CONSOLE_METRO_HELP, u8"메트로놈이 다음에 발동할 아이템을 설정합니다.\n예시:\n(metro c1)을 입력하면 메트로놈을 사용했을 때 눈물나는 양파의 효과가 발동됩니다.")

I(CONSOLE_NETDELAY_DESC, u8"네트워크 지연 조정")
I(CONSOLE_NETDELAY_HELP, u8"지정한 값으로 네트워크 지연 시간을 변경합니다. 온라인 플레이 중 끊김 현상에 도움이 될 수 있습니다.")

I(CONSOLE_NETSTART_DESC, u8"온라인 Co-op 시작")
I(CONSOLE_NETSTART_HELP, u8"지정한 Steam ID와 연결합니다. (온라인 멀티플레이) 최대 4인까지 가능합니다.\n예시:\nnetstart <steam_user_id1> <steam_user_id2>")

I(CONSOLE_PLAYSFX_DESC, u8"효과음 재생")
I(CONSOLE_PLAYSFX_HELP, u8"지정한 효과음을 즉시 재생합니다.\n예시:\n(playsfx 187)을 입력하면 incorrect 버저음을 재생합니다.")

I(CONSOLE_PROF_DESC, u8"[손상] 데이터 추적 시작")
I(CONSOLE_PROF_HELP, u8"원래라면 CSV로 정보 로그를 남겨야 합니다. Nicalis 때문이에요!")

I(CONSOLE_PROFSTOP_DESC, u8"[손상] 데이터 추적 중지")
I(CONSOLE_PROFSTOP_HELP, u8"데이터 추적을 중단해야 하는데, 추적 기능 자체가 고장 났습니다. 좋은 걸 가질 수 없는 운명이죠.")

I(CONSOLE_REMOVE_DESC, u8"아이템 제거")
I(CONSOLE_REMOVE_HELP, u8"캐릭터에게서 아이템을 즉시 제거합니다. 사용법은 give 명령어와 동일하니, 자세한 내용은 해당 도움말을 참조하세요.")

I(CONSOLE_REMOVE2_DESC, u8"아이템 제거")
I(CONSOLE_REMOVE2_HELP, u8"2번째 캐릭터에게서 아이템을 즉시 제거합니다. 사용법은 give 명령어와 동일하니, 자세한 내용은 해당 도움말을 참조하세요.")

I(CONSOLE_RELOADFX_DESC, u8"층 오버레이 새로고침")
I(CONSOLE_RELOADFX_HELP, u8"현재 층의 시각 효과 다시 불러옵니다.")

I(CONSOLE_RELOADSHADERS_DESC, u8"셰이더 새로고침")
I(CONSOLE_RELOADSHADERS_HELP, u8"현재 불러온 모든 셰이더를 다시 불러옵니다.")

I(CONSOLE_RELOADWISPS_DESC, u8"위습 새로고침")
I(CONSOLE_RELOADWISPS_HELP, u8"미덕의 서의 위습과 무저갱의 메뚜기를 다시 불러옵니다.")

I(CONSOLE_REPEAT_DESC, u8"명령 반복 실행")
I(CONSOLE_REPEAT_HELP, u8"직전에 입력한 명령어를 지정한 횟수만큼 반복합니다.\n예시:\n(giveitem 1)을 입력하면 캐릭터에게 눈물나는 양파를 지급하고, 그 후로 (repeat 5)을 입력하면 추가로 5개 더 지급합니다.")

I(CONSOLE_RESEED_DESC, u8"현재 층을 무작위 시드로 교체")
I(CONSOLE_RESEED_HELP, u8"현재 층을 무작위 시드로 교체하여 새로운 방 배치를 생성합니다.")

I(CONSOLE_RESTART_DESC, u8"새 게임 시작")
I(CONSOLE_RESTART_HELP, u8"새 게임을 시작합니다. (선택)매개변수로 새로 시작할 캐릭터 ID를 지정할 수 있습니다.\n예시:\n(restart 3)을 입력하면 유다로 새 게임을 시작합니다.")

I(CONSOLE_RESTOCK_DESC, u8"모든 상점 재입고")
I(CONSOLE_RESTOCK_HELP, u8"모든 상점을 재입고합니다.")

I(CONSOLE_REWIND_DESC, u8"게임 상태를 이전 방의 상태로 되돌리기")
I(CONSOLE_REWIND_HELP, u8"현재 방에서의 모든 변화를 무시하고 게임을 이전 방으로 되돌립니다. 동기화 오류가 발생한 방에서 오류를 해결할 수 있습니다. (빛나는 모래시계 효과와 유사)")

I(CONSOLE_SEED_DESC, u8"지정한 시드로 새 게임 시작")
I(CONSOLE_SEED_HELP, u8"지정한 시르도 새 게임을 시작합니다.\n예시:\n(seed N1CA L1SY)을 입력하면 N1CA L1SY 시드로 새 게임을 시작합니다.")

I(CONSOLE_SPAWN_DESC, u8"엔티티 생성")
I(CONSOLE_SPAWN_HELP, u8"새 엔티티를 생성합니다. 문법은 (type).(variant).(subtype).(champion)입니다.\n예시:\n(spawn 5.40.1)을 입력하면 폭탄을 생성합니다.")

I(CONSOLE_STAGE_DESC, u8"스테이지 이동")
I(CONSOLE_STAGE_HELP, u8"지정한 스테이지로 즉시 이동합니다. (a~d)를 수정자로 사용하며, (a)는 어린 양의 분노, (b)는 애프터버스, (c)는 안티버스, (d)는 리펜턴스에서의 변형 스테이지를 의미합니다..\n예시:\n(stage 4d)을 입력하면 잿구덩이 II로 이동합니다.")

I(CONSOLE_TIME_DESC, u8"게임 시간 출력")
I(CONSOLE_TIME_HELP, u8"현재 게임에서 경과한 총 시간을 출력합니다.")

I(CONSOLE_TESTBOSSPOOL_DESC, u8"보스 목록 출력")
I(CONSOLE_TESTBOSSPOOL_HELP, u8"현재 층의 보스 이름과 등장 확률(100%=10000)을 출력합니다.")

I(CONSOLE_FORCEIMPORT_DESC, u8"[실험적] 이전 DLC 저장파일 가져오기.")
I(CONSOLE_FORCEIMPORT_HELP, u8"저장파일 슬롯 ID를 인수로 받습니다.")

I(CONSOLE_STAGE_BLUE_WOMB, u8"??? / 푸른 자궁")
I(CONSOLE_STAGE_HOME_DAY, u8"집 (낮)")
I(CONSOLE_STAGE_HOME_NIGHT, u8"집 (밤)")

I(CONSOLE_GRID_DECORATION_A, u8"Decoration A"                                       u8" 치장성 타일 A")
I(CONSOLE_GRID_DECORATION_B, u8"Decoration B"                                       u8" 치장성 타일 B")
I(CONSOLE_GRID_DECORATION_C, u8"Decoration C"                                       u8" 치장성 타일 C")
I(CONSOLE_GRID_ROCK, u8"Rock"                                                       u8" 돌")
I(CONSOLE_GRID_ROCK_CONNECTING, u8"Rock (connecting)"                               u8" 돌 (연결)")
I(CONSOLE_GRID_BOMB_ROCK, u8"Bomb Rock"                                             u8" 폭탄 돌")
I(CONSOLE_GRID_ALT_ROCK, u8"Alt Rock"                                               u8" 특수 돌")
I(CONSOLE_GRID_TINTED_ROCK, u8"Tinted Rock"                                         u8" 색돌")
I(CONSOLE_GRID_MARKED_SKULL, u8"Marked Skull"                                       u8" 표식 해골")
I(CONSOLE_GRID_EVENT_ROCK, u8"Event Rock"                                           u8" 이벤트 돌")
I(CONSOLE_GRID_SPIKED_ROCK, u8"Spiked Rock"                                         u8" 색돌")
I(CONSOLE_GRID_FOOLS_GOLD_ROCK, u8"Fool's Gold Rock"                                u8" 바보의 황금 돌")
I(CONSOLE_GRID_TNT, u8"TNT"                                                         u8" TNT")
I(CONSOLE_GRID_FIRE_PLACE, u8"Fire Place"                                           u8" 모닥불")
I(CONSOLE_GRID_RED_FIRE_PLACE, u8"Red Fire Place"                                   u8" 빨간 모닥불")
I(CONSOLE_GRID_RED_POOP, u8"Red Poop"                                               u8" 빨간 응아")
I(CONSOLE_GRID_RAINBOW_POOP, u8"Rainbow Poop"                                       u8" 무지개 응아")
I(CONSOLE_GRID_CORNY_POOP, u8"Corny Poop"                                           u8" 옥수수 응아")
I(CONSOLE_GRID_GOLDEN_POOP, u8"Golden Poop"                                         u8" 황금 응아")
I(CONSOLE_GRID_BLACK_POOP, u8"Black Poop"                                           u8" 검은 응아")
I(CONSOLE_GRID_WHITE_POOP, u8"White Poop"                                           u8" 하얀 응아")
I(CONSOLE_GRID_GIANT_POOP, u8"Giant Poop"                                           u8" 거대 응아")
I(CONSOLE_GRID_POOP, u8"Poop"                                                       u8" 응아")
I(CONSOLE_GRID_CHARMING_POOP, u8"Charming Poop"                                     u8" 예쁜 응아")
I(CONSOLE_GRID_BLOCK, u8"Block"                                                     u8" 블록")
I(CONSOLE_GRID_PILLAR, u8"Pillar"                                                   u8" 기둥")
I(CONSOLE_GRID_SPIKES, u8"Spikes"                                                   u8" 가시")
I(CONSOLE_GRID_SANGUINE_BOND_SPIKES, u8"Sanguine Bond Spikes"                       u8" 피의 결속 가시")
I(CONSOLE_GRID_RETRACTABLE_SPIKES, u8"Retractable Spikes"                           u8" 개폐식 가시")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_1_5, u8"Retractable Spikes (down 1/5)"       u8" 개폐식 가시 (1/5 하강)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_2_5, u8"Retractable Spikes (down 2/5)"       u8" 개폐식 가시 (2/5 하강)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_3_5, u8"Retractable Spikes (down 3/5)"       u8" 개폐식 가시 (3/5 하강)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_4_5, u8"Retractable Spikes (down 4/5)"       u8" 개폐식 가시 (4/5 하강)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_5_5, u8"Retractable Spikes (down 5/5)"       u8" 개폐식 가시 (5/5 하강)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_1_5, u8"Retractable Spikes (up 1/5)"           u8" 개폐식 가시 (1/5 상승)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_2_5, u8"Retractable Spikes (up 2/5)"           u8" 개폐식 가시 (2/5 상승)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_3_5, u8"Retractable Spikes (up 3/5)"           u8" 개폐식 가시 (3/5 상승)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_4_5, u8"Retractable Spikes (up 4/5)"           u8" 개폐식 가시 (4/5 상승)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_5_5, u8"Retractable Spikes (up 5/5)"           u8" 개폐식 가시 (5/5 상승)")
I(CONSOLE_GRID_COBWEB, u8"Cobweb"                                                   u8" 거미줄")
I(CONSOLE_GRID_INVISIBLE_BLOCK, u8"Invisible Block"                                 u8" 투명 블록")
I(CONSOLE_GRID_PIT, u8"Pit"                                                         u8" 구멍")
I(CONSOLE_GRID_FISSURE_SPAWNER, u8"Fissure Spawner"                                 u8" 피셔 생성기")
I(CONSOLE_GRID_EVENT_RAIL, u8"Event Rail"                                           u8" 이벤트 레일")
I(CONSOLE_GRID_EVENT_PIT, u8"Event Pit"                                             u8" 이벤트 구멍")
I(CONSOLE_GRID_KEY_BLOCK, u8"Key Block"                                             u8" 열쇠 블록")
I(CONSOLE_GRID_PRESSURE_PLATE, u8"Pressure Plate"                                   u8" 압력판")
I(CONSOLE_GRID_REWARD_PLATE, u8"Reward Plate"                                       u8" 보상 압력판")
I(CONSOLE_GRID_GREED_PLATE, u8"Greed Plate"                                         u8" 탐욕 압력판")
I(CONSOLE_GRID_RAIL_PLATE, u8"Rail Plate"                                           u8" 레일 압력판")
I(CONSOLE_GRID_KILL_PLATE, u8"Kill Plate"                                           u8" 즉사 압력판")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_0, u8"Event Plate (group 0)"                       u8" 이벤트 압력판 (그룹 0)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_1, u8"Event Plate (group 1)"                       u8" 이벤트 압력판 (그룹 1)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_2, u8"Event Plate (group 2)"                       u8" 이벤트 압력판 (그룹 2)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_3, u8"Event Plate (group 3)"                       u8" 이벤트 압력판 (그룹 3)")
I(CONSOLE_GRID_DEVIL_STATUE, u8"Devil Statue"                                       u8" 악마 석상")
I(CONSOLE_GRID_ANGEL_STATUE, u8"Angel Statue"                                       u8" 천사 석상")
I(CONSOLE_GRID_RAIL_HORIZONTAL, u8"Rail (horizontal)"                               u8" 레일 (가로)")
I(CONSOLE_GRID_RAIL_VERTICAL, u8"Rail (vertical)"                                   u8" 레일 (세로)")
I(CONSOLE_GRID_RAIL_DOWN_TO_RIGHT, u8"Rail (down-to-right)"                         u8" 레일 (우-하)")
I(CONSOLE_GRID_RAIL_DOWN_TO_LEFT, u8"Rail (down-to-left)"                           u8" 레일 (좌-하)")
I(CONSOLE_GRID_RAIL_UP_TO_RIGHT, u8"Rail (up-to-right)"                             u8" 레일 (우-상)")
I(CONSOLE_GRID_RAIL_UP_TO_LEFT, u8"Rail (up-to-left)"                               u8" 레일 (좌-상)")
I(CONSOLE_GRID_RAIL_CROSSROAD, u8"Rail (crossroad)"                                 u8" 레일 (교차)")
I(CONSOLE_GRID_RAIL_END_LEFT, u8"Rail (end-left)"                                   u8" 레일 (좌측 끝)")
I(CONSOLE_GRID_RAIL_END_RIGHT, u8"Rail (end-right)"                                 u8" 레일 (우측 끝)")
I(CONSOLE_GRID_RAIL_END_UP, u8"Rail (end-up)"                                       u8" 레일 (상단 끝)")
I(CONSOLE_GRID_RAIL_END_DOWN, u8"Rail (end-down)"                                   u8" 레일 (하단 끝)")
I(CONSOLE_GRID_RAIL_CART_LEFT, u8"Rail (cart-left)"                                 u8" 레일 (좌-카트)")
I(CONSOLE_GRID_RAIL_CART_UP, u8"Rail (cart-up)"                                     u8" 레일 (상-카트)")
I(CONSOLE_GRID_RAIL_CART_RIGHT, u8"Rail (cart-right)"                               u8" 레일 (우-카트)")
I(CONSOLE_GRID_RAIL_CART_DOWN, u8"Rail (cart-down)"                                 u8" 레일 (하-카트)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_1, u8"Mineshaft Rail (horizontal 1)"       u8" 폐광 레일 (가로 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_1, u8"Mineshaft Rail (vertical 1)"           u8" 폐광 레일 (세로 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_RIGHT_1, u8"Mineshaft Rail (down-to-right 1)" u8" 폐광 레일 (우-하 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_LEFT_1, u8"Mineshaft Rail (down-to-left 1)"   u8" 폐광 레일 (좌-하 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_RIGHT_1, u8"Mineshaft Rail (up-to-right 1)"     u8" 폐광 레일 (우-상 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_LEFT_1, u8"Mineshaft Rail (up-to-left 1)"       u8" 폐광 레일 (좌-상 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_2, u8"Mineshaft Rail (horizontal 2)"       u8" 폐광 레일 (가로 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_2, u8"Mineshaft Rail (vertical 2)"           u8" 폐광 레일 (세로 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_RIGHT_2, u8"Mineshaft Rail (down-to-right 2)" u8" 폐광 레일 (우-하 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_LEFT_2, u8"Mineshaft Rail (down-to-left 2)"   u8" 폐광 레일 (좌-하 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_RIGHT_2, u8"Mineshaft Rail (up-to-right 2)"     u8" 폐광 레일 (우-상 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_LEFT_2, u8"Mineshaft Rail (up-to-left 2)"       u8" 폐광 레일 (좌-상 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_3, u8"Mineshaft Rail (horizontal 3)"       u8" 폐광 레일 (가로 3)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_3, u8"Mineshaft Rail (vertical 3)"           u8" 폐광 레일 (세로 3)")
I(CONSOLE_GRID_RAIL_PIT_HORIZONTAL, u8"Rail Pit (horizontal)"                       u8" 레일 구멍 (가로)")
I(CONSOLE_GRID_RAIL_PIT_VERTICAL, u8"Rail Pit (vertical)"                           u8" 레일 구멍 (세로)")
I(CONSOLE_GRID_RAIL_PIT_DOWN_TO_RIGHT, u8"Rail Pit (down-to-right)"                 u8" 레일 구멍 (우-하)")
I(CONSOLE_GRID_RAIL_PIT_DOWN_TO_LEFT, u8"Rail Pit (down-to-left)"                   u8" 레일 구멍 (좌-하)")
I(CONSOLE_GRID_RAIL_PIT_UP_TO_RIGHT, u8"Rail Pit (up-to-right)"                     u8" 레일 구멍 (우-상)")
I(CONSOLE_GRID_RAIL_PIT_UP_TO_LEFT, u8"Rail Pit (up-to-left)"                       u8" 레일 구멍 (좌-상)")
I(CONSOLE_GRID_RAIL_PIT_CROSSROAD, u8"Rail Pit (crossroad)"                         u8" 레일 구멍 (교차)")
I(CONSOLE_GRID_RAIL_PIT_CART_LEFT, u8"Rail Pit (cart-left)"                         u8" 레일 구멍 (좌-카트)")
I(CONSOLE_GRID_RAIL_PIT_CART_UP, u8"Rail Pit (cart-up)"                             u8" 레일 구멍 (상-카트)")
I(CONSOLE_GRID_RAIL_PIT_CART_RIGHT, u8"Rail Pit (cart-right)"                       u8" 레일 구멍 (우-카트)")
I(CONSOLE_GRID_RAIL_PIT_CART_DOWN, u8"Rail Pit (cart-down)"                         u8" 레일 구멍 (하-카트)")
I(CONSOLE_GRID_TELEPORTER_SQUARE, u8"Teleporter (square)"                           u8" 순간이동기 (네모)")
I(CONSOLE_GRID_TELEPORTER_MOON, u8"Teleporter (moon)"                               u8" 순간이동기 (달)")
I(CONSOLE_GRID_TELEPORTER_RHOMBUS, u8"Teleporter (rhombus)"                         u8" 순간이동기 (마름모)")
I(CONSOLE_GRID_TELEPORTER_M, u8"Teleporter (M)"                                     u8" 순간이동기 (M)")
I(CONSOLE_GRID_TELEPORTER_PENTAGRAM, u8"Teleporter (pentagram)"                     u8" 순간이동기 (오망성)")
I(CONSOLE_GRID_TELEPORTER_CROSS, u8"Teleporter (cross)"                             u8" 순간이동기 (십자가)")
I(CONSOLE_GRID_TELEPORTER_TRIANGLE, u8"Teleporter (triangle)"                       u8" 순간이동기 (삼각형)")
I(CONSOLE_GRID_TRAP_DOOR, u8"Trap Door"                                             u8" 순간이동기 (다락문)")
I(CONSOLE_GRID_VOID_PORTAL, u8"Void Portal"                                         u8" 순간이동기 (공허 포탈)")
I(CONSOLE_GRID_CRAWLSPACE, u8"Crawlspace"                                           u8" 사다리 방")
I(CONSOLE_GRID_CRAWLSPACE_GIDEON_DUNGEON, u8"Crawlspace (Gideon dungeon)"           u8" 사다리 방 (기드온 던전)")
I(CONSOLE_GRID_CRAWLSPACE_SECRET_SHOP, u8"Crawlspace (secret shop)"                 u8" 사다리 방 (암시장)")
I(CONSOLE_GRID_CRAWLSPACE_STARTING_ROOM, u8"Crawlspace (starting room)"             u8" 사다리 방 (시작 방)")
I(CONSOLE_GRID_GRAVITY, u8"Gravity"                                                 u8" 중력")

//=========== Game Options =============

I(OPT_RESET_BTN_HINT, u8"기본값으로 되돌리기")

I(OPT_OPTIONS, u8"설정")
I(OPT_SFX_VOLUME, u8"SFX")
I(OPT_MUSIC_VOLUME, u8"음악")
I(OPT_MAP_OPACITY, u8"맵 투명도")
I(OPT_FILTER, u8"필터")
I(OPT_FILTER_MARK, u8"게임 화면을 비픽셀 스타일로 스타일로 보이게 하는 셰이더를 적용합니다. 일부 텍스트 렌더링 문제를 일으킬 수 있습니다!")
I(OPT_POP_UPS, u8"팝업")
I(OPT_ANNOUNCER_VOICE_MODE, u8"아나운서 음성 모드")

I(OPT_HUD_HUDS, u8"HUD")
I(OPT_HUD_HUD_OFFSET, u8"HUD 오프셋")
I(OPT_HUD_FOUND_HUD, u8"HUD 보강")
I(OPT_HUD_FOUND_HUD_MARKER, u8"화면 왼쪽에 플레이어의 능력치를 표시합니다.")
I(OPT_HUD_CHARGE_BARS, u8"충전 막대")
I(OPT_HUD_BOSS_HEALTH_BAR_ON_BOTTOM, u8"보스 체력 막대를 하단에 표시")
I(OPT_HUD_EXTRA_HUD, u8"추가 HUD")
I(OPT_HUD_EXTRA_HUD_MARKER, u8"화면 오른쪽에 플레이어가 획득한 아이템을 표시합니다.")

I(OPT_CONSOLE, u8"콘솔")
I(OPT_CONSOLE_ENABLE_DEBUG_CONSOLE, u8"디버그 콘솔 활성화")
I(OPT_CONSOLE_FADED_CONSOLE_DISPLAY, "투명 콘솔")
I(OPT_CONSOLE_SAVE_COMMAND_HISTORY, u8"명령어 기록 저장")
I(OPT_CONSOLE_CONSOLE_FONT, u8"콘솔 글꼴")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT, u8"유니코드 글꼴 사용")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT_MARK, u8"비라틴 문자를 위한 설정입니다. 메모리 사용량이 약간 증가합니다.")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE, u8"유니폰트 렌더링 모드")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE_MARK, u8"최적의 효과를 위해 글꼴 크기를 조정합니다. 재시작이 필요합니다.")

I(OPT_VIS, u8"그래픽")
I(OPT_VIS_GAMMA, u8"감마")
I(OPT_VIS_EXPOSURE, u8"노출")
I(OPT_VIS_BRIGHTNESS, u8"밝기")
I(OPT_VIS_CONTRAST, u8"명암")
I(OPT_VIS_MAX_SCALE, u8"최대 화면 비율")
I(OPT_VIS_MAX_RENDER_SCALE, u8"최대 렌더 비율")
I(OPT_VIS_ACTIVE_CAMERA, u8"액티브 카메라")
I(OPT_VIS_VSYNC, u8"수직 동기화")
I(OPT_VIS_BORDERLESS_FULLSCREEN, u8"테두리 없는 전체화면")
I(OPT_VIS_EFFECTS, u8"효과")
I(OPT_VIS_EFFECTS_MARK, u8"이 항목은 config.ini 파일의 값에 따라 작동합니다. 저장되지 않으며, 사용자가 변경하는 용도가 아닙니다!")
I(OPT_VIS_ANIM_INTERP, u8"프레임 보간")
I(OPT_VIS_ANIM_INTERP_MARK, u8"이 옵션은 \"intermediate\" 프레임이 활성화되는지 결정합니다. (게임은 30FPS의 논리 프레임, 60FPS의 애니메이션 프레임으로 실행됩니다. intermediate 프레임은 이를 더 \"부드럽게\" 만듭니다.)")
I(OPT_VIS_COLOR_CORRECTION, u8"색상 보정")
I(OPT_VIS_COLOR_MODIFIER, u8"색상 조정")
I(OPT_VIS_LIGHTING, u8"조명")
I(OPT_VIS_SHOCKWAVES, u8"충격파")
I(OPT_VIS_CAUSTICS, u8"굴절")
I(OPT_VIS_PIXELATION, u8"픽셀화")
I(OPT_VIS_PIXELATION_MARK, u8"예: 전자오락 시야경 알약")
I(OPT_VIS_BLOOM, u8"블룸")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING, u8"수면 렌더링 활성화")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING_MARK, u8"수면 설정 변경 시 필요합니다.")
I(OPT_VIS_WATER_SURFACE, u8"수면")

I(OPT_ACC, u8"접근성")
I(OPT_ACC_AIM_LOCK, u8"에임 고정")
I(OPT_ACC_RUMBLE, u8"진동")
I(OPT_ACC_MOUSE_CONTROL, u8"마우스 조작")
I(OPT_ACC_BULLET_VISIBILITY, u8"탄 표시")
I(OPT_ACC_TOUCH_MODE, u8"터치 모드")

I(OPT_MISC, u8"기타")
I(OPT_MISC_ENABLE_MODS, u8"모드 활성화")
I(OPT_MISC_ENABLE_CTRL_HOTPLUG, "컨트롤러 핫플러그 활성화")
I(OPT_MISC_USE_STEAM_CLOUD, u8"Steam 클라우드 사용")
I(OPT_MISC_PAUSE_ON_FOCUS_LOST, u8"포커스를 잃을 때 일시정지")

I(OPT_REPENTOGON, u8"REPENTOGON")
I(OPT_REPENTOGON_BETTER_VOID_GEN, u8"더 나은 공허 생성")
I(OPT_REPENTOGON_BETTER_VOID_GEN_MARK, u8"공허 층이 변종 층을 포함해 해금된 모든 층에서 방을 가져옵니다.")
I(OPT_REPENTOGON_HUSH_LASER_FIX, u8"침묵의 레이저 속도 수정")
I(OPT_REPENTOGON_HUSH_LASER_FIX_MARK, u8"침묵의 레이저가 의도보다 빠르게 움직이던 버그를 수정합니다.")
I(OPT_REPENTOGON_STAT_HUD_PLANETARIUM, u8"천체관 확률 표시")
I(OPT_REPENTOGON_STAT_HUD_PLANETARIUM_MARK, u8"능력치 HUD에 천체관 등장 확률을 표시합니다. 천체관이 해금되지 않았다면 표시되지 않습니다.")
I(OPT_REPENTOGON_SKIP_INTRO, u8"인트로 컷신 스킵")
I(OPT_REPENTOGON_SKIP_INTRO_MARK, u8"인트로 컷신을 완전히 생략합니다. 더 이상 \"아이작과 그의 엄마\"가 표시되지 않습니다.")
I(OPT_REPENTOGON_INTERPOLV2, u8"60FPS 보간")
I(OPT_REPENTOGON_INTERPOLV2_MARK, u8"렌더링 중 프레임 보간을 적용해 매우 부드러운 움직임을 만듭니다. 이 기능을 세상에 공개한 것에 대해 사과드립니다.")
I(OPT_REPENTOGON_FAST_LASERS, u8"빠른 레이저 (정확도 낮음)")
I(OPT_REPENTOGON_FAST_LASERS_MARK, u8"레이저의 정밀도를 줄이는 대신 게임 성능을 향상시킵니다. 피격 범위이 약간 변경될 수 있습니다. (레이저의 샘플링 포인트 수를 줄입니다.)")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR, u8"빠른 방 정리")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR_MARK, u8"방 클리어 후 문이 열리고 보상이 생성되는 속도를 빠르게 합니다. 보스 방은 보스 사망 애니메이션이 끝날 때까지 기다려야 합니다.")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES, u8"모드 업데이트 차단")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES_MARK, u8"게임이 창작마당 폴더와의 동기화를 건너뜁니다. 모드 개발 중 모드 폴더가 바뀌는 것을 방지할 때 유용합니다.")
I(OPT_REPENTOGON_MARSDOUBLETAP, u8"화성 이중 입력 간격")
I(OPT_REPENTOGON_MARSDOUBLETAP_MARK, u8"화성 아이템의 돌진을 위한 이중 입력 간격을 설정합니다.")
I(OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT, u8"명령어 자동 완성 제한")
I(OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT_MARK, u8"자동 완성 명령어 수를 설정합니다. 수치가 높으면 콘솔에서 입력 시 성능이 저하될 수 있습니다.")

I(OPT_REPENTOGON_FILE_MAP, u8"모드 파일 맵 생성 활성화")
I(OPT_REPENTOGON_FILE_MAP_MARK, u8"모드 파일을 사전 스캔하여 게임 시작 속도와 버벅임을 줄입니다. 문제가 발생하면 이 옵션을 비활성화하고 저희에게 보고해 주세요.")

I(OPT_REPENTOGON_ECO_MODE, u8"최소화 중 성능 조절")
I(OPT_REPENTOGON_ECO_MODE_MARK, u8"게임이 최소화될 때 렌더링을 중단하고 전력 최적화를 적용합니다. 매 프레임 렌더링에 의존하는 모드에 영향을 끼칠 수 있습니다.")

I(OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS, u8"FindInRadius 디버그 렌더링")
I(OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS_MARK, u8"DebugFlag.HITSPHERES가 활성화되었을 때, FindInRadius/QueryRadius 범위를 시각적으로 표시합니다.")

I(OPT_REPENTOGON_DISABLE_EXIT_PROMPT, u8"종료 알림 비활성화")
I(OPT_REPENTOGON_DISABLE_EXIT_PROMPT_MARK, u8"게임을 종료하려 할 때, 확인 창을 표시하지 않고 원래처럼 즉시 종료합니다.")

I(OPT_EXTRA_HUD_MODES_OFF, u8"끄기")
I(OPT_EXTRA_HUD_MODES_NORMAL, u8"일반")
I(OPT_EXTRA_HUD_MODES_MINI, u8"작게")

I(OPT_ANNOUNCER_MODES_RANDOM, u8"무작위")
I(OPT_ANNOUNCER_MODES_OFF, u8"끄기")
I(OPT_ANNOUNCER_MODES_ALWAYS_ON, u8"항상 켜기")

I(OPT_POPUP_MODES_OFF, u8"끄기")
I(OPT_POPUP_MODES_BIG, u8"크게")
I(OPT_POPUP_MODES_SMALL, u8"작게")

I(OPT_CONSOLE_FOND_MODES_DEFAULT, u8"기본")
I(OPT_CONSOLE_FOND_MODES_SMALL, u8"작게")
I(OPT_CONSOLE_FOND_MODES_TINY, u8"매우 작게")

I(OPT_OFF_ON_MODES_OFF, u8"끄기")
I(OPT_OFF_ON_MODES_ON, u8"켜기")

I(OPT_UNIFONT_RENDER_MODE_NORMAL, u8"일반: 13px, 비라틴 문자만")
I(OPT_UNIFONT_RENDER_MODE_LARGE, u8"깨지지 않게 크게: 16px")
I(OPT_UNIFONT_RENDER_MODE_MEDIUM, u8"중간: 14px")
I(OPT_UNIFONT_RENDER_MODE_TINY, u8"깨지지 않게 작게: 16px and 0.5 scale")
I(OPT_UNIFONT_RENDER_MODE_TINY_LOW, u8"매우 작게: 8px")

//============== Help Menu ===============

// Egg Game
I(HELP_EGG_PRESENT, u8"선물이다!")
I(HELP_EGG_DELI_COOKIE, u8"맛있는 쿠키. 한 입 먹었다.")
I(HELP_EGG_GREAT_COOKIE, u8"쿠키가 정말 맛있다!")
I(HELP_EGG_EATEN_COOKIE, u8"쿠키를 전부 먹어치웠다.")

I(HELP_EGG_ANOTHER_GIFT, u8"또 다른 선물이 있다...")
I(HELP_EGG_WEIRED, u8"선물을 챙겼다! 냄새가 이상한데...")
I(HELP_EGG_PIECE_OF_SHIT, u8"그냥 응아덩어리다...\n영앙가는 없어 보인다...")

I(HELP_EGG_TOILET, u8"응아를 변기에 버렸다!")
I(HELP_EGG_PRESENT_BEHIND_TOILET, u8"변기 뒤를 보니 근사한 선물이 있다.")
I(HELP_EGG_ANCIENT_EGG, u8"고대의 알처럼 생겼다. 따듯한 기운이 느껴진다...")
I(HELP_EGG_DRAGON_FROM_EGG, u8"알에서 용이 태어났다!")
I(HELP_EGG_HUNGRY_DRAGON, u8"용이 배고파 보이고 보살핌을 못 받은 것 같다.")
I(HELP_EGG_FEED_DRAGON, u8"용에게 먹이를 주려 한다...")
I(HELP_EGG_LEMON, u8"용에게 레몬을 줬다.\n혼란스런 눈빛으로 쳐다본다...")
I(HELP_EGG_CARROT, u8"당근을 줬다.\n마음에 들지 않는 것 같다...")
I(HELP_EGG_BACON, u8"베이컨을 줬다.\n맛있게 먹는다!")
I(HELP_EGG_ATTENTION, u8"용이 관심을 원한다!")
I(HELP_EGG_PET_AND_LOVE, u8"용을 쓰다듬으며 사랑을 듬뿍 준다.")
I(HELP_EGG_DRAGON_HAPPY, u8"용이 행복해 보인다!")

I(HELP_EGG_RIDEABLE, u8"이 용을 타고 날 수 있을지 궁금해졌다.")
I(HELP_EGG_TAKE_OFF, u8"나와 용이 하늘로 날아오른다!")
I(HELP_EGG_INSEPARABLE, u8"이제 용과는 떼려야 뗄 수 없는 관계라고 느껴진다.")
I(HELP_EGG_REACH_THE_HEAVEN, u8"계속 올라간다!\n마침내 천국에 도달했다.")
I(HELP_EGG_GOD_SMITES_DRAGON, u8"하지만 신은 이런 장난을 용납하지 않았다.\n용은 신의 벼락에 맞았고, 나는 미지의 심연으로 떨어졌다.")
I(HELP_EGG_YOU_SURVIVE, u8"나는 살아남았지만, 용은 즉사했다...")

I(HELP_EGG_REVENGE, u8"나는 복수를 다짐한다.\n하지만 어떻게...")
I(HELP_EGG_REBUILD, u8"우린 부활시킬 수 있다.")
I(HELP_EGG_STRONGER, u8"더 강하게 만들 수 있다.")
I(HELP_EGG_REBORN_AS_GUN, u8"내 용이 총으로 다시 태어났다!")

I(HELP_EGG_RIDEABLE_AGAIN, u8"합법적인 드래'건'을 탈 수 있을까?")
I(HELP_EGG_TAKE_OFF_AGAIN, u8"나와 이 노골적인 용은 하늘로 날아오른다!")
I(HELP_EGG_INSEPARABLE_AGAIN, u8"이 노골적인 용과는 떼려야 뗄 수 없는 관계라고 느껴진다.")
I(HELP_EGG_ASCENDING_1, u8"계속 올라간다!")
I(HELP_EGG_ASCENDING_2, u8"계속해서 올라간다!")
I(HELP_EGG_ASCENDING_3, u8"계속 더 올라간다!")
I(HELP_EGG_ASCENDING, u8"계속 올라간다...")

I(HELP_EGG_SINS_CRAWLING, u8"죄악이 등을 타고 오르는 것을 느꼈다.")
I(HELP_EGG_NO_HEAVEN, u8"어리석군, 천국 따위는 존재하지 않아.")
I(HELP_EGG_NO_DRAGON, u8"여긴 용도 없어.")
I(HELP_EGG_NOTHING, u8"여긴 아무 것도 없어.")

I(HELP_EGG_HOWDY, u8"반가워! 나야, 플라위. 노란 꽃 플라위!")
I(HELP_EGG_I_OWE_YOU, u8"너한테 정말 큰 빛을 졌어! 진짜 그 늙은 멍청이를 쓰러뜨렸구나!\n네가 없었더라면, 그 놈을 이길 순 없었을 거야!")
I(HELP_EGG_WITH_YOUR_HELP, u8"하지만, 네 도움으로...")
I(HELP_EGG_HE_DEAD, u8"놈은 죽었어.")
I(HELP_EGG_GOT_HUMAN_SOUL, u8"그리고 난 인간의 영혼을 손에 넣었지!")
I(HELP_EGG_SOUL_INSIDE_ME_AGAIN, u8"세상에! 너무 오랫동안 공허했었어...\n다시 \"영혼\"을 얻는다는 거 기분 좋은데.")
I(HELP_EGG_FELL_WRIGGLING, u8"으음, 꿈틀대는 게 느껴져...")
I(HELP_EGG_FEELING_LEFT_OUT, u8"어우, 왕따가 된 기분인가 보네, 그렇지?")
I(HELP_EGG_NEED_ONE_MORE_SOUL, u8"뭐, 그럼 완벽하네. 어쨌거나, 난 영혼이 6개밖에 없거든.\n하나가 더 필요해...")
I(HELP_EGG_BEFORE_BECOME_GOD, u8"그리고 '신'이 되는 거야.")
I(HELP_EGG_POWER, u8"그러면, 내 새로운 힘으로...")
I(HELP_EGG_MONSTER, u8"괴물들...")
I(HELP_EGG_HUMAN, u8"인간들...")
I(HELP_EGG_EVERYONE, u8"모두...")
I(HELP_EGG_SHOW_MEANING, u8"모두에게 이 세상의 진짜 의미를 보여줘야지!")
I(HELP_EGG_STILL_GUN, u8"용은 없을지 몰라도 총은 그대로 있다.\n나는 총을 뽑아 플라위를 한 번에 죽였다.")
I(HELP_EGG_POWER_OF_REPENTOGON, u8"세상을 구했다! 이게 바로 REPENTOGON의 힘이다!")

// Controls

I(HELP_CTRL_PS_DPAD, u8"십자키")
I(HELP_CTRL_PS_DPAD_DESC, u8"이동, 값 조절 (입력 시 활성화)")
I(HELP_CTRL_PS_TOUCH, u8"터치패드")
I(HELP_CTRL_PS_TOUCH_DESC, u8"마우스")
I(HELP_CTRL_PS_LSTICK, u8"L 스틱")
I(HELP_CTRL_PS_LSTICK_DESC, u8"스크롤")
I(HELP_CTRL_PS_SQUARE, u8"□")
I(HELP_CTRL_PS_SQUARE_DESC, u8"누르기: 메뉴 전환\n길게 누르기 + L1/R1: 창 전환\n길게 누르기 + 십자키: 창 크기 조절\n길게 누르기 + L 스틱: 창 이동")
I(HELP_CTRL_PS_TRIANGLE, u8"△")
I(HELP_CTRL_PS_TRIANGLE_DESC, u8"텍스트 편집 / 가상 키보드")
I(HELP_CTRL_PS_CIRCLE, u8"○")
I(HELP_CTRL_PS_CIRCLE_DESC, u8"취소 / 닫기 / 종료")
I(HELP_CTRL_PS_CROSS, u8"×")
I(HELP_CTRL_PS_CROSS_DESC, u8"활성화 / 열기 / 토글\n십자키로 값 고정\n(+L1/R1으로 느리게/빠르게)")

I(HELP_CTRL_XB_DPAD, u8"십자키")
I(HELP_CTRL_XB_DPAD_DESC, u8"이동, 값 조절 (입력 시 활성화)")
I(HELP_CTRL_XB_STICK, u8"L 스틱")
I(HELP_CTRL_XB_STICK_DESC, u8"스크롤")
I(HELP_CTRL_XB_X, u8"X")
I(HELP_CTRL_XB_X_DESC, u8"누르기: 메뉴 전환\n길게 누르기 + L1/R1: 창 전환\n길게 누르기 + 십자키: 창 크기 조절\n길게 누르기 + L 스틱: 창 이동")
I(HELP_CTRL_XB_Y, u8"Y")
I(HELP_CTRL_XB_Y_DESC, u8"텍스트 편집 / 가상 키보드")
I(HELP_CTRL_XB_B, u8"B")
I(HELP_CTRL_XB_B_DESC, u8"취소 / 닫기 / 종료")
I(HELP_CTRL_XB_A, u8"A")
I(HELP_CTRL_XB_A_DESC, u8"활성화 / 열기 / 토글\n십자키로 값 고정\n(+L1/R1으로 느리게/빠르게)")

I(HELP_CTRL_SWITCH_DPAD, u8"십자키")
I(HELP_CTRL_SWITCH_DPAD_DESC, u8"이동, 값 조절 (입력 시 활성화)")
I(HELP_CTRL_SWITCH_LSTICK, u8"L 스틱")
I(HELP_CTRL_SWITCH_LSTICK_DESC, u8"스크롤")
I(HELP_CTRL_SWITCH_Y, u8"Y")
I(HELP_CTRL_SWITCH_Y_DESC, u8"누르기: 메뉴 전환\n길게 누르기 + L1/R1: 창 전환\n길게 누르기 + 십자키: 창 크기 조절\n길게 누르기 + L 스틱: 창 이동")
I(HELP_CTRL_SWITCH_X, u8"X")
I(HELP_CTRL_SWITCH_X_DESC, u8"텍스트 편집 / 가상 키보드")
I(HELP_CTRL_SWITCH_B, u8"B")
I(HELP_CTRL_SWITCH_B_DESC, u8"취소 / 닫기 / 종료")
I(HELP_CTRL_SWITCH_A, u8"A")
I(HELP_CTRL_SWITCH_A_DESC, u8"활성화 / 열기 / 토글\n십자키로 값 고정\n(+L1/R1으로 느리게/빠르게)")

// Help Menu
I(HELP_MENU, ICON_U8_FA_CIRCLE_QUESTION u8" 도움말")
I(HELP_MENU_CTRLS, ICON_U8_FA_GAMEPAD u8" 조작법")
I(HELP_MENU_CTRL_MOUSE, u8"마우스")
I(HELP_MENU_CTRL_MOUSE_HINT, u8"텍스트 확대/축소: 텍스트/창 위에 마우스를 올려놓고 CTRL + 마우스 휠로 확대/축소")

I(HELP_MENU_CTRL_KEYBOARD, u8"키보드")
I(HELP_MENU_CTRL_CONTROLLER, u8"컨트롤러")

I(HELP_MENU_CTRL_PLAYSTATION, u8"PlayStation")
I(HELP_MENU_CTRL_PLAYSTATION_TEXT, u8"DualShock4 컨트롤러 설명")

I(HELP_MENU_CTRL_XBOX, u8"Xbox")
I(HELP_MENU_CTRL_XBOX_TEXT, u8"Xbox 컨트롤러 설명")

I(HELP_MENU_CTRL_SWITCH, u8"Switch")
I(HELP_MENU_CTRL_SWITCH_TEXT, u8"Joy-Con 컨트롤러 설명")

I(HELP_MENU_DOC, ICON_U8_FA_BOOK u8" 개발 문서")
I(HELP_MENU_DOC_TEXT, u8"오른쪽 링크에서 문서 확인: ")

I(HELP_MENU_TRANSLATE, ICON_U8_FA_GLOBE u8" 번역 정보")

// ============ ImGui ===============

I(IMGUI_WIN_CTX_MENU_PIN_WINDOW, u8"창 고정")
I(IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC, u8"고정된 창은 개발자 도구를 닫아도 계속 표시됩니다.")

I(IMGUI_CHANGE_KEY_BTN_NAME, u8"변경")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_CTRL, u8"컨트롤러의 아무 버튼이나 누르세요.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_KEYBOARD, u8"키보드의 아무 키나 누르세요.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_ESC, u8"ESC를 눌러 취소")

// ============= Log Viewer ==============

I(LOGV_WIN_NAME, u8"로그 뷰어")
I(LOGV_FILTER, u8"필터")
I(LOGV_PIN_WINDOW, u8"창 고정")
I(LOGV_PIN_WINDOW_HINT, u8"고정된 창은 개발자 도구를 닫아도 계속 표시됩니다.")
I(LOGV_AUTOSCROLL, u8"자동 스크롤")
I(LOGV_SHOW_CATEGORIES, u8"카테고리 표시:")

// ============ Performance Window ==========

I(PERF_WIN_NAME, u8"성능")
I(PERF_TIMEFRAME_NAME, u8"시간 범위")
I(PERF_TIMEFRAME_FORMAT, u8"%.1f초")
I(PERF_LUA_MEM_USE, u8"Lua 메모리 사용량")

// =========== Translate Report ============

I(TRANS_LANG_JAPANESE, u8"일본어")
I(TRANS_LANG_KOREAN, u8"한국어")
I(TRANS_LANG_CHINESE_SIMPLE, u8"중국어(간체)")
I(TRANS_LANG_RUSSIAN, u8"러시아어")
I(TRANS_LANG_GERMAN, u8"독일어")
I(TRANS_LANG_SPANISH, u8"스페인어")
I(TRANS_LANG_FRENCH, u8"프랑스어")

I(TRANS_REPORT_WIN_NAME, u8"번역 보고서")
I(TRANS_LANGUAGE_HINT, u8"%s의 번역 정보")
I(TRANS_UNTRANS_ITEMS, u8"미번역 항목:")
I(TRANS_COUNT_HINT, u8"총 항목: %d 번역된 항목: %d")
