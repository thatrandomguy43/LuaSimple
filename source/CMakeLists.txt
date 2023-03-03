cmake_minimum_required(VERSION 3.26)
cmake_policy(SET CMP0141 NEW)
project(LuaSimple_ExampleProject
LANGUAGES CXX
VERSION 0.1
)

add_executable(example_project)
target_sources(example_project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
set_property(TARGET example_project PROPERTY MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")
target_link_options(example_project PUBLIC "$<$<CONFIG:Debug>:/INCREMENTAL:NO>")

endif(MSVC)

#the important line for adding this to somethign else
set(EXECUTABLE_NAME 
#INSERT_YOUR_EXECUTABLE_NAME_HERE
example_project
)

add_subdirectory(LuaSimple)
