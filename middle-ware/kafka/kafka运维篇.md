# Kafka运维篇

[TOC]

## 部署

详见deploy文件夹。

## 配置项



## 工具链

### kafka-acls.sh

### kafka-broker-api-versions.sh

### kafka-configs.sh

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

- `--bootstrap-server h1:9092,h2:9092`：服务地址。很少用，一般仅用zk就够了
- `--zookeeper z1:2181`：zk地址

- `--alter`：更改Topic的分区数、副本分配和/或配置

- `--create`：创建Topic

- `--delete`：删除Topic

- `--describe`：展示Topic详情

- `--list`：展示所有可用Topic

- `--topic name`：指定Topic Name

- `--if-exists`：支持：alter、delete、describe

- `--if-not-exists`：支持：create

- `--partitions n`：分区数量

- `--replication-factor n`：副本数量（主副本也被计算在内）

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

- `--config name-value,name1=value1`：设置Topic相关的配置项
  - Topic Configurations
    - `cleanup.policy`
    - `compression.type`
    - `delete.retention.ms`
    - `file.delete.delay.ms`
    - `flush.messages`
    - `flush.ms`
    - `follower.replication.throttled`
  - Replica Configurations
    - `index.interval.bytes`
    - `leader.replication.throttled.replicas`
    - `max.compaction.lag.ms`
    - `max.message.bytes`
    - `message.downconversion.enable`
    - `message.format.version`
    - `message.timestamp.difference.max.ms`
    - `message.timestamp.type`
    - `min.cleanable.dirty.ratio`
    - `min.compaction.lag.ms`
    - `min.insync.replicas`
    - `preallocate`
    - `retention.bytes`
    - `retention.ms`
    - `segment.bytes`
    - `segment.index.bytes`
    - `segment.jitter.ms`
    - `segment.ms`
      - `unclean.leader.election.enable`
- `--delete-config name=value,name1=value1`：删除Topic相关的配置项
- `--disable-rack-aware`：禁用Rack自适应的副本分配机制
- `--exclude-internal`：暴露Server内部的一些Topic

### kafka-verifiable-consumer.sh

### kafka-verifiable-producer.sh

## 监控

