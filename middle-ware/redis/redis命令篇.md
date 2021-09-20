# Redis命令篇

[TOC]

## 全局命令

- 登录验证：`auth password`

- 查看所有键：`key *`
- 查看当前库的键总数：`dbsize`
- 检查键是否存在：`exists key`
- 删除键：`del key1 key2 ...`
- 设置键过期：`expire key second`
- 查看键过期时间：`ttl key`
  - 返回>0的整数：键剩余的过期时间
  - 返回-1：键没有设置过期时间
  - 返回-2：键不存在
- 查看键的数据结构类型：`type key`
- 查看数据结构的内部编码：`object encoding key `

## 字符串

字符串类型是Redis最基础的数据类型。Key都是字符串类型，Value可以是字符串、数字等（最大不能超过512MB）。

- 设置值：`set key value [ex seconds] [px milliseconds] [nx|xx]`
  - `ex seconds`：设置秒级别的过期时间
  - `px milliseconds`：设置毫秒级别的过期时间
  - `nx`：Key必须不存在，才能设置成功。用于添加
  - `xx`：Key必须存在，才能设置成功。用于更新
- 获取值：`get key`
- 批量设置值：`mset key value [key1 value1]`
- 批量获取值：`mget key key1 ...`
- 自增：`incr key`
- 自减：`decr key`
- 自增指定数字：`incrby key number`
- 自减指定数字：`decrby key number`
- 向字符串末尾追加值：`append key value`
- 获取字符串长度：`strlen key`
  - 注意中文的编码方式
- 设置并返回原值：`getset key value`
- 设置指定位置的字符：`setrange key offset value`
- 获取部分字符串：`getrange key start end`