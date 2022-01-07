package main

import (
	"bytes"
	"context"
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"sync"
	"time"

	"github.com/google/uuid"
)

const (
	FileChunk = 1 * 1024 * 1024
	MaxConn   = 8
	Retry     = 3

	Method = "POST"
	URL    = "http://127.0.0.1:38999/v1/multipart_upload"
)

func makeFileChunk(filePath string) ([][]byte, error) {
	f, err := os.Open(filePath)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	fileBytes, err := io.ReadAll(f)
	if err != nil {
		return nil, err
	}

	index := 0
	result := make([][]byte, 0, len(fileBytes)/FileChunk+1)
	for index*FileChunk < len(fileBytes) {
		result = append(result, fileBytes[index*FileChunk:(index+1)*FileChunk])
		index += 1
	}

	return result, nil
}

func retryFunc(funcObj func(string, int, int, []byte) error, param0 string, param1, param2 int, param3 []byte) error {
	var err error = nil
	for i := 0; i < Retry; i++ {
		if err = funcObj(param0, param1, param2, param3); err != nil {
			time.Sleep(time.Duration(i+1) * time.Second)
			continue
		}
		break
	}
	return err
}

func handleChunk(uid string, index, allChunks int, chunk []byte) error {
	payload := &bytes.Buffer{}
	writer := multipart.NewWriter(payload)
	mw, err := writer.CreateFormField("file")
	if err != nil {
		return err
	}

	_, err = io.Copy(mw, bytes.NewBuffer(chunk))
	if err != nil {
		return err
	}

	if err := writer.Close(); err != nil {
		return err
	}

	req, err := http.NewRequestWithContext(context.Background(), Method, URL, payload)
	if err != nil {
		return err
	}

	md5Sum := md5.Sum(chunk)
	req.Header.Add("Chunk-Info", fmt.Sprintf("%v/%v", index, allChunks))
	req.Header.Add("Md5", hex.EncodeToString(md5Sum[:]))
	req.Header.Add("Content-Type", writer.FormDataContentType())
	req.Header.Add("Uid", uid)

	rsp, err := http.DefaultClient.Do(req)
	if err != nil {
		return err
	}
	defer rsp.Body.Close()

	if rsp.StatusCode != http.StatusOK {
		return fmt.Errorf("invalid status code: %v", rsp.StatusCode)
	}

	return nil
}

func uploadChunk(chunks [][]byte) error {
	uid := uuid.New().String()

	wg := sync.WaitGroup{}
	wg.Add(MaxConn + 1)

	inCh := make(chan int, MaxConn)
	errCh := make(chan int, MaxConn)
	go func() {
		defer wg.Done()
		for i := 0; i < len(chunks); i++ {
			inCh <- i
		}
		close(inCh)
	}()

	for i := 0; i < MaxConn; i++ {
		go func() {
			defer wg.Done()
			for {
				select {
				case index, ok := <-inCh:
					if !ok {
						return
					}

					if err := retryFunc(handleChunk, uid, index, len(chunks), chunks[index]); err != nil {
						log.Println(err)
						errCh <- index
						return
					}
				}
			}
		}()
	}

	wg.Wait()
	return nil
}

func main() {
	fileChunks, err := makeFileChunk("/home/sensetime/Downloads/测试.mp4")
	if err != nil {
		log.Fatal(err)
	}

	if err := uploadChunk(fileChunks); err != nil {
		log.Fatal(err)
	}
}
