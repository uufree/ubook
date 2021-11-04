# Kafka运维篇

[TOC]

## 部署

详见deploy文件夹。

## 配置项



## 工具链

### kafka-acls.sh

### kafka-broker-api-versions.sh

### kafka-configs.sh

配置项管理工具。

| Topic可选配置项                         | Broker可选配置项                        | 释义                                                         |
| --------------------------------------- | --------------------------------------- | ------------------------------------------------------------ |
| unclean.leader.election.enable          | unclean.leader.election.enable          | 是否可以从非ISR集合中选举leader副本。默认为false。若修改为true，将可能导致数据丢失 |
| max.message.bytes                       | max.message.bytes                       | 消息的最大字节数，默认1000012                                |
| cleanup.policy                          | log.cleanup.policy                      | 日志压缩策略，默认为delete。可选compact                      |
| compression.type                        | compression.type                        | 消息压缩类型，默认producer，表示保存原始类型。可选：snappy, lz4, gzip |
| delete.retention.ms                     | log.cleaner.delete.retention.ms         | 标记为被删除的数据，多久可已被真正删除。默认86400000ms，即1天 |
| file.delete.delay.ms                    | log.segment.delete.delay.ms             | 清理文件之前可以等待多长时间，默认值为60000ms，即1分钟       |
| index.interval.bytes                    | log.index.interval .bytes               | 用来控制添加索引项的频率，每超过配置值，就添加一个索引项。默认4096 |
| min.insync.replicas                     | min.insync.replicas                     | 分区ISR集合中至少应该有多少个副本，默认1                     |
| retention.ms                            | log.retention.ms                        | 使用delete策略清理消息时，消息最大的保存时间。默认604800000ms，即7天 |
| max.compaction.lag.ms                   |                                         |                                                              |
| message.downconversion.enable           |                                         |                                                              |
| message.format.version                  | log.message.format.version              | 消息格式的版本                                               |
| message.timestamp.difference.max.ms     | log.message.timestamp.difference.max.ms | 消息时间戳与broker时间戳的最大差值。不要修改！！             |
| message.timestamp.type                  | log.message.timestamp.type              | 消息时间戳类型。不要修改！！                                 |
| min.cleanable.dirty.ratio               | log.min.cleanable.dirty.ratio           | 清理日志消息时的最小污浊率，默认0.5                          |
| min.compaction.lag.ms                   | log.min.compaction.lag.ms               | 日志再被清理前的最小保留时间 ,默认值为 0                     |
| preallocate                             | log.preallocate                         | 创建日志分段时是否需要预留分区，默认为false                  |
| retention.bytes                         | log.retention.bytes                     | 分区中能保留的消息总量，默认值为-1，即没有限制               |
| segment.bytes                           | log.segment.bytes                       | 日志分段的最大值，默认1073 741824，即1GB                     |
| segment.index.bytes                     | log.index.size.max.bytes                | 日志分段索引的最大值，默认10485760，即10MB                   |
| segment.jitter.ms                       | log.roll.jitter.ms                      | 滚动日志分段时，增加的随机时间。不要修改！！                 |
| segment.ms                              | log.roll.ms                             | 最长多久滚动一次日志分段，默认604800000，即7天               |
| flush.messages                          | log.flush.interval.messages             | 需要收集多少消息，才会强制刷新数据到磁盘上。不要修改！！     |
| flush.ms                                | log.flush.interval.ms                   | 需要等待多久才会将消息强制刷新至磁盘。不要修改！！           |
| follower.replication.throttled.replicas | follower.replication.throttled.rate     | 限制分区副本的复制速率                                       |
| leader.replication.throttled.replicas   | leader.replication.throttled.rate       | 限制leader分区副本的复制速率                                 |

> 以下配置项可能与最新版本的配置命名有出入，详细可见：kafka-configs.sh脚本

> 如果没有修改过主题的任何参数，那么新创建的主题将使用broker的配置参数作为它的默认值

- `--bootstrap-server`：服务地址，这个工具必须使用。

- `--zookeeper`：zk地址

- `--entity-type <String>`：配置类型，可选：topics/clients/users/brokers/broker-loggers。注：我们仅关注broker、topic的配置

- `--entity-type <String>`：具体的类型实例，可选：topic-name/client-id/user-principal-name/broker-id

- `--alter`：修改配置项时，必须携带这个参数

- `--describe`：展示配置项

- `--add-config <String>`：增加配置项。格式如下：'k1=v1,k2=[v1,v2,v2],k3=v3'

- `--delete-config <String>`：删除配置项。格式如下：'k1,k2'

  ```bash
  # 对topic新增配置项
  # 注：必须携带alter选项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type topics --entity-name hello --alter --add-config cleanup.policy=delete
  
  # 展示topic的配置项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type topics --entity-name hello --describe
  
  # 删除topic的配置项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type topics --entity-name hello --alter --delete-config cleanup.policy
  
  # 对broker新增配置项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type brokers --entity-name 1 --alter --add-config log.cleanup.policy=delete
  
  # 展示broker的配置项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type brokers --entity-name 1 --describe
  
  # 删除broker的配置项
  ./kafka-configs.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --entity-type brokers --entity-name 1 --alter --delete-config log.cleanup.policy
  ```

### kafka-console-consumer.sh

### kafka-console-producer.sh

### kafka-consumer-groups.sh

### kafka-consumer-perf-test.sh

### kafka-delegation-tokens.sh

### kafka-delete-records.sh

### kafka-dump-log.sh

### kafka-features.sh

### kafka-leader-election.sh

### kafka-log-dirs.sh

### kafka-mirror-maker.sh

### kafka-preferred-replica-election.sh

### kafka-producer-perf-test.sh

### kafka-reassign-partitions.sh

### kafka-replica-verification.sh

### kafka-run-class.sh

### kafka-server-start.sh

### kafka-server-stop.sh

### kafka-streams-application-reset.sh

### kafka-topics.sh

Topic管理工具

- `--zookeeper <String: hosts>`：zk地址。格式如下：`host:port`

- `--alter`：更改Topic的分区数

- `--create`：创建Topic

- `--delete`：删除Topic

- `--describe`：展示Topic详情

- `--list`：展示所有可用Topic

- `--topic <String>`：指定Topic Name

- `--if-exists`：支持：alter、delete、describe

- `--if-not-exists`：支持：create

- `--partitions <Integer>`：分区数量

- `--replication-factor <Integer>`：副本数量（主副本也被计算在内）

  ```bash
  # create topic
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --create --topic test --partitions 4 --replication-factor 2 --if-not-exists
  
  # delete topic
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --delete --topic test
  
  # topic detail
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --describe --topic hello
  
  # list topic
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --list
  
  # 修改topic分区数量
  # 注意：partitions数量仅能增长，不能缩减
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --alter --topic test --partitions 6
  
  # 修改topic副本因子
  # 注：不支持修改topic的副本因子
  ```


### kafka-verifiable-consumer.sh

### kafka-verifiable-producer.sh

## 监控

