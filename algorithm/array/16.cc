#include <iostream>
#include <vector>
#include <algorithm>
#include <inttypes.h>
#include <numeric>

using namespace std;

class Solution {
 public:
  int threeSumClosest(vector<int> &nums, int target) {
    if (nums.size() < 0) {
      return 0;
    }

    int output;
    int min_diff = INT32_MAX;
    std::sort(nums.begin(), nums.end());

    for (int mid = 1; mid < nums.size() - 1; mid++) {
      int left = 0, right = nums.size() - 1;
      while (left < mid && right > mid) {
        int sum = nums[left] + nums[mid] + nums[right];
        int diff = sum > target ? sum - target : target - sum;

        if (diff < min_diff) {
          min_diff = diff;
          output = sum;
        }

        if (sum > target) {
          --right;
        } else {
          ++left;
        }


      }
    }

    return output;
  }
};

int main() {
  Solution sol;
  std::vector<int> input{-1,2,1,-4};
  int target = 2;

  std::cout << sol.threeSumClosest(input, target) << std::endl;
  return 0;
}