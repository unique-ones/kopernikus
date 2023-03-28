cmake_minimum_required(VERSION 3.22)

project(kopernikus VERSION 0.0.1 DESCRIPTION "tracking sequencer")
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfw)

add_subdirectory(extern)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(kopernikus
        "src/kopernikus.cpp"
        "src/display.h"
        "src/display.cpp"
		"src/render.h"
		"src/render.cpp")

target_include_directories(kopernikus PUBLIC "extern/")
target_link_libraries(kopernikus PUBLIC "glfw" "ImGui" "glad" "imnodes")
