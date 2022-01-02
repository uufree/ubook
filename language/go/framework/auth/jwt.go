package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"time"
)

func main() {
	router := gin.New()
	// control log format
	router.Use(gin.LoggerWithFormatter(func(param gin.LogFormatterParams) string {
		return fmt.Sprintf("%s - [%s] \"%s %s %s %d %s \"%s\" %s\"\n",
			param.ClientIP,
			param.TimeStamp.Format(time.RFC1123),
			param.Method,
			param.Path,
			param.Request.Proto,
			param.StatusCode,
			param.Latency,
			param.Request.UserAgent(),
			param.ErrorMessage,
		)
	}))

	// offical recover middleware
	router.Use(gin.Recovery())

	// registry handler
	v1 := router.Group("/api/v1/")

	// user
	v1.POST("/user")
	v1.DELETE("/user/:id")
	v1.PUT("/user/:id")

	// token
	v1.GET("/auth/login")
	v1.GET("/auth/logout")
	v1.GET("/auth/refresh")
	v1.GET("/auth/test")
}
