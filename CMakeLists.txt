cmake_minimum_required(VERSION 3.31)
project(CNet C)

set(CMAKE_C_STANDARD 23)

add_executable(CNet main.c include/globals.h src/globals.c)

if (WIN32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()