cmake_minimum_required(VERSION 3.2)
project(sdl)

# system setting
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-pthread -std=c++11")

include_directories(/usr/local/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(/usr/local/lib)

file(GLOB SRC_FILES src/*.cc)

add_executable(main ${SRC_FILES})
target_link_libraries(main SDL2)