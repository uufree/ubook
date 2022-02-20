# Interview

[TOC]

## project



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

- 为什么要使用kafka？

  异步处理；服务解耦合；请求削峰；提升吞吐量

- kafka有哪些缺点？

  部署需要引入zk，有点麻烦；不支持延时消息；不支持多分区消息有序

- kafka有哪些特点？

  高吞吐（顺序读写文件、消息索引）；高拓展（topic partition、consumer group）；高可靠（通过写磁盘进行持久化、分区副本）

- kafka如何实现高吞吐？

  顺序读写文件；消息索引（在.index文件中存储.log文件的msg offset<->log file position，并将.index文件映射到内存中）；同步消息时采用零拷贝技术

- kafka的高可靠是怎么实现的？

  两个角度：消息会被顺序持久化至磁盘；采用分区副本（主分区读写，从分区只负责同步），使用acks参数控制写入动作

- kafka的高拓展是怎么实现的？

  两个角度：consumer group提升消费能力；多partition提升写入能力

- 讲讲kafka的体系结构

  Broker；Producer；Consumer Group、Consumer；Topic、Partition；

- kafka分区的目的？

  横向拓展消费能力；采用分区副本保证消息的高可靠

- 如何保证kafka消息有序？

  单一partation

- 讲讲kafka的消息一致性。

  从HW和LEO的机制交付来回答。

  - HW（High Water Mark）：ISR（In-Sync Replication）中，Offset最小的位置，相当于木桶的最短板。只有HW以上的消息才能被读取，控制消费者读取数据的位置。
  - LEO（Log End Offset）：ISR中，Offset最大的位置，相当于木桶的最长板，即主副本Offset。

- 怎么做kafka消息幂等？

  MongoDB中我用了唯一索引，ES不清楚；Mysql可以使用唯一索引；

- kafka在什么情况下会丢消息？

  acks=0时；producer recv&commit，还没来得及处理，就crash了；

- kafka在什么情况下会出现消息重复？

  producer采用auto commit，消费了但没有commit，然后出现了crash

- consumer和consumer group是什么关系？

  consumer属于consumer group，从一个或者多个topic中读消息；每个consumer至少能分到1个partition

- 讲讲consumer group rebalance

  消费者数量或者分区数量发生变化时，controller将分区重新分配给某个consumer的过程

- 讲讲partition rebalance。

  broker的数量发生变化时，为了保证每个broker的吞吐大致平衡，需要手动调整下partition在broker list中分不

- kafka分区数量可以减少吗？

  不行，对于减少的分区中的数据如何处理比较复杂

- 讲讲kafka的分区副本

  主副本负责读写，从副本负责同步数据。引入一些ISR、OSR，HW、LEO这样的概念。需要注意的是，分区副本的数量（包含主副本）不能超过broker

- zk在kafka中有什么作用？

  维护kafka节点状态，包括但不限于：controller选举、broker管理、配置管理等

- consumer长时间没有pull，会有什么问题吗？

  pull中存在心跳，长时间没有心跳，会导致被踢出consumer group，触发rebalance

- 如何减少数据丢失？

  acks=all；在producer中，处理完消息之后，再commit；加入幂等性处理重复的消息；禁止OSR中的副本竞选主副本

- offset有什么作用？

  kafka中的消息是不能修改的，使用offset能描述唯一的消息位置；使用offset和index文件建立索引，加速读取

- 如何修改kafka能接受的最大的消息大小？

  同时修改producer、broker、consumer中的配置项

- 如何估算kafka集群的大小？

  使用Kafka-consumer/producer-pref-test.sh这样的工具，结合消息大小测试下

- 如何调优kafka？

  根绝消息大小、延迟等需求，从broker、topic、consumer、produce以及JVMr这5个角度分开去看

- kafka为什么不支持读写分离？

  避免发生一致性问题

- kafka磁盘容量规划需要考虑哪些因素？

  消息大小、留存时间、是否压缩

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