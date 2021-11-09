package main

import (
	"context"
	"github.com/Shopify/sarama"
	"github.com/hashicorp/go-uuid"
	log "github.com/sirupsen/logrus"
	"time"
)

const (
	ConsumerGroup = "consumer-group"
	Topic         = "test"
)

var (
	KafkaServer = []string{"0.0.0.0:9092", "0.0.0.0:9093", "0.0.0.0:9094"}
)

func GetUUID() string {
	u, _ := uuid.GenerateUUID()
	return u
}

type KafkaConsumer struct {
	cg     sarama.ConsumerGroup
	ctx    context.Context
	conf   *sarama.Config
	cancel context.CancelFunc
	topics []string
}

func NewKafkaConsumer(ctx context.Context, topics []string, conf *sarama.Config) *KafkaConsumer {
	cg, err := sarama.NewConsumerGroup(KafkaServer, ConsumerGroup, conf)
	if err != nil {
		log.Fatal(err)
	}

	kc := &KafkaConsumer{
		cg:     cg,
		ctx:    ctx,
		conf:   conf,
		topics: topics,
	}

	return kc
}

func (kc *KafkaConsumer) Run() {
	for {
		log.Info(time.Now())
		err := kc.cg.Consume(kc.ctx, kc.topics, kc)
		if err != nil {
			log.Fatal("consume msg failed, err: %v", err)
		}

		if kc.ctx.Err() != nil {
			break
		}
	}
}

func (kc *KafkaConsumer) Close() error {
	return kc.cg.Close()
}

func (kc *KafkaConsumer) Setup(sarama.ConsumerGroupSession) error {
	return nil
}

func (kc *KafkaConsumer) Cleanup(sarama.ConsumerGroupSession) error {
	return nil
}

func (kc *KafkaConsumer) ConsumeClaim(session sarama.ConsumerGroupSession, claim sarama.ConsumerGroupClaim) error {
	for message := range claim.Messages() {
		log.Printf("Message claimed: value = %s, timestamp = %v, topic = %s", string(message.Value), message.Timestamp, message.Topic)
		session.MarkMessage(message, "")
	}

	return nil
}

func main() {
	conf := sarama.NewConfig()
	conf.Consumer.Offsets.Initial = sarama.OffsetOldest
	conf.Consumer.Group.Rebalance.Strategy = sarama.BalanceStrategyRoundRobin
	conf.Version = sarama.V2_7_0_0
	conf.ClientID = GetUUID()

	ctx, cancel := context.WithCancel(context.Background())
	kc := NewKafkaConsumer(ctx, []string{Topic}, conf)

	go kc.Run()

	defer kc.Close()
	defer cancel()

	time.Sleep(10000 * time.Second)
}
