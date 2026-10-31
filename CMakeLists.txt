cmake_minimum_required(VERSION 3.13.0)

function (linux_to_wine_path path result)
    execute_process(COMMAND winepath -w "${path}" OUTPUT_VARIABLE TRANSLATED_PATH)
    string(REPLACE "\n" "" TRANSLATED_PATH_NO_NL "${TRANSLATED_PATH}")
    message (STATUS "linux_to_wine_path: path = ${path}, path_slash = ${TRANSLATED_PATH_NO_NL}")
    set (${result} "${TRANSLATED_PATH_NO_NL}" PARENT_SCOPE)
endfunction ()

if (CMAKE_SYSTEM_NAME MATCHES Windows)
  if (CMAKE_HOST_SYSTEM_NAME MATCHES Linux)
    set (CLANG_CL_XCC 1)
    message (STATUS "Cross-compiling from Linux to Windows (assuming clang-cl)")
    set (WINE_RUNNER "wine")
    set (ZHLPARSER_EXEC "zhlparser.exe")
    set (NAKED_ASM_GEN_EXEC "super_asm_generator.exe")
    set (ZHLGEN_ASM_FILE "${CMAKE_CURRENT_BINARY_DIR}/IsaacRepentance.asm")
    # linux_to_wine_path("${ZHLGEN_ASM_FILE}" ZHLGEN_ASM_FILE_WINE_PATH)
    set (ZHLGEN_ASM "--output-asm=\"${ZHLGEN_ASM_FILE}\"")
    set (ZHLGEN_LIB_FILE "${CMAKE_CURRENT_BINARY_DIR}/IsaacRepentance_stub.cpp")
    # linux_to_wine_path("${ZHLGEN_LIB_FILE}" ZHLGEN_LIB_FILE_WINE_PATH)
    set (ZHLGEN_LIB_FLAG "--output-gen-lib=\"${ZHLGEN_LIB_FILE}\"")
    set (ZHLGEN_STUB_LIBRARY_OBJ "${CMAKE_CURRENT_BINARY_DIR}/IsaacRepentance_stub.obj")
    # linux_to_wine_path("${CMAKE_SOURCE_DIR}/libs/rapidxml" RAPIDXML_WINE_PATH)
    # linux_to_wine_path("${CMAKE_SOURCE_DIR}/libs/lua" LUA_WINE_PATH)
    # linux_to_wine_path("${CMAKE_SOURCE_DIR}/include" ZHLGEN_INCLUDE_DIR_WINE_PATH)
    # linux_to_wine_path("${ZHLGEN_STUB_LIBRARY_OBJ}" ZHLGEN_STUB_LIBRARY_OBJ_WINE_PATH)
    # Add libzhl to the include path as it it where we generate the header of
    # the implementation lib. This avoids the need to convert the path from
    # Linux to Wine.
    linux_to_wine_path("${ZHLGEN_LIB_FILE}" ZHLGEN_LIB_FILE_WINE)
    linux_to_wine_path("${ZHLGEN_STUB_LIBRARY_OBJ}" ZHLGEN_STUB_LIBRARY_OBJ_WINE)
    set (ZHLGEN_LIB_CMD "--gen-lib-cmd=\"/LD /std:c++17 /I${CMAKE_SOURCE_DIR}/include /I${CMAKE_SOURCE_DIR}/libs/rapidxml /I${CMAKE_SOURCE_DIR}/libs/lua /Fo:${ZHLGEN_STUB_LIBRARY_OBJ} /c ${ZHLGEN_LIB_FILE_WINE}\"")
    if (NOT ZHLGEN_TC)
      message (FATAL_ERROR "ZHLGEN_TC is unset, compiling the base library for IsaacRepentance.asm is impossible")
    endif ()
    # linux_to_wine_path("${ZHLGEN_TC}" ZHLGEN_TC_WINE)
    set (ZHLGEN_LIB_TC "--gen-lib-tc=\"${ZHLGEN_TC}\"")
    set (ZHLGEN_STUB_FLAG "--stub-obj=\"${ZHLGEN_STUB_LIBRARY_OBJ_WINE}\"")
    set (ZHL_ISAAC_REPENTANCE_ASM_OBJ "${CMAKE_CURRENT_BINARY_DIR}/IsaacRepentance.asm.obj")
  else ()
    message (STATUS "Building natively on Windows")
    set (CLANG_CL_XCC 0)
    set (WINE_RUNNER "")
    set (ZHLPARSER_EXEC "zhlparser")
    set (NAKED_ASM_GEN_EXEC "super_asm_generator")
    set (ZHLGEN_ASM "")
    set (ZHLGEN_ASM_FILE "")
    set (ZHLGEN_LIB_FILE "")
    set (ZHLGEN_LIB_FLAG "")
    set (ZHLGEN_LIB_CMD "")
    set (ZHLGEN_LIB_TC "")
    set (ZHLGEN_STUB_FLAG "")
    set (ZHL_ISAAC_REPENTANCE_ASM_OBJ "")
  endif ()
else ()
  message (FATAL_ERROR "Unknown system configuration, CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}, CMAKE_HOST_SYSTEM_NAME = ${CMAKE_HOST_SYSTEM_NAME}")
endif ()

function (clang_cl_library libname kind export)
  # message(STATUS "Adding clang-cl library with objects: ${ARGN}")
    add_library("${libname}_config" INTERFACE)
    if (CLANG_CL_XCC EQUAL 1)
	add_library("${libname}_base" OBJECT ${ARGN})
	target_compile_definitions("${libname}_base" PUBLIC "${export}")
	target_link_libraries("${libname}_base" PUBLIC "${libname}_config")
	set (ASM_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${libname}.asm")
	set (ASM_OUTPUT_OBJ "${ASM_OUTPUT}.obj")
	set (OBJ_FILE_LIST "${CMAKE_CURRENT_BINARY_DIR}/obj_list_${libname}_$<CONFIG>.txt")
	file (GENERATE OUTPUT "${OBJ_FILE_LIST}" CONTENT "$<JOIN:$<TARGET_OBJECTS:${libname}_base>,\n>")
	add_custom_command(
	    OUTPUT
		"${ASM_OUTPUT}"
		"${ASM_OUTPUT_OBJ}"
	    COMMAND
		    "$<TARGET_FILE_DIR:super_asm_generator>/${NAKED_ASM_GEN_EXEC}"
		    --output="${ASM_OUTPUT}"
		    --toolchain="${ZHLGEN_TC}"
		    --file-list="${OBJ_FILE_LIST}"
	    COMMAND
		    nasm -fwin32 -o "${ASM_OUTPUT_OBJ}" "${ASM_OUTPUT}"
	    DEPENDS
		$<TARGET_OBJECTS:${libname}_base>
		${OBJ_FILE_LIST}
		super_asm_generator
	)
	add_library ("${libname}" "${kind}" "${ASM_OUTPUT_OBJ}")
	target_link_libraries ("${libname}" PRIVATE "${libname}_base")
    else ()
	add_library ("${libname}" "${kind}" ${ARGN})
	target_link_libraries("${libname}" PUBLIC "${libname}_config")
    endif ()
endfunction ()

set ("CMAKE_CXX_STANDARD" 17)
set (ISAAC_DIRECTORY "" CACHE PATH "Path to the root directory of the game")
set (CMAKE_BUILD_TYPE "Debug")
set (ZHL_LOG_FILE "" CACHE PATH "Path to the file in which logs will be written")

add_executable (super_asm_generator "${CMAKE_SOURCE_DIR}/scripts/naked_asm_generator.cpp")

if (NOT CMAKE_ZHL_VERSION)
    set (CMAKE_ZHL_VERSION "dev")
endif ()

if (NOT CMAKE_REPENTOGON_VERSION)
    set (CMAKE_REPENTOGON_VERSION "dev")
endif ()

project(RepExtender)
# add_compile_options("$<$<CONFIG:Release>:/Zi>")
# add_link_options("$<$<CONFIG:Release>:/DEBUG>")

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include "${CMAKE_SOURCE_DIR}/libs/lua" 
    "${CMAKE_SOURCE_DIR}/libs/rapidxml" "${CMAKE_SOURCE_DIR}/libs/rapidjson/include/rapidjson" 
    "${CMAKE_SOURCE_DIR}/libs/LuaBridge/Source/LuaBridge" 
    "${CMAKE_SOURCE_DIR}/libzhl" "${CMAKE_SOURCE_DIR}/libs/imgui" 
    "${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype" "${CMAKE_SOURCE_DIR}/libs/imgui/backends" 
    "${CMAKE_SOURCE_DIR}/libs/NaturalSort" "${CMAKE_SOURCE_DIR}/libs/glad/include" "${CMAKE_SOURCE_DIR}/libs/mINI/src/mini"
    "${CMAKE_SOURCE_DIR}/libs/zydis/include"
    "${CMAKE_SOURCE_DIR}/libs/zydis/dependencies/zycore/include"
)

add_compile_definitions("NOMINMAX")

# Old version of rapidjson does not define this for MSVC, which results in
# build errors.
if (CLANG_CL_XCC EQUAL 1)
    add_compile_definitions("RAPIDJSON_HAS_CXX11_RVALUE_REFS")
endif ()
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

# General logger
add_library(logger STATIC ${CMAKE_SOURCE_DIR}/logger/logger.cpp)

if (CLANG_CL_CXX EQUAL 1)
    add_executable(
	naked_asm_generator
	${CMAKE_SOURCE_DIR}/scripts/naked_asm_generator.cpp
    )
endif ()

# Parser
FILE(GLOB PARSER_SRC ${CMAKE_SOURCE_DIR}/libzhlgen/*.cpp)
add_executable(zhlparser ${PARSER_SRC})
target_link_libraries(zhlparser antlr4_shared logger)
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
add_library(Lua5.3.3r SHARED ${LUA_SRC})

set (ZHL_ISAAC_REPENTANCE_ITF_BASENAME "IsaacRepentance.h")
set (ZHL_ISAAC_REPENTANCE_ITF "${CMAKE_SOURCE_DIR}/libzhl/${ZHL_ISAAC_REPENTANCE_ITF_BASENAME}")
set (ZHL_ISAAC_REPENTANCE_IMPL "${CMAKE_SOURCE_DIR}/libzhl/IsaacRepentance.cpp")

# IsaacRepentance.h/.cpp
FILE(GLOB ZHL_FILES ${CMAKE_SOURCE_DIR}/libzhl/functions/*.zhl)
LIST(APPEND ZHL_FILES ${CMAKE_SOURCE_DIR}/libzhl/functions/ExtraTypes)

add_custom_command(
    OUTPUT
	"${ZHL_ISAAC_REPENTANCE_IMPL}"
	"${ZHLGEN_ASM_FILE}"
	"${ZHL_ISAAC_REPENTANCE_ASM_OBJ}"
    COMMAND
	${WINE_RUNNER} "$<TARGET_FILE_DIR:zhlparser>/${ZHLPARSER_EXEC}"
	    --output-header="${ZHL_ISAAC_REPENTANCE_ITF}"
	    --output-header-base="IsaacRepentance.h"
	    --output-impl="${ZHL_ISAAC_REPENTANCE_IMPL}"
            --output-hooks="${CMAKE_SOURCE_DIR}/libzhl/hooks.json"
	    "${ZHLGEN_ASM}"
	    "${ZHLGEN_LIB_FLAG}"
	    "${ZHLGEN_LIB_CMD}"
	    "${ZHLGEN_LIB_TC}"
	    "${ZHLGEN_STUB_FLAG}"
            --folder="${CMAKE_SOURCE_DIR}/libzhl/functions"
            --file="${CMAKE_SOURCE_DIR}/libzhl/functions/ExtraTypes"
    COMMAND
	nasm -f win32 -o "${ZHL_ISAAC_REPENTANCE_ASM_OBJ}" "${ZHLGEN_ASM_FILE}"
    WORKING_DIRECTORY
	"${CMAKE_SOURCE_DIR}/libzhlgen"
    DEPENDS
	${ZHL_FILES}
	zhlparser
)
set_source_files_properties(${ZHL_ISAAC_REPENTANCE_IMPL} PROPERTIES SYMBOLIC TRUE)

# ZHL
FILE(GLOB ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/*.cpp ${CMAKE_SOURCE_DIR}/libzhl/*.h)
LIST(REMOVE_ITEM ZHL_SRC "${ZHLGEN_LIB_FILE}")
LIST(APPEND ZHL_SRC "${ZHL_ISAAC_REPENTANCE_IMPL}" ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
if (CLANG_CL_XCC EQUAL 1)
    message (STATUS "Appending some stuff")
    LIST (APPEND ZHL_SRC "${ZHL_ISAAC_REPENTANCE_IMPL}" "${ZHL_ISAAC_REPENTANCE_ASM_OBJ}")
endif ()
add_compile_definitions(GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
clang_cl_library(libzhl SHARED libzhl_EXPORTS ${ZHL_SRC})
target_sources(libzhl PRIVATE "${ZHL_ISAAC_REPENTANCE_ASM_OBJ}")
target_compile_options(libzhl_config INTERFACE "/MD" "/wd4251" "/wd4996")
target_compile_options(libzhl_config INTERFACE "/EHs")
target_compile_definitions(libzhl_config INTERFACE CMAKE_ZHL_VERSION="${CMAKE_ZHL_VERSION}")

add_custom_command(TARGET libzhl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/libzhl/hooks.json" "$<TARGET_FILE_DIR:zhlparser>")
# wd4251 ignore warning C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
# wd4996 ignores warning C4996: using a depricated function

# ZHL Loader
FILE(GLOB ZHL_LOADER_SRC ${CMAKE_SOURCE_DIR}/loader/*.cpp)
add_library (zhlLoader SHARED ${ZHL_LOADER_SRC})
if (CLANG_CL_XCC EQUAL 1)
  set (IMAGEHLP_NAME imagehlp)
else ()
  set (IMAGEHLP_NAME Imagehlp)
endif ()
target_link_libraries(zhlLoader "${IMAGEHLP_NAME}" libzhl)
target_compile_options(zhlLoader PUBLIC "/MD" "/wd4996")
target_compile_definitions(zhlLoader PRIVATE CMAKE_ZHL_VERSION="${CMAKE_ZHL_VERSION}")

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
add_library(imgui ${IMGUI_SRC} ${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype/imgui_freetype.h ${CMAKE_SOURCE_DIR}/libs/imgui/misc/freetype/imgui_freetype.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl2.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl2.h ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.h ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3_loader.h ${IMGUI_BACKENDS_DIR}/imgui_impl_win32.h ${IMGUI_BACKENDS_DIR}/imgui_impl_win32.cpp)
target_include_directories(imgui PUBLIC "${CMAKE_SOURCE_DIR}/libs/freetype/include")
target_compile_options(imgui PUBLIC "/MD")
target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE IMGUI_USE_WCHAR32)

# Repentogon
file(GLOB_RECURSE REPENTOGON_SRC ${CMAKE_SOURCE_DIR}/repentogon/*.*)
# Display folder structure inside of VS solution - https://stackoverflow.com/a/54531576
foreach(_source IN ITEMS ${REPENTOGON_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# add_library(zhlREPENTOGON SHARED ${REPENTOGON_SRC} )
clang_cl_library(zhlREPENTOGON SHARED zhlREPENTOGON_EXPORTS ${REPENTOGON_SRC})
target_compile_definitions(zhlREPENTOGON_config INTERFACE IMGUI_USE_WCHAR32)
target_compile_options(zhlREPENTOGON_config INTERFACE "/MD" "/MP" "/wd4251" "/wd4996")
target_compile_options(zhlREPENTOGON_config INTERFACE "/EHs")
target_compile_definitions(zhlREPENTOGON_config INTERFACE NOMINMAX CMAKE_REPENTOGON_VERSION="${CMAKE_REPENTOGON_VERSION}")

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
target_compile_options(zhlDelirium PRIVATE "/EHs")

add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources" "$<TARGET_FILE_DIR:zhlREPENTOGON>/resources")
add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources-repentogon" "$<TARGET_FILE_DIR:zhlREPENTOGON>/resources-repentogon")
# add_custom_command(TARGET zhlDelirium POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/delirium/resources-delirium" "$<TARGET_FILE_DIR:zhlDelirium>/resources-delirium")

add_subdirectory("libs/injector")
add_executable(offline_scanner "${CMAKE_SOURCE_DIR}/offline_scanner/offline_scanner.cpp")
target_include_directories(offline_scanner PRIVATE "${CMAKE_SOURCE_DIR}/libzhl" "${CMAKE_SOURCE_DIR}/libs/injector/include")
target_link_libraries(offline_scanner injector)


if(NOT ISAAC_DIRECTORY STREQUAL "")
    message (STATUS "Files will be installed to " ${ISAAC_DIRECTORY})
    add_custom_command(TARGET Lua5.3.3r POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:Lua5.3.3r>/Lua5.3.3r.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET zhlLoader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlLoader>/zhlLoader.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET libzhl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:libzhl>/libzhl.dll" "${ISAAC_DIRECTORY}")
	add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:freetype>/freetype.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlREPENTOGON>/zhlREPENTOGON.dll" "${ISAAC_DIRECTORY}")
	add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources" "${ISAAC_DIRECTORY}/resources")
    add_custom_command(TARGET zhlREPENTOGON POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/repentogon/resources-repentogon" "${ISAAC_DIRECTORY}/resources-repentogon")
    # add_custom_command(TARGET zhlDelirium POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlDelirium>/zhlDelirium.dll" "${ISAAC_DIRECTORY}")
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
target_link_libraries(zhlREPENTOGON PRIVATE Lua5.3.3r libzhl freetype "Zydis" imgui ${OPENGL_LIBRARIES})
target_link_libraries(libzhl PRIVATE Lua5.3.3r "Zydis" dbghelp ${IMAGEHLP_NAME})

# Put libraries into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(freetype antlr4_shared imgui Lua5.3.3r stb stbc Zydis Zycore PROPERTIES FOLDER "External Libs")
set_target_properties(zhlLoader libzhl zhlparser PROPERTIES FOLDER "libzhl")
set_target_properties(zhlDelirium zhlREPENTOGON PROPERTIES FOLDER "Repentogon")
