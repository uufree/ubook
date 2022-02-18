# C++

[TOC]

## 关键字

### const

1. const有哪些作用？

   - **修饰变量**：说明该变量不可修改。常用于定义一组不可变的类型。

   - **修饰指针**
     - `const int* p = &abc;`或者`int const* p = &abc;`：指针指向的值不能变，但是指向的位置是可以修改的
     - `int *const p = &abc;`：指针指向的位置不能变，但是指向的值是可以修改的

   - **修饰引用**：引用的值无法修改。常用于不可变的传参。

   - **修饰成员函数**
     - `const int& abc()`：该函数返回的引用值不能变
     - `int abc() const`：该函数不能修改object内的成员

2. #define和const有什么区别？

   | 宏定义 #define         | const 常量     |
   | ---------------------- | -------------- |
   | 宏定义，相当于字符替换 | 常量声明       |
   | 预处理器处理           | 编译器处理     |
   | 无类型安全检查         | 有类型安全检查 |
   | 不分配内存             | 要分配内存     |
   | 存储在代码段           | 存储在数据段   |
   | 可通过 `#undef` 取消   | 不可取消       |

### static

1. static有什么作用？
   - **修饰普通变量**：无论是局部还是全局，编译器都会将static变量放在DATA段（有初始值）或者BSS段（没有初始值）中，而非堆栈。生命周期等于进程的生命周期。并且会限制static的访问作用域，只能在当前文件访问，即使使用extern修饰都不行。
   - **修饰普通函数**：限制函数的作用域，表明函数仅在当前文件才有效，即使使用extern修饰都不行。
   - **修饰成员变量**：所有的对象共享同一个静态成员，可以直接通过`class::staticVariable`这样的方式访问
   - **修饰成员函数**：可以直接通过`class::staticFunc()`这样的方式访问。注意：静态成员函数只能操作静态成员

### extern

1. extern关键字有什么作用？

   - `extern`：用来标示变量或者函数定义在别的文件中，提示编译器（链接阶段）遇到此变量和函数时在其他模块（其他.o文件）中寻找其定义

     ```c++
     // test.cc
     int test() {
       std::cout << "num" << std::endl;
       return 0;
     }
     
     // main.cc
     extern int test();
     
     int main() {
       test();
       return 0;
     }
     
     // build
     // 整个过程无须使用.h文件进行引用
     g++ main.cc test.cc -o a -std=c++11
     ```

   - `extern "C"`：C++有编译期多态的特性，会在编译时，会将函数名和参数联合起来生成一个中间的函数名称，例如：

     ```bash
     > nm -D libPocoJSON.so
     ...
     _ZTVN4Poco7Dynamic13VarHolderImplIbEE
     ...
     
     > c++filt _ZTVN4Poco7Dynamic13VarHolderImplIbEE
     vtable for Poco::Dynamic::VarHolderImpl<bool>
     ```

     而C语言则不会，因此会造成链接时找不到对应函数的情况。这时，就需要使用`extern "C"`告诉编译器：请保持我的名称，不要给我生成用于链接的中间函数名。使用方式如下所示：

     ```c++
     #ifdef __cplusplus
     #if __cplusplus
     extern "C"{
     　#endif
     　#endif /* __cplusplus */
     　…
     　…
     　//.h文件结束的地方
     　#ifdef __cplusplus
     　#if __cplusplus
     }
     #endif
     #endif /* __cplusplus */ 
     ```

### this

1. this指针是什么？

   - this指针是一个隐含于每一个非静态成员函数中的特殊指针，指向调用该成员函数的那个对象

   - 当一个成员函数被调用时，自动向它传递一个隐含的参数，该参数是一个指向这个成员函数所在的对象的指针

     ```c++
     class Obj {
     	...
     };
     
     int main() {
     	Obj o;
     	o.print(a,b,c);	// print(Obj* const this, a,b,c)
     	...
     }
     ```

   - this指针是一个常量指针：`Obj *const this`，即this指针指向的值是不能变的

### inline

1. inline有什么作用？
   - 相当于#define，会在编译的时候自动展开。节省了运行时函数调用的开销；
   - 编译器一般不内联包含循环、递归、switch 等复杂操作的内联函数；
   - 在类声明中定义的函数，除了虚函数的其他函数都会自动隐式地当成内联函数；
2. inline有什么优缺点？
   - 优点
     - 节省运行期函数调用开销
     - 内联函数展开时，会做详细的类型检查
   - 缺点
     - 代码膨胀
     - 内联函数无法随动态库升级而升级，必须要重新编译

### volatile

告诉编译器：**不要对这个对象做任何访问优化，每次都要从内存中去读这个变量**。常用于嵌入式开发，解决内存映射硬件场景中的内存读写问题。

### sizeof()

- sizeof对数组，会返回整个数组所占的空间大小

  ```c++
  int nums[10];
  std::cout << sizeof(nums) << std::endl;		// 40
  ```

- sizeof对指针，会返回指针本身所占空间的大小

  ```c++
  int num = 10;
  int* p = &num;
  std::cout << sizeof(p) << std::endl;			// 8
  ```

### #pragma pack(m)

TODO..

### 位域

TODO..

### struct、class

1. struct和class有什么区别？
   - 内存布局：
     - 如果没有class中没有virtual函数，那struct和class的空间布局是一样的
     - 如果有virtual函数，那么class相比struct，会多出一个vptr指针，指向virtual table
   - 访问权限：
     - struct默认是public的
     - class默认是private的

### union

一种节省空间的特殊的类，一个union可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值。

### explicit

主要用于修饰构造函数和复制构造函数，可以有效防止隐式类型转换。需要注意的是，仅对单个参数的初始化类型生效：

```c++
struct A
{
	A(int) { }
	operator bool() const { return true; }
};

struct B
{
	explicit B(int) {}
	explicit operator bool() const { return true; }
};
```

### friend

友元函数和友元类能访问私有成员，破坏了类的完整性。示例如下：

```c++
// 友元函数
class Sales {
  friend int readBooks(const Sales& sales);
 public:
  Sales() = default;
  Sales(int books) : books_(books) {};
  ~Sales(){};

  int books() const {return books_;};

 private:
  int books_;
};

int readBooks(const Sales& sales) {
  return sales.books_;
}

// 友元类
class Sales {
  friend class Markets;
 public:
  Sales() = default;
  Sales(int books) : books_(books) {};
  ~Sales(){};

  int books() const {return books_;};

 private:
  int books_;
};

class Markets {
 public:
  Markets() = default;
  Markets(const Sales& sales) : sales_(sales) {}
  ~Markets(){};

  int books() {return sales_.books_;}

 private:
  Sales sales_;
};
```

### decltype

尾置返回类型，主要用于返回类型不确定的场景：

```c++
// 尾置返回允许我们在参数列表之后声明返回类型
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    // 处理序列
    return *beg;    // 返回序列中一个元素的引用
}
// 为了使用模板参数成员，必须用 typename
template <typename It>
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type
{
    // 处理序列
    return *beg;    // 返回序列中一个元素的拷贝
}
```

### &、&&



### initializer_list



## STL



## 面向对象



## 模板与泛型



## 编译链接

