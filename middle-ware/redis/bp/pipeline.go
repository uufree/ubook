package main

import (
	"context"
	"github.com/go-redis/redis/v8"
	log "github.com/sirupsen/logrus"
)

func main() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	p := rdb.Pipeline()
	p.Get(context.Background(), "pl")
	p.Get(context.Background(), "pl")
	p.Get(context.Background(), "pl")

	result, _ := p.Exec(context.Background())
	for _, res := range result {
		log.Println(res.String())
	}

	txp := rdb.TxPipeline()
	txp.Get(context.Background(), "pl")
	txp.Get(context.Background(), "pl")
	txp.Get(context.Background(), "pl")

	txpr, _ := txp.Exec(context.Background())
	for _, res := range txpr {
		log.Println(res.String())
	}
}