# 设计模式

[TOC]

## 创建型模式

关注对象创建的方式。

### ××× 工厂模式

- 意图：隐藏对象的创建过程
- 主要解决：业务层的接口选择问题，隐藏对象的创建过程
- 何时使用：明确的在不同条件下创建不同的实例
- 应用实例：
  - 需要1辆汽车，可以直接从工厂中提货，而不用在意这个汽车是如何制造出来的

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

### ××× 抽象工厂模式

- 意图：提供一个创建一组相互依赖的对象的接口，而无需指定他们具体的类
- 主要解决：接口的选择问题
- 何时使用：产品有多个产品族，而业务层只使用某个产品族中的某一个产品
- 应用实例：
  - 家用服装，有：商务系列（3种）、时尚系列（2种）、休闲系列（5种），拿出服装时，可以使用抽象工厂模式先生成套装系列，再产出具体的服装

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

### ××× 单例模式

- 意图：一个类仅有一个实例，并提供一个访问它的全局访问点
- 主要解决：全局对象的频繁创建和频繁销毁
- 应用实例：
  - 一个班级只有一个班主任

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

- 意图：将一个复杂的构建过程与其表示相分离，使用同样的构建过程可以创建不同的表示
- 主要解决：一个复杂对象的创建过程。这个复杂对象由多个子对象构成，由于需求的剧烈变换，这个复杂对象的各部分经常面临着剧烈的变换，但是将这些子对象组装在一起的算法相对稳定
- 何时使用：基本组件基本没有变换，组合经常变换
- 应用实例：
  - 肯德基中的汉堡、可乐、鸡翅、薯条是不变的，其组合经常变换，生出所谓的套餐

```c++
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

class Food {
 public:
  Food() {};

  virtual const std::string &FoodName() const = 0;
  virtual float FoodPrice() const = 0;

 protected:
  std::string name_;
  float price_;
};

template<typename T>
class FoodImpl : public Food {
 public:
  FoodImpl(const std::string &food_name, float food_price) {
    name_ = food_name;
    price_ = food_price;
  }

  const std::string &FoodName() const override {
    return name_;
  }

  float FoodPrice() const override {
    return price_;
  }
};

struct FoodBase {};
struct Hamburger : public FoodBase {};
struct FrenchFries : public FoodBase {};
struct Coke : public FoodBase {};
struct Chicken : public FoodBase {};

// 套餐A：汉堡 + 鸡翅 + 可乐
// 套餐B：汉堡 + 可乐 + 薯条
// 套餐C：可乐 + 鸡翅 + 薯条

class Builder {
 public:
  Builder() {};
  virtual void PrintFoodList() const = 0;
  virtual float CalculateFoodPrice() const = 0;

 protected:
  std::vector<Food *> foods_;
};

enum PackageType {
  PACKAGE_A,
  PACKAGE_B,
  PACKAGE_C
};

class APackageBuilder : public Builder {
 public:
  APackageBuilder() {
    foods_.push_back(new FoodImpl<Hamburger>("Hamburger", 10));
    foods_.push_back(new FoodImpl<Chicken>("Chicken", 5));
    foods_.push_back(new FoodImpl<Coke>("Coke", 2));
  }

  void PrintFoodList() const override {
    for (const Food* food : foods_) {
      std::cout << food->FoodName() << "    ";
    }
    std::cout << std::endl;
  }

  float CalculateFoodPrice() const override {
    float price;
    std::for_each(foods_.begin(), foods_.end(), [&](const Food *food) { price += food->FoodPrice(); });
    return price;
  }
};

class BPackageBuilder : public Builder {
 public:
  BPackageBuilder() {
    foods_.push_back(new FoodImpl<Hamburger>("Hamburger", 10));
    foods_.push_back(new FoodImpl<Coke>("Coke", 2));
    foods_.push_back(new FoodImpl<FrenchFries>("FrenchFries", 3));
  }

  void PrintFoodList() const override {
    for (const Food* food : foods_) {
      std::cout << food->FoodName() << "    ";
    }
    std::cout << std::endl;
  }

  float CalculateFoodPrice() const override {
    float price;
    std::for_each(foods_.begin(), foods_.end(), [&](const Food *food) { price += food->FoodPrice(); });
    return price;
  }
};

class CPackageBuilder : public Builder {
 public:
  CPackageBuilder() {
    foods_.push_back(new FoodImpl<Chicken>("Chicken", 5));
    foods_.push_back(new FoodImpl<Coke>("Coke", 2));
    foods_.push_back(new FoodImpl<FrenchFries>("FrenchFries", 3));
  }

  void PrintFoodList() const override {
    for (const Food* food : foods_) {
      std::cout << food->FoodName() << "    ";
    }
    std::cout << std::endl;
  }

  float CalculateFoodPrice() const override {
    float price;
    std::for_each(foods_.begin(), foods_.end(), [&](const Food *food) { price += food->FoodPrice(); });
    return price;
  }
};

class BuilderFactory {
 public:
  Builder *CreateBuilder(PackageType type) {
    switch (type) {
      case PACKAGE_A:return new APackageBuilder();
      case PACKAGE_B:return new BPackageBuilder();
      case PACKAGE_C:return new CPackageBuilder();
      default:return nullptr;
    }
  }
};
```

### 原型模式

- 意图：实现了一个原型接口，该接口用于创建当前对象的克隆

```c++
#include <iostream>
using namespace std;
 
class Prototype
{
public:
	 Prototype(){}
	 ~Prototype(){}
 
	 virtual Prototype *clone() = 0;
};
 
class ConcretePrototypeA :public Prototype
{
public:
	ConcretePrototypeA() :member(0){}
	~ConcretePrototypeA(){}
 
	ConcretePrototypeA(const ConcretePrototypeA &rhs)
	{
		member = rhs.member;
	}
 
	virtual ConcretePrototypeA* clone()
	{
		cout << "copy of self" << endl;
		return new ConcretePrototypeA(*this);
	}
 
private:
	int member;
};
 
int main(int argc, char **argv)
{
	//生成对像
	ConcretePrototypeA *conPro = new ConcretePrototypeA();
 
	//复制自身
	ConcretePrototypeA * conPro1 = conPro->clone();
 
	//复制自身
	ConcretePrototypeA * conPro2 = conPro->clone();
 
	delete conPro;
	conPro = NULL;
 
	delete conPro1;
	conPro1 = NULL;
 
	delete conPro2;
	conPro2 = NULL;
 
	return 0;
}
```

## 结构型模式

关注类和对象的组合。

### ××× 适配器模式

- 意图：将一个类的接口转换成客户希望的另外一个接口，使原本由于接口不兼容而不能一起工作的那些类可以一起工作
- 主要解决：将一些现存的对象加入到新环境中，而新环境要求的接口是现对象不能满足的

```c++
#include <iostream>
using namespace std;

class Target {  // Target，客户期望的接口，可以使具体或抽象的类，也可以是接口
public:
    virtual void Request() = 0;
    virtual ~Target(){};
};

class Adaptee { // 需适配的类
public:
    void SpecificRequest() { cout << "Adaptee" << endl; }
};

class Adapter : public Target { // 通过内部包装一个Adaptee对象，把源接口转换为目标接口：
private:
    Adaptee* adaptee;
public:
    Adapter() { adaptee = new Adaptee(); }
    void Request() { adaptee->SpecificRequest(); }  // 调用Request()方法会转换成调用adaptee.SpecificRequest()
    ~Adapter() { delete adaptee; }
};

int main() {
    Target* target = new Adapter();
    target->Request();

    delete target;
    return 0;
}
```

### ××× 桥接模式

- 意图：将抽象部分与实现部分分离，使他们可以独立的发生变化
- 主要解决：在多种变化的情况下，使用继承会造成类数量爆炸的问题，扩展起来不灵活
- 何时使用：系统有多个角度分类，每个角度都可能发生变换
- 应用实例：
  - 有3支不同笔尖的画笔、12种颜料，那么可以画3 × 12种线条；而蜡笔的话，需要使用36只蜡笔。其中，画笔就是桥接模式（笔尖是一个维度，颜色是另外一个维度）

```c++
class Color {
 public:
  Color() {};
  virtual ~Color() {};

  virtual const std::string Use() const = 0;
};

class Blue : public Color {
 public:
  Blue() {};
  ~Blue() override {};

  const std::string Use() const {
    return "blue";
  }
};

class Red : public Color {
 public:
  Red() {};
  ~Red() override {};

  const std::string Use() const {
    return "red";
  }
};

class Pen {
 public:
  Pen() {};
  virtual ~Pen() {};

  virtual void Draw(const Color* color) = 0;
};

class PenA : public Pen {
 public:
  PenA() {};
  ~PenA() override {};

  void Draw(const Color* color) override  {
    std::cout << "pen A use " << color->Use() << std::endl;
  }
};

class PenB : public Pen {
 public:
  PenB() {};
  ~PenB() override {};

  void Draw(const Color* color) override  {
    std::cout << "pen B use " << color->Use() << std::endl;
  }
};
```

### 过滤器模式

```c++
class Person {
 public:
  Person(const std::string &name, int age) : name_(name), age_(age) {};
  ~Person() {};

  const std::string &Name() const {
    return name_;
  }

  const int Age() const {
    return age_;
  }

 private:
  std::string name_;
  int age_;
};

class Filter {
 public:
  Filter() {};
  virtual ~Filter() {};

  virtual int MeetFilter(const std::vector<Person> &persons, std::vector<Person> &out_persons) = 0;
};

class NameFilter : public Filter {
 public:
  NameFilter(const std::string &name) : name_(name) {};
  virtual ~NameFilter() {};

  int MeetFilter(const std::vector<Person> &persons, std::vector<Person> &out_persons) override {
    out_persons.clear();
    std::for_each(persons.begin(),
                  persons.end(),
                  [&](const Person &p) { if (p.Name() != name_) out_persons.push_back(p); });
    return 0;
  }
 private:
  std::string name_;
};

class AgeFilter : public Filter {
 public:
  AgeFilter(const int &age) : age_(age) {};
  virtual ~AgeFilter() {};

  int MeetFilter(const std::vector<Person> &persons, std::vector<Person> &out_persons) override {
    out_persons.clear();
    std::for_each(persons.begin(),
                  persons.end(),
                  [&](const Person &p) { if (p.Age() != age_) out_persons.push_back(p); });
    return 0;
  }
 private:
  int age_;
};
```

### 组合模式

- 意图：将一组相似的对象当做单一的对象，使用户对单个对象和组合对象的使用具有一致性
- 主要解决：模糊了相似对象间的区别（树中枝节点和叶节点），业务层可以使用相同的代码处理相似的对象
- 何时使用：希望在业务层忽略相似的相似对象的区别

```c++
class Component {
 public:
  Component(const std::string &name) : name_(name) {};
  virtual ~Component() {};

  virtual void Add(Component *c) = 0;
  virtual void Remove(Component *c) = 0;
  virtual const std::string Display() const = 0;

 protected:
  std::string name_;
};

class Leaf : public Component {
 public:
  Leaf(const std::string &name) : Component(name) {};
  ~Leaf() override {};

  void Add(Component *c) override {};
  void Remove(Component *c) override {};
  const std::string Display() const override { return name_; }
};

class Branch : public Component {
 public:
  Branch(const std::string &name) : Component(name) {};
  ~Branch() {};

  void Add(Component *c) override { leafs_.push_back(c); }
  void Remove(Component *c) override {
    leafs_.erase(std::remove_if(leafs_.begin(),
                                leafs_.end(),
                                [&](const Component *cp) { return cp->Display() == c->Display(); }), leafs_.end());
  };
  const std::string Display() const override {
    return name_;
  }

 private:
  std::vector<Component *> leafs_;
};
```

### ××× 装饰器模式

- 意图：动态的给对象添加一些额外的职责
- 主要解决：防止继承链爆炸。将具体的功能按照职责划分，每个功能继承装饰器模式
- 桥接模式与装饰器模式
  - 桥接模式说的分离，是将结构与实现分离；而装饰器模式是将基于属性的行为封装成独立的类。比如：**异常类和异常处理类之间就可以使用桥接模式来完成，而不能使用装饰器模式；对于异常的处理需要拓展时，就可以使用装饰器模式**
  - 桥接模式中的行为是彼此分离的，没有关联；而装饰器中的行为具有可叠加性，其表现出来的结果是一个整体，是各个行为组合之后的结果。

```c++
class Phone {
 public:
  Phone() {};
  virtual ~Phone() {};
  virtual void ShowDecorate() {};
};

// 手机类型
class IPhone : public Phone {
 public:
  IPhone() {};
  ~IPhone() override {};
  void ShowDecorate() {
    std::cout << "IPhone Decorate" << std::endl;
  }
};

class NokiaPhone : public Phone {
 public:
  NokiaPhone() {};
  ~NokiaPhone() override {};
  void ShowDecorate() override {
    std::cout << "Nokia Phone Decorate" << std::endl;
  }
};

// 收集装饰器类型
class DecoratePhone : public Phone {
 public:
  DecoratePhone(Phone* phone) : phone_(phone) {};
  virtual ~DecoratePhone() override {};
  virtual void ShowDecorate() override {
    phone_->ShowDecorate();
  }
 private:
  Phone* phone_;
};

class DecoratePhoneA : public DecoratePhone {
 public:
  DecoratePhoneA(Phone* phone) : DecoratePhone(phone) {};
  ~DecoratePhoneA() override {};

  void ShowDecorate() override {
    DecoratePhone::ShowDecorate();
    std::cout << "Add Decorate A" << std::endl;
  }
};

class DecoratePhoneB : public DecoratePhone {
 public:
  DecoratePhoneB(Phone* phone) : DecoratePhone(phone) {};
  ~DecoratePhoneB() override {};

  void ShowDecorate() override {
    DecoratePhone::ShowDecorate();
    std::cout << "Add Decorate A" << std::endl;
  }
};

int main() {
  Phone *iphone = new IPhone();
  Phone *iphoneA = new DecoratePhoneA(iphone);
  Phone *iphoneB = new DecoratePhoneB(iphone);

  iphone->ShowDecorate();
  iphoneA->ShowDecorate();
  iphoneB->ShowDecorate();
  return 0;
}
```

### 外观模式

- 意图：为子系统中的一组接口提供一个一致的界面，外观模式定义了一个高层接口，这个接口使子系统更容易使用

```c++
class SubSystemOne {
public:
    void MethodOne() { cout << "SubSystemOne" << endl; }
};

class SubSystemTwo {
public:
    void MethodTwo() { cout << "SubSystemTwo" << endl; }
};

class SubSystemThree {
public:
    void MethodThree() { cout << "SubSystemThree" << endl; }
};

class SubSystemFour {
public:
    void MethodFour() { cout << "SubSystemFour" << endl; }
};

class Facade {
private:
    SubSystemOne s1;
    SubSystemTwo s2;
    SubSystemThree s3;
    SubSystemFour s4;
public:
    void MethodA() { // 方法组合
        s1.MethodOne();
        s3.MethodThree();
    }
    void MethodB() { // 方法组合
        s2.MethodTwo();
        s3.MethodThree();
        s4.MethodFour();
    }
};

int main() {
    Facade f;
    f.MethodA(); // SubSystemOne
                 // SubSystemThree
    cout << endl;
    f.MethodB(); // SubSystemTwo
                 // SubSystemThree
                 // SubSystemFour
    return 0;
}
```

### 享元模式

- 意图：使用共享技术有效的支持大量细粒度对象

```c++
class Flyweight {
public:
    virtual void Operation(int extrinsicstate) = 0;
    virtual ~Flyweight() {}
};

class ConcreteFlyweight : public Flyweight {
public:
    void Operation(int extrinsicstate) {
        cout << "ConcreteFlyweight: " << extrinsicstate << endl;
    }
};

class UnsharedConcreteFlyweight : public Flyweight { // 不强制共享对象，但也可以共享
public:
    void Operation(int extrinsicstate) {
        cout << "UnsharedConcreteFlyweight: " << extrinsicstate << endl;
    }
};

class FlyweightFactory {
private:
    map<string, Flyweight*> flyweights;
public:
    Flyweight* GetFlyweight(string key) {
        if (flyweights[key] == NULL) flyweights[key] = new ConcreteFlyweight();
        return (Flyweight*)flyweights[key];
    }
};

int main() {
    int extrinsicstate = 22;   // 外部状态

    FlyweightFactory* f = new FlyweightFactory();
    Flyweight* fx = f->GetFlyweight("X");   　// X、Y、Z为内部状态
    fx->Operation(--extrinsicstate);    // ConcreteFlyweight: 21

    Flyweight* fy = f->GetFlyweight("Y");
    fy->Operation(--extrinsicstate);    // ConcreteFlyweight: 20

    Flyweight* fz = f->GetFlyweight("Z"); // ConcreteFlyweight: 19
    fz->Operation(--extrinsicstate);    

    UnsharedConcreteFlyweight* uf = new UnsharedConcreteFlyweight();
    uf->Operation(--extrinsicstate);    // UnsharedConcreteFlyweight: 18

    delete fx;
    delete fy;
    delete fz;
    delete f;
    delete uf;

    return 0;
}
```

### ××× 代理模式

- 意图：为其他对象提供一种代理以控制对这个对象的访问

```c++
class Subject {
 public:
  virtual void Request() const = 0;
};

class RealSubject : public Subject {
 public:
  void Request() const override {
    std::cout << "RealSubject: Handling request.\n";
  }
};

class Proxy : public Subject {
 private:
  RealSubject *real_subject_;

  bool CheckAccess() const {
    std::cout << "Proxy: Checking access prior to firing a real request.\n";
    return true;
  }
  void LogAccess() const {
    std::cout << "Proxy: Logging the time of request.\n";
  }

 public:
  Proxy(RealSubject *real_subject) : real_subject_(new RealSubject(*real_subject)) {
  }

  ~Proxy() {
    delete real_subject_;
  }

  void Request() const override {
    if (this->CheckAccess()) {
      this->real_subject_->Request();
      this->LogAccess();
    }
  }
};

void ClientCode(const Subject &subject) {
  // ...
  subject.Request();
  // ...
}

int main() {
  std::cout << "Client: Executing the client code with a real subject:\n";
  RealSubject *real_subject = new RealSubject;
  ClientCode(*real_subject);
  std::cout << "\n";
  std::cout << "Client: Executing the same client code with a proxy:\n";
  Proxy *proxy = new Proxy(real_subject);
  ClientCode(*proxy);

  delete real_subject;
  delete proxy;
  return 0;
}
```



## 行为型模式

关注对象之间的通信。

### 责任链模式

- 意图：避免请求发送者和接受者耦合在一次，让多个对象都有可能接受并处理请求，将这些对象链接成一条链，直到有对象处理它为止
- 主要解决：发送者只需要将请求发送到链路上，无需关心请求的处理细节
- 何时使用：处理消息的时候可以过滤很多道

```c++
class Handler {
 public:
  Handler() {};
  virtual ~Handler() {};

  void SetHandler(Handler *handler) {
    handler_ = handler;
  }

  virtual void Handle(int request) {
    handler_->Handle(request);
  }

 protected:
  Handler *handler_;
};

class HandlerA : public Handler {
 public:
  HandlerA() {};
  ~HandlerA() override {};

  void Handle(int request) {
    if (request == 1) {
      std::cout << "Handle A >> 1" << std::endl;
      return;
    }

    if (handler_ != nullptr) {
      handler_->Handle(request);
    }
  }
};

class HandlerB : public Handler {
 public:
  HandlerB() {};
  ~HandlerB() override {};

  void Handle(int request) {
    if (request == 2) {
      std::cout << "Handle B >> 2" << std::endl;
      return;
    }

    if (handler_ != nullptr) {
      handler_->Handle(request);
    }
  }
};

int main() {
  Handler* h = new Handler;
  Handler* h1 = new HandlerA;
  Handler* h2 = new HandlerB;

  h->SetHandler(h1);
  h1->SetHandler(h2);

  h->Handle(1);
  return 0;
}
```

### 命令模式

- 意图：将请求封装成对象，从而可以用不同的请求对客户进行参数化
- 主要解决：在软件系统中，行为的请求者和行为的实现者通常是一种紧耦合关系，但在某些场合（行为记录、行为撤销），这种紧耦合就显得不太合适。

```c++
class Stock {
 public:
  void Buy() {
    std::cout << "buy stock" << std::endl;
  }

  void Sell() {
    std::cout << "sell stock" << std::endl;
  }
};

class Command {
 public:
  Command() {};
  virtual ~Command() {};

  virtual void Execuate() = 0;
};

class BuyCommand : public Command {
 public:
  BuyCommand(Stock *stock) : stock_(stock) {};
  ~BuyCommand() override {};

  void Execuate() {
    stock_->Buy();
  }

 private:
  Stock *stock_;
};

class SellCommand : public Command {
 public:
  SellCommand(Stock *stock) : stock_(stock) {};
  ~SellCommand() override {};

  void Execuate() {
    stock_->Sell();
  }

 private:
  Stock *stock_;
};

class Invoke {
 public:
  void TakeCommand(Command *command) {
    commands_.push_back(command);
  }

  void PlaceCommands() {
    for (auto *command : commands_) {
      command->Execuate();
    }
    commands_.clear();
  }

 private:
  std::list<Command *> commands_;
};

int main() {
  Stock s;
  Invoke invoke;

  invoke.TakeCommand(new BuyCommand(&s));
  invoke.TakeCommand(new SellCommand(&s));

  invoke.PlaceCommands();

  return 0;
}
```

### 解释器模式

- 意图：给定一个语言，并定义一组解释器，由解释器在不同的业务场景下，对语言进行解释

```c++
class Context {   // 包含解释器之外的全局信息
private:
    string input;
    string output;
public:
    void SetInput(string i) { input = i; }
    string GetInput() { return input; }
    void SetOutput(string o) { output = o; }
    string GetOutput() { return output; }
};

class AbstractExpression {
public:
    virtual void Interpret(Context* context) = 0;
    virtual ~AbstractExpression() {}
};

class TerminalExpression : public AbstractExpression {  // 终结符表达式
public:
    void Interpret(Context* context) {
        cout << "TerminalExpression: " << context->GetInput() << ", " << context->GetOutput() << endl;
    }
};

class NonterminalExpression : public AbstractExpression {   // 非终结符表达式
private:
    AbstractExpression* expression;
public:
    NonterminalExpression(AbstractExpression* e) { expression = e; }
    void Interpret(Context* context) {
        cout << "NonterminalExpression: " << context->GetInput() << ", " << context->GetOutput() << endl;
        expression->Interpret(context);
    }
};

int main() {
    Context* c = new Context();
    c->SetInput("Hello");
    c->SetOutput("World");

    AbstractExpression* exp1 = new TerminalExpression();
    AbstractExpression* exp2 = new NonterminalExpression(exp1);
    exp1->Interpret(c); // TerminalExpression: Hello, World
    exp2->Interpret(c); // NonterminalExpression: Hello, World
                        // TerminalExpression: Hello, World
    delete exp1;
    delete exp2;
}
```

### 迭代器模式

- 意图：提供一种方法顺序访问一个聚合对象中的各种元素，而无需暴露该对象的内部实现
- 何时使用：遍历一个集合对象

```c++
class Iterator { // 迭代器抽象类
public:
    virtual string First() = 0;
    virtual string Next() = 0;
    virtual bool IsDone() = 0;
    virtual string CurrentItem() = 0;
    virtual ~Iterator(){}
};

class Aggregate { // 聚集抽象类
public:
    virtual Iterator* CreateIterator() = 0;
    virtual void Push(string s) = 0;
    virtual string Pop(int index) = 0;
    virtual int Count() = 0;
};

class ConcreteIterator : public Iterator { // 具体迭代器类
private:
    Aggregate* aggregate;
    int index;
public:
    ConcreteIterator(Aggregate* a) {
        index = 0;
        aggregate = a;
    }
    string First() { return aggregate->Pop(0); }
    string Next() { 
        string str;
        index++;
        if (index < aggregate->Count()) str = aggregate->Pop(index);
        return str;
    }

    bool IsDone() { return (index >= aggregate->Count()); }
    string CurrentItem() { return aggregate->Pop(index); }
};

class ConcreteAggregate : public Aggregate {
private:
    vector<string> items;
    Iterator* iterator;
public:
    ConcreteAggregate() {
        iterator = NULL;
        items.clear();
    }
    ~ConcreteAggregate() {
        if (iterator){
            delete iterator;
            iterator = NULL;
        }
    }
    Iterator* CreateIterator() {
        if (iterator == NULL) iterator = new ConcreteIterator(this);
        return iterator;
    }
    int Count() { return items.size(); }
    void Push(string s) { items.push_back(s); }
    string Pop(int index) {
        string str;
        if (index < Count()) str = items[index];
        return str;
    }
};

int main() {
    ConcreteAggregate* ca = new ConcreteAggregate();
    ca->Push("Hello");
    ca->Push("World");

    Iterator* it = new ConcreteIterator(ca);
    while(!it->IsDone()) {
        cout << it->CurrentItem() << endl; 
        it->Next();
    }

    delete it;
    delete ca;
    return 0;
}
```

### 中介者模式

- 意图：用一个中介对象封装一系列的对象交互，中介对象使各个对象不再需要显示的相互引用，从而使其可以耦合松散，并且可以独立的改变他们之间的交互
- 主要解决：对象之间存在大量的关联关系
- 何时使用：多个类相互耦合，形成网状结构

```c++
class Colleague;
class Mediator {
 public:
  virtual ~Mediator() {};
  virtual void Send(std::string msg, Colleague *colleague) = 0;
};

class Colleague {
 public:
  Colleague(Mediator *mediator) : mediator_(mediator) {};
  virtual ~Colleague() {};

 protected:
  Mediator *mediator_;
};

class ColleagueA : public Colleague {
 public:
  ColleagueA(Mediator *mediator) : Colleague(mediator) {};
  ~ColleagueA() override {};

  void Send(std::string msg) {
    mediator_->Send(msg, this);
  }

  void Notify(std::string msg) {
    std::cout << "colleague A recived: " << msg << std::endl;
  }
};

class ColleagueB : public Colleague {
 public:
  ColleagueB(Mediator *mediator) : Colleague(mediator) {};
  ~ColleagueB() override {};

  void Send(std::string msg) {
    mediator_->Send(msg, this);
  }

  void Notify(std::string msg) {
    std::cout << "colleague B recived: " << msg << std::endl;
  }
};

class ColleagueMediator : public Mediator {
 public:
  void Set(ColleagueA *colleague_a) {
    colleague_a_ = colleague_a;
  }
  void Set(ColleagueB *colleague_b) {
    colleague_b_ = colleague_b;
  }

  void Send(std::string msg, Colleague *colleague) override {
    if (colleague == colleague_a_) {
      colleague_b_->Notify(msg);
    } else {
      colleague_a_->Notify(msg);
    }
  }
 private:
  ColleagueA *colleague_a_;
  ColleagueB *colleague_b_;
};

int main() {
  ColleagueMediator *cm = new ColleagueMediator;
  ColleagueA *ca = new ColleagueA(cm);
  ColleagueB *cb = new ColleagueB(cm);

  cm->Set(ca);
  cm->Set(cb);

  ca->Send("hello");
  cb->Send("world");

  return 0;
}
```

### 备忘录模式

- 意图：在不破坏封装的前提下保存一个对象的状态，并在该对象之外保存该状态
- 主要解决：在不破坏封装的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态，这样可以在以后将对象恢复到原先保存的状态

```c++
// 对象状态
class Memento
{
public:
    int m_score;                 
    int m_grade;             
    string m_equipment ; 
public:
    Memento(int score,int grade,string equipment):m_score(score),m_grade(grade),m_equipment(equipment) {}
    
    Memento& operator=(const Memento &memento) {
        m_score = memento.m_score;
        m_grade = memento.m_grade;
        m_equipment = memento.m_equipment;
        return *this;
    }
};

// 需要保存、加载状态的类
class GamePlayer
{
public:
    GamePlayer():m_score(0),m_grade(0),m_equipment("无装备") {}
    void load(Memento memento) {
        m_score = memento.m_score;
        m_grade = memento.m_grade;
        m_equipment = memento.m_equipment;
    }
    Memento  save() {
        Memento memento(m_score,m_grade,m_equipment);
        return memento;
    }
    void show() {
        cout << "score: " <<m_score <<endl;
        cout << "grade: " <<m_grade <<endl;
        cout << "equipment :" <<m_equipment <<endl;
     }
private:
    int m_score;                 
    int m_grade;                 
    string m_equipment ; 
};

// 类外部集中保存状态
class Archive
{
public:
    Archive() {}
    void addItem(Memento memento)
    {
        m_mementArray.push_back(memento);
    }
    Memento Load(int state) 
    {
        return m_mementArray[state];
    }
private:
    std::vector<Memento> m_mementArray;
};
```

### 观察者模式

- 意图：定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变的时候，所有依赖它的对象都会得到通知
- 主要解决：1个对象状态改变给其他对象通知的问题

```c++
class Observer {
 public:
  Observer(int num) : num_(num) {};
  virtual ~Observer() {};
  virtual void Update() = 0;

  int GetNum() const {
    return num_;
  }
 private:
  int num_;
};

class Subject {
 public:
  void Attach(Observer *observer) { observers_.push_back(observer); }
  void Detach(Observer *observer) {
    observers_.erase(std::remove_if(observers_.begin(),
                                    observers_.end(),
                                    [&](const Observer *o) { return observer->GetNum() == o->GetNum(); }),
                     observers_.end());
  }
  void Notify() {
    for (auto *o : observers_) {
      o->Update();
    }
  }

  void SetStatus(int status) { status_ = status; }
  int GetStatus() { return status_; }

 private:
  std::list<Observer *> observers_;
  int status_;
};

class InjObserver : public Observer {
 public:
  InjObserver(int num, Subject *subject) : Observer(num), subject_(subject) {}
  ~InjObserver() override {};

  void Update() override {
    status_ = subject_->GetStatus();
    std::cout << "status: " << status_ << std::endl;
  }

 private:
  Subject *subject_;
  int status_;
};

int main() {
  Subject* subject = new Subject;
  Observer* o1 = new InjObserver(1, subject);
  Observer* o2 = new InjObserver(1, subject);
  Observer* o3 = new InjObserver(1, subject);

  subject->Attach(o1);
  subject->Attach(o2);
  subject->Attach(o3);

  subject->SetStatus(2);
  subject->Notify();

  return 0;
}
```

### 状态模式

- 意图：允许对象在内部状态发生改变时改变它的行为，对象看起来好像修改了它的类
- 主要解决：对象的行为依赖于它的状态（属性），并且可以根据它的状态改变而改变它的相关行为。
- 何时使用：将各种具体的状态类抽象出来

```c++
class Context;
class Status {
 public:
  virtual ~Status() {};
  virtual void Handle(Context *context) = 0;
};

class StatusA : public Status {
 public:
  void Handle(Context *context) override;
};

class StatusB : public Status {
 public:
  void Handle(Context *context) override;
};

class StatusC : public Status {
 public:
  void Handle(Context *context) override;
};

class Context {
 public:
  Context(Status *status) : status_(status) {};
  void Request() {
    if (status_ == nullptr) {
      return;
    }
    status_->Handle(this);
  };

  void SetStatus(Status *status) { status_ = status; };
 private:
  Status *status_;
};

void StatusA::Handle(Context *context) {
  std::cout << "handle status A" << std::endl;
  context->SetStatus(new StatusB);
}

void StatusB::Handle(Context *context) {
  std::cout << "handle status B" << std::endl;
  context->SetStatus(new StatusC);
}

void StatusC::Handle(Context *context) {
  std::cout << "handle status C" << std::endl;
  context->SetStatus(new StatusA);
}

int main() {
  Context* context = new Context(new StatusA);
  context->Request();
  context->Request();
  context->Request();

  return 0;
}
```

### 空对象模式

```c++

class Book
{
public:
	virtual bool IsNull() = 0;  //添加判断Book对象是否为空对象
	virtual void show() = 0;   //展示图书信息
};

class ConcreteBook :public  Book   //具体书类
{
public:
	ConcreteBook(int id, string _name, string  _author)
		:ID(id)
		, name(_name)
		, author(_author)
	{}
	void show()  
	{
		cout << ID << "**" << name << "&&" << author << endl;
	}
	bool IsNull()
	{
		return false;
	}
private:
	int ID;
	string name;
	string  author;
};
 
class NullBook : public Book
{
public :
	bool IsNull()
	{
		return true;
	}
	void show()
	{
		cout <<"Sorry ， 未找到ID对应的图书 ， 请再次输入"<< endl;
	}
};
 
class BookFactory//图书 工厂
{
public :
	Book* getBook(int id)
	{
		Book* book;
		switch (id)
		{
		case 1:
			book = new ConcreteBook(id, "设计模式", "GOF");
			break;
		case 2:
			book = new ConcreteBook(id, "空对象模式", "Null Object Pattern");
			break;
		default:
			book = new NullBook();  //创建一个NullBook 对象
			break;
		}
		return book;
	}
	
};
```

### 策略模式

- 意图：定义一系列的算法,把它们一个个封装起来, 并且使它们可相互替换
- 何时使用：一个系统有许多许多类，而区分它们的只是他们直接的行为
- 如何解决：将这些算法封装成一个一个的类，任意地替换

```c++
class Strategy { // 抽象算法类
public:
    virtual void AlgorithmInterface() = 0; // 算法方法
    virtual ~Strategy(){}
};

class ConcreteStrategyA : public Strategy { // 具体算法A
public:
    void AlgorithmInterface() {
        cout << "ConcreteStrategyA" << endl;
    }
};

class ConcreteStrategyB : public Strategy { // 具体算法B
public:
    void AlgorithmInterface() {
        cout << "ConcreteStrategyB" << endl;
    }
};

class Context { // 上下文
private:
    Strategy* strategy;
public:
    Context(Strategy* s) { strategy = s; }
    void ContextInterface() { // 上下文接口
        strategy->AlgorithmInterface();
    }
};

int main() {
    Strategy* s = new ConcreteStrategyA();
    Context* c = new Context(s);
    c->ContextInterface();  // ConcreteStrategyA
    delete s;
    delete c;
    return 0;
}
```

### 模板模式

- 意图：定义一个操作中的算法的骨架，而将一些步骤延迟到子类中。模板方法使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。
- 主要解决：一些方法通用，却在每一个子类都重新写了这一方法
- 如何解决：抽象通用方法

```c++
class AbstractClass {
// 抽象类，其实就是一个抽象模板，定义并实现了一个模板方法。
// 这个模板方法一般是一个具体方法，给出了一个顶级逻辑的骨架，而逻辑的组成步骤在相应的抽象操作中，推迟到了子类实现。
// 顶级逻辑也可能调用一些具体的方法。
public:
    void TemplateMethod() { // 模板方法，给出了逻辑骨架，而逻辑的组成是一些相应的抽象操作，推迟到子类实现.
        PrimitiveOperation1();
        PrimitiveOperation2();
    }
    virtual void PrimitiveOperation1() = 0;
    virtual void PrimitiveOperation2() = 0; // 一些抽象行为，放到子类实现
    virtual ~AbstractClass() {}
};

class ConcreteClassA : public AbstractClass {
// ConcreteClass实现父类所定义的一个或多个抽象方法。
// 每个AbstractClass都可以有任意多个ConcreteClass与之对应，
// 而每一个ConcreteClass都可以给出这些抽象方法的不同实现，从而使得顶级逻辑的实现各不相同。
public:
    void PrimitiveOperation1() {
        cout << "ConcreteClassA: PrimitiveOperation1 & ";
    }
    void PrimitiveOperation2() {
        cout << "PrimitiveOperation2" << endl;
    }
};

class ConcreteClassB : public AbstractClass {
public:
    void PrimitiveOperation1() {
        cout << "ConcreteClassB: PrimitiveOperation1 & ";
    }
    void PrimitiveOperation2() {
        cout << "PrimitiveOperation2" << endl;
    }
};

int main() {
    AbstractClass* aa = new ConcreteClassA();
    aa->TemplateMethod(); // ConcreteClassA: PrimitiveOperation1 & PrimitiveOperation2

    AbstractClass* ab = new ConcreteClassB();
    ab->TemplateMethod(); // ConcreteClassB: PrimitiveOperation1 & PrimitiveOperation2

    delete aa;
    delete ab;
    return 0;
}
```

### 访问者模式

- 意图：将数据操作与数据结构分离
- 主要解决：稳定的数据结构和易变的操作耦合问题
- 何时使用：需要对一个对象结构中的数据进行很多不同且不相关的操作

```c++
class Element;
class ConcreteElementA;
class ConcreteElementB;

class Visitor {
public:
    virtual void VisitConcreteElementA(ConcreteElementA* ca) = 0;
    virtual void VisitConcreteElementB(ConcreteElementB* cb) = 0;
};

class ConcreteVisitorA : public Visitor{
public:
    void VisitConcreteElementA(ConcreteElementA* ca) {
        cout << "ConcreteVisitorA::VisitConcreteElementA" << endl;
    }
    void VisitConcreteElementB(ConcreteElementB* cb) {
        cout << "ConcreteVisitorA::VisitConcreteElementB" << endl;
    }
};

class ConcreteVisitorB : public Visitor{
public:
    void VisitConcreteElementA(ConcreteElementA* ca) {
        cout << "ConcreteVisitorB::VisitConcreteElementA" << endl;
    }
    void VisitConcreteElementB(ConcreteElementB* cb) {
        cout << "ConcreteVisitorB::VisitConcreteElementB" << endl;
    }
};

class ConcreteVisitorC : public Visitor{
public:
    void VisitConcreteElementA(ConcreteElementA* ca) {
        cout << "ConcreteVisitorC::VisitConcreteElementA" << endl;
    }
    void VisitConcreteElementB(ConcreteElementB* cb) {
        cout << "ConcreteVisitorC::VisitConcreteElementB" << endl;
    }
};

class Element {
public:
    virtual void Accept(Visitor* visitor) = 0;
};

class ConcreteElementA : public Element {
public:
    void Accept(Visitor* visitor) {
        cout << "ConcreteElementA: ";
        visitor->VisitConcreteElementA(this);
    }
    void OperationA() {
        cout << "OperationA" << endl;
    }
};

class ConcreteElementB : public Element {
public:
    void Accept(Visitor* visitor) {
        cout << "ConcreteElementB: ";
        visitor->VisitConcreteElementB(this);
    }
    void OperationB() {
        cout << "OperationB" << endl;
    }
};

class ObjectStructure {
private:
    list<Element* > elements;
public:
    void Attach(Element* element) {
        elements.push_back(element);
    }
    void Detach(Element* element) {
        elements.remove(element);
    }
    void Accept(Visitor* visitor) {
        for (auto e = elements.begin(); e != elements.end(); e++) {
            (*e)->Accept(visitor);
        }
    }
};

int main() {
    ObjectStructure* o = new ObjectStructure();

    ConcreteElementA* ca = new ConcreteElementA();
    ConcreteElementB* cb = new ConcreteElementB();

    o->Attach(ca);
    o->Attach(cb);

    ConcreteVisitorA* cv1 = new ConcreteVisitorA();
    ConcreteVisitorB* cv2 = new ConcreteVisitorB();
    ConcreteVisitorC* cv3 = new ConcreteVisitorC();

    o->Accept(cv1);
    // ConcreteElementA: ConcreteVisitorA::VisitConcreteElementA
    // ConcreteElementB: ConcreteVisitorA::VisitConcreteElementB
    o->Accept(cv2);
    // ConcreteElementA: ConcreteVisitorB::VisitConcreteElementA
    // ConcreteElementB: ConcreteVisitorB::VisitConcreteElementB
    o->Accept(cv3);
    // ConcreteElementA: ConcreteVisitorC::VisitConcreteElementA
    // ConcreteElementB: ConcreteVisitorC::VisitConcreteElementB
    delete ca;
    delete cb;
    delete cv1;
    delete cv2;
    delete cv3;
    delete o;

    return 0;
}
```