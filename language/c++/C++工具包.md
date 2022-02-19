# C++工具包

[TOC]

## g++

- **-Wl:rpath**：添加运行时库路径。例如：`-Wl:rpath=/home/hello/lib`。运行时的寻找库路径为：`/home/hello/lib -> /usr/lib -> /usr/local/lib -> ...` 
- **-I(i)**：告诉预处理器从什么地方找头文件。例如：`-I/home/hello/include`

- **-L**：告诉链接器从什么地方找库。例如：`-L/home/hello/lib`
- **-l**：告诉链接器要引用什么库。例如：`-lpthread`
- **-g**：在编译的时候，产生调试信息。有助于gdb调试
- **-ggdb**：产生尽可能多的调试信息
- **-static**：禁止使用动态库
- **-shared**：尽可能使用动态库
- **-Wall**：尽可能的让g++发出警告信息
- **-werror**：将所有的warning转化为error，并在出现warn时，中止编译过程
- **-w**：关闭所有警告
- **-o**： 指定输出文件名称，例如：`g++ hello.c -o hello`
- **-O0**、**-O1**、**-O2**、**-O3**：指定优化级别
- **-fPIC**：生成位置无关的目标码，适用于打包动态库so时
- **-v**：显示详细的编译、汇编、连接命令
- **-std=c++11**：使用c++11标准

## gdb

> 使用gdb调试时，务必在编译时加入`-g`选项。否则编译出来的二进制文件是没有符号信息的

gdb有3种调试方式：

- **二进制**：`gdb filename`

- **core文件**：`gdb filename corename`

  > 想要生成core文件，必须先要打开`ulimit -c unlimited`

- **进程**：`gdb attach pid`

命令概览：

| 命令名称    | 命令缩写 | 命令说明                                         |
| ----------- | -------- | ------------------------------------------------ |
| run         | r        | 运行程序                                         |
| continue    | c        | 让暂停的程序继续运行                             |
| next        | n        | 运行到下一行，会进入函数内部                     |
| step        | s        | 运行到下一行，会进入函数内部                     |
| until       | u        | 运行到指定行停下来                               |
| finish      | fi       | 结束当前函数调用，到上一层函数调用处             |
| return      | return   | 结束当前函数调用到上一层函数调用处，并返回指定值 |
| print       | p        | 打印变量                                         |
| backtrace   | bt       | 查看线程堆栈                                     |
| frame       | f        | 切换到具体的堆栈                                 |
| thread      | thread   | 切换到指定线程                                   |
| break       | b        | 添加断点                                         |
| tbreak      | tb       | 添加临时断点                                     |
| delete      | del      | 删除断点                                         |
| enable      | enable   | 启用某个断点                                     |
| disable     | disable  | 删除某个断点                                     |
| watch       | watch    | 监视某个值是否发生变化                           |
| list        | l        | 显示源码                                         |
| info        | info     | 查看断点、线程等信息                             |
| ptype       | ptype    | 查看变量类型                                     |
| disassemble | dis      | 查看汇编代码                                     |
| set args    |          | 设置程序启动命令行参数                           |
| show args   |          | 查看设置的命令行参数                             |
|             |          |                                                  |
|             |          |                                                  |

指令详细用法如下：

```shell
# break指令
> break functionname
> break LineNo
> break filename:LineNo

# 查看当前断点
> info break

# 启用/禁用断点
> disable 1
> disable 2
> enable 3
> enable 4

# 删除所有断点
> delete

# 删除指定断点
> delete 1

# 打印堆栈
> bt

# 切换堆栈
> f 5
> f 4

# 打印代码（可切换到对应的堆栈后，再打印代码）
> list
> l

# 打印变量、变量地址
> p int
> p float
> p struct.port
> p struct.addr
> p &double

# 打印函数调用结果
> p func(10)		
> p func(20)

# 打印变量类型
> ptype struct

# 查看线程
> info thread

# 切换线程
> thread 1
> thread 2

# 执行完当前函数，并正常退出
> finish

# 执行完当前函数，并返回指定值
> return 10
> return 1000

# 运行到某一行停下来
> u 10
> u 20

# 在r之前，设置args
> set args ../redis.conf

# 展示运行参数
> show args

# 当一个变量发生变化时，gdb就会停下来
> watch int
> watch *p			// char* p
> watch buf			// char buf[128]

# 展示某些变量或者地址
> display int
> display float

# 展示当前有哪些展示的变量
> info display

# 删除展示的变量
> delete display 1
> delete display 2

# 当字符串过长时，print打印的不完整。使用以下方式可以处理：
> set print element 0
```

## valgrind

只会最基本的用法...

## nm

## ldd

## c++filt

## objdump

## string