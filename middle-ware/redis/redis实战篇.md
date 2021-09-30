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


