cmake_minimum_required(VERSION 3.31)
project(CNetLib LANGUAGES C)
project(CNet LANGUAGES C)
set(CMAKE_C_STANDARD 17)
add_definitions(-DCONFIG_CTRL_IFACE)

message(STATUS, ${CMAKE_INSTALL_PREFIX})

add_library(CNetLib ${PROJECT_SOURCE_DIR}/src/cnet.c)

add_executable(CNet main.c ${PROJECT_SOURCE_DIR}/src/cnet.c)

target_include_directories(CNetLib PRIVATE include)
target_include_directories(CNet PRIVATE include)

if (WIN32)
    target_link_libraries(CNetLib PRIVATE ws2_32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()

install(TARGETS CNetLib DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
install(FILES ${PROJECT_SOURCE_DIR}/include/cnet.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)

target_compile_definitions(CNet PUBLIC CONFIG_CTRL_IFACE)