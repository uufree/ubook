package main

import (
	"context"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strconv"
	"sync"
)

const (
	MaxConnection = 8
	ChunkLength   = 1 * 1024 * 1024
)

const (
	RequestURL = "http://127.0.0.1:80/1.mp4"

	KeyContentLength  = "Content-Length"
	KeyAcceptRanges   = "Accept-Ranges"
	ValueAcceptRanges = "bytes"
)

func head(url string) (int64, error) {
	req, err := http.NewRequest("HEAD", url, nil)
	if err != nil {
		return 0, err
	}

	rsp, err := http.DefaultClient.Do(req)
	if err != nil {
		return 0, err
	}
	defer rsp.Body.Close()

	sContentLength := rsp.Header.Get(KeyContentLength)
	contentLength, err := strconv.ParseInt(sContentLength, 10, 64)
	if err != nil {
		return 0, err
	}

	if ValueAcceptRanges != rsp.Header.Get(KeyAcceptRanges) {
		return 0, fmt.Errorf("not found accept-ranges field")
	}

	return contentLength, nil
}

func handle(url string, index int) ([]byte, error) {
	req, err := http.NewRequestWithContext(context.Background(), "GET", url, nil)
	if err != nil {
		return nil, err
	}

	req.Header.Add("Range", fmt.Sprintf("bytes=%v-%v", index*ChunkLength, (index+1)*ChunkLength-1))

	rsp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}
	defer rsp.Body.Close()

	if rsp.StatusCode != http.StatusPartialContent {
		return nil, fmt.Errorf("invalid status code: %v", rsp.StatusCode)
	}

	buf, err := ioutil.ReadAll(rsp.Body)
	if err != nil {
		return nil, err
	}

	return buf, nil
}

func main() {
	lock := sync.Mutex{}
	cache := make(map[int][]byte)
	wg := sync.WaitGroup{}
	wg.Add(MaxConnection + 1)

	contentLength, err := head(RequestURL)
	if err != nil {
		log.Fatal(err)
	}

	inCh := make(chan int, MaxConnection)
	go func() {
		defer wg.Done()
		for i := 0; i < int(contentLength/ChunkLength)+1; i++ {
			inCh <- i
		}
		close(inCh)
	}()

	for i := 0; i < MaxConnection; i++ {
		go func() {
			defer wg.Done()
			for {
				select {
				case index, ok := <-inCh:
					if !ok {
						return
					}

					bs, err := handle(RequestURL, index)
					if err != nil {
						log.Fatal(err)
					}

					lock.Lock()
					cache[index] = bs
					lock.Unlock()
				}
			}
		}()
	}

	wg.Wait()

	// merge
	wf, err := os.Create("./1.mp4")
	if err != nil {
		log.Fatal(err)
	}
	defer wf.Close()

	for i := 0; i < int(contentLength/ChunkLength)+1; i++ {
		wf.Write(cache[i])
	}
}
