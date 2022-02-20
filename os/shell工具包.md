# Shell工具包

[TOC]

## C++

### g++

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

### gdb

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

### valgrind

只会最基本的用法...

### nm

查看ELF文件中的符号表，使用方式如下：

```bash
# 查询动态符号
> nm -D libtest.so

# 查询所有类型的符号，包括：静态、动态、外部等
> nm -a libtest.so
```

### ldd

查看动态库依赖，使用方式如下：

```bash
> ldd test
libstdc++.so.6 => /usr/lib64/libstdc++.so.6 (0x00000039a7e00000)
libm.so.6 => /lib64/libm.so.6 (0x0000003996400000)
libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00000039a5600000)
libc.so.6 => /lib64/libc.so.6 (0x0000003995800000)
/lib64/ld-linux-x86-64.so.2 (0x0000003995400000)
```

### c++filt

将C++编译期重载的符号展开，使用方式如下：

```bash
> c++filt _ZTVN4Poco7Dynamic13VarHolderImplIbEE
vtable for Poco::Dynamic::VarHolderImpl<bool>
```

### objdump

查询可执行二进制文件的信息，使用方式如下：

```bash
# 查看本机目标结构（使用大端还是小端存储）
> objdump -i
```

### readelf

ELF文件共有三种：**.o文件**、**.so文件**、**可执行二进制文件**，共有以下几种section：

- .text section 里装载了可执行代码；
- .data section 里面装载了被初始化的数据；
- .bss section 里面装载了未被初始化的数据；
- 以 .rec 打头的 sections 里面装载了重定位条目；
- .symtab 或者 .dynsym section 里面装载了符号信息；
- .strtab 或者 .dynstr section 里面装载了字符串信息；

使用方式如下：

```bash
# 查询ELF可运行的架构平台
> readelf -h main| grep Machine

# 查询ELF文件是否加入了-g调试信息
> readelf -S main| grep debug
```

### strings

查询动态库版本版本，使用方式如下：

```bash
> strings libtbb.so | grep GLIBC
```

### pstack

跟踪进程堆栈，使用方式如下所示：

```bash
# 打印进程堆栈
> pstack 21356
#0  0x00000039958c5620 in __read_nocancel () from /lib64/libc.so.6
#1  0x000000000047dafe in rl_getc ()
#2  0x000000000047def6 in rl_read_key ()
#3  0x000000000046d0f5 in readline_internal_char ()
#4  0x000000000046d4e5 in readline ()
#5  0x00000000004213cf in ?? ()
#6  0x000000000041d685 in ?? ()
#7  0x000000000041e89e in ?? ()
#8  0x00000000004218dc in yyparse ()
#9  0x000000000041b507 in parse_command ()
#10 0x000000000041b5c6 in read_command ()
#11 0x000000000041b74e in reader_loop ()
#12 0x000000000041b2aa in main ()
```

### strace

跟踪进程中的系统调用，使用方式如下所示：

```bash
# 跟踪系统调用
> strace cat /dev/null
execve("/bin/cat", ["cat", "/dev/null"], [/* 22 vars */]) = 0
brk(0)                                  = 0xab1000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f29379a7000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)

# 跟踪指定进程的系统调用
> strace -p 23451
```

## 进程

### ps

列出系统中当前运行的那些进程，使用方式如下：

```bash
# 显示所有进程信息
> pa -A

# 显示指定用户的进程信息
> ps -u root

# 显示所有进程信息，连同命令行
> ps -ef

# ps 与grep 组合使用，查找特定进程
> ps -ef | grep nginx

# 列出目前所有的正在内存中的程序
> ps aux
```

### top

实时展示系统中各个进程的资源占用情况，使用方式如下：

```bash
# 常用的交互指令如下：
> top
q: quit，退出
<Space>: 立刻刷新
s: 设置时间刷新间隔
c: 展示完整的启动命令
P: 按照CPU使用率排行 
T: 按照TIME时间排行
M: 按照MEMORY排行
u: 指定显示用户进程
1: 展示多核CPU中每个核的使用情况

# 显示指定的进程
> top -p 21345
```

## 内存

### free

查看系统的可用内存，使用方式如下：

```bash
> free
        	  total       used       free     shared    buffers     cached
Mem:       8175320    6159248    2016072          0     310208    5243680
-/+ buffers/cache:     605360    7569960
Swap:      6881272      16196    6865076
```

### vmstat

监视操作系统中内存的使用情况，使用方式如下：

```bash
# 每5s打印一次，共打印10次
> vmstat 5 10
procs -----------memory---------- ---swap-- -----io---- --system-- -----cpu-----
r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
6  0      0 27900472 204216 28188356    0    0     0     9    1    2 11 14 75  0  0
9  0      0 27900380 204228 28188360    0    0     0    13 33312 126221 22 20 58  0  0
2  0      0 27900340 204240 28188364    0    0     0    10 32755 125566 22 20 58  0  0

# 字段释义：
swpd: 使用虚拟内存大小
free: 可用内存大小
buff: 用作缓冲的内存大小（内存 <-> 磁盘）
cache: 用作缓存的内存大小（cpu <-> 内存）
```

## 文件系统

### file

查看文件类型，使用方式如下：

```bash
> file test.zip
c++ primer.md: C++ source text, UTF-8 Unicode text
```

### lsof

查看当前文件资源依赖关系，使用方式如下：

```bash
# 查找某个文件相关的进程
> lsof /bin/bash
COMMAND     PID USER  FD   TYPE DEVICE SIZE/OFF    NODE NAME
mysqld_sa  2169 root txt    REG  253,0   938736 4587562 /bin/bash
ksmtuned   2334 root txt    REG  253,0   938736 4587562 /bin/bash
bash      20121 root txt    REG  253,0   938736 4587562 /bin/bash

# 列出某个用户打开的文件信息
> lsof -u username

# 列出某个程序进程所打开的文件信息
> lsof -c mysql

# 列出某个用户以及某个进程所打开的文件信息
> lsof  -u test -c mysql

# 通过某个进程号显示该进程打开的文件
> lsof -p 11968

# 列出所有的网络连接
> lsof -i 

# 列出所有tcp网络连接信息
> lsof -i tcp

# 列出谁在使用某个端口
> lsof -i :3306

# 列出某个用户的所有活跃的网络端口
> lsof -a -u test -i

# 列出被进程号为1234的进程所打开的所有IPV4 network files
> lsof -i 4 -a -p 1234
```

### iotop

动态每个进程的磁盘使用情况，使用方式如下：

```bash
# 展示所有进程的磁盘使用情况
> iotop

# 展示某个进程的磁盘使用情况
> iotop -p 21345

# 展示某个用户下面所有进程的磁盘使用情况
> iotop -u root
```

### iostat

动态监视磁盘的活动情况，使用方式如下：

```bash
# 每5s打印一次，共打印10次
> iostat 5 10
Linux 2.6.32-279.el6.x86_64 (colin)   07/16/2014      _x86_64_        (4 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
10.81    0.00   14.11    0.18    0.00   74.90

Device:            tps   Blk_read/s   Blk_wrtn/s   Blk_read   Blk_wrtn
sda               1.95         1.48        70.88    9145160  437100644
dm-0              3.08         0.55        24.34    3392770  150087080
dm-1              5.83         0.93        46.49    5714522  286724168
dm-2              0.01         0.00         0.05      23930     289288

# 以MB为单位进行统计
> iostat -m 5 10

# 字段释义：
tps：该设备每秒的传输次数
kB_read/s：每秒从设备读取的数据量；
kB_wrtn/s：每秒向设备写入的数据量；
kB_read：读取的总数据量；
kB_wrtn：写入的总数量数据量；
```

## 代码管理

### git

git rebase

## 网络

### netstat

### ping

### telnet

### ifstat

查询每块网卡的流量，使用方式如下：

```bash
> ifstat
```

### iftop

查询某块网卡上，某个地址的流量统计：

```bash
> iftop
```

### ifconfig

### route

### iptables

## 通用命令

### uname

```bash
# 查看系统及内核使用版本
> uname -a
```

### hostname

```bash
# 查看系统名称
> hostname
```

### env

```bash
# 查看环境变量
> env
```

### source

```bash
# 使配置文件生效
> source .bashrc
```

### wget

### scp

### crontab
