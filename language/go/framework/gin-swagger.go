package main

import (
	"github.com/gin-gonic/gin"
)

// 参考文档如下：
// https://swaggo.github.io/swaggo.io/
// https://wdpm.gitbook.io/swagger-documentation/swagger-specification/describing-responses
// https://editor.swagger.io/
// https://github.com/eddycjy/go-gin-example
// https://github.com/swaggo/gin-swagger


// BasePath：不建议使用。用于描述统一路径前缀，但是渲染的不明显，建议直接将path写在router中。 在外部通过v1, v2这样的文件夹对接口做区分。注：如果要使用的话，一定要有空行。
// Tags: 用于给API进行逻辑分组，例如instance, run, pipeline, dataset...这样子
// Summary: 简短解释API功能
// Description: 详细解释API功能
// Accept: 这个参数一般不怎么用。HTTP Request MIME Type, such as: application/json, multipart/form-data..
// Param: [字段名称] [参数格式] [参数类型] [是否必须] [详细描述]
//		[字段名称]: 字段名字
//		[参数格式]: 可选：path, query, header, formData, body
//		[参数类型]: 可选： int, string, bool, file
//		[是否必须]: 可选：true, false
//		[详细描述]: 字段的详细描述。注：最好加上默认值
// 需要注意的是：formData用于描述application/x-www-form-urlencoded, multipart/form-data; body用于描述其他类型
// Produce: 描述返回类型。一般直接选json。更详细的需求见MIME
// Success: [状态码] [类型] [实体]
// Failure: [状态码] [类型] [实体]
//		[状态码]: 类似200,400,404,500这样子
//		[类型]: 可选string, object
//		[实体]: 类型对应的实体字符串或者Response结构。实体可自动引用包路径，例如：app.Response; 实体可做二次填充
// Router: [url] [method]

type GetResponse struct {
	Name string `json:"name"`
	Age  int    `json:"age"`
}

type Response struct {
	Code int         `json:"code"`
	Msg  string      `json:"msg"`
	Data interface{} `json:"data"`
}

// CASE 1: get json
// @Tags GET
// @Summary 获取json类型的结果
// @Description 获取json类型的结果
// @Param auth header string true "鉴权"
// @Param id path string true "查询ID"
// @Param name query string true "名字"
// @Param age query int true "年龄"
// @Param find query bool false "查询条件"
// @Produce json
// @Success 200 {object} Response{data=GetResponse}
// @Failure 400 {object} Response
// @Router /get/json [get]
func GetJson(c *gin.Context) {
	c.JSON(200, &Response{})
}

// CASE 2: get file
// @Tags GET
// @Summary 获取文件
// @Description 获取文件
// @Param id path string true "查询ID"
// @Produce application/octet-stream
// @Success 200 {file} file-content
// @Router /get/file [get]
func GetFile(c *gin.Context) {
	c.FileAttachment("test.json", "test.json")
}

// CASE 3: post json
// @Tags POST
// @Summary 发送JSON类型的POST请求
// @Description 发送JSON类型的POST请求
// @Param name body string true "名字"
// @Param age body int true "年龄"
// @Param look body bool true "查找"
// @Produce json
// @Success 200 {object} Response
// @Failure 400 {object} Response
// @Router /post/json [post]
func PostJson(c *gin.Context) {
	c.JSON(200, &Response{})
}

// CASE 4: post file
// @Tags POST
// @Summary 发送文件的POST请求
// @Description 发送文件的POST请求
// @Param file1 formData file true "文件1"
// @Param file2 formData file true "文件2"
// @Produce json
// @Success 200 {object} Response
// @Failure 400 {object} Response
// @Router /post/file [post]
func PostFile(c *gin.Context) {
	c.JSON(200, &Response{})
}

// CASE 5: post form-data
// @Tags POST
// @Summary 发送form-data的POST请求
// @Description 发送form-data的POST请求
// @Param form1 formData string true "表单参数1"
// @Param form2 formData int true "post数字"
// @Param form3 formData bool true "post布尔类型"
// @Produce json
// @Success 200 {object} Response
// @Failure 400 {object} Response
// @Router /post/formdata [post]
func PostFormData(c *gin.Context) {
	c.JSON(200, &Response{})
}