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
  vector<vector<int>> cache;
 public:
  int dp(const string& s1, int i, const string& s2, int j) {
    if (i == s1.size()) {
      return 0;
    }

    if (j == s2.size()) {
      return 0;
    }

    if (cache[i][j]) {
      return cache[i][j];
    }

    if (s1[i] == s2[j]) {
      cache[i][j] = 1 + dp(s1, i+1, s2, j+1);
    } else {
      cache[i][j] = std::max(dp(s1, i+1, s2, j), dp(s1, i, s2, j+1));
    }

    return cache[i][j];
  }

  int longestPalindromeSubseq(string s) {
    if (s.empty()) {
      return 0;
    }

    if (s.size() == 1) {
      return 1;
    }

    cache.resize(s.size());
    for (auto& nums : cache) {
      nums.resize(s.size());
    }

    string rs = s;
    std::reverse(rs.begin(), rs.end());


    return dp(s, 0, rs, 0);
  }
};

int main() {
  Solution sol;
  std::cout << sol.longestPalindromeSubseq("bbbab") << std::endl;
  return 0;
}