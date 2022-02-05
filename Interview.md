# 面试记录

[TOC]

## 2021.10.11 15:00-16:00 

> 校招、后台开发工程师

- 算法：middle。有较强的debug能力。
- 自我介绍：热场
- 项目经历：面试者的项目**偏应用**，且特别标注唯一开发者，故要求：能讲清楚**需求**、**设计思路**、**技术选型**、**编码细节**。基础知识的问题全部穿插至项目中。
  - 长亭科技项目
    - 问题1：讲讲项目内容。要求：能给小白（我）讲清楚做了一件什么事情。描述清晰。
    - 问题2：优化数据处理模块。20s -> 200ms。有自我驱动发现问题并进行修复的能力，但思考过于单一，相当于只解决了一半。
      - 子问题a：怎么定位？
      - 子问题b：优化思路？
      - 子问题c：为了解决问题做了哪些实验？
    - 问题3：用Go开发简易型内存数据库。no
      - 子问题a：为什么没有选用Mysql或者Redis这种成熟的组件？
      - 子问题b：设计思路？
    - 问题4：golang基础考察
      - 子问题a：map不经初始化，直接进行存取操作会发生什么事情？get ok，set panic。no
      - 子问题b：map for range时，能否在遍历中进行delete操作？可以。no
      - 子问题d：map的扩容时机和扩容策略是什么？no
        - 时机：负载因子>6.5 & bucket数量过多
        - 策略：bucket数量 * 2
      - 子问题e：make和new的区别是什么
        - new：仅分配内存空间，返回指针，不进行初始化
        - make：分配空间&初始化，返回引用。适用于：slice、channel、map
      - 子问题f：对未初始化的channel进行读写，会发生什么？no
        - 读：deadlock/panic
        - 写：deadlock/panic
      - 子问题g：对已关闭的channel进行读写，会发生什么？no
        - 读：如果channel有值，返回value, true；如果无值，返回zero，false
        - 写：panic
      - 子问题h：goroutine并发控制的几种方式。写伪代码。50%
        - sync.WaitGroup
        - channel
  - 微信服务号统一推送平台
    - 问题1：rpc
      - 子问题a：如果是自己实现：
        - 讲讲实现rpc的设计思路
      - 子问题b：如果采用grpc：
        - rpc接口怎么转http调用？
        - 会不会写proto？常用哪些proto中的数据结构
      - 子问题c：如果采用其他rpc框架
        - 讲讲这个rpc框架怎么使用
  - 王者荣耀分析项目
    - 问题1：这个项目数据存储的特点是什么？读多写少？写多读少？
    - 问题2：为什么选用MongoDB？有考虑其他数据库吗？无思考。
    - 问题3：数据量过大、频繁更新导致性能降低。这个是怎么定位到的？
    - 问题4：用过哪些MongoDB的索引。普通索引、复合索引、TTL索引、唯一索引。
    - 问题5：讲讲MongoDB索引的结构。B-树。不了解。
      - 每个叶子结点都存储数据
      - 叶子结点之间无指针相邻
      - 做单一数据查询的时候，使用B树平均性能更好。但是，由于B树中各节点之间没有指针相邻，因此B树不适合做一些数据遍历操作
      - 非关系型数据库，单一查询较多。
      - https://www.cnblogs.com/rjzheng/p/12316685.html
    - 问题6：讲讲Mysql的索引结构。B+树。通过。
      - 数据只出现在叶子结点
      - 所有叶子结点都增加了一个链指针
      - 在做单一数据的查询上，其平均性能并不如B树。但是，B+树的叶子节点上有指针进行相连，因此在做数据遍历的时候，只需要对叶子节点进行遍历即可，这个特性使得B+树非常适合做范围查询
      - 关系型数据库，范围查询较多
      - https://www.cnblogs.com/rjzheng/p/12316685.html
    - 问题7：讲讲MongoDB -> PostgreSQL的数据迁移流程。通过。
    - 问题8：操作系统基础。不太行。
      - 讲讲Linux进程的内存模型。
        - 代码区
        - 已初始化全局变量、静态变量和常量区
        - 未初始化全局变量区
        - 堆区
        - 栈区
      - 进程间通信方式有哪些？ok
        - 共享内存。ok
        - 信号。ok
        - 管道。ok
        - 消息队列。ok
        - 信号量。
        - 套接字。ok
      - 线程间通信有哪些？ok
        - 全局变量 queue
        - 互斥量 mutex
        - 信号量 cond
        - 消息队列 （生产者、消费者模型）
      - 父进程派生子进程时，子进程从父进程哪里继承了那些东西？
        - 相当于就是把父进程PCB控制块复制了一遍，包括：页表、文件描述符等已打开的资源。
        - 对页表中的修改会做写时复制
      - 进程切换时，具体发生了哪些事情？
        - 挂起当前进程
        - 从待运行进程PCB中，加载新的页表
        - 切换内核栈及CPU上下文（寄存器），更新寄存器中的地址
      - 线程切换时，具体发生了那些事情？为什么线程切换快于进程切换？
        - 因为线程共享内存空间，所有无需加载页表。仅切换内核栈和CPU上下文（寄存器）
      - 协程切换时，发生了那些事情？为什么快于线程切换
        - 协程切换仅涉及用户态，无需陷入内核态。省去了通过信号中断进入内核态的时间
        - 协程切换仅切换上下文（寄存器）
    - 问题9：网络基础。不太行。
      - 问题1：TCP和UDP的区别，讲讲

        - 连接性：tcp是面向连接的协议；udp是无连接协议
        - 可靠性：tcp提供可靠服务；udp不提供可靠服务
        - 报文头部：tcp 20字节；udp 8字节
        - 吞吐量：TCP拥塞控制、流量控制、重传机制、滑动窗口等机制保证传输质量；UDP没有

      - 问题2：讲讲Tcp的三次握手和四次挥手。

        https://zhuanlan.zhihu.com/p/53374516

      - 问题3：讲讲timewait状态

        time-wait开始的时间为tcp四次挥手中主动关闭连接方发送完最后一次挥手，也就是ACK=1的信号结束后，主动关闭连接方所处的状态。然后time-wait的的持续时间为2MSL。

        原因1：为了保证客户端发送的最后一个ack报文段能够到达服务器。因为这最后一个ack确认包可能会丢失，然后服务器就会超时重传第三次挥手的fin信息报，然后客户端再重传一次第四次挥手的ack报文。如果没有这2msl，客户端发送完最后一个ack数据报后直接关闭连接，那么就接收不到服务器超时重传的fin信息报(**此处应该是客户端收到一个非法的报文段，而返回一个RST的数据报，表明拒绝此次通信，然后双方就产生异常，而不是收不到。**)，那么服务器就不能按正常步骤进入close状态。那么就会耗费服务器的资源。当网络中存在大量的timewait状态，那么服务器的压力可想而知。

        原因2：在第四次挥手后，经过2msl的时间足以让本次连接产生的所有报文段都从网络中消失，这样下一次新的连接中就肯定不会出现旧连接的报文段了。也就是防止我们上一篇文章 [为什么tcp是三次握手而不是两次握手？](https://zhuanlan.zhihu.com/p/51448333) 中说的：已经失效的连接请求报文段出现在本次连接中。如果没有的话就可能这样：这次连接一挥手完马上就结束了，没有timewait。这次连接中有个迷失在网络中的syn包，然后下次连接又马上开始，下个连接发送syn包，迷失的syn包忽然又到达了对面，所以对面可能同时收到或者不同时间收到请求连接的syn包，然后就出现问题了
  - 其他
    - 问题1：经历了这么多项目，有熟悉哪些设计模式吗？
    - 问题2：经历了这么多项目，解决过的那个问题给你的印象最深刻

## 2021.10.13 15:00-16:00

> 校招、后台开发工程师

- 算法：middle。考察编码能力

- 自我介绍：暖场

- 项目经历：从**需求**、**设计思路**、**技术选型**、**编码细节**四个角度讲讲。检验项目真实程度。

- 算法：右旋链表。35min没做出来..

- Java：还不错

- 操作系统

    - 用过Linux吗？
    - 用Linux做过什么？
    - 进程和线程有什么区别？
    - 进程切换时，发生了那些事情？
    - 线程切换时，发生了那些事情？
    - 讲讲对虚拟内存和物理内存的理解

- 网络：还不错

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

- 数据库

  - Redis：仅会掉接口，没有更深层次的理解
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - Mysql：
    - 讲讲Mysql的索引结构。为什么要用B+树

    - 什么是事务？满足ACID的一组操作
    - 讲讲ACID
    - I：讲讲隔离级别。串行、可重复读、读已提交、读未提交
    - 讲讲两阶段提交
    - 讲讲Mysql的索引结构。为什么要用B+树

- 设计模式

## 2021.10.29 11:00-12:00

- 算法：不同的二叉搜索树2，Middle

- 自我介绍

- 项目经历

- 语言基础。

  - 引用和指针有什么区别？
  - struct和class有什么区别？
  - New/delete malloc/free的区别？
  - 智能指针用过吗？讲讲shared_ptr、unique_ptr和weak_ptr？
  - 用过哪些STL容器。讲讲map和unordered_map以及使用场合
  - 虚函数有用过吗？虚函数和纯虚函数有什么区别？在一个继承体系中，有那些函数必须要声明为虚函数？为什么？
  - 强制类型转换有用过吗？挑两种熟悉的讲讲
  - 讲讲C++对象模型
  - 讲讲Linux C进程内存模型

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

- 数据库

  - Redis
  - Mysql

## 2021.10.29 14:00-15:00

- 自我介绍

- 算法：不同的二叉搜索树2，Middle

- 项目经历

- 语言基础：golang

  - 子问题a：map不经初始化，直接进行存取操作会发生什么事情？get ok，set panic。
  - 子问题b：map for range时，能否在遍历中进行delete操作？可以。
  - 子问题d：map的扩容时机和扩容策略是什么？
    - 时机：负载因子>6.5 & bucket数量过多
    - 策略：bucket数量 * 2
  - 子问题e：make和new的区别是什么
    - new：仅分配内存空间，返回指针，不进行初始化
    - make：分配空间&初始化，返回引用。适用于：slice、channel、map
  - 子问题f：对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 子问题g：对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。写伪代码。50%
    - sync.WaitGroup
    - channel

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log

## 2021.11.02 19:30-20:30

- 自我介绍

- 算法：不同的二叉搜索树

- 项目经历

- 语言基础：golang

  - 子问题b：map for range时，能否在遍历中进行delete操作？可以。
  - 子问题d：map的扩容时机和扩容策略是什么？
    - 时机：负载因子>6.5 & bucket数量过多
    - 策略：bucket数量 * 2
  - 子问题e：make和new的区别是什么
    - new：仅分配内存空间，返回指针，不进行初始化
    - make：分配空间&初始化，返回引用。适用于：slice、channel、map
  - 子问题f：对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 子问题g：对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 用过Mongo的哪些几种索引？
    - 讲讲mongo的索引结构，为什么要用B树？
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log

## 2021.11.04 11:00-12:00

- 自我介绍

- 算法：不同的二叉搜索树

- 项目经历

- 语言基础：使用小潘的Java面试

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

## 2021.11.05 14:00-15:00

- 自我介绍

- 算法：不同的二叉搜索树

- 语言基础：使用小潘的Java面试

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲UDP和TCP的区别

  - 讲讲三次握手、四次挥手

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

## 2021.11.09 14:00-15:00

- 自我介绍

- 算法：不同的二叉搜索树
- 项目经历
- 语言基础：golang
  - 子问题b：map for range时，能否在遍历中进行delete操作？可以。
  - 子问题d：map的扩容时机和扩容策略是什么？
    - 时机：负载因子>6.5 & bucket数量过多
    - 策略：bucket数量 * 2
  - 子问题e：make和new的区别是什么
    - new：仅分配内存空间，返回指针，不进行初始化
    - make：分配空间&初始化，返回引用。适用于：slice、channel、map
  - 子问题f：对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 子问题g：对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- 操作系统
  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换
- 开源组件
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log

## 2021.11.09 15:00-16:00

- 自我介绍

- 算法：不同的二叉搜索树
- 项目经历
- 语言基础：golang
  - 子问题b：map for range时，能否在遍历中进行delete操作？可以。
  - 子问题d：map的扩容时机和扩容策略是什么？
    - 时机：负载因子>6.5 & bucket数量过多
    - 策略：bucket数量 * 2
  - 子问题e：make和new的区别是什么
    - new：仅分配内存空间，返回指针，不进行初始化
    - make：分配空间&初始化，返回引用。适用于：slice、channel、map
  - 子问题f：对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 子问题g：对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- 操作系统
  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换
- 开源组件
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？

## 2021.11.15 16:00

- 自我介绍

- 算法：不同的二叉搜索树
- 项目经历
- 语言基础：golang
  - 子问题b：map for range时，能否在遍历中进行delete操作？可以。
  - 子问题d：map的扩容时机和扩容策略是什么？
    - 时机：负载因子>6.5 & bucket数量过多
    - 策略：bucket数量 * 2
  - 子问题e：make和new的区别是什么
    - new：仅分配内存空间，返回指针，不进行初始化
    - make：分配空间&初始化，返回引用。适用于：slice、channel、map
  - 子问题f：对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 子问题g：对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- 操作系统
  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换
- 开源组件
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？

## 2021.11.19 14:00-15:00

- 自我介绍

- 算法：找链表的环节点

- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？

- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - make和new有什么区别
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？

  - mongodb
    - 什么情况下适合用mongodb？
    - 用过mongodb哪些功能？聚合用过吗？讲讲
    - mongo分片用过吗？mongo分片时，数据是如何分布的？redis呢？
    - 3 master + 3 slave。master 1挂掉了，能否自动拉起来？问什么？
    - mongodb怎么做备份？有什么好的实践吗？
    - 怎么排查慢查询？
    - 副本集部署。怎么保证高可用？

## 2021.11.30 11:00-12:00

- 自我介绍

- 算法：不同的二叉搜索树

- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？

- 语言基础

  - 引用和指针有什么区别？
  - struct和class有什么区别？
  - New/delete malloc/free的区别？
  - 智能指针用过吗？讲讲shared_ptr、unique_ptr和weak_ptr？
  - 用过哪些STL容器。讲讲map和unordered_map以及使用场合
  - 虚函数有用过吗？虚函数和纯虚函数有什么区别？在一个继承体系中，有那些函数必须要声明为虚函数？为什么？
  - 强制类型转换有用过吗？挑两种熟悉的讲讲
  - 讲讲C++对象模型
  - 讲讲Linux C进程内存模型
  - 左值和右值的区别、move语义, 右值引用。 
  - 静态库，动态库 
  - 编译链接，详细说 

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- 网络

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

  - 讲讲你对socket编程中IO多路复用的理解。在golang中是怎么实现这个多路复用的

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log

## 2021.12.08 19:00

- 自我介绍

- 算法：不同的二叉搜索树

- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？

- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - make和new有什么区别
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组

- 操作系统

  - 用过Linux吗？
  - 用Linux做过什么？
  - 进程和线程有什么区别？
  - 进程切换时，发生了那些事情？
  - 线程切换时，发生了那些事情？
  - 讲讲对虚拟内存和物理内存的理解
  - 协程切换时，发生了那些事情？为什么快于线程切换

- web

  - 有没有用过http/2？讲讲和http/1.1的区别

  - 讲讲https的理解

    机密性、完整性，身份认证和不可否认

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log

## 2021.12.20 11:00

- 自我介绍
- 算法：不同的二叉搜索树
- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？
- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - make和new有什么区别
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- web
  - 讲讲你对http/1.1中长短连接的理解？长链接存在哪些问题？
  - http有几种文件传输的方式？
  - 了解http2嘛？和http/1.1相比有哪些特点？
- 开源组件
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？



## 2021.12.30 14:00

- 自我介绍
- 算法：寻找链表的入口环结点
- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？
- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - make和new有什么区别
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- web
  - 讲讲你对http/1.1中长短连接的理解？长链接存在哪些问题？
  - http有几种文件传输的方式？
  - 了解http2嘛？和http/1.1相比有哪些特点？
- 开源组件
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？

## 2022.01.10 15:00

## 2021.12.30 14:00

- 自我介绍
- 算法：寻找链表的入口环结点
- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？
- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组
- web
  - 讲讲你对http/1.1中长短连接的理解？长链接存在哪些问题？
  - http有几种文件传输的方式？
  - 了解http2嘛？和http/1.1相比有哪些特点？
- 开源组件
  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 用过mysql哪几种存储引擎？innodb有什么特点？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log


## 2022.01.14 11:00

- 自我介绍
- 算法：寻找链表的入口环结点
- 项目经历

  - 遇到哪些让人印象深刻的问题？怎么解决的？
- 语言基础

  - 讲讲数组、切片、字符串的区别。讲讲切片扩容的条件
  - 讲讲map的实现。扩容条件。是否是线程安全的
  - 讲讲对context的理解
  - 对未初始化的channel进行读写，会发生什么？
    - 读：deadlock/panic
    - 写：deadlock/panic
  - 对已关闭的channel进行读写，会发生什么？
    - 读：如果channel有值，返回value, true；如果无值，返回zero，false
    - 写：panic
  - 讲讲进程、线程、协程的区别
  - 讲讲GMP模型
  - 讲讲GC
  - 子问题h：goroutine并发控制的几种方式。
    - 使用8个协程随机打印1-100的数字
    - 使用8个协程顺序打印1-100的数组

- 网络

  - 讲讲你对http/1.1中长短连接的理解？长链接存在哪些问题？

  - 大文件下载

  - http2相对http1.x有哪些新特性

  - cookie和token有什么区别？

  - 讲讲timewait状态

  - 讲讲拥塞控制。

    https://zhuanlan.zhihu.com/p/37379780

    - 慢开始、拥塞避免
    - 快速重传
    - 快速恢复

  - 讲讲你对socket编程中IO多路复用的理解。在golang中是怎么实现这个多路复用的

- 开源组件

  - redis
    - 用过Redis吗？用Redis做过那些事情？
    - Redis有那些数据结构？讲讲
    - Redis集群的高可用是怎么做到的？讲讲Sentinel模式下，故障转移的步骤
    - Redis异常阻塞，讲讲你的排查思路
    - 有用过Cluster吗？讲讲Redis Cluster中数据分布的方式？有考虑过为什么使用这种分布方式吗？
  - kafka
    - 什么场景下使用kafka
    - 介绍一下kafka的体系架构，讲讲里面有哪些比较重要的概念
    - 什么是消费者组重平衡？什么情况下会触发？
    - kafka多副本了解吗，讲讲kafka多副本与redis sentinel多副本的区别？
    - 如何保障kafka多个消息顺序
    - kafka集群增加、或者减少broker，怎么保证集群负载均衡？
    - 什么情况下会出现消息丢失？怎么解决
    - 什么情况下会出现消息重复？怎么解决？
  - mysql
    - 讲讲使用Mysql时，遇到了哪些让你印象比较深刻的问题？
    - 用过mysql哪几种存储引擎？innodb有什么特点？
    - 讲讲Mysql事务的特性（ACID），重点讲讲I（隔离性）
    - 讲讲Mysql的索引结构。为什么要用B+树
    - 讲讲几种Mysql Log
      - bin log
      - redo log
      - undo log
