package main

import (
	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"
	"net/http"
	"sync"
)

func main() {
	wg := sync.WaitGroup{}
	wg.Add(2)

	go func() {
		r := gin.New()
		r.GET("/ping", func(c *gin.Context) {
			logrus.Info("recv 38080")
			c.JSON(200, gin.H{
				"message": "pong",
			})
		})

		server := &http.Server{
			Addr:         "0.0.0.0:38080",
			Handler:      r,
			// IdleTimeout:  30 * time.Second,
		}

		server.ListenAndServe()
	}()

	go func() {
		r := gin.New()
		r.GET("/ping", func(c *gin.Context) {
			logrus.Info("recv 38081")
			c.JSON(200, gin.H{
				"message": "pong",
			})
		})
		http.ListenAndServe("0.0.0.0:38081", r)
	}()

	wg.Wait()
}
