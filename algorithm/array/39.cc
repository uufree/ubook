#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

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
  std::istream_iterator<int> oi(std::cout, ",");
  for (auto& out : output) {
    std::copy(out.begin(), out.end(), oi);
    std::cout << std::endl;
  }

  return 0;
}
