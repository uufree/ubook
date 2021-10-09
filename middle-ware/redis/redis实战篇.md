# Redis实战篇

[TOC]

## 最佳运维实践

### 慢查询日志

- 配置项：
  - `slowlog-log-slower-than`：慢查询阈值，默认10000us = 10ms。
  - `slowlog-max-len`：慢查询日志队列长度，默认128
  
- 操作命令：

  - 获取慢查询日志：`slowlog get [n]`
  - 获取慢查询日志队列长度：`slowlog len`
  - 慢查询日志重置：`slowlog reset`

- 慢查询日志消息格式：

  ![image-20210930110925907](assets/image-20210930110925907.png)

- 注意事项：

  - 慢查询**仅记录命令执行时间**，并不包括排队和网络传输的时间
  - 慢查询是一个先进先出的队列，如果慢查询比较多的情况下，可能会导致慢查询日志丢失。为了防止这种情况发生，可以定期执行`slowlog get`命令将慢查询日志持久化到其他存储中。

### AOF与RDB持久化

- **fork操作**

  fork操作涉及到页表拷贝，比较耗时。改善方式如下：

  - 限制单个实例的内存。建议控制在10GB以内（对应页表大小约为20MB）
  - 降低fork操作的频率。包括放宽aof-rewrite的限制、降低bgsave的频率。

- **子进程开销监控和优化**

  子进程负责AOF和RDB文件的重写，主要涉及到CPU、内存、硬盘的开销、

  - CPU
    - 不要做绑定单核CPU操作，由于子进程非常消耗CPU，会和父进程产生单核资源竞争
    - 不要和其他CPU密集型服务部署在一起，防止CPU过度竞争
  - 内存：页表拷贝和写时复制会消耗一定的内存
  - 硬盘
    - 防止和消息队列、存储等高硬盘负载的服务部署在一起
    - AOF重写时会消耗大量的硬盘IO，可以开启配置`no-appendfile-on-rewrite`，表示在AOF期间不做fsync操作
    - 对于单机配置多个Redis实例的情况，可以配置不同实例分盘存储AOF文件，分摊硬盘写入压力

- **AOF追加阻塞**

  使用AOF同步策略为`everysec`时，Redis实例会启用后台线程每秒执行一次fsync同步硬盘。当系统硬盘资源繁忙时，会造成Redis主线程阻塞。优化方式如上所述。

## 最佳使用实践

### 数据库

- Redis Cluster仅使用0号数据库
- Redis是单线程架构，使用多个数据库无明显的性能提升
- 0号数据库的慢查询会影响其他数据库的查询效率
- 部分Redis Client不支持多数据库

### Lua脚本

- Lua脚本在Redis中是原子执行的，执行过程中不会插入其他命令
  - Tips：假设Lua脚本中有5步，中间步骤执行失败怎么办？
- 可根据业务自定义命令，并将这些命令常驻内存，实现复用效果
- 可以将多条命令一次性打包，有效的减少网络开销

## 基础数据类型

### 字符串类型的使用场景

- 缓存

  ```java
  UserInfo getUserInfo(long id) {
  	userRedisKey = "user:info:" + id
  	value = redis.get(userRedisKey);
  	
  	UserInfo userInfo;
  	if (value != null) {
  		userInfo = deserialize(value);
  	} else {
  		userInfo = mysql.get(id);
  		if (userInfo != null)
  			redis.setex(userRedisKey, 3600, serialize(userInfo));
  	}
  	return userInfo;
  }
  ```

- 计数

  ```java
  long incrVideoCounter(long id) {
  	key = "video:playCount:" + id;
  	return redis.incr(key);
  }
  ```

- 限速

  ```java
  phoneNum = "138xxxxxxxx";
  key = "shortMsg:limit:" + phoneNum;
  
  // SET key value EX 60 NX
  isExists = redis.set(key,1,"EX 60","NX");
  if(isExists != null || redis.incr(key) <=5){
  	// 通过
  }else{
  	// 限速
  }
  ```


