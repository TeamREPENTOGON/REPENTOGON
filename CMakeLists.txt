cmake_minimum_required(VERSION 3.0.0)

project(RepExtender)

include_directories(${CMAKE_SOURCE_DIR}/include)

FILE(GLOB LUA_SRC ${CMAKE_SOURCE_DIR}/libs/lua-5.3.3/src/*.c)
list(REMOVE_ITEM LUA_SRC "${CMAKE_SOURCE_DIR}/libs/lua-5.3.3/src/luac.c")
add_library(Lua5.3.3r SHARED ${LUA_SRC})

FILE(GLOB ZHL_SRC ${CMAKE_SOURCE_DIR}/libzhl/*.cpp)
add_library(libzhl SHARED ${ZHL_SRC})

FILE(GLOB LAUNCHER_SRC ${CMAKE_SOURCE_DIR}/launcher/*.cpp)
add_library(dsound SHARED ${LAUNCHER_SRC})

FILE(GLOB MODTEST_SRC ${CMAKE_SOURCE_DIR}/modtest/*.cpp)
add_library(zhlModTest SHARED ${MODTEST_SRC})

target_link_libraries(zhlModTest Lua5.3.3r libzhl)
target_link_libraries(libzhl Lua5.3.3r)