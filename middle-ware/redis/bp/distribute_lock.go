package main

import (
	"context"
	"fmt"
	"github.com/go-redis/redis/v8"
	"github.com/go-redsync/redsync/v4"
	"github.com/go-redsync/redsync/v4/redis/goredis/v8"
	"github.com/google/uuid"
	"log"
	"sync"
	"time"
)

func main() {
	sl := Redlock{}
	if err := sl.Init(); err != nil {
		log.Fatalf("init redis client failed, err: %v", err)
	}

	UnLockDemo(&sl)
	LockDemo(&sl)
}

type Single struct {
	lock         sync.Mutex
	rdb          *redis.Client
	randomString string
	scriptSha256 string
}

type DistributeLock interface {
	Init() error
	Lock() error
	UnLock() error
	Set(key string, Value interface{}) error
	Get(key string) (int, error)
	Del(key string) error
}

func (s *Single) Init() error {
	s.rdb = redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	return nil
}

func (s *Single) Lock() error {
	s.lock.Lock()
	defer s.lock.Unlock()

	uid := uuid.New().String()
	if err := s.rdb.SetArgs(context.Background(), "dlock", uid, redis.SetArgs{
		Mode: "NX",
		TTL:  5 * time.Second,
	}).Err(); err != nil {
		return err
	}
	s.randomString = uid

	return nil
}

func (s *Single) UnLock() error {
	s.lock.Lock()
	defer s.lock.Unlock()

	var unlock = redis.NewScript(`
if redis.call("get", KEYS[1]) == ARGV[1] then 
  	return redis.call("del", KEYS[1]) 
else 
  	return 0 
end
`)

	res, err := unlock.Run(context.Background(), s.rdb, []string{"dlock"}, s.randomString).Int64()
	if err != nil {
		return err
	}

	s.randomString = ""
	if res == 0 {
		return fmt.Errorf("lock has expired")
	}
	return nil
}

func (s *Single) Get(key string) (int, error) {
	return s.rdb.Get(context.Background(), key).Int()
}

func (s *Single) Set(key string, value interface{}) error {
	return s.rdb.Set(context.Background(), key, value, redis.KeepTTL).Err()
}

func (s *Single) Del(key string) error {
	return s.rdb.Del(context.Background(), key).Err()
}

func UnLockDemo(d DistributeLock) error {
	if err := d.Del("sell_count"); err != nil {
		return err
	}

	if err := d.Set("sell_count", 0); err != nil {
		return err
	}

	// expect 5050
	wg := sync.WaitGroup{}
	for i := 1; i <= 100; i++ {
		wg.Add(1)
		go func(index int) {
			defer wg.Done()

			count, err := d.Get("sell_count")
			if err != nil {
				log.Fatalf("incr failed, err: %v", err)
				return
			}

			if err := d.Set("sell_count", count+index); err != nil {
				log.Fatalf("incr count failed, err: %v", err)
				return
			}
		}(i)
	}

	wg.Wait()

	count, err := d.Get("sell_count")
	if err != nil {
		log.Fatalf("get failed, err: %v", err)
	}
	log.Printf("count: %v\n", count)
	return nil
}

func LockDemo(d DistributeLock) error {
	if err := d.Del("sell_count"); err != nil {
		return err
	}

	if err := d.Set("sell_count", 0); err != nil {
		return err
	}

	// expect 5050
	wg := sync.WaitGroup{}
	for i := 1; i <= 100; i++ {
		wg.Add(1)
		go func(index int) {
			defer wg.Done()

			for {
				if err := d.Lock(); err != nil {
					time.Sleep(10 * time.Millisecond)
				} else {
					break
				}
			}

			defer d.UnLock()

			count, err := d.Get("sell_count")
			if err != nil {
				log.Fatalf("incr failed, err: %v", err)
				return
			}

			if err := d.Set("sell_count", count+index); err != nil {
				log.Fatalf("incr count failed, err: %v", err)
				return
			}
		}(i)
	}

	wg.Wait()

	count, err := d.Get("sell_count")
	if err != nil {
		log.Fatalf("get failed, err: %v", err)
	}
	log.Printf("count: %v\n", count)
	return nil
}

type Redlock struct {
	rdb       *redis.Client
	mutex     *redsync.Mutex
	mutexname string
}

func (r *Redlock) Init() error {
	r.rdb = redis.NewClient(&redis.Options{
		Addr:     ":6379",
		DB:       0,
		Password: "sensetime",
	})

	pool := goredis.NewPool(r.rdb)

	// 这个步骤可以加多个redis实例
	rs := redsync.New(pool)

	r.mutexname = "my-global-mutex"
	r.mutex = rs.NewMutex(r.mutexname)
	return nil
}

func (r *Redlock) Lock() error {
	if err := r.mutex.Lock(); err != nil {
		return err
	}
	return nil
}

func (r *Redlock) UnLock() error {
	if ok, err := r.mutex.Unlock(); !ok || err != nil {
		return fmt.Errorf("unlock failed, err: %v", err)
	}
	return nil
}

func (r *Redlock) Get(key string) (int, error) {
	return r.rdb.Get(context.Background(), key).Int()
}

func (r *Redlock) Set(key string, value interface{}) error {
	return r.rdb.Set(context.Background(), key, value, redis.KeepTTL).Err()
}

func (r *Redlock) Del(key string) error {
	return r.rdb.Del(context.Background(), key).Err()
}
