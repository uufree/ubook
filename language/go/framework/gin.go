package main

import (
	"bytes"
	"fmt"
	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"time"
)

var (
	getting  gin.HandlerFunc
	posting  gin.HandlerFunc
	putting  gin.HandlerFunc
	deleting gin.HandlerFunc
	patching gin.HandlerFunc
	head     gin.HandlerFunc
	options  gin.HandlerFunc
)

// Using GET, POST, PUT, PATCH, DELETE and OPTIONS
func simple1() {
	router := gin.New()

	router.GET("/get", getting)
	router.POST("/somePost", posting)
	router.PUT("/somePut", putting)
	router.DELETE("/someDelete", deleting)
	router.PATCH("/somePatch", patching)
	router.HEAD("/someHead", head)
	router.OPTIONS("/someOptions", options)

	router.Run(":8080")
}

// Parameters in path
func simple2() {
	router := gin.New()

	// This handler will match /user/john but will not match /user/ or /user
	router.GET("/user/:name", func(c *gin.Context) {
		name := c.Param("name")
		c.String(http.StatusOK, "Hello %s", name)
	})

	// However, this one will match /user/john/ and also /user/john/send
	// If no other routers match /user/john, it will redirect to /user/john/
	router.GET("/user/:name/:action", func(c *gin.Context) {
		name := c.Param("name")
		action := c.Param("action")
		message := name + " is " + action
		c.String(http.StatusOK, message)
	})

	router.Run(":8080")
}

// Parameters in Querystring
func simple3() {
	router := gin.New()

	router.GET("/welcome", func(c *gin.Context) {
		firstname := c.DefaultQuery("firstname", "Guest")
		lastname := c.Query("lastname") // shortcut for c.Request.URL.Query().Get("lastname")

		c.String(http.StatusOK, "Hello %s %s", firstname, lastname)
	})
	router.Run(":8080")
}

// Multipart/Urlencoded Form
func simple4() {
	router := gin.New()

	router.POST("/form_post", func(c *gin.Context) {
		message := c.PostForm("message")
		nick := c.DefaultPostForm("nick", "anonymous")

		c.JSON(200, gin.H{
			"status":  "posted",
			"message": message,
			"nick":    nick,
		})
	})
	router.Run(":8080")
}

// Upload files
func simple5() {
	router := gin.New()

	// single file
	router.POST("/upload", func(c *gin.Context) {
		file, err := c.FormFile("file")
		if err != nil {
			c.JSON(400, "failed!")
		}

		logrus.Info(file.Filename)
		rc, err := file.Open()
		if err != nil {
			c.JSON(400, "failed!!")
		}
		defer rc.Close()

		f, err := os.Create(file.Filename)
		if err != nil {
			c.JSON(400, "failed!!!")
		}
		defer f.Close()

		io.Copy(f, rc)
	})

	// multiple files
	router.POST("/uploads", func(c *gin.Context) {
		form, err := c.MultipartForm()
		if err != nil {
			c.JSON(400, "failed!")
		}

		for _, files := range form.File {
			if len(files) != 1 {
				continue
			}

			file := files[0]
			rc, err := file.Open()
			if err != nil {
				c.JSON(400, "failed!!")
			}

			f, err := os.Create(file.Filename)
			if err != nil {
				c.JSON(400, "failed!!!")
			}

			io.Copy(f, rc)

			f.Close()
			rc.Close()
		}
	})

	router.Run(":8080")
}

// Group
func simple6() {
	router := gin.New()

	v1 := router.Group("/v1")
	v1.GET("/get", getting)
	v1.POST("/somePost", posting)
	v1.PUT("/somePut", putting)
	v1.DELETE("/someDelete", deleting)

	v2 := router.Group("/v2")
	v2.GET("/get", getting)
	v2.POST("/somePost", posting)
	v2.PUT("/somePut", putting)
	v2.DELETE("/someDelete", deleting)

	router.Run(":8080")
}

// use middleware
func simple7() {
	router := gin.New()

	// custom middleware
	router.Use(func(c *gin.Context) {
		// before
		for k, v := range c.Request.Header {
			logrus.Info(k, v)
		}
		logrus.Info("before")

		c.Next()

		// after
		logrus.Info("after")
	})

	router.Use(func(c *gin.Context) {
		logrus.Info("before 1")
		c.Next()
		logrus.Info("after 1")
	})

	// offical logger,  write the logs to gin.DefaultWriter
	router.Use(gin.Logger())

	// custom logger
	// control log color, open
	gin.ForceConsoleColor()
	gin.DisableConsoleColor()

	// Logging to file.
	f, _ := os.Create("gin.log")
	gin.DefaultWriter = io.MultiWriter(f)

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

	// custom recover middleware
	router.Use(gin.CustomRecovery(func(c *gin.Context, recovered interface{}) {
		if err, ok := recovered.(string); ok {
			c.String(http.StatusInternalServerError, fmt.Sprintf("error: %s", err))
		}
		c.AbortWithStatus(http.StatusInternalServerError)
	}))

	router.GET("/test", func(c *gin.Context) {
		logrus.Info("middle")
		c.JSON(200, "pong")
	})

	router.Run(":8080")
}

// bind model
// 参数校验不建议使用validator，有以下两个原因：
// 1. 参数之间可能存在关联
// 2. 出现错误时，可能需要使用自定义的错误码
func simple8() {
	router := gin.New()

	router.POST("/test-t", func(c *gin.Context) {
		// must bind。如果出现bind错误，直接返回错误给浏览器。errcode=400，content-type=’text/plain; charset=utf-8‘
		// 这类方式有点激进，不推荐使用
		c.Bind(nil)
		c.BindHeader(nil)
		c.BindJSON(nil)
		c.BindQuery(nil)
		c.BindUri(nil)
		c.BindWith(nil, nil)
		c.BindXML(nil)
		c.BindYAML(nil)
	})

	router.POST("/test", func(c *gin.Context) {
		// should bind。如果出现bind错误，需要开发者手动处理错误

		type JsonReq struct {
			Name string `form:"name" binding:"required"`
		}
		jr := JsonReq{}

		// 通用写法，可以自动依据content-type进行bind model操作
		c.ShouldBind(&jr)

		// bind header
		c.ShouldBindHeader(&jr)

		// bind json
		// content-type: application/json
		c.ShouldBindJSON(&jr)

		// bind query string
		c.ShouldBindQuery(&jr)

		// bind /v1/:name 这样的model
		c.ShouldBindUri(&jr)

		// 较少使用
		c.ShouldBindWith(nil, nil)
		c.ShouldBindBodyWith(nil, nil)
		c.ShouldBindXML(nil)
		c.ShouldBindYAML(nil)

		logrus.Infof("%+v", jr)
	})

	router.Run(":8080")
}

// static file server
func simple9() {
	router := gin.New()

	// 这个函数没啥用
	router.Static("/s", ".")

	// file system & file
	router.StaticFS("/sfs", http.Dir("."))
	router.StaticFile("/sf/jwt.go", "./jwt.go")
	router.Run(":8080")
}

// return file and bigdata
func simple10() {
	router := gin.New()

	// get file content
	router.GET("/file", func(c *gin.Context) {
		c.File("./gin.go")
	})

	// get file content and file name
	router.GET("/file1", func(c *gin.Context) {
		c.FileAttachment("./gin.go", "gin.go")
	})

	// return data
	router.GET("/file2", func(c *gin.Context) {
		f, err := os.Open("./gin.go")
		if err != nil {
			c.AbortWithStatus(400)
		}
		defer f.Close()

		buf, err := ioutil.ReadAll(f)
		if err != nil {
			c.AbortWithStatus(400)
		}

		extraHeaders := map[string]string{
			"Content-Disposition": `attachment; filename="gin.go"`,
		}

		c.DataFromReader(200, int64(len(buf)), "text/plain; charset=utf-8", bytes.NewReader(buf), extraHeaders)
	})

	router.Run(":8080")
}

// use offical http server
func simple11() {
	router := gin.New()

	s := &http.Server{
		Addr:           ":8080",
		Handler:        router,
		ReadTimeout:    10 * time.Second,
		WriteTimeout:   10 * time.Second,
		MaxHeaderBytes: 1 << 20,
	}
	s.ListenAndServe()
}

func main() {
	simple7()
}
