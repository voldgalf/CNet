cmake_minimum_required(VERSION 3.31)
project(CNet LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(CNet main.c ${PROJECT_SOURCE_DIR}/include/globals.h ${PROJECT_SOURCE_DIR}/src/globals.c)

if (MSVC)
    target_compile_options(CNet PRIVATE /TC)
endif()

if (WIN32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()