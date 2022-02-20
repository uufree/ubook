# Interview

[TOC]

## components

### mysql

- 基本的sql语句，了解insert/replace的区别
- 了解怎么分析sql慢查询，explain结果的分析
- 了解事务的隔离级别及相应可能的问题
- 了解mvcc是如何解决幻读问题的
- 了解B树和B+树的区别以及为什么mysql会选择B+树作为存储结构
- 了解索引的基本原理，包括聚簇索引和联合索引
- 了解索引底层的存储结构是怎么样的
- binlog，redo log，undo log。。。redo log为什么快
  - 不可重复读跟幻读的区别，
  - 一致性是什么意思（满足约束条件，比如余额不能小于0），一致性读？
  - 索引失效有哪些情况，为什么会索引失效https://segmentfault.com/a/1190000021464570
  - undo log是怎么样的？是针对库的还是表的，是全局的吗？

### redis

- 了解redis和mysql之间更新的策略，如双删策略、先删再更新策略，并知道每个策略会有什么问题
  https://www.cnblogs.com/upnote/p/13185047.html
- 了解redis每种存储结构(字符串、hash、list、set、zset)的底层数据结构
  - 为什么用跳表不用二叉树
  - ziplist怎么查找某个key
  - zset如果是ziplist的时候我怎么找某个key
- 了解redis的缓存淘汰策略，如redis内存满了，这个时候怎么办，有什么参数可以控制淘汰策略
- 了解redis持久化方式aof和rdb，明白两者的区别和使用场景
- 了解如果要同时更新多个key时，有什么方法可以保证原子性？需要了解lua在redis的应用及相应的问题
- 了解redis集群的架构，进而了解主从的数据是怎么同步，数据分片为什么采用16556个slot平分，如果添加或删除节点会有什么问题，数据分片的算法和一致性hash算法有什么异同https://www.cnblogs.com/rjzheng/p/10360619.html
  https://zhuanlan.zhihu.com/p/129105249
- redis为什么快，redis的事件驱动有哪些
- redis热key处理
- 两个线程同时写一个key会怎么样？怎么保证顺序

### kafka

- kafka怎么保证快的？
- 怎么做kafka msg的幂等？

### mongodb

### nginx

## os

## network

- tcp/udp
  - 了解常见的tcp相关内核参数调整及调整位置
  - 了解滑动窗口、慢启动、拥塞算法https://www.cnblogs.com/xiaolincoding/p/12732052.html
  - 了解tcp超时机制
  - 了解tcp状态转移图
  - 了解如何使用udp搭建文件上传服务
  - epoll poll select的优缺点
  - epoll水平触发和边缘触发的优缺点及适用场景？go是用哪种？
  - accept是在三次握手之前还是之后？
  - 了解backlog参数的作用
  - tcp为什么可靠，为什么需要滑动窗口，回报时的ack是什么？
  - tcp三次握手最后一次失败了会怎么样？此时客户端发数据过去服务端会怎么处理？
- http
  - http访问的整个流程
    - http访问dns解析域名的流程
    - 如果我某个页面访问很卡我会怎么做？
      http2.0https://github.com/zqjflash/http2-protocol
  - 了解cookie和session的区别
  - 了解常见http错误码，如304、400、401、403、419、201、502、500
    https://zhuanlan.zhihu.com/p/45173862
  - 了解http2基本原理，包括帧类型等
  - http与https的区别，对称加密与非对称加密，数字签名。

## language

### c++

### golang

- 了解new和make的区别
- 了解三色标记回收算法https://www.jianshu.com/p/4c5a303af470
- 了解channel的底层实现，协程怎么调度的
- gmp模型，p优先从全局拿还是从别的p偷
- 了解sync.Map的底层实现
- hashmap的实现
- 了解sync.WaitGroup的使用场
- 了解go调度的原理，GMP模型，stealing work算法及原因，什么时候会触发调度
- 了解有哪些类型可以作为map的key
- 了解接口的底层原理
- 了解context包的使用场景
- 了解defer的作用
- 了解go的锁有哪几种，各自的适用场景是什么？
- 了解什么是逃逸分析

## design

- 设计一个短连接生成系统