cmake_minimum_required(VERSION 3.2)
project(CNetLib LANGUAGES C)
project(CNet LANGUAGES C)
set(CMAKE_C_STANDARD 17)

add_executable(CNet main.c)

target_include_directories(CNet INTERFACE include)
target_link_libraries(CNet PRIVATE CNetLib)

if (WIN32)
    target_link_libraries(CNet PRIVATE ws2_32)
endif ()

add_library(CNetLib ${PROJECT_SOURCE_DIR}/src/cnet.c)

target_include_directories(CNetLib PRIVATE include)

if (WIN32)
    target_link_libraries(CNetLib PRIVATE ws2_32)

endif ()

install(TARGETS CNetLib DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
install(FILES ${PROJECT_SOURCE_DIR}/include/cnet.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)
