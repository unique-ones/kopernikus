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

# Add subprojects
add_subdirectory(libcore)
add_subdirectory(libascom)

file(GLOB KOPERNIKUS_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c)

add_executable(${PROJECT_NAME} ${KOPERNIKUS_SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE "core" "ascom")

# we want to use unsecure functions ;)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS=1)
    target_compile_definitions(core PRIVATE _CRT_SECURE_NO_WARNINGS=1)
    target_compile_definitions(ascom PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif ()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-strict-prototypes -Wno-language-extension-token)
endif ()

# Copy Assets to the Output Directory
file(COPY ${CMAKE_CURRENT_LIST_DIR}/data DESTINATION ${CMAKE_INSTALL_PREFIX})
