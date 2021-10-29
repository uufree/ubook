# Kafka原理篇

[TOC]

## 概述

Kafka是一个分布式流处理平台，它以**高吞吐、可持久化、可水平拓展、支持流数据处理**等多种特性被广泛使用。Kafka常在应用系统中扮演3种角色：

1. 消息系统

   作为消息中间件，Kafka具有：**系统解耦、流量削峰、缓冲、异步通信、高拓展性、高可用性**等基础特性。此外，还提供了**消息顺序性保障**及**回溯消费**的能力。

2. 存储系统

   Kafka 把消息持久化到磁盘，相比于其他基于内存存储的系统而言，有效地降低了数据丢失的风险。也正是得益于 Kafka 的消息持久化功能和多副本机制，我们可以把 Kafka 作为长期的数据存储系统来使用，只需要把对应的数据保留策略设置为“永久”或启用Topic的日志压缩功能即可。

3. 流处理平台

   Kafka提供了一个完整的流式处理类库，比如窗口、连接、变换和聚合等各类操作。

## 术语

- Producer：生产者

- Consumer：消费者

- Broker：服务节点。可以看做是一个Kafka实例

- Topic：消息以Topic为单位进行归类。Producer将消息发往Topic；Consumer从Topic中读取消息

- Partition：一个Topic可以有多个Partition，每个Partition负责维护自己的Offset。Kafka就是通过这个Offset保证消息在分区内是有序的。Partition横向拓展了Kafka的吞吐能力

- AR（分区副本，Assigned Replicas）：Kafka提供分区多副本机制，通过增加分区副本的数量来提高容灾能力。分区副本为主从结构，其中：**Leader副本负责处理读写；Follower副本仅负责与Leader副本的消息同步**。

- ISR（In-Sync Replicas）：与Leader副本保持同步的一组副本（包括Leader副本），是AR的一个子集。

- OSR（Out-Of-Sync Replicas）：与Leader副本同步滞后过多的副本（不包括Leader副本），是AR的一个子集

- HW（High Watermark）：高水位，标识了一个特定的偏移量，消费者只能拉取到这个偏移量之前的消息

- LEO（Log End Offset）：标识当前日志文件中下一条待写入消息的Offset

  ![image-20211028205157261](assets/image-20211028205157261.png)

