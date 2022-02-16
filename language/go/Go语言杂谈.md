# Go语言杂谈

[TOC]

## Tips

1. 可以使用`_`注释掉未使用的import、未使用的变量

   ```go
   import (  
       _ "fmt"
       "log"
       "time"
   )
   
   var _ = log.Println
   ```

2. 不能使用短变量声明来设置字段值

   ```go
   func main() {  
     var data info
   
     data.result, err := work() //error
     fmt.Printf("info: %+v\n",data)
   }
   ```

3. 不能使用`nil`来初始化没有显式类型的变量。**nil标识符可以用作接口、函数、指针、映射、切片和通道的零值。**

   ```go
   func main() {  
       var x = nil //error
   
       _ = x
   }
   ```

4. 不能使用`cap()`来计算map的容量

5. 字符串不能为nil

6. 数组传参时，会将原始数组的副本传递给函数。如果想要在函数中修改数据，请使用指针传递

   > Go仅支持值传递

   ```go
   func main() {  
       x := [3]int{1,2,3}
   
       func(arr [3]int) {
           arr[0] = 7
           fmt.Println(arr) //prints [7 2 3]
       }(x)
   
       fmt.Println(x) //prints [1 2 3] (not ok if you need [7 2 3])
   }
   ```

7. 在map中访问不存在的key的正确方式

   ```go
   func main() {  
       x := map[string]string{"one":"a","two":"","three":"c"}
   
       if _,ok := x["two"]; !ok {
           fmt.Println("no entry")
       }
   }
   ```

8. 计算字符串长度时，请使用以下方式：

   ```go
   package main
   
   import (  
       "fmt"
       "unicode/utf8"
   )
   
   func main() {  
       data := "♥"
       fmt.Println(utf8.RuneCountInString(data)) //prints: 1
   }
   ```

9. 可以使用`fallthrough`替换`switch .. case ..`语句中缺省的`break`语义

10. 从已关闭的Channel中读数据，会读取到数据的零值；往已关闭的Channel中写数据，会产生panic

11. 从未初始化的Channel中读写数据，都会发生panic

12. `select .. case ..`只能能处理Channel类型的读写事件

13. `select .. case ..`中可能发生的事件：

    ```go
    // 如果ch1和ch2都不可读的话，会发生阻塞
    select {
      case <-ch1:
      ..
      case <-ch2:
      ...
    }
    
    // 如果ch1和ch2都不可读，控制流会到default，然后退出循环
    select {
      case <-ch1:
      .. 
      case <-ch2:
      ..
    	default:
      ..
    }
    ```

14. `defer`仅会在函数退出时执行，与代码块无关

15. **结构体比较**：如果结构体中的每个字段都具有可比性，那么就可以用等号运算符`==`比较结构体；如果结构体中有任一字段不具有可比性，那么在编译期间就会报错

16. **数组、切片、map比较**：可以使用`reflect.DeepEqual(v1, v2)`

    ```go
    package main
    
    import (  
        "fmt"
        "reflect"
    )
    
    type data struct {  
        num int                //ok
        checks [10]func() bool //无法比较
        doit func() bool       //无法比较
        m map[string] string   //无法比较
        bytes []byte           //无法比较
    }
    
    func main() {  
        v1 := data{}
        v2 := data{}
        fmt.Println("v1 == v2:",reflect.DeepEqual(v1,v2)) //prints: v1 == v2: true
    
        m1 := map[string]string{"one": "a","two": "b"}
        m2 := map[string]string{"two": "b", "one": "a"}
        fmt.Println("m1 == m2:",reflect.DeepEqual(m1, m2)) //prints: m1 == m2: true
    
        s1 := []int{1, 2, 3}
        s2 := []int{1, 2, 3}
        fmt.Println("s1 == s2:",reflect.DeepEqual(s1, s2)) //prints: s1 == s2: true
    }
    ```

17. 仅有在defer中执行`recover()`才能达到从panic中恢复的目的

    ```go
    func main() {  
        defer func() {
            fmt.Println("recovered:",recover())
        }()
    
        panic("not good")
    }
    ```

18. **使用range遍历数组、切片、map时，遍历的是原始数据的引用，而非原始数据。因此，在遍历过程中做的修改也是无效的**。如果想要修改原始数据，请使用index

    ```go
    func main() {  
        data := []int{1,2,3}
        for _,v := range data {
            v *= 10 //原始项目不变
        }
    
        fmt.Println("data:",data) //prints data: [1 2 3]
    }
    ```

19. 可使用匿名字段拓展功能

    ```go
    type myLocker struct {  
        sync.Mutex
    }
    
    func main() {  
        var lock myLocker
        lock.Lock() //ok
        lock.Unlock() //ok
    }
    ```

20. **在`for .. select .. break ..`语句中，break仅能脱离select，无法脱离for。需要使用`break tag`或者`return`才能脱离for**

    ```go
    func main() {  
        loop:
            for {
                switch {
                case true:
                    fmt.Println("breaking out...")
                    break loop
                }
            }
    
        fmt.Println("out!")
    }
    ```

21. **`for ..`与闭包同时存在时，务必需要copy value。因为range中index、value在整个循环过程中，使用的同一个变量。**

    ```go
    // 方法1:
    func main() {  
        data := []string{"one","two","three"}
    
        for _,v := range data {
            vcopy := v //
            go func() {
                fmt.Println(vcopy)
            }()
        }
    
        time.Sleep(3 * time.Second)
        //goroutines print: one, two, three
    }
    
    // 方法2:
    func main() {  
        data := []string{"one","two","three"}
    
        for _,v := range data {
            go func(in string) {
                fmt.Println(in)
            }(v)
        }
    
        time.Sleep(3 * time.Second)
        //goroutines print: one, two, three
    }
    ```

22. 失败的类型断言的处理方式：

    ```go
    func main() {  
        var data interface{} = "great"
    
        if res, ok := data.(int); ok {
            fmt.Println("[is an int] value =>",res)
        } else {
            fmt.Println("[not an int] value =>",data) 
            //prints: [not an int] value => great (as expected)
        }
    }
    ```

23. 防止Worker Goroutine因此阻塞导致Goroutine泄露

    ```go
    func First(query string, replicas ...Search) Result {  
        c := make(chan Result,1)
        searchReplica := func(i int) { 
          	// 要么写入成功，要么直接返回
          	// 不过对于这种场景，还是使用ErrGroup或者Wg更好一些
            select {
            case c <- replicas[i](query):
            default:
            }
        }
        for i := range replicas {
            go searchReplica(i)
        }
        return <-c
    }
    ```

24. 使用gdb调试golang时，需要加入以下参数：

    ```go
    go build -gcflags "-N -l" main.go
    ```

## 反射

TODO..

## 底层编程

TODO..

## 工具链

### build

### clean

### doc

### env

### fix

### fmt

### generate

### get

### install

### list

### mod

- `go mod download [-x] [-json] [modules]`：较少使用
- `go mod edit [editing flags] [-fmt|-print|-json] [go.mod]`：较少使用
- `go mod graph`：打印模块间依赖，较少使用
- `go mod init [module-path]`：初始化module
- `go mod tidy`：更新并下载最新的模块，并删除无用的模块
- `go mod vendor`：将所有依赖放在当前vendor文件下
- `go mod verify`：检查依赖是否正常
- `go mod why`：很少使用

### run

### test

### tool

### vet

## 包管理

TODO...