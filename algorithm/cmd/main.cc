#include <iostream>
#include <vector>
#include <memory>
#include "test.h"
#include <unistd.h>
#include <initializer_list>
#include <string>

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

class Solution {
  int min = INT32_MAX;
  int res = 0;
  bool flag = false;
 public:
  void find(const vector<int>& nums, int key, int left, int right, int target) {
    while (left < right) {
      int sum = key + nums[left] + nums[right];
      if (sum == target) {
        flag = true;
        res = sum;
        return;
      } else if (sum < target) {
        ++left;
        if (target-sum < min) {
          min = target-sum;
          res = sum;
        }
      } else {
        --right;
        if (sum-target < min) {
          min = sum - target;
          res = sum;
        }
      }
    }
  }

  int threeSumClosest(vector<int>& nums, int target) {
    if (nums.size() < 3) {
      return 0;
    }

    std::sort(nums.begin(), nums.end());
    for (int i=0; i<nums.size(); i++) {
      if (flag) {
        break;
      }
      find(nums, nums[i], i+1, nums.size()-1, target);
    }

    return res;
  }
};

int main() {
  vector<int> input{1,1,-1,-1,3};
  Solution sol;
  std::cout << sol.threeSumClosest(input, -1) << std::endl;
  return 0;
}