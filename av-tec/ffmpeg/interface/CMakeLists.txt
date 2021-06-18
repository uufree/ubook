cmake_minimum_required(VERSION 3.2)
project(audio_extract)

set(CMAKE_C_COMPILER "/usr/local/gcc-4.9.4/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/local/gcc-4.9.4/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -lpthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(/usr/local/lib)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

#add_library(audio_extract SHARED ${SRC_FILES})
#target_link_libraries(audio_extract avformat avdevice avcodec avutil swresample avfilter postproc swscale)