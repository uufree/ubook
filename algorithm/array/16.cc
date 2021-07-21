#include <iostream>
#include <vector>
#include <algorithm>
#include <inttypes.h>
#include <numeric>

using namespace std;

class Solution {
 public:
  int sum(int left, int mid , int right) {
    return left + mid + right;
  }

  int threeSumClosest(vector<int>& nums, int target) {
    if (nums.size() < 0) {
      return 0;
    }

    int output;
    int min_diff = INT32_MAX;
    bool neg = true;
    std::sort(nums.begin(), nums.end());

    for (int mid = 1; mid < nums.size()-1; mid++) {
      int left = 0, right = nums.size()-1;
      while(left < mid && right > mid) {
        int sum = nums[left] + nums[mid] + nums[right];
        int diff = target - sum;

        if (diff < min_diff) {
          min_diff = diff;
          neg = sum > target ? false : true;
          ++left;
          continue;
        } else {
          if (diff > 0) {
            --right;
          } else if (diff < 0) {
            ++left;
          }
        }
      }
    }

    return neg ? target + min_diff : target - min_diff;
  }
};

int main() {
  Solution sol;
  std::vector<int> input{-1,2,1,-4};
  int target = 1;

  std::cout << sol.threeSumClosest(input, target) << std::endl;
  return 0;
}