package main

import (
	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"
	"io"
	"net/http"
	"net/url"
	"os"
	"sync"
)

func main() {
	wg := sync.WaitGroup{}
	wg.Add(3)

	go func() {
		defer wg.Done()

		r := gin.New()
		r.GET("/ping", func(c *gin.Context) {
			logrus.Info("recv 38080")
			c.JSON(200, gin.H{
				"message": "pong",
			})
		})

		r.GET("/bigfile", func(c *gin.Context) {
			
		})

		server := &http.Server{
			Addr:         "0.0.0.0:38080",
			Handler:      r,
			// IdleTimeout:  30 * time.Second,
		}

		server.ListenAndServe()
	}()

	go func() {
		defer wg.Done()

		r := gin.New()
		r.GET("/ping", func(c *gin.Context) {
			logrus.Info("recv 38081")
			c.JSON(200, gin.H{
				"message": "pong",
			})
		})
		http.ListenAndServe("0.0.0.0:38081", r)
	}()

	go func() {
		defer wg.Done()

		http.HandleFunc("/bigfile", func(w http.ResponseWriter, _ *http.Request) {
			w.Header().Set("Content-Disposition", "attachment; filename="+url.QueryEscape("测试.mp4"))
			w.Header().Set("Content-Type", "application/x-zip-compressed")

			f, _ := os.Open("/home/sensetime/1.mp4")
			defer f.Close()

			io.Copy(w, f)
		})

		http.ListenAndServe("0.0.0.0:38082", nil)
	}()

	wg.Wait()
}
