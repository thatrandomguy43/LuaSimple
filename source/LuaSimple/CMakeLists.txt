cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
include(FetchContent)
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/walterschell/Lua.git
  GIT_TAG        v5.4.3
)
FetchContent_MakeAvailable(lua)
target_sources(${EXECUTABLE_NAME} PUBLIC LuaInstance.cpp LuaInstance.hpp SetGlobal.hpp PushVariable.hpp GetGlobal.hpp GetVariable.hpp)

target_link_libraries(${EXECUTABLE_NAME} lua_static)
target_include_directories(${EXECUTABLE_NAME} PUBLIC luainclude)