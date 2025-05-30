/* This is saved as UTF-8 with BOM(code page 65001) */
#ifndef I
#define I(ITEM, TRANSLATE)
#endif

#ifndef V
#define V(TYPE, VAR, VALUE)
#endif

// V(float, UNIFONT_DEFAULT_SIZE, 13)

//=========== Main Menu Bar ==========
I(BAR_TOOLS, ICON_U8_FA_SCREWDRIVER_WRENCH u8" Herramientas")
I(BAR_DEBUG_CONSOLE, ICON_U8_FA_TERMINAL u8" Consola de Depuración")
I(BAR_LOG_VIEWER, ICON_U8_FA_NEWSPAPER u8" Visor de Registros")
I(BAR_GAME_OPTIONS, ICON_U8_FA_GEARS u8" Configuración del juego")
I(BAR_PERFORMANCE, ICON_U8_FA_GAUGE_HIGH u8" Rendimiento")
I(BAR_STYLE_EDITOR, ICON_U8_FA_PENCIL u8" Editor de Estilos")

I(DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, u8"Editor de Estilo ImGui Estimado")

//=========== Console Mega =============

I(CONSOLE_WINDOW_NAME, u8"Consola")

I(CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, u8"[ERROR] %s can't be used if not in-game!")
I(CONSOLE_HELP_OUTSIDE_GAME_HINT, u8"(Only commands enabled to show outside of the game will appear right now.)\n")
I(CONSOLE_NO_MACRO_HINT, u8"No hay macro con ese nombre.\n")

I(CONSOLE_CMD_HINT, u8"Escriba su comando aqui (\"help\" para ver ayuda)")

I(CONSOLE_DEBUG_HINT_1, u8"Posiciones de Entidades")
I(CONSOLE_DEBUG_HINT_2, u8"Cuadricula")
I(CONSOLE_DEBUG_HINT_3, u8"Salud Infinita")
I(CONSOLE_DEBUG_HINT_4, u8"Daño Alto")
I(CONSOLE_DEBUG_HINT_5, u8"Mostrar Informacion de sala")
I(CONSOLE_DEBUG_HINT_6, u8"Mostrar Hitspheres")
I(CONSOLE_DEBUG_HINT_7, u8"Mostrar Valores de Daño")
I(CONSOLE_DEBUG_HINT_8, u8"Cargas de Objeto Infinitas")
I(CONSOLE_DEBUG_HINT_9, u8"Suerte Alta")
I(CONSOLE_DEBUG_HINT_10, u8"Muerte Rápida")
I(CONSOLE_DEBUG_HINT_11, u8"Informacion de Cuadricula")
I(CONSOLE_DEBUG_HINT_12, u8"Informacion del Objeto de Jugador")
I(CONSOLE_DEBUG_HINT_13, u8"Mostrar Puntos de Colision de Cuadricula")
I(CONSOLE_DEBUG_HINT_14, u8"Mostrar uso de Memoria Lua")

I(CONSOLE_COMBO_HINT_0, u8"Tesoro")
I(CONSOLE_COMBO_HINT_1, u8"Tienda")
I(CONSOLE_COMBO_HINT_2, u8"Jefe")
I(CONSOLE_COMBO_HINT_3, u8"Diablo")
I(CONSOLE_COMBO_HINT_4, u8"Ángel")
I(CONSOLE_COMBO_HINT_5, u8"Secreta")
I(CONSOLE_COMBO_HINT_6, u8"Biblioteca")
I(CONSOLE_COMBO_HINT_7, u8"Desafío")
I(CONSOLE_COMBO_HINT_8, u8"Cofre Dorado")
I(CONSOLE_COMBO_HINT_9, u8"Cofre rojo")
I(CONSOLE_COMBO_HINT_10, u8"Mendigo")
I(CONSOLE_COMBO_HINT_11, u8"Mendigo Demoníaco")
I(CONSOLE_COMBO_HINT_12, u8"Maldicion")
I(CONSOLE_COMBO_HINT_13, u8"Mendigo de Llaves")
I(CONSOLE_COMBO_HINT_14, u8"Desafío de Jefes")
I(CONSOLE_COMBO_HINT_15, u8"Mazmorra")

//=========== Console Mega Commands ==========
I(CONSOLE_ACHIEVEMENT_DESC, u8"Desbloquea Logros")
I(CONSOLE_ACHIEVEMENT_HELP, u8"Desbloquea logros")

I(CONSOLE_ADD_PLAYER_DESC, u8"Genera un nuevo Jugador")
I(CONSOLE_ADD_PLAYER_HELP, u8"Genera una nueva entidad de Jugador. Por defecto, genera a Isaac con ID de controlador 0.\nPlayer ID -1 te permite tomar control de un enemigo aleatorio en la sala.\nEjemplo:\n(addplayer 7 1) Genera a Azazel y puede ser controlado con el segundo dispositivo de entrada (controlador 1 la mayoría de las veces)")

I(CONSOLE_CHALLENGE_DESC, u8"Iniciar nueva Partida de Desafío")
I(CONSOLE_CHALLENGE_HELP, u8"Detiene la partida actual e inicia una nueva en una semilla aleatoria con la ID del desafio proporcionada.\nEjemplo:\n(challenge 20) Iniciará una nueva partida del desafio \"Purista\".\n")

I(CONSOLE_CLEAR_DESC, u8"Limpiar la consola de depuración")
I(CONSOLE_CLEAR_HELP, u8"Borra todo el texto dentro de la consola de depuración. Solo permanecerá la linea \"Repentance Console\".")

I(CONSOLE_CLEARCACHE_DESC, u8"Limpiar caché de sprites")
I(CONSOLE_CLEARCACHE_HELP, u8"Borra la caché de sprites del juego. Puede ser útil al intentar solucionar problemas de memoria.\nTambien permite recargar los sprites de mods sin tener que reiniciar el juego por completo.")

I(CONSOLE_CLEARSEEDS_DESC, u8"Limpiar semillas \"easter egg\" de la partida actual")
I(CONSOLE_CLEARSEEDS_HELP, u8"Limpia cualquier efecto \"especial\" de semilla en la partida actual.\nEjemplo:\nIniciar una partida con la semilla GFVE LLLL aplicara su efecto. Ejecutar \"clearseeds\" removerá ese efecto.")

I(CONSOLE_COMBO_DESC, u8"Otorga objetos de una pool especificada")
I(CONSOLE_COMBO_HELP, u8"Otorga un número especifico de objetos de una pool de objetos especifica.\nEjemplo:\n(combo 4.6) otorgara seis objetos aleatorios from the Angel item pool.\nNo, No sé por qué hicieron un sistema de ID a la medida para este comando.")

I(CONSOLE_COPY_DESC, u8"Copia los comandos anteriores al portapapeles")
I(CONSOLE_COPY_HELP, u8"Copia un número especifico de comandos anteriores de la consola al portapapeles del sistema.\nEjemplo:\n(copy 3) copiará los tres comandos anteriores.")

I(CONSOLE_COSTUMETEST_DESC, u8"Otorga al jugador trajes aleatorios")
I(CONSOLE_COSTUMETEST_HELP, u8"Otorga al jugador una cantidad especifica de trajes aleatorios.\nEjemplo:\n(costumetest 34) otorgara al jugador 34 trajes aleatorios.")

I(CONSOLE_CURSE_DESC, u8"Añade maldiciones a la partida actual")
I(CONSOLE_CURSE_HELP, u8"Añade permanentemente (hasta ser anulado) una maldición a la partida. Este comando usa un bitmask- la maldición con una ID de 1 es 1, 2 es 2, 3 es 4, 4 es 8, etc. De esta manera, se suman las IDs de las maldiciones deseadas permitiendo habilitar multiples simultáneamente.\nEjemplo:\n(curse 96) activará la Maldición del ciego y la Maldición del Laberinto simultaneamente.")

I(CONSOLE_CUTSCENE_DESC, u8"Reproducir cinematica")
I(CONSOLE_CUTSCENE_HELP, u8"Reproduce inmediatamente la cinematica especificada.\nEjemplo:\n(cutscene 1) reproducirá la intro del juego.")

I(CONSOLE_DEBUG_DESC, u8"Activar un flag de depuración")
I(CONSOLE_DEBUG_HELP, u8"Activa la flag de depuración especifica.\nEjemplo:\n(debug 3) otorgará al jugador salud infinita.")

I(CONSOLE_DELIRIOUS_DESC, u8"Forzar Delirious a volverse un Jefe determinado")
I(CONSOLE_DELIRIOUS_HELP, u8"Anula el próximo jefe en el que se convertirá el objeto Delirious.\nEjemplo:\n(delirious 3) forzará Delirious a transformarse en Chub.")

I(CONSOLE_EGGS_DESC, u8"Desbloquea todas las semillas \"easter egg\"")
I(CONSOLE_EGGS_HELP, u8"PERMANENTEMENTE desbloquea todos los easter eggs en este archivo de guardado.")

I(CONSOLE_FORCEROOM_DESC, u8"Forzar el uso de una sala en el generador de niveles")
I(CONSOLE_FORCEROOM_HELP, u8"Permite establecer cualquier sala como \"sala obligatoria\". Dicha sala tendra un peso de 1000, volviendola más probable que aparezca en el suelo con el comando reseed.")

I(CONSOLE_FULLRESTART_DESC, u8"Reiniciar el juego")
I(CONSOLE_FULLRESTART_HELP, u8"Cierra y vuelve a abrir el juego")

I(CONSOLE_GIVEITEM_DESC, u8"Otorga al jugador objetos, trinkets, cartas, y pildoras")
I(CONSOLE_GIVEITEM_HELP, u8"Otorga the jugador principal objetos, trinkets, cartas, y pildoras. Estos pueden ser por nombre o por prefijo. Los prefijos son (c) para objetos, (t) para trinkets, (T) para trinkets dorados, (p) para pildoras, (P) para pildoras de caballo, y (k) para cartas. La mayoría de objetos de bolsillo cuentan como cartas.\Este comando también tiene una abreviatura (g).\nEjemplos:\n(giveitem c1) le dará al jugador Sad Onion.\n(giveitem t1) le dará al jugador ¡Gulp!\n(giveitem p1) le dará al jugador la pildora Mal viaje.\n(giveitem k1) le dará al jugador 0 - El Bufón.")

I(CONSOLE_GIVEITEM2_DESC, u8"Otorga al segundo jugador objetos, trinkets, cartas, y pildoras")
I(CONSOLE_GIVEITEM2_HELP, u8"Otorga al segundo jugador objetos, trinkets, cartas y pildoras. Estos pueden ser por nombre o por prefijo. Los prefijos son (c) para objetos, (t) para trinkets, (T) para trinkets dorados, (p) para pildoras, (P) para pildoras de caballo, y (k) para cartas. La mayoría de objetos de bolsillo cuentan como cartas.\Este comando también tiene una abreviatura (g).\nEjemplos:\n(giveitem c1) le dará al jugador Sad Onion.\n(giveitem t1) le dará al jugador ¡Gulp!\n(giveitem p1) le dará al jugador la pildora Mal viaje.\n(giveitem k1) le dará al jugador 0 - El Bufón.")

I(CONSOLE_GOTO_DESC, u8"Te teletransporta a una habitación nueva")
I(CONSOLE_GOTO_HELP, u8"Teletransporta al personaje a una nueva habitación. Usa (d) para una sala común, (s) para una sala especial, o tres numeros para teletransportarte a una sala existente del piso actual.\nEjemplo:\n(goto s.boss.1010) te llevará a la pelea de Monstro.")

I(CONSOLE_GRIDSPAWN_DESC, u8"Genera una entidad de cuadricula")
I(CONSOLE_GRIDSPAWN_HELP, u8"Genera una nueva entidad de cuadricula de la ID especificada en un punto aleatorio de la sala.")

I(CONSOLE_HELP_DESC, u8"Obtiene información sobre los comandos")
I(CONSOLE_HELP_HELP, u8"Proporciona información adicional sobre un comando y su sintaxis.")

I(CONSOLE_LISTCOLLECTIBLES_DESC, u8"Listar objetos actuales")
I(CONSOLE_LISTCOLLECTIBLES_HELP, u8"Enumera los objetos que el jugador tiene actualmente.")

I(CONSOLE_LOCKACHIEVEMENT_DESC, u8"Bloquear Logros")
I(CONSOLE_LOCKACHIEVEMENT_HELP, u8"Bloquea los logros")

I(CONSOLE_LUA_DESC, u8"Ejecutar codigo Lua")
I(CONSOLE_LUA_HELP, u8"Ejecuta el codigo Lua dado inmediatamente. Cualquier cosa que funcione en un archivo estándar funcionará aquí.\nEste comando también tiene una abreviatura (l).")

I(CONSOLE_LUAMEM_DESC, u8"Mostrar uso de memoria lua")
I(CONSOLE_LUAMEM_HELP, u8"Muestra la RAM utilizada actualmente de LUA.")

I(CONSOLE_LUAMOD_DESC, u8"Recargar un mod Lua")
I(CONSOLE_LUAMOD_HELP, u8"Recarga el codigo Lua de la carpeta de mod especificada.\nEjemplo:\n(luamod testmod) recargará el codigo Lua para el mod en la carpeta \"testmod\".")

I(CONSOLE_LUARESET_DESC, u8"[EXPERIMENTAL] Reiniciar el contexto Lua")
I(CONSOLE_LUARESET_HELP, u8"Destruye el contexto Lua actual y lo recrea desde cero. Este es esencialmente un comando de backend para ayudar a sincronizar el juego en red.\nEste tiene Consecuencias Imprevistas al ejecutarse in-game, Por favor solo usalo en el menu a menos que sepas lo que haces. Plis?")

I(CONSOLE_LUARUN_DESC, u8"Ejecutar un archivo Lua")
I(CONSOLE_LUARUN_HELP, u8"Ejecuta el archivo Lua dado inmediatamente.\nEjemplo:\n(luarun mods/test/test.lua) ejecutará \"test.lua\" dentro de la carpeta del mod \"test\".")

I(CONSOLE_MACRO_DESC, u8"Activar un conjunto de comandos")
I(CONSOLE_MACRO_HELP, u8"Activa un conjunto de comandos en el orden especifcado. These are effectively shortcuts. Consulte el autocompletado para obtener una lista de comandos macro.")

I(CONSOLE_METRO_DESC, u8"Forzar Metronomo a ser un objeto especifico")
I(CONSOLE_METRO_HELP, u8"Anula el proximo objeto en que Metronome cambiara.\nEjemplo:\n(metro c1) obligará a Metronomo a convertirse en Sad Onion.")

I(CONSOLE_NETDELAY_DESC, u8"Cambiar latencia de red")
I(CONSOLE_NETDELAY_HELP, u8"Cambia la latencia de red a un valor especifico. Puede ser util si notas cortes al jugar en linea.")

I(CONSOLE_NETSTART_DESC, u8"Iniciar cooperativo online")
I(CONSOLE_NETSTART_HELP, u8"Conecta jugador(es) con la Steam ID especificada a tu juego (multijugador online). Permite hasta 4 jugadores.\nEjemplo:\nnetstart <steam_user_id1> <steam_user_id2>")

I(CONSOLE_PLAYSFX_DESC, u8"Reproducir efecto de sonido")
I(CONSOLE_PLAYSFX_HELP, u8"Reproduce un efecto de sonido inmediatamente.\nEjemplo:\n(playsfx 187) reproducirá un boton de incorrecto.")

I(CONSOLE_PROF_DESC, u8"[ROTO] Start profiling")
I(CONSOLE_PROF_HELP, u8"Supuestamente almacena informacion en un CSV. Culpa a Nicalis!")

I(CONSOLE_PROFSTOP_DESC, u8"[BROKEN] Stop profiling")
I(CONSOLE_PROFSTOP_HELP, u8"Supuestamente deberia detener profiling pero profiling no funciona porque no podemos tener cosas bonitas.")

I(CONSOLE_REMOVE_DESC, u8"Remover un objeto")
I(CONSOLE_REMOVE_HELP, u8"Remueve un objeto del jugador inmediatamente. Acepta la misma sintaxis que \"give\", revisar la ayuda de ese comando para mas información.")

I(CONSOLE_REMOVE2_DESC, u8"Remover un objeto")
I(CONSOLE_REMOVE2_HELP, u8"Remueve un objeto del segundo jugador inmediatamente. Acepta la misma sintaxis que \"give\", revisar la ayuda de ese comando para mas información.")

I(CONSOLE_RELOADFX_DESC, u8"Recargar capa del piso")
I(CONSOLE_RELOADFX_HELP, u8"Recarga los efectos del piso actual.")

I(CONSOLE_RELOADSHADERS_DESC, u8"Recargar sombreadores del juego")
I(CONSOLE_RELOADSHADERS_HELP, u8"Recarga todo sombreador cargado actualmente.")

I(CONSOLE_RELOADWISPS_DESC, u8"Recargar wisps")
I(CONSOLE_RELOADWISPS_HELP, u8"Recarga los wisps generados por Book of Virtues y langostas generadas por Abyss.")

I(CONSOLE_REPEAT_DESC, u8"Repetir comando anterior")
I(CONSOLE_REPEAT_HELP, u8"Repite el comando ingresado previamente X cantidad de veces.\nEjemplo:\n(giveitem 1) es usado para darle al jugadore \"Sad Onion\". (repeat 5) es usado para darle al jugador \"Sad Onion\" cinco veces mas.")

I(CONSOLE_RESEED_DESC, u8"Regenerar el piso actual")
I(CONSOLE_RESEED_HELP, u8"Reinicia el piso actual, generando un nuevo diseño.")

I(CONSOLE_RESTART_DESC, u8"Reiniciar en una nueva partida")
I(CONSOLE_RESTART_HELP, u8"Reinicia el juego en una nueva partida. Opcionalmente permite elegir un personaje especifico con su ID.\nEjemplo:\n(restart 3) inicia una nueva partida como Judas.")

I(CONSOLE_RESTOCK_DESC, u8"Reabastecer tiendas")
I(CONSOLE_RESTOCK_HELP, u8"Reabastece todas las tiendas.")

I(CONSOLE_REWIND_DESC, u8"Restablecer juego hasta el último estado de la sala")
I(CONSOLE_REWIND_HELP, u8"Hace que el juego olvide los cambios en la sala actual y teletransporta a Isaac a la sala anterior. Puede utilizarse para solucionar problemas de sincronización si se usa este comando en la sala donde sucedio. (Efecto similar al \"Glowing Hour Glass\")")

I(CONSOLE_SEED_DESC, u8"Iniciar nueva partida con semilla establecida")
I(CONSOLE_SEED_HELP, u8"Inicia una nueva partida con la semilla establecida.\nEjemplo:\n(seed N1CA L1SY) Iniciará una nueva partida con la semilla N1CA L1SY.")

I(CONSOLE_SPAWN_DESC, u8"Generar entidad")
I(CONSOLE_SPAWN_HELP, u8"Genera una nueva entidad. La sintaxis es (tipo).(variante).(subtipo).(campeon).\nEjemplo:\n(spawn 5.40.1) generará una bomba.")

I(CONSOLE_STAGE_DESC, u8"Ir a un escenario")
I(CONSOLE_STAGE_HELP, u8"Te envia inmediatamente al escenario especificado. Acepta (a-d) como modificadores, donde (a) corresponde a rutas alt de WOTL, (b) coresponde a rutas alt de Afterbirth, (c) corresponde a rutas alt de Antibirth, y (d) corresponde a rutas alt de Repentance.\nEjemplo:\n(stage 4d) enviará al jugador a Ashpit II.")

I(CONSOLE_TIME_DESC, u8"Imprimir tiempo de juego")
I(CONSOLE_TIME_HELP, u8"Imprime en la consola la cantidad total de tiempo en la partida.")

I(CONSOLE_TESTBOSSPOOL_DESC, u8"Imprimir lista de jefes del piso actual")
I(CONSOLE_TESTBOSSPOOL_HELP, u8"Imprime en la consola una lista de nombres de jefes y porcentajes (100%=10000) para el piso actual.")

I(CONSOLE_FORCEIMPORT_DESC, u8"[EXPERIMENTAL] Forzar la importación de un archivo de guardado de un DLC anterior.")
I(CONSOLE_FORCEIMPORT_HELP, u8"Utiliza la ID del archivo de guardado como argumento.")

I(CONSOLE_STAGE_BLUE_WOMB, u8"??? / Vientre Azul")
I(CONSOLE_STAGE_HOME_DAY, u8"Hogar (dia)")
I(CONSOLE_STAGE_HOME_NIGHT, u8"Hogar (noche)")

I(CONSOLE_GRID_DECORATION_A, u8"Decoracion A")
I(CONSOLE_GRID_DECORATION_B, u8"Decoracion B")
I(CONSOLE_GRID_DECORATION_C, u8"Decoracion C")
I(CONSOLE_GRID_ROCK, u8"Roca")
I(CONSOLE_GRID_ROCK_CONNECTING, u8"Roca (conectada)")
I(CONSOLE_GRID_BOMB_ROCK, u8"Roca Explosiva")
I(CONSOLE_GRID_ALT_ROCK, u8"Roca Alterna")
I(CONSOLE_GRID_TINTED_ROCK, u8"Roca Marcada")
I(CONSOLE_GRID_MARKED_SKULL, u8"Craneo Marcado")
I(CONSOLE_GRID_EVENT_ROCK, u8"Roca de Evento")
I(CONSOLE_GRID_SPIKED_ROCK, u8"Roca con Espinas")
I(CONSOLE_GRID_FOOLS_GOLD_ROCK, u8"Roca Dorada")
I(CONSOLE_GRID_TNT, u8"TNT")
I(CONSOLE_GRID_FIRE_PLACE, u8"Fogata")
I(CONSOLE_GRID_RED_FIRE_PLACE, u8"Fogata Roja")
I(CONSOLE_GRID_RED_POOP, u8"Caca Roja")
I(CONSOLE_GRID_RAINBOW_POOP, u8"Caca Arcoiris")
I(CONSOLE_GRID_CORNY_POOP, u8"Caca Corny")
I(CONSOLE_GRID_GOLDEN_POOP, u8"Caca Dorada")
I(CONSOLE_GRID_BLACK_POOP, u8"Caca Negra")
I(CONSOLE_GRID_WHITE_POOP, u8"Caca Blanca")
I(CONSOLE_GRID_GIANT_POOP, u8"Caca Gigante")
I(CONSOLE_GRID_POOP, u8"Caca")
I(CONSOLE_GRID_CHARMING_POOP, u8"Caca Encantadora")
I(CONSOLE_GRID_BLOCK, u8"Bloque")
I(CONSOLE_GRID_PILLAR, u8"Pilar")
I(CONSOLE_GRID_SPIKES, u8"Espinas")
I(CONSOLE_GRID_SANGUINE_BOND_SPIKES, u8"Espinas de Vinculo de Sangre")
I(CONSOLE_GRID_RETRACTABLE_SPIKES, u8"Espinas Retraibles")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_1_5, u8"Espinas Retraibles (abajo 1/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_2_5, u8"Espinas Retraibles (abajo 2/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_3_5, u8"Espinas Retraibles (abajo 3/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_4_5, u8"Espinas Retraibles (abajo 4/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_DOWN_5_5, u8"Espinas Retraibles (abajo 5/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_1_5, u8"Espinas Retraibles (arriba 1/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_2_5, u8"Espinas Retraibles (arriba 2/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_3_5, u8"Espinas Retraibles (arriba 3/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_4_5, u8"Espinas Retraibles (arriba 4/5)")
I(CONSOLE_GRID_RETRACTABLE_SPIKES_UP_5_5, u8"Espinas Retraibles (arriba 5/5)")
I(CONSOLE_GRID_COBWEB, u8"Telaraña")
I(CONSOLE_GRID_INVISIBLE_BLOCK, u8"Bloque Invisible")
I(CONSOLE_GRID_PIT, u8"Pozo")
I(CONSOLE_GRID_FISSURE_SPAWNER, u8"Generador de Fissure")
I(CONSOLE_GRID_EVENT_RAIL, u8"Riel de Evento")
I(CONSOLE_GRID_EVENT_PIT, u8"Pozo de Evento")
I(CONSOLE_GRID_KEY_BLOCK, u8"Bloque Cerrado")
I(CONSOLE_GRID_PRESSURE_PLATE, u8"Placa de Presion")
I(CONSOLE_GRID_REWARD_PLATE, u8"Placa de Recompensa")
I(CONSOLE_GRID_GREED_PLATE, u8"Placa de Greed")
I(CONSOLE_GRID_RAIL_PLATE, u8"Placa de Riel")
I(CONSOLE_GRID_KILL_PLATE, u8"Placa de Muerte")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_0, u8"Placa de Evento (grupo 0)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_1, u8"Placa de Evento (grupo 1)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_2, u8"Placa de Evento (grupo 2)")
I(CONSOLE_GRID_EVENT_PLATE_GROUP_3, u8"Placa de Evento (grupo 3)")
I(CONSOLE_GRID_DEVIL_STATUE, u8"Estatua de Diablo")
I(CONSOLE_GRID_ANGEL_STATUE, u8"Estatua de Ángel")
I(CONSOLE_GRID_RAIL_HORIZONTAL, u8"Riel (horizontal)")
I(CONSOLE_GRID_RAIL_VERTICAL, u8"Riel (vertical)")
I(CONSOLE_GRID_RAIL_DOWN_TO_RIGHT, u8"Riel (abajo-a-derecha)")
I(CONSOLE_GRID_RAIL_DOWN_TO_LEFT, u8"Riel (abajo-a-izquierda)")
I(CONSOLE_GRID_RAIL_UP_TO_RIGHT, u8"Riel (arriba-a-derecha)")
I(CONSOLE_GRID_RAIL_UP_TO_LEFT, u8"Riel (arriba-a-izquierda)")
I(CONSOLE_GRID_RAIL_CROSSROAD, u8"Riel (cruce)")
I(CONSOLE_GRID_RAIL_END_LEFT, u8"Riel (final-izquierda)")
I(CONSOLE_GRID_RAIL_END_RIGHT, u8"Riel (final-derecha)")
I(CONSOLE_GRID_RAIL_END_UP, u8"Riel (final-arriba)")
I(CONSOLE_GRID_RAIL_END_DOWN, u8"Riel (final-abajo)")
I(CONSOLE_GRID_RAIL_CART_LEFT, u8"Riel (carrito-izquierda)")
I(CONSOLE_GRID_RAIL_CART_UP, u8"Riel (carrito-arriba)")
I(CONSOLE_GRID_RAIL_CART_RIGHT, u8"Riel (carrito-derecha)")
I(CONSOLE_GRID_RAIL_CART_DOWN, u8"Riel (carrito-abajo)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_1, u8"Riel de Mina (horizontal 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_1, u8"Riel de Mina (vertical 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_RIGHT_1, u8"Riel de Mina (abajo-a-derecha 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_LEFT_1, u8"Riel de Mina (abajo-a-izquierda 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_RIGHT_1, u8"Riel de Mina (arriba-a-derecha 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_LEFT_1, u8"Riel de Mina (arriba-a-izquierda 1)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_2, u8"Riel de Mina (horizontal 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_2, u8"Riel de Mina (vertical 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_RIGHT_2, u8"Riel de Mina (abajo-a-derecha 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_DOWN_TO_LEFT_2, u8"Riel de Mina (abajo-a-izquierda 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_RIGHT_2, u8"Riel de Mina (arriba-a-derecha 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_UP_TO_LEFT_2, u8"Riel de Mina (arriba-a-izquierda 2)")
I(CONSOLE_GRID_MINESHAFT_RAIL_HORIZONTAL_3, u8"Riel de Mina (horizontal 3)")
I(CONSOLE_GRID_MINESHAFT_RAIL_VERTICAL_3, u8"Riel de Mina (vertical 3)")
I(CONSOLE_GRID_RAIL_PIT_HORIZONTAL, u8"Pozo de Riel (horizontal)")
I(CONSOLE_GRID_RAIL_PIT_VERTICAL, u8"Pozo de Riel (vertical)")
I(CONSOLE_GRID_RAIL_PIT_DOWN_TO_RIGHT, u8"Pozo de Riel (abajo-a-derecha)")
I(CONSOLE_GRID_RAIL_PIT_DOWN_TO_LEFT, u8"Pozo de Riel (abajo-a-izquierda)")
I(CONSOLE_GRID_RAIL_PIT_UP_TO_RIGHT, u8"Pozo de Riel (arriba-a-derecha)")
I(CONSOLE_GRID_RAIL_PIT_UP_TO_LEFT, u8"Pozo de Riel (arriba-a-izquierda)")
I(CONSOLE_GRID_RAIL_PIT_CROSSROAD, u8"Pozo de Riel (cruce)")
I(CONSOLE_GRID_RAIL_PIT_CART_LEFT, u8"Pozo de Riel (carrito-izquierda)")
I(CONSOLE_GRID_RAIL_PIT_CART_UP, u8"Pozo de Riel (carrito-arriba)")
I(CONSOLE_GRID_RAIL_PIT_CART_RIGHT, u8"Pozo de Riel (carrito-derecha)")
I(CONSOLE_GRID_RAIL_PIT_CART_DOWN, u8"Pozo de Riel (carrito-abajo)")
I(CONSOLE_GRID_TELEPORTER_SQUARE, u8"Teleportador (cuadrado)")
I(CONSOLE_GRID_TELEPORTER_MOON, u8"Teleportador (luna)")
I(CONSOLE_GRID_TELEPORTER_RHOMBUS, u8"Teleportador (rombo)")
I(CONSOLE_GRID_TELEPORTER_M, u8"Teleportador (M)")
I(CONSOLE_GRID_TELEPORTER_PENTAGRAM, u8"Teleportador (pentagrama)")
I(CONSOLE_GRID_TELEPORTER_CROSS, u8"Teleportador (cruz)")
I(CONSOLE_GRID_TELEPORTER_TRIANGLE, u8"Teleportador (triangulo)")
I(CONSOLE_GRID_TRAP_DOOR, u8"Trampilla")
I(CONSOLE_GRID_VOID_PORTAL, u8"Portal de Void")
I(CONSOLE_GRID_CRAWLSPACE, u8"Mazmorra")
I(CONSOLE_GRID_CRAWLSPACE_GIDEON_DUNGEON, u8"Mazmorra (calabozo de Gideon)")
I(CONSOLE_GRID_CRAWLSPACE_SECRET_SHOP, u8"Mazmorra (tienda secreta)")
I(CONSOLE_GRID_CRAWLSPACE_STARTING_ROOM, u8"Mazmorra (sala de inicio)")
I(CONSOLE_GRID_GRAVITY, u8"Gravedad")

//=========== Game Options =============

I(OPT_RESET_BTN_HINT, u8"Restablecer Valores Predeterminados")

I(OPT_OPTIONS, u8"Opciones")
I(OPT_SFX_VOLUME, u8"Volumen SFX")
I(OPT_MUSIC_VOLUME, u8"Volumen Musica")
I(OPT_MAP_OPACITY, u8"Opacidad del Mapa")
I(OPT_FILTER, u8"Filtro")
I(OPT_FILTER_MARK, u8"Aplica un sombreador that makes the game look no longer pixelated. Can cause issues with text rendering on screen!")
I(OPT_POP_UPS, u8"Pop-Ups")
I(OPT_ANNOUNCER_VOICE_MODE, u8" Modo Voz del Anunciador")

I(OPT_HUD_HUDS, u8"HUDs")
I(OPT_HUD_HUD_OFFSET, u8"Offset HUD")
I(OPT_HUD_FOUND_HUD, u8"Found HUD")
I(OPT_HUD_FOUND_HUD_MARKER, u8"Muestra las estadisticas del jugador en el lado izquierdo de la pantalla.")
I(OPT_HUD_CHARGE_BARS, u8"Barras de Carga")
I(OPT_HUD_BOSS_HEALTH_BAR_ON_BOTTOM, u8"Salud de Jefes en el fondo")
I(OPT_HUD_EXTRA_HUD, u8"Extra Hud")
I(OPT_HUD_EXTRA_HUD_MARKER, u8"Muestra los objetos recolectados del jugador en el lado derecho de la pantalla.")

I(OPT_CONSOLE, u8"Consola")
I(OPT_CONSOLE_ENABLE_DEBUG_CONSOLE, u8"Habilitar Consola de depuración")
I(OPT_CONSOLE_FADED_CONSOLE_DISPLAY, "Pantalla de Consola traslucidas")
I(OPT_CONSOLE_SAVE_COMMAND_HISTORY, u8"Save Command History")
I(OPT_CONSOLE_CONSOLE_FONT, u8"Fuente de Consola")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT, u8"Habilitar fuente Unicode")
I(OPT_CONSOLE_ENABLE_UNICODE_FONT_MARK, u8"Para mejorar el soporte de lenguajes no-latin. Incrementa de forma menor el uso de memoria.")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE, u8"Modo de Renderizado Unifont")
I(OPT_CONSOLE_UNIFONT_RENDER_MODE_MARK, u8"Cambia el tamaño de fuente para hacer la visualización más perfecta. Requiere reiniciar.")

I(OPT_VIS, u8"Visuales")
I(OPT_VIS_GAMMA, u8"Gamma")
I(OPT_VIS_EXPOSURE, u8"Exposicion")
I(OPT_VIS_BRIGHTNESS, u8"Brillo")
I(OPT_VIS_CONTRAST, u8"Contraste")
I(OPT_VIS_MAX_SCALE, u8"Escala Maxima")
I(OPT_VIS_MAX_RENDER_SCALE, u8"Escala de Renderizado Maxima")
I(OPT_VIS_ACTIVE_CAMERA, u8"Camara Activa")
I(OPT_VIS_VSYNC, u8"VSync")
I(OPT_VIS_BORDERLESS_FULLSCREEN, u8"Pantalla Completa Sin Bordes")
I(OPT_VIS_EFFECTS, u8"Efectos")
I(OPT_VIS_EFFECTS_MARK, u8"Estos ajustes representan los valores leídos por el juego desde el archivo config.ini, no son guardados en ningun lugar y no deberían ser cambiados por el jugador!")
I(OPT_VIS_ANIM_INTERP, u8"Fotogramas Interpolados")
I(OPT_VIS_ANIM_INTERP_MARK, u8"Esta opción controla si los fotogramas \"intermedios\" son habilitados. (La logica del juego corre a 30 ticks por segundo al contrario del objetivo de fotogramas de 60FPS, la interpolacion es lo que le permite aparentar ser \"suave\".)")
I(OPT_VIS_COLOR_CORRECTION, u8"Correccion de Color")
I(OPT_VIS_COLOR_MODIFIER, u8"Modificador de Color")
I(OPT_VIS_LIGHTING, u8"Iluminación")
I(OPT_VIS_SHOCKWAVES, u8"Ondas de Choque")
I(OPT_VIS_CAUSTICS, u8"Cáustica")
I(OPT_VIS_PIXELATION, u8"Pixelación")
I(OPT_VIS_PIXELATION_MARK, u8"e.j. pildora de Retro vision")
I(OPT_VIS_BLOOM, u8"Efecto Bloom")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING, u8"Activar Renderizado de Superficie del Agua")
I(OPT_VIS_ENABLE_WATER_SURFACE_RENDERING_MARK, u8"Necesita establecerse al cambiar el ajuste de Superficie del Agua")
I(OPT_VIS_WATER_SURFACE, u8"Superficie del Agua")

I(OPT_ACC, u8"Accesibilidad")
I(OPT_ACC_AIM_LOCK, u8"Bloquear Puntería")
I(OPT_ACC_RUMBLE, u8"Vibración")
I(OPT_ACC_MOUSE_CONTROL, u8"Control de Ratón")
I(OPT_ACC_BULLET_VISIBILITY, u8"Visibilidad de Balas")
I(OPT_ACC_TOUCH_MODE, u8"Modo Táctil")

I(OPT_MISC, u8"Misc")
I(OPT_MISC_ENABLE_MODS, u8"Activar Mods")
I(OPT_MISC_ENABLE_CTRL_HOTPLUG, "Habilitar Conexión en Caliente(Hotplug)")
I(OPT_MISC_USE_STEAM_CLOUD, u8"Usar Steam Cloud")
I(OPT_MISC_PAUSE_ON_FOCUS_LOST, u8"Pausar al desenfocar la ventana")

I(OPT_REPENTOGON, u8"REPENTOGON")
I(OPT_REPENTOGON_BETTER_VOID_GEN, u8"Mejor Generación de Void")
I(OPT_REPENTOGON_BETTER_VOID_GEN_MARK, u8"The Void ahora elige habitaciónes de todos los pisos desbloqueados, incluyendo las rutas alternas.")
I(OPT_REPENTOGON_HUSH_LASER_FIX, u8"Solución velocidad de láseres de Hush")
I(OPT_REPENTOGON_HUSH_LASER_FIX_MARK, u8"Corrige un error en vanilla con Hush que que provoca que los láseres se muevan más rápido de lo previsto.")
I(OPT_REPENTOGON_STAT_HUD_PLANETARIUM, u8"Probabilidad de Planetario en HUD de Estadisticas")
I(OPT_REPENTOGON_STAT_HUD_PLANETARIUM_MARK, u8"Añade la probabilidad de generacion de Planetarios en la HUD de estadisticas. No tendrá efecto alguno si los Planetarios no han sido desbloqueados.")
I(OPT_REPENTOGON_SKIP_INTRO, u8"Saltear Escena Intro")
I(OPT_REPENTOGON_SKIP_INTRO_MARK, u8"Evita que se reproduzca la introducción, no mas \"Isaac and his mother\".")
I(OPT_REPENTOGON_INTERPOLV2, u8"Interpolación a 60FPS")
I(OPT_REPENTOGON_INTERPOLV2_MARK, u8"Interpola los fotogramas de animación durante el renderizado, resultando en animaciones exageradamente suaves y fluidas. Lamento haber liberado esto al mundo.")
I(OPT_REPENTOGON_FAST_LASERS, u8"Láseres Rápidos (menor precisión)")
I(OPT_REPENTOGON_FAST_LASERS_MARK, u8"Mejora el rendimiento del juego sacrificando la precisión de láseres, puede alterár ligeramente el área de impacto (Esta opción reduce el número de puntos de muestreo de los láseres).")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR, u8"Limpiar Habitaciónes Rapido")
I(OPT_REPENTOGON_QUICKER_ROOM_CLEAR_MARK, u8"Las Habitaciónes abrirán sus puertas y generarán recompenzas más rapido. Los jefes aún requieren que sus animaciónes de muerte finalicen.")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES, u8"Prevenir actualizar mods")
I(OPT_REPENTOGON_PREVENT_MOD_UPDATES_MARK, u8"El juego se salteará la sincronización de mods de la workshop. Muy util para prevenir cambios en la carpeta de mods al desarrollar mods.")
I(OPT_REPENTOGON_MARSDOUBLETAP, u8"Ventana de Doble-Tap para Mars")
I(OPT_REPENTOGON_MARSDOUBLETAP_MARK, u8"Establece la ventana de tiempo para detectar un doble-tap en el movimiento para embestir con Mars.")
I(OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT, u8"Limite de Comandos de Autocompletado")
I(OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT_MARK, u8"Controla el limite de cantidad de lineas mostradas en la Consola de Depuracion al escribir un comando. Incrementar esto a un numero mayor puede disminuir el rendimiento al escribir en la consola.")

I(OPT_REPENTOGON_FILE_MAP, u8"Habilitar generación de mapas de archivos modificados")
I(OPT_REPENTOGON_FILE_MAP_MARK, u8"Comprueba Preemptively todos los archivos modificados para un arranque mas rapido del juego y menos bajones. Deshabilitalo y reportalo a nosotros si encuentras problemas al cargar assets modificados.")

I(OPT_REPENTOGON_ECO_MODE, u8"Limitar juego mientras esté minimizado")
I(OPT_REPENTOGON_ECO_MODE_MARK, u8"Previene que el juego se renderice y aplica una optimización de energía mientras se encuentre minimizado. Puede afectar a mods que dependen en la renderización sucediendo en cada fotograma.")

I(OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS, u8"Renderizar Depuración Extra para FindInRadius")
I(OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS_MARK, u8"Mientras el indicador DebugFlag.HITSPHERES esté habilitado, se dibujaran esferas de depuración para llamadas de FindInRadius/QueryRadius.")

I(OPT_EXTRA_HUD_MODES_OFF, u8"Apagado")
I(OPT_EXTRA_HUD_MODES_NORMAL, u8"Normal")
I(OPT_EXTRA_HUD_MODES_MINI, u8"Mini")

I(OPT_ANNOUNCER_MODES_RANDOM, u8"Aleatorio")
I(OPT_ANNOUNCER_MODES_OFF, u8"Apagado")
I(OPT_ANNOUNCER_MODES_ALWAYS_ON, u8"Always on")

I(OPT_POPUP_MODES_OFF, u8"Apagado")
I(OPT_POPUP_MODES_BIG, u8"GRANDE")
I(OPT_POPUP_MODES_SMALL, u8"Pequeño")

I(OPT_CONSOLE_FOND_MODES_DEFAULT, u8"Por Defecto")
I(OPT_CONSOLE_FOND_MODES_SMALL, u8"Pequeño")
I(OPT_CONSOLE_FOND_MODES_TINY, u8"Diminuto")

I(OPT_OFF_ON_MODES_OFF, u8"Apagado")
I(OPT_OFF_ON_MODES_ON, u8"Encendido")

I(OPT_UNIFONT_RENDER_MODE_NORMAL, u8"Normal: 13px, Solo caractéres non-latin")
I(OPT_UNIFONT_RENDER_MODE_LARGE, u8"GrandePerfecto: 16px")
I(OPT_UNIFONT_RENDER_MODE_MEDIUM, u8"Medio: 14px")
I(OPT_UNIFONT_RENDER_MODE_TINY, u8"DiminutoPerfecto: 16px a escala 0.5")
I(OPT_UNIFONT_RENDER_MODE_TINY_LOW, u8"DiminutoBajo: 8px")

//============== Help Menu ===============

// Egg Game
I(HELP_EGG_PRESENT, u8"Un Regalo!")
I(HELP_EGG_DELI_COOKIE, u8"Una galleta deliciosa. le das una mordida.")
I(HELP_EGG_GREAT_COOKIE, u8"La galleta sabe genial!")
I(HELP_EGG_EATEN_COOKIE, u8"Te has comido la galleta.")

I(HELP_EGG_ANOTHER_GIFT, u8"Hay otro regalo...")
I(HELP_EGG_WEIRED, u8"Tomas el otro regalo! Huele raro...")
I(HELP_EGG_PIECE_OF_SHIT, u8"Es un pedazo de mierda ordinaria...\nNo parece muy nutritiva...")

I(HELP_EGG_TOILET, u8"Tiraste la mierda por el inodoro!")
I(HELP_EGG_PRESENT_BEHIND_TOILET, u8"Miras detras del inodoro y encuentras un regalo que parece elegante.")
I(HELP_EGG_ANCIENT_EGG, u8"Un huevo de aspecto antiguo. Se siente tibio...")
I(HELP_EGG_DRAGON_FROM_EGG, u8"Un dragón nació del huevo!")
I(HELP_EGG_HUNGRY_DRAGON, u8"Tú dragón se ve hambriento y necesitado.")
I(HELP_EGG_FEED_DRAGON, u8"Tratas de alimentar al dragón...")
I(HELP_EGG_LEMON, u8"Le das de comer al dragón un limón.\nËl te mira confundido...")
I(HELP_EGG_CARROT, u8"Le das de comer al dragón una zanahoria.\nNo parece gustarle...")
I(HELP_EGG_BACON, u8"Le das de comer al dragón álgo de tocino.\nSe lo come deleitado!")
I(HELP_EGG_ATTENTION, u8"Tú dragón quiere atencion!")
I(HELP_EGG_PET_AND_LOVE, u8"Acaricias mucho al dragón y le das mucho amor.")
I(HELP_EGG_DRAGON_HAPPY, u8"Tú dragón se ve contento!")

I(HELP_EGG_RIDEABLE, u8"Empiezas a preguntarte si el dragón se puede montar.")
I(HELP_EGG_TAKE_OFF, u8"Tú y tu dragón despegan hacia el cielo!")
I(HELP_EGG_INSEPARABLE, u8"Sientes como si tú y tu compañero dragón fuesen inseparables.")
I(HELP_EGG_REACH_THE_HEAVEN, u8"Sigues ascendiendo!\Has alcanzado los cielos.")
I(HELP_EGG_GOD_SMITES_DRAGON, u8"Desafortunadamente, A DIOS no le interesan tus payasadas.\nÉl derriba tu dragón en el acto, mientras deciendes hacia las desconocidas profundidades que se encuentran debajo.")
I(HELP_EGG_YOU_SURVIVE, u8"Sobrevives a la caida, pero tu dragon muere en el impacto..")

I(HELP_EGG_REVENGE, u8"Tendras tu venganza.\nPero cómo?")
I(HELP_EGG_REBUILD, u8"Podemos reconstruirlo.")
I(HELP_EGG_STRONGER, u8"podemos hacerlo mas fuerte.")
I(HELP_EGG_REBORN_AS_GUN, u8"Tu dragon ha renacido como un arma!")

I(HELP_EGG_RIDEABLE_AGAIN, u8"Empiezas a preguntarte si tu arma dragón legalmente distinta se puede montar.")
I(HELP_EGG_TAKE_OFF_AGAIN, u8"Tú y tu referencia descarada despegan hacia el cielo!")
I(HELP_EGG_INSEPARABLE_AGAIN, u8"Te caes como si tú y tu referencia al juego de referencia fueran inseparables.")
I(HELP_EGG_ASCENDING_1, u8"Sigues ascendiendo!")
I(HELP_EGG_ASCENDING_2, u8"Sigues ascendiendo!")
I(HELP_EGG_ASCENDING_3, u8"Sigues ascendiendo!")
I(HELP_EGG_ASCENDING, u8"Sigues ascendiendo...")

I(HELP_EGG_SINS_CRAWLING, u8"Sientes tus pecados trepar por tu espalda.")
I(HELP_EGG_NO_HEAVEN, u8"Qué tonto eres, porque no hay ningun cielo.")
I(HELP_EGG_NO_DRAGON, u8"No hay ningun dragón.")
I(HELP_EGG_NOTHING, u8"No hay nada.")

I(HELP_EGG_HOWDY, u8"Buenas! Soy FLOWEY. FLOWEY la FLOR!")
I(HELP_EGG_I_OWE_YOU, u8"I owe you a HUGE thanks! You really did a number on that old fool!\nWithout you, I NEVER could have gotten past him!")
I(HELP_EGG_WITH_YOUR_HELP, u8"Pero ahora, con TU ayuda...")
I(HELP_EGG_HE_DEAD, u8"Está MUERTO.")
I(HELP_EGG_GOT_HUMAN_SOUL, u8"Y YO tengo las ALMAS humanas!")
I(HELP_EGG_SOUL_INSIDE_ME_AGAIN, u8"Uff! He estado vacio por tanto tiempo...\nSe siente genial tener un ALMA dentro de mi otra vez.")
I(HELP_EGG_FELL_WRIGGLING, u8"Mmmm, Puedo sentirlas retorciéndose...")
I(HELP_EGG_FEELING_LEFT_OUT, u8"Awww, te sientes excluido, no es asi?")
I(HELP_EGG_NEED_ONE_MORE_SOUL, u8"Bueno, eso es maravilloso. Despues de todo, solo tengo seis almas.\nAún necesito una más...")
I(HELP_EGG_BEFORE_BECOME_GOD, u8"Para convertirme en DIOS.")
I(HELP_EGG_POWER, u8"Y entonces, con mis nuevos poderes...")
I(HELP_EGG_MONSTER, u8"Monstruos...")
I(HELP_EGG_HUMAN, u8"Humanos...")
I(HELP_EGG_EVERYONE, u8"Todo el mundo...")
I(HELP_EGG_SHOW_MEANING, u8"Les mostraré el VERDADERO significado de este mundo!")
I(HELP_EGG_STILL_GUN, u8"Puede que ya no haya un dragón, pero aún hay un arma.\nSacas tu arma y le disparas a Flowey, matándolo instantáneamente.")
I(HELP_EGG_POWER_OF_REPENTOGON, u8"El dia fue salvado! Ese es el poder de REPENTOGON.")

// Controls

I(HELP_CTRL_PS_DPAD, u8"D-PAD")
I(HELP_CTRL_PS_DPAD_DESC, u8"Mover, Ajustar valores (cuando la entrada está activa)")
I(HELP_CTRL_PS_TOUCH, u8"Panel Táctil")
I(HELP_CTRL_PS_TOUCH_DESC, u8"Raton")
I(HELP_CTRL_PS_LSTICK, u8"Stick Izq")
I(HELP_CTRL_PS_LSTICK_DESC, u8"Desplazar")
I(HELP_CTRL_PS_SQUARE, u8"Cuadrado")
I(HELP_CTRL_PS_SQUARE_DESC, u8"Tap: Cambiar Menu\nMantener+L1/R1: Cambiar foco de Ventana\nMantener+D-Pad: Ajustar Ventana\nMantener+Stick-Izq: Mover Ventana")
I(HELP_CTRL_PS_TRIANGLE, u8"Triangulo")
I(HELP_CTRL_PS_TRIANGLE_DESC, u8"Editar Texto / Teclado en pantalla")
I(HELP_CTRL_PS_CIRCLE, u8"Circulo")
I(HELP_CTRL_PS_CIRCLE_DESC, u8"Cancelar / Cerrar / Salir")
I(HELP_CTRL_PS_CROSS, u8"Cruz")
I(HELP_CTRL_PS_CROSS_DESC, u8"Activar / Abrir / Cambiar\nAjustar valor con el D-Pad\n(+L1/R1 para ajustar más lento/rapido)")

I(HELP_CTRL_XB_DPAD, u8"D-PAD")
I(HELP_CTRL_XB_DPAD_DESC, u8"Mover, Ajustar valores (cuando la entrada está activa)")
I(HELP_CTRL_XB_STICK, u8"Stick Izq")
I(HELP_CTRL_XB_STICK_DESC, u8"Desplazar")
I(HELP_CTRL_XB_X, u8"X")
I(HELP_CTRL_XB_X_DESC, u8"Tap: Cambiar Menu\nMantener+L1/R1: Change Ventana focus\nMantener+D-Pad: Ajustar Ventana\nMantener+Stick-Izq: Mover Ventana")
I(HELP_CTRL_XB_Y, u8"Y")
I(HELP_CTRL_XB_Y_DESC, u8"Editar Texto / Teclado en pantalla")
I(HELP_CTRL_XB_B, u8"B")
I(HELP_CTRL_XB_B_DESC, u8"Cancelar / Cerrar / Salir")
I(HELP_CTRL_XB_A, u8"A")
I(HELP_CTRL_XB_A_DESC, u8"Activar / Abrir / Cambiar\nAjustar valor con el D-Pad\n(+L1/R1 para ajustar más lento/rapido)")

I(HELP_CTRL_SWITCH_DPAD, u8"D-PAD")
I(HELP_CTRL_SWITCH_DPAD_DESC, u8"Mover, Ajustar valores (cuando la entrada está activa)")
I(HELP_CTRL_SWITCH_LSTICK, u8"Stick Izq")
I(HELP_CTRL_SWITCH_LSTICK_DESC, u8"Desplazar")
I(HELP_CTRL_SWITCH_Y, u8"Y")
I(HELP_CTRL_SWITCH_Y_DESC, u8"Tap: Cambiar Menu\nMantener+L1/R1: Change Ventana focus\nMantener+D-Pad: Ajustar Ventana\nMantener+Stick-Izq: Mover Ventana")
I(HELP_CTRL_SWITCH_X, u8"X")
I(HELP_CTRL_SWITCH_X_DESC, u8"Editar Texto / Teclado en pantalla")
I(HELP_CTRL_SWITCH_B, u8"B")
I(HELP_CTRL_SWITCH_B_DESC, u8"Cancelar / Cerrar / Salir")
I(HELP_CTRL_SWITCH_A, u8"A")
I(HELP_CTRL_SWITCH_A_DESC, u8"Activar / Abrir / Cambiar\nAjustar valor con el D-Pad\n(+L1/R1 para ajustar más lento/rapido)")

// Help Menu
I(HELP_MENU, ICON_U8_FA_CIRCLE_QUESTION u8" Ayuda")
I(HELP_MENU_CTRLS, ICON_U8_FA_GAMEPAD u8" Controles")
I(HELP_MENU_CTRL_MOUSE, u8"Ratón")
I(HELP_MENU_CTRL_MOUSE_HINT, u8"Escalar texto: Rueda del Ratón + CTRL pasando el cursor sobre el texto o ventana que deseas escalar")

I(HELP_MENU_CTRL_KEYBOARD, u8"Teclado")
I(HELP_MENU_CTRL_CONTROLLER, u8"Controlador")

I(HELP_MENU_CTRL_PLAYSTATION, u8"Playstation")
I(HELP_MENU_CTRL_PLAYSTATION_TEXT, u8"Controles para controladores DualShock4")

I(HELP_MENU_CTRL_XBOX, u8"XBox")
I(HELP_MENU_CTRL_XBOX_TEXT, u8"Controles para controladores XBox")

I(HELP_MENU_CTRL_SWITCH, u8"Switch")
I(HELP_MENU_CTRL_SWITCH_TEXT, u8"Controles para controladores Joy-Con")

I(HELP_MENU_DOC, ICON_U8_FA_BOOK u8" Documentación")
I(HELP_MENU_DOC_TEXT, u8"Puede encontrar la documentación aquí: ")

I(HELP_MENU_TRANSLATE, ICON_U8_FA_GLOBE u8" Traducir información")

// ============ ImGui ===============

I(IMGUI_WIN_CTX_MENU_PIN_WINDOW, u8"Fijár Ventana")
I(IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC, u8"Fijár una Ventana la mantendrá visible aún tras cerrar las Herramiéntas Dev.")

I(IMGUI_CHANGE_KEY_BTN_NAME, u8"Cambiar")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_CTRL, u8"Presione un botón en tu control.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_KEY_KEYBOARD, u8"Presione una tecla en tu teclado.")
I(IMGUI_CHANGE_KEY_BTN_PRESS_ESC, u8"Presione ESC para cancelar")

// ============= Log Viewer ==============

I(LOGV_WIN_NAME, u8"Visor de Registro")
I(LOGV_FILTER, u8"Filtrar")
I(LOGV_PIN_WINDOW, u8"Fijár Ventana")
I(LOGV_PIN_WINDOW_HINT, u8"Fijár una Ventana la mantendrá visible aún tras cerrar las Herramiéntas Dev.")
I(LOGV_AUTOSCROLL, u8"Desplazamiento automático")
I(LOGV_SHOW_CATEGORIES, u8"Mostrar categorías:")

// ============ Performance Window ==========

I(PERF_WIN_NAME, u8"Rendimiento")
I(PERF_TIMEFRAME_NAME, u8"Periodo de tiempo")
I(PERF_TIMEFRAME_FORMAT, u8"%.1f Segundos")
I(PERF_LUA_MEM_USE, u8"Uso Mem Lua")

// =========== Translate Report ============

I(TRANS_LANG_JAPANESE, u8"Japones")
I(TRANS_LANG_KOREAN, u8"Koreano")
I(TRANS_LANG_CHINESE_SIMPLE, u8"Chino(Simplificado)")
I(TRANS_LANG_RUSSIAN, u8"Ruso")
I(TRANS_LANG_GERMAN, u8"Aleman")
I(TRANS_LANG_SPANISH, u8"Español")
I(TRANS_LANG_FRENCH, u8"Francés")

I(TRANS_REPORT_WIN_NAME, u8"Reportar Traducción")
I(TRANS_LANGUAGE_HINT, u8"Traducír info para el lenguaje: %s")
I(TRANS_UNTRANS_ITEMS, u8"Objetos sin Traducír:")
I(TRANS_COUNT_HINT, u8"Conteo total: %d Traducidos: %d")