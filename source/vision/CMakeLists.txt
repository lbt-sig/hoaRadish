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

set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Project Name.
project(binoculars)
set(CMAKE_BUILD_TYPE "Debug")

#Macros
#add_compile_definitions()
include_directories(./)
include_directories(./include)

aux_source_directory(./src SRC_DIR)

# add the executable
add_library(binoculars STATIC ${SRC_DIR} )


#target_include_directories(raserver PUBLIC ${PROJECT_SOURCE_DIR}/raserver/inc/)
# target_link_libraries(test hoaRadish)

IF(UNIX OR APPLE)
#    target_link_libraries(test pthread)
ENDIF ()

IF(WIN32)
#    target_link_libraries(test ws2_32)
ENDIF ()