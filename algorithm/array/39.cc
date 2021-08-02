/*************************************************************************
	> File Name: 39.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年07月27日 星期二 06时54分56秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
 public:
  vector<vector<int>> combinationSum(vector<int>& candidates, int target) {

  }
};

int main() {
  Solution sol;
  std::vector<int> input{};
  int target = 0;
  auto output = sol.combinationSum(input, target);
  for (auto& out : output) {
    for (auto o : out) {
      std::cout << o << "    ";
    }
    std::cout << std::endl;
  }
}


