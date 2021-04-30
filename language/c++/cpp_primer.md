

# C++ Primer

[TOC]

## 第7章 类

- 定义在类内部的函数是**隐式的inline函数**

- 常量成员函数：`std::string isbn() const`，此函数无法修改成员变量。除非，成员变量被**mutable**修饰

- **this**指针

  - `std::string isbn() {...}` >> `std::string isbn(Sales* const this) {...}`

    `const`修饰指针，意味着指针指向的位置不能改变。

  - `std::string isbn() const {...}` >> `std::string isbn(const Sales* const this) {...}`

    `const`修饰指针和指针指向的数据，意味着指针指向的位置和对应位置的值不能发生改变

- **当类没有声明任何构造函数时，编译器才会生成默认的构造函数**。反之，当声明了构造函数，便不能依赖编译器生成默认构造函数。除非使用：`Sales() = default`这样的语法，强制编译器生成默认构造函数。

- **重载**成员函数：

  ```c++
  class Sales {
  public:
  	void get();
      void get(int x, int y);
  }
  ```

- **重载常量**成员函数：

  ```c++
  class Screen {
  public:
  	Screen& display(ostream& os) {do_display(os); return *this;}
  	const Screen& display(ostream& os) const {do_display(os); return *this;}
  private:
  	void do_display(ostream& os) const {os << content;}
  }
  
  Screen myScreen;
  const Screen myBlock;
  myScreen.set("#").display(cout);	// 非常量版本
  myBlock.display(cout);				// 常量版本
  
  // 更深层的解释：
  // 上述两个display函数会被编译器解释为：
  // 因此，类形成const重载。其次，根据调用对象的const特性，会选用不同类型的函数
  Screen& display(Screen *const this, ostream& os) {do_display(os); return *this;}
  const Screen& display(const Screen *const this, ostream& os) {do_display(os); return *this;}
  ```

- **前向声明**：前向声明的类是一个不完全类型，仅可以**指针或者引用**的方式使用。

- 友元：

  友元函数。类中的声明=**访问权限**；在类外声明=**函数声明**

  - 友元函数：

    ```c++
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
    ```

  - 友元类

    ```c++
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

- **explicit**可以抑制构造函数中的隐式类型转换。注：

  - 仅适用与单参数的构造函数
  - 仅需要在类中进行声明，类外无需声明

- **静态成员** & **静态函数**

  ```c++
  class Sales {
   public:
    static int reader;
   public:
    Sales() = default;
    Sales(int books) : books_(books) {};
    ~Sales(){};
  
    int books() {return books_;};
    static int getBooks() {return reader;}
  
   private:
    int books_;
  };
  
  int Sales::reader = 10;
  ```

## 第12章 动态内存

## 第13章 拷贝控制

- 如果一个类需要定义析构函数，可以肯定，它也需要定义拷贝构造函数和拷贝构造运算符

- 使用**=default**显示要求编译器合成默认的构造、拷贝、析构函数；使用**=delete**显式拒绝编译器合成构造、拷贝函数；使用**noexcept**修饰的成员函数不会抛出异常

- 开发拷贝函数的时候，需要注意：

  - 如果将一个对象赋予它自身，赋值运算符必须能正常工作
  - 大多数赋值运算符组合了析构函数和构造函数的工作

- **右值引用**：

  ```c++
  int i = 42;						
  int &r = i;						// 正确，左值引用
  int &&rr = i;					// 错误，不能将一个左值绑定到右值rr上
  int & r2 = i * 2;				// 错误，不能将一个右值绑定到左值r2上
  const int &r3 = i * 2;			// 正确，可以将右值绑定到const左值r3上
  int &&rr2 = i * 2;				// 正确，可以将右值绑定到右值rr2上
  ```

- **左值持久，右值短暂**

  - 左值具有持久的状态

  - 右值要么是字面常量，要么是表达式求值过程中创建的临时对象。由于右值引用只能绑定到临时对象，可以得知：

    - 右值所引用的对象即将被销毁
    - 该对象没有其他用户

    即，使用右值引用的代码可以自由的接管所引用的对象的资源。

- `std::move`告诉编译器：我们有一个左值，但是我们希望向处理右值一样处理它。即承诺：在调用`std::move`之后，我们将不再使用它。

- **移动构造函数**：不抛出异常的移动构造、赋值函数都必须标记为**noexcept**。

  ```c++
  StrVec::StrVec(StrVec&& lhs) noexcept :
  	elements(lhs.elements), first_free(lhs.first_free), cap(lhs.cap) {
  	lhs.elements = lhs.first_free = lhs.cap = nullptr;	
  }
  ```

  从一个对象移动数据并不会销毁此对象，但有时在移动操作完成后，源对象会被销毁。因此，开发移动操作时，必须要确保移动后的源对象进入一个有效的、可以析构的状态，但是用户不能对这个源对象保有的值进行任何假设。

- 如果一个类没有定义移动构造函数，即使使用了`std::move`，这个类的对象会被拷贝。

- `make_move_iterator(vec.begin)`将普通的迭代器转换为**移动迭代器**

## 第14章 重载运算与类型转换

- 重载运算符分类

  - **输入>>**、**输出<<**运算符

  - 算术运算符

  - 关系运算符

  - 赋值运算符：**=**

  - 下标运算符：**[]**

  - 递增和递减运算符：

    - 前置：先递增，后返回

      ```c++
      Blob& operator++();
      Blob& operator--();
      ```

    - 后置：先返回，后递增

      ```c++
      Blob& operator++(int);
      Blob& operator--(int);
      ```

  - 成员访问运算符：*****和**->**
  - 函数调用运算符：`Blob& operator()(int val);`

## 第15章 面向对象程序设计

- 面向对象的核心思想：**数据抽象、继承、动态绑定**

- **override、final**关键字

- 强制执行指定的虚函数版本：`bulk->Base::insert(10);`

- 每个基类都要定义**虚析构函数**，否则派生类无法使用正确的析构函数

- **protected**和友元：友元关系无法继承，每个负责控制各自成员的访问权限

  ```c++
  class Base {
  protected:
      int number;
  };
  
  class Sneaky : public Base {
    friend void Dot(Base& b);			// 无法访问Base中的protected成员
    friend void Dot(Sneaky& s);		// 可以访问Sneaky中的protected成员
  };
  ```

- 派生类的复制构造函数、复制运算符

  ```c++
  class Base {...};
  class D : public Base {
    D(const D& d) : Base(d) {...};
    D(D&& d) : Base(std::move(d)) {...};  
    D& operator=(const D& d) {
        Base::operator=(d);
        ...
    };
    D& operator=(D&& d) {
        Base::operator=(std::move(d));
        ...
    }  
  };
  
  
  ```

  