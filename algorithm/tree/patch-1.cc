#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution111 {
 public:
  int minDepth(TreeNode* root) {
    if (root == nullptr) {
      return 0;
    }

    if (root->left == nullptr && root->right == nullptr) {
      return 1;
    }

    int leftMin = INT32_MAX;
    int rightMin = INT32_MAX;
    if (root->left) {
      leftMin = minDepth(root->left);
    }

    if (root->right) {
      rightMin = minDepth(root->right);
    }

    return std::min(leftMin, rightMin)+1;
  }
};

class Solution112 {
 public:
  TreeNode* construct() {
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);

    n1->left = n2;
    n1->right = n3;
    return n1;
  }

  void traverse(TreeNode* root, int sum, int target) {
    if (root->left == nullptr && root->right == nullptr && sum == target) {
      equal = true;
      return;
    }

    if (root->left) {
      traverse(root->left, sum+root->left->val, target);
    }

    if (root->right) {
      traverse(root->right, sum+root->right->val, target);
    }
  }

  bool equal = false;
  bool hasPathSum(TreeNode* root, int targetSum) {
    if (root == nullptr) {
      return false;
    }

    traverse(root, root->val, targetSum);
    return equal;
  }
};

class Solution113 {
 public:
  TreeNode* construct() {
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);

    n1->left = n2;
    n1->right = n3;

    return n1;
  }

  void traverse(TreeNode* root, int sum, int targetSum, vector<int>& cur, vector<vector<int>>& res) {
    if (root->left== nullptr && root->right== nullptr && sum == targetSum) {
      res.push_back(cur);
      return;
    }

    if (root->left){
      cur.push_back(root->left->val);
      traverse(root->left, sum+root->left->val, targetSum, cur, res);
      cur.pop_back();
    }

    if (root->right) {
      cur.push_back(root->right->val);
      traverse(root->right, sum+root->right->val, targetSum, cur, res);
      cur.pop_back();
    }
  }
  vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
      return result;
    }

    vector<int> cur{root->val};
    traverse(root, root->val, targetSum, cur, result);
    return result;
  }
};

class Solution114 {
 public:
  void flatten(TreeNode* root) {

  }
};

int main() {
  Solution113 sol;
  TreeNode* root = sol.construct();
  auto result = sol.pathSum(root, 3);
  std::cout << "len: " << result.size() << std::endl;
  for (auto& res : result) {
    for (auto& r : res) {
      std::cout << r << "    ";
    }
    std::cout << std::endl;
  }
  return 0;
}