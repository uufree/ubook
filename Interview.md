# Interview

[TOC]

## project

### ocr

### radar

### media

## components

### mysql

1. `DISTINCT`和`GROUP BY`有什么区别？

   `DISITINCT`是用来对目标字段去重的；`GROUP BY`是用来分组的，一般会接聚合函数。

2. `INSETR`和`REPLACE`有什么区别？

   `INSERT`是直接插入，遇到有UK的重复字段时会直报错；`REPLACE`遇到有UK的重复字段时会先删后插，其他情况与`INSERT`一样；

3. `DELETE`、`DROP`、`TRUNCATE`有什么区别？

   `DELETE`是DML操作，删除时需要记redolog，比较慢。只删除数据，不删除表结构

   `TRUNCATE`是DDL操作，只删除数据，不删表结构；

   `DROP`是DDL操作直接删表数据和表结构

4. `INNER JOIN`和`LEFT JOIN`有什么区别？

   - `INNER JOIN`的结果相当于两张表的交集，优化器会用数据量小的表作为驱动表，数据量大的作为被驱动表
   - `LEFT JOIN`的结果相当于左表+两张表的交集。优化器会用左表作为驱动表，右表作为被驱动表

5. 用过哪些Mysql函数？

   字符串相关；日期相关的；聚合相关的。这块需要注意的是：**不能在索引上使用函数，否则查询不会走索引**

6. LIKE语句中的%和_有什么区别？

   %表示0-n个字符；_仅表示一个字符

7. BLOB和TEXT有什么区别？

   范围都是1-65536。BLOB存储二进制数据，TEXT存储文本数据

8. 外键是什么？什么时候需要用？

   用外键约束将上下两张表联系起来，主要是为了维护两张表关系的一致性。`pipelines`和`models`这两张表就需要使用外键关联起来，并需要在models表中声明：如何应对pipeline的UPDATE & DELETE

9. 讲讲一条SQL语句的执行过程

   统一的流程是：连接器 -> 分析器（词法、语法分析，决定做什么）-> 优化器（选索引、联表、排序等，决定怎么做） -> 执行器（调用存储引擎接口）

   - Create、Update
     - 唯一索引，修改数据页（没有就去读，需要判断唯一性）、写redo log
     - 非唯一索引：写change buffer、写redo log。
   - Delete：写change buffer、写redo log。
   - Retrieve：通用流程

10. 删除操作会减少数据库文件的大小吗？怎么减小数据库文件的大小？

    不会，删除操作仅将数据行标记为删除状态；需要使用`ALTER TABLE t ENGINE=InnoDB;`重建表

11. Mysql长连接会有什么问题？怎么解决？

    长连接中使用的内存只有在连接释放时才会清理，容易造成内存占用过高，导致crash；定期执行**mysql_reset_connection**清理

12. 查询缓存是什么？有什么使用场景？

    优化查询速度的一个组件，适用于没有修改的静态表。当某张表出现修改时，Mysql会删除这张表所有的查询缓存

13. 讲讲Mysql优化器的作用

    选索引、联表、排序等操作

14. Mysql是怎么选择索引的？

    从扫描行数、语句执行代价（主要考虑非主键索引的回表）这两个角度进行考虑

15. Mysql是怎么确认扫描行数的？

    用随机采样的方式统计行数。具体来说就是从InnoDB中随机获取这张表相关的几个页面，统计页面中Item数量的平均值 * 页面数量，就能大概算出扫描行数

16. 哪些情况会导致索引失效？怎么解决？

    - 索引有多个列。需要从左到右，不跳中间列
    - 在索引列上用函数
    - 使用`!=`,`%string`,`<>`这样的操作会导致索引失效

17. 讲讲联表的过程

    选出驱动表和被驱动表。然后，将驱动表放在Join Buffer中，如果一次放不下，那就分成n段放。如果被驱动表的连接字段上有索引，那就根据索引进行条件过滤；如果被驱动表没有索引，那就在被驱动表上进行全表扫描。

    相应的调优手段是：在被驱动表的连接字段上加索引；增加join buffer size，减少扫描次数

18. 讲讲排序。有什么调优思路？

    有两个关键的变量：

    - sort buffer size：决定使用内存排序还是临时文件排序
    - row size：决定使用全字段排序还是RowID排序

    优化思路就是根据需求调大sort buffer size或者row size，防止出现文件排序或者回表

19. Mysql的缓冲池是怎么防止热点数据被淘汰的？

    将LRU链表分为yuang和old两部分（5:3）；新来的数据加入old list，当下次访问到的时候，发现已经在old list中超过1000ms，就提升到yuang部分

20. MyISAM和InnoDB有什么区别？

    InnoDB支持行锁；InnoDB支持MVCC；InnoDB支持事务；InnoDB支持外键

21. MyISAM和InnoDB执行`select count(*)`谁更快？

    MyISAM内部有存储count数量

22. InnoDB的事务是如何实现的？

    A：通过Undo Log做原子性。当执行rollback时，Undo Log会进行回滚

    C：通过原子性、隔离型、持久性来保证一致性状态

    I：MVCC和Next-Key Lock保证可重复读、读已提交级别的隔离性

    D：通过Redo Log做持久性

23. InnoDB有哪些特点？

    - change buffer：主要用于非主键索引，优化了随机读
    - double write：从存储引擎的角度看，将一组脏页刷新的磁盘时，如果刷到一半突然断电了，那就gg了。double write就是为了应对这样的场景。主要就是说，先将这一组脏页聚合起来，顺序写入到磁盘的某一个位置（2MB），之后再用随机写去刷新，刷新过程中即使出现了问题，也可以用顺序写入的部分进行恢复。
    - 自适应哈希索引：当InnoDB发现某个页面被频繁访问时，就会建立hash索引，加速访问速度
    - 预读机制：1Page=16KB；1Extent=64Page。当从这个Extent中读取的Page数量超过阈值时，就会去预读下一个Extent的数据

24. Change Buffer是什么？有什么作用？适用于什么场景？

    主要用于非主键索引的插入与更新、主键索引的删除操作。减少了随机读取数据页。

    适用于写多读少的场景；不适用于写后立刻读的场景

25. Change Buffer Merge的触发时机有哪些？触发时会做什么事情？

    读取数据页时触发；后台线程定期触发；关闭数据库时触发

    就修改合并至数据页中，然后由脏页刷新将数据刷回磁盘

26. 什么时候会执行脏页刷新？脏页刷新时，会修改redo log吗？

    内存中的脏页比例超过某个阈值；redo log写满了，需要往前推进时；系统内存不足时；系统空闲时；系统关闭时；

    脏页刷新时，不会修改redo log

27. 主键索引和非主键索引有什么区别？

    主键索引叶子节点放的是数据；非主键索引叶子节点放的是主键ID

28. Mysql为什么选择B+树做索引？B树和B+树有什么区别？

    Mysql的数据是放在磁盘中的，对于机械硬盘来说，读取的开销比较高（寻道、寻址）。为了减少读取次数，需要在一个节点上存放更多的数据。其次是，如果一个节点上放的数据比较多，那这颗树的层高就比较低，减少了遍历一颗树时的开销。

    B+树不在非叶子节点存数据，进一步降低层高；B+树使用链表连接叶子节点，增加对范围查询的支持速度；

29. 讲讲对联合索引的理解

    多列索引，按照从左到右的顺序进行排序。使用时，切记不要跳过中间的某个字段，否则可能导致索引失效

30. 讲讲对Mysql事务的理解？

    A（原子性）、C（一致性）、I（隔离型）、D（持久性）

31. 怎么理解事务的隔离级别？每个隔离级别会存在什么样的问题？

    隔离级别：

    - 读未提交：脏读、不可重复读、幻读
    - 读已提交：不可重复读、幻读
    - 可重复读：幻读
    - 串型化：事务排队，最直接没啥问题

    问题：

    - 脏读：读到其他事务没有提交的修改
    - 不可重复读：在事务中前后两次读一个值，读到的数据不一致
    - 幻读：在事务中前后两次读一个区间，读到的数据不一致

32. Mysql一致性是什么意思？一致性读呢？

    从某个一致性的状态转化到另外一个一致性的状态。就好比：A、B账户各有500块，不管他俩怎么转账，总数始终等于1000

33. Mysql是怎么实现读已提交和可重复读隔离级别的？MVCC是怎么实现的？

    使用MVCC（多版本数据控制）；

    视图：记录2个值（当前已提交事务的最大值、自己的事务ID）、1个数组（已提交事务最大值和自己的事务ID之间的数据，表示未提交的事务）。

    读已提交时在SQL语句执行时，创建这个视图；可重复读是在事务开始时，创建这个视图，并且在事务执行期间统一用这个视图。读取数据时，只能读到已提交事务的值。

34. Mysql是怎么解决幻读的？

    并发读用MVCC解决；当前读用Next-Key Lock（行锁+间隙锁）解决

35. Mysql有哪些锁类型？分别有什么用？

    MDL锁、行锁、间隙锁、Next-Key Lock

36. 如果在某行上加锁，什么时候提交？

    事务结束时提交

37. Mysql死锁是什么？

    两个事务互相等待对方先释放某一行的锁；主要有两种方式：

    - 等待对方持有的锁超时；
    - 主动发起死锁检测，即扫描事务列表，查询是否有某个事务持有行A的锁，并在等待行B的锁。如果有的话，就进行回滚

38. 怎么用Mysql实现悲观锁和乐观锁？

    乐观锁是说乐观的认为没有其他线程和我争抢锁，即读不加锁，更新再加锁；悲观锁是说悲观的认为有很多线程在和我争抢锁，读、更新都加锁

    - 乐观锁：使用数据版本进行控制
    - 悲观锁：for update，先下手为强

39. BinLog、Undo Log、Redo Log分别是什么？详细讲讲

    - binlog：Mysql提供的日志，记录原始的写入型SQL语句
    - redo log：InnoDB提供的日志，记录运行期间InnoDB的修改，主要是为了保证crash safe
    - undo log：主要用于实现MVCC和数据回滚

40. BinLog有哪些格式？有什么使用建议？

    statement（记录SQL原文，有可能导致数据不一致）；row（记录针对CRUD的操作，记录的日志可能比较多）；mixed

41. Undo Log是针对库还是针对表的？

    Undo Log存放在全局表空间中

42. 可以只使用BinLog吗？

    不行。BinLog没有Crash Safe的保证，没有记录数据页中完整的修改。

43. 可以只使用RedoLog吗？

    理论上可以，但常用BinLog来进行完整的备份与主从同步。

44. 为什么需要两阶段提交？

    主要是为了保证BinLog和RedoLog的一致性。如果没有的话：

    - BinLog->RedoLog：多出一个事务
    - RedoLog->BinLog：丢一个事务

45. 如何安全的修改表字段呢？

    修改表字段需要持有MDL锁，在事务中，为了防止表字段和返回结果不一致，会加一个MDL读锁。在修改字段时，TPS肯定会归0。在业务低峰期进行较好。

46. 怎么安全的复制表？

    ```mysql
    INSERT INTO t1(a, b, c) SELECT a, b, c FROM t;
    ```

47. 怎么安全的备份库？

    在可重复读的隔离级别下开启一个事务，在这个事务中备份数据。`mysqldump -single-transaction`

48. CPU的利用率很高，但是每秒处理的事务量太少。有什么解决思路？

    可能是出现了死锁，CPU忙于做死锁检测。用分布式锁控制访问Mysql的并发度

49. 长事务有什么影响呢？

    造成Undo Log中的回滚段过大；造成连接占用的内存过多

50. 如果数据库的命中率骤降并且更新语句执行的非常慢，有什么解决思路？

    判断是否引入了唯一索引，导致更新操作不能用change buffer。修改为正常的索引

51. Update、Insert、Delete时，RedoLog和Change Buffer分别有什么作用？

    Redo Log避免了随机写；Change Buffer避免了随机读

52. 怎么排查Mysql慢查询？

    在运行时，合理设置慢查询日志。通过慢查询日志定位带慢语句，之后在通过explain确认语句选用的索引是否正确

53. 怎么排查Mysql阻塞？

    看下是否有其他人用于占用了MDL写锁；看下是否有其他人占用了Next-Key Lock；

54. Select语句是否启用了事务？

    启用

55. 如何处理QPS暴增？

    加缓存，降低数据库的负载

56. 若Mysql IO出现瓶颈，怎么进行调优？

    写磁盘的就3个地方：Redo Log、BinLog、脏页刷新。BinLog不是很重要，可以尝试修改下BinLog的写入策略。

57. 为什么 binlog cache 是每个线程自己维护的，而 redo log buffer 是全局共用的？

    BinLog中记录的是一个完整的事务，不能被打断；而Redo Log没有这个需求

58. 事务执行期间，还没到提交阶段，如果发生Crash的话，redo log肯定丢了，这会不会导致主备不一致呢？

    不会，BinLog只有在Commit之后才会发送给备库

59. 如何处理误删数据？

    一般在事前会给账户分配权限，还会做一些删除操作的特殊设置；BinLog启用Row模式，回滚数据

60. 主键ID为什么是自增的？

    尽可能的让B+树顺序插入，避免页分裂，让索引更加紧凑

61. 自增主键ID为什么是不连续的？

    插入失败时，事务回滚，但是主键ID不回滚，就会导致主键ID不连续。

62. 自增ID用完了怎么办？

    用完前分库分表；使用Alter Table修改主键类型为BigInt

63. 当自增ID到达上限是，继续插入会怎么样呢？

    到达上限之后，获取的主键都是一样的。插入会因为主键ID重复而报错

64. Mysql的主从复制原理

    主：将事务放进BinLog Cache中

    从：主动从主节点上拉BinLog，并进行处理

65. 怎么让Mysql支持表情？

    使用utf8_mb4字符集

66. Mysql的行锁是怎么实现的？

    通过在主键索引上加锁完成的

67. 什么是事务？

    并发控制的基本单位，要么全部成功，要么全部不成功。具有ACID属性

68. 为什么要拆分大的DELETE操作？

    长时间占用MDL读锁；长事务导致连接占用的内存过多；DELETE会造成Undo Log过大；容易造成锁冲突，阻塞其他事务

69. char和varchar有什么区别？

    char(20)固定长度，范围1-255，申请多少就用多少，多出的部分用空格代替；varchar可变长度，范围1-255，申请的是可变长度，实际长度的len+1

70. 锁有什么优化思路？

    有冲突的行往后放，减少锁的粒度；尽量使用相同的加锁顺序，防止死锁

### redis

1. redis缓存满了怎么办？

   redis在配置文件中提供了几种策略，大致为：LRU，random，return error。可以根据需求进行配置

2. redis为什么这么快？

   纯内存操作；网络IO模型（epoll-ET）；线程模型（单线程+命令排队+无锁）；高效的数据结构

3. 讲讲redis的基础数据结构及底层结构

   - 基础：
     - string；
     - list(quick-list)；
     - hash(zippiest、hashtable)；
     - set(ziplist、hashtable)；
     - zset(ziplist、skiplist)；
   - 拓展：bitmap、cell、bloomfilter

4. 为什么zset使用跳表不使用二叉树？

   skiplist，需要支持范围查询。Hash是无序的，不能用；红黑树不支持范围查询，不能用

5. ziplist怎么查找某个key？zset如果是ziplist，怎么查找某个key？

   ziplist大致是这样一个结构：header + item length + items + footer。在ziplist中查找数据，用的是遍历的方式

6. 什么时候需要用bloom filter？讲讲bloom filter原理？有什么缺点？怎么解决这个缺点？

   - 应用场景：图片md5去重：如果不存在，那就进行处理；如果存在，那就去mongodb中进一步检查是否真的存在。有效降低直接查询mongo的次数；leveldb中好像有用
   - 特征：如果bf告诉你不存在，那就肯定不存在；如果bf告诉你存在，那只是可能存在

   - 原理：首先一组初始化为0的buckets，用一组hash计算key，计算完成后插入buckets中。查询时，也是先计算，然后看对应的buckets中是否有值。如果没有值，肯定不存在；如果有值，不一定存在
   - 缺点：bf不支持删除操作
   - 解决方式：使用cuckoo filter（布谷鸟过滤器）

7. cell是什么？什么时候会用？和nginx的限流相比有什么区别？

   - 漏桶限流组件；

   - 分布式限流；
   - 区别：nginx只能用于单个nginx示例，不适用于多层负载均衡中的限流场景。都是漏桶限流，好像没什么区别

8. 讲讲hash扩容的过程

   当len(buckets) = len(items)时，会发生渐进式rehash。即在old hashtable上发生curd操作时，会将old->new

9. redis是怎么删除数据的？有哪些删除数据的命令？不同的删除命令有什么区别？

   - 后台线程定时删除+访问时惰性删除
   - delete（同步阻塞删除）
   - unlink（异步非阻塞删除，真正的删除操作在后台线程中执行）

10. 为什么需要持久化机制？

    从原理上说，他解决的是高可用问题，防止数据丢失；但实际使用上，主要是为了解决冷启动

11. 讲讲aof和rdb。有什么区别？使用场景有哪些？

    - rdb：使用二进制的方式保存当前进程内存快照。解决冷启动问题；
    - aof：每次执行写入命令时，先写内存，再写aof文件。解决实时命令持久化问题
    
12. 讲讲aof rewrite

    当AOF文件过大时（有配置项），会用当前的内存快照重写aof文件。在重写的过程中，通过Fork产生子进程，重写过程会用到COW

13. 如何选择合适的持久化方式？

    - 如果能接受小时级别的数据丢失，选择RDB
    - 如果不能丢数据，选择AOF

14. 如果要同时更新多个key，怎么保证原子性？lua script在redis中有什么问题？

    内嵌lua脚本；要注意lua脚本的超时时间，默认5s；

15. 两个线程同时写一个key会怎么样？怎么保证顺序性？

    根据请求到达顺序进行排队处理；用分布式锁或者watch..mutil..exec这样打包一组命令

16. sentinel和cluster分别解决了什么问题？

    sentinel解决高可用问题；cluster解决了单实例写入问题

17. 讲讲redis sentinel、redis cluster的选举过程

    - sentinel：主观下线->客观下线->sentinel领导者选举(raft)->选择新的主节点->让其他从节点同步主节点
    - cluster：主观下线->客观下线->持有slots的节点投票选举新的主节点->其他从节点同步新的主节点

18. 主从副本的数据同步方式？

    全同步用rdb；半同步用aof

19. redis cluster是怎么做数据分布的？为什么选用slots？如果用一致性哈希有什么问题？

    用槽( 0~16383)的方式分布数据，slot = hash(key) / 16384

    一致性哈希：存在一个指定长度的hash环，然后通过hash(key)，将数据映射到环中的某一个节点上。这样做的优点是：增加或者删除节点时，仅影响相邻节点的数据；缺点是，数据分布不均匀，容易造成数据倾斜。

    slots就相当于固定长度的一致性哈希+虚拟节点。

20. 讲讲redis、mysql数据同步策略

    - 先删缓存，再写数据库：线程A删了cache -> 线程B cache miss & 从db中获取就数据放到缓存中 -> 线程A update db。走完上述流程后，cache中依旧是old data。这种方案不可行
    - 先写数据库，再删缓存：线程A更新DB -> 线程B从cache中获取了old data -> 线程A删除cache -> 线程C触发miss，更新cache中的数据。这种方案是可行的
    - 只更新数据库，然后通过binlog将数据自动同步到缓存中。最终一致性策略

21. 有人说Redis只适合做缓存，不适合做数据库，你怎么看？

    不适合做数据库：缺少事务支持；占用大量内存

22. 讲讲缓存穿透、缓存击穿、缓存雪崩。有什么好的处理建议？

    - 缓存穿透：数据库没有数据，缓存也没有数据。解决方式：布隆过滤器拦截；缓存空对象；使用分布式锁限制同时访问存储端的并发量
    - 缓存击穿：数据库有数据，缓存没有数据。解决方式：定期更新热点Key TTL；使用分布式锁限制同时访问存储端的并发量
    - 缓存雪崩：大量Key同时失效。解决方式：Key的过期时间+Random Number；使用分布式锁限制同时访问存储端的并发量

23. 讲讲用redis做分布式锁需要注意哪些事项？

    加锁要有`create if not exists`语义，并且为了防止锁超时，需要设置random key；解锁要有`del if equal`语义

    redlock是多实例分布式锁方案，并且实例要无关。在超过n/2+1的节点上`set nx ex`成功即加锁成功。 

24. 怎么发现redis大key？大Key可能引发哪些问题？有哪些处理建议？

    - 排查：`redis-cli --bigkeys`、`debug object key`

    - 问题：针对大key的curd操作可能变慢，阻塞应用
    - 处理建议：对大key进行拆分

25. 怎么发现redis的热点key？热点Key可能引发哪些问题？有哪些处理建议？

    - 排查：`redis-cli --hotkeys`

    - 问题：从集群的角度看，容易造成流量倾斜；热点Key超时将导致缓存击穿，影响DB的稳定性
    - 处理建议：增加热点key的过期时间防止缓存击穿；使用分级缓存，应用程序统计访问同一个cache的次数

26. 怎么排查redis的慢操作？怎么处理？

    - 排查：`slowlog get`
    - 处理思路：
      - 是否使用了复杂度高的命令，例如keys、hgetall等
      - 是否操作了bigkey，使用：`redis-cli -a xxxx -d 0 --bigkeys`检测
      - 查看cpu利用率，判断是否到达了实例处理上限
      - 查看IO，判断是否出现了写rdb或者aof rewrite

27. 主从部署+读写分离，可能遇到哪些问题？怎么解决？

    半同步采用AOF机制，存在些许延迟。导致可能读到已经删除或者已经过期的数据，这个需要应用容忍这种情况。

28. 在redis的使用上有什么建议？

    - 不要在master上做RDB或者AOF，将备份工作交给从节点
    - 使用链式部署结构，降低节点压力

29. redis事务有什么问题吗？

    不建议使用。redis只是一组批量语句提交，若其中一行执行失败，依旧会顺序往下执行。如果一定要用，建议使用lua script，至少可以在逻辑中加入一些rollback

### kafka

1. 为什么要使用kafka？

   异步处理；服务解耦合；请求削峰；提升吞吐量

2. kafka有哪些缺点？

   部署需要引入zk，有点麻烦；不支持延时消息；不支持多分区消息有序

3. kafka有哪些特点？

   高吞吐（顺序读写文件、消息索引）；高拓展（topic partition、consumer group）；高可靠（通过写磁盘进行持久化、分区副本）

4. kafka如何实现高吞吐？

   顺序读写文件；消息索引（在.index文件中存储.log文件的msg offset<->log file position，并将.index文件映射到内存中）；同步消息时采用零拷贝技术

5. kafka的高可靠是怎么实现的？

   两个角度：消息会被顺序持久化至磁盘；采用分区副本（主分区读写，从分区只负责同步），使用acks参数控制写入动作

6. kafka的高拓展是怎么实现的？

   两个角度：consumer group提升消费能力；多partition提升写入能力

7. 讲讲kafka的体系结构

   Broker；Producer；Consumer Group、Consumer；Topic、Partition；

8. kafka分区的目的？

   横向拓展消费能力；采用分区副本保证消息的高可靠

9. 如何保证kafka消息有序？

   单一partation

10. 讲讲kafka的消息一致性。

    从HW和LEO的机制交付来回答。

    - HW（High Water Mark）：ISR（In-Sync Replication）中，Offset最小的位置，相当于木桶的最短板。只有HW以上的消息才能被读取，控制消费者读取数据的位置。
    - LEO（Log End Offset）：ISR中，Offset最大的位置，相当于木桶的最长板，即主副本Offset。

11. 怎么做kafka消息幂等？

    MongoDB中我用了唯一索引，ES不清楚；Mysql可以使用唯一索引；

12. kafka在什么情况下会丢消息？

    acks=0时；producer recv&commit，还没来得及处理，就crash了；

13. kafka在什么情况下会出现消息重复？

    producer采用auto commit，消费了但没有commit，然后出现了crash

14. consumer和consumer group是什么关系？

    consumer属于consumer group，从一个或者多个topic中读消息；每个consumer至少能分到1个partition

15. 讲讲consumer group rebalance

    消费者数量或者分区数量发生变化时，controller将分区重新分配给某个consumer的过程

16. 讲讲partition rebalance。

    broker的数量发生变化时，为了保证每个broker的吞吐大致平衡，需要手动调整下partition在broker list中分不

17. kafka分区数量可以减少吗？

    不行，对于减少的分区中的数据如何处理比较复杂

18. 讲讲kafka的分区副本

    主副本负责读写，从副本负责同步数据。引入一些ISR、OSR，HW、LEO这样的概念。需要注意的是，分区副本的数量（包含主副本）不能超过broker

19. zk在kafka中有什么作用？

    维护kafka节点状态，包括但不限于：controller选举、broker管理、配置管理等

20. consumer长时间没有pull，会有什么问题吗？

    pull中存在心跳，长时间没有心跳，会导致被踢出consumer group，触发rebalance

21. 如何减少数据丢失？

    acks=all；在producer中，处理完消息之后，再commit；加入幂等性处理重复的消息；禁止OSR中的副本竞选主副本

22. offset有什么作用？

    kafka中的消息是不能修改的，使用offset能描述唯一的消息位置；使用offset和index文件建立索引，加速读取

23. 如何修改kafka能接受的最大的消息大小？

    同时修改producer、broker、consumer中的配置项

24. 如何估算kafka集群的大小？

    使用Kafka-consumer/producer-pref-test.sh这样的工具，结合消息大小测试下

25. 如何调优kafka？

    根据吞吐、延迟等需求，从broker、topic、consumer、produce以及JVMr这5个角度分开去看

26. kafka为什么不支持读写分离？

    避免发生一致性问题

27. kafka磁盘容量规划需要考虑哪些因素？

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