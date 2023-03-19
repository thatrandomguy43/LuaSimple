# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-src"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-build"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/tmp"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/src"
  "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Asger/Desktop/programming/LuaSimple/build/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
