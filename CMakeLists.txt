cmake_minimum_required(VERSION 3.24)
project(project_lost C)

set(CMAKE_C_STANDARD 23)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(libs_pdcurses ${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses/wincon)
endif()

FILE(GLOB_RECURSE PDCURSES_SRC ${libs_pdcurses}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses/pdcurses/*.c)
FILE(GLOB_RECURSE LOG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libs/logging/*.c)

add_executable(project_lost main.c ${PDCURSES_SRC} src/core.c src/core.h src/menu.c src/menu.h src/walk_mode.c src/walk_mode.h src/game.c src/game.h libs/heightmap/height_map.c)
#add_compile_options(-DDEBUG)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/pdcurses)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/logging/src)
add_library(pdcurses STATIC ${PDCURSES_SRC})
add_library(log STATIC ${LOG_SRC})

target_link_libraries(project_lost log pdcurses)
