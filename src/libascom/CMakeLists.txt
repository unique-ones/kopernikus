# Gather libascom files
file(GLOB ASCOM_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/http/*.c
        ${CMAKE_CURRENT_LIST_DIR}/http/*.h
        ${CMAKE_CURRENT_LIST_DIR}/utils/*.c
        ${CMAKE_CURRENT_LIST_DIR}/utils/*.h)

add_library(ascom ${ASCOM_SOURCE_LIST})
target_include_directories(ascom PRIVATE "${CMAKE_SOURCE_DIR}/src/")

# Options for curl
set(HTTP_ONLY ON)
set(BUILD_CURL_EXE OFF)  # Do not build the curl executable
set(BUILD_SHARED_LIBS OFF)  # Build static libraries to reduce number of targets
set(CURL_DISABLE_TESTS ON)  # Disable tests to reduce the number of targets
FetchContent_Declare(CURL GIT_REPOSITORY https://github.com/curl/curl.git)

# Link libraries
FetchContent_MakeAvailable(CURL)
target_link_libraries(ascom PRIVATE CURL::libcurl core)
