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

	if err := rdb.Del(context.Background(), "bf").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.Do(context.Background(), "bf.reserve", "bf", 0.01, 10000).Err(); err != nil {
		log.Fatal(err)
	}

	for i := 0; i < 10000; i++ {
		if err := rdb.Do(context.Background(), "bf.add", "bf", i).Err(); err != nil {
			log.Fatal(err)
		}
	}

	m := make(map[int64]int64)
	for i:=0; i<20000; i++ {
		e, err := rdb.Do(context.Background(), "bf.exists", "bf", i).Result()
		if err != nil {
			log.Fatal(err)
		}

		v, ok := e.(int64)
		if !ok {
			log.Fatal(ok)
		}

		m[v] = m[v] + 1
	}

	for v, c := range m {
		log.Info(v, c)
	}
}