cmake_minimum_required(VERSION 3.15)
MESSAGE([OS])

IF (WIN32)
    MESSAGE(STATUS "Windows.\n")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSEIF (UNIX)
    MESSAGE(STATUS "UNIX-like.\n")
    set (CMAKE_C_COMPILER "gcc")
    set(PROGRAM_BITS -m64)
    set(WARN_LEVEL -Wall)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}")
    MESSAGE(STATUS "program_bits ${PROGRAM_BITS}")
ENDIF ()

# Project Name.
project(binoculars)

set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(HOA_SUPPORT_DIR "${PROJECT_SOURCE_DIR}/cmake")
set(HOA_CONTRIB_DIR "${PROJECT_SOURCE_DIR}/contrib")

set(CMAKE_BUILD_TYPE "Debug")

#Macros
include_directories(./)
include_directories(./include)
include_directories(../../include/common)

aux_source_directory(./ MAIN_DIR)
aux_source_directory(./src SRC_DIR)
aux_source_directory(./src/util UTIL_DIR)

# add the library.
add_library(binoculars STATIC ${SRC_DIR} ${UTIL_DIR})

# add the executable.
# add_executable(ebinoculars ${MAIN_DIR})


# auto download deps.
# opencv 3.4.5.
# add_subdirectory(contrib)

# build test project.
add_subdirectory(test)
IF(UNIX OR APPLE)
#    target_link_libraries(test pthread)
ENDIF ()

IF(WIN32)
#    target_link_libraries(test ws2_32)
ENDIF ()