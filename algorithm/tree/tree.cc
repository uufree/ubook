#include <iostream>
#include <vector>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution94 {
 public:
  void traverse(TreeNode* node, vector<int>& inOrder) {
    if (node == nullptr) {
      return;
    }

    traverse(node->left, inOrder);
    inOrder.push_back(node->val);
    traverse(node->right, inOrder);
  }

  vector<int> inorderTraversal(TreeNode* root) {
    vector<int> result;
    if (root == nullptr) {
      return result;
    }

    traverse(root, result);
    return result;
  }
};

class Solution95 {
 public:
  vector<TreeNode*> build(int left, int right) {
    std::vector<TreeNode*> result;

    if (left > right) {
      result.push_back(nullptr);
      return result;
    }

    for (int i=left; i<=right; i++) {
      auto lefts = build(left, i-1);
      auto rights = build(i+1, right);
      for (auto left : lefts) {
        for (auto right : rights) {
          TreeNode* root = new TreeNode(i);
          root->left = left;
          root->right = right;
          result.push_back(root);
        }
      }
    }

    return result;
  }

  vector<TreeNode*> generateTrees(int n) {
    if (n == 0) {
      return std::vector<TreeNode*>{nullptr};
    }

    if (n == 1) {
      return std::vector<TreeNode*>{new TreeNode(1)};
    }

    return build(1, n);
  }
};

class Solution96 {
  std::vector<std::vector<int>> cache;
 public:
  int count(int left, int right) {
    int result = 0;

    if (left > right) {
      return 1;
    }

    if (cache[left][right] > 0) {
      return cache[left][right];
    }

    for (int i=left; i<=right; i++) {
      int leftCount = count(left, i-1);
      int rightCount = count(i+1, right);
      result += leftCount * rightCount;
    }

    cache[left][right] = result;

    return result;
  }

  int numTrees(int n) {
    if (n == 0) {
      return 0;
    }

    if (n == 1) {
      return 1;
    }

    cache.resize(n+1);
    for (int i=0; i<=n; i++) {
      cache[i].resize(n+1);
    }

    return count(1, n);
  }
};

class Solution98 {
  int64_t max = INT64_MIN;
 public:
  bool isValidBST(TreeNode* root) {
    if (root == nullptr) {
      return true;
    }

    bool leftValid = isValidBST(root->left);
    bool midValid = false;
    if (root->val > max) {
      max = root->val;
      midValid = true;
    }
    bool rightValid = isValidBST(root->right);
    return leftValid && midValid && rightValid;
  }
};

class Solution99 {
 public:
  TreeNode* construct() {
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(3);
    TreeNode* n3 = new TreeNode(2);

    n1->left = n2;
    n2->right = n3;
    return n1;
  }

  TreeNode* construct1() {
    TreeNode* n1 = new TreeNode(3);
    TreeNode* n2 = new TreeNode(1);
    TreeNode* n3 = new TreeNode(4);
    TreeNode* n4 = new TreeNode(2);

    n1->left = n2;
    n1->right = n3;
    n3->left = n4;
    return n1;
  }

  void print(TreeNode* root) {
    if (root == nullptr) {
      return;
    }

    print(root->left);
    std::cout << root->val << std::endl;
    print(root->right);
  }

  TreeNode* getLeftMax(TreeNode* node) {
    if (node == nullptr) {
      return nullptr;
    }
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  TreeNode* getRightMin(TreeNode* node) {
    if (node == nullptr) {
      return nullptr;
    }

    while (node->left) {
      node = node->left;
    }
    return node;
  }

  void traverse(TreeNode* root, std::vector<TreeNode*>& nodes) {
    if (root == nullptr) {
      return;
    }

    traverse(root->left, nodes);
    nodes.push_back(root);
    traverse(root->right, nodes);
  }

  void recoverTree(TreeNode* root) {
    if (root == nullptr) {
      return;
    }

    std::vector<TreeNode*> nodes;
    traverse(root, nodes);

    // find
    TreeNode* n1 = nullptr;
    TreeNode *n2 = nullptr;
    for (int i=1;i<nodes.size(); i++) {
      if (nodes[i]->val < nodes[i-1]->val) {
        n1 = nodes[i-1];
        break;
      }
    }

    for (int i=0; i<nodes.size(); i++) {
      if (nodes[i]->val > n1->val) {
        n2 = nodes[i];
        break;
      }
    }

    if (n2 == nullptr) {
      n2 = nodes.back();
    }

    for (auto node : nodes) {
      std::cout << "out: " << node->val << std::endl;
    }

    std::cout << "in: " << n1->val << std::endl;
    std::cout << "in: " << n2->val << std::endl;

    // recover
    swap(n1->val, n2->val);
  }
};

int main() {
  Solution99 sol;
  auto root = sol.construct1();
  sol.recoverTree(root);
  sol.print(root);
  return 0;
}