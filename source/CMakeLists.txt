cmake_minimum_required(VERSION 3.25.2)
cmake_policy(SET CMP0141 NEW)
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


if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")
target_link_options(${EXECUTABLE_NAME} PUBLIC "$<$<CONFIG:Debug>:/INCREMENTAL:NO>")
target_compile_options(${EXECUTABLE_NAME} PRIVATE /W3 /WX)
endif(MSVC)

add_subdirectory(LuaSimple)
