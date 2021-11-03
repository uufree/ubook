#include <iostream>
#include <vector>
#include <map>
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
    if (root == nullptr) {
      return;
    }

    flatten(root->left);
    flatten(root->right);

    TreeNode* left = root->left;
    TreeNode* right = root->right;

    root->left = nullptr;
    root->right = left;

    TreeNode* temp = root;
    while (temp->right) {
      temp = temp->right;
    }
    temp->right = right;
  }
};

class Node {
 public:
  int val;
  Node* left;
  Node* right;
  Node* next;

  Node() : val(0), left(NULL), right(NULL), next(NULL) {}
  Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}
  Node(int _val, Node* _left, Node* _right, Node* _next)
      : val(_val), left(_left), right(_right), next(_next) {}
};
class Solution116 {
 public:
  void connect(Node* left, Node* right) {
    if (left == nullptr || right == nullptr) {
      return;
    }

    left->next = right;

    connect(left->left, left->right);
    connect(right->left, right->right);
    connect(left->right, right->left);
  }

  Node *connect(Node *root) {
    if (root == nullptr) {
      return nullptr;
    }

    connect(root->left, root->right);
    return root;
  }
};

class Solution {
 public:
  int calculate(TreeNode* root) {
    if (root == nullptr) {
      return 0;
    }

    int leftSum = std::max(calculate(root->left), 0);
    int rightSum = std::max(calculate(root->right), 0);
    int sum = root->val + leftSum + rightSum;
    if (sum > max) {
      max = sum;
    }

    return root->val+std::max(leftSum, rightSum);
  }

  int64_t max = INT64_MIN;
  int maxPathSum(TreeNode* root) {
    if (root == nullptr) {
      return 0;
    }
    calculate(root);
    return max;
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