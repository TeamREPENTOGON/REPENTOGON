cmake_minimum_required(VERSION 3.13.0)

set ("CMAKE_CXX_STANDARD" 17)
set (ISAAC_DIRECTORY "" CACHE PATH "Path to the root directory of the game")
set (CMAKE_BUILD_TYPE "Debug")
set (ZHL_LOG_FILE "" CACHE PATH "Path to the file in which logs will be written")

project(RepExtender)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include "${CMAKE_SOURCE_DIR}/libs/lua" 
    "${CMAKE_SOURCE_DIR}/libs/rapidxml" "${CMAKE_SOURCE_DIR}/libs/rapidjson/include/rapidjson" 
    "${CMAKE_SOURCE_DIR}/libs/LuaBridge/Source/LuaBridge" 
    "${CMAKE_SOURCE_DIR}/libzhl" "${CMAKE_SOURCE_DIR}/libs/imgui" 
    "${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype" "${CMAKE_SOURCE_DIR}/libs/imgui/backends" 
    "${CMAKE_SOURCE_DIR}/libs/NaturalSort" "${CMAKE_SOURCE_DIR}/libs/glad/include" "${CMAKE_SOURCE_DIR}/libs/mINI/src/mini")

# Zydis
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
option(ZYDIS_BUILD_DOXYGEN "" OFF)
add_subdirectory("libs/zydis")
target_compile_options("Zydis" PUBLIC "/MD")

# ANTLR
option(WITH_STATIC_CRT "" OFF)
option(ANTLR_BUILD_CPP_TESTS "" OFF)
option(ANTLR_BUILD_STATIC "" OFF)
add_subdirectory("libs/antlr4/runtime/Cpp")
include_directories ("libs/antlr4/runtime/Cpp/runtime/src")

# libcurl
option(BUILD_TESTING "" OFF)
option(BUILD_CURL_EXE "" OFF)
option(BUILD_SHARED_LIBS "" OFF)
option(BUILD_STATIC_LIBS "" ON)
option(ENABLE_THREADED_RESOLVER "" OFF)
option(CURL_USE_SCHANNEL "" ON)
# Disable everything except HTTPS, we don't need to compile most of this.
option(CURL_DISABLE_DICT "" ON)
option(CURL_DISABLE_FILE "" ON)
option(CURL_DISABLE_FTP "" ON)
option(CURL_DISABLE_GOPHER "" ON)
option(CURL_DISABLE_IMAP "" ON)
option(CURL_DISABLE_LDAP "" ON)
option(CURL_DISABLE_LDAPS "" ON)
option(CURL_DISABLE_MQTT "" ON)
option(CURL_DISABLE_POP3 "" ON)
option(CURL_DISABLE_RTSP "" ON)
option(CURL_DISABLE_SMB "" ON)
option(CURL_DISABLE_SMTP "" ON)
option(CURL_DISABLE_TELNET "" ON)
option(CURL_DISABLE_TFTP "" ON)
add_subdirectory("libs/curl")

# Crypto++
option(CRYPTOPP_BUILD_TESTING "" OFF)
add_subdirectory("libs/cryptopp-cmake")
target_compile_options(cryptopp PUBLIC "/MD")

# Parser
FILE(GLOB PARSER_SRC ${CMAKE_SOURCE_DIR}/libzhlgen/*.cpp)
add_executable(zhlparser ${PARSER_SRC})
target_link_libraries(zhlparser antlr4_shared)
target_compile_options(zhlparser PUBLIC "/wd4251" "/wd4275")
# wd4251 ignore warning C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
# wd4275 ignore warning C4275: non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
target_compile_options(antlr4_shared PUBLIC "/wd4100" "/wd4244" "/wd4251" "/wd4275" "/wd4996")
# ignore all warnings from that lib. We are not the author, so we dont care about them

add_custom_command(TARGET zhlparser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:antlr4_shared>/antlr4-runtime.dll" "$<TARGET_FILE_DIR:zhlparser>")

# Lua
FILE(GLOB LUA_SRC ${CMAKE_SOURCE_DIR}/libs/lua/*.c)
list(REMOVE_ITEM LUA_SRC "${CMAKE_SOURCE_DIR}/libs/lua/onelua.c")
add_compile_definitions(LUA_BUILD_AS_DLL)
add_library(Lua5.4 SHARED ${LUA_SRC})

# IsaacRepentance.h/.cpp
FILE(GLOB ZHL_FILES ${CMAKE_SOURCE_DIR}/libzhl/functions/*.zhl)
LIST(APPEND ZHL_FILES ${CMAKE_SOURCE_DIR}/libzhl/functions/ExtraTypes)
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/libzhl/IsaacRepentance.cpp COMMAND zhlparser WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libzhlgen" DEPENDS ${ZHL_FILES} zhlparser)

# ZHL
FILE(GLOB ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/*.cpp ${CMAKE_SOURCE_DIR}/libzhl/*.h)
LIST(APPEND ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/IsaacRepentance.cpp ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
add_compile_definitions(GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
add_library(libzhl SHARED ${ZHL_SRC})
target_compile_options(libzhl PUBLIC "/MD" "/wd4251" "/wd4996")
# wd4251 ignore warning C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
# wd4996 ignores warning C4996: using a depricated function

# ZHL Launcher
add_library (zhlLauncher SHARED ${CMAKE_SOURCE_DIR}/launcher/launcher.cpp)

FILE(GLOB LAUNCHER_CORE_SRC ${CMAKE_SOURCE_DIR}/launcher_core/*.cpp)
add_library(launcher_core STATIC ${LAUNCHER_CORE_SRC})
target_compile_options(launcher_core PUBLIC "/MD")

# Injected DLL
set(UPDATER_BASE_BUILD_DIR "${CMAKE_SOURCE_DIR}/updater_rsrc")
configure_file(${CMAKE_SOURCE_DIR}/updater/updater.rc.in ${CMAKE_SOURCE_DIR}/updater/updater.rc)
FILE(GLOB LAUNCHER_SRC ${CMAKE_SOURCE_DIR}/launcher/*.cpp ${CMAKE_SOURCE_DIR}/updater/updater.rc)
add_library(dsound SHARED ${LAUNCHER_SRC})
target_include_directories(dsound PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_include_directories(dsound PUBLIC ${CURL_INCLUDE_DIR})
# target_link_libraries(dsound libcurl cryptopp)
target_compile_options(dsound PUBLIC "/MD" "/wd4996")
target_link_libraries(dsound Imagehlp launcher_core)

# Embedded Updater

add_executable(UpdaterBase ${CMAKE_SOURCE_DIR}/updater/Updater.cpp)
target_include_directories(UpdaterBase PRIVATE ${CURL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(UpdaterBase PRIVATE libcurl cryptopp launcher_core)
target_compile_options(UpdaterBase PUBLIC "/MD")
add_custom_command (TARGET UpdaterBase POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:UpdaterBase> "${CMAKE_SOURCE_DIR}/updater_rsrc")

add_dependencies(dsound UpdaterBase)

# ImGUI
enable_language(RC)
set(FREETYPE_SRC
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/autofit/autofit.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftbase.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftbbox.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftbdf.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftbitmap.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftcid.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftfstype.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftgasp.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftglyph.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftgxval.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftinit.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftmm.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftotval.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftpatent.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftpfr.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftstroke.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftsynth.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/fttype1.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftwinfnt.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/bdf/bdf.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/bzip2/ftbzip2.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/cache/ftcache.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/cff/cff.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/cid/type1cid.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/gzip/ftgzip.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/lzw/ftlzw.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/pcf/pcf.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/pfr/pfr.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/psaux/psaux.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/pshinter/pshinter.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/psnames/psnames.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/raster/raster.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/sdf/sdf.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/sfnt/sfnt.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/smooth/smooth.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/svg/svg.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/truetype/truetype.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/type1/type1.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/type42/type42.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/winfonts/winfnt.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/builds/windows/ftsystem.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/builds/windows/ftdebug.c
  ${CMAKE_SOURCE_DIR}/libs/freetype/src/base/ftver.rc
)
add_library(freetype SHARED ${FREETYPE_SRC} ${WinSDK})
target_include_directories(freetype PUBLIC "${CMAKE_SOURCE_DIR}/libs/freetype/include")
target_compile_options(freetype PUBLIC "/MD")
set_target_properties(freetype PROPERTIES C_VISIBILITY_PRESET hidden)
target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY PRIVATE DLL_EXPORT _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)

FILE(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/libs/imgui/*.cpp)
FILE(GLOB IMGUI_BACKENDS_DIR ${CMAKE_SOURCE_DIR}/libs/imgui/backends/)
set(IMGUI_BACKENDS_DIR ${CMAKE_SOURCE_DIR}/libs/imgui/backends/)
add_library(imgui ${IMGUI_SRC} ${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype/imgui_freetype.h ${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype/imgui_freetype.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.h ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3_loader.h ${IMGUI_BACKENDS_DIR}/imgui_impl_win32.h ${IMGUI_BACKENDS_DIR}/imgui_impl_win32.cpp)
target_include_directories(imgui PUBLIC "${CMAKE_SOURCE_DIR}/libs/freetype/include")
target_compile_options(imgui PUBLIC "/MD")
target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE)

# Repentogon
file(GLOB_RECURSE REPENTOGON_SRC ${CMAKE_SOURCE_DIR}/repentogon/*.*)
# Display folder structure inside of VS solution - https://stackoverflow.com/a/54531576
foreach(_source IN ITEMS ${REPENTOGON_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
add_library(zhlREPENTOGON SHARED ${REPENTOGON_SRC} )
target_compile_options(zhlREPENTOGON PUBLIC "/MD" "/MP" "/wd4251" "/wd4996")
target_compile_definitions(zhlREPENTOGON PRIVATE NOMINMAX)

# Delirium
FILE(GLOB_RECURSE DELIRIUM_SRC ${CMAKE_SOURCE_DIR}/delirium/*.*)
foreach(_source IN ITEMS ${DELIRIUM_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
add_library(zhlDelirium SHARED ${DELIRIUM_SRC})
target_link_libraries(zhlDelirium libzhl)
target_compile_options(zhlDelirium PUBLIC "/MD")

add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources" "$<TARGET_FILE_DIR:zhlREPENTOGON>/resources")
add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources-repentogon" "$<TARGET_FILE_DIR:zhlREPENTOGON>/resources-repentogon")
# add_custom_command(TARGET zhlDelirium POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/delirium/resources-delirium" "$<TARGET_FILE_DIR:zhlDelirium>/resources-delirium")

if(NOT ISAAC_DIRECTORY STREQUAL "")
    message (STATUS "Files will be installed to " ${ISAAC_DIRECTORY})
    add_custom_command(TARGET Lua5.4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:dsound>/Lua5.4.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET dsound POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:dsound>/dsound.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET libzhl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:libzhl>/libzhl.dll" "${ISAAC_DIRECTORY}")
	add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:freetype>/freetype.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlREPENTOGON>/zhlREPENTOGON.dll" "${ISAAC_DIRECTORY}")
	add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources" "${ISAAC_DIRECTORY}/resources")
    add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources-repentogon" "${ISAAC_DIRECTORY}/resources-repentogon")
    add_custom_command(TARGET zhlDelirium POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlDelirium>/zhlDelirium.dll" "${ISAAC_DIRECTORY}")
    # add_custom_command(TARGET zhlDelirium POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/delirium/resources-delirium" "${ISAAC_DIRECTORY}/resources-delirium")
endif()

if (NOT ZHL_LOG_FILE STREQUAL "")
    add_compile_definitions(ZHL_LOG_FILE="${ZHL_LOG_FILE}")
endif()

add_dependencies(zhlREPENTOGON libzhl)

# STB parser
add_library(stb SHARED stbParser/stbParser.cpp)
add_executable(stbc stbParser/stbParserc.cpp)
target_link_libraries(stbc stb)
target_link_libraries(zhlREPENTOGON Lua5.4 libzhl freetype "Zydis" imgui ${OPENGL_LIBRARIES})
target_link_libraries(libzhl Lua5.4 "Zydis" dbghelp Imagehlp)

# Put libraries into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(freetype antlr4_shared imgui Lua5.4 stb stbc Zydis Zycore cryptopp libcurl_object libcurl_static curl_uninstall PROPERTIES FOLDER "External Libs")
set_target_properties(libzhl zhlparser PROPERTIES FOLDER "libzhl")
set_target_properties(dsound zhlDelirium zhlREPENTOGON PROPERTIES FOLDER "Repentogon")
