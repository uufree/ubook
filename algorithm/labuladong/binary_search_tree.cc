#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 在二叉搜索树中寻找第k大元素
class Solution230 {
  int pos = 0;
  int value = 0;
 public:
  int kthSmallest(TreeNode* root, int k) {
    if (root == nullptr) {
      return 0;
    }

    kthSmallest(root->left, k);
    if (++pos == k) {
      value = root->val;
    }
    kthSmallest(root->right, k);
    return value;
  }
};

// 二叉树变累加树
class Solution538 {
  int sum = 0;
 public:
  TreeNode* convertBST(TreeNode* root) {
    if (root == nullptr) {
      return nullptr;
    }

    convertBST(root->right);
    sum += root->val;
    root->val = sum;
    convertBST(root->left);
    return root;
  }
};

// 验证二叉搜索树是否有效
// Tips：当前value值要大于左子树max，小于右子树min
class Solution98 {
 int max = INT32_MIN;
 public:
  bool isValidBST(TreeNode* root) {
    if (root == nullptr) {
      return true;
    }
    bool isValidLeft = isValidBST(root->left);
    bool isValidMid = false;
    if (root->val >= max) {
      isValidMid = true;
      max = root->val;
    }
    bool isValidRight = isValidBST(root->right);
    return isValidLeft && isValidMid && isValidRight;
  }
};

// 寻找二叉搜索树中的某个节点
class Solution700 {
 public:
  TreeNode* searchBST(TreeNode* root, int val) {
    if (root == nullptr) {
      return nullptr;
    }

    TreeNode* node = root;
    while (node && node->val != val) {
      if (node->val == val) {
        break;
      } else if (node->val > val) {
        node = node->left;
      } else if (node->val < val) {
        node = node->right;
      }
    }
    return node;
  }
};

// 删除二叉搜索树中的某个节点
// 1. 如果没有子节点，直接删除
// 2. 如果仅有一个子节点，直接替换
// 3. 如果有两个子节点，寻找左子树最大 or 右子树最小的节点
class Solution450 {
 public:
  TreeNode* getRightMin(TreeNode* root) {
    while (root->left) {
      root = root->left;
    }
    return root;
  }

  TreeNode* deleteNode(TreeNode* root, int key) {
    if (root == nullptr) {
      return nullptr;
    }

    if (root->val == key) {
      if (!root->left && !root->right) {
        return nullptr;
      } else if (root->left && !root->right) {
        return root->left;
      } else if (root->right && !root->left) {
        return root->right;
      } else if (root->left && root->right) {
        TreeNode* min = getRightMin(root->right);
        root->val = min->val;
        root->right = deleteNode(root->right, min->val);
      }

    } else if (root->val > key) {
      root->left = deleteNode(root->left, key);
    } else if (root->val < key) {
      root->right = deleteNode(root->right, key);
    }

    return root;
  }
};

// 在二叉搜索树中插入指定节点
class Solution701 {
 public:
  TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (root == nullptr) {
      return new TreeNode(val);
    }
    if (root->val > val) {
      root->left = insertIntoBST(root->left, val);
    } else if (root->val < val) {
      root->right = insertIntoBST(root->right, val);
    }

    return root;
  }
};

int main() {
  std::cout << INT32_MIN << std::endl;
  return 0;
}