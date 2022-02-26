#include <iostream>
#include <vector>
#include <memory>
#include "test.h"
#include <initializer_list>

using namespace std;

extern int test();

class Base {
 public:
  virtual ~Base() {
    std::cout << "~a" << std::endl;
  }
  virtual void print() {
    std::cout << "a" << std::endl;
  }
 protected:
  int num;
};

class Derived final : public Base {
 public:
  virtual ~Derived() {
    std::cout << "~b" << std::endl;
  }
  void print() override {
    std::cout << "b" << std::endl;
  }
};

class DD;
class Based {
 public:
  virtual ~Based();
  virtual void ff() {
    std::cout << "ff" << std::endl;
  };
 private:
  DD* dd;
};

class DD {};

int test(std::initializer_list<int> args) {
  std::cout << args.size() << std::endl;
  for (auto iter=args.begin(); iter!=args.end(); ++iter) {
    std::cout << *iter << std::endl;
  }
  return 0;
}

int cal(const std::vector<int>& nums, int k) {
  int res = 0;
  int copy = k;

  for (int i=0; i<nums.size();) {
    if (copy - nums[i] >= 0) {
      copy -= nums[i];
      ++i;
    } else {
      ++res;
      copy = k;
    }
  }

  if (copy < k) {
    ++res;
  }

  return res;
}

class Test {
 public:
  Test() noexcept {
    throw "except...";
  }
};

int main() {
  Test t;

  return 0;
}