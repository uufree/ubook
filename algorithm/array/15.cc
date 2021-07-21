#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

class Solution {
 public:
  enum ResultType {
    THREE_ZERO,
    NEG_ZERO_POS,
    TWO_NEG_POS,
    TWO_POS_NEG,
  };

  ResultType checkResultType(int left, int mid, int right) {
    ResultType type;
    if (left == mid && mid == right) {
      type = THREE_ZERO;
    } else if(mid == 0) {
      type = NEG_ZERO_POS;
    } else if (mid < 0) {
      type = TWO_NEG_POS;
    } else if (mid > 0) {
      type = TWO_POS_NEG;
    }

    return type;
  }

  vector<vector<int>> threeSum(vector<int>& nums) {
    std::vector<std::vector<int>> result;
    if (nums.size() < 3) {
      return result;
    }

    std::set<int> zero, neg_zero_pos;
    std::map<int, std::set<int>> two_neg_pos, two_pos_neg;
    std::sort(nums.begin(), nums.end());
    for (int mid = 1; mid < nums.size()-1; mid++) {
      int left = 0, right = nums.size()-1;
      while (left < right && right > mid && left < mid) {
        int sum = nums[left] + nums[mid] + nums[right];
        if (sum > 0) {
          --right;
        } else if (sum < 0) {
          ++left;
        } else {
          bool insert = false;
          switch (checkResultType(nums[left], nums[mid], nums[right])) {
            case THREE_ZERO:
              if (zero.empty()) {
                zero.insert(0);
                insert = true;
              }
              break;
            case NEG_ZERO_POS:
              if (neg_zero_pos.find(nums[left]) == neg_zero_pos.end()) {
                neg_zero_pos.insert(nums[left]);
                insert = true;
              }
              break;
            case TWO_NEG_POS:
              if (two_neg_pos.find(nums[left]) == two_neg_pos.end()) {
                two_neg_pos[nums[left]].insert(nums[mid]);
                insert = true;
              } else {
                auto s = two_neg_pos[nums[left]];
                if (s.find(nums[mid]) == s.end()) {
                  two_neg_pos[nums[left]].insert(nums[mid]);
                  insert = true;
                }
              }
              break;
            case TWO_POS_NEG:
              if (two_pos_neg.find(nums[mid]) == two_pos_neg.end()) {
                two_pos_neg[nums[mid]].insert(nums[right]);
                insert = true;
              } else {
                auto s = two_pos_neg[nums[mid]];
                if (s.find(nums[right]) == s.end()) {
                  two_pos_neg[nums[mid]].insert(nums[right]);
                  insert = true;
                }
              }
              break;
          }

          if (insert) {
            result.push_back({nums[left], nums[mid], nums[right]});
          }

          if ((right-mid) > (mid - left)) {
            --right;
          } else {
            ++left;
          }
        }
      }
    }

    return result;
  }
};

int main() {
  Solution sol;
  std::vector<int> input{-1,0,1,2,-1,-4,-2,-3,3,0,4};
  // [[-4,0,4],[-4,1,3],[-3,-1,4],[-3,0,3],[-3,1,2],[-2,-1,3],[-2,0,2],[-1,-1,2],[-1,0,1]]
  auto result = sol.threeSum(input);
  for (auto& res : result) {
    for (auto& r : res) {
      std::cout << r << "    ";
    }
    std::cout << std::endl;
  }

  return 0;
}