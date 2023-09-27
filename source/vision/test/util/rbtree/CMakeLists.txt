cmake_minimum_required(VERSION 3.15)

#Macros
include_directories(./)
include_directories(../../../include/comm)
include_directories(../../../include/util)

aux_source_directory(./ MAIN_DIR)

# add the executable.
add_executable(test_rbtree ${MAIN_DIR})

# set library search path.
# link_directories(${CMAKE_SOURCE_DIR}/build)
target_link_libraries(test_rbtree binoculars)