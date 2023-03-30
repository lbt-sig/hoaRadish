# 设置运行此配置文件所需的CMake最低版本
cmake_minimum_required(VERSION 3.15)

# 平台识别输出
MESSAGE([OS])

IF (WIN32)
    MESSAGE(STATUS "Windows.\n")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")


ELSEIF (UNIX)
    MESSAGE(STATUS "UNIX-like.\n")
#编译器unix
    set (CMAKE_C_COMPILER "gcc")
    set (CMAKE_CXX_COMPILER "g++")
# 32、64位编译选项 unix
    set(PROGRAM_BITS -m64)
#警告级别
    set(WARN_LEVEL -Wall)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROGRAM_BITS} ${WARN_LEVEL}")
    MESSAGE(STATUS "program_bits ${PROGRAM_BITS}")
ENDIF ()

# C++语言标准
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

# 启动对C标准的支持
set(CMAKE_C_STANDARD 99)
# 显式要求指明支持C标准
set(CMAKE_C_STANDARD_REQUIRED True)



# 设置项目名称
project(hoaRadish)
# 调试/发布
set(CMAKE_BUILD_TYPE "Debug")

#Macros
#add_compile_definitions()

# 头文件目录
include_directories(./)
include_directories(./include)

# 源代码目录
aux_source_directory(./src SRC_DIR)

# add the executable
# 添加一个可执行文件
# add_executable(test ${TEST_DIR} )
add_library(hoaRadish STATIC ${SRC_DIR} )


#target_include_directories(raserver PUBLIC ${PROJECT_SOURCE_DIR}/raserver/inc/)
# target_link_libraries(test hoaRadish)

IF(UNIX OR APPLE)
#    target_link_libraries(test pthread)
ENDIF ()

IF(WIN32)
#    target_link_libraries(test ws2_32)
ENDIF ()