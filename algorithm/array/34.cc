/*************************************************************************
	> File Name: 34.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年07月22日 星期四 23时16分44秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
 public:
  vector<int> searchRange(vector<int>& nums, int target) {
    std::vector<int> output;
    auto start = std::find_if(nums.begin(), nums.end(), [&](const int& lhs) {return lhs == target;});
    if (start == nums.end()) {
      output.push_back(-1);
      output.push_back(-1);
    } else {
      int start_distance = std::distance(nums.begin(), start);
      int end_distance = start_distance;
      for (auto iter = start+1; iter!=nums.end(); ++iter) {
        if (*iter == target) {
          ++end_distance;
        }
      }

      output.push_back(start_distance);
      output.push_back(end_distance);
    }

    return output;
  }
};

int main() {
  Solution sol;
  std::vector<int> input{};
  int target = 0;

  auto output = sol.searchRange(input, target);
  for (auto& out : output) {
    std::cout << out << std::endl;
  }
  return 0;
}
