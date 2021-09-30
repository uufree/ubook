# Redis原理篇

[TOC]

## 基础数据类型

### 字符串

字符串类型的内部编码有3种：

- int：8字节的长整形
- embstr：小于等于39字节的字符串
- raw：大于39字节的字符串

redis会根据当前值的类型和长度决定使用那种内部实现。

### 哈希

哈希类型的内部编码有2种：

- ziplist：压缩列表。当元素数量小于**hash-max-ziplist-entries**（默认512）时、同时所有值小于**hash-max-ziplist-value**（默认64）字节时，Redis会使用ziplist作为哈希的内部实现。ziplist使用更加紧凑的结构实现多个元素的连续存储，比较节省内存。
- hashtable：哈希表。当无法满足ziplist的要求是，转用hashtable作为其内部实现。

### 列表

列表类型的内部编码有2种：

- ziplist：压缩列表。当元素数量小于**list-max-ziplist-entries**（默认512）时、同时所有值小于**list-max-ziplist-value**（默认64）字节时，Redis会使用ziplist作为列表的内部实现。
  - 注：**此规则在v6.2版本已放弃**
- linkedlist：当列表类型无法满足ziplist的条件时，Redis会使用linkedlist作为列表的内部实现。

### 集合

集合类型的内部编码有2种：

- intset：整数集合。当集合中的元素都是整数，且元素个数小于**set-max-intset-entries**（默认512）时，Redis会使用intset作为集合的内部实现。
- hashtable：哈希表。当集合类型无法满足intset条件时，Redis会使用hashtable作为集合的内部实现。

### 有序集合

有序集合的内部编码有2种：

- ziplist：压缩列表。当有序集合的元素个数小于**zset-max-ziplist-entries**（默认128个），同时每个元素的值都小于**zset-max-ziplist-value**（默认64字节）时，Redis会用ziplist来作为有序集合的内部实现。
- skiplist：跳跃表。当不满足ziplist条件时，有序集合会使用skiplist作为内部实现。

## Redis客户端通信协议

Redis制定了**RESP**（Redis Serialization Protocol）实现客户端和服务端的正常交互。

- 发送命令格式

  ```shell
  # 模板
  *< 参数数量 > \r\n
  $< 参数 1 的字节数量 > \r\n
  < 参数 1> \r\n
  ...
  $< 参数 N 的字节数量 > \r\n
  < 参数 N> \r\n
  
  # 示例
  $3
  SET
  $5
  hello
  $5
  world
  
  # 序列化
  *3\r\n$3\r\nSET\r\n$5\r\nhello\r\n$5\r\nworld\r\n
  ```

- 返回结果格式

  - 状态回复：在RESP中第一个字节为"+"
  - 错误回复：在RESP中第一个字节为"-"
  - 整数回复：在RESP中第一个字节为":"
  - 字符串回复：在RESP中第一个字节为"$"
  - 多条字符串回复：在RESP中第一个字节为"*"

  