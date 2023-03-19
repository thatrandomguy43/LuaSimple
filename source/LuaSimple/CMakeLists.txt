cmake_minimum_required(VERSION 3.25)

include(FetchContent)
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/walterschell/Lua.git
)
FetchContent_MakeAvailable(lua)
target_sources(${EXECUTABLE_NAME} PUBLIC LuaInstance.hpp LuaTypeClasses.hpp LuaInstance.cpp)

target_link_libraries(${EXECUTABLE_NAME} lua_static)
target_include_directories(${EXECUTABLE_NAME} PUBLIC luainclude)