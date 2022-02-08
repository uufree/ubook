# Linux操作系统

[TOC]

系统初始化篇章存在部分有用的内容：**权限及系统调用**

## 概述

<img src="assets/下载.jpeg" alt="下载" style="zoom: 50%;" />

## 系统初始化

### 硬件概述

计算机工作模式图：

![下载 (1)](assets/下载 (1).jpeg)

CPU包含3个部分：

- **运算单元**：只管运算，例如加法、位移等
- **数据单元**：CPU内部缓存和寄存器组，空间小，运算速度快，可以暂时存放数据和运算结果
- **控制单元**：统一的指挥中心，可以获得下一条指令，然后执行这条指令。这个指令会指导运算单元取出数据单元中的某几个数据，计算出个结果，然后放在数据单元的某个地方。

整个流程如下图所示：

![下载 (2)](assets/下载 (2).jpeg)

x86控制器组件如下图所示：

![下载 (3)](assets/下载 (3).jpeg)

- **数据单元**

  8个16位的通用寄存器，分别为：AX、BX、CX、DX、SP、BP、SI、DI。主要用于**暂存数据**。

- **控制单元**

  - IP：**指令指针寄存器**，存放下一条指令在内存中的地址。CPU会不断从这个寄存器指向的指令地址加载指令，并将指令存放至指令队列中
  - CS：代码段起始地址寄存器。**代码段Offset放在IP寄存器中**
  - DS：数据段起始地址寄存器。**数据段Offset放在通用寄存器中**
  - SS：栈寄存器

拓展到32位之后，寄存器的布局如下所示：

![下载 (4)](assets/下载 (4).jpeg)

### 系统加载

<img src="assets/下载 (5).jpeg" alt="下载 (5)" style="zoom:50%;" />

### 系统初始化

![下载 (6)](assets/下载 (6).jpeg)

> 初始化办公大厅的意思是：初始化系统中断表，包括硬中断和软中断。
>
> 硬中断：初始化各种硬件设备
>
> 软中断：初始化内核系统调用

### 系统调用

x86提供了分层的权限机制，如下图所示：

![下载 (7)](assets/下载 (7).jpeg)

内核代码指令存放在Ring 0区域，被称作**内核态**；用户代码指令放在Ring 3区域，被称作**用户态**。如果在用户态通过系统调用去访问内核态的资源，会发生什么事情呢？简要的流程如下图所示：

![下载 (8)](assets/下载 (8).jpeg)

#### 32位系统调用

![下载 (9)](assets/下载 (9).jpeg)

> `ENTER_KERNEL int $0x80`意思是触发一个软中断，通过它进入内核

#### 64位系统调用

![下载 (10)](assets/下载 (10).jpeg)

> 64位通过**syscall指令**进入内核态

## 进程管理

> 本章节的进程特指C语言开发的程序

### 进程加载

Linux下的二进制程序统称为**ELF**（Executeable and Linkable Format，可执行与可链接格式）文件。编译过程如下图所示：

![](assets/下载 (11).jpeg)

整个编译过程分为以下几个步骤：

1. **预处理**：在c文件中嵌入文件；展开宏定义等

2. 编译成.o文件，又称**可重定位文件**。格式如下所示：

   > 可重定位的意思是说：这些编译完成的Section在加载到内存中的时候，是可以修改加载位置的

   - **.text**：放编译好的二进制可执行代码
   - **.data**：已经初始化好的全局变量
   - **.rodata**：只读数据，例如字符串常量、const 的变量
   - **.bss**：未初始化全局变量，运行时会置 0
   - **.symtab**：符号表，记录的则是函数和变量
   - **.strtab**：字符串表、字符串常量和变量名
   - **.rel.text**：代码段重定位信息
   - **.rel.data**：数据段重定位信息

   ![下载 (12)](assets/下载 (12).jpeg)

3. **静态链接 -> 可执行文件**

   静态链接库.a特指一组.o文件的归档集合。**在进行静态链接时，会从.a文件中提取出.o文件，并将.o文件中的Section合并至最终的可执行文件。**通过静态链接产生的二进制文件，可以独立执行。

4. **动态链接 -> 可执行文件**

   动态链接库.so时一组.o文件中Section的重新组合，并在链接的过程中新增了两个Section：

   - **.plt**：过程链接表。存放寻找真正函数指令地址的Wrapper代码
   - **.got.plt**：全局偏移量表。存放函数指令地址。

5. **可执行文件**

   ![下载 (13)](assets/下载 (13).jpeg)

完整的加载流程如下图所示：

![下载 (14)](assets/下载 (14).jpeg)

### 进程管理

进程在内核中的结构如下所示：

![下载 (17)](assets/下载 (17).jpeg)

详细的信息如下：

#### 任务ID

- 主线程：pid、tgid、group_leader都是它自己
- 子线程：pid是自己，tgid、group_leader是主线程

#### 进程状态

有以下状态分类：

- **TASK_RUNNING**：表示进程在时刻准备运行的状态。如果抢到了时间片，那就运行；如果没有抢到，那就等待运行
- **TASK_INTERRUPTIBLE**：表示进程处于可中断睡眠状态。内核调用阻塞可以被信号中断。
- **TASK_UNINTERRUPTIBLE**：表示进程处于不可中断睡眠状态。内核调用阻塞不可以被信号中断。
- **TASK_KILLABLE**：表示进程处于不可中断睡眠状态，但是在阻塞期间可以相应KILL信号。
- **TASK_STOPPED**：程接收到 SIGSTOP、SIGTTIN、SIGTSTP 或者 SIGTTOU 信号之后进入该状态
- **TASK_TRACED**：进程执行被调试程序所停止。当一个进程被另外的进程所监视，每一个信号都会让进程进入该状态
- **EXIT_ZOMBIE**：进程执行结束，但是它的父进程还没有调用wait()
- **EXIT_DEAD**：进程彻底结束

状态转化如下图所示：

![下载 (18)](assets/下载 (18).jpeg)

#### 运行统计信息

记录进程在用户态和内核态消耗的时间、上下文切换的次数等。

```c
u64				utime;// 用户态消耗的 CPU 时间
u64				stime;// 内核态消耗的 CPU 时间
unsigned long			nvcsw;// 自愿 (voluntary) 上下文切换计数
unsigned long			nivcsw;// 非自愿 (involuntary) 上下文切换计数
u64				start_time;// 进程启动时间，不包含睡眠时间
u64				real_start_time;// 进程启动时间，包含睡眠时间
```

#### 亲缘关系

**任何一个进程都有父进程**。且子进程结束时，必须向它的父进程发送信号。进程树如下图所示：

![下载 (19)](assets/下载 (19).jpeg)

#### 权限

主要用于控制进程的文件访问权限。

#### 内存管理

详见内存管理

#### 文件系统

详见文件系统

#### 信号处理

详见进程IPC

#### 内核栈

- `void *stack`：进入内核态时，用这个栈保存当前CPU中的寄存器信息
- `struct thread_info thread_info`：退出内核态时，用thread_info获取PCB控制块信息，从而获取之前保存好的寄存器信息

### 进程调度

#### 调度算法

Linux采用**完全公平调度算法**（CFS）。CPU会提供一个时钟，过一段时间就触发一个时钟中断。就像咱们的表滴答一下，这个我们叫 Tick。CFS 会为每一个进程安排一个虚拟运行时间vruntime。如果一个进程在运行，随着时间的增长，也就是一个个tick的到来，进程的vruntime将不断增大。没有得到执行的进程vruntime不变。显然，那些vruntime少的，原来受到了不公平的对待，需要给它补上，所以会优先运行这样的进程。

vruntime的计算方式如下所示：

```
虚拟运行时间 vruntime += 实际运行时间 delta_exec * NICE_0_LOAD/ 权重
```

**CFS基于红黑树实现，每次调度时，会优先选择vruntime最小的节点**。如下图所示：

![下载 (20)](assets/下载 (20).jpeg)

#### 调度方式

- **主动调度**：陷入耗时的IO操作时，会主动让出CPU。整个过程分为以下两步：选取下一个被调度的进程；进行上下文切换
- **抢占式调度**：在内核处理时钟中断时，会发现一个进程运行的时间过长。此时，就会发生抢占式调度。但是抢占的时机只能是以下几种情况：
  - 用户态：系统调用返回用户态
  - 用户态：中断返回用户态
  - 内核态：有的系统调用不能被抢占，会主动打开preempt_disable。当再次启用preempt_enable时，就可以被抢占
  - 内核态：中断返回内核态

### Fork

- 复制PCB
  1. Alloc New PCB
  2. Alloc NewPCB.stack
  3. 调用memcpy拷贝Old PCB -> New PCB
  4. Alloc NewPCB.thread_info
  5. 更新New PCB中的信息，主要分为以下几个角度：
     - 重置运行时统计信息
     - 访问权限与父进程一致
     - 重置调度信息vruntime等，但是优先级等还是和父进程一致
     - 完全拷贝文件系统资源
     - 完全拷贝信号处理方式
     - 设置pid、tid等信息
     - 完全拷贝父进程的页表。运行时，采用写时复制进行替换
- 唤醒子进程
  1. 子进程尝试能否抢占父进程或者其他进程

### 线程

进程是资源分配的最小单位；线程是CPU调度的最小单位。

#### 创建

![下载 (15)](assets/下载 (15).jpeg)

#### 数据

![下载 (16)](assets/下载 (16).jpeg)

- 线程栈：默认8MB，可通过`ulimit -a`查看
- 共享变量：共享进程数据。访问时，需要用锁保护
- 线程私有数据：一旦被创建，所有线程都可以访问它。但各线程可根据自己的需要往key中填入不同的值，这就**相当于提供了一个同名而不同值的全局变量**。

#### 线程同步

pthread生产者、消费者伪代码：

```c++
#include <stdio.h>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <string.h>

using namespace std;

const int BUFFER_SIZE = 1500;

typedef struct Packet_s{
    char buffer[BUFFER_SIZE];
    int len;
}Packet;

queue<Packet> packetQueue;
const int LIMIT_SIZE = 1500;
// 缓冲区相关数据结构
pthread_mutex_t lock; /* 互斥体lock 用于对缓冲区的互斥操作 */
// int readpos, writepos; /* 读写指针*/
pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
pthread_cond_t notfull; /* 缓冲区未满的条件变量 */    


// 生产者线程处理函数
void *produce(void *data){
    for(int i=0; i<100000; i++){
        Packet tmp;
        memcpy(tmp.buffer, &i, sizeof(i));
        tmp.len = sizeof(i);

        // 假定队列的大小限制为1500，到达这个值认为队列满了，等待
        // 消费者取出后，再向队列里push
        pthread_mutex_lock(&lock);
        while(packetQueue.size() == LIMIT_SIZE){
            pthread_cond_wait(&notfull, &lock);
        }

        packetQueue.push(tmp);
        pthread_mutex_unlock(&lock); // 给互斥体变量解除锁

        pthread_cond_signal(&notempty);
        // 每一次push后设置notempty条件变量
        cout << "Producer[" << i << "]" << endl;
    }
    return NULL;
}

// 消费者线程处理函数
void *consume(void *data){
    while(1){
        pthread_mutex_lock(&lock);
        while(packetQueue.size() == 0){
            pthread_cond_wait(&notempty, &lock);
        }
        Packet packet = packetQueue.front();
        packetQueue.pop();
        pthread_mutex_unlock(&lock);    

        pthread_cond_signal(&notfull);
        int *data = (int *)packet.buffer;
        cout << "Consumer[" << *data << "]" << endl;
        if(*data == 99999)
            break;
    }   
    return NULL;    
}

int main(){
    //初始化条件变量和互斥锁
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&notempty, NULL);
    pthread_cond_init(&notfull, NULL);

    pthread_t producer, consumer;

    /* 创建生产者和消费者线程*/
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    /* 等待线程退出 */
    pthread_join(producer, NULL);
    cout << "producer exit!" << endl;
    pthread_join(consumer, NULL);
    cout << "consumer exit!" << endl; 

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notempty);
    pthread_cond_destroy(&notfull);
    return 0;
}
```

## 内存管理

### 概述

物理地址对进程而言是不可见的，任何进程都不能直接访问物理地址。操作系统会给进程分配一个虚拟地址。所有进程看到的这个地址都是一样的，里面的内存都是从 0 开始编号。在程序里面，指令写入的地址是虚拟地址。例如，位置为 10M 的内存区域，操作系统会提供一种机制，将不同进程的虚拟地址和不同内存的物理地址映射起来。当程序要访问虚拟地址的时候，由内核的数据结构进行转换，转换成不同的物理地址，这样不同的进程运行的时候，写入的是不同的物理地址，这样就不会冲突了。

### 虚拟内存

进程内存布局如下图所示：

![下载 (21)](assets/下载 (21).jpeg)

> 所有用户态进程共享同一个内核态空间

用户态内存与内核态内存的划分比例如下：

![下载 (25)](assets/下载 (25).jpeg)

用户态虚拟内存空间布局：

![下载 (26)](assets/下载 (26).jpeg)

内核态虚拟内存空间布局：

![下载 (27)](assets/下载 (27).jpeg)

### 物理内存

物理内存访问模型：

![下载 (28)](assets/下载 (28).jpeg)

物理页分配方式：

![下载 (29)](assets/下载 (29).jpeg)

> 使用伙伴系统进行页管理；
>
> 使用Slab进行小对象管理；

物理页管理方式总结：

- 物理内存分 NUMA 节点，分别进行管理；
- 每个 NUMA 节点分成多个内存区域；
- 每个内存区域分成多个物理页面；
- 伙伴系统将多个连续的页面作为一个大的内存块分配给上层；
- kswapd 负责物理页面的换入换出；
- Slab Allocator 将从伙伴系统申请的大内存块切成小块，分配给其他系统。

![下载 (30)](assets/下载 (30).jpeg)

### 内存映射

内存映射有2种方式：**分段**、**分页**，我们重点讨论分页这种映射方式。

对于物理内存，操作系统把它分成一块一块大小相同的页，这样更方便管理，例如有的内存页面长时间不用了，可以暂时写到硬盘上，称为**换出**。一旦需要的时候，再加载进来，叫作**换入**。这样可以扩大可用物理内存的大小，提高物理内存的利用率。

这个换入和换出都是以页为单位的。页面的大小一般为 4KB。为了能够定位和访问每个页，需要有个**页表**，保存每个页的起始地址，再加上在页内的偏移量，组成线性地址，就能对于内存中的每个位置进行访问了。如下图所示：

![下载 (22)](assets/下载 (22).jpeg)

虚拟地址分为两部分，**页号**和**页内偏移**。页号作为页表的索引，页表包含物理页每页所在物理内存的基地址。这个基地址与页内偏移的组合就形成了物理内存地址。

考虑到页表的内存占用，在32位环境下，**页表是分层的**，如下图所示：

> 页表内存占用计算：32 位环境下，虚拟地址空间共 4GB。如果分成 4KB 一个页，那就是 1M 个页。每个页表项需要 4 个字节来存储，那么整个 4GB 空间的映射就需要 4MB 的内存来存储映射表。如果每个进程都有自己的映射表，100 个进程就需要 400MB 的内存。对于内核来讲，有点大了。

![下载 (23)](assets/下载 (23).jpeg)

64位环境的页表实际上会分为4层，如下图所示：

![下载 (24)](assets/下载 (24).jpeg)

## 文件系统

### 概述

![下载](assets/下载.png)

### 硬盘文件系统

#### 硬盘介质

每个盘面有多个磁道；每个磁道有多个扇区；**每个扇区的大小是512字节；多个扇区组成一个Block，大小为4KB**

![下载 (31)](assets/下载 (31).jpeg)

#### Inode

每个文件在文件系统的表现都是一个**Inode**，结构如下所示：

```c
struct ext4_inode {
	__le16	i_mode;		/* File mode */
	__le16	i_uid;		/* Low 16 bits of Owner Uid */
	__le32	i_size_lo;	/* Size in bytes */
	__le32	i_atime;	/* Access time */
	__le32	i_ctime;	/* Inode Change time */
	__le32	i_mtime;	/* Modification time */
	__le32	i_dtime;	/* Deletion Time */
	__le16	i_gid;		/* Low 16 bits of Group Id */
	__le16	i_links_count;	/* Links count */
	__le32	i_blocks_lo;	/* Blocks count */
	__le32	i_flags;	/* File flags */
......
	__le32	i_block[EXT4_N_BLOCKS];/* Pointers to blocks */
	__le32	i_generation;	/* File version (for NFS) */
	__le32	i_file_acl_lo;	/* File ACL */
	__le32	i_size_high;
......
};
```

Inode中的各种字段描述文件的元信息。文件的具体内容保存在**inode.i_block**字段中，组织形式如下图所示：

- Ext2、Ext3

  考虑到文件大小，**i_block[12], i_block[13], i_block[14]可以展开为层级的形式，从而存放更大的文件**

  ![下载 (32)](assets/下载 (32).jpeg)

- Ext4

  对于大文件来讲，如果按照Ext3的组织形式，需要访问多个Block才能获取完整的文件信息。因此，Ext4做了一些修改：inode.i_block[n]中存放一个**Exents**树结构，如下图所示：

  ![下载 (33)](assets/下载 (33).jpeg)

  **ext4_extent_header** 用来描述某个节点。结构如下所示：

  ```
  struct ext4_extent_header {
  	__le16	eh_magic;	/* probably will support different formats */
  	__le16	eh_entries;	/* number of valid entries */
  	__le16	eh_max;		/* capacity of store in entries */
  	__le16	eh_depth;	/* has tree real underlying blocks? */
  	__le32	eh_generation;	/* generation of the tree */
  };
  ```

  eh_entries 表示这个节点里面有多少项。这里的项分两种，如果是叶子节点，这一项会直接指向硬盘上的连续块的地址，我们称为数据节点 ext4_extent；如果是分支节点，这一项会指向下一层的分支节点或者叶子节点，我们称为索引节点 ext4_extent_idx。这两种类型的项的大小都是 12 个 byte。

#### Fs整体布局

从整个文件系统的角度看，Inode是这样分布的：

![下载 (34)](assets/下载 (34).jpeg)**超级块和块组描述符非常重要，所以每个块组中都有备份**

#### 目录

![下载 (35)](assets/下载 (35).jpeg)

#### 软硬链接

![下载 (36)](assets/下载 (36).jpeg)

硬链接共用同一个Inode；软链接会重新创建一个New Inode，New Inode中的内容指向Old Inode

### 虚拟文件系统

VFS主要是为了适配各种不同的硬盘文件系统，为OS层提供统一的接口。

![下载 (37)](assets/下载 (37).jpeg)

### 文件缓存

![下载 (1)](assets/下载 (1).png)

## 输入输出系统

CPU 并不直接和设备打交道，它们中间有一个叫作**设备控制器**（Device Control Unit）的组件，例如硬盘有磁盘控制器、USB 有 USB 控制器、显示器有视频控制器等。这些控制器就像代理商一样，它们知道如何应对硬盘、鼠标、键盘、显示器的行为。输入输出设备我们大致可以分为两类：

- 块设备：
- 字符设备：

## 进程通信

## 网络协议栈

## 虚拟化

## 容器化