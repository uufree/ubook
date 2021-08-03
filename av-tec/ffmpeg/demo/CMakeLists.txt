cmake_minimum_required(VERSION 3.2)
project(main)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -lpthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB SRC_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc)
file(GLOB SRC_CMD ${CMAKE_CURRENT_SOURCE_DIR}/cmd/main.cc)

add_executable(main ${SRC_FILES} ${SRC_CMD} ${SRC_TESTS})
target_link_libraries(main avformat avdevice avcodec avutil swresample avfilter postproc swscale)
