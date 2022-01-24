# Kafka运维篇

[TOC]

## 部署

详见deploy文件夹。

## 配置项

- Broker

  | Broker配置项                            | 释义                                                         |
  | --------------------------------------- | ------------------------------------------------------------ |
  | broker.id                               | 标志broker身份。必选                                         |
  | zookeeper.connect                       | 链接的zk地址。必选                                           |
  | log.dirs                                | 日志存储位置。必选                                           |
  | advertised.listeners                    | 推送到zk的地址，也就是告诉client，kafka server的地址是什么。内网部署的话，需要client能解析到；公网部署的话，需要使用公网ip。必选 |
  | listeners                               | Kafka启动所使用的的协议及端口                                |
  | auto.create.topics.enable               | 是否启用自动创建topic的功能，默认true                        |
  | num.partitions                          | 主题中默认的分区数量，默认1                                  |
  | delete.topic.enable                     | 是否可以删除Topic，默认为true                                |
  | auto.leader.rebalance.enable            | 是否启用自动分区leader平衡（优先副本选举）。默认为true       |
  | leader.imbalance.check.interval.seconds | 检查分区是否均衡的周期，默认300，即5min                      |
  | log.retention.bytes                     | 分区中能保存的消息总量，默认-1，没有限制                     |
  | log.retention.hours                     | 使用delete策略清理消息时，消息最大的保存时间。默认168，即7天 |
  | log.roll.hours                          | 最长多久滚动一次日志分段，默认168，即7天                     |
  | log.segment.bytes                       | 日志分段的最大值，默认1073 741824，即1GB。与log.roll.hours配置满足其一就行 |
  | max.message.bytes                       | 消息的最大字节数，默认1048588                                |
  | min.insync.replicas                     | producer发送消息时，收到多少了ack，才能返回消息发送成功。默认1，即仅leader副本收到消息即可。可选：-1（all），1～n视部署而定 |
  | compression.type                        | 消息压缩类型，默认producer，表示保存原始类型。可选：snappy, lz4, gzip。不建议在broker和topic这一层设置压缩选项，不灵活 |
  | replica.fetch.min.bytes                 | 可以从每个分区中获取的最小的消息大小，默认1 bytes            |
  | replica.fetch.max.bytes                 | 可以从每个分区中获取的最大的消息大小，默认1MB                |
  | replica.fetch.wait.max.ms               | 副本fetch数据时，可以等待的的最长时间。默认500ms             |
  | unclean.leader.election.enable          | 是否可以从非ISR集合中选举leader副本。默认为false。若修改为true，将可能导致数据丢失 |
  | fetch.max.bytes                         | 一次请求，最多可以获取的消息数量，默认55MB                   |
  | broker.id.generation.enable             | 是否启用自动生成broker id的功能，默认true                    |
  | reserved. broker.max.id                 | broker id的最大值，默认1000                                  |
  | log.cleanup.policy                      | 日志压缩策略，默认为delete。可选compact                      |
  | log.index.interval.bytes                | 用来控制添加索引项的频率，每超过配置值，就添加一个索引项。默认4096 bytes |
  | log.index.size.max.bytes                | 日志分段索引的最大值，默认10485760，即10MB                   |

- Topic

  | Topic配置项                             | 释义                                                         |
  | --------------------------------------- | ------------------------------------------------------------ |
  | unclean.leader.election.enable          | 是否可以从非ISR集合中选举leader副本。默认为false。若修改为true，将可能导致数据丢失 |
  | max.message.bytes                       | 消息的最大字节数，默认1000012                                |
  | cleanup.policy                          | 日志压缩策略，默认为delete。可选compact                      |
  | compression.type                        | 消息压缩类型，默认producer，表示保存原始类型。可选：snappy, lz4, gzip |
  | delete.retention.ms                     | 标记为被删除的数据，多久可已被真正删除。默认86400000ms，即1天 |
  | file.delete.delay.ms                    | 清理文件之前可以等待多长时间，默认值为60000ms，即1分钟       |
  | index.interval.bytes                    | 用来控制添加索引项的频率，每超过配置值，就添加一个索引项。默认4096 |
  | min.insync.replicas                     | 分区ISR集合中至少应该有多少个副本，默认1                     |
  | log.retention.ms                        | 使用delete策略清理消息时，消息最大的保存时间。默认604800000ms，即7天 |
  | segment.bytes                           | 日志分段的最大值，默认1073 741824，即1GB                     |
  | segment.index.bytes                     | 日志分段索引的最大值，默认10485760，即10MB                   |
  | segment.ms                              | 最长多久滚动一次日志分段，默认604800000，即7天               |
  | follower.replication.throttled.replicas | 限制分区副本的复制速率                                       |
  | leader.replication.throttled.replicas   | 限制leader分区副本的复制速率                                 |

- Consumer

  - **group.id**：消费者组ID，默认为空
  - **fetch.min.bytes**：指定消费者从服务器一次请求获取的最小字节数，默认1Bytes
  - **fetch.max.bytes**：指定消费者从服务器一次请求获取的最大字节数，默认50MB
  - **fetch.max.wait.ms**：消费者一次请求最大的等待时间，默认500ms
  - **max.partation.fetch.bytes**：broker从每个分区中返回给消费者的最大字节数，默认为1MB。假设20个分区+4个消费者，那么每个消费者至少需要5MB的内存来存储从broker中获取的数据
  - **max.poll.records**：一起拉取请求中拉取的最大消息数量，默认500条
  - **exclude.internal.topics**：指定Kafka两个内部Topic：`__consumer_offsets`和`__transaction_state`是否可以向外部暴露。默认true
  - **receive.buffer.bytes**：用来设置Socket接收消息缓冲区（SO_RECBUF）的大小，默认为64KB
  - **send.buffer.bytes**：用来设置Socket发送消息缓冲区（SO_SNDBUF）的大小，默认128KB
  - **request.timeout.ms**：用来配置Consumer等待请求响应的最长时间，默认30stemp1
  - **metadata.max.age.ms**：消费者持有的元数据的过期时间，默认300s
  - **reconnect.backoff.ms**：重新连接主机之前的等待时间（退避重试时间），默认50ms
  - **retry.backoff.ms**：尝试重新发送失败请求到服务器的等待时间，默认100ms
  - **isolation.level**：用来配置消费者的事务隔离级别，默认`read_uncommitted`
    - read_uncommitted：无法读到未提交的消息，可以消费到HW的位置。
    - read_committed：消费者会忽略未提交的事务，只能消费到LSO位置
  - **session.timeout.ms**：消费者与broekr的最大心跳时间，如果在`session.timeout.ms`期间没有发送心跳给broker，就会发生Partation Reblance。默认10s
  - **auto.offset.reset**：消费者在读取一个没有偏移量或者偏移量失效的分区时，该如何处理
    - **latest**：从最新的位置开始读取数据
    - **earlist**：从分区最开始的位置读取消息
  - **enable.auto.commit**：配置是否需要auto commit，默认为true
  - **auto.commit.interval.ms**：控制auto commit的频率。默认5s

- Producer

  - **acks**：指定了必须要有多少个分区副本接收到消息，才能认为消息是写入成功的
    - **acks=0**：生产者写入消息直接返回，不在意是否有副本接收到消息。**吞吐量最大，可靠性最低。**
    - **acks=1**：只要Partation Leader副本接收到消息，就返回success。**吞吐量和可靠性的折中选择**。
    - **acks=-1(all)**：生产者在消息发送之后，需要等待 **ISR 中的所有副本**都成功写入消息之后才能够收到来自服务端的成功响应。理论上来说，**吞吐量最小，可靠性最高**。但是需要注意，在集群状态不健康的情况下，ISR中仅剩Partation Leader副本，将退化成acks=1。
  - **max.request.size**：单个请求中可以发送的消息的大小，默认为1MB。使用时，需要broker、producer、consumer都配置
  - **retries**：遇到可重试错误时，决定了生产者可重发消息的次数。默认为0，不进行重试。注意：**重试有可能引起消息错乱。**
  - **retry.backoff.ms**：可重新发送消息的时间间隔，默认100ms。
  - **compression.type**：决定发送消息时，消息是否被压缩。默认值none。可选："gzip", "snappy", "lz4"
  - **linger.size**：生产者发送同一批次的消息时，等待的时间。增加消息延时的同时，增加消息发送数量。默认值为0，生产者客户端会在Batch填满时或者超过"linger.ms"值时发送出去。
  - **receive.buffer.bytes**：用来设置Socket接收消息缓冲区（SO_RECBUF）的大小，默认为32KB
  - **send.buffer.bytes**：用来设置Socket发送消息缓冲区（SO_SNDBUF）的大小，默认128KB
  - **request.timeout.ms**：生产者在发送数据时等待服务器返回响应的时间。默认30s

  - **buffer.memory**：用来设置生产者的内存缓冲区大小。默认32MB
  - **batch.size**：同一批次可以发送的消息大小。默认16KB
  - **max.block.ms**：调用send方法阻塞（发送缓冲区已满）或者获取分区元数据阻塞（partation leader 选举）时，最大的block时间。默认60s。
  - **max.in.flight.requests.per.connection**：指定生产者在收到服务器响应之前可以发送多少个消息，默认5。值越大，占用内存越多，吞吐量越高；**设置为1，可以保证消息一定按照顺序写入服务器的，即使发生了重试**。
  - **timeout.ms**：指定broker等待同步副本返回消息确认的时间，与acks的配置相匹配：如果在指定时间内没有收到同步副本的确认，那么broker就会返回一个错误
  - **fetch.timeout.ms**：生产者在在获取broker元数据（例如partation leader是谁）时，等待服务器返回响应的时间

## 工具链

### connect-distributed.sh

### connect-mirror-maker.sh

### connect-standalone.sh

connect-xxx tools can import and export data from external data sources, such as Mysql, Es, Mongo..

### kafka-acls.sh

This tool helps to manage acls on kafka.

### kafka-broker-api-versions.sh

This tool helps to retrieve broker version information.

### kafka-configs.sh

配置项管理工具。配置项详见原理篇。

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

消费者终端测试工具

- `--bootstrap-server <String: host:port>`：服务地址

- `--from-beginning`：对应`earlist`策略。consumer启动时，默认采用`latest`策略

- `--isolation-level <String>`：消费隔离级别。可选：read_uncommitted/read_committed，默认uncommited

- `--partition <Integer>`：指定读取分区

- `--topic <String>`：Topic Name

  ```bash
  # 读取消息
  ./kafka-console-consumer.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --topic test --from-beginning
  ```

### kafka-console-producer.sh

生产者终端测试工具

- `--bootstrap-server <String: host:port>`：服务地址

- `--batch-size <Integer: size>`：批量生产消息数量。默认200

- `--compression-codec <String>`：压缩格式，可选gzip/snappy/lz4

- `--request-required-acks <Integer>`：需要多少个分区副本确认消息已接收。默认1

- `--request-timeout-ms <Integer>`：确认ack的超时时间，默认1500ms

- `--sync`：同步发送消息

- `--topic <String>`：Topic Name

  ```bash
  # 发送消息
  ./kafka-console-producer.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --topic test
  ```

### kafka-consumer-groups.sh

消费者组管理工具

- `--bootstrap-server <String: host:port>`：服务地址

- `--list`：list所有的consumer groups

- `--describe`：用于展示consumer group的具体信息

- `--delete`：用于删除consumer group

- `--delete-offsets`：用于删除某个consumer group在某个topic上的消费记录

- `--reset-offsets <String>`：重置offsets，可选：

  - `--to-current`
  - `--to-datetime <String: datetime>`：格式为：'YYYY-MM-DDTHH:mm:SS.sss'
  - `--to-earliest`
  - `--to-latest`
  - `--to-offset <Long: offset>`

- `--group <String>`：指定操作作用于某个具体的consumer group上

- `--all-groups`：指定所有的consumer groups

- `--topic <String>`：指定操作作用于某个具体的topic上

- `--all-topics`：指定所有的topics

- `--members`：获取consumer group中的所有members

  ```bash
  # 列出所有的consumer groups
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --list
  
  # 展示某个consumer group的详细信息
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --describe --group perf-consumer-28068
  
  # 删除某个consumer group
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --delete --group perf-consumer-28068
  
  # 删除某个consumer group上的offsets（其实这样做，就等于删除consumer group）
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --delete-offsets --group perf-consumer-6890 --topic test
  
  # 展示某个consumer group中活跃的members
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --describe --group perf-consumer-44443 --members
  
  # 根据条件重置consumer group
  ./kafka-consumer-groups.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --reset-offsets --to-earliest --group perf-consumer-28068 --topic test
  ```

### kafka-consumer-perf-test.sh

消费能力测试基准工具

- `--bootstrap-server <String: host:port>`：服务地址

- `--consumer.config <String: config file>`：消费配置项

- `--topic <String>`：Topic名称

- `--timeout <Integer: milliseconds>`：超时时间

- `--fetch-size <Integer: size>`：一次拉取的消息大小，默认1048576，即1MB

- `--from-latest`：默认使用earliest策略拉取消息，这个参数将使用latest策略

- `--messages <Interge>`：拉取消息的数量

  ```bash
  ./kafka-consumer-perf-test.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --topic test --messages 10000
  
  start.time, end.time, data.consumed.in.MB, MB.sec, data.consumed.in.nMsg, nMsg.sec, rebalance.time.ms, fetch.time.ms, fetch.MB.sec, fetch.nMsg.sec
  2021-11-04 19:11:08:397, 2021-11-04 19:11:08:733, 9.7427, 28.9962, 10216, 30404.7619, 1636024268653, -1636024268317, -0.0000, -0.0000
  ```

### kafka-delegation-tokens.sh

This tool helps to create, renew, expire, or describe delegation tokens.

### kafka-delete-records.sh

从Topic.Partition中删除Record

- `--bootstrap-server <String: host:port>`：服务地址

- `--offset-json-file <String>`

  ```json
  // json文件格式如下
  // 在开发中，应该没有这么奇怪的操作
  {
  	"partitions": [
  		{
  			"topic": "foo", 
  			"partition": 1,   
  			"offset": 1
  		}
  	],                      
  	"version":1                          
  }
  ```

### kafka-dump-log.sh

日志解析工具。

### kafka-features.sh

This tool describes and updates finalized features.

### kafka-leader-election.sh

分区leader副本选举工具

- `--bootstrap-server <String: host:port>`：服务地址

- `--election-type <String>`：选举类型，可选：preferred/unclean

- `--all-topic-partitions`：对所有topic的所有分区进行优先副本选举

- `--topic <String>`：选择topic

- `--partition <Integer>`：选择分区

  ```bash
  # 对全部Topic的全部分区执行优先级副本选举
  # 注：建议在空闲时间段执行
  ./kafka-leader-election.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --election-type preferred --all-topic-partitions
  
  # 对指定Topic、指定分区执行优先副本选举
  ./kafka-leader-election.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --election-type preferred --topic hello --partition 1
  
  # 对全部Topic的全部分区执行选举
  ./kafka-leader-election.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --election-type unclean --all-topic-partitions
  
  # 对指定Topic、指定分区进行副本选举
  ./kafka-leader-election.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --election-type unclean --topic hello --partition 0
  ```

### kafka-log-dirs.sh

This tool helps to query log directory usage on the specified brokers.

### kafka-mirror-maker.sh

This tool helps to continuously copy data between two Kafka clusters.

### kafka-producer-perf-test.sh

消费能力基准测试工具

- `--producer-props PROP-NAME=PROP-VALUE [PROP-NAME=PROP-VALUE ...]`：服务端配置

- `--topic <String>`：Topic名称

- `--num-records <Interge>`：测试消息数量

- `--record-size <Interge>`：消息大小。单位：bytes

- `--throughput <Integer>`：永远设置为-1

  ```bash
  ./kafka-producer-perf-test.sh --producer-props bootstrap.servers=0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 ack=0 --topic test --num-records 1000000 --record-size 1000 --throughput -1
  
  [2021-11-04 18:59:32,103] WARN The configuration 'ack' was supplied but isn't a known config. (org.apache.kafka.clients.producer.ProducerConfig)
  795857 records sent, 159171.4 records/sec (151.80 MB/sec), 180.7 ms avg latency, 624.0 ms max latency.
  1000000 records sent, 156225.589752 records/sec (148.99 MB/sec), 193.80 ms avg latency, 730.00 ms max latency, 146 ms 50th, 556 ms 95th, 691 ms 99th, 729 ms 99.9th.
  ```

### kafka-reassign-partitions.sh

分区重分配工具

> 如果仔细观察的话，这个脚本还可以添加、删除分区副本

- `--bootstrap-server <String: host:port>`：服务地址

- `--broker-list <String>`：指定将分区分配到那些Broker上，如：“1,2,3”

- `--throttle <Interge>`：复制限流。默认不限制，单位是："bytes/sec"

- `--generate`：生成分区重分配方案

- `--execute`：执行分区重分配方案

- `--list`：列出当前正在执行的分区重分配任务

- `--cancel`：取消一个正在进行中的分区重分配任务

- `--topics-to-move-json-file <String: Json File Path>`：由Topic生成的分区重分配方案，Json格式如下：

  ```json
  {
  	"topics": [ 
          {
  			"topic": "test"
  		}
  	],
  	"version": 1
  }
  ```

- `--reassignment-json-file <String: Json File Path>`：由Generate步骤生成的重分配方案

  ```bash
  # bg：3个Broker（1,2,3），test Topic，3 partitions，2 replicas
  # 展示初始状态
  # 可以发现，分区leader均匀的分布在broker 1,2,3上
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --describe --topic test
  Topic: test	PartitionCount: 3	ReplicationFactor: 2	Configs: 
  	Topic: test	Partition: 0	Leader: 1	Replicas: 1,3	Isr: 1,3
  	Topic: test	Partition: 1	Leader: 2	Replicas: 2,1	Isr: 1,2
  	Topic: test	Partition: 2	Leader: 3	Replicas: 3,2	Isr: 2,3
  
  ### 集群删除节点实验
  # 计划在集群中删除Broker 1，将分区均匀的分布在Broker 2和Broker 3上
  # 此处使用的reassign.json格式详见--topics-to-move-json-file选项
  # Current: 当前的分区分布
  # Proposed: 重分配之后的分区分布
  ./kafka-reassign-partitions.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --generate --topics-to-move-json-file reassign.json --broker-list 2,3
  Current partition replica assignment
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[1,3],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[2,1],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[3,2],"log_dirs":["any","any"]}]}
  
  Proposed partition reassignment configuration
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[3,2],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[2,3],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[3,2],"log_dirs":["any","any"]}]}
  
  # 保存上面的Proposed Json文件，执行：
  ./kafka-reassign-partitions.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --execute --reassignment-json-file plan.json
  Current partition replica assignment
  
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[1,3],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[2,1],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[3,2],"log_dirs":["any","any"]}]}
  
  Save this to use as the --reassignment-json-file option during rollback
  Successfully started partition reassignments for test-0,test-1,test-2
  
  # 展示当前状态
  # 可以看到，分区leader及分区副本仅分配在broker 2,3上
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --describe --topic test
  Topic: test	PartitionCount: 3	ReplicationFactor: 2	Configs: 
  	Topic: test	Partition: 0	Leader: 3	Replicas: 3,2	Isr: 3,2
  	Topic: test	Partition: 1	Leader: 2	Replicas: 2,3	Isr: 2,3
  	Topic: test	Partition: 2	Leader: 3	Replicas: 3,2	Isr: 2,3
  
  ### 集群新增节点实验
  # 计划在集群中新增broker 1
  # 此处使用的reassign.json格式详见--topics-to-move-json-file选项
  ./kafka-reassign-partitions.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --generate --topics-to-move-json-file reassign.json --broker-list 1,2,3
  Current partition replica assignment
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[3,2],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[2,3],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[3,2],"log_dirs":["any","any"]}]}
  
  Proposed partition reassignment configuration
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[2,3],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[3,1],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[1,2],"log_dirs":["any","any"]}]}
  
  # 保存Proposed Json文件，执行：
  ./kafka-reassign-partitions.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --execute --reassignment-json-file plan.json
  Current partition replica assignment
  
  {"version":1,"partitions":[{"topic":"test","partition":0,"replicas":[3,2],"log_dirs":["any","any"]},{"topic":"test","partition":1,"replicas":[2,3],"log_dirs":["any","any"]},{"topic":"test","partition":2,"replicas":[3,2],"log_dirs":["any","any"]}]}
  
  Save this to use as the --reassignment-json-file option during rollback
  Successfully started partition reassignments for test-0,test-1,test-2
  
  # 展示当前状态
  # 可以发现，分区副本已经均匀分布至broker 1,2,3上
  # 但是分区副本没有均匀分布在broker上
  # 这种情况下，便需要执行一次优先副本选举，来保证集群负载均衡
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --describe --topic test
  Topic: test	PartitionCount: 3	ReplicationFactor: 2	Configs: 
  	Topic: test	Partition: 0	Leader: 3	Replicas: 2,3	Isr: 3,2
  	Topic: test	Partition: 1	Leader: 3	Replicas: 3,1	Isr: 3,1
  	Topic: test	Partition: 2	Leader: 1	Replicas: 1,2	Isr: 2,1
  
  # 优先副本选举
  ./kafka-leader-election.sh --bootstrap-server 0.0.0.0:9092,0.0.0.0:9093,0.0.0.0:9094 --election-type preferred --all-topic-partitions
  
  # 展示当前状态
  # 此时，leader副本已经均匀的分布在broker上
  ./kafka-topics.sh --zookeeper 0.0.0.0:2181 --describe --topic testTopic: test	PartitionCount: 3	ReplicationFactor: 2	Configs: 
  	Topic: test	Partition: 0	Leader: 2	Replicas: 2,3	Isr: 3,2
  	Topic: test	Partition: 1	Leader: 3	Replicas: 3,1	Isr: 3,1
  	Topic: test	Partition: 2	Leader: 1	Replicas: 1,2	Isr: 2,1
  ```

### kafka-replica-verification.sh

Validate that all replicas for a set of topics have the same data.

### kafka-run-class.sh

Unknown Tool

### kafka-server-start.sh

Start Service

### kafka-server-stop.sh

Stop Service

### kafka-streams-application-reset.sh

Kafka Stream Tool

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

Such as kafka-console-consumer.sh

### kafka-verifiable-producer.sh

Such as kafka-console-producer.sh

## 监控

TODO...