cmake_minimum_required(VERSION 3.13.0)

set ("CMAKE_CXX_STANDARD" 17)
set (ISAAC_DIRECTORY "" CACHE PATH "Path to the root directory of the game")

project(RepExtender)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include "${CMAKE_SOURCE_DIR}/include/LuaBridge" "${CMAKE_SOURCE_DIR}/libs/lua-5.4.4/src" "${CMAKE_SOURCE_DIR}/libs/rapidxml-1.13" "${CMAKE_SOURCE_DIR}/libs/rapidjson" "${CMAKE_SOURCE_DIR}/libs/LuaSource-2.0/Source/LuaBridge" "${CMAKE_SOURCE_DIR}/libzhl")

# Zydis
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
option(ZYDIS_BUILD_DOXYGEN "" OFF)
add_subdirectory("libs/zydis")

FILE(GLOB LUA_SRC ${CMAKE_SOURCE_DIR}/libs/lua-5.4.4/src/*.c)
list(REMOVE_ITEM LUA_SRC "${CMAKE_SOURCE_DIR}/libs/lua-5.4.4/src/luac.c")
add_compile_definitions(LUA_BUILD_AS_DLL)
add_library(Lua5.3.3r SHARED ${LUA_SRC})

add_executable(lua "${CMAKE_SOURCE_DIR}/libs/lua-5.4.4/src/lua.c")
target_link_libraries (lua Lua5.3.3r)

FILE(GLOB LUA_LFS_SRC ${CMAKE_SOURCE_DIR}/libs/luafilesystem/src/*.c)
add_library(lfs SHARED ${LUA_LFS_SRC})
target_link_libraries(lfs Lua5.3.3r)

FILE (GLOB LUA_LPEG_SRC ${CMAKE_SOURCE_DIR}/libs/lpeg-1.0.2/*.c)
add_library(lpeg SHARED ${LUA_LPEG_SRC})
target_link_libraries(lpeg Lua5.3.3r)

FILE(GLOB ZHL_FILES ${CMAKE_SOURCE_DIR}/libzhl/functions/*.zhl)
# add_custom_target(ZHL COMMAND lua parsefuncs.lua WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libzhlgen" DEPENDS ${ZHL_FILES})
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/libzhl/IsaacRepentance.cpp COMMAND lua parsefuncs.lua WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/libzhlgen" DEPENDS ${ZHL_FILES} lpeg lfs)
# add_dependencies(ZHL lpeg lfs)

FILE(GLOB ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/*.cpp)
LIST(APPEND ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/IsaacRepentance.cpp)
add_library(libzhl SHARED ${ZHL_SRC})

FILE(GLOB LAUNCHER_SRC ${CMAKE_SOURCE_DIR}/launcher/*.cpp)
add_library(dsound SHARED ${LAUNCHER_SRC})

#[[FILE(GLOB LUAUP_SRC ${CMAKE_SOURCE_DIR}/Lua54Upgrader/*.cpp)
add_library(Lua5.3.3r SHARED ${LUAUP_SRC})]]

FILE(GLOB MODTEST_SRC ${CMAKE_SOURCE_DIR}/modtest/*.cpp)
add_library(zhlModTest SHARED ${MODTEST_SRC})

add_custom_command(TARGET zhlModTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/modtest/scripts" "$<TARGET_FILE_DIR:zhlModTest>/resources/scripts")
add_custom_command(TARGET zhlModTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/modtest/mods" "$<TARGET_FILE_DIR:zhlModTest>/mods")

if(NOT ISAAC_DIRECTORY STREQUAL "")
    message (STATUS "Files will be installed to " ${ISAAC_DIRECTORY})
    add_custom_command(TARGET Lua5.3.3r POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:dsound>/Lua5.3.3r.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET dsound POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:dsound>/dsound.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET libzhl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:libzhl>/libzhl.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET zhlModTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:zhlModTest>/zhlModTest.dll" "${ISAAC_DIRECTORY}")
    add_custom_command(TARGET zhlModTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/modtest/scripts" "${ISAAC_DIRECTORY}/resources/scripts")
    add_custom_command(TARGET zhlModTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/modtest/mods" "${ISAAC_DIRECTORY}/mods")
endif()
		
# add_dependencies(libzhl ZHL)

add_dependencies(zhlModTest libzhl)

add_library(stb SHARED stbParser/stbParser.cpp)
add_executable(stbc stbParser/stbParserc.cpp)
target_link_libraries(stbc stb)
target_link_libraries(zhlModTest Lua5.3.3r libzhl "Zydis" ${OPENGL_LIBRARIES})
target_link_libraries(libzhl Lua5.3.3r "Zydis")
