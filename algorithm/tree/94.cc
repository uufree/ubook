/*************************************************************************
	> File Name: 94.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年09月07日 星期二 07时17分42秒
 ************************************************************************/

#include <iostream>
#include <vector>

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
 public:
  void inOrder(TreeNode* node, std::vector<int>& vec) {
    if (node == nullptr) {
      return;
    }

    inOrder(node->left, vec);
    vec.push_back(node->val);
    inOrder(node->right, vec);
  }

  std::vector<int> inorderTraversal(TreeNode *root) {
    std::vector<int> result;
    inOrder(root, result);
    return result;
  }
};

TreeNode* construct() {
  TreeNode* n1 = new TreeNode(1);
  TreeNode* n2 = new TreeNode(2);
  TreeNode* n3 = new TreeNode(3);

  n1->right = n2;
  n2->left = n3;

  return n1;
}

int main() {
  TreeNode* node = construct();
  Solution sol;
  std::vector<int> res = sol.inorderTraversal(node);
  for (auto& r : res) {
    std::cout << r << std::endl;
  }

  return 0;
}