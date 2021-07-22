/*************************************************************************
	> File Name: 11.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年07月20日 星期二 23时28分16秒
 ************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

class Solution {
 public:
  int maxArea(vector<int>& height) {
    int left = 0;
    int right = height.size() - 1;
    int max = (right - left) * std::min(height[left], height[right]);

    while (left < right) {
      if (height[left] < height[right]) {
        ++left;
      } else {
        --right;
      }

      if (left == right) {
        break;
      }

      max = std::max(max, (right - left) * std::min(height[left], height[right]));
    }

    return max;
  }
};

int main() {
  Solution sol;
  std::vector<int> height({1,2,1});
  std::cout << sol.maxArea(height) << std::endl;
  return 0;
}
