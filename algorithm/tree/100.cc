/*************************************************************************
	> File Name: 100.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2021年09月07日 星期二 07时25分40秒
 ************************************************************************/

#include <iostream>

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
  bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr) {
      return true;
    } else if (p == nullptr || q == nullptr) {
      return false;
    } else if (p->val != q->val) {
      return false;
    } else {
      return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
  }
};