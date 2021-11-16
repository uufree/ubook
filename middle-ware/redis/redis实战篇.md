# Redis实战篇

[TOC]

## 最佳运维实践

### Linux系统

- `vm.overcommit_memory`：建议配置为1，为Fork留足空间

  - 0：内核不允许使用超量内存。
  - 1：内核允许使用超量内存。

  ```bash
  echo "vm.overcommit_memory=1" >> /etc/sysctl.conf
  sysctl vm.overcommit_memory=1
  ```

- `swappiness`：swap空间

  ![image-20211017181816705](assets/image-20211017181816705.png)

  ```bash
  echo vm.swappiness={bestvalue} >> /etc/sysctl.conf
  ```

- `THP`：大内存页（2MB），默认开启。为了防止Fork过慢，建议关闭。

  ```bash
  echo never > /sys/kernel/mm/transparent_hugepage/enabled
  ```

- `NTP`：宿主机时间同步配置

  ```bash
  0 * * * * /usr/sbin/ntpdate ntp.xx.com > /dev/null 2>&1
  ```

- 增加进程的文件描述符数量

  ```bash
  ulimit –Sn {max-open-files}
  ```

- `TCP backlog`

  ```bash
  echo 511 > /proc/sys/net/core/somaxconn
  ```

## 最佳使用实践

### 缓存

1. **为什么使用缓存**？

   - 读写加速，提升用户体验

   - 降低后端负载
   
    但是会引两个问题：
   
   - 数据不一致。缓存层与数据层存在数据不一致的可能，与**更新策略**相关
   - 引入缓存组件，提升运维成本

2. **缓存更新策略最佳实践**

![image-20211017180040197](assets/image-20211017180040197.png)

- 低一致性业务：配置最大内存和淘汰策略，需要的话，再加上定时即可
- 高一致性业务：**超时剔除 + 主动更新**

3. 什么是**缓存穿透**？怎么优化？

   缓存穿透将导致不存在的数据每次请求都要到存储层去查询，失去了缓存保护后端存储的意义。即：**底层数据库没有数据且缓存内也没有数据**

   两种解决方式：

   - 缓存空对象

   ![image-20211017180452627](assets/image-20211017180452627.png)

   - 布隆过滤器拦截

     > 如果布隆过滤器认为这个值不存在，那么这个值就一定不存在
     >
     > 如果布控过滤去认为这个值存在，那么这个值就一定存在

   ![image-20211017180721486](assets/image-20211017180721486.png)

4. 什么是**缓存击穿**，怎么优化？

   当热点数据key从缓存内失效时，大量访问同时请求这个数据，就会将查询下沉到数据库层，此时数据库层的负载压力会骤增，我们称这种现象为"缓存击穿"。即：**底层数据库有数据而缓存内没有数据**。解决方式：
   
- 热点Key永不过期
   - 利用互斥锁保证同一时刻只有一个客户端可以查询底层数据库的这个数据
   
5. 什么是**缓存雪崩**，怎么优化？

   **缓存雪崩是指Redis中大量的key几乎同时过期**，然后大量并发查询穿过redis击打到底层数据库上，导致数据库层的负载压力会骤增的现象。解决方式：

   - 热点Key永不过期

   - 在Key的过期时间上加随机数
   - **使用互斥锁保证：同时只有一个客户端访问数据层**

![image-20211017181126955](assets/image-20211017181126955.png)

### 统计

#### 聚合统计

统计多个集合元素的聚合结果，例如：统计手机 App 每天的新增用户数和第二天的留存用户数。包括以下类型：

- 统计多个集合的共有元素（求交集）
- 把两个集合相比，求一个集合独有的元素（求差集）
- 统计多个集合的所有元素（求并集）

上述需求，采用**Set**即可。但需要注意的是：求Set的差集、并集、交集的计算复杂度很高，在数据量较大的情况下，如果直接执行这些命令，会导致Redis阻塞。Tips：**可以从主从集群中选择一个从库，让它专门负责聚合计算，或者是把数据读取到客户端，在客户端来完成聚合统计**

#### 排序统计

要求集合类型能对元素保序。例如：展示所有评论中最新的留言需求。

- 采用List：执行LPUSH后，将导致分页乱序
- **采用Sorted Set**：插入数据时，采用Timestamp作为Score。在展示时，执行**ZRANGEBYSCORE**命令，可以保证分页数据不乱

#### 二值状态统计

集合元素的取值只有0、1两种情况，例如：签到打卡。

这种场景**使用Bitmap**，可以有效的节省空间。且Bitmap 本身是用 String 类型作为底层数据结构实现的一种统计二值状态的数据类型。

#### 基数统计

统计一个集合中不重复的元素个数。例如：统计网页的UV（unique visit）。

- 采用Set：可以完成工作，但是如果遇到了热Key，将占用大量空间
- **采用HyperLogLog**：此种计数方式存在误差，大约0.81%。但非常非常节省空间

### 分布式锁

> **原子操作**：不会被线程调度机制打断的操作，不会被switch context中断

- 单实例锁

  ```bash
  // acquire lock
  set lock {random-number} ex 5 nx
  ....
  
  // release lock
  if  equal random-number:
  	del lock
  ```

  使用**random number**是因为考虑到这样一种情形：

  1. 线程A持有锁超过了超时时间的限制
  2. 锁过期后线程B获取了这把锁
  3. 线程A的任务结束。如果没有用**random number**去判断，那么线程A会直接删除这把锁，导致线程C直接拿到锁

  但是引入**random number**后，因为redis不支持delifequal这样的命令，所以需要用Lua脚本封装一个**delifequal**的原子操作：

  ```bash
  if redis.call("get",KEYS[1]) == ARGV[1] then
  	return redis.call("del",KEYS[1])
  else
  	return 0
  end
  ```

- 多实例锁：**RedLock**

  在Redis的分布式环境下，假设存在N个Redis Master，完全相互独立，不存在主从复制或者其他集群协调机制。在这样的环境下，一个客户端想要获取锁，需要以下几个步骤：

  1. 获取当前的Unix时间戳
  2. 依次尝试从N个实例上，使用相同的Key和Random Value获取锁。注意：此步骤在请求时，应该设置一个响应超时时间，且这个超时时间要小于锁失效时间，这种做法是为了防止Redis已经挂掉，但是客户端还在等待响应结果。
  3. 当且仅当从大多数节点（N/2+1）获取锁成功后，并且使用时间小于锁失效时间，才算获取成功
  4. 如果获取锁失败，Client应该尝试在所有的Redis实例上进行解锁

  尽管使用上述策略，但是依旧是不安全的。例如：有A、B、C这3个节点。

  1. 客户1在A、B上加锁成功，C上加锁失败
  2. 这时B意外重启，但是由于持久化策略B上的锁没有来得及同步到AOF文件中
  3. 客户2发起申请，在B、C上加锁成功
  4. 这时，系统中就会出现两把锁，不安全！！

  解决方案：**延迟重启，且延迟时间>锁的失效时间**

- **锁冲突的解决方式**：
  
  1. 抛出异常，由业务代码做**退避重试**

### 限流

**漏斗限流算法**，使用Redis-Cell插件实现。使用方式详见：redis命令篇。

![image-20211018210535205](assets/image-20211018210535205.png)



### 去重

图片MD5去重、防止大量伪造的RequestID打穿Mysql等。实现方案：

- 方案1：使用Set过滤重复的请求。精确，但是占用空间过大
- 方案2：使用**Bloom Filter**过滤重复的请求。有误差，但是占用空间小。

### 时间序列数据

- 数据特征：

  - 写入：快速写入
  - 读取：
    - **点查询**：根据一个时间戳，查询相应时间的数据
    - **范围查询**：查询起始和截止时间戳范围内的数据
    - **范围聚合计算**：针对起始和截止时间戳范围内的所有数据进行计算，例如求最大 / 最小值，求均值等

- 解决方案：

  - 方案1：采用Hash + Sorted Set

    将数据同时保存在Hash和Sorted Set中，这种方案采用Hash满足了**点查询**；采用Sorted Set满足了**范围查询**。但是这种做法也带了两个问题：

    - 问题a：需要将数据取到客户端进行聚合计算，网络IO开销随数据量增大而增大
    - 问题b：数据会保存两份，导致较大的内存开销。

    > 采用此方案时，需要使用MULTI、EXEC来保证原子性

  - 方案2：采用RedisTimeSeries模块（第三方拓展模块）

    详细使用方式详见redis命令篇

## Go Client

采用https://github.com/go-redis/redis作为Golang Redis Client，生态如下：

- Mock：https://github.com/go-redis/redismock
- Distributed Lock：https://github.com/go-redsync/redsync
- Cache（类似ORM框架）：https://github.com/go-redis/cache
- Rate Limiting：https://github.com/go-redis/redis_rate
- Distributed Tracing：https://opentelemetry.uptrace.dev/guide/distributed-tracing.html#what-is-tracing

### Best Practices

- 使用`redis.Nil`检查Key是否存在

  ```go
  val, err := rdb.Get(ctx, "key").Result()
  switch {
  case err == redis.Nil:
  	fmt.Println("key does not exist")
  case err != nil:
  	fmt.Println("Get failed", err)
  case val == "":
  	fmt.Println("value is empty")
  }
  ```

- 执行接口不支持的命令

  ```go
  val, err := rdb.Do(ctx, "get", "key").Result()
  if err != nil {
  	if err == redis.Nil {
  		fmt.Println("key does not exists")
  		return
  	}
  	panic(err)
  }
  fmt.Println(val.(string))
  ```

- 事务

  ```go
  const maxRetries = 1000
  
  // Increment transactionally increments key using GET and SET commands.
  increment := func(key string) error {
  	// Transactional function.
  	txf := func(tx *redis.Tx) error {
  		// Get current value or zero.
  		n, err := tx.Get(ctx, key).Int()
  		if err != nil && err != redis.Nil {
  			return err
  		}
  
  		// Actual operation (local in optimistic lock).
  		n++
  
  		// Operation is commited only if the watched keys remain unchanged.
  		_, err = tx.TxPipelined(ctx, func(pipe redis.Pipeliner) error {
  			pipe.Set(ctx, key, n, 0)
  			return nil
  		})
  		return err
  	}
  
  	for i := 0; i < maxRetries; i++ {
  		err := rdb.Watch(ctx, txf, key)
  		if err == nil {
  			// Success.
  			return nil
  		}
  		if err == redis.TxFailedErr {
  			// Optimistic lock lost. Retry.
  			continue
  		}
  		// Return any other error.
  		return err
  	}
  
  	return errors.New("increment reached maximum number of retries")
  }
  ```

- 创建Cluster Client

  ```go
  rdb := redis.NewFailoverClusterClient(&redis.ClusterOptions{
      NewClient: func(opt *redis.Options) *redis.NewClient {
          user, pass := userPassForAddr(opt.Addr)
          opt.Username = user
          opt.Password = pass
  
          return redis.NewClient(opt)
      },
  })
  
  err := rdb.ForEachShard(ctx, func(ctx context.Context, shard *redis.Client) error {
      return shard.Ping(ctx).Err()
  })
  if err != nil {
      panic(err)
  }
  ```

- 创建Sentinel Client

  ```go
  import "github.com/go-redis/redis/v8"
  
  rdb := redis.NewFailoverClusterClient(&redis.FailoverOptions{
      MasterName:    "master-name",
      SentinelAddrs: []string{":9126", ":9127", ":9128"},
  
      // To route commands by latency or randomly, enable one of the following.
      //RouteByLatency: true,
      //RouteRandomly: true,
  })
  ```

- 创建Redis Ring

  Ring is a Redis client that uses consistent hashing to distribute keys across multiple Redis servers (shards). It's safe for concurrent use by multiple goroutines.

  ```go
  rdb := redis.NewRing(&redis.RingOptions{
      NewClient: func(opt *redis.Options) *redis.NewClient {
          user, pass := userPassForAddr(opt.Addr)
          opt.Username = user
          opt.Password = pass
  
          return redis.NewClient(opt)
      },
  })
  ```

  ```go
  import "github.com/golang/groupcache/consistenthash"
  
  ring := redis.NewRing(&redis.RingOptions{
      NewConsistentHash: func() {
          return consistenthash.New(100, crc32.ChecksumIEEE)
      },
  })
  ```







