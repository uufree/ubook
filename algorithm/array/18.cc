#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdio>

using namespace std;

class Solution {
 public:
  std::string construct(const std::vector<int>& lhs) {
    return std::string(std::to_string(lhs[0]) + "_"
    + std::to_string(lhs[1]) + "_"
    + std::to_string(lhs[2]) + "_"
    + std::to_string(lhs[3]));
  }

  std::vector<int> split(const std::string& s) {
    long one, two, three, four;
    sscanf(s.c_str(), "%ld_%ld_%ld_%ld", &one, &two, &three, &four);
    return std::vector<int>({int(one), int(two), int(three), int(four)});
  }


  // no de-dup
  // nums was be sorted
  vector<vector<int>> threeSum(std::vector<int>& nums, int targer) {
    std::vector<std::vector<int>> output;
    for (int mid=1; mid<nums.size()-1; mid++) {
      int left = 0, right = nums.size()-1;
      while (left < mid && right > mid) {
        int sum = nums[left] + nums[mid] + nums[right];
        if (sum > targer) {
          --right;
        } else if (sum < targer) {
          ++left;
        } else {
          output.push_back({nums[left], nums[mid], nums[right]});
          ++left;
        }
      }
    }

    return output;
  }

  vector<vector<int>> fourSum(vector<int>& nums, int target) {
    std::vector<std::vector<int>> result;
    if (nums.size() < 4) {
      return result;
    }

    std::sort(nums.begin(), nums.end());
    if (nums.front() > 0 && nums.front() > target) {
      return result;
    }

    std::vector<int> input(nums.begin(), nums.end());
    input.erase(std::unique(input.begin(), input.end()), input.end());

    for (int i=0; i<input.size(); i++) {
      std::vector<int> copy_nums(nums.begin(), nums.end());
      //copy_nums.erase(std::remove_if(copy_nums.begin(), copy_nums.end(), [&](int key) {return key==input[i];}),
      //                copy_nums.end());
      copy_nums.erase(std::find_if(copy_nums.begin(), copy_nums.end(), [&](const int& key) {return key == input[i];}));
      if (copy_nums.size() >= 3) {
        int new_target = target - input[i];
        auto output = threeSum(copy_nums, new_target);
        if (!output.empty()) {
          for (auto out : output) {
            out.push_back(input[i]);
            std::sort(out.begin(), out.end());
            result.push_back(out);
          }
        }
      }
    }

    // duplication
    std::vector<std::string> string_result;
    std::vector<std::vector<int>> final_result;
    for (auto& res : result){
      string_result.push_back(construct(res));
    }
    std::sort(string_result.begin(), string_result.end());
    string_result.erase(std::unique(string_result.begin(), string_result.end()), string_result.end());
    for (auto& str : string_result) {
      final_result.push_back(split(str));
    }

    return final_result;
  }
};

int main() {
  Solution sol;
  std::vector<int> input{1,0,-1,0,-2,2};
  int targer = 0;

  auto output = sol.fourSum(input, targer);
  for (auto& out : output) {
    for (auto& o : out) {
      std::cout << o << "    ";
    }
    std::cout << std::endl;
  }
  return 0;
}