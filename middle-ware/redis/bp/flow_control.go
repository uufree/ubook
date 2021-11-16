package main

import (
	"context"
	"github.com/go-redis/redis/v8"
	log "github.com/sirupsen/logrus"
	"sync"
	"time"
)

func main() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "cell").Err(); err != nil {
		log.Fatal(err)
	}

	start := time.Now()
	wg := sync.WaitGroup{}
	wg.Add(20)
	for i:=0; i<20; i++ {
		go func(index int) {
			defer wg.Done()
			for {
				// 10s内，仅处理10个请求
				results, err := rdb.Do(context.Background(), "cl.throttle", "cell", 5, 10, 10).Int64Slice()
				if err != nil {
					log.Fatal(err)
				}

				if results[0] == 0 {
					log.Infof("index %v pass", index)
					return
				}

				time.Sleep(100 * time.Millisecond)
			}
		}(i)
	}

	wg.Wait()
	log.Infof("spend: %+v", time.Now().Sub(start))
}