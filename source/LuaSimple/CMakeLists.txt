cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
include(FetchContent)
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG        v5.4.4
)
FetchContent_MakeAvailable(lua)
target_sources(${EXECUTABLE_NAME} PUBLIC LuaInstance.hpp SetGlobal.hpp PushVariable.hpp GetGlobal.hpp GetVariable.hpp luainclude/lua.hpp luainclude/lua.h luainclude/lualib.h luainclude/lauxlib.h luainclude/luaconf.h)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_23)