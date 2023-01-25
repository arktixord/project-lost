cmake_minimum_required(VERSION 3.22)
project(project_lost C)

set(CMAKE_C_STANDARD 23)

#
# PLATFORM-INDEPENDENT SECTION
#

# general source
SET(SRC_PATH ${CMAKE_SOURCE_DIR}/src)
FILE(GLOB_RECURSE SRC ${SRC_PATH}/*.c)

# log library
SET(LOG_PATH ${CMAKE_SOURCE_DIR}/libs/log/src)
FILE(GLOB_RECURSE LOG_SRC ${LOG_PATH}/*.c)
add_library(log STATIC ${LOG_SRC})
include_directories(${LOG_PATH})

# heightmap library
SET(HEIGHTMAP_PATH ${CMAKE_SOURCE_DIR}/libs/heightmap)
FILE(GLOB_RECURSE HEIGHTMAP_SRC ${HEIGHTMAP_PATH}/*.c)
add_library(heightmap STATIC ${HEIGHTMAP_SRC})
include_directories(${HEIGHTMAP_PATH})


#
# PLATFORM-DEPENDENT SECTION
#

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(PDCURSES_PATH ${CMAKE_SOURCE_DIR}/libs/pdcurses)
    FILE(GLOB_RECURSE PDCURSES_SRC ${PDCURSES_PATH}/wincon/*.c ${PDCURSES_PATH}/pdcurses/*.c)
    add_library(pdcurses STATIC ${PDCURSES_SRC})
    include_directories(${PDCURSES_PATH})

    add_executable(project_lost main.c ${SRC})

    target_link_libraries(project_lost log pdcurses heightmap)

    add_compile_definitions(OS_WINDOWS)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_executable(project_lost main.c ${SRC})

    target_link_libraries(project_lost log heightmap curses)

    add_compile_definitions(OS_LINUX)
endif()



