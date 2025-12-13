cmake_minimum_required(VERSION 3.31)
project(CNet LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_executable(CNet main.c ${PROJECT_SOURCE_DIR}/src/globals.c)
target_include_directories(CNet PRIVATE include)
if (WIN32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()