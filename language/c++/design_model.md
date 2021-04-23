# 设计模式

[TOC]

## 创建型模式

关注对象创建的方式。

### 工厂模式

```c++
#include<iostream>

using namespace std;

class Product
{
public:
    virtual void show() = 0;
};

class Product_A : public Product
{
public:
    void show()
    {
        cout << "Product_A" << endl;
    }
};

class Product_B : public Product
{
public:
    void show()
    {
        cout << "Product_B" << endl;
    }
};

class Factory
{
public:
    Product* Create(int i)
    {
        switch (i)
        {
        case 1:
            return new Product_A;
            break;
        case 2:
            return new Product_B;
            break;
        default:
            break;
        }
    }
};

int main()
{
    Factory *factory = new Factory();
    factory->Create(1)->show();
    factory->Create(2)->show();
    return 0;
}
```

### 抽象工厂模式

```c++
#include <iostream>
using namespace std;

//定义抽象类
class product1
{
public:
    virtual void show() = 0;
};

//定义具体类
class product_A1 :public product1
{
public:
    void show(){ cout << "product A1" << endl; }
};

class product_B1 :public product1
{
public:
    void show(){ cout << "product B1" << endl; }
};

//定义抽象类
class product2
{
public:
    virtual void show() = 0;
};

//定义具体类
class product_A2 :public product2
{
public:
    void show(){ cout << "product A2" << endl; }
};

class product_B2 :public product2
{
public:
    void show(){ cout << "product B2" << endl; }
};


class Factory
{
public:
    virtual product1 *creat1() = 0;
    virtual product2 *creat2() = 0;
};

class FactoryA : public Factory
{
public:
    product1 *creat1(){ return new product_A1(); }
    product2 *creat2(){ return new product_A2(); }
};

class FactoryB : public Factory
{
public:
    product1 *creat1(){ return new product_B1(); }
    product2 *creat2(){ return new product_B2(); }
};

int main()
{
    Factory *factoryA = new FactoryA();
    factoryA->creat1()->show();
    factoryA->creat2()->show();

    Factory *factoryB = new FactoryB();
    factoryB->creat1()->show();
    factoryB->creat2()->show();

    return 0;
}
```

### 单例模式

- 饿汉模式

  ```c++
  // singleton.h
  class singleton{
  private:
      singleton(){}
      static singleton* p;
  
  public:
      static singleton* instance();
      int a;
  };
  
  //singleton.cpp
  singleton* singleton::p = new singleton();
  singleton* singleton::instance()
  {
      return p;
  }
  
  //main.c
  int main(void)
  {
      singleton * s = singleton::instance();
      s->a = 10;
  
      return 0;
  }
  ```

- 懒汉模式

  ```c++
  #include <mutex>
  
  // singleton.h
  class singleton{
  private:
      singleton(){}
      static singleton* p;
  	static std::mutex mutex;
  
  public:
      static singleton* instance();
      int a;
  };
  
  //singleton.cpp
  std::mutex singleton::mutex;
  singleton* singleton::p = nullptr;
  singleton* singleton::instance()
  {
  	std::lock_guard<std::mutex> guard(mutex);
      static singleton single;
  	return &single;
  }
  
  //main.c
  int main(void)
  {
  	singleton * s = singleton::instance();
      s->a = 10;
  
      return 0;
  }
  ```

### 建造者模式

使用多个简单的对象，一步一步的构造一个复杂的对象。



### 原型模式

## 结构型模式

关注类和对象的组合。

- 适配器模式
- 桥接模式
- 过滤器模式
- 组合模式
- 装饰器模式
- 外观模式
- 享元模式
- 代理模式

## 行为型模式

关注对象之间的通信。

- 责任链模式
- 命令模式
- 解释器模式
- 迭代器模式
- 中介者模式
- 备忘录模式
- 观察者模式
- 状态模式
- 空对象模式
- 策略模式
- 模板模式
- 访问者模式

## J2EE模式

关注表示层。

- MVC模式
- 业务代表模式
- 组合实体模式
- 数据访问对象模式
- 前端控制器模式
- 拦截过滤器模式
- 服务定位器模式
- 传输对象模式