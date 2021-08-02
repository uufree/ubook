/*************************************************************************
	> File Name: 33.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年07月22日 星期四 23时10分38秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
 public:
  int search(vector<int>& nums, int target) {
    auto iter = std::find_if(nums.begin(), nums.end(), [&](const int lhs) {return lhs == target;});
    if (iter != nums.end()) {
      return std::distance(nums.begin(), iter);
    } else {
      return -1;
    }
  }
};

int main() {
  Solution sol;
  std::vector<int> input{1};
  int target = 0;
  std::cout << sol.search(input, target) << std::endl;
  return 0;
}
