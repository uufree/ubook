#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Solution {
 public:
  void nextPermutation(vector<int>& nums) {
    if (std::is_sorted(nums.begin(), nums.end(), std::greater<int>())) {
      std::reverse(nums.begin(), nums.end());
      return;
    }

    int right = nums.size()-1;
    while (right > 0) {
      if (nums[right-1] < nums[right]) {
        int start = right;
        int big = nums[right];
        int small = nums[right-1];
        int key = big;
        int key_point = right;
        while (start < nums.size()) {
          if (nums[start] > small && nums[start] < big && nums[start] < key) {
            key = nums[start];
            key_point = start;
          }
          ++start;
        }
        std::swap(nums[right-1], nums[key_point]);

        if (key_point == right) {
          break;
        } else {
          if (nums[key_point] < nums[right]) {
            std::swap(nums[key_point], nums[right]);
          }
        }
      }
      --right;
    }
  }
};

int main() {
  Solution sol;
  std::vector<int> input({2,3,1});
  sol.nextPermutation(input);
  std::ostream_iterator<int> oi(std::cout, ",");
  std::copy(input.begin(), input.end(), oi);
  std::cout << std::endl;
  return 0;
}