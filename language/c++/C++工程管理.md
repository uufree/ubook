# C++工程管理

[TOC]

## makefile

TODO..

## cmake

### 基本语法

```cmake
# 单行注释

message("Hello, World")

# 多行注释
#[[
message("Hello, World.")
message("Hello, World.")
]]

# 设置变量
set(var 123)

# 打印变量
message("var = ${var}")

# 打印系统变量
message("path = $ENV{PATH}")

# 定义列表
set (list 1 2 3 4 5 6)

# 引用列表
message(${list})    	# 输出：123456
message("${list}")    # 输出：1;2;3;4;5;6

# 宏变量
cmake_minimum_required(VERSION 3.14)
if(CRC)
    message("crc ON")
else(CRC)
    message("crc OFF")
endif()

cmake -DCRC=OFF .     # 输出：crc OFF
cmake -DCRC=ON .    	# 输出：crc ON

# 操作符
一元操作符：EXISTS、COMMAND、DEFINED
逻辑操作符：NOT、AND、OR
二元数值操作符：EQUAL、LESS、LESS_EQUAL、GREATER、GREATER_EQUAL
二元字符串操作符：STREQUAL、STRLESS、STRLESS_EQUAL、STRGREATER、STRGREATER_EQUAL

# 布尔值
ON、OFF

# 条件语句
if(${tag})
    message("if")
else(${tag})
    message("else")
endif(${tag})

# 循环语句
while(NOT str STREQUAL "xxxxx")
    set(str "${str}x")
    message("str: ${str}")
endwhile()

# 循环语句控制流
break()
continue()

# 循环遍历语句
foreach(var RANGE 10)
    message(${var})
endforeach()    # 0 1 2 3 4 5 6 7 8 9 10

# 函数
function(foo argv1 argv2 argv3)
    message("a/argv1: ${argv1}")
    message("b/rgv2: ${argv2}")
    message("c/argv3: ${argv3}")
    message("argc: ${ARGC}")
endfunction(foo)

foo("1" "2" "3")
```

### 完整流程

```cmake
# 声明cmake版本和工程名称
cmake_minimum_required(VERSION 3.14)				# 指定最小的CMake版本
project(UKERNEL)														# 设置工程名称
set(EXECUTABLE_OUTPUT_PATH [output_path])		# 可执行文件的输出路径
set(LIBRARY_OUTPUT_PATH [output_path]) 			# 生成库文件的输出路径


# 设置编译器选项
set(CMAKE_C_COMPILER "gcc")											# 设置c编译器
set(CMAKE_C_FLAGS "-Wall -pthread -std=c++11")	# 设置c编译器的通用属性
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb")					# 设置c编译器的debug选项。使用cmake -DCMAKE_BUILD_TYPE=Debug时生效
set(CMAKE_C_FLAGS_RELEASE "-O3")								# 设置c编译器的release选项。使用cmake -DCMAKE_BUILD_TYPE=Release时生效

set(CMAKE_CXX_COMPILER "g++")											# 设置c++编译器
set(CMAKE_CXX_FLAGS "-Wall -pthread -std=c++11")	# 设置c++编译器的通用属性
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")					# 设置c++编译器的debug选项。使用cmake -DCMAKE_BUILD_TYPE=Debug时生效
set(CMAKE_CXX_FLAGS_RELEASE "-O3")								# 设置c++编译器的release选项。使用cmake -DCMAKE_BUILD_TYPE=Release时生效

# 设置库和头文件的路径
include_directories(./include ${MY_INCLUDE})		# 设置头文件位置
link_directories(./lib)													# 设置动态库位置

# 设置源文件
aux_source_directory(../cc-test/ SRC_LIST)			# 收集源文件并保存到相应变量中
file(GLOB variable *.cc)												# 收集源文件并保存到相应变量中

# 编译目标文件
add_library(<name> [STATIC|SHARED] sourceFiles)	# 构建库
add_executable(<name> ${SRC_LIST})							# 构建二进制文件

# 设置库链接
target_link_libraries(<name> lib1 lib2 lib3)		# 设置库链接
```

### Q&A

1. `find_package`怎么用？

   ```cmake
   find_package(CURL)
   add_executable(curltest curltest.cc)
   if(CURL_FOUND)
       target_include_directories(clib PRIVATE ${CURL_INCLUDE_DIR})
       target_link_libraries(curltest ${CURL_LIBRARY})
   else(CURL_FOUND)
       message(FATAL_ERROR ”CURL library not found”)
   endif(CURL_FOUND)
   ```

   其次，`find_package`存在两种模式：

   - Module：在`path_to_your_cmake/share/cmake-<version>/Modules`目录下，寻找`Find<LibaryName>.cmake`
   - Config：主要通过`<LibraryName>Config.cmake` or `<lower-case-package-name>-config.cmake`这两个文件来引入我们需要的库

## vcpkg

## conna

TODO..

