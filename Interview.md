# Interview

[TOC]

## project

### ocr

### radar

### media

## components

### mysql

1. `DISTINCT`和`GROUP BY`有什么区别？
2. `INSETR`和`REPLACE`有什么区别？
3. `DELETE`、`DROP`、`TRUNCATE`有什么区别？
4. `INNER JOIN`和`LEFT JOIN`有什么区别？
5. 用过哪些Mysql函数？
6. LIKE语句中的%和_有什么区别？
7. BLOB和TEXT有什么区别？
8. 外键是什么？什么时候需要用？
9. 讲讲一条SQL语句的执行过程
10. 删除操作会减少数据库文件的大小吗？怎么减小数据库文件的大小？
11. Mysql长连接会有什么问题？怎么解决？
12. 查询缓存是什么？有什么使用场景？
13. 讲讲Mysql优化器的作用
14. Mysql是怎么选择索引的？
15. Mysql是怎么确认扫描行数的？
16. 哪些情况会导致索引失效？怎么解决？

17. 讲讲联表的过程
18. 讲讲排序。有什么调优思路？
19. Mysql的缓冲池是怎么防止热点数据被淘汰的？
20. MyISAM和InnoDB有什么区别？
21. MyISAM和InnoDB执行`select count(*)`谁更快？
22. InnoDB的事务是如何实现的？
23. InnoDB有哪些特点？
24. Change Buffer是什么？有什么作用？适用于什么场景？
25. Change Buffer Merge的触发时机有哪些？触发时会做什么事情？
26. 什么时候会执行脏页刷新？脏页刷新时，会修改redo log吗？
27. 主键索引和非主键索引有什么区别？
28. Mysql为什么选择B+树做索引？B树和B+树有什么区别？
29. 讲讲对联合索引的理解
30. 讲讲对Mysql事务的理解？
31. 怎么理解事务的隔离级别？每个隔离级别会存在什么样的问题？
32. Mysql一致性是什么意思？一致性读呢？
33. Mysql是怎么实现读已提交和可重复读隔离级别的？MVCC是怎么实现的？
34. Mysql是怎么解决幻读的？

35. Mysql有哪些锁类型？分别有什么用？
36. 如果在某行上加锁，什么时候提交？
37. Mysql死锁是什么？
38. 怎么用Mysql实现悲观锁和乐观锁？
39. BinLog、Undo Log、Redo Log分别是什么？详细讲讲
40. BinLog有哪些格式？有什么使用建议？
41. Undo Log是针对库还是针对表的？
42. 可以只使用BinLog吗？
43. 可以只使用RedoLog吗？
44. 为什么需要两阶段提交？
45. 如何安全的修改表字段呢？
46. 怎么安全的复制表？
47. 怎么安全的备份库？
48. CPU的利用率很高，但是每秒处理的事务量太少。有什么解决思路？
49. 长事务有什么影响呢？
50. 如果数据库的命中率骤降并且更新语句执行的非常慢，有什么解决思路？
51. Update、Insert、Delete时，RedoLog和Change Buffer分别有什么作用？
52. 怎么排查Mysql慢查询？
53. 怎么排查Mysql阻塞？
54. Select语句是否启用了事务？
55. 如何处理QPS暴增？
56. 若Mysql IO出现瓶颈，怎么进行调优？
57. 为什么 binlog cache 是每个线程自己维护的，而 redo log buffer 是全局共用的？
58. 事务执行期间，还没到提交阶段，如果发生Crash的话，redo log肯定丢了，这会不会导致主备不一致呢？
59. 如何处理误删数据？
60. 主键ID为什么是自增的？
61. 自增主键ID为什么是不连续的？
62. 自增ID用完了怎么办？
63. 当自增ID到达上限是，继续插入会怎么样呢？
64. Mysql的主从复制原理
65. 怎么让Mysql支持表情？
66. Mysql的行锁是怎么实现的？
67. 什么是事务？
68. 为什么要拆分大的DELETE操作？
69. char和varchar有什么区别？
70. 锁有什么优化思路？

### redis

1. redis缓存满了怎么办？

2. redis为什么这么快？

3. 讲讲redis的基础数据结构及底层结构

4. 为什么zset使用跳表不使用二叉树？

5. ziplist怎么查找某个key？zset如果是ziplist，怎么查找某个key？

6. 什么时候需要用bloom filter？讲讲bloom filter原理？有什么缺点？怎么解决这个缺点？

7. cell是什么？什么时候会用？和nginx的限流相比有什么区别？

8. 讲讲hash扩容的过程

9. redis是怎么删除数据的？有哪些删除数据的命令？不同的删除命令有什么区别？

10. 为什么需要持久化机制？

11. 讲讲aof和rdb。有什么区别？使用场景有哪些？

12. 讲讲aof rewrite

13. 如何选择合适的持久化方式？

14. 如果要同时更新多个key，怎么保证原子性？lua script在redis中有什么问题？

15. 两个线程同时写一个key会怎么样？怎么保证顺序性？

16. sentinel和cluster分别解决了什么问题？

17. 讲讲redis sentinel、redis cluster的选举过程

18. 主从副本的数据同步方式？

19. redis cluster是怎么做数据分布的？为什么选用slots？如果用一致性哈希有什么问题？

20. 讲讲redis、mysql数据同步策略

21. 有人说Redis只适合做缓存，不适合做数据库，你怎么看？

22. 讲讲缓存穿透、缓存击穿、缓存雪崩。有什么好的处理建议？

23. 讲讲用redis做分布式锁需要注意哪些事项？ 

24. 怎么发现redis大key？大Key可能引发哪些问题？有哪些处理建议？

25. 怎么发现redis的热点key？热点Key可能引发哪些问题？有哪些处理建议？

26. 怎么排查redis的慢操作？怎么处理？

27. 主从部署+读写分离，可能遇到哪些问题？怎么解决？

28. 在redis的使用上有什么建议？

29. redis事务有什么问题吗？


### kafka

1. 为什么要使用kafka？

2. kafka有哪些缺点？

3. kafka有哪些特点？

4. kafka如何实现高吞吐？

5. kafka的高可靠是怎么实现的？

6. kafka的高拓展是怎么实现的？

7. 讲讲kafka的体系结构

8. kafka分区的目的？

9. 如何保证kafka消息有序？

10. 讲讲kafka的消息一致性。

11. 怎么做kafka消息幂等？

12. kafka在什么情况下会丢消息？

13. kafka在什么情况下会出现消息重复？

14. consumer和consumer group是什么关系？

15. 讲讲consumer group rebalance

16. 讲讲partition rebalance。

17. kafka分区数量可以减少吗？

18. 讲讲kafka的分区副本

19. zk在kafka中有什么作用？

20. consumer长时间没有pull，会有什么问题吗？

21. 如何减少数据丢失？

22. offset有什么作用？

23. 如何修改kafka能接受的最大的消息大小？

24. 如何估算kafka集群的大小？

25. 如何调优kafka？
26. kafka为什么不支持读写分离？

27. kafka磁盘容量规划需要考虑哪些因素？


### mongodb

1. 什么是mongodb？
2. 什么是聚合操作？需要注意哪些事项？
3. mongodb相比于mysql有哪些优势？
4. 副本集是什么？
5. 使用副本集时，怎么尽可能保证数据都写入成功？
6. mongodb有哪些索引？
7. mongodb索引的实现原理？
8. mongo怎么保证crash-safe？
9. mongodb在更新、删除、插入时，是立刻写入文件吗？
10. 为什么mongo的存储文件比预估的稍大些？
11. 主从副本之间怎么同步数据？
12. 分片是什么？
13. 如果一个分片正处在迁移期间，在这个分片上出现更新会发生什么事情呢？
14. mongo有事务吗？怎么理解多文档事务？
15. 如果一个分片意外终止时，发起一个查询会发生什么事情？
16. 怎么排查mongo的慢查询？
17. mongodb的数据模式与mysql有什么区别？
18. 怎么安全的备份数据？

### nginx

### kfp

全称kubeflow pipeline，是一个基于k8s的任务调度框架。提供了以下几个重要的概念：

- pipeline：流程模板
- experiment：一组训练任务的统称
- run：从流程模板实例化出来的一个实例，一次训练任务。
- step：训练任务中的一个步骤
- artifacts：step之间的input/ouptut文件。存放在minio中
- parameters：step之间有两种参数传递方式：一种是parameters字符串；一种是artifacts文件

从使用上来看，kfp有以下几个重要的点：

- kubeflow pipeline提供了一组rpc接口。可以直接在业务层进行调用
- kubeflow pipeline提供了一个python sdk，用来定义pipeline。写出来并compile之后，直接上传到kfp上就行

在kfp1.7.1之后，社区提出了一个Pipeline IR（Intermediate representation）的计划。是为了解决这个问题：之前的python sdk生成的是一个yaml文件，但是这个文件仅能在k8s algo调度器上执行。为了解耦调度器，所以提出了Pipeline IR。就是用Python Compile出一种中间描述文件，再由kfp backend对这个文件进行解释以适配不同的调度器

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