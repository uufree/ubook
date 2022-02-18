#include <iostream>
#include "test.h"

extern int test();

class a {
 public:
  virtual ~a() {
    std::cout << "~a" << std::endl;
  }
  virtual void print() {
    std::cout << "a" << std::endl;
  }
 protected:
  int num;
};

class b : public a {
 public:
  virtual ~b() {
    std::cout << "~b" << std::endl;
  }
  void print() override {
    std::cout << "b" << std::endl;
  }
};

class q {
 public:
  virtual ~q() {} ;
  virtual void print() = 0;
};

class w : public q {
 public:
  ~w() {};
  void print() override{
    std::cout << "w" << std::endl;
  }
};

int main() {
  int a = 10;
  float b = dynamic_cast;
  std::cout << sizeof(q) << std::endl;
  std::cout << sizeof(w) << std::endl;
  return 0;
}