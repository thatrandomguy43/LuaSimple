cmake_minimum_required(VERSION 3.25)

project(LuaSimple_ExampleProject
LANGUAGES CXX
VERSION 0.1
)

add_executable(example_project)
target_sources(example_project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

#the important line for adding this to somethign else
set(EXECUTABLE_NAME 
#INSERT_YOUR_EXECUTABLE_NAME_HERE
example_project
)

add_subdirectory(LuaSimple)
