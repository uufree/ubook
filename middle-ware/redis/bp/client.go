package main

import (
	"context"
	"github.com/go-redis/redis/v8"
	log "github.com/sirupsen/logrus"
	"time"
)

const (
	SINGLE   = 0
	SENTINEL = 1
	CLUSTER  = 2
	RING     = 3
	GLOBAL   = 4
)

func main() {
	usage := RING

	switch usage {
	case SINGLE:
		single()
	case SENTINEL:
		sentinel()
	case CLUSTER:
		cluster()
	case RING:
		ring()
	case GLOBAL:
		global()
	default:
		log.Fatal("invalid usage")
	}
}

func global() {
	// redis包级别除了NewClient*系列的函数之外，没有有用的global function
}

func single() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})

	rdb.SetArgs(context.Background(), "name", "uuchen", redis.SetArgs{
		Mode: "NX",
		TTL:  3 * time.Second,
	})

	value, err := rdb.Get(context.Background(), "name").Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("value: %v", value)

	if err := rdb.Del(context.Background(), "name").Err(); err != nil {
		log.Fatal(err)
	}

	redis.NewRing(&redis.RingOptions{

	})
}

func sentinel() {
	// 采用这种方式，可以将read command均分到slave上
	sentinel := redis.NewFailoverClusterClient(&redis.FailoverOptions{
		MasterName: "mymaster",
		SentinelAddrs: []string{
			"0.0.0.0:26379",
			"0.0.0.0:26380",
			"0.0.0.0:26381",
		},
		RouteRandomly: true,
		Password:      "sensetime",
		DB:            0,
	})

	if err := sentinel.Del(context.Background(), "name").Err(); err != nil {
		log.Fatal(err)
	}

	sentinel.SetArgs(context.Background(), "name", "uuchen", redis.SetArgs{
		Mode: "NX",
		TTL:  5 * time.Second,
	})

	for i := 0; i < 10000; i++ {
		value, err := sentinel.Get(context.Background(), "name").Result()
		if err != nil {
			log.Fatal(err)
		}
		log.Infof("value: %v", value)
	}

	if err := sentinel.Del(context.Background(), "name").Err(); err != nil {
		log.Fatal(err)
	}
}

func cluster() {
	cluster := redis.NewClusterClient(&redis.ClusterOptions{
		Addrs: []string{
			"0.0.0.0:6379",
			"0.0.0.0:6380",
			"0.0.0.0:6381",
			"0.0.0.0:6382",
			"0.0.0.0:6383",
			"0.0.0.0:6384",
		},
		Password: "sensetime",
	})

	if err := cluster.Del(context.Background(), "name").Err(); err != nil {
		log.Fatal(err)
	}

	cluster.SetArgs(context.Background(), "name", "uuchen", redis.SetArgs{
		Mode: "NX",
		TTL:  5 * time.Second,
	})

	for i := 0; i < 10000; i++ {
		value, err := cluster.Get(context.Background(), "name").Result()
		if err != nil {
			log.Fatal(err)
		}
		log.Infof("value: %v", value)
	}
}

func ring() {
	ring := redis.NewRing(&redis.RingOptions{
		Addrs: map[string]string{
			"n1": "0.0.0.0:6379",
		},
		Password: "sensetime",
		DB:       0,
	})

	if err := ring.Del(context.Background(), "name").Err(); err != nil {
		log.Fatal(err)
	}

	ring.SetArgs(context.Background(), "name", "uuchen", redis.SetArgs{
		Mode: "NX",
		TTL:  5 * time.Second,
	})

	for i := 0; i < 10000; i++ {
		value, err := ring.Get(context.Background(), "name").Result()
		if err != nil {
			log.Fatal(err)
		}
		log.Infof("value: %v", value)
	}
}
