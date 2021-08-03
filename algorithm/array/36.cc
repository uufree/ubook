#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

class Solution {
 public:
  bool isValidRowAndCol(const std::vector<char>& nums) {
    bool is_valid = true;
    std::bitset<9> bs;

    for (auto& c : nums) {
      if (c == '.') {
        continue;
      }

      if (bs[c - int('a')] != 0) {
        is_valid = false;
        break;
      } else {
        bs[c - int('a')] = 1;
      }
    }

    return is_valid;
  }

  std::vector<std::vector<char>> split(const vector<vector<char>>& board) {
    std::vector<std::vector<char>> result;
    for (int row=0; row<9; row+=3) {
      for (int col=0; col<9; col+=3) {
        std::vector<char> vec;
        for (int i=row; i<row+3; i++) {
          for (int j=col;j<col+3;j++) {
            vec.push_back(board[i][j]);
          }
        }
        result.push_back(vec);
      }
    }

    return result;
  }

  bool isValidSudoku(vector<vector<char>>& board) {
    // row
    for (const auto& row : board) {
      if (!isValidRowAndCol(row)) {
        return false;
      }
    }

    // col
    for (int col = 0; col < 9; col++) {
      std::vector<char> col_nums;
      for (const auto& row : board) {
        col_nums.push_back(row[col]);
      }

      if (!isValidRowAndCol(col_nums)) {
        return false;
      }
    }

    // rect
    auto result = split(board);
    for (const auto& res : result) {
      if (!isValidRowAndCol(res)) {
        return false;
      }
    }

    return true;
  }
};

int main() {
  Solution sol;
  std::vector<std::vector<char>> input{
    {'.','.','.','.','5','.','.','1','.'},
    {'.','4','.','3','.','.','.','.','.'},
    {'.','.','.','.','.','3','.','.','1'},
    {'8','.','.','.','.','.','.','2','.'},
    {'.','.','2','.','7','.','.','.','.'},
    {'.','1','5','.','.','.','.','.','.'},
    {'.','.','.','.','.','2','.','.','.'},
    {'.','2','.','9','.','.','.','.','.'},
    {'.','.','4','.','.','.','.','.','.'}
  };

  std::cout << sol.isValidSudoku(input) << std::endl;

  return 0;
}