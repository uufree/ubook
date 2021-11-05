# Kafka运维篇

[TOC]

## 部署

详见deploy文件夹。

## 配置项

- Broker
- Topic
- Consumer
- Producer

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

