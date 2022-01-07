package main

import (
	"context"
	"github.com/Shopify/sarama"
	"github.com/hashicorp/go-uuid"
	log "github.com/sirupsen/logrus"
	"time"
)

const (
	ProducerTopic = "test"
)

var (
	ProducerServers = []string{"0.0.0.0:9092", "0.0.0.0:9093", "0.0.0.0:9094"}
)

type KafkaProducer struct {
	ctx  context.Context
	sp   sarama.SyncProducer
	conf *sarama.Config
}

func GetPUUID() string {
	u, _ := uuid.GenerateUUID()
	return u
}

func (kp *KafkaProducer) Run(ctx context.Context) error {
	ticker := time.NewTicker(100 * time.Millisecond)
	for {
		select {
		case <-ticker.C:
			_, _, err := kp.sp.SendMessage(&sarama.ProducerMessage{
				Topic: ProducerTopic,
				Key:   sarama.ByteEncoder(GetPUUID()),
				Value: sarama.ByteEncoder(GetPUUID()),
			})

			if err != nil {
				return err
			}
		case <-ctx.Done():
			return nil
		}
	}

	return nil
}

func (kp *KafkaProducer) Close() error {
	return kp.sp.Close()
}

func NewkafkaProducer(ctx context.Context, servers []string, conf *sarama.Config) *KafkaProducer {
	sp, err := sarama.NewSyncProducer(servers, conf)
	if err != nil {
		log.Fatal(err)
	}

	kp := &KafkaProducer{
		ctx:  ctx,
		sp:   sp,
		conf: conf,
	}

	return kp
}

func main() {
	conf := sarama.NewConfig()
	conf.ClientID = GetPUUID()
	conf.Producer.Return.Successes = true
	conf.Producer.Return.Errors = true
	conf.Version = sarama.V2_7_0_0

	kp := NewkafkaProducer(context.Background(), ProducerServers, conf)
	defer kp.Close()

	ctx, _ := context.WithTimeout(context.Background(), 3 * time.Minute)
	err := kp.Run(ctx)
	if err != nil {
		log.Fatal(err)
	}
}
