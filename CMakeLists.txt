cmake_minimum_required(VERSION 3.31)
project(CNet C)
project(CNetLibray C)

set(CMAKE_C_STANDARD 23)

add_library(CNetLibray main.c ${PROJECT_SOURCE_DIR}/src/globals.c)

target_include_directories(CNetLibray
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (WIN32)
    target_link_libraries(CNetLibray PRIVATE ws2_32)
endif ()

add_executable(CNet main.c ${PROJECT_SOURCE_DIR}/include/globals.h)

target_link_libraries(CNet PRIVATE CNetLibray)

if (WIN32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()