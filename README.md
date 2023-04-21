[![CodeQL](https://github.com/thatrandomguy43/LuaSimple/actions/workflows/codeql.yml/badge.svg)](https://github.com/thatrandomguy43/LuaSimple/actions/workflows/codeql.yml) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/45096f165bef42d6b6034c485f8ea561)](https://app.codacy.com/gh/thatrandomguy43/LuaSimple/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
# LuaSimple
Yet another Lua wrapper for C++ because i got fed up with another one i was trying to use.

### Usage
This library uses a wrapper class around the Lua state pointer provided by the Lua API to simplify its usage. Each wrapper respresents an independent Lua execution enviroment, with its own global variables and stack. Lua threads with the coroutine library are not supported in this library, but multiple wrappers can be created to have multiple threads of execution.

### main.cpp
A main.cpp file is included, which contains some sample code that tests the varius features of LuaSimple. If examples of how to use the library are required, that is the place to look. The code ran inside it is in no way needed for the library's functioning, and is not needed in any other project.

### Compilation tests
The code has been tested on 2 computers of mine with MSVC on C++23 (or whatever -std:c++latest is exactly), but it does not use any features newer than C++17 to my knowledge. The code can also be compiled by GitHub CodeQL, which appears to use g++ on 64-bit Linux. 

### CMake
CMake is used to build the test application, and a CMake file in included in the library directory. CMake automatically downloads a CMake-buildable fork of Lua as part of the build process. The usabillity of the library with any alternative build systems is not known to me.
