# CMake generated Testfile for 
# Source directory: C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src
# Build directory: C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(lua-testsuite "lua" "-e" "_U=true" "all.lua")
  set_tests_properties(lua-testsuite PROPERTIES  WORKING_DIRECTORY "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/lua-5.4.4-tests" _BACKTRACE_TRIPLES "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;31;add_test;C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(lua-testsuite "lua" "-e" "_U=true" "all.lua")
  set_tests_properties(lua-testsuite PROPERTIES  WORKING_DIRECTORY "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/lua-5.4.4-tests" _BACKTRACE_TRIPLES "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;31;add_test;C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(lua-testsuite "lua" "-e" "_U=true" "all.lua")
  set_tests_properties(lua-testsuite PROPERTIES  WORKING_DIRECTORY "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/lua-5.4.4-tests" _BACKTRACE_TRIPLES "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;31;add_test;C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(lua-testsuite "lua" "-e" "_U=true" "all.lua")
  set_tests_properties(lua-testsuite PROPERTIES  WORKING_DIRECTORY "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/lua-5.4.4-tests" _BACKTRACE_TRIPLES "C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;31;add_test;C:/Users/Bruger/Skrivebord/LuaSimple/build/_deps/lua-src/CMakeLists.txt;0;")
else()
  add_test(lua-testsuite NOT_AVAILABLE)
endif()
subdirs("lua-5.4.4")
