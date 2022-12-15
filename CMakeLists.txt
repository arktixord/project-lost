cmake_minimum_required(VERSION 3.24)
project(project_lost C)

set(CMAKE_C_STANDARD 23)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(libs_pdcurses ${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses/wincon)
endif()

FILE(GLOB_RECURSE PDCURSES_SRC ${libs_pdcurses}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses/pdcurses/*.c)

add_executable(project_lost main.c ${PDCURSES_SRC} src/battle_mode.c src/battle_mode.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses)
add_library(pdcurses STATIC ${PDCURSES_SRC})
target_link_libraries(project_lost pdcurses)
