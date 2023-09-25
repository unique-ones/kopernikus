#
#  MIT License
#
#  Copyright (c) 2023 Elias Engelbert Plank
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

include(FetchContent)
FetchContent_Declare(solaris GIT_REPOSITORY https://github.com/koepla/solaris.git GIT_TAG "origin/main")
FetchContent_Declare(cimgui GIT_REPOSITORY https://github.com/koepla/cimgui.git GIT_TAG "origin/main")
FetchContent_MakeAvailable(solaris cimgui)

file(GLOB CORE_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/ui/*.c
        ${CMAKE_CURRENT_LIST_DIR}/ui/*.h)

add_library(libcore ${CORE_SOURCE_LIST} "${CMAKE_SOURCE_DIR}/src/extern/glad/glad.h" "${CMAKE_SOURCE_DIR}/src/extern/glad/glad.c")
target_include_directories(libcore PUBLIC "${CMAKE_SOURCE_DIR}/src/extern/" "${CMAKE_SOURCE_DIR}/src/")
target_link_libraries(libcore PUBLIC "solaris" "cimgui")
target_compile_definitions(libcore PUBLIC CIMGUI_DEFINE_ENUMS_AND_STRUCTS)
