# gather libascom files
file(GLOB ASCOM_SOURCE_LIST
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/http/*.c
    ${CMAKE_CURRENT_LIST_DIR}/http/*.h)

add_library(libascom ${ASCOM_SOURCE_LIST})
target_include_directories(libascom PRIVATE "${CMAKE_SOURCE_DIR}/src/")

# options for curl
set(HTTP_ONLY ON)
set(BUILD_CURL_EXE OFF)  # Do not build the curl executable
set(BUILD_SHARED_LIBS OFF)  # Build static libraries to reduce number of targets
set(CURL_DISABLE_TESTS ON)  # Disable tests to reduce the number of targets

include(FetchContent)
FetchContent_Declare(CURL GIT_REPOSITORY https://github.com/curl/curl.git)
FetchContent_MakeAvailable(CURL)
target_link_libraries(libascom PUBLIC CURL::libcurl libcore)
